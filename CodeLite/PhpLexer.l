%{
//
//====================--------------------------------------------------------------
// Eran Ifrah 2014 (c)
//====================--------------------------------------------------------------
//
// To generate a source file from this .l file, you will need
// a flex version 2.5.34 and later
// Under Windows, you will need to run the following command 
// from within *MSYS* terminal (or run codelite from an MSYS shell):
// /usr/bin/flex --noline --batch --outfile=CppLexer.cpp cpp_lexer.l
//
//====================--------------------------------------------------------------
//
extern "C" int yywrap(void*) { return 1; }

#include <wx/string.h>
#include <wx/filename.h>
#include "PhpLexerAPI.h"
#include "PHPScannerTokens.h"

#define YYSTYPE wxString
#define ECHO

#define YY_NO_UNISTD_H

#define RETURN_WHITESPACE()                                         \
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r; \
    if(userData->IsCollectingWhitespace()) {                        \
        return kPHP_T_WHITESPACE;                                        \
    }
#define RETURN_NEWLINE()                                            \
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r; \
    if(userData->IsCollectingWhitespace()) {                        \
        return kPHP_T_NEWLINE;                                           \
    }

#define LEX_RETURN(x) {\
    return x;\
}

%}

/* regex and modes */

/* options */
%option yylineno
%option default
%option reentrant

%x PHP
%x CPP_COMMENT
%x DOC_COMMENT 
%x HEREDOC
%x DSTRING
%x SINGLE_STRING

identifier [a-zA-Z_][0-9a-zA-Z_]*

exponent_part [eE][-+]?[0-9]+
fractional_constant ([0-9]*"."[0-9]+)|([0-9]+".")
floating_constant (({fractional_constant}{exponent_part}?)|([0-9]+{exponent_part}))[FfLl]?

integer_suffix_opt ([uU]?[lL]?)|([lL][uU])
decimal_constant [1-9][0-9]*{integer_suffix_opt}
octal_constant "0"[0-7]*{integer_suffix_opt}
hex_constant "0"[xX][0-9a-fA-F]+{integer_suffix_opt}

simple_escape [abfnrtv'"?\\]
octal_escape  [0-7]{1,3}
hex_escape "x"[0-9a-fA-F]+

escape_sequence [\\]({simple_escape}|{octal_escape}|{hex_escape})
c_char [^'\\\n]|{escape_sequence}
s_char [^"\\\n]|{escape_sequence}
WHITESPACE [ \t]+
TABS_AND_SPACES [ \t]*
LABEL [a-zA-Z_\x7f-\xff][a-zA-Z0-9_\x7f-\xff]*
NEWLINE ("\n"|"\r\n")
h_tab [\011]
form_feed [\014]
v_tab [\013]
c_return [\015]

horizontal_white [ ]|{h_tab}

%% 
<INITIAL>"<script"{WHITESPACE}+"language"{WHITESPACE}*"="{WHITESPACE}*("php"|"\"php\""|"'php'"){WHITESPACE}*">" {
    BEGIN(PHP);
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->SetInsidePhp(true);
    return kPHP_T_OPEN_TAG;
}
<INITIAL>"<?=" {
    BEGIN(PHP);
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->SetInsidePhp(true);
    return kPHP_T_OPEN_TAG_WITH_ECHO;
}
<INITIAL>"<?php" {
    BEGIN(PHP);
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->SetInsidePhp(true);
    return kPHP_T_OPEN_TAG;
}
<INITIAL>"<?" {
    BEGIN(PHP);
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->SetInsidePhp(true);
    return kPHP_T_OPEN_TAG;
}
<INITIAL>\n {
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    if(userData->IsCollectingAllNonPhp()) {
        return yytext[0];
    }
}
<INITIAL>. {
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    if(userData->IsCollectingAllNonPhp()) {
        return yytext[0];
    }
}

<PHP>{horizontal_white}+ { RETURN_WHITESPACE(); }
<PHP>({horizontal_white}|{v_tab}|{c_return}|{form_feed})*"\n" { RETURN_NEWLINE();}
<PHP>({v_tab}|{c_return}|{form_feed})+ {RETURN_WHITESPACE();}
<PHP>"$"{LABEL}             {LEX_RETURN(kPHP_T_VARIABLE);}
<PHP>b?"<<<"{TABS_AND_SPACES}({LABEL}|([']{LABEL}['])|(["]{LABEL}["])){NEWLINE} {
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    wxString label = yytext;
    label.Remove(0, 3).Trim(); // remove the "<<<" from the begining of the string
    if(label.StartsWith("'")) label.Remove(0, 1);
    if(label.EndsWith("'")) label.RemoveLast();
    
    userData->SetRawStringLabel(label);
    userData->GetString() << yytext;
    BEGIN(HEREDOC);
    return kPHP_T_START_HEREDOC;
}
<HEREDOC>{LABEL} {
    // anything else
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->GetString() << yytext;
    if(yytext == userData->GetRawStringLabel()) {
        // end of HEREDOC
        BEGIN(PHP);
        return kPHP_T_END_HEREDOC;
    }
}
<HEREDOC>\n {
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->GetString() << yytext;
}
<HEREDOC>. {
    // anything else
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->GetString() << yytext;
}
<PHP>"/*" {
    // Clear the comment collected
    BEGIN(DOC_COMMENT);
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    if(userData->IsCollectingComments()) {
        userData->ClearComment();
        userData->AppendToComment("/*");
        userData->SetCommentStartLine(yylineno);
    }
}
<DOC_COMMENT>"*/" {
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    if(userData->IsCollectingComments()) {
        userData->AppendToComment("*/");
        userData->SetCommentEndLine(yylineno);
    }
    BEGIN(PHP);
    if(userData->IsCollectingComments()) {
        return kPHP_T_C_COMMENT;
    }
}
<DOC_COMMENT>"\n" { 
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    if(userData->IsCollectingComments()) {
        userData->AppendToComment("\n");
    }
}
<DOC_COMMENT>. {
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    if(userData->IsCollectingComments()) {
        userData->AppendToComment(yytext[0]);
    }
}

<PHP>"//"|"#" { 
    BEGIN(CPP_COMMENT); 
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    if(userData->IsCollectingComments()) {
        userData->ClearComment();
        userData->AppendToComment(yytext);
        userData->SetCommentStartLine(yylineno);
    }
}
<CPP_COMMENT>"\n" {
    BEGIN(PHP);
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    if(userData->IsCollectingWhitespace()) {
        userData->AppendToComment("\n");
    }
    if(userData->IsCollectingComments()) {
        return kPHP_T_CXX_COMMENT;
    }
}
<CPP_COMMENT>. { 
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    if(userData->IsCollectingComments()) {
        userData->AppendToComment(yytext[0]);
    }
}

<PHP>{decimal_constant}     {LEX_RETURN(kPHP_T_DNUMBER);}
<PHP>{octal_constant}       {LEX_RETURN(kPHP_T_LNUMBER);}
<PHP>{hex_constant}         {LEX_RETURN(kPHP_T_LNUMBER);}
<PHP>{floating_constant}    {LEX_RETURN(kPHP_T_DNUMBER);}
<PHP>"define"               {LEX_RETURN(kPHP_T_DEFINE);}
<PHP>"exit"                 {LEX_RETURN(kPHP_T_EXIT);}
<PHP>"die"                  {LEX_RETURN(kPHP_T_EXIT);}
<PHP>"function"             {LEX_RETURN(kPHP_T_FUNCTION);}
<PHP>"const"                {LEX_RETURN(kPHP_T_CONST);}
<PHP>"return"               {LEX_RETURN(kPHP_T_RETURN);}
<PHP>"try"                  {LEX_RETURN(kPHP_T_TRY);}
<PHP>"catch"                {LEX_RETURN(kPHP_T_CATCH);}
<PHP>"throw"                {LEX_RETURN(kPHP_T_THROW);}
<PHP>"if"                   {LEX_RETURN(kPHP_T_IF);}
<PHP>"elseif"               {LEX_RETURN(kPHP_T_ELSEIF);}
<PHP>"endif"                {LEX_RETURN(kPHP_T_ENDIF);}
<PHP>"else"                 {LEX_RETURN(kPHP_T_ELSE);}
<PHP>"while"                {LEX_RETURN(kPHP_T_WHILE);}
<PHP>"endwhile"             {LEX_RETURN(kPHP_T_ENDWHILE);}
<PHP>"do"                   {LEX_RETURN(kPHP_T_DO);}
<PHP>"for"                  {LEX_RETURN(kPHP_T_FOR);}
<PHP>"endfor"               {LEX_RETURN(kPHP_T_ENDFOR);}
<PHP>"foreach"              {LEX_RETURN(kPHP_T_FOREACH);}
<PHP>"endforeach"           {LEX_RETURN(kPHP_T_ENDFOREACH);}
<PHP>"declare"              {LEX_RETURN(kPHP_T_DECLARE);}
<PHP>"enddeclare"           {LEX_RETURN(kPHP_T_ENDDECLARE);}
<PHP>"instanceof"           {LEX_RETURN(kPHP_T_INSTANCEOF);}
<PHP>"as"                   {LEX_RETURN(kPHP_T_AS);}
<PHP>"switch"               {LEX_RETURN(kPHP_T_SWITCH);}
<PHP>"endswitch"            {LEX_RETURN(kPHP_T_ENDSWITCH);}
<PHP>"case"                 {LEX_RETURN(kPHP_T_CASE);}
<PHP>"default"              {LEX_RETURN(kPHP_T_DEFAULT);}
<PHP>"break"                {LEX_RETURN(kPHP_T_BREAK);}
<PHP>"continue"             {LEX_RETURN(kPHP_T_CONTINUE);}
<PHP>"goto"                 {LEX_RETURN(kPHP_T_GOTO);}
<PHP>"echo"                 {LEX_RETURN(kPHP_T_ECHO);}
<PHP>"print"                {LEX_RETURN(kPHP_T_PRINT);}
<PHP>"class"                {LEX_RETURN(kPHP_T_CLASS);}
<PHP>"interface"            {LEX_RETURN(kPHP_T_INTERFACE);}
<PHP>"trait"                {LEX_RETURN(kPHP_T_TRAIT);}
<PHP>"extends"              {LEX_RETURN(kPHP_T_EXTENDS);}
<PHP>"implements"           {LEX_RETURN(kPHP_T_IMPLEMENTS);}
<PHP>"->"                   {LEX_RETURN(kPHP_T_OBJECT_OPERATOR);}
<PHP>"::"                   {LEX_RETURN(kPHP_T_PAAMAYIM_NEKUDOTAYIM);}
<PHP>"\\"                   {LEX_RETURN(kPHP_T_NS_SEPARATOR);}
<PHP>"new"                  {LEX_RETURN(kPHP_T_NEW);}
<PHP>"clone"                {LEX_RETURN(kPHP_T_CLONE);}
<PHP>"var"                  {LEX_RETURN(kPHP_T_VAR);}
<PHP>"eval"                 {LEX_RETURN(kPHP_T_EVAL);}
<PHP>"include"              {LEX_RETURN(kPHP_T_INCLUDE);}
<PHP>"include_once"         {LEX_RETURN(kPHP_T_INCLUDE_ONCE);}
<PHP>"require"              {LEX_RETURN(kPHP_T_REQUIRE);}
<PHP>"require_once"         {LEX_RETURN(kPHP_T_REQUIRE_ONCE);}
<PHP>"namespace"            {LEX_RETURN(kPHP_T_NAMESPACE);}
<PHP>"use"                  {LEX_RETURN(kPHP_T_USE);}
<PHP>"insteadof"            {LEX_RETURN(kPHP_T_INSTEADOF);}
<PHP>"global"               {LEX_RETURN(kPHP_T_GLOBAL);}
<PHP>"isset"                {LEX_RETURN(kPHP_T_ISSET);}
<PHP>"empty"                {LEX_RETURN(kPHP_T_EMPTY);}
<PHP>"static"               {LEX_RETURN(kPHP_T_STATIC);}
<PHP>"abstract"             {LEX_RETURN(kPHP_T_ABSTRACT);}
<PHP>"__halt_compiler"      {LEX_RETURN(kPHP_T_ABSTRACT);}
<PHP>"final"                {LEX_RETURN(kPHP_T_FINAL);}
<PHP>"private"              {LEX_RETURN(kPHP_T_PRIVATE);}
<PHP>"protected"            {LEX_RETURN(kPHP_T_PROTECTED);}
<PHP>"public"               {LEX_RETURN(kPHP_T_PUBLIC);}
<PHP>"unset"                {LEX_RETURN(kPHP_T_UNSET);}
<PHP>"=>"                   {LEX_RETURN(kPHP_T_DOUBLE_ARROW);}
<PHP>"list"                 {LEX_RETURN(kPHP_T_LIST);}
<PHP>"array"                {LEX_RETURN(kPHP_T_ARRAY);}
<PHP>"callable"             {LEX_RETURN(kPHP_T_CALLABLE);}
<PHP>"++"                   {LEX_RETURN(kPHP_T_INC);}
<PHP>"--"                   {LEX_RETURN(kPHP_T_DEC);}
<PHP>"==="                  {LEX_RETURN(kPHP_T_IS_IDENTICAL);}
<PHP>"!=="                  {LEX_RETURN(kPHP_T_IS_NOT_IDENTICAL);}
<PHP>"=="                   {LEX_RETURN(kPHP_T_IS_EQUAL);}
<PHP>"!="|"<>"              {LEX_RETURN(kPHP_T_IS_NOT_EQUAL);}
<PHP>"<="                   {LEX_RETURN(kPHP_T_IS_SMALLER_OR_EQUAL);}
<PHP>">="                   {LEX_RETURN(kPHP_T_IS_GREATER_OR_EQUAL);}
<PHP>"+="                   {LEX_RETURN(kPHP_T_PLUS_EQUAL);}
<PHP>"-="                   {LEX_RETURN(kPHP_T_MINUS_EQUAL);}
<PHP>"*="                   {LEX_RETURN(kPHP_T_MUL_EQUAL);}
<PHP>"/="                   {LEX_RETURN(kPHP_T_DIV_EQUAL);}
<PHP>".="                   {LEX_RETURN(kPHP_T_CONCAT_EQUAL);}
<PHP>"%="                   {LEX_RETURN(kPHP_T_MOD_EQUAL);}
<PHP>"<<="                  {LEX_RETURN(kPHP_T_SL_EQUAL);}
<PHP>">>="                  {LEX_RETURN(kPHP_T_SR_EQUAL);}
<PHP>"&="                   {LEX_RETURN(kPHP_T_AND_EQUAL);}
<PHP>"|="                   {LEX_RETURN(kPHP_T_OR_EQUAL);}
<PHP>"^="                   {LEX_RETURN(kPHP_T_XOR_EQUAL);}
<PHP>"||"                   {LEX_RETURN(kPHP_T_BOOLEAN_OR);}
<PHP>"&&"                   {LEX_RETURN(kPHP_T_BOOLEAN_AND);}
<PHP>"OR"                   {LEX_RETURN(kPHP_T_LOGICAL_OR);}
<PHP>"AND"                  {LEX_RETURN(kPHP_T_LOGICAL_AND);}
<PHP>"XOR"                  {LEX_RETURN(kPHP_T_LOGICAL_XOR);}
<PHP>"<<"                   {LEX_RETURN(kPHP_T_SL);}
<PHP>">>"                   {LEX_RETURN(kPHP_T_SR);}
<PHP>"__CLASS__"            {LEX_RETURN(kPHP_T_CLASS_C);}
<PHP>"__TRAIT__"            {LEX_RETURN(kPHP_T_TRAIT_C);}
<PHP>"__FUNCTION__"         {LEX_RETURN(kPHP_T_FUNC_C);}
<PHP>"__METHOD__"           {LEX_RETURN(kPHP_T_METHOD_C);}
<PHP>"__LINE__"             {LEX_RETURN(kPHP_T_LINE);}
<PHP>"__FILE__"             {LEX_RETURN(kPHP_T_FILE);}
<PHP>"__DIR__"              {LEX_RETURN(kPHP_T_DIR);}
<PHP>"__NAMESPACE__"        {LEX_RETURN(kPHP_T_NS_C);}
<PHP>"yield"                {LEX_RETURN(kPHP_T_YIELD);}
<PHP>"("{TABS_AND_SPACES}("int"|"integer"){TABS_AND_SPACES}")" {
    LEX_RETURN(kPHP_T_INT_CAST);
}
<PHP>"("{TABS_AND_SPACES}("real"|"double"|"float"){TABS_AND_SPACES}")" {
    LEX_RETURN(kPHP_T_DOUBLE_CAST);
}
<PHP>"("{TABS_AND_SPACES}("string"|"binary"){TABS_AND_SPACES}")" {
    LEX_RETURN(kPHP_T_STRING_CAST);
}
<PHP>"("{TABS_AND_SPACES}"array"{TABS_AND_SPACES}")" {
    LEX_RETURN(kPHP_T_ARRAY_CAST);
}
<PHP>"("{TABS_AND_SPACES}"object"{TABS_AND_SPACES}")" {
    LEX_RETURN(kPHP_T_OBJECT_CAST);
}
<PHP>"("{TABS_AND_SPACES}("bool"|"boolean"){TABS_AND_SPACES}")" {
    return kPHP_T_BOOL_CAST;
}
<PHP>"("{TABS_AND_SPACES}("unset"){TABS_AND_SPACES}")" {
    return kPHP_T_UNSET_CAST;
}
<PHP>{LABEL} {LEX_RETURN(kPHP_T_IDENTIFIER);}
<PHP>("?>"|"</script"{WHITESPACE}*">")"\n"? {
    BEGIN(INITIAL);
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->SetInsidePhp(false);
    return kPHP_T_CLOSE_TAG;
}
<PHP>"%>"{NEWLINE}? {
    BEGIN(INITIAL);
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->SetInsidePhp(false);
    return kPHP_T_CLOSE_TAG;
}
<PHP>"'" {
    BEGIN(SINGLE_STRING); 
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->SetString("'");
}
<PHP>"\"" {
    BEGIN(DSTRING);
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->SetString("\"");
}
<PHP><<EOF>> { 
    yyterminate(); 
    return 0;
}
<PHP>. {
    LEX_RETURN(yytext[0]);
}
<SINGLE_STRING>"\n" {
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->GetString().Append("\n");
}
<SINGLE_STRING>{c_char}  {
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->GetString().Append(yytext);
}
<SINGLE_STRING>"'"               {
    BEGIN(PHP);
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->GetString().Append("'");
    return kPHP_T_CONSTANT_ENCAPSED_STRING;
}
<SINGLE_STRING>. {
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->GetString().Append(yytext[0]);
}
<DSTRING>"\n" {
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->GetString().Append("\n");
}
<DSTRING>{s_char}  {
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->GetString().Append(yytext);
}

<DSTRING>"\"" {
    BEGIN(PHP);
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->GetString().Append("\"");
    return kPHP_T_CONSTANT_ENCAPSED_STRING;
}
<DSTRING>. {
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    userData->GetString().Append(yytext[0]);
}
%%

//=============-------------------------------
// API methods implementation
//=============-------------------------------

bool phpLexerIsPHPCode(void* scanner)
{
    struct yyguts_t * yyg = (struct yyguts_t*)scanner;
    phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
    return userData->IsInsidePhp();
}

void* phpLexerNew(const wxString& filename, size_t options )
{
    wxFileName fn = filename;
    if(fn.IsRelative()) {
        fn.MakeAbsolute();
    }
    
    FILE* fp = ::fopen(fn.GetFullPath().mb_str(wxConvUTF8).data(), "rb");
    if(!fp) {
        return NULL;
    }
    yyscan_t scanner;
    phplex_init(&scanner);
    struct yyguts_t * yyg = (struct yyguts_t*)scanner;
    yyg->yyextra_r = new phpLexerUserData(options);
    php_switch_to_buffer(php_create_buffer(fp, YY_BUF_SIZE, scanner), scanner);
    return scanner;
}

void phpLexerDestroy(void** scanner)
{
    struct yyguts_t * yyg = (struct yyguts_t*)(*scanner);
    delete (phpLexerUserData*)yyg->yyextra_r;
    php_delete_buffer(YY_CURRENT_BUFFER, *scanner);

    phplex_destroy(*scanner);
    *scanner = NULL;
}

bool phpLexerNext(void* scanner, phpLexerToken& token)
{
    token.endLineNumber = -1;
    token.type = phplex(scanner);
    if(token.type != 0) {
        struct yyguts_t * yyg = (struct yyguts_t*)scanner;
        phpLexerUserData* userData = (phpLexerUserData*)yyg->yyextra_r;
        switch(token.type) {
        case kPHP_T_END_HEREDOC:
            token.lineNumber = phpget_lineno(scanner); 
            token.text = userData->GetString();
            userData->GetString().Clear();
            break;
        case kPHP_T_START_HEREDOC:
            token.lineNumber = phpget_lineno(scanner);
            token.text.Clear();
            break;
        case kPHP_T_CONSTANT_ENCAPSED_STRING:
            token.lineNumber = phpget_lineno(scanner);
            token.text = userData->GetString();
            userData->GetString().Clear();
            break;
        case kPHP_T_CXX_COMMENT:
            // One line up for CXX comments
            token.lineNumber = userData->GetCommentStartLine();
            token.text = userData->GetComment();
            userData->ClearComment();
            break;
        case kPHP_T_C_COMMENT:
            token.text = userData->GetComment();
            token.lineNumber = userData->GetCommentStartLine();
            token.endLineNumber = userData->GetCommentEndLine();
            userData->ClearComment();
            break;
        default:
            token.lineNumber = phpget_lineno(scanner);
            token.text = phpget_text(scanner);
            break;
        }

    } else {
        token.text.Clear();
        token.lineNumber = 0;
    }
    return token.type != 0;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2014 The CodeLite Team
// file name            : DiffSideBySidePanel.cpp
//
// -------------------------------------------------------------------------
// A
//              _____           _      _     _ _
//             /  __ \         | |    | |   (_) |
//             | /  \/ ___   __| | ___| |    _| |_ ___
//             | |    / _ \ / _  |/ _ \ |   | | __/ _ )
//             | \__/\ (_) | (_| |  __/ |___| | ||  __/
//              \____/\___/ \__,_|\___\_____/_|\__\___|
//
//                                                  F i l e
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include "DiffSideBySidePanel.h"
#include "clDTL.h"
#include "editor_config.h"
#include "lexer_configuration.h"
#include "drawingutils.h"
#include <wx/msgdlg.h>
#include "globals.h"
#include "plugin.h"
#include "event_notifier.h"
#include "art_metro.h"
#include "DiffConfig.h"
#include <wx/menu.h>

#define RED_MARKER 5
#define GREEN_MARKER 6
#define PLACE_HOLDER_MARKER 7

#define RED_MARKER_MASK (1 << RED_MARKER)
#define GREEN_MARKER_MASK (1 << GREEN_MARKER)
#define PLACE_HOLDER_MARKER_MASK (1 << PLACE_HOLDER_MARKER)

#define MARKER_SEQUENCE 8
#define MARKER_SEQUENCE_VERTICAL 9

DiffSideBySidePanel::DiffSideBySidePanel(wxWindow* parent)
    : DiffSideBySidePanelBase(parent)
    , m_flags(0)
{
    m_config.Load();

#ifdef __WXMSW__
    m_ribbonBar->SetArtProvider(new wxRibbonMetroArtProvider);
#endif
    EventNotifier::Get()->Connect(
        wxEVT_NOTIFY_PAGE_CLOSING, wxNotifyEventHandler(DiffSideBySidePanel::OnPageClosing), NULL, this);

    Connect(ID_COPY_LEFT_TO_RIGHT,
            wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(DiffSideBySidePanel::OnMenuCopyLeft2Right));
    Connect(ID_COPY_LEFT_TO_RIGHT_AND_MOVE,
            wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(DiffSideBySidePanel::OnMenuCopyLeft2Right));
    Connect(ID_COPY_RIGHT_TO_LEFT,
            wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(DiffSideBySidePanel::OnMenuCopyRight2Left));
    Connect(ID_COPY_RIGHT_TO_LEFT_AND_MOVE,
            wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(DiffSideBySidePanel::OnMenuCopyRight2Left));
    CallAfter(&DiffSideBySidePanel::DoLayout);
}

DiffSideBySidePanel::~DiffSideBySidePanel()
{
    if(m_flags & kDeleteLeftOnExit) {
        ::wxRemoveFile(m_filePickerLeft->GetPath());
    }
    if(m_flags & kDeleteRightOnExit) {
        ::wxRemoveFile(m_filePickerRight->GetPath());
    }

    // save the configuration
    m_config.Save();

    EventNotifier::Get()->Disconnect(
        wxEVT_NOTIFY_PAGE_CLOSING, wxNotifyEventHandler(DiffSideBySidePanel::OnPageClosing), NULL, this);
}

void DiffSideBySidePanel::Diff()
{
    wxFileName fnLeft(m_filePickerLeft->GetPath());
    wxFileName fnRIght(m_filePickerRight->GetPath());

    if(!fnLeft.Exists()) {
        ::wxMessageBox(wxString() << _("Left Side File:\n") << fnLeft.GetFullPath() << _(" does not exist!"),
                       "CodeLite",
                       wxICON_ERROR | wxCENTER | wxOK);
        return;
    }

    if(!fnRIght.Exists()) {
        ::wxMessageBox(wxString() << _("Right Side File:\n") << fnRIght.GetFullPath() << _(" does not exist!"),
                       "CodeLite",
                       wxICON_ERROR | wxCENTER | wxOK);
        return;
    }

    // Cleanup
    DoClean();

    // Prepare the views
    PrepareViews();

    // Prepare the diff
    clDTL d;
    d.Diff(m_filePickerLeft->GetPath(),
           m_filePickerRight->GetPath(),
           m_config.IsSingleViewMode() ? clDTL::kOnePane : clDTL::kTwoPanes);
    const clDTL::LineInfoVec_t& resultLeft = d.GetResultLeft();
    const clDTL::LineInfoVec_t& resultRight = d.GetResultRight();
    m_sequences = d.GetSequences();

    if(m_sequences.empty()) {
        // Files are the same !
        m_stcLeft->SetReadOnly(false);
        m_stcRight->SetReadOnly(false);

        m_stcLeft->LoadFile(fnLeft.GetFullPath());
        m_stcRight->LoadFile(fnRIght.GetFullPath());

        m_stcLeft->SetSavePoint();
        m_stcRight->SetSavePoint();

        m_stcLeft->SetReadOnly(true);
        m_stcRight->SetReadOnly(true);
        return;
    }

    m_cur_sequence = 0; // the first line of the sequence

    // Create 2 strings "left" and "right"
    wxString leftContent, rightContent;

    // The left pane is always the one with the deletions "-"
    for(size_t i = 0; i < resultLeft.size(); ++i) {
        if(resultLeft.at(i).m_type == clDTL::LINE_ADDED) {
            leftContent << resultLeft.at(i).m_line;
            m_leftGreenMarkers.push_back(i);

        } else if(resultLeft.at(i).m_type == clDTL::LINE_REMOVED) {
            leftContent << resultLeft.at(i).m_line;
            m_leftRedMarkers.push_back(i);

        } else if(resultLeft.at(i).m_type == clDTL::LINE_PLACEHOLDER) {
            // PLACEHOLDER
            leftContent << resultLeft.at(i).m_line;
            m_leftPlaceholdersMarkers.push_back(i);

        } else {
            // COMMON
            leftContent << resultLeft.at(i).m_line;
        }
    }

    // The right pane is always with the new additions "+"
    for(size_t i = 0; i < resultRight.size(); ++i) {
        if(resultRight.at(i).m_type == clDTL::LINE_REMOVED) {
            rightContent << resultRight.at(i).m_line;
            m_rightRedMarkers.push_back(i);

        } else if(resultRight.at(i).m_type == clDTL::LINE_ADDED) {
            rightContent << resultRight.at(i).m_line;
            m_rightGreenMarkers.push_back(i);

        } else if(resultRight.at(i).m_type == clDTL::LINE_PLACEHOLDER) {
            rightContent << resultRight.at(i).m_line;
            m_rightPlaceholdersMarkers.push_back(i);

        } else {
            // COMMON
            rightContent << resultRight.at(i).m_line;
        }
    }
    UpdateViews(leftContent, rightContent);
    m_stcLeft->SetSavePoint();
    m_stcRight->SetSavePoint();

    // Select the first diff
    wxRibbonButtonBarEvent dummy;
    m_cur_sequence = -1;
    OnNextDiffSequence(dummy);
}

void DiffSideBySidePanel::PrepareViews()
{
    // Prepare the views by selecting the proper syntax highlight
    wxFileName fnLeft(m_filePickerLeft->GetPath());
    wxFileName fnRight(m_filePickerRight->GetPath());

    bool useRightSideLexer = false;
    if(fnLeft.GetExt() == "svn-base") {
        // doing svn diff, use the lexer for the right side file
        useRightSideLexer = true;
    }

    LexerConf::Ptr_t leftLexer =
        EditorConfigST::Get()->GetLexerForFile(useRightSideLexer ? fnRight.GetFullName() : fnLeft.GetFullName());
    wxASSERT(leftLexer);

    LexerConf::Ptr_t rightLexer = EditorConfigST::Get()->GetLexerForFile(fnRight.GetFullName());
    wxASSERT(rightLexer);

    leftLexer->Apply(m_stcLeft, true);
    rightLexer->Apply(m_stcRight, true);

    // Create the markers we need
    DefineMarkers(m_stcLeft);
    DefineMarkers(m_stcRight);

    // Turn off PP highlighting
    m_stcLeft->SetProperty("lexer.cpp.track.preprocessor", "0");
    m_stcLeft->SetProperty("lexer.cpp.update.preprocessor", "0");

    m_stcRight->SetProperty("lexer.cpp.track.preprocessor", "0");
    m_stcRight->SetProperty("lexer.cpp.update.preprocessor", "0");
}

void DiffSideBySidePanel::DefineMarkers(wxStyledTextCtrl* ctrl)
{
    wxColour red, green, grey, sideMarker;
    if(DrawingUtils::IsThemeDark()) {
        red = "RED";
        green = "GREEN";
        grey = "dark grey";
        sideMarker = "CYAN";

    } else {
        red = "RED";
        green = "GREEN";
        grey = "LIGHT GREY";
        sideMarker = "BLUE";
    }

    ctrl->MarkerDefine(GREEN_MARKER, wxSTC_MARK_BACKGROUND);
    ctrl->MarkerSetBackground(GREEN_MARKER, green);
    ctrl->MarkerSetAlpha(GREEN_MARKER, 50);

    ctrl->MarkerDefine(RED_MARKER, wxSTC_MARK_BACKGROUND);
    ctrl->MarkerSetBackground(RED_MARKER, red);
    ctrl->MarkerSetAlpha(RED_MARKER, 50);

    ctrl->MarkerDefine(PLACE_HOLDER_MARKER, wxSTC_MARK_BACKGROUND);
    ctrl->MarkerSetBackground(PLACE_HOLDER_MARKER, grey);
    ctrl->MarkerSetAlpha(PLACE_HOLDER_MARKER, 50);

    ctrl->MarkerDefine(MARKER_SEQUENCE, wxSTC_MARK_FULLRECT);
    ctrl->MarkerSetBackground(MARKER_SEQUENCE, sideMarker);

    ctrl->MarkerDefine(MARKER_SEQUENCE_VERTICAL, wxSTC_MARK_VLINE);
    ctrl->MarkerSetBackground(MARKER_SEQUENCE_VERTICAL, sideMarker);
}

void DiffSideBySidePanel::UpdateViews(const wxString& left, const wxString& right)
{
    m_stcLeft->SetEditable(true);
    m_stcRight->SetEditable(true);

    m_stcLeft->SetText(left);
    m_stcLeft->MarkerDeleteAll(RED_MARKER);

    m_stcRight->SetText(right);
    m_stcRight->MarkerDeleteAll(GREEN_MARKER);

    // Show whitespaces
    m_stcRight->SetViewWhiteSpace(wxSTC_WS_VISIBLEALWAYS);
    m_stcLeft->SetViewWhiteSpace(wxSTC_WS_VISIBLEALWAYS);

    // apply the markers
    for(size_t i = 0; i < m_leftRedMarkers.size(); ++i) {
        int line = m_leftRedMarkers.at(i);
        m_stcLeft->MarkerAdd(line, RED_MARKER);
    }
    for(size_t i = 0; i < m_leftGreenMarkers.size(); ++i) {
        int line = m_leftGreenMarkers.at(i);
        m_stcLeft->MarkerAdd(line, GREEN_MARKER);
    }
    for(size_t i = 0; i < m_leftPlaceholdersMarkers.size(); ++i) {
        int line = m_leftPlaceholdersMarkers.at(i);
        m_stcLeft->MarkerAdd(line, PLACE_HOLDER_MARKER);
    }
    for(size_t i = 0; i < m_rightGreenMarkers.size(); ++i) {
        int line = m_rightGreenMarkers.at(i);
        m_stcRight->MarkerAdd(line, GREEN_MARKER);
    }
    for(size_t i = 0; i < m_rightRedMarkers.size(); ++i) {
        int line = m_rightRedMarkers.at(i);
        m_stcRight->MarkerAdd(line, RED_MARKER);
    }
    for(size_t i = 0; i < m_rightPlaceholdersMarkers.size(); ++i) {
        int line = m_rightPlaceholdersMarkers.at(i);
        m_stcRight->MarkerAdd(line, PLACE_HOLDER_MARKER);
    }

    // Restore the 'read-only' state
    m_stcLeft->SetEditable(false);
    m_stcRight->SetEditable(false);
}

void DiffSideBySidePanel::OnLeftStcPainted(wxStyledTextEvent& event)
{
    wxUnusedVar(event);
    int rightFirstLine = m_stcRight->GetFirstVisibleLine();
    int leftFirsLine = m_stcLeft->GetFirstVisibleLine();
    if(rightFirstLine != leftFirsLine) {
        m_stcRight->SetFirstVisibleLine(leftFirsLine);
    }

    int rightScrollPos = m_stcRight->GetXOffset();
    int leftScrollPos = m_stcLeft->GetXOffset();
    if(leftScrollPos != rightScrollPos) {
        m_stcRight->SetXOffset(leftScrollPos);
    }
}

void DiffSideBySidePanel::OnRightStcPainted(wxStyledTextEvent& event)
{
    wxUnusedVar(event);
    int rightFirstLine = m_stcRight->GetFirstVisibleLine();
    int leftFirsLine = m_stcLeft->GetFirstVisibleLine();
    if(rightFirstLine != leftFirsLine) {
        m_stcLeft->SetFirstVisibleLine(rightFirstLine);
    }

    int rightScrollPos = m_stcRight->GetXOffset();
    int leftScrollPos = m_stcLeft->GetXOffset();
    if(leftScrollPos != rightScrollPos) {
        m_stcLeft->SetXOffset(rightScrollPos);
    }
}

void DiffSideBySidePanel::SetFilesDetails(const DiffSideBySidePanel::FileInfo& leftFile,
                                          const DiffSideBySidePanel::FileInfo& rightFile)
{
    // left file
    m_filePickerLeft->SetPath(leftFile.filename.GetFullPath());
    m_staticTextLeft->SetLabel(leftFile.title);

    m_filePickerRight->SetPath(rightFile.filename.GetFullPath());
    m_staticTextRight->SetLabel(rightFile.title);

    m_flags = 0x0;
    if(leftFile.readOnly) m_flags |= kLeftReadOnly;
    if(leftFile.deleteOnExit) m_flags |= kDeleteLeftOnExit;
    if(rightFile.readOnly) m_flags |= kRightReadOnly;
    if(rightFile.deleteOnExit) m_flags |= kDeleteRightOnExit;
}

void DiffSideBySidePanel::OnNextDiffSequence(wxRibbonButtonBarEvent& event)
{
    m_cur_sequence++; // advance the sequence
    int firstLine = m_sequences.at(m_cur_sequence).first;
    int lastLine = m_sequences.at(m_cur_sequence).second;
    DoDrawSequenceMarkers(firstLine, lastLine, m_stcLeft);
    DoDrawSequenceMarkers(firstLine, lastLine, m_stcRight);
}

void DiffSideBySidePanel::OnPrevDiffSequence(wxRibbonButtonBarEvent& event)
{
    m_cur_sequence--;
    int firstLine = m_sequences.at(m_cur_sequence).first;
    int lastLine = m_sequences.at(m_cur_sequence).second;
    DoDrawSequenceMarkers(firstLine, lastLine, m_stcLeft);
    DoDrawSequenceMarkers(firstLine, lastLine, m_stcRight);
}

void DiffSideBySidePanel::OnRefreshDiff(wxRibbonButtonBarEvent& event)
{
    if(m_stcLeft->IsModified() || m_stcRight->IsModified()) {
        wxStandardID res = ::PromptForYesNoDialogWithCheckbox(
            _("Refreshing the view will lose all your changes\nDo you want to continue?"),
            "DiffRefreshViewDlg",
            _("Refresh"),
            _("Don't refresh"));
        if(res != wxID_YES) {
            return;
        }
    }
    Diff();
}

void DiffSideBySidePanel::DoClean()
{
    // Cleanup
    m_leftRedMarkers.clear();
    m_leftPlaceholdersMarkers.clear();
    m_leftGreenMarkers.clear();

    m_rightGreenMarkers.clear();
    m_rightRedMarkers.clear();
    m_rightPlaceholdersMarkers.clear();
    m_sequences.clear();

    m_stcLeft->SetReadOnly(false);
    m_stcRight->SetReadOnly(false);
    m_stcLeft->SetText("");
    m_stcRight->SetText("");
    m_stcLeft->SetSavePoint();
    m_stcRight->SetSavePoint();
    m_stcLeft->SetReadOnly(true);
    m_stcRight->SetReadOnly(true);
    m_cur_sequence = wxNOT_FOUND;
}

void DiffSideBySidePanel::DoDrawSequenceMarkers(int firstLine, int lastLine, wxStyledTextCtrl* ctrl)
{
    // delete old markers
    ctrl->MarkerDeleteAll(MARKER_SEQUENCE);
    ctrl->MarkerDeleteAll(MARKER_SEQUENCE_VERTICAL);

    int line1 = firstLine;
    int line2 = lastLine;

    for(int i = line1; i < line2; ++i) {
        ctrl->MarkerAdd(i, MARKER_SEQUENCE);
    }

    // Make sure that the seq lines are visible
    int visibleLine = firstLine - 5;
    if(visibleLine < 0) {
        visibleLine = 0;
    }

    ctrl->ScrollToLine(visibleLine);
}

void DiffSideBySidePanel::OnNextDiffUI(wxUpdateUIEvent& event) { event.Enable(CanNextDiff()); }

void DiffSideBySidePanel::OnPrevDiffUI(wxUpdateUIEvent& event) { event.Enable(CanPrevDiff()); }

void DiffSideBySidePanel::OnCopyLeftToRightUI(wxUpdateUIEvent& event)
{
    event.Enable(!IsRightReadOnly() && !m_config.IsSingleViewMode());
}

void DiffSideBySidePanel::OnCopyRightToLeftUI(wxUpdateUIEvent& event)
{
    event.Enable(!IsLeftReadOnly() && !m_config.IsSingleViewMode());
}

void DiffSideBySidePanel::OnCopyLeftToRight(wxRibbonButtonBarEvent& event)
{
    bool moveToNext = m_config.GetFlags() & DiffConfig::kCopyLeftToRightAndMove;
    DoCopyCurrentSequence(m_stcLeft, m_stcRight);
    if(moveToNext && CanNextDiff()) {
        wxRibbonButtonBarEvent dummy;
        OnNextDiffSequence(dummy);
    }
}

void DiffSideBySidePanel::OnCopyRightToLeft(wxRibbonButtonBarEvent& event)
{
    bool moveToNext = m_config.GetFlags() & DiffConfig::kCopyRightToLeftAndMove;
    DoCopyCurrentSequence(m_stcRight, m_stcLeft);
    if(moveToNext && CanNextDiff()) {
        wxRibbonButtonBarEvent dummy;
        OnNextDiffSequence(dummy);
    }
}

void DiffSideBySidePanel::DoCopyCurrentSequence(wxStyledTextCtrl* from, wxStyledTextCtrl* to)
{
    if(m_cur_sequence == wxNOT_FOUND) return;
    to->SetReadOnly(false);
    int fromStartPos = wxNOT_FOUND;
    int fromEndPos = wxNOT_FOUND;

    int toStartPos = wxNOT_FOUND;
    int toEndPos = wxNOT_FOUND;
    int placeHolderMarkerFirstLine = wxNOT_FOUND;
    int placeHolderMarkerLastLine = wxNOT_FOUND;
    int dummy;
    DoGetPositionsToCopy(from, fromStartPos, fromEndPos, placeHolderMarkerFirstLine, placeHolderMarkerLastLine);
    if(fromStartPos == wxNOT_FOUND || fromEndPos == wxNOT_FOUND) return;

    DoGetPositionsToCopy(to, toStartPos, toEndPos, dummy, dummy);
    if(toStartPos == wxNOT_FOUND || toEndPos == wxNOT_FOUND) return;

    // remove the old markers from the range of lines
    int toLine1 = to->LineFromPosition(toStartPos);
    int toLine2 = to->LineFromPosition(toEndPos);

    for(int i = toLine1; i < toLine2; ++i) {
        to->MarkerDelete(i, RED_MARKER);
        to->MarkerDelete(i, GREEN_MARKER);
        to->MarkerDelete(i, PLACE_HOLDER_MARKER);
        to->MarkerDelete(i, MARKER_SEQUENCE);

        from->MarkerDelete(i, RED_MARKER);
        from->MarkerDelete(i, GREEN_MARKER);
        from->MarkerDelete(i, PLACE_HOLDER_MARKER);
    }

    wxString textToCopy = from->GetTextRange(fromStartPos, fromEndPos);
    to->SetSelection(toStartPos, toEndPos);
    to->ReplaceSelection(textToCopy);
    for(int i = placeHolderMarkerFirstLine; i < placeHolderMarkerLastLine; ++i) {
        to->MarkerAdd(i, PLACE_HOLDER_MARKER);
        from->MarkerAdd(i, PLACE_HOLDER_MARKER);
    }

    // Restore the MARKER_SEQUENCE
    for(int i = toLine1; i < toLine2; ++i) {
        to->MarkerAdd(i, MARKER_SEQUENCE);
    }
    to->SetReadOnly(true);
}

void DiffSideBySidePanel::DoGetPositionsToCopy(wxStyledTextCtrl* stc,
                                               int& startPos,
                                               int& endPos,
                                               int& placeHolderMarkerFirstLine,
                                               int& placeHolderMarkerLastLine)
{
    startPos = wxNOT_FOUND;
    endPos = wxNOT_FOUND;
    placeHolderMarkerFirstLine = wxNOT_FOUND;

    int from_line = m_sequences.at(m_cur_sequence).first;
    int to_line = m_sequences.at(m_cur_sequence).second;

    for(int i = from_line; i < to_line; ++i) {
        if((stc->MarkerGet(i) & PLACE_HOLDER_MARKER_MASK)) {
            placeHolderMarkerFirstLine = i;      // first line of the placholder marker
            placeHolderMarkerLastLine = to_line; // last line of the placholder marker
            break;
        }
    }

    startPos = stc->PositionFromLine(from_line);
    endPos = stc->PositionFromLine(to_line) + stc->LineLength(to_line);
}

void DiffSideBySidePanel::DoSave(wxStyledTextCtrl* stc, const wxFileName& fn)
{
    if(!stc->IsModified()) return;

    // remove all lines that have the 'placeholder' markers
    wxString newContent = DoGetContentNoPlaceholders(stc);

    stc->SetReadOnly(false);
    stc->SetText(newContent);
    stc->SaveFile(fn.GetFullPath());
    stc->SetReadOnly(true);
    stc->SetSavePoint();
    stc->SetModified(false);

    // Emit a file-saved event
    EventNotifier::Get()->PostFileSavedEvent(fn.GetFullPath());

    // Reload any file opened in codelite
    EventNotifier::Get()->PostReloadExternallyModifiedEvent(false);
}

void DiffSideBySidePanel::OnSaveChanges(wxRibbonButtonBarEvent& event)
{
    DoSave(m_stcLeft, m_filePickerLeft->GetPath());
    DoSave(m_stcRight, m_filePickerRight->GetPath());
    Diff();
}

void DiffSideBySidePanel::OnSaveChangesUI(wxUpdateUIEvent& event)
{
    event.Enable((m_stcLeft->IsModified() || m_stcRight->IsModified()) && !m_config.IsSingleViewMode());
}

bool DiffSideBySidePanel::CanNextDiff()
{
    bool canNext = ((m_cur_sequence + 1) < (int)m_sequences.size());
    return !m_sequences.empty() && canNext;
}

bool DiffSideBySidePanel::CanPrevDiff()
{
    bool canPrev = ((m_cur_sequence - 1) >= 0);
    return !m_sequences.empty() && canPrev;
}

void DiffSideBySidePanel::OnCopyFileFromRight(wxRibbonButtonBarEvent& event)
{
    DoCopyFileContent(m_stcRight, m_stcLeft);
}

void DiffSideBySidePanel::OnCopyFileLeftToRight(wxRibbonButtonBarEvent& event)
{
    DoCopyFileContent(m_stcLeft, m_stcRight);
}

void DiffSideBySidePanel::DoCopyFileContent(wxStyledTextCtrl* from, wxStyledTextCtrl* to)
{
    to->SetReadOnly(false);
    wxString newContent = DoGetContentNoPlaceholders(from);
    to->SetText(newContent);
    to->SetReadOnly(true);

    // Clear RED and GREEN markers
    to->MarkerDeleteAll(RED_MARKER);
    to->MarkerDeleteAll(GREEN_MARKER);

    from->MarkerDeleteAll(RED_MARKER);
    from->MarkerDeleteAll(GREEN_MARKER);
}

void DiffSideBySidePanel::OnPageClosing(wxNotifyEvent& event)
{
    if(m_stcLeft->IsModified() || m_stcRight->IsModified()) {
        wxStandardID res = ::PromptForYesNoDialogWithCheckbox(
            _("Closing the diff viewer, will lose all your changes.\nContinue?"), "PromptDiffViewClose");
        if(res != wxID_YES) {
            event.Veto();
        } else {
            event.Skip();
        }

    } else {
        event.Skip();
    }
}

void DiffSideBySidePanel::OnHorizontal(wxRibbonButtonBarEvent& event)
{
    m_splitter->Unsplit();
    m_splitter->SplitHorizontally(m_splitterPageLeft, m_splitterPageRight);
    m_config.SetViewMode(DiffConfig::kViewHorizontalSplit);
    Diff();
}

void DiffSideBySidePanel::OnHorizontalUI(wxUpdateUIEvent& event) { event.Check(m_config.IsSplitHorizontal()); }

void DiffSideBySidePanel::OnVertical(wxRibbonButtonBarEvent& event)
{
    m_splitter->Unsplit();
    m_splitter->SplitVertically(m_splitterPageLeft, m_splitterPageRight);
    m_config.SetViewMode(DiffConfig::kViewVerticalSplit);
    Diff();
}

void DiffSideBySidePanel::OnVerticalUI(wxUpdateUIEvent& event) { event.Check(m_config.IsSplitVertical()); }

void DiffSideBySidePanel::DiffNew()
{
    m_flags = 0x0;
    m_config.SetViewMode(DiffConfig::kViewVerticalSplit);
    m_splitter->Unsplit();
    m_splitter->SplitVertically(m_splitterPageLeft, m_splitterPageRight);
}

void DiffSideBySidePanel::OnRefreshDiffUI(wxUpdateUIEvent& event) { wxUnusedVar(event); }
void DiffSideBySidePanel::OnLeftPickerUI(wxUpdateUIEvent& event) { event.Enable(!IsOriginSourceControl()); }

void DiffSideBySidePanel::OnRightPickerUI(wxUpdateUIEvent& event) { event.Enable(!IsOriginSourceControl()); }

wxString DiffSideBySidePanel::DoGetContentNoPlaceholders(wxStyledTextCtrl* stc) const
{
    wxString newContent;
    for(int i = 0; i < stc->GetLineCount(); ++i) {
        if(!(stc->MarkerGet(i) & PLACE_HOLDER_MARKER_MASK)) {
            newContent << stc->GetLine(i);
        }
    }
    return newContent;
}

void DiffSideBySidePanel::OnCopyLeftToRightMenu(wxRibbonButtonBarEvent& event)
{
    size_t flags = m_config.GetFlags();
    bool copyAndMove = flags & DiffConfig::kCopyLeftToRightAndMove;

    wxMenu menu;
    menu.Append(ID_COPY_LEFT_TO_RIGHT, _("Copy to the right"), wxEmptyString, wxITEM_RADIO);
    menu.Check(ID_COPY_LEFT_TO_RIGHT, !copyAndMove);

    menu.Append(
        ID_COPY_LEFT_TO_RIGHT_AND_MOVE, _("Copy to the right and move to the next diff"), wxEmptyString, wxITEM_RADIO);
    menu.Check(ID_COPY_LEFT_TO_RIGHT_AND_MOVE, copyAndMove);

    event.PopupMenu(&menu);
}

void DiffSideBySidePanel::OnCopyRightToLeftMenu(wxRibbonButtonBarEvent& event)
{
    size_t flags = m_config.GetFlags();
    bool copyAndMove = flags & DiffConfig::kCopyRightToLeftAndMove;

    wxMenu menu;
    menu.Append(ID_COPY_RIGHT_TO_LEFT, _("Copy to the left"), wxEmptyString, wxITEM_RADIO);
    menu.Check(ID_COPY_RIGHT_TO_LEFT, !copyAndMove);

    menu.Append(
        ID_COPY_RIGHT_TO_LEFT_AND_MOVE, _("Copy to the left and move to the next diff"), wxEmptyString, wxITEM_RADIO);
    menu.Check(ID_COPY_RIGHT_TO_LEFT_AND_MOVE, copyAndMove);
    event.PopupMenu(&menu);
}

void DiffSideBySidePanel::OnMenuCopyLeft2Right(wxCommandEvent& event)
{
    size_t flags = m_config.GetFlags();

    flags &= ~DiffConfig::kCopyLeftToRightAndMove;

    if(event.IsChecked() && event.GetId() == ID_COPY_LEFT_TO_RIGHT_AND_MOVE) {
        // save the new settings
        flags |= DiffConfig::kCopyLeftToRightAndMove;
    }
    m_config.SetFlags(flags);
}

void DiffSideBySidePanel::OnMenuCopyRight2Left(wxCommandEvent& event)
{
    size_t flags = m_config.GetFlags();

    flags &= ~DiffConfig::kCopyRightToLeftAndMove;

    if(event.IsChecked() && event.GetId() == ID_COPY_RIGHT_TO_LEFT_AND_MOVE) {
        // save the new settings
        flags |= DiffConfig::kCopyRightToLeftAndMove;
    }
    m_config.SetFlags(flags);
}

void DiffSideBySidePanel::OnSingleUI(wxUpdateUIEvent& event) { event.Check(m_config.IsSingleViewMode()); }

void DiffSideBySidePanel::OnSingleView(wxRibbonButtonBarEvent& event)
{
    m_config.SetViewMode(DiffConfig::kViewSingle);
    m_splitter->Unsplit();
    Diff();
}

void DiffSideBySidePanel::DoLayout()
{
    if(m_config.IsSingleViewMode()) {
        m_splitter->Unsplit();
    }
    GetSizer()->Layout();
    Refresh();
}
void DiffSideBySidePanel::OnMouseWheel(wxMouseEvent& event)
{
    event.Skip();
    if(::wxGetKeyState(WXK_CONTROL) && !EditorConfigST::Get()->GetOptions()->IsMouseZoomEnabled()) {
        event.Skip(false);
        return;
    }
}

//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: CMakePlugin.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "CMakePluginUi.h"


// Declare the bitmap loading function
extern void wxCrafterR3nJ3cInitBitmapResources();

static bool bBitmapLoaded = false;


CMakeSettingsDialogBase::CMakeSettingsDialogBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterR3nJ3cInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    wxBoxSizer* boxSizerMain = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizerMain);
    
    wxFlexGridSizer* flexGridSizer = new wxFlexGridSizer(0, 2, 5, 5);
    flexGridSizer->SetFlexibleDirection( wxBOTH );
    flexGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    flexGridSizer->AddGrowableCol(1);
    
    boxSizerMain->Add(flexGridSizer, 1, wxALL|wxEXPAND, 5);
    
    m_staticTextProgram = new wxStaticText(this, wxID_ANY, _("CMake program:"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    flexGridSizer->Add(m_staticTextProgram, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    
    m_filePickerProgram = new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*"), wxDefaultPosition, wxSize(-1,-1), wxFLP_DEFAULT_STYLE|wxFLP_USE_TEXTCTRL);
    m_filePickerProgram->SetToolTip(_("Path to cmake executable."));
    
    flexGridSizer->Add(m_filePickerProgram, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
    m_filePickerProgram->SetMinSize(wxSize(200,-1));
    
    m_staticTextDefaultGenerator = new wxStaticText(this, wxID_ANY, _("Default Generator:"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    flexGridSizer->Add(m_staticTextDefaultGenerator, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    
    wxArrayString m_choiceDefaultGeneratorArr;
    m_choiceDefaultGenerator = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), m_choiceDefaultGeneratorArr, 0);
    m_choiceDefaultGenerator->SetToolTip(_("You can specify default generator for all projects (if is not overrided by project settings). If generator is not selected the CMake uses platform's default."));
    
    flexGridSizer->Add(m_choiceDefaultGenerator, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
    
    m_stdBtnSizer = new wxStdDialogButtonSizer();
    
    boxSizerMain->Add(m_stdBtnSizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
    
    m_buttonOk = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxSize(-1, -1), 0);
    m_buttonOk->SetDefault();
    m_stdBtnSizer->AddButton(m_buttonOk);
    
    m_buttonCancel = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxSize(-1, -1), 0);
    m_stdBtnSizer->AddButton(m_buttonCancel);
    m_stdBtnSizer->Realize();
    
    SetMinSize( wxSize(400,150) );
    SetSizeHints(-1,-1);
    if ( GetSizer() ) {
         GetSizer()->Fit(this);
    }
    Centre(wxBOTH);
}

CMakeSettingsDialogBase::~CMakeSettingsDialogBase()
{
}

CMakeHelpTabBase::CMakeHelpTabBase(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterR3nJ3cInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    wxBoxSizer* boxSizerMain = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizerMain);
    
    wxFlexGridSizer* flexGridSizer225 = new wxFlexGridSizer(0, 3, 0, 0);
    flexGridSizer225->SetFlexibleDirection( wxBOTH );
    flexGridSizer225->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    flexGridSizer225->AddGrowableCol(1);
    
    boxSizerMain->Add(flexGridSizer225, 0, wxALL|wxEXPAND, 0);
    
    m_staticTextVersion = new wxStaticText(this, wxID_ANY, _("Version:"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    flexGridSizer225->Add(m_staticTextVersion, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    
    m_staticTextVersionValue = new wxStaticText(this, wxID_ANY, _("?"), wxDefaultPosition, wxSize(-1,-1), 0);
    wxFont m_staticTextVersionValueFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    m_staticTextVersionValueFont.SetWeight(wxFONTWEIGHT_BOLD);
    m_staticTextVersionValue->SetFont(m_staticTextVersionValueFont);
    
    flexGridSizer225->Add(m_staticTextVersionValue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    
    flexGridSizer225->Add(0, 0, 1, wxALL, 5);
    
    m_staticText222 = new wxStaticText(this, wxID_ANY, _("Select Topic:"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    flexGridSizer225->Add(m_staticText222, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    
    wxArrayString m_choiceTopicsArr;
    m_choiceTopicsArr.Add(wxT("Modules"));
    m_choiceTopicsArr.Add(wxT("Commands"));
    m_choiceTopicsArr.Add(wxT("Properties"));
    m_choiceTopicsArr.Add(wxT("Variables"));
    m_choiceTopics = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), m_choiceTopicsArr, 0);
    m_choiceTopics->SetSelection(0);
    
    flexGridSizer225->Add(m_choiceTopics, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
    
    m_buttonReload = new wxButton(this, wxID_REFRESH, _("Reload"), wxDefaultPosition, wxSize(-1,-1), 0);
    m_buttonReload->SetToolTip(_("Reloads Help from CMake"));
    
    flexGridSizer225->Add(m_buttonReload, 0, wxALL, 5);
    
    m_splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxSP_LIVE_UPDATE|wxSP_NO_XP_THEME|wxSP_3DSASH);
    m_splitter->SetSashGravity(0.3);
    m_splitter->SetMinimumPaneSize(100);
    
    boxSizerMain->Add(m_splitter, 1, wxALL|wxEXPAND, 5);
    
    m_splitterPageList = new wxPanel(m_splitter, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL);
    
    wxBoxSizer* boxSizerList = new wxBoxSizer(wxVERTICAL);
    m_splitterPageList->SetSizer(boxSizerList);
    
    m_searchCtrlFilter = new wxSearchCtrl(m_splitterPageList, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(-1,-1), wxTE_PROCESS_ENTER);
    m_searchCtrlFilter->SetFocus();
    m_searchCtrlFilter->ShowSearchButton(true);
    m_searchCtrlFilter->ShowCancelButton(false);
    
    boxSizerList->Add(m_searchCtrlFilter, 0, wxBOTTOM|wxEXPAND, 5);
    m_searchCtrlFilter->SetMinSize(wxSize(-1,22));
    
    wxArrayString m_listBoxListArr;
    m_listBoxList = new wxListBox(m_splitterPageList, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), m_listBoxListArr, wxLB_SINGLE);
    m_listBoxList->SetToolTip(_("Double click to insert in the current editor."));
    
    boxSizerList->Add(m_listBoxList, 1, wxALL|wxEXPAND, 0);
    m_listBoxList->SetMinSize(wxSize(100,200));
    
    m_splitterPageText = new wxPanel(m_splitter, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL);
    m_splitter->SplitVertically(m_splitterPageList, m_splitterPageText, 100);
    
    wxBoxSizer* boxSizerText = new wxBoxSizer(wxVERTICAL);
    m_splitterPageText->SetSizer(boxSizerText);
    
    m_htmlWinText = new wxHtmlWindow(m_splitterPageText, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxHW_SCROLLBAR_AUTO|wxBORDER_THEME);
    m_htmlWinText->SetPage(wxT(""));
    
    boxSizerText->Add(m_htmlWinText, 1, wxALL|wxEXPAND, 0);
    
    m_gaugeLoad = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxSize(-1,-1), wxGA_HORIZONTAL);
    m_gaugeLoad->Hide();
    m_gaugeLoad->SetValue(0);
    
    boxSizerMain->Add(m_gaugeLoad, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 5);
    
    SetMinSize( wxSize(300,400) );
    SetSizeHints(300,400);
    if ( GetSizer() ) {
         GetSizer()->Fit(this);
    }
    Centre(wxBOTH);
    // Connect events
    m_staticTextVersion->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeHelpTabBase::OnUpdateUi), NULL, this);
    m_staticTextVersionValue->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeHelpTabBase::OnUpdateUi), NULL, this);
    m_staticText222->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeHelpTabBase::OnUpdateUi), NULL, this);
    m_choiceTopics->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(CMakeHelpTabBase::OnChangeTopic), NULL, this);
    m_choiceTopics->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeHelpTabBase::OnUpdateUi), NULL, this);
    m_buttonReload->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CMakeHelpTabBase::OnReload), NULL, this);
    m_buttonReload->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeHelpTabBase::OnUpdateUi), NULL, this);
    m_splitter->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(CMakeHelpTabBase::OnRightClick), NULL, this);
    m_searchCtrlFilter->Connect(wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(CMakeHelpTabBase::OnSearch), NULL, this);
    m_searchCtrlFilter->Connect(wxEVT_COMMAND_SEARCHCTRL_CANCEL_BTN, wxCommandEventHandler(CMakeHelpTabBase::OnSearchCancel), NULL, this);
    m_searchCtrlFilter->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(CMakeHelpTabBase::OnSearch), NULL, this);
    m_searchCtrlFilter->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeHelpTabBase::OnUpdateUi), NULL, this);
    m_listBoxList->Connect(wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(CMakeHelpTabBase::OnSelect), NULL, this);
    m_listBoxList->Connect(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler(CMakeHelpTabBase::OnInsert), NULL, this);
    m_listBoxList->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeHelpTabBase::OnUpdateUi), NULL, this);
    m_htmlWinText->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeHelpTabBase::OnUpdateUi), NULL, this);
    
}

CMakeHelpTabBase::~CMakeHelpTabBase()
{
    m_staticTextVersion->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeHelpTabBase::OnUpdateUi), NULL, this);
    m_staticTextVersionValue->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeHelpTabBase::OnUpdateUi), NULL, this);
    m_staticText222->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeHelpTabBase::OnUpdateUi), NULL, this);
    m_choiceTopics->Disconnect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(CMakeHelpTabBase::OnChangeTopic), NULL, this);
    m_choiceTopics->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeHelpTabBase::OnUpdateUi), NULL, this);
    m_buttonReload->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CMakeHelpTabBase::OnReload), NULL, this);
    m_buttonReload->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeHelpTabBase::OnUpdateUi), NULL, this);
    m_splitter->Disconnect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(CMakeHelpTabBase::OnRightClick), NULL, this);
    m_searchCtrlFilter->Disconnect(wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(CMakeHelpTabBase::OnSearch), NULL, this);
    m_searchCtrlFilter->Disconnect(wxEVT_COMMAND_SEARCHCTRL_CANCEL_BTN, wxCommandEventHandler(CMakeHelpTabBase::OnSearchCancel), NULL, this);
    m_searchCtrlFilter->Disconnect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(CMakeHelpTabBase::OnSearch), NULL, this);
    m_searchCtrlFilter->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeHelpTabBase::OnUpdateUi), NULL, this);
    m_listBoxList->Disconnect(wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(CMakeHelpTabBase::OnSelect), NULL, this);
    m_listBoxList->Disconnect(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler(CMakeHelpTabBase::OnInsert), NULL, this);
    m_listBoxList->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeHelpTabBase::OnUpdateUi), NULL, this);
    m_htmlWinText->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeHelpTabBase::OnUpdateUi), NULL, this);
    
}

CMakeProjectSettingsPanelBase::CMakeProjectSettingsPanelBase(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterR3nJ3cInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer);
    
    m_checkBoxEnable = new wxCheckBox(this, wxID_ANY, _("Enable CMake for this project"), wxDefaultPosition, wxSize(-1,-1), 0);
    m_checkBoxEnable->SetValue(false);
    
    boxSizer->Add(m_checkBoxEnable, 0, wxALL, 5);
    
    wxFlexGridSizer* flexGridSizer = new wxFlexGridSizer(5, 2, 5, 5);
    flexGridSizer->SetFlexibleDirection( wxBOTH );
    flexGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    flexGridSizer->AddGrowableCol(1);
    
    boxSizer->Add(flexGridSizer, 0, wxALL|wxEXPAND, 5);
    
    m_staticTextParent = new wxStaticText(this, wxID_ANY, _("Parent project:"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    flexGridSizer->Add(m_staticTextParent, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
    
    wxArrayString m_choiceParentArr;
    m_choiceParent = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), m_choiceParentArr, 0);
    m_choiceParent->SetToolTip(_("Allow you to specify parent project. Specify this when project is sub-directory (see add_subdirectory) and it's built with the parent project."));
    
    flexGridSizer->Add(m_choiceParent, 0, wxALL|wxEXPAND, 0);
    
    m_staticTextSourceDir = new wxStaticText(this, wxID_ANY, _("Sources directory:"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    flexGridSizer->Add(m_staticTextSourceDir, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
    
    m_dirPickerSourceDir = new wxDirPickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxSize(-1,-1), wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL);
    m_dirPickerSourceDir->SetToolTip(_("Path to directory where CMakeLists.txt is located."));
    
    flexGridSizer->Add(m_dirPickerSourceDir, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);
    
    m_staticTextBuildDir = new wxStaticText(this, wxID_ANY, _("Build directory:"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    flexGridSizer->Add(m_staticTextBuildDir, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
    
    m_dirPickerBuildDir = new wxDirPickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxSize(-1,-1), wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL);
    m_dirPickerBuildDir->SetToolTip(_("Directory where the project will be built. Path is relative to $(WorkspacePath)."));
    
    flexGridSizer->Add(m_dirPickerBuildDir, 0, wxALL|wxEXPAND, 0);
    
    m_staticTextGenerator = new wxStaticText(this, wxID_ANY, _("Generator:"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    flexGridSizer->Add(m_staticTextGenerator, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
    
    wxArrayString m_choiceGeneratorArr;
    m_choiceGenerator = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), m_choiceGeneratorArr, 0);
    m_choiceGenerator->SetToolTip(_("Generator that will be used for CMake configuration. If no generator is selected, plugin uses global default generator selected in plugin settings."));
    
    flexGridSizer->Add(m_choiceGenerator, 0, wxALL|wxEXPAND, 0);
    
    m_staticTextBuildType = new wxStaticText(this, wxID_ANY, _("Build Type:"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    flexGridSizer->Add(m_staticTextBuildType, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
    
    wxArrayString m_comboBoxBuildTypeArr;
    m_comboBoxBuildTypeArr.Add(wxT("None"));
    m_comboBoxBuildTypeArr.Add(wxT("Debug"));
    m_comboBoxBuildTypeArr.Add(wxT("Release"));
    m_comboBoxBuildTypeArr.Add(wxT("RelWithDebInfo"));
    m_comboBoxBuildTypeArr.Add(wxT("MinSizeRel"));
    m_comboBoxBuildType = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(-1,-1), m_comboBoxBuildTypeArr, 0);
    m_comboBoxBuildType->SetToolTip(_("When is selected pass -DCMAKE_BUILD_TYPE to cmake."));
    #if wxVERSION_NUMBER >= 3000
    m_comboBoxBuildType->SetHint(wxT(""));
    #endif
    
    flexGridSizer->Add(m_comboBoxBuildType, 0, wxALL|wxEXPAND, 0);
    
    m_staticTextArguments = new wxStaticText(this, wxID_ANY, _("CMake arguments (used for configuration)"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    boxSizer->Add(m_staticTextArguments, 0, wxALL, 5);
    
    m_textCtrlArguments = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(-1,-1), wxTE_MULTILINE);
    m_textCtrlArguments->SetToolTip(_("Argument list used when CMake is called. Each argument must be separated by new line. Multiple arguments on the one line are OK too if they're separated by space.\nDo not use arguments -DCMAKE_BUILD_TYPE, -G and 'path', they are passed by the plugin.\n\nExample:\n-DCMAKE_CXX_FLAGS=-g\n-DCMAKE_C_FLAGS=-g"));
    
    boxSizer->Add(m_textCtrlArguments, 1, wxALL|wxEXPAND, 5);
    
    SetMinSize( wxSize(400,300) );
    SetSizeHints(400,300);
    if ( GetSizer() ) {
         GetSizer()->Fit(this);
    }
    Centre(wxBOTH);
    // Connect events
    m_staticTextParent->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck), NULL, this);
    m_choiceParent->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck), NULL, this);
    m_staticTextSourceDir->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_dirPickerSourceDir->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_staticTextBuildDir->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_dirPickerBuildDir->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_staticTextGenerator->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_choiceGenerator->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_staticTextBuildType->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_comboBoxBuildType->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_staticTextArguments->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_textCtrlArguments->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    
}

CMakeProjectSettingsPanelBase::~CMakeProjectSettingsPanelBase()
{
    m_staticTextParent->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck), NULL, this);
    m_choiceParent->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck), NULL, this);
    m_staticTextSourceDir->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_dirPickerSourceDir->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_staticTextBuildDir->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_dirPickerBuildDir->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_staticTextGenerator->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_choiceGenerator->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_staticTextBuildType->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_comboBoxBuildType->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_staticTextArguments->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    m_textCtrlArguments->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CMakeProjectSettingsPanelBase::OnCheck2), NULL, this);
    
}

cmakeImages::cmakeImages()
    : wxImageList(16, 16, true)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterR3nJ3cInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    {
        wxBitmap bmp;
        wxIcon icn;
        bmp = wxXmlResource::Get()->LoadBitmap(wxT("cmake_16"));
        icn.CopyFromBitmap( bmp );
        this->Add( icn );
        m_bitmaps.insert( std::make_pair(wxT("cmake_16"), bmp ) );
    }
    
}

cmakeImages::~cmakeImages()
{
}

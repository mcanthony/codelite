//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: CMakePlugin.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef CMAKEPLUGIN_BASE_CLASSES_H
#define CMAKEPLUGIN_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/filepicker.h>
#include <wx/choice.h>
#include <wx/arrstr.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/srchctrl.h>
#include <wx/listbox.h>
#include <wx/html/htmlwin.h>
#include <wx/gauge.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/textctrl.h>
#include <wx/imaglist.h>
#include <wx/bitmap.h>
#include <map>
#include <wx/icon.h>

class CMakeSettingsDialogBase : public wxDialog
{
protected:
    wxStaticText* m_staticTextProgram;
    wxFilePickerCtrl* m_filePickerProgram;
    wxStaticText* m_staticTextDefaultGenerator;
    wxChoice* m_choiceDefaultGenerator;
    wxStdDialogButtonSizer* m_stdBtnSizer;
    wxButton* m_buttonOk;
    wxButton* m_buttonCancel;

protected:

public:
    CMakeSettingsDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("CMakePlugin Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1,-1), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
    virtual ~CMakeSettingsDialogBase();
};


class CMakeHelpTabBase : public wxPanel
{
protected:
    wxStaticText* m_staticTextVersion;
    wxStaticText* m_staticTextVersionValue;
    wxStaticText* m_staticText222;
    wxChoice* m_choiceTopics;
    wxButton* m_buttonReload;
    wxSplitterWindow* m_splitter;
    wxPanel* m_splitterPageList;
    wxSearchCtrl* m_searchCtrlFilter;
    wxListBox* m_listBoxList;
    wxPanel* m_splitterPageText;
    wxHtmlWindow* m_htmlWinText;
    wxGauge* m_gaugeLoad;

protected:
    virtual void OnUpdateUi(wxUpdateUIEvent& event) { event.Skip(); }
    virtual void OnChangeTopic(wxCommandEvent& event) { event.Skip(); }
    virtual void OnReload(wxCommandEvent& event) { event.Skip(); }
    virtual void OnRightClick(wxMouseEvent& event) { event.Skip(); }
    virtual void OnSearch(wxCommandEvent& event) { event.Skip(); }
    virtual void OnSearchCancel(wxCommandEvent& event) { event.Skip(); }
    virtual void OnSelect(wxCommandEvent& event) { event.Skip(); }
    virtual void OnInsert(wxCommandEvent& event) { event.Skip(); }

public:
    CMakeHelpTabBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(300,400), long style = wxTAB_TRAVERSAL);
    virtual ~CMakeHelpTabBase();
};


class CMakeProjectSettingsPanelBase : public wxPanel
{
protected:
    wxCheckBox* m_checkBoxEnable;
    wxStaticText* m_staticTextParent;
    wxChoice* m_choiceParent;
    wxStaticText* m_staticTextSourceDir;
    wxDirPickerCtrl* m_dirPickerSourceDir;
    wxStaticText* m_staticTextBuildDir;
    wxDirPickerCtrl* m_dirPickerBuildDir;
    wxStaticText* m_staticTextGenerator;
    wxChoice* m_choiceGenerator;
    wxStaticText* m_staticTextBuildType;
    wxComboBox* m_comboBoxBuildType;
    wxStaticText* m_staticTextArguments;
    wxTextCtrl* m_textCtrlArguments;

protected:
    virtual void OnCheck(wxUpdateUIEvent& event) { event.Skip(); }
    virtual void OnCheck2(wxUpdateUIEvent& event) { event.Skip(); }

public:
    CMakeProjectSettingsPanelBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(400,300), long style = wxTAB_TRAVERSAL);
    virtual ~CMakeProjectSettingsPanelBase();
};


class cmakeImages : public wxImageList
{
protected:
    // Maintain a map of all bitmaps representd by their name
    std::map<wxString, wxBitmap> m_bitmaps;


protected:

public:
    cmakeImages();
    const wxBitmap& Bitmap(const wxString &name) const {
        if ( !m_bitmaps.count(name) )
            return wxNullBitmap;
        return m_bitmaps.find(name)->second;
    }
    virtual ~cmakeImages();
};

#endif

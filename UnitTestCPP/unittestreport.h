//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: unittestreport.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef CODELITE_UNITTESTCPP_UNITTESTREPORT_BASE_CLASSES_H
#define CODELITE_UNITTESTCPP_UNITTESTREPORT_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/panel.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include "progressctrl.h"
#include <wx/listctrl.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

class UnitTestsBasePage : public wxPanel
{
protected:
    wxStaticText* m_staticText7;
    ProgressCtrl* m_progressPassed;
    wxStaticText* m_staticText8;
    ProgressCtrl* m_progressFailed;
    wxStaticText* m_staticText10;
    wxStaticText* m_staticTextTotalTests;
    wxStaticText* m_staticText12;
    wxStaticText* m_staticTextFailTestsNum;
    wxStaticText* m_staticText14;
    wxStaticText* m_staticTextSuccessTestsNum;
    wxListCtrl* m_listCtrlErrors;

protected:
    virtual void OnItemActivated(wxListEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticText7() { return m_staticText7; }
    ProgressCtrl* GetProgressPassed() { return m_progressPassed; }
    wxStaticText* GetStaticText8() { return m_staticText8; }
    ProgressCtrl* GetProgressFailed() { return m_progressFailed; }
    wxStaticText* GetStaticText10() { return m_staticText10; }
    wxStaticText* GetStaticTextTotalTests() { return m_staticTextTotalTests; }
    wxStaticText* GetStaticText12() { return m_staticText12; }
    wxStaticText* GetStaticTextFailTestsNum() { return m_staticTextFailTestsNum; }
    wxStaticText* GetStaticText14() { return m_staticText14; }
    wxStaticText* GetStaticTextSuccessTestsNum() { return m_staticTextSuccessTestsNum; }
    wxListCtrl* GetListCtrlErrors() { return m_listCtrlErrors; }
    UnitTestsBasePage(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1,-1), long style = wxTAB_TRAVERSAL);
    virtual ~UnitTestsBasePage();
};

#endif

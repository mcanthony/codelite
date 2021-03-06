//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: messagepane.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef CODELITE_LITEEDITOR_MESSAGEPANE_BASE_CLASSES_H
#define CODELITE_LITEEDITOR_MESSAGEPANE_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/panel.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

class MessagePaneBase : public wxPanel
{
protected:
    wxPanel* m_panel2;
    wxStaticBitmap* m_bitmap1;
    wxPanel* m_panel1;
    wxStaticText* m_staticText2;
    wxStaticText* m_staticTextMessage;
    wxCheckBox* m_DontAnnoyMeCheck;
    wxButton* m_buttonClose;
    wxButton* m_buttonAction;
    wxButton* m_buttonAction1;
    wxButton* m_buttonAction2;

protected:
    virtual void OnEraseBG(wxEraseEvent& event) { event.Skip(); }
    virtual void OnPaint(wxPaintEvent& event) { event.Skip(); }
    virtual void OnButtonClose(wxCommandEvent& event) { event.Skip(); }
    virtual void OnActionButton(wxCommandEvent& event) { event.Skip(); }
    virtual void OnActionButton1(wxCommandEvent& event) { event.Skip(); }
    virtual void OnActionButton2(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticBitmap* GetBitmap1() { return m_bitmap1; }
    wxStaticText* GetStaticText2() { return m_staticText2; }
    wxStaticText* GetStaticTextMessage() { return m_staticTextMessage; }
    wxCheckBox* GetDontAnnoyMeCheck() { return m_DontAnnoyMeCheck; }
    wxPanel* GetPanel1() { return m_panel1; }
    wxButton* GetButtonClose() { return m_buttonClose; }
    wxButton* GetButtonAction() { return m_buttonAction; }
    wxButton* GetButtonAction1() { return m_buttonAction1; }
    wxButton* GetButtonAction2() { return m_buttonAction2; }
    wxPanel* GetPanel2() { return m_panel2; }
    MessagePaneBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1,-1), long style = wxTAB_TRAVERSAL);
    virtual ~MessagePaneBase();
};

#endif

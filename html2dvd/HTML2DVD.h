
#ifndef _HTML2DVD_H_
#define _HTML2DVD_H_

#define HTML2DVD_VER "1.0.1"

#include <wx/wx.h>
#include <wx/confbase.h>
#include <wx/msw/iniconf.h>
#include <wx/config.h>
#include <wx/datetime.h>
#include <wx/dialup.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/log.h>
#include <wx/notebook.h>
#include <wx/socket.h>
#include <wx/string.h>
#include <wx/textdlg.h>
#include <wx/utils.h>
#include <wx/image.h>
#include <wx/html/htmprint.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/fs_inet.h>
#include <wx/fs_zip.h>
#include <wx/fs_mem.h>
#include <wx/wizard.h>
#include <wx/splitter.h>
#include <wx/statline.h>
#include <wx/dirdlg.h>
#include <wx/busyinfo.h>
#include <wx/scrolwin.h>
#include <wx/settings.h>
#include <wx/valtext.h>
#include <wx/spinctrl.h>
#include <wx/mstream.h>

#include "wxImageResampler.h"
#include "dvdauthorController.h"
#include "DVDMenuButton.h"

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class HTML2DVDApp : public wxApp
{
public:
    virtual bool OnInit();
};

class WelcomePage : public wxWizardPageSimple
{
	DECLARE_CLASS(WelcomePage);
public:
    WelcomePage(wxWizard *parent);

protected:

};

class SetupPage : public wxWizardPageSimple
{
	DECLARE_CLASS(SetupPage);
public:
    SetupPage(wxWizard *parent);

protected:

};

class HTMLPage : public wxWizardPageSimple
{
	DECLARE_CLASS(HTMLPage);
public:
   HTMLPage(wxWizard *parent);
	 virtual ~HTMLPage();

	void OnButtonBrowse(wxCommandEvent &event);
	void OnButtonRender(wxCommandEvent &event);
	void OnMouseWheel(wxMouseEvent &event);

	wxString GetPath() { return text_ctrl_html_file->GetValue(); };
	DVDMenu &GetDVDMenu() { return m_Menu; };

protected:
	DVDMenu m_Menu;
  wxStaticText* label_preview;
	wxScrolledWindow *preview_window;
  wxStaticBitmap* bitmap_preview;
  wxPanel* window_html_pane_preview;
  wxStaticText* label_html_file;
  wxTextCtrl* text_ctrl_html_file;
  wxButton* button_html_file_browse;
	wxButton* button_html_file_render;
	wxSpinCtrl* spin_ctrl_render_width;
  wxPanel* window_html_pane_settings;	

private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

class DVDPage : public wxWizardPageSimple
{
	DECLARE_CLASS(DVDPage);
public:
    DVDPage(wxWizard *parent);
		virtual ~DVDPage();

		void OnButtonBrowse(wxCommandEvent &event);
		void OnButtonWrite(wxCommandEvent &event);		

protected:
    wxTextCtrl* text_ctrl_dvd_output;
    wxButton* button_dvd_output_browse;
    wxButton* button_dvd_write;
		;
private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

class FinishPage : public wxWizardPageSimple
{
	DECLARE_CLASS(FinishPage);
public:
    FinishPage(wxWizard *parent);

protected:


};
// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
	HTMLPage_ButtonRender,
	HTMLPage_ButtonBrowse,
	DVDPage_ButtonBrowse,
	DVDPage_ButtonWrite
};

#endif // _HTML2DVD_H_


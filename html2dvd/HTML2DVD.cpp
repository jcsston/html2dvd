
#include "HTML2DVD.h"
#include "HTML2DVDImages.h"
#include "ResourceLoader.h"

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.

BEGIN_EVENT_TABLE(WelcomePage, wxWizardPage)
    EVT_CHECKBOX(WelcomePage_CheckBoxSetup,		WelcomePage::OnCheckSetup)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(SetupPage, wxWizardPage)
    EVT_BUTTON(SettingsPage_ButtondvdauthorLocationBrowse,		SetupPage::OnButtondvdauthorLocationBrowse)
    EVT_BUTTON(SettingsPage_ButtonFontChange,									SetupPage::OnButtonFontChange)
		EVT_TEXT(SettingsPage_TextdvdauthorLocation,							SetupPage::OnTextdvdauthorLocationChange)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(HTMLScrolledWindow, wxScrolledWindow)
		EVT_LEFT_UP(HTMLScrolledWindow::OnMouseLeftDoubleClick)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(HTMLPage, wxWizardPage)
    EVT_BUTTON(HTMLPage_ButtonBrowse,		HTMLPage::OnButtonBrowse)
    EVT_BUTTON(HTMLPage_ButtonRender,		HTMLPage::OnButtonRender)
		EVT_MOUSEWHEEL(HTMLPage::OnMouseWheel)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(DVDPage, wxWizardPage)
    EVT_BUTTON(DVDPage_ButtonBrowse,		DVDPage::OnButtonBrowse)
		EVT_BUTTON(DVDPage_ButtonWrite,			DVDPage::OnButtonWrite)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(HTML2DVDApp)

IMPLEMENT_CLASS(WelcomePage, wxWizardPage);
IMPLEMENT_CLASS(SetupPage, wxWizardPage);
IMPLEMENT_CLASS(HTMLPage, wxWizardPage);
IMPLEMENT_CLASS(DVDPage, wxWizardPage);
IMPLEMENT_CLASS(FinishPage, wxWizardPage);

wxConfig *g_configuration = NULL;


// 'Main program' equivalent: the program execution "starts" here
bool HTML2DVDApp::OnInit()
{
  wxImage::AddHandler( new wxPNGHandler );
  wxImage::AddHandler( new wxJPEGHandler );
#ifdef HAVE_GIF
	// Evil GIF
  wxImage::AddHandler( new wxGIFHandler );
#endif
	wxFileSystem::AddHandler( new wxInternetFSHandler );
	//wxFileSystem::AddHandler( new wxZipFSHandler );
 
  SetAppName(wxT("HTML2DVD"));
	SetVendorName(wxT(""));
  wxConfigBase::Set(g_configuration = new wxConfig(wxT("HTML2DVD"), wxT("")));
	g_configuration->SetRecordDefaults();
  
#ifdef WIN32_RESOURCE_LOAD
	LoadWin32Resources();
#endif
/*
	long elaspedTime;
	int i;
	
	wxStartTimer();	
	// Load PNG
	for (i = 0; i < 5; i++) {
		//wxMemoryInputStream memInput(welcome_page_png, welcome_page_png_size);
		//wxImage pngImg(memInput);
		wxBitmap pngBitmap(LoadImageFromMemory(welcome_page_png, welcome_page_png_size));
	}	
	elaspedTime = wxGetElapsedTime();
	wxLogMessage(wxT("PNG Loading took an avg of %f ms."), (float)elaspedTime / 5);

	wxStartTimer();
	// Load XPM
	for (i = 0; i < 5; i++) {
		wxBitmap xpmImg(welcome_page_xpm);
	}
	elaspedTime = wxGetElapsedTime();
	wxLogMessage(wxT("XPM Loading took an avg of %f ms."), (float)elaspedTime / 5);
*/
	wxWizard *wizard = new wxWizard(NULL, -1, wxT("HTML2DVD ") wxT(HTML2DVD_VER), wxBitmap(LoadImageFromMemory(welcome_page_png, welcome_page_png_size)));
	
	bool bNeedSetup = false;
	wxString setupReason;

	if (!wxFileName::DirExists(g_configuration->Read(wxT("dvdauthor Location"), wxEmptyString))) {
		bNeedSetup = true;
		setupReason += wxT("dvdauthor Location Missing\n");
	}
	
	WelcomePage *welcome = new WelcomePage(wizard, bNeedSetup);
	SetupPage *setup = new SetupPage(wizard, setupReason);	
	welcome->SetSetupPage(setup);

	HTMLPage *html = new HTMLPage(wizard);
	welcome->SetHTMLPage(html);

	DVDPage *dvd = new DVDPage(wizard);
	FinishPage *finish = new FinishPage(wizard);
	
	
	//wxWizardPageSimple::Chain(welcome, setup);
	//wxWizardPageSimple::Chain(setup, html);
	
	//wxWizardPageSimple::Chain(welcome, html);
	wxWizardPageSimple::Chain(html, dvd);
	wxWizardPageSimple::Chain(dvd, finish);
	wizard->RunWizard(welcome);

	wizard->Destroy();
	return FALSE;
}

WelcomePage::WelcomePage(wxWizard *parent, bool bSetupRequired)
	: wxWizardPage(parent, wxBitmap(LoadImageFromMemory(welcome_page_png, welcome_page_png_size)))
{
  wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
	sizer_1->Add(new wxStaticText(this, -1, wxT("Welcome to HTML2DVD\n")
                                   wxT("This program can convert your html web pages\n")
																	 wxT("into DVD Menu's.\n")
																	 wxT("Now lets get on with this, click Next.")), 0, wxALL, 2);		  

	check_box_show_setup_page = new wxCheckBox(this, WelcomePage_CheckBoxSetup, wxT("Show Setup Page"));
	check_box_show_setup_page->SetValue(bSetupRequired);
	if (bSetupRequired)
		check_box_show_setup_page->Disable();

	sizer_1->Add(check_box_show_setup_page);

	SetAutoLayout(true);
  SetSizer(sizer_1);
  sizer_1->Fit(this);
  sizer_1->SetSizeHints(this);
  Layout();
}

void WelcomePage::OnCheckSetup(wxCommandEvent &event)
{
/*	
	if (check_box_show_setup_page->IsChecked())
		SetNext(m_SetupPage);
	else
		SetNext(m_HTMLPage);
*/
}

void WelcomePage::SetHTMLPage(wxWizardPageSimple *page)
{
	m_HTMLPage = page;
}

void WelcomePage::SetSetupPage(wxWizardPageSimple *page)
{
	m_SetupPage = page;
}

wxWizardPage *WelcomePage::GetPrev() const
{
	return NULL;
}

wxWizardPage *WelcomePage::GetNext() const
{
	if (check_box_show_setup_page->IsChecked()) {
		m_SetupPage->SetNext(m_HTMLPage);
		m_SetupPage->SetPrev((wxWizardPage *)this);
		m_HTMLPage->SetPrev(m_SetupPage);
		return m_SetupPage;
	} else {
		m_HTMLPage->SetPrev((wxWizardPage *)this);
		return m_HTMLPage;
	}
}

SetupPage::SetupPage(wxWizard *parent, const wxString &setupReason)
	: wxWizardPageSimple(parent, NULL, NULL, wxBitmap(LoadImageFromMemory(welcome_page_png, welcome_page_png_size)))
{
  text_ctrl_dvdauthor_location = new wxTextCtrl(this, SettingsPage_TextdvdauthorLocation, wxT(""));
	text_ctrl_dvdauthor_location->SetValue(g_configuration->Read(wxT("dvdauthor Location"), wxT("")));

  button_dvdauthor_location_browse = new wxButton(this, SettingsPage_ButtondvdauthorLocationBrowse, wxT("..."));
  text_ctrl_button_text_font = new wxTextCtrl(this, -1, wxT("Sample Up Down Button Text"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  button_button_text_font_change = new wxButton(this, SettingsPage_ButtonFontChange, wxT("Change"));
	
	wxFont buttonFont = GetFontConfig(g_configuration, wxT("Button Text"));
	wxTextAttr buttonTextAttr(*wxBLACK, *wxWHITE, buttonFont);
	text_ctrl_button_text_font->SetStyle(0, text_ctrl_button_text_font->GetLastPosition(), buttonTextAttr);
  
	wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
	sizer_1->Add(new wxStaticText(this, -1, wxT("You need to set some settings here")), 0, wxALL, 2);		  
	
  wxStaticBoxSizer* sizer_dvdauthor_location = new wxStaticBoxSizer(new wxStaticBox(this, -1, wxT("dvdauthor Location:")), wxHORIZONTAL);
  sizer_dvdauthor_location->Add(text_ctrl_dvdauthor_location, 1, 0, 0);
  sizer_dvdauthor_location->Add(button_dvdauthor_location_browse, 0, 0, 0);
  sizer_1->Add(sizer_dvdauthor_location, 0, wxALL|wxEXPAND, 2);

	wxStaticBoxSizer* sizer_button_font = new wxStaticBoxSizer(new wxStaticBox(this, -1, wxT("Font for Button Text")), wxHORIZONTAL);
  sizer_button_font->Add(text_ctrl_button_text_font, 1, 0, 0);
  sizer_button_font->Add(button_button_text_font_change, 0, 0, 0);
  sizer_1->Add(sizer_button_font, 0, wxALL|wxEXPAND, 2);
  
	if (setupReason.length() > 0) {
		wxStaticBoxSizer* sizer_setup_reason = new wxStaticBoxSizer(new wxStaticBox(this, -1, wxT("Setup Reason")), wxHORIZONTAL);
		sizer_setup_reason->Add(new wxTextCtrl(this, -1, setupReason, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE), 1, wxEXPAND, 0);
		sizer_1->Add(sizer_setup_reason, 0, wxALL|wxEXPAND, 2);
	}

	SetAutoLayout(true);
  SetSizer(sizer_1);
  sizer_1->Fit(this);
  sizer_1->SetSizeHints(this);
  Layout();
}

void SetupPage::OnButtondvdauthorLocationBrowse(wxCommandEvent &event)
{
	wxString startDir = text_ctrl_dvdauthor_location->GetValue();
  
	wxDirDialog dirDialog(this, wxT("Choose a folder"), startDir);
  if (dirDialog.ShowModal() == wxID_OK)
  {
    wxString dir = dirDialog.GetPath();
		text_ctrl_dvdauthor_location->SetValue(dir);
	}
}

void SetupPage::OnButtonFontChange(wxCommandEvent &event)
{
	wxFont buttonFont = GetFontConfig(g_configuration, wxT("Button Text"), true, this);
	wxTextAttr buttonTextAttr(*wxBLACK, *wxWHITE, buttonFont);
	text_ctrl_button_text_font->SetStyle(0, text_ctrl_button_text_font->GetLastPosition(), buttonTextAttr);
}

void SetupPage::OnTextdvdauthorLocationChange(wxCommandEvent &event)
{
	g_configuration->Write(wxT("dvdauthor Location"), text_ctrl_dvdauthor_location->GetValue());
}

void HTMLScrolledWindow::OnMouseLeftDoubleClick(wxMouseEvent &event)
{
	int i;
	i = 0;
	//Render();
}

HTMLPage::HTMLPage(wxWizard *parent)
	: wxWizardPageSimple(parent, NULL, NULL, wxBitmap(LoadImageFromMemory(html_page_png, html_page_png_size)))
{
    window_html_pane_settings = new wxPanel(this, -1);
    window_html_pane_preview = new wxPanel(this, -1);
    
		label_preview = new wxStaticText(window_html_pane_preview, -1, wxT("Preview:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    
		preview_window = new HTMLScrolledWindow;		
		preview_window->Create(window_html_pane_preview, -1, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
		preview_window->EnableScrolling(true, true);		

		bitmap_preview = new wxStaticBitmap(preview_window, -1, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE|wxNO_3D);
    text_ctrl_html_file = new wxTextCtrl(window_html_pane_settings, -1, g_configuration->Read(wxT("Last Source HTML File"), wxT("")));
    button_html_file_browse = new wxButton(window_html_pane_settings, HTMLPage_ButtonBrowse, wxT("..."));
		
		button_html_file_render = new wxButton(window_html_pane_preview, HTMLPage_ButtonRender, wxT("Render"));

		spin_ctrl_render_width = new wxSpinCtrl(window_html_pane_settings, -1, wxString::Format(wxT("%i"), g_configuration->Read(wxT("HTML Render Width"), 800)), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 320, 2048);

		spin_ctrl_render_depth = new wxSpinCtrl(window_html_pane_settings, -1, wxString::Format(wxT("%i"), g_configuration->Read(wxT("HTML Render Depth"), (long)0)), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 16);


		wxBoxSizer* sizer_base2 = new wxBoxSizer(wxVERTICAL);
		
    sizer_base2->Add(bitmap_preview, 1, wxEXPAND, 0);
    preview_window->SetAutoLayout(true);
    preview_window->SetSizer(sizer_base2);
    sizer_base2->Fit(preview_window);
    sizer_base2->SetSizeHints(preview_window);
    preview_window->Layout();

		
    wxBoxSizer* sizer_base = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_settings = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* sizer_settings_render = new wxBoxSizer(wxHORIZONTAL);
    //wxBoxSizer* sizer_html_file = new wxBoxSizer(wxVERTICAL);
    //wxBoxSizer* sizer_html_file_text = new wxBoxSizer(wxHORIZONTAL);
		wxStaticBoxSizer* sizer_html_file = new wxStaticBoxSizer(new wxStaticBox(window_html_pane_settings, -1, wxT("Source HTML File:")), wxHORIZONTAL);
		wxStaticBoxSizer* sizer_render_width = new wxStaticBoxSizer(new wxStaticBox(window_html_pane_settings, -1, wxT("Render Width:")), wxHORIZONTAL);
		wxStaticBoxSizer* sizer_render_depth = new wxStaticBoxSizer(new wxStaticBox(window_html_pane_settings, -1, wxT("Render Depth:")), wxHORIZONTAL);
    wxBoxSizer* sizer_preview = new wxBoxSizer(wxVERTICAL);  
		sizer_preview->Add(label_preview, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    sizer_preview->Add(preview_window, 1, wxALL|wxEXPAND, 2);
		sizer_preview->Add(button_html_file_render, 0, wxALL|wxEXPAND, 1);
    window_html_pane_preview->SetAutoLayout(true);
    window_html_pane_preview->SetSizer(sizer_preview);
    sizer_preview->Fit(window_html_pane_preview);
    sizer_preview->SetSizeHints(window_html_pane_preview);
		
    sizer_html_file->Add(text_ctrl_html_file, 1, wxEXPAND, 0);
    sizer_html_file->Add(button_html_file_browse, 0, 0, 0);		
    sizer_settings->Add(sizer_html_file, 0, wxEXPAND|wxALL, 1);
		
		sizer_render_width->Add(spin_ctrl_render_width, 0, 0, 0);
		sizer_render_width->Add(new wxStaticText(window_html_pane_settings, -1, wxT("px")), 0, wxALIGN_BOTTOM|wxLEFT, 1);				
		sizer_settings_render->Add(sizer_render_width, 1, wxALL, 1);		

		sizer_render_depth->Add(spin_ctrl_render_depth, 0, 0, 0);
		sizer_settings_render->Add(sizer_render_depth, 0, wxALL, 1);		

		//sizer_settings_render->Add(button_html_file_render, 0, wxALIGN_RIGHT|wxALIGN_BOTTOM|wxALL, 3);

		sizer_settings->Add(sizer_settings_render, 0, wxEXPAND, 0);
		
    window_html_pane_settings->SetAutoLayout(true);
    window_html_pane_settings->SetSizer(sizer_settings);
    sizer_settings->Fit(window_html_pane_settings);
    sizer_settings->SetSizeHints(window_html_pane_settings);
		sizer_base->Add(window_html_pane_preview, 1, wxEXPAND, 0);
    sizer_base->Add(window_html_pane_settings, 0, wxEXPAND, 0);
    SetAutoLayout(true);
    SetSizer(sizer_base);
    sizer_base->Fit(this);
    sizer_base->SetSizeHints(this);
    Layout();

		// Create a memory DC
		wxMemoryDC bitmapDC;
		wxBitmap bitmapHTML;
		bitmapHTML.Create(500, 500);
		bitmapDC.SelectObject(bitmapHTML);
		
		// We can now draw into the memory DC...
		bitmapDC.SetPen(*wxTRANSPARENT_PEN);
		bitmapDC.SetBrush(*wxWHITE_BRUSH);
		bitmapDC.DrawRectangle(0, 0, bitmapHTML.GetWidth(), bitmapHTML.GetHeight());

		// Release the bitmap
		bitmapDC.SelectObject(wxNullBitmap);

		bitmap_preview->SetBitmap(bitmapHTML);

		preview_window->SetScrollbars(1, 1, 10, 10);
}

HTMLPage::~HTMLPage()
{
	g_configuration->Write(wxT("Last Source HTML File"), text_ctrl_html_file->GetValue());
	g_configuration->Write(wxT("HTML Render Width"), spin_ctrl_render_width->GetValue());
	g_configuration->Write(wxT("HTML Render Depth"), spin_ctrl_render_depth->GetValue());
}

void HTMLPage::OnButtonBrowse(wxCommandEvent &event)
{
	wxString supportedTypes = _T("HTML Files (*.htm;*.html)|*.htm;*.html|");
	wxFileDialog dialog(this, _("Open Input File"), _T(""), _T(""), supportedTypes, wxOPEN);

	if (dialog.ShowModal() == wxID_OK) {
		wxString filename;
		filename = dialog.GetPath();
		text_ctrl_html_file->SetValue(filename);
	}
}

void HTMLPage::OnButtonRender(wxCommandEvent &event)
{
	Render();
}

void HTMLPage::Render()
{
	if (wxFileExists(text_ctrl_html_file->GetValue())) {
		wxBusyInfo wait(wxT("Please wait, rendering..."), this);

		wxString filename = text_ctrl_html_file->GetValue();
		int width = spin_ctrl_render_width->GetValue();
		int depth = spin_ctrl_render_depth->GetValue();

		m_Menu.Generate(filename, width, depth);

		wxBitmap &htmlBitmap = m_Menu.GetRootPage().title;
		preview_window->SetVirtualSize(htmlBitmap.GetWidth(), htmlBitmap.GetHeight());		
		preview_window->SetScrollbars(1, 1, htmlBitmap.GetWidth(), htmlBitmap.GetHeight());
		preview_window->Layout();
		bitmap_preview->SetBitmap(htmlBitmap);
		

	} else {
		// Create a memory DC
		wxMemoryDC bitmapDC;
		wxBitmap bitmapHTML;
		bitmapHTML.Create(bitmap_preview->GetClientSize().GetWidth(), bitmap_preview->GetClientSize().GetHeight());
		bitmapDC.SelectObject(bitmapHTML);
		
		// We can now draw into the memory DC...
		bitmapDC.SetPen(*wxWHITE_PEN);
		bitmapDC.SetBrush(*wxWHITE_BRUSH);
		bitmapDC.DrawRectangle(0, 0, bitmapHTML.GetWidth(), bitmapHTML.GetHeight());

		bitmapDC.SetBrush(*wxBLACK_BRUSH);

		// Draw a "No HTML Page Loaded"
		wxCoord textWidth = 0;
		wxCoord textHeight = 0;
		bitmapDC.GetTextExtent(wxT("No HTML Page Loaded"), &textWidth, &textHeight);
		bitmapDC.DrawText(wxT("No HTML Page Loaded"), (bitmapHTML.GetWidth() - textWidth) / 2, (bitmapHTML.GetHeight() - textHeight) / 2);

		// Release the bitmap
		bitmapDC.SelectObject(wxNullBitmap);

		bitmap_preview->SetBitmap(bitmapHTML);
	}
}

void HTMLPage::OnMouseWheel(wxMouseEvent &event)
{
	int x = event.GetX();
	int y = event.GetY();
	wxRect scrollWindowRect = preview_window->wxWindow::GetRect();
	
	scrollWindowRect.Offset(this->ClientToScreen(this->GetPosition()));

	if (scrollWindowRect.Inside(x, y)) {
		float rotation = (float)event.GetWheelRotation() / (float)event.GetWheelDelta() * -1;
		if (event.IsPageScroll()) {
			preview_window->ScrollPages(event.GetWheelRotation()*rotation);
		} else {
			preview_window->ScrollLines(event.GetLinesPerAction()*rotation);
		}		
	}
}

DVDPage::DVDPage(wxWizard *parent)
	: wxWizardPageSimple(parent, NULL, NULL, wxBitmap(LoadImageFromMemory(dvd_page_png, dvd_page_png_size)))
{
    text_ctrl_dvd_output = new wxTextCtrl(this, -1, g_configuration->Read(wxT("Last DVD Output Dir"), wxT("")));
    button_dvd_output_browse = new wxButton(this, DVDPage_ButtonBrowse, wxT("..."));
    button_dvd_write = new wxButton(this, DVDPage_ButtonWrite, wxT("Write"));
    
    // begin wxGlade: DVDFrame::do_layout
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sizer_dvd_output = new wxStaticBoxSizer(new wxStaticBox(this, -1, wxT("Output DVD Files to:")), wxHORIZONTAL);
    sizer_dvd_output->Add(text_ctrl_dvd_output, 1, wxEXPAND, 0);
    sizer_dvd_output->Add(button_dvd_output_browse, 0, 0, 0);
    sizer_2->Add(sizer_dvd_output, 0, wxALL|wxEXPAND, 3);
    sizer_2->Add(button_dvd_write, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
    SetAutoLayout(true);
    SetSizer(sizer_2);
    sizer_2->Fit(this);
    sizer_2->SetSizeHints(this);
    Layout();
}

DVDPage::~DVDPage()
{
	g_configuration->Write(wxT("Last DVD Output Dir"), text_ctrl_dvd_output->GetValue());
}

void DVDPage::OnButtonBrowse(wxCommandEvent &event)
{
	wxString startDir = text_ctrl_dvd_output->GetValue();
	if ((startDir.length() == 0) && (wxDynamicCast(this->GetPrev(), HTMLPage) != NULL)) {
		HTMLPage *html = wxDynamicCast(this->GetPrev(), HTMLPage);
		startDir = wxFileName(html->GetPath()).GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);
	}
  
	wxDirDialog dirDialog(this, wxT("Choose a folder"), startDir, wxDD_NEW_DIR_BUTTON);
  if (dirDialog.ShowModal() == wxID_OK)
  {
    wxString dir = dirDialog.GetPath();
		text_ctrl_dvd_output->SetValue(dir);
	}
}

void DVDPage::OnButtonWrite(wxCommandEvent &event)
{
	Write();
}

void DVDPage::Write()
{
	wxString dir;
	dir = text_ctrl_dvd_output->GetValue();	
	HTMLPage *html = wxDynamicCast(this->GetPrev(), HTMLPage);
	if ((dir.length() > 0) && (html != NULL)) {		
		if (!html->GetDVDMenu().IsGenerated())
			html->Render();

		wxBusyInfo wait(wxT("Please wait, writing..."), this);
		wxFileName fileDir;
		fileDir.AssignDir(dir);
		
		dvdauthorController dvd;
		dvd.SetdvdauthorFolder(g_configuration->Read(wxT("dvdauthor Location"), wxEmptyString));
		dvd.SetDestFolder(fileDir.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR));
		dvd.Write(html->GetDVDMenu(), this);
		/*DVDMenuWriter dvd;
		dvd.SetDestFolder(fileDir.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR));
		dvd.Write();*/
	}
}

FinishPage::FinishPage(wxWizard *parent)
	: wxWizardPageSimple(parent, NULL, NULL, wxBitmap(LoadImageFromMemory(finish_page_png, finish_page_png_size)))
{
  (void)new wxStaticText(this, -1, wxT("It looks like you are done!\n\n")
                                   wxT("Now you can burn the VIDEO_TS folder\n")
																	 wxT("with you favorite DVD burning program."));
}
/*
jpeg2yuv -j "..\..\..\My Videos\voyager%04d.jpg" -b 0 -Ip -f 24 |
mpeg2enc -M 0 -f 3 -b 3500 -q 9 -o out.m2v
*/

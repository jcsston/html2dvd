// dvdauthorController.cpp: implementation of the dvdauthorController class.
//
//////////////////////////////////////////////////////////////////////

#include "dvdauthorController.h"

dvdauthorControllerProcess::dvdauthorControllerProcess(dvdauthorControllerProcessData *data)
	: wxProcess(wxPROCESS_REDIRECT)
{
	m_ProcessData = data;
}

dvdauthorControllerProcess::~dvdauthorControllerProcess()
{

}

void dvdauthorControllerProcess::OnTerminate(int pid, int status)
{
	wxASSERT(m_ProcessData != NULL);

	m_ProcessData->running = false;
	
	wxInputStream *In = GetInputStream();
	wxInputStream *Err = GetErrorStream();
	
	char buffer[256];
	while (!In->Eof()) {
		memset(buffer, 0, 256);
		In->Read(buffer, 255);
		m_ProcessData->buffer += wxString(buffer, wxConvUTF8);
	}
	while (!Err->Eof()) {
		memset(buffer, 0, 256);
		Err->Read(buffer, 255);
		m_ProcessData->buffer += wxString(buffer, wxConvUTF8);
	}	

	delete this;
}

dvdauthorControllerStatusDialog::dvdauthorControllerStatusDialog(wxWindow* parent)
	: wxDialog(parent, -1, wxT("dvdauthor Status"), wxDefaultPosition, wxSize(300, 400), wxDEFAULT_DIALOG_STYLE)
{
	label_status = new wxStaticText(this, -1, wxT("dvdauthor Status"));
	text_ctrl_status = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxHSCROLL|wxTE_RICH|wxTE_READONLY);
	button_cancel = new wxButton(this, -1, wxT("Cancel"));

	wxBoxSizer* sizer_base = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizer_buttons = new wxBoxSizer(wxHORIZONTAL);
	sizer_base->Add(label_status, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	sizer_base->Add(text_ctrl_status, 1, wxALL|wxEXPAND, 5);
	sizer_buttons->Add(button_cancel, 0, 0, 0);
	sizer_base->Add(sizer_buttons, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
	SetAutoLayout(true);
	SetSizer(sizer_base);
	sizer_base->Fit(this);
	sizer_base->SetSizeHints(this);
	Layout();
}

void dvdauthorControllerStatusDialog::AddStatus(wxString status)
{
	SetSize(wxSize(500, 300));
	Center();
	text_ctrl_status->AppendText(status);
}

dvdauthorController::dvdauthorController()
{

}

dvdauthorController::~dvdauthorController()
{

}

void dvdauthorController::SetdvdauthorFolder(const wxString &_dvdauthorFolder)
{
	m_dvdauthorFolder = _dvdauthorFolder;
}

void dvdauthorController::SetDestFolder(const wxString &_destFolder)
{
	m_DestFolder = _destFolder;
}

int dvdauthorController::Write(DVDMenu &menu, wxWindow *parent)
{
	wxString exe_ext;
#ifdef WIN32
	exe_ext = wxT(".exe");
#endif
	wxString cmd;
	dvdauthorControllerProcessData processData;

	wxSetWorkingDirectory(m_dvdauthorFolder);
	// Make sure we have all the programs we need
	wxFileName jpeg2yuvExe;
	jpeg2yuvExe.Assign(m_dvdauthorFolder, wxT("jpeg2yuv")+exe_ext);	
	if (!jpeg2yuvExe.FileExists()) {
		wxLogError(wxT("jpeg2yuv not found in '%s'"), m_dvdauthorFolder.c_str());
		return -1;
	}

	wxFileName mpeg2encExe;
	mpeg2encExe.Assign(m_dvdauthorFolder, wxT("mpeg2enc")+exe_ext);	
	if (!mpeg2encExe.FileExists()) {
		wxLogError(wxT("mpeg2enc not found in '%s'"), m_dvdauthorFolder.c_str());
		return -1;
	}

	wxFileName mplexExe;
	mplexExe.Assign(m_dvdauthorFolder, wxT("mplex")+exe_ext);	
	if (!mplexExe.FileExists()) {
		wxLogError(wxT("mplex not found in '%s'"), m_dvdauthorFolder.c_str());
		return -1;
	}

	wxFileName spumuxExe;
	spumuxExe.Assign(m_dvdauthorFolder, wxT("spumux")+exe_ext);	
	if (!spumuxExe.FileExists()) {
		wxLogError(wxT("spumux not found in '%s'"), m_dvdauthorFolder.c_str());
		return -1;
	}

	wxFileName dvdauthorExe;
	dvdauthorExe.Assign(m_dvdauthorFolder, wxT("dvdauthor")+exe_ext);	
	if (!dvdauthorExe.FileExists()) {
		wxLogError(wxT("dvdauthor not found in '%s'"), m_dvdauthorFolder.c_str());
		return -1;
	}
	
	wxFile scriptFile(wxT("html2dvd-script.bat"), wxFile::write);
	wxFileOutputStream scriptFileOut(scriptFile);
	wxTextOutputStream scriptTextFileOut(scriptFileOut);

	// First, Convert the images to MPEG-2 streams
	DVDMenuPageArray &array = menu.GetArray();
	wxString tempBase;
	wxString imageFilename;
	wxString maskImageFilename;
	wxImage mask;
	wxString highlightImageFilename;
	wxString mpeg2encFilename;
	wxString mplexMpegFilename;
	wxString spumuxXmlFilename;
	wxString spumuxMpegFilename;
	wxString dvdauthorXmlFilename;
	size_t i;
	size_t b;
	
	for (i = 0; i < array.GetCount(); i++) {
		DVDMenuPage &page = *array[i];
		tempBase = wxFileName(m_DestFolder, wxString::Format(wxT("html2dvdtemp-%02i-"), i)).GetFullPath();
				
		wxCHECK_MSG(page.title.Ok(), -1, wxT("Invalid title bitmap image!"));
		
		imageFilename = tempBase + wxT("image.jpg");
		page.title.ConvertToImage().SaveFile(imageFilename);

		cmd = jpeg2yuvExe.GetFullName();	
		cmd += wxT(" -n 1 -I t -L 1 -f 29.97 -j ");
		cmd += wxT("\"") + imageFilename + wxT("\"");
		
		mpeg2encFilename = tempBase + wxT("menu-mpeg2enc.m2v");

		cmd += wxT(" | ");
		cmd += mpeg2encExe.GetFullName();	
		cmd += wxT(" -b 8000 -f 8 -o ");
		cmd += wxT("\"") + mpeg2encFilename + wxT("\"");
		
		scriptTextFileOut << cmd << wxT("\n");
		
		// Build the mplex command-line
		cmd = mplexExe.GetFullName();
		
		mplexMpegFilename = tempBase + wxT("menu-mplex.m2v");

		cmd += wxT(" -f 8 -o ");
		cmd += wxT("\"") + mplexMpegFilename + wxT("\"");
		cmd += wxT(" ");
		cmd += wxT("\"") + mpeg2encFilename + wxT("\"");
		scriptTextFileOut << cmd << wxT("\n");

		// Build the spumux command-line
		cmd = spumuxExe.GetFullName();
		
		spumuxXmlFilename = tempBase + wxT("menu-spumux.xml");
		spumuxMpegFilename = tempBase + wxT("menu-spumux.m2v");

		cmd += wxT(" ");
		cmd += wxT("\"") + spumuxXmlFilename + wxT("\"");	
		cmd += wxT(" < ");
		cmd += wxT("\"") + mplexMpegFilename + wxT("\"");	
		cmd += wxT(" > ");
		cmd += wxT("\"") + spumuxMpegFilename + wxT("\"");	

		scriptTextFileOut << cmd << wxT("\n");

		// Now we need to save the mask images
		wxCHECK_MSG(page.mask.Ok(), -1, wxT("Invalid mask bitmap image!"));
		
		maskImageFilename = tempBase + wxT("image-select.png");
		mask = page.GetSelectionMask();		
		mask.SaveFile(maskImageFilename);
		
		highlightImageFilename = tempBase + wxT("image-highlight.png");		
		// Replace the green color with red
		mask = page.GetSelectionMask();
		mask.SaveFile(highlightImageFilename);
		
		// Generate the spumux xml settings file	
		wxFileOutputStream spumuxXmlFileOut(spumuxXmlFilename);
		wxTextOutputStream spumuxXmlTextOut(spumuxXmlFileOut);
		spumuxXmlTextOut << wxT("<subpictures>") << wxT("\n");
		spumuxXmlTextOut << wxT("\t<stream>") << wxT("\n");
		spumuxXmlTextOut << wxT("\t\t<spu start=\"00:00:00.0\"") << wxT("\n");
		spumuxXmlTextOut << wxT("\t\t     end=\"00:00:00.0\"") << wxT("\n");
		spumuxXmlTextOut << wxT("\t\t     highlight=\"") << highlightImageFilename << wxT("\"") << wxT("\n");
		spumuxXmlTextOut << wxT("\t\t     select=\"") << maskImageFilename << wxT("\"") << wxT("\n");
		spumuxXmlTextOut << wxT("\t\t     autooutline=\"infer\"") << wxT("\n");
		spumuxXmlTextOut << wxT("\t\t     autoorder=\"rows\">") << wxT("\n");
		/*for (b = 0; b < page.buttons.GetCount(); b++) {
			DVDMenuButton &button = page.buttons[b];
									
			spumuxXmlTextOut << wxT("\t\t\t<button label=\"") << wxString::Format(wxT("menu%i_button%i"), i, b) << wxT("\"") << wxT("\n");
			spumuxXmlTextOut << wxT("\t\t\t        x0=\"") << button.x << wxT("\"") << wxT("\n");
			spumuxXmlTextOut << wxT("\t\t\t        y0=\"") << button.y << wxT("\"") << wxT("\n");
			spumuxXmlTextOut << wxT("\t\t\t        x1=\"") << button.x+button.width << wxT("\"") << wxT("\n");
			spumuxXmlTextOut << wxT("\t\t\t        y1=\"") << button.x+button.height << wxT("\" />") << wxT("\n");
		}*/
		spumuxXmlTextOut << wxT("\t\t</spu>") << wxT("\n");
		spumuxXmlTextOut << wxT("\t</stream>") << wxT("\n");
		spumuxXmlTextOut << wxT("</subpictures>") << wxT("\n");
	}

	// Build the final dvdauthor command-line
	cmd = dvdauthorExe.GetFullName();
	
	dvdauthorXmlFilename = wxFileName(m_DestFolder, wxT("temp-dvdauthor.xml")).GetFullPath();

	cmd += wxT(" -x ");
	cmd += wxT("\"") + dvdauthorXmlFilename + wxT("\"");
	scriptTextFileOut << cmd << wxT("\n");
	
	{ // These are so it will close the .xml file
	wxFileOutputStream dvdauthorXmlFileOut(dvdauthorXmlFilename);
	wxTextOutputStream dvdauthorXmlTextOut(dvdauthorXmlFileOut);

	dvdauthorXmlTextOut << wxT("<dvdauthor dest=\"");
	dvdauthorXmlTextOut << m_DestFolder;
	dvdauthorXmlTextOut << wxT("\">") << wxT("\n");
	dvdauthorXmlTextOut << wxT(" <vmgm>")												<< wxT("\n");
	dvdauthorXmlTextOut << wxT("  <menus>")											<< wxT("\n");
	dvdauthorXmlTextOut << wxT("   <pgc>")											<< wxT("\n");
	dvdauthorXmlTextOut << wxT("    <vob file=\"thx_dvd.mpg\" />") << wxT("\n");
	dvdauthorXmlTextOut << wxT("    <post>")										<< wxT("\n");
	//dvdauthorXmlTextOut << wxT("    {")													<< wxT("\n");
	//dvdauthorXmlTextOut << wxT("       g1 = 1;")								<< wxT("\n");
	dvdauthorXmlTextOut << wxT("       jump titleset 1 menu;")	<< wxT("\n");
	//dvdauthorXmlTextOut << wxT("    }")													<< wxT("\n");
	dvdauthorXmlTextOut << wxT("    </post>")										<< wxT("\n");
	dvdauthorXmlTextOut << wxT("   </pgc>")											<< wxT("\n");
	dvdauthorXmlTextOut << wxT("  </menus>")										<< wxT("\n");
	dvdauthorXmlTextOut << wxT(" </vmgm>")											<< wxT("\n");
	dvdauthorXmlTextOut																					<< wxT("\n");
  dvdauthorXmlTextOut << wxT(" <titleset>")										<< wxT("\n");
  dvdauthorXmlTextOut << wxT("  <menus>")											<< wxT("\n");

	for (i = 0; i < array.GetCount(); i++) {
		DVDMenuPage &page = *array[i];
		tempBase = wxFileName(m_DestFolder, wxString::Format(wxT("html2dvdtemp-%02i-menu-spumux.m2v"), i)).GetFullPath();
		
		dvdauthorXmlTextOut << wxT("   <pgc>")																			<< wxT("\n");
		dvdauthorXmlTextOut << wxString::Format(wxT("<!-- menu: %i --> \n"), i);
		dvdauthorXmlTextOut << wxString::Format(wxT("<!-- name: %s --> \n"), page.name.c_str());

		for (b = 0; b < page.buttons.GetCount(); b++) {
			DVDMenuButton &button = page.buttons[b];
			size_t target = menu.FindPageByIndex(button.target);
			wxASSERT(target != wxNOT_FOUND);
			target++;

			dvdauthorXmlTextOut << wxT("    <button>\n");
			dvdauthorXmlTextOut << wxString::Format(wxT("<!-- menu: %i button: %i --> \n"), i, b);
			dvdauthorXmlTextOut << wxString::Format(wxT("<!-- target: %s --> \n"), button.target.c_str());
			// name=\"") << wxString::Format(wxT("menu%i_button%i"), i, b) << wxT("\" >")																<< wxT("\n");
			dvdauthorXmlTextOut << wxT("       jump menu ") << target << wxT(";")		<< wxT("\n");
			dvdauthorXmlTextOut << wxT("    </button>")																<< wxT("\n");			
		}
		dvdauthorXmlTextOut << wxT("    <vob file=\"");
		dvdauthorXmlTextOut << tempBase;
		dvdauthorXmlTextOut << wxT("\" pause=\"inf\"/>")											<< wxT("\n");
		dvdauthorXmlTextOut << wxT("   </pgc>")																	<< wxT("\n");
	}
	
	dvdauthorXmlTextOut << wxT("  </menus>")										<< wxT("\n");

	dvdauthorXmlTextOut << wxT("<titles><pgc><vob file=\"thx_dvd.mpg\" /></pgc></titles>\n");

	dvdauthorXmlTextOut << wxT(" </titleset>")									<< wxT("\n");
	dvdauthorXmlTextOut << wxT("</dvdauthor>")									<< wxT("\n");
	}
/*
    <titles>
      <pgc>
        <vob file="matrix_dvd.mpg"/>
          <post>
            call menu;
          </post>
      </pgc>
    </titles>

    <titles>
      <pgc>
        <vob file="outtakes_dvd.mpg" />
          <post>
            call menu;
          </post>
      </pgc>
    </titles>

  </titleset>

</dvdauthor>*/

	// Remove temp files, Should I leave them?
	//scriptTextFileOut << wxT("del ") << imageFilename << wxT("\n");

	scriptTextFileOut << wxT("@pause") << wxT("\n");
	scriptFile.Close();

	wxShell(wxT("html2dvd-script.bat"));

	return 0;
}

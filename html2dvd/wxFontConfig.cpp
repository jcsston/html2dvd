// wxFontConfig.cpp: implementation of the wxFontConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "wxFontConfig.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxFont GetFontConfig(wxConfig *config, const wxString &entry, bool bAskUser, wxWindow *parent)
{
	wxFont font = *wxNORMAL_FONT;
	
	font.SetFamily(config->Read(wxT("/Fonts/")+entry+wxT("/Family"), font.GetFamily()));
	font.SetFaceName(config->Read(wxT("/Fonts/")+entry+wxT("/Face Name"), font.GetFaceName()));
	font.SetPointSize(config->Read(wxT("/Fonts/")+entry+wxT("/Point Size"), font.GetPointSize()));
	font.SetWeight(config->Read(wxT("/Fonts/")+entry+wxT("/Weight"), font.GetWeight()));
	font.SetStyle(config->Read(wxT("/Fonts/")+entry+wxT("/Style"), font.GetStyle()));
	font.SetUnderlined(config->Read(wxT("/Fonts/")+entry+wxT("/Underlined"), font.GetUnderlined()));

	if (bAskUser) {
		wxFontData data;
		data.SetInitialFont(font);
		wxFontDialog dialog(parent, data);	
		if (dialog.ShowModal() == wxID_OK) {
			data = dialog.GetFontData();
			font = data.GetChosenFont();

			config->Write(wxT("/Fonts/")+entry+wxT("/Family"), font.GetFamily());
			config->Write(wxT("/Fonts/")+entry+wxT("/Face Name"), font.GetFaceName());
			config->Write(wxT("/Fonts/")+entry+wxT("/Point Size"), font.GetPointSize());
			config->Write(wxT("/Fonts/")+entry+wxT("/Weight"), font.GetWeight());
			config->Write(wxT("/Fonts/")+entry+wxT("/Style"), font.GetStyle());
			config->Write(wxT("/Fonts/")+entry+wxT("/Underlined"), font.GetUnderlined());
		}
	}

	return font;
}

// wxFontConfig.h: interface for the wxFontConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WXFONTCONFIG_H__86E2F0E9_5B49_4997_9EBD_C9F83859F0A5__INCLUDED_)
#define AFX_WXFONTCONFIG_H__86E2F0E9_5B49_4997_9EBD_C9F83859F0A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wx/font.h>
#include <wx/fontdlg.h>
#include <wx/config.h>

wxFont GetFontConfig(wxConfig *config, const wxString &etnry, bool bAskUser = false, wxWindow *parent = NULL);

#endif // !defined(AFX_WXFONTCONFIG_H__86E2F0E9_5B49_4997_9EBD_C9F83859F0A5__INCLUDED_)

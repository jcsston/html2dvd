// wxHTMLBitmapRenderer.h: interface for the wxHTMLBitmapRenderer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WXHTMLBITMAPRENDERER_H__8C6DA4D9_DDAD_4100_A483_5A4AABAD0F4E__INCLUDED_)
#define AFX_WXHTMLBITMAPRENDERER_H__8C6DA4D9_DDAD_4100_A483_5A4AABAD0F4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wx/wx.h>
#include <wx/dc.h>
#include <wx/dcmemory.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/html/htmlwin.h>
#include <wx/html/htmlcell.h>
#include <wx/html/winpars.h>

#include "DVDMenuButton.h"

WX_DECLARE_OBJARRAY(wxHtmlLinkInfo, wxHtmlLinkInfoArray);

class wxHTMLBitmapRenderer : public wxObject  
{
public:
	wxHTMLBitmapRenderer();
	virtual ~wxHTMLBitmapRenderer();

	// Call SetWidth, SetHtmlFile, then Render
  void SetWidth(int width);
  void SetHtmlFile(const wxString& filename);

  // Sets fonts to be used when displaying HTML page. (if size null then default sizes used).
  void SetFonts(wxString normal_face, wxString fixed_face, const int *sizes = NULL);

  wxBitmap &Render();

	DVDMenuButtons &GetDVDButtons() { return m_Buttons; };

protected:		
	void ScanNestedHTMLLinks(wxHtmlCell *cell);
	DVDMenuButtons m_Buttons;
  wxMemoryDC m_DC;
	wxBitmap m_Bitmap;
	wxHtmlLinkInfoArray m_Links;
  wxHtmlWinParser *m_Parser;
  wxFileSystem *m_FS;
  wxHtmlContainerCell *m_Cells;
  int m_MaxWidth, m_Width, m_Height;
};

#endif // !defined(AFX_WXHTMLBITMAPRENDERER_H__8C6DA4D9_DDAD_4100_A483_5A4AABAD0F4E__INCLUDED_)

// wxHTMLBitmapRenderer.cpp: implementation of the wxHTMLBitmapRenderer class.
//
//////////////////////////////////////////////////////////////////////

#include "wxHTMLBitmapRenderer.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(wxHtmlLinkInfoArray);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxHTMLBitmapRenderer::wxHTMLBitmapRenderer()
{
    m_Width = m_Height = 0;
    m_Cells = NULL;
    m_Parser = new wxHtmlWinParser(NULL);
    m_FS = new wxFileSystem();
    m_Parser->SetFS(m_FS);
}

wxHTMLBitmapRenderer::~wxHTMLBitmapRenderer()
{
    if (m_Cells) delete m_Cells;
    if (m_Parser) delete m_Parser;
    if (m_FS) delete m_FS;
}

void wxHTMLBitmapRenderer::SetWidth(int width)
{
	m_Width = width;
		
	m_Bitmap.Create(m_Width, 100, 32);				
	m_DC.SelectObject(m_Bitmap);

	m_Parser->SetDC(&m_DC, 1.0);
}


void wxHTMLBitmapRenderer::SetHtmlFile(const wxString& filename)
{
	wxString html;
  if (m_Cells != NULL) 
		delete m_Cells;

	wxFile htmlFile;		
	wxFileSystem fs;
	wxFSFile *fsFile;
	fsFile = fs.OpenFile(filename);
	
	if (fsFile != NULL) {
		wxInputStream *in;
		char *buffer;
		long len;
		
		in = fsFile->GetStream();
		wxCHECK_RET(in != NULL, wxT("Failed to get wxInputStream pointer from wxFSFile."));

		len = in->GetSize();
		wxASSERT(len != (~(size_t)0));
				
		buffer = new char[len+1];
		memset(buffer, 0, len+1);

		in->Read(buffer, len);
		
		html = wxString(buffer, wxConvUTF8);
		delete [] buffer;
		buffer = NULL;	
	}

  m_FS->ChangePathTo(wxFileName(filename).GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR), true);
  m_Cells = (wxHtmlContainerCell*) m_Parser->Parse(html);
  m_Cells->SetIndent(10, wxHTML_INDENT_ALL, wxHTML_UNITS_PIXELS);
	m_Cells->SetBackgroundColour(*wxWHITE);
  m_Cells->Layout(m_Width);
}


void wxHTMLBitmapRenderer::SetFonts(wxString normal_face, wxString fixed_face, const int *sizes)
{
    m_Parser->SetFonts(normal_face, fixed_face, sizes);
    if (m_Cells != NULL) m_Cells->Layout(m_Width);
}

wxBitmap &wxHTMLBitmapRenderer::Render()
{	
  if (m_Cells == NULL) return wxNullBitmap;

	m_Height = m_Cells->GetHeight();

	m_DC.SelectObject(wxNullBitmap);
	m_Bitmap.Create(m_Width, m_Height, 32);				
	m_DC.SelectObject(m_Bitmap);
	
	m_DC.SetPen(*wxWHITE_PEN);
  m_DC.SetBrush(*wxWHITE_BRUSH);
	m_DC.DrawRectangle(0, 0, m_Width, m_Height);

  m_DC.SetClippingRegion(0, 0, m_Width, m_Height);
	wxHtmlRenderingInfo info;
  m_Cells->Draw(m_DC, 0, 0, 0, m_Height, info);
	//m_Cells->Draw(m_DC, 0, 0, 0, m_Height);
  m_DC.DestroyClippingRegion();

	m_DC.SelectObject(wxNullBitmap);

	m_Links.Clear();
	
	// Draw hyperlink mask
	ScanNestedHTMLLinks(m_Cells->GetFirstChild());
	/*wxHtmlCell* cell = m_Cells->GetFirstChild();
	wxHtmlContainerCell *pCell = dynamic_cast<wxHtmlContainerCell *>(cell);
	
	while (cell != NULL) {
		pCell = dynamic_cast<wxHtmlContainerCell *>(cell);//wxDynamicCast(cell, wxHtmlContainerCell);
		DVDMenuButton button;
		button.x = cell->GetPosX();
		button.y = cell->GetPosY();
		button.width = cell->GetWidth();
		button.height = cell->GetHeight();
		wxHtmlLinkInfo* cellLink = cell->GetLink();
		if (cellLink != NULL) {
			button.target = cellLink->GetHref();
			m_Buttons.Add(button);
		}

		cell = cell->GetNext();
	}*/

	return m_Bitmap;
}

void wxHTMLBitmapRenderer::ScanNestedHTMLLinks(wxHtmlCell *cell)
{
	while (cell != NULL) {
		DVDMenuButton button;
		button.x = cell->GetPosX();
		button.y = cell->GetPosY();
		button.width = cell->GetWidth();
		button.height = cell->GetHeight();
		wxHtmlLinkInfo* cellLink = cell->GetLink();
		if (cellLink != NULL) {
			button.target = cellLink->GetHref();
			m_Buttons.Add(button);
		}

		ScanNestedHTMLLinks(cell->GetFirstChild());

		cell = cell->GetNext();
	}
}

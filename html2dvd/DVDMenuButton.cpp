// DVDMenuButton.cpp: implementation of the DVDMenuButton class.
//
//////////////////////////////////////////////////////////////////////

#include "DVDMenuButton.h"

#include "wxHTMLBitmapRenderer.h"
#include "wxImageResampler.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(DVDMenuButtons);
WX_DEFINE_OBJARRAY(DVDMenuPages);

extern wxConfig *g_configuration;

int DVDMenuButtonCompareFunc(DVDMenuButton **_first, DVDMenuButton **_second)
{
	DVDMenuButton *first = *_first;
	DVDMenuButton *second = *_second;

	int f = (first->x + first->width) + ((first->x + first->width) * first->y);
	int s = (second->x + second->width) + ((second->x + second->width) * second->y);
	
	if (f > s)
		return -1;
	
	if (f < s)
		return -1;
	
	//if (f == s)
	return 0;
}

bool IsLineBlank(const wxImage &image, long line)
{
	long blankPixels = 0;
	unsigned char r, g, b;
	for (long w = 0; w < image.GetWidth(); w++) {
		r = image.GetRed(w, line);
		g = image.GetGreen(w, line);
		b = image.GetBlue(w, line);
		if (r == 255 && g == 255 && b == 255)
			blankPixels++;
	}
	
	if (blankPixels >= image.GetWidth()-2)
		return true;

	return false;
}

DVDMenuButton::DVDMenuButton()
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

DVDMenuButton::~DVDMenuButton()
{

}

DVDMenuPage::DVDMenuPage()
{
	
}

DVDMenuPage::~DVDMenuPage()
{

}

void DVDMenuPage::DrawUpButton(const wxString &href)
{
	long buttonOutlineWidth = g_configuration->Read(wxT("Button Outline Width"), 4);
	long buttonPaddingWidth = g_configuration->Read(wxT("Button Padding Width"), 60);
	long buttonPaddingHeight = g_configuration->Read(wxT("Button Padding Height"), 40);
	long buttonTextPaddingWidth = g_configuration->Read(wxT("Button Text Padding Width"), 7);
	long buttonTextPaddingHeight = g_configuration->Read(wxT("Button Text Padding Height"), 5);

	wxMemoryDC memoryDC;

	wxCHECK_RET(title.Ok(), wxT("Invalid title bitmap."));

	memoryDC.SelectObject(title);
	
	// Draw the up button
	memoryDC.SetPen(wxPen(*wxBLUE, buttonOutlineWidth, wxSOLID));
	memoryDC.SetBrush(wxBrush(*wxWHITE, wxSOLID));
	memoryDC.SetFont(GetFontConfig(g_configuration, wxT("Button Text")));
	
	wxCoord buttonWidth = 0;
	wxCoord buttonHeight = 0;
	wxCoord textWidth = 0;
	wxCoord textHeight = 0;
	
	memoryDC.GetTextExtent(wxT("Down"), &textWidth, &textHeight);
	
	buttonWidth = textWidth + buttonTextPaddingWidth * 2;
	buttonHeight = textHeight + buttonTextPaddingHeight * 2;
	
	memoryDC.GetTextExtent(wxT("Up"), &textWidth, &textHeight);

	int x1, y1;
	x1 = (720-buttonPaddingWidth) - buttonWidth;
	y1 = buttonPaddingHeight;

	memoryDC.DrawRectangle(x1, y1, buttonWidth, buttonHeight);

	memoryDC.DrawText(wxT("Up"), x1+(buttonWidth - textWidth) / 2, y1+(buttonHeight - textHeight) / 2);
						 	
	memoryDC.SelectObject(wxNullBitmap);

	// Draw the button mask		
	if (!mask.Ok()) {
		// A New Mask
		mask.Create(720, 480, 32);

		memoryDC.SelectObject(mask);	
		
		memoryDC.SetPen(*wxWHITE_PEN);
		memoryDC.SetBrush(*wxWHITE_BRUSH);			
		memoryDC.DrawRectangle(0, 0, 720, 480);
	} else {
		// Just select it for drawing
		memoryDC.SelectObject(mask);	
	}
	
	memoryDC.SetPen(wxPen(*wxGREEN, buttonOutlineWidth, wxSOLID));
	memoryDC.SetBrush(*wxWHITE_BRUSH);				
	memoryDC.DrawRectangle(x1, y1, buttonWidth, buttonHeight);

	memoryDC.SelectObject(wxNullBitmap);

	// Add the actual button data to this page
	DVDMenuButton button;
	button.x = x1;
	button.y = y1;
	button.width = buttonWidth;
	button.height = buttonHeight;
	button.target = href;
	
	buttons.Add(button);
}

void DVDMenuPage::DrawDownButton(const wxString &href)
{
	long buttonOutlineWidth = g_configuration->Read(wxT("Button Outline Width"), 4);
	long buttonPaddingWidth = g_configuration->Read(wxT("Button Padding Width"), 60);
	long buttonPaddingHeight = g_configuration->Read(wxT("Button Padding Height"), 40);
	long buttonTextPaddingWidth = g_configuration->Read(wxT("Button Text Padding Width"), 7);
	long buttonTextPaddingHeight = g_configuration->Read(wxT("Button Text Padding Height"), 5);

	wxMemoryDC memoryDC;

	wxCHECK_RET(title.Ok(), wxT("Invalid title bitmap."));

	memoryDC.SelectObject(title);
	
	// Draw the down button
	memoryDC.SetPen(wxPen(*wxRED, buttonOutlineWidth, wxSOLID));
	memoryDC.SetBrush(wxBrush(*wxWHITE, wxSOLID));			
	memoryDC.SetFont(GetFontConfig(g_configuration, wxT("Button Text")));

	wxCoord buttonWidth = 0;
	wxCoord buttonHeight = 0;
	wxCoord textWidth = 0;
	wxCoord textHeight = 0;
	
	memoryDC.GetTextExtent(wxT("Down"), &textWidth, &textHeight);
	
	buttonWidth = textWidth + buttonTextPaddingWidth * 2;
	buttonHeight = textHeight + buttonTextPaddingHeight * 2;
	
	int x1, y1, x2, y2;
	x2 = 720-buttonPaddingWidth;
	y2 = 480-buttonPaddingHeight;
	x1 = x2 - buttonWidth;
	y1 = y2 - buttonHeight;

	memoryDC.DrawRectangle(x1, y1, buttonWidth, buttonHeight);

	memoryDC.DrawText(wxT("Down"), x1+(buttonWidth - textWidth) / 2, y1+(buttonHeight - textHeight) / 2);
						 	
	memoryDC.SelectObject(wxNullBitmap);

	// Draw the button mask		
	if (!mask.Ok()) {
		// A New Mask
		mask.Create(720, 480, 32);

		memoryDC.SelectObject(mask);	
		
		memoryDC.SetPen(*wxWHITE_PEN);
		memoryDC.SetBrush(*wxWHITE_BRUSH);			
		memoryDC.DrawRectangle(0, 0, 720, 480);
	} else {
		// Just select it for drawing
		memoryDC.SelectObject(mask);	
	}
	
	memoryDC.SetPen(wxPen(*wxGREEN, buttonOutlineWidth, wxSOLID));
	memoryDC.SetBrush(*wxWHITE_BRUSH);				
	memoryDC.DrawRectangle(x1, y1, buttonWidth, buttonHeight);

	memoryDC.SelectObject(wxNullBitmap);

	// Add the actual button data to this page
	DVDMenuButton button;
	button.x = x1;
	button.y = y1;
	button.width = buttonWidth;
	button.height = buttonHeight;
	button.target = href;
	
	buttons.Add(button);
}

wxImage DVDMenuPage::GetHighlightMask()
{
	wxImage img = mask.ConvertToImage();	
	img.SetMaskColour(255, 255, 255);	
	return img;
}

wxImage DVDMenuPage::GetSelectionMask()
{
	wxImage img = mask.ConvertToImage();	
	img.SetMaskColour(255, 255, 255);	
	img.Replace(0, 255, 0, 255, 0, 0);
	return img;
}

DVDMenu::DVDMenu()
{
	m_Generated = false;
}

DVDMenu::~DVDMenu()
{

}

void DVDMenu::Generate(const wxString &htmlFilename, int width, int depthLimit)
{
	int currentDepth = 0;
	m_Root = NestedGenerate(htmlFilename, width, currentDepth, depthLimit);

	// Update Array
	m_Array.Empty();
	UpdateArray(m_Root);
	
	m_Generated = true;
}

DVDMenuPage *DVDMenu::FindPage(const wxString &href)
{
	size_t i = FindPageByIndex(href);
	if (i != wxNOT_FOUND)
		return m_Array[i];

	return NULL;
}

size_t DVDMenu::FindPageByIndex(const wxString &href)
{
	for (size_t i = 0; i < m_Array.GetCount(); i++) {
		DVDMenuPage *page = m_Array[i];
		if (page->name == href)
			return i;
	}

	return static_cast<size_t>(wxNOT_FOUND);
}

void DVDMenu::UpdateArray(DVDMenuPage &page)
{
	m_Array.Add(&page);
	for (size_t c = 0; c < page.children.GetCount(); c++) {
		DVDMenuPage &child = page.children[c];
		UpdateArray(child);
	}	
}

DVDMenuPage DVDMenu::NestedGenerate(const wxString &htmlFilename, int width, int depth, int depthLimit)
{
	DVDMenuPage page;

	wxHTMLBitmapRenderer htmlRenderer;
#ifdef _DEBUG0
	wxDialog *dialog = new wxDialog(NULL, -1, htmlFilename, wxDefaultPosition, wxSize(500, 500));
	wxHtmlWindow *window = new wxHtmlWindow(dialog, -1, wxDefaultPosition, wxSize(500, 500));
	window->LoadFile(htmlFilename);
	dialog->Show(TRUE);
#endif
	htmlRenderer.SetWidth(width);
	htmlRenderer.SetHtmlFile(htmlFilename);
	wxBitmap &htmlBitmap = htmlRenderer.Render();

	//wxScreenDC screen;
	//screen.DrawBitmap(htmlBitmap, 0, 0);
		
	if (htmlBitmap.GetWidth() > 720) {
		wxImage htmlImage = htmlBitmap;
		wxSize targetSize = SmartResize(wxSize(htmlImage.GetWidth(), htmlImage.GetHeight()), wxSize(720, 0x8000));

		wxImageResampler::Bicubic(htmlImage, targetSize.GetWidth(), targetSize.GetHeight());
		//htmlImage.Rescale(targetSize.GetWidth(), targetSize.GetHeight());

		htmlBitmap = htmlImage.ConvertToBitmap();
	}

	page.name = htmlFilename;
	page.title = htmlBitmap;

	WX_APPEND_ARRAY(page.buttons, htmlRenderer.GetDVDButtons());
	page.buttons.Sort(DVDMenuButtonCompareFunc);

	depth++;

	if ((page.buttons.GetCount() > 0) && (depthLimit <= depth)) {
		// We have some links on this page,
		for (size_t b = 0; b < page.buttons.GetCount(); b++) {
			DVDMenuButton &button = page.buttons[b];
			
			DVDMenuPage nestedPage = NestedGenerate(button.target, width, depth, depthLimit);
			page.children.Add(nestedPage);
		}
	}

	if (page.title.GetHeight() > 480) {
		// We need to split this page
		long heightLeft = page.title.GetHeight() - 460;		
		long top = 0;
		long topLast = 0;
		int s = 0;
		
		wxImage titleImage = page.title.ConvertToImage();
		while (heightLeft > 0) {
			DVDMenuPage splitPage;
			splitPage.name = htmlFilename + wxString::Format(wxT("#splitpage%i"), s);

			if (s == 0)
				page.DrawDownButton(splitPage.name);

			// Split the bitmap
			wxMemoryDC memoryDC;

			splitPage.title.Create(720, 480, 32);							
			memoryDC.SelectObject(splitPage.title);
			memoryDC.SetBrush(*wxWHITE_BRUSH);
			memoryDC.DrawRectangle(0, 0, 720, 480);
			
			// Move our top position 480px
			top += 480;

			// If we cannot find a clean cut spot in the previous half page...
			for (long h = top; h > (topLast+(480/2)); h--) {
				if (IsLineBlank(titleImage, h)) {
					top = h;
					break;
				}
			}
			top -= 20; // we have a 20px overlap
			
			memoryDC.DrawBitmap(titleImage.ConvertToBitmap(), 0, top*-1);
			
			memoryDC.SelectObject(wxNullBitmap);
		
			if (s != 0) {
				splitPage.DrawUpButton(htmlFilename + wxString::Format(wxT("#splitpage%i"), s-1));			
			} else {
				splitPage.DrawUpButton(htmlFilename);
			}
			if (heightLeft > 480)
				splitPage.DrawDownButton(htmlFilename + wxString::Format(wxT("#splitpage%i"), s+1));

			page.children.Add(splitPage);
			
			heightLeft -= (top - topLast);
			topLast = top;
			s++;
		}

#ifdef _DEBUG
		page.title.ConvertToImage().SaveFile(wxString::Format(wxT("page-title.jpg"), 0));
#endif

		// Don't forget to 'crop' the first image
		wxMemoryDC memDC;
		wxBitmap memBitmap;

		memBitmap.Create(720, 480, 32);							
		memDC.SelectObject(memBitmap);			
		memDC.DrawBitmap(page.title, 0, 0);
		memDC.SelectObject(wxNullBitmap);
		page.title = memBitmap;

#ifdef _DEBUG
		page.title.ConvertToImage().SaveFile(wxString::Format(wxT("page-title-%i.jpg"), 0));
		page.mask.ConvertToImage().SaveFile(wxString::Format(wxT("page-mask-%i.png"), 0));
		for (size_t c = 0; c < page.children.GetCount(); c++) {
			DVDMenuPage &entry = page.children[c];
			entry.title.ConvertToImage().SaveFile(wxString::Format(wxT("page-title-%i.jpg"), c+1));
			entry.mask.ConvertToImage().SaveFile(wxString::Format(wxT("page-mask-%i.png"), c+1));
		}
#endif
	}
	return page;
}

// DVDMenuButton.h: interface for the DVDMenuButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DVDMENUBUTTON_H__587CEE39_8CAE_428C_87A7_E2F081A66AA0__INCLUDED_)
#define AFX_DVDMENUBUTTON_H__587CEE39_8CAE_428C_87A7_E2F081A66AA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wx/wx.h>
#include <wx/dynarray.h>
#include <wx/dcscreen.h>
#include <wx/config.h>
#include "wxFontConfig.h"

class DVDMenuButton  
{
public:
	DVDMenuButton();
	virtual ~DVDMenuButton();

	wxString target;
	long x;
	long y;
	long width;
	long height;
};

WX_DECLARE_OBJARRAY(DVDMenuButton, DVDMenuButtons);

class DVDMenuPage;
WX_DECLARE_OBJARRAY(DVDMenuPage, DVDMenuPages);
WX_DEFINE_ARRAY(DVDMenuPage *, DVDMenuPageArray);

class DVDMenuPage
{
public:
	DVDMenuPage();
	virtual ~DVDMenuPage();

	void DrawUpButton(const wxString &href);
	void DrawDownButton(const wxString &href);
	wxImage GetHighlightMask();
	wxImage GetSelectionMask();

	wxString name;
	wxBitmap title;
	wxBitmap mask;

	DVDMenuButtons buttons;
	DVDMenuPages children;
};

class DVDMenu
{
public:
	DVDMenu();
	virtual ~DVDMenu();

	void Generate(const wxString &htmlFilename, int width = 800, int depthLimit = 0);	
	DVDMenuPage *FindPage(const wxString &href);
	size_t FindPageByIndex(const wxString &href);
	DVDMenuPage &GetRootPage() { return m_Root; };
	DVDMenuPageArray &GetArray() { return m_Array; };
	bool IsGenerated() { return m_Generated; };
	
protected:
	void UpdateArray(DVDMenuPage &page);
	DVDMenuPage NestedGenerate(const wxString &htmlFilename, int width, int depth, int depthLimit);	

	bool m_Generated;
	DVDMenuPage m_Root;
	DVDMenuPageArray m_Array;
};

int DVDMenuButtonCompareFunc(DVDMenuButton **_first, DVDMenuButton **_second);
bool IsLineBlank(const wxImage &image, long line);

#endif // !defined(AFX_DVDMENUBUTTON_H__587CEE39_8CAE_428C_87A7_E2F081A66AA0__INCLUDED_)

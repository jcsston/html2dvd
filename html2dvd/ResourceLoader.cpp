
#include "ResourceLoader.h"

#include <wx/filesys.h>
#include <wx/fs_mem.h>
#include <wx/image.h>
#include <wx/mstream.h>

#ifdef WIN32_RESOURCE_LOAD
#include <windows.h>
#include <tchar.h>

#include "resource.h"

void LoadWin32Resources()
{
	HMODULE myModule = GetModuleHandle(NULL);
	HRSRC hPNGFile;
	HGLOBAL hgPNGFile;
	void *pPNGFile;
	DWORD dwPNGFile;

	hPNGFile = FindResource(myModule, MAKEINTRESOURCE(IDR_ZIP1), _T("ZIP"));
	hgPNGFile = LoadResource(myModule, hPNGFile);
	pPNGFile = LockResource(hgPNGFile);
	dwPNGFile = SizeofResource(myModule, hPNGFile);
  wxMemoryFSHandler::AddFile(_T("resources.zip"), pPNGFile, dwPNGFile);
}
#endif // WIN32_RESOURCE_LOAD

wxInputStream &_LoadResource(wxString filename)
{
	static char buffer[1] = { 0 };
	static wxMemoryInputStream ims(buffer, 1);
	static wxFileSystem fs;
	static wxFSFile *fsFile = NULL;
	
	if (fsFile != NULL) {
		delete fsFile;
		fsFile = NULL;
	}

	fsFile = fs.OpenFile(wxT("file:resources.zip#zip:") + filename);
	
	if (fsFile == NULL)
		return ims;

	return *fsFile->GetStream();
}

wxImage LoadImageFromMemory(void *data, long size)
{
	wxMemoryInputStream memInput(data, size);
	wxImage img(memInput);

	return img;
}

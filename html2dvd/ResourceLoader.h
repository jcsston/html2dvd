
#ifndef _WIN32_RESOURCE_LOADER_H_
#define _WIN32_RESOURCE_LOADER_H_

#include <wx/url.h>

#define LOAD_RESOURCE(x) _LoadResource(wxT(x))

wxImage LoadImageFromMemory(void *data, long size);

wxInputStream &_LoadResource(wxString filename);

#ifdef WIN32_RESOURCE_LOAD
void LoadWin32Resources();
#endif // WIN32_RESOURCE_LOAD

#endif // _WIN32_RESOURCE_LOADER_H_

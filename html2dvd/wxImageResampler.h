// wxImageResampler.h: interface for the wxImageResampler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WXIMAGERESAMPLER_H__363BD1F2_1E5C_4C27_B702_D729D1B1E617__INCLUDED_)
#define AFX_WXIMAGERESAMPLER_H__363BD1F2_1E5C_4C27_B702_D729D1B1E617__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/timer.h>
#include <math.h>

class wxImageResampler  
{
public:
	wxImageResampler();
	virtual ~wxImageResampler();
	static wxImage &Bicubic(wxImage &source, int newx, int newy);

	static float b3spline(float x);
};

wxSize SmartResize(wxSize source, wxSize max);

#endif // !defined(AFX_WXIMAGERESAMPLER_H__363BD1F2_1E5C_4C27_B702_D729D1B1E617__INCLUDED_)

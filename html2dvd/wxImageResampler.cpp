// wxImageResampler.cpp: implementation of the wxImageResampler class.
//
//////////////////////////////////////////////////////////////////////

#include "wxImageResampler.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxImageResampler::wxImageResampler()
{

}

wxImageResampler::~wxImageResampler()
{

}

wxImage &wxImageResampler::Bicubic(wxImage &source, int newx, int newy)
{
	//wxCHECK_MSG(source.GetBpp() == 24, source, wxT("wxImageResampler::Bicubic requires 24bpp input images."));
	wxCHECK_MSG(source.Ok(), source, wxT("Invalid source wxImage passed to wxImageResampler::Bicubic."));

	float xScale, yScale;
	xScale = (float)source.GetWidth()  / (float)newx;
	yScale = (float)source.GetHeight() / (float)newy;
	
	wxImage target;
	target.Create(newx, newy);
  
	wxUint8 *src = source.GetData();
	long srcPos;

	// bicubic interpolation by Blake L. Carlson <blake-carlson(at)uiowa(dot)edu
	float f_x, f_y, a, b, rr, gg, bb, r1, r2;
	int   i_x, i_y, xx, yy;
	for(long y=0; y<newy; y++){
		//info.nProgress = (long)(100*y/newy);
		//if (info.nEscape) break;
		f_y = (float) y * yScale;
		i_y = (int) floor(f_y);
		a   = f_y - (float)floor(f_y);
		for(long x=0; x<newx; x++){
			f_x = (float) x * xScale;
			i_x = (int) floor(f_x);
			b   = f_x - (float)floor(f_x);

			rr = gg = bb = 0.0F;
			for(int m=-1; m<3; m++) {
				r1 = b3spline((float) m - a);
				for(int n=-1; n<3; n++) {
					r2 = b3spline(-1.0F*((float)n - b)); 
					xx = i_x+n+2;
					yy = i_y+m+2;
					if (xx<0) xx=0;
					if (yy<0) yy=0;
					if (xx>=source.GetWidth()) xx=source.GetWidth()-1;
					if (yy>=source.GetHeight()) yy=source.GetHeight()-1;
					
					srcPos = (yy * source.GetWidth() + xx) * 3;

					rr += src[srcPos] * r1 * r2;
					gg += src[srcPos+1] * r1 * r2;
					bb += src[srcPos+2] * r1 * r2;
				}
			}
			target.SetRGB(x, y, rr, gg, bb);
		}
	}

	source = target;

	return source;
}

float wxImageResampler::b3spline(float x)
{
	float a, b, c, d;

	if((x + 2.0f) <= 0.0f) a = 0.0f; else a = (float)pow((x + 2.0f), 3.0f);
	if((x + 1.0f) <= 0.0f) b = 0.0f; else b = (float)pow((x + 1.0f), 3.0f);
	if(x <= 0) c = 0.0f; else c = (float)pow(x, 3.0f);  
	if((x - 1.0f) <= 0.0f) d = 0.0f; else d = (float)pow((x - 1.0f), 3.0f);

	return (0.16666666666666666667f * (a - (4.0f * b) + (6.0f * c) - (4.0f * d)));
}

wxSize SmartResize(wxSize source, wxSize max)
{
	wxLogTrace(wxT("calls"), wxT("SmartResize(sourceX = %i, sourceY = %i, maxX = %i, maxY = %i)"), source.GetWidth(), source.GetHeight(), max.GetWidth(), max.GetHeight());

	double dAspect;

	if (((double)source.GetHeight() / max.GetHeight()) > ((double)source.GetWidth() / max.GetWidth()))
		//aspect ratio is determined by width
		dAspect = (double)max.GetHeight() / source.GetHeight();
	else
		//aspect ratio is determined by height	
		dAspect = (double)max.GetWidth() / source.GetWidth();

	wxSize finalSize;
	finalSize.SetHeight((int)(source.GetHeight() * dAspect));
	finalSize.SetWidth((int)(source.GetWidth() * dAspect));

	return finalSize;
};

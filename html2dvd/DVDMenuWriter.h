//////////////////////////////////////////////////////////////////////
//
// DVDMenuWriter.h: interface for the DVDMenuWriter class.
//
// (c) 2004 Jory Stone (jcsston@toughguy.net), All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DVDMENUWRITER_H__08E2FC55_1130_43AA_8305_3A8ABCA8F09D__INCLUDED_)
#define AFX_DVDMENUWRITER_H__08E2FC55_1130_43AA_8305_3A8ABCA8F09D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h> // needed for plain strlen
#include <wx/string.h>
#include <wx/filename.h>
#include <wx/stream.h>
#include <wx/mstream.h>
#include <wx/wfstream.h>
#include <wx/datstrm.h>

class DVDMenuWriter  
{
public:
	DVDMenuWriter();
	virtual ~DVDMenuWriter();

	void Write();	

	void SetDestFolder(const wxString &_destFolder);
protected:
	void _Write_VMGM_PGCI_UT(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut);
	void _Write_VMGM_PGC_CellPosInfoTable(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut);
	void _Write_VMGM_PGC_CellPlayInfoTable(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut);
	void _Write_BCD_PlaybackTime(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut);
	void _Write_VMGM_PGC_ProgramMap(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut);
	void _Write_VMGM_PGC_CommandTable(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut);
	void _Write_VMGM_PGC_Palette(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut);

	void _Write_FP_PGC_CommandTable(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut);
	void _Write_FP_PGC(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut);

	void _Write_TT_SRPT(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut);

	/// Pads the stream to the last DVD sector and returns a the DVD End sector, 
	off_t _SeekToEndDVDSector(wxUint8 *buffer, wxOutputStream &fOut);

	long m_FrameCount;
	wxString m_DestFolder;
};

#endif // !defined(AFX_DVDMENUWRITER_H__08E2FC55_1130_43AA_8305_3A8ABCA8F09D__INCLUDED_)

// dvdauthorController.h: interface for the dvdauthorController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DVDAUTHORCONTROLLER_H__9C28D2EA_9475_4C6C_A2E5_81A7776C0C85__INCLUDED_)
#define AFX_DVDAUTHORCONTROLLER_H__9C28D2EA_9475_4C6C_A2E5_81A7776C0C85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/filename.h>
#include <wx/process.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>

#include "DVDMenuButton.h"

struct dvdauthorControllerProcessData
{
	wxString buffer;
	bool running;
};

class dvdauthorControllerProcess : public wxProcess 
{
public:
	dvdauthorControllerProcess(dvdauthorControllerProcessData *data);
	virtual ~dvdauthorControllerProcess();

	virtual void OnTerminate(int pid, int status);

protected:
	dvdauthorControllerProcessData *m_ProcessData;
};

class dvdauthorControllerStatusDialog: public wxDialog {
public:
	dvdauthorControllerStatusDialog(wxWindow* parent);

	void AddStatus(wxString status);

protected:
	// begin wxGlade: dvdauthorControllerStatusDialog::attributes
	wxStaticText* label_status;
	wxTextCtrl* text_ctrl_status;
	wxButton* button_cancel;
	// end wxGlade
};

class dvdauthorController  
{
public:
	dvdauthorController();
	virtual ~dvdauthorController();

	void SetdvdauthorFolder(const wxString &_dvdauthorFolder);
	void SetDestFolder(const wxString &_destFolder);

	// This function creates status dialog, so knowing it's parent will help
	int Write(DVDMenu &menu, wxWindow *parent = NULL);	

protected:	
	wxString m_DestFolder;
	wxString m_dvdauthorFolder;
};

#endif // !defined(AFX_DVDAUTHORCONTROLLER_H__9C28D2EA_9475_4C6C_A2E5_81A7776C0C85__INCLUDED_)

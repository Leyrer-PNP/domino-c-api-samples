// CapiErr.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CapiErr.h"
#include "CapiErrD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCapiErrApp

BEGIN_MESSAGE_MAP(CCapiErrApp, CWinApp)
	//{{AFX_MSG_MAP(CCapiErrApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCapiErrApp construction

CCapiErrApp::CCapiErrApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCapiErrApp object

CCapiErrApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCapiErrApp initialization

BOOL CCapiErrApp::InitInstance()
{
	// Standard initialization
	CCapiErrD dlg;
	m_pMainWnd = &dlg;
    dlg.DoModal();
	
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

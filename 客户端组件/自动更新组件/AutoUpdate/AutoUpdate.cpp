// AutoUpdate.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AutoUpdate.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAutoUpdateApp

BEGIN_MESSAGE_MAP(CAutoUpdateApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAutoUpdateApp construction

CAutoUpdateApp::CAutoUpdateApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pAutoUpdateDlg = NULL;
	//_CrtSetBreakAlloc(570);
	//_CrtSetBreakAlloc(569);
}


// The one and only CAutoUpdateApp object

CAutoUpdateApp theApp;


// CAutoUpdateApp initialization

BOOL CAutoUpdateApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();
	CoInitialize(NULL);
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization

	TCHAR szModuleFile[MAX_PATH]={0};

	GetModuleFileName(AfxGetInstanceHandle(), szModuleFile, MAX_PATH);

	//设置INI文件
	PathRemoveExtension(szModuleFile);
	free((void*)m_pszProfileName);
	PathAddExtension(szModuleFile, _T(".INI"));
	m_pszProfileName = _tcsdup(szModuleFile);

	//创建窗口
	m_pAutoUpdateDlg = new CAutoUpdateDlg();
	m_pMainWnd = m_pAutoUpdateDlg;
	m_pAutoUpdateDlg->Create(IDD_AUTOUPDATE_DIALOG, NULL);

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return TRUE;
}

int CAutoUpdateApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	CoUninitialize();
	delete m_pAutoUpdateDlg;

	return CWinApp::ExitInstance();
}
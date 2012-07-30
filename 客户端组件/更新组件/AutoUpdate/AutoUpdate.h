// AutoUpdate.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "DownloadMgr.h"
#include "AutoUpdateDlg.h"

// CAutoUpdateApp:
// See Upgrade.cpp for the implementation of this class
//

class CAutoUpdateApp : public CWinApp
{
protected:
	CAutoUpdateDlg*		m_pAutoUpdateDlg;		//´°¿ÚÖ¸Õë


public:
	CAutoUpdateApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
protected:

	DECLARE_MESSAGE_MAP()
};

extern CAutoUpdateApp theApp;
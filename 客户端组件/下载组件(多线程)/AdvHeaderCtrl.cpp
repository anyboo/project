// AdvHeaderCtrl.cpp : implementation file
//
// Subclassed CHeaderCtrl sample
//
// Written by Matt Weagle (matt_weagle@hotmail.com)
// Copyright (c) 2000
// 
// Modified: 
//		11.19.2000
//		Initial release
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in this file is used in any commercial application 
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AdvHeaderCtrl.h"
#include "ProgressListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdvHeaderCtrl

CAdvHeaderCtrl::CAdvHeaderCtrl():m_nSavedImage(-1)
{
}

CAdvHeaderCtrl::~CAdvHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CAdvHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CAdvHeaderCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(HDN_ENDTRACKW, OnEndTrack)
//	ON_NOTIFY_REFLECT(HDN_BEGINTRACKW, OnBeginTrack)
	ON_NOTIFY_REFLECT(HDN_ENDTRACKA, OnEndTrack)
//	ON_NOTIFY_REFLECT(HDN_BEGINTRACKA, OnBeginTrack)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvHeaderCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
/*
	Init

	This initialization function must be called after the initial CHeaderCtrl
	has been created.  It subclasses the pHeader control passed in as a 
	parameter so that the CAdvHeaderCtrl can handle the reflected messages.
	
	Params:
		pHeader	pointer to created CHeaderCtrl

	Returns:
		BOOL	could the existing header be subclassed?
*/
/////////////////////////////////////////////////////////////////////////////
BOOL CAdvHeaderCtrl::Init(CHeaderCtrl *pHeader)
{	
	ASSERT(pHeader && pHeader->GetSafeHwnd());
	if (!SubclassWindow(pHeader->GetSafeHwnd()))
	{
		OutputDebugString(_T("Unable to subclass existing header!\n"));
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/*
	OnEndTrack
	
	Params:
		pNMHDR	pointer to NMHDR structure
		pResult	pointer to LRESULT code

	Returns:
		None
*/
/////////////////////////////////////////////////////////////////////////////
void CAdvHeaderCtrl::OnEndTrack(NMHDR * pNMHDR, LRESULT* pResult)
{
	NMHEADER *pHdr = (NMHEADER*)pNMHDR;
  ((CProgressListCtrl*)GetParent())->ResizeProg();
	*pResult = 0;
}


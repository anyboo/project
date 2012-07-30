#if !defined(AFX_PROGRESSLISTCTRL_H__BDB8B31B_5EA5_11D5_A5DD_00A0CCD221FF__INCLUDED_)
#define AFX_PROGRESSLISTCTRL_H__BDB8B31B_5EA5_11D5_A5DD_00A0CCD221FF__INCLUDED_

#include "AdvHeaderCtrl.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressListCtrl.h : header file
//
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CProgressListCtrl window

class CProgressListCtrl : public CListCtrl
{
// Construction
public:
	CProgressListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitHdr();
	void ResizeProg();
	void SetProgress(int Index, int prog);
//	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void InsertSomeItems();
	void CreateProgress(int Index, int nSubItem);
	void CreateColumns();
	virtual ~CProgressListCtrl();

	// Generated message map functions
protected:

  class CProgressEntry {
  public:
    CProgressEntry(int Index, int SubIndex);
    virtual ~CProgressEntry();
//    CProgressEntry();

//    CProgressEntry& operator=(const CProgressEntry& pe);
    int m_Index;
    int m_SubIndex;
  	CProgressCtrl m_Prog;
  };

  CMap < int, int, CProgressEntry*, CProgressEntry*& > m_ProgEntries;

  CAdvHeaderCtrl m_AdvHdr;
	//{{AFX_MSG(CProgressListCtrl)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSLISTCTRL_H__BDB8B31B_5EA5_11D5_A5DD_00A0CCD221FF__INCLUDED_)

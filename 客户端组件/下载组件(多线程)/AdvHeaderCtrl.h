#if !defined(AFX_ADVHEADERCTRL_H__9AA4C1F6_C084_46ED_975D_CC528F242A63__INCLUDED_)
#define AFX_ADVHEADERCTRL_H__9AA4C1F6_C084_46ED_975D_CC528F242A63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdvHeaderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdvHeaderCtrl window

class CAdvHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CAdvHeaderCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvHeaderCtrl)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Init(CHeaderCtrl *pHeader);
	virtual ~CAdvHeaderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdvHeaderCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	void OnEndTrack(NMHDR * pNMHDR, LRESULT* pResult);

	int	 m_nSavedImage;			// Index of saved image	
	CImageList m_cImageList;	// Image list for this header control

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVHEADERCTRL_H__9AA4C1F6_C084_46ED_975D_CC528F242A63__INCLUDED_)

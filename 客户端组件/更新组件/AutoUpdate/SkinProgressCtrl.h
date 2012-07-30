#pragma once


// The progress control data structure
typedef struct tagPRODATA {
	HWND  hwnd;
	DWORD dwStyle;
	int   iLow, iHigh;
	int   iPos;
	int   iStep;
	HFONT hfont;
	COLORREF _clrBk;
	COLORREF _clrBar;
} PRODATA, NEAR *PPRODATA;    // ppd

////////////////////////////////////////////////////////////////

// CSkinProgressCtrl

class CSkinProgressCtrl : public CProgressCtrl
{
	DECLARE_DYNAMIC(CSkinProgressCtrl)

protected:
	CBitmap		m_bmTile;
	CBitmap		m_bmBack;

public:
	CSkinProgressCtrl();
	virtual ~CSkinProgressCtrl();

protected:
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};



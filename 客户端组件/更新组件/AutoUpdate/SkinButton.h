#pragma once
#include "TransparentHelper.h"

// CSkinButton

class CSkinButton : public CButton
{
	DECLARE_DYNAMIC(CSkinButton)

protected:
	BOOL				m_bTracking;
	CBitmap				m_bmBack;
	CTransparentHelper	m_objTransparent;

public:
	CSkinButton();
	virtual ~CSkinButton();

protected:
	UINT GetTextFormat() const;

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};



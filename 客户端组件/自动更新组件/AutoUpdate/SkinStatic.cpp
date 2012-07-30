// SkinStatic.cpp : implementation file
//

#include "stdafx.h"
#include "AutoUpdate.h"
#include "SkinStatic.h"


// CSkinStatic

IMPLEMENT_DYNAMIC(CSkinStatic, CStatic)

CSkinStatic::CSkinStatic()
{
	m_crTextColor=RGB(0,0,0);
}

CSkinStatic::~CSkinStatic()
{
}


BEGIN_MESSAGE_MAP(CSkinStatic, CStatic)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CSkinStatic message handlers

void CSkinStatic::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	m_objTransparent.Install(GetSafeHwnd());
	ModifyStyle(0, SS_OWNERDRAW);
}

void CSkinStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct!=NULL);
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	m_objTransparent.TransparentBk( pDC->GetSafeHdc(), GetSafeHwnd());
	CString strText;
	GetWindowText( strText);

	CRect rtText;
	GetClientRect( &rtText);
	pDC->SetBkMode( TRANSPARENT);
	CFont *pOldFont=pDC->SelectObject( GetFont());
	COLORREF crOldColor=pDC->SetTextColor(m_crTextColor);
	pDC->DrawText( strText, &rtText, DT_LEFT|DT_SINGLELINE);
	pDC->SetTextColor(crOldColor);
	pDC->SelectObject(pOldFont);
}

BOOL CSkinStatic::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
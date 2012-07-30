// SkinButton.cpp : implementation file
//

#include "stdafx.h"
#include "AutoUpdate.h"
#include "SkinButton.h"


// CSkinButton

IMPLEMENT_DYNAMIC(CSkinButton, CButton)

CSkinButton::CSkinButton()
{
	m_bTracking = FALSE;
}

CSkinButton::~CSkinButton()
{
}


BEGIN_MESSAGE_MAP(CSkinButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()



// CSkinButton message handlers
UINT CSkinButton::GetTextFormat() const
{
	UINT nFormat = 0;
	DWORD dwButtonStyle = GetButtonStyle();
	if (dwButtonStyle & BS_LEFT)
		nFormat |= DT_LEFT;
	if (dwButtonStyle & BS_CENTER)
		nFormat |= DT_CENTER;
	if (dwButtonStyle & BS_RIGHT)
		nFormat |= DT_RIGHT;
	if (dwButtonStyle & BS_TOP)
		nFormat |= DT_TOP;
	if (dwButtonStyle & BS_VCENTER)
		nFormat |= DT_VCENTER;
	if (dwButtonStyle & BS_BOTTOM)
		nFormat |= DT_BOTTOM;
	return nFormat;
}

void CSkinButton::PreSubclassWindow()
{
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);

	m_objTransparent.Install(GetSafeHwnd());

	BITMAP bmInfo;
	m_bmBack.LoadBitmap(IDB_BUTTON_BACK);
	m_bmBack.GetBitmap(&bmInfo);
	SetWindowPos(NULL, 0, 0, bmInfo.bmWidth/4, bmInfo.bmHeight, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
}

void CSkinButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct!=NULL);
	m_objTransparent.TransparentBk( lpDrawItemStruct->hDC, GetSafeHwnd());

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem = lpDrawItemStruct->rcItem;

	CDC MemDC;
	CBitmap* pOldBitmap;
	BITMAP bmInfo;

	m_bmBack.GetBitmap(&bmInfo);
	int nWidth = bmInfo.bmWidth / 4;
	int nHeight = bmInfo.bmHeight;

	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = MemDC.SelectObject(&m_bmBack);

	if (lpDrawItemStruct->itemState & ODS_DISABLED)
	{
		pDC->TransparentBlt(0, 0, rcItem.Width(), rcItem.Height(), &MemDC, nWidth * 3, 0, nWidth, nHeight, RGB(255,0,255));
	}
	else if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		pDC->TransparentBlt(0, 0, rcItem.Width(), rcItem.Height(), &MemDC, nWidth * 2, 0, nWidth, nHeight, RGB(255,0,255));
	}
	else if (m_bTracking)
	{
		pDC->TransparentBlt(0, 0, rcItem.Width(), rcItem.Height(), &MemDC, nWidth * 1, 0, nWidth, nHeight, RGB(255,0,255));
	}
	else
	{
		pDC->TransparentBlt(0, 0, rcItem.Width(), rcItem.Height(), &MemDC, nWidth * 0, 0, nWidth, nHeight, RGB(255,0,255));
	}


	CString strText;
	GetWindowText(strText);
	if (!strText.IsEmpty())
	{
		UINT nFormat = GetTextFormat();
		int nBkMode = pDC->SetBkMode(TRANSPARENT);
		if (nFormat == 0)
		{
			nFormat = DT_CENTER|DT_SINGLELINE|DT_VCENTER;
		}

		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			rcItem.OffsetRect(1,1);
		}
		pDC->DrawText(strText, &rcItem, nFormat);
		pDC->SetBkMode(nBkMode);
	}

	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
}

BOOL CSkinButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CSkinButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof (tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
		Invalidate(FALSE);
	}

	CButton::OnMouseMove(nFlags, point);
}

LRESULT CSkinButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;
	Invalidate(FALSE);
	return 0;
}
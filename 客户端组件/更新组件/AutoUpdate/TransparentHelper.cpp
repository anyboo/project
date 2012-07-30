#include "StdAfx.h"
#include ".\transparenthelper.h"

const TCHAR g_szTransparentTag[] = _T("###tranparentctrl_tag###"); 

CTransparentHelper::CTransparentHelper(void)
{
}

CTransparentHelper::~CTransparentHelper(void)
{
}


void CTransparentHelper::TransparentBk(HDC hdc, HWND hwnd)
{
	_ASSERT(hdc);
	_ASSERT(hwnd);

	HWND hParent = ::GetParent(hwnd);

	LPARAM lP = (LPARAM)hwnd;

	LRESULT lRet = FALSE;
	if ( 0 == ::SendMessage(hParent, WM_TRANSPARENT_BK, (WPARAM)hdc, (LPARAM)hwnd))
	{
		lRet = ::SendMessage(hParent, WM_ERASEBKGND, (WPARAM)hdc, 0);
	}
}

BOOL CTransparentHelper::OnTranparentControl(HDC hdcMem, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = ( HDC)wParam;
	HWND hwnd = ( HWND)lParam;

	CRect rt;

	::GetWindowRect(hwnd,&rt);
	ScreenToClient( ::GetParent( hwnd), &rt);

	DWORD dwStyle = (DWORD)GetWindowLong( ::GetParent( hwnd), GWL_STYLE);
	int nCaption = 0, nBorder = 0;

	return ::BitBlt(hdc,0, 0, rt.Width() , rt.Height(), hdcMem, rt.left , rt.top ,SRCCOPY);
}

void CTransparentHelper::ScreenToClient(HWND hwnd, LPRECT lpRect) 
{
	_ASSERT(::IsWindow( hwnd));
	::ScreenToClient(hwnd, (LPPOINT)lpRect);
	::ScreenToClient(hwnd, ((LPPOINT)lpRect)+1);
}

BOOL CTransparentHelper::SetTransparentTag(HWND hwnd)
{
	if ( ::GetProp(hwnd, g_szTransparentTag) == NULL)
	{
		::SetProp(hwnd, g_szTransparentTag, hwnd);
		return TRUE;
	}

	return FALSE;
}

BOOL CTransparentHelper::RemoveTransparentTag(HWND hwnd)
{

	if ( ::GetProp(hwnd, g_szTransparentTag) != NULL)
	{
		::RemoveProp( hwnd, g_szTransparentTag);
		return TRUE;
	}

	return FALSE;
}

BOOL CTransparentHelper::IsWindowTransparent( HWND hwnd)
{
	if ( ::GetProp(hwnd, g_szTransparentTag) != NULL)
	{
		return TRUE;
	}

	return FALSE;
}

void CTransparentHelper::NotifyTransparentChild( HWND hwnd)
{
	HWND hChildWnd = ::GetTopWindow( hwnd);

	while (hChildWnd != NULL )
	{
		_ASSERT(::GetParent(hChildWnd) == hwnd);

		if (::IsWindowVisible(hChildWnd) && CTransparentHelper::IsWindowTransparent(hChildWnd))
		{
			if ( ::SendMessage( hChildWnd, WM_NOTIFY_TRANSPARENT, 0,0) !=  TRUE)
			{
                 InvalidateRect( hChildWnd, NULL, TRUE);
			}
		}

		hChildWnd = ::GetNextWindow(hChildWnd, GW_HWNDNEXT);
	}
}



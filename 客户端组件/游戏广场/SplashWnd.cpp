// SplashWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "SplashWnd.h"


// CSplashWnd
BOOL CSplashWnd::s_bShowSplashWnd = FALSE;
BOOL CSplashWnd::s_bTimerHide = FALSE;
CSplashWnd* CSplashWnd::s_pSplashWnd = NULL;

IMPLEMENT_DYNAMIC(CSplashWnd, CWnd)

CSplashWnd::CSplashWnd()
{

}

CSplashWnd::~CSplashWnd()
{
}


BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CSplashWnd::EnableSplashScreen( BOOL bEnable )
{
	s_bShowSplashWnd = bEnable;
}

void CSplashWnd::ShowSplashScreen( CWnd* pParentWnd, BOOL bTimerHide)
{
	//如果不要显示SplashScreen或SplashWnd对象已经被创建则返回
	if (!s_bShowSplashWnd || s_pSplashWnd != NULL)
		return;

	s_bTimerHide = bTimerHide;
	s_pSplashWnd = new CSplashWnd;

	if (!s_pSplashWnd->Create(pParentWnd))
		delete s_pSplashWnd;
	else
		s_pSplashWnd->UpdateWindow();
}

BOOL CSplashWnd::PreTranslateAppMessage( MSG* pMsg )
{
	if (s_pSplashWnd == NULL)
		return FALSE;

	if (pMsg->message == WM_KEYDOWN ||
		pMsg->message == WM_SYSKEYDOWN ||
		pMsg->message == WM_LBUTTONDOWN ||
		pMsg->message == WM_RBUTTONDOWN ||
		pMsg->message == WM_MBUTTONDOWN ||
		pMsg->message == WM_NCLBUTTONDOWN ||
		pMsg->message == WM_NCRBUTTONDOWN ||
		pMsg->message == WM_NCMBUTTONDOWN)
	{
		s_pSplashWnd->HideSplashScreen();
		return TRUE;
	}
	return FALSE;
}

BOOL CSplashWnd::Create( CWnd* pParentWnd )
{
	if (!m_bitmap.LoadBitmap(IDB_SPLASH))
		return FALSE;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	LPCTSTR pszClassName = AfxRegisterWndClass(CS_SAVEBITS,AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	return CreateEx(WS_EX_TOPMOST, pszClassName, NULL,WS_POPUP|WS_VISIBLE, 
		0, 0, bm.bmWidth, bm.bmHeight,pParentWnd->GetSafeHwnd(), NULL);
}

void CSplashWnd::HideSplashScreen()
{
	s_pSplashWnd->DestroyWindow();
	AfxGetMainWnd()->ShowWindow(SW_SHOWMAXIMIZED);
	AfxGetMainWnd()->UpdateWindow();
	AfxGetMainWnd()->SendMessage(WM_SPLASH_HIDE);
}

void CSplashWnd::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd::PostNcDestroy();
	s_pSplashWnd = NULL;
	delete this;
}


int CSplashWnd::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CenterWindow(); //窗口居中显示

	if (s_bTimerHide)
	{
		SetTimer(1, TIME_SPLASH, NULL); //设置定时器
	}
	return 0;
}

void CSplashWnd::OnPaint()
{
	CPaintDC dc(this);

	CDC MemDC;
	CBitmap* pOldBitmap;
	CRect rcClient;

	MemDC.CreateCompatibleDC(&dc);
	pOldBitmap = MemDC.SelectObject(&m_bitmap);
	GetClientRect(&rcClient);

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
}

void CSplashWnd::OnTimer( UINT nIDEvent )	
{
	
	KillTimer(1);
	HideSplashScreen();
}
// CSplashWnd message handlers




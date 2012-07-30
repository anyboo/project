#pragma once


//显示时间
#ifdef _DEBUG
#define TIME_SPLASH             0L
#else	
#define TIME_SPLASH				2000L
#endif

#define WM_SPLASH_HIDE			(WM_USER+200)

// CSplashWnd

class CSplashWnd : public CWnd
{
	DECLARE_DYNAMIC(CSplashWnd)

protected:
	static BOOL s_bShowSplashWnd; //是否要显示SplashScreen的标志量
	static BOOL s_bTimerHide;
	static CSplashWnd* s_pSplashWnd;
	
	CBitmap m_bitmap;

public:
	CSplashWnd();
	virtual ~CSplashWnd();

	static void EnableSplashScreen(BOOL bEnable = TRUE);
	static void ShowSplashScreen(CWnd* pParentWnd = NULL, BOOL bTimerHide=FALSE);
	static BOOL PreTranslateAppMessage(MSG* pMsg);
	BOOL Create(CWnd* pParentWnd = NULL);
	static void HideSplashScreen();

protected:
	virtual void PostNcDestroy();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()	
};



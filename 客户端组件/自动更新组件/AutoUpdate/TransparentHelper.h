#pragma once

#define  WM_TRANSPARENT_BK  (WM_USER + 1021)
#define  WM_NOTIFY_TRANSPARENT (WM_USER + 3221)

class CTransparentHelper
{
public:
	CTransparentHelper(void);
	virtual ~CTransparentHelper(void);


	BOOL Install( HWND hwnd)
	{ 
		m_hwnd = hwnd; 
		SetTransparentTag( hwnd);
		return TRUE;
	};

	HWND GetHwnd() { return m_hwnd ;};
	BOOL IsValid() { return m_hwnd ? TRUE : FALSE; };


	virtual LRESULT OnMsg(  UINT uMsg,  WPARAM wParam, LPARAM lParam) { return FALSE;};

	
	void TransparentBk(HDC hdc, HWND hwnd);

	//set the tag to identify the control that is to be transparent 
	static BOOL SetTransparentTag( HWND hwnd);
	static BOOL IsWindowTransparent( HWND hwnd);
	static BOOL RemoveTransparentTag( HWND hwnd);

	static BOOL OnTranparentControl( HDC hdcMem, WPARAM wParam, LPARAM lParam);

	//tool function
	static void ScreenToClient(HWND hwnd, LPRECT lpRect);
	static void NotifyTransparentChild( HWND hwnd);

private:
	HWND m_hwnd;

};

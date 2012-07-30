#pragma once

#define WM_LBUTTONUPU (WM_USER+11)

#include "SkinButton.h"
// CDownloadButton

class CDownloadButton : public CSkinButton
{
	DECLARE_DYNAMIC(CDownloadButton)

public:
	CDownloadButton(HWND hWnd );
	virtual ~CDownloadButton();

private:
	HWND m_hWndMsg;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};



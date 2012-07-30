// DownloadButton.cpp : 实现文件
//

#include "stdafx.h"
#include "Resource.h"
#include "DownloadButton.h"


// CDownloadButton

IMPLEMENT_DYNAMIC(CDownloadButton, CSkinButton)
CDownloadButton::CDownloadButton( HWND hWnd ) : m_hWndMsg( hWnd) 
{
}

CDownloadButton::~CDownloadButton()
{
}


BEGIN_MESSAGE_MAP(CDownloadButton, CSkinButton)
END_MESSAGE_MAP()



// CDownloadButton 消息处理程序


LRESULT CDownloadButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	if ( message == WM_LBUTTONUP )
	{

		UINT id;
		id = GetDlgCtrlID();
	//	m_hWnd=GetParent()->GetSafeHwnd();

		if ( ::IsWindow(m_hWndMsg) == TRUE)
		{
			::SendMessage(m_hWndMsg,WM_LBUTTONUPU,id,0);
		}
	}

	return CSkinButton::DefWindowProc(message, wParam, lParam);
}
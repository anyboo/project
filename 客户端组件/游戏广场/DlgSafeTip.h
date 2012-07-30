#ifndef SAFE_TIP_FILE
#define SAFE_TIP_FILE

#include "DlgUserManager.h"
#pragma once
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////

//自定义头像
class CDlgSafeTip : public CSkinDialogEx//, CSkinWndObject
{
	//变量定义
protected:
	
	CBrush						m_brBkground;						//背景刷子
	CSkinButton					m_btSet;
	CSkinButton					m_BtOk;
	CSkinButton					m_btCancle;

public:
	
	//控件变量
protected:


public:
	CDlgUserManage			  m_DlgUserManager1;				
	
	
	//函数定义
public:
	//构造函数
	CDlgSafeTip(CWnd* pParent = NULL);
	//虚构函数
	virtual ~CDlgSafeTip();

	//函数定义
public:
	


	//函数定义
protected:
	//交换函数
	virtual void DoDataExchange(CDataExchange* pDX);
	//初始函数
	virtual BOOL OnInitDialog();

	//辅助函数
public:
	//显示消息
	void ShowMessage(TCHAR *pMessage);

	//更新控件
	void UpdateControls();


	//消息函数
protected:
	
	//绘画函数
	afx_msg void OnPaint();
	//颜色处理
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//显示消息
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//销毁消息
	afx_msg void OnClose();

	afx_msg void OnOk();
	
	afx_msg void OnSet();

	



	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif
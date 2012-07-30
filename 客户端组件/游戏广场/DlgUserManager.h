#ifndef USER_MANAGE_FILE
#define USER_MANAGE_FILE

#include "DlgUserInfoSample.h"
#include "DlgUserInfoXX.h"
#include "DlgUserInfoPass.h"
#include "DlgUserInfoLock.h"


#pragma once
////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////

//自定义头像
class CDlgUserManage : public CSkinDialogEx1//, CSkinWndObject
{
	//变量定义
protected:
	CBrush						m_brBkground;						//背景刷子
	CPngImage					m_pngImageBack;						//背景图片	

public:
	//CTreeCtrl					m_tree;
	CSkinButton					m_btOk;		

	CSkinButton					m_btSample;		
	CSkinButton					m_btXX;		
	CSkinButton					m_btLock;		
	CSkinButton					m_btPass;		
	
	
	CDlgUserSample				m_DlgUserSample;				
	CDlgUserXX					m_DlgUserXX;					
	CDlgUserLock				m_DlgUserLock;
	CDlgUserPass				m_DlgUserPass;
	
	//函数定义
public:
	//构造函数
	CDlgUserManage();
	//虚构函数
	virtual ~CDlgUserManage();

	void InitRoomName();

	//函数定义
protected:
	//交换函数
	virtual void DoDataExchange(CDataExchange* pDX);
	//初始函数
	virtual BOOL OnInitDialog();

	//消息函数
protected:
	
	//绘画函数
	afx_msg void OnPaint();
	//颜色处理
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

//	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	
	//销毁消息
	afx_msg void OnClose();

	afx_msg void OnSample();
	afx_msg void OnXX();
	afx_msg void OnLock();
	afx_msg void OnPass();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	

	void OnClose1();
	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif
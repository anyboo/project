#pragma once

#include "Stdafx.h"
#include "Resource.h"
//////////////////////////////////////////////////////////////////////////

//兑换游戏币 
class  CDlgPlayerInfo : public CSkinDialogEx
{

protected:
	tagUserData*				 m_pUserData;

	//组件定义
public:
	CGameRankManagerHelper				m_GameRankManager;					//游戏等级
public:
	//更新用户信息
	void	UpdataUserData(tagUserData* pUserData=NULL);
	//刷新显示
	void	UpdataUserInfo();
	//基础函数
public:
	//构造函数
	CDlgPlayerInfo(CWnd* pParent = NULL, tagUserData* pUserData = NULL); 
	//析构函数
	virtual ~CDlgPlayerInfo();
protected:
	//用户状态
	LPCTSTR GetUserStatus(tagUserData* pUserData);

	//控件绑定
	virtual void DoDataExchange(CDataExchange* pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定按钮
	afx_msg void OnBnClickedOk();
	//绘画背景
	//BOOL OnEraseBkgnd(CDC * pDC);
	//绘画消息
	//afx_msg void OnPaint();
	//颜色消息
	//afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};
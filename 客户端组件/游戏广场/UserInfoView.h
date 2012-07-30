#ifndef PLAZA_USER_INFO_VIEW_HEAD_FILE
#define PLAZA_USER_INFO_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include"SuoJi.h"
//////////////////////////////////////////////////////////////////////////

//用户信息
class CUserInfoView : public CDialog
{
	friend class CPlazaViewItem;
	//状态变量
protected:
	bool							m_bShowInfo;						//显示标志
	bool							m_bCreateFlag;						//创建标志
	UINT							m_nTitleHeight;						//标题高度
    
	//资源变量
protected:
	CSkinImage						m_ImageTitleL;						//位图对象
	CSkinImage						m_ImageTitleM;						//位图对象
	CSkinImage						m_ImageTitleR;						//位图对象
    CSkinImage						m_ImageScoreNumber;					//数字视图
	CSkinImage						m_ImageScore;						//分数图片
	bool                            UpdateViewKZ;                       //
	CSkinImage						m_ImageMale;                        //男性
	CSkinImage						m_ImageFemale;                      //女性
    
	CSkinImage						m_ImageMan;						    //位图对象
	CSkinImage						m_ImageWoMan;						//位图对象

	CRect							m_rcFace;							//头像位置
	bool							m_bMouseOnFace;						//鼠标停靠
    
    
	//函数定义
public:
	//构造函数
	CUserInfoView();
	//析构函数
	virtual ~CUserInfoView();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK() { return; };
	//取消函数
	virtual VOID OnCancel() { return; };
public:
   
	//功能函数
public:
	//标题高度
	UINT GetTitleHeight();
	//更新信息
	VOID UpdateUserInfo();
	//显示信息
	VOID ShowUserInfo(bool bShowInfo);
	//更新资源
	bool UpdateSkinResource();

	//消息函数
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	void DrawNumberString(CDC * pDC, __int64 lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
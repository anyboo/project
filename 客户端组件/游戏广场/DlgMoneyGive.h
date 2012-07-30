#ifndef MONEY_GIVE_FILE
#define MONEY_GIVE_FILE

#pragma once
#include "DlgStatus.h"
////////////////////////////////////////////////////////////////////////////////////

enum enOperateMoneyGiveStatus
{
	enOperateMoneyGiveStatus_NULL,
	enOperateGiveStatus_Save,
	enOperateGiveStatus_Refresh
};

////////////////////////////////////////////////////////////////////////////////////

//自定义头像
class CDlgMoneyGive : public CDialog//, CSkinWndObject
{
	//变量定义
protected:
	CDlgStatus					m_DlgStatus;					//状态窗口
	CBrush						m_brBkground;						//背景刷子
	enOperateMoneyGiveStatus	m_enOperateStatus;					//状态变量
	bool						m_bIsOk;						//确认了
	
	LONGLONG					m_lTotalMoney;						//总钱数
	LONGLONG					m_lRoomMoney;						//房间钱数
public:
		
	//控件变量
protected:

	ITCPSocket					* m_pIClientSocket;					//网络组件
	//tagUserData					* m_pMeUserData;					//用户信息
public:
	CComboBox					m_UserSelect;
	CSkinButton					m_btSave;						//存
	CSkinButton					m_btRefresh;					//刷新
	CMyComboBox					m_chkUserID;			

public:
	//设置状态
	void SetStatus(enOperateMoneyGiveStatus enOperateMoneyGiveStatus){m_enOperateStatus = enOperateMoneyGiveStatus;}
	//函数定义
public:
	//构造函数
	CDlgMoneyGive(CWnd* pParent = NULL);
	//虚构函数
	virtual ~CDlgMoneyGive();

	//函数定义
public:
	//发送数据
	bool SendData();
	//初始化房间
	bool InitRoomName();
	//查找房间
	DWORD GetGameKind(CString m_strKindName);
	//房间钱数
	void SetMoney(LONGLONG TotalMoney,LONGLONG RoomMoney);

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
	//显示确认
	void ShowOk(LPCTSTR szTargetAccount,DWORD dwTargetUserID,LONGLONG lScore);
	//网络链接
	bool ConnectServer();
	//更新控件
	void UpdateControls();
	//销毁消息
	virtual void OnClose();
	//格式化数据
	CString LongToString(LONGLONG lScore);
	//
	afx_msg void OnBnClickedRefresh();
	//
	INT64 GetDlgItemInt64(UINT nID);

	//消息函数
protected:
	
	//绘画函数
	afx_msg void OnPaint();
	//颜色处理
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//显示消息
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnBnClickedSave();


	//密码房间
	afx_msg void OnEnChangeUser();
	//
	afx_msg void OnEnChangeNum();


	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif
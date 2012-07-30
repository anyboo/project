#ifndef MONEY_MANAGER_FILE
#define MONEY_MANAGER_FILE

#pragma once
#include "DlgStatus.h"
////////////////////////////////////////////////////////////////////////////////////

enum enOperateMoneyStatus
{
	enOperateMoneyStatus_NULL,
	enOperateStatus_Save,
	enOperateStatus_Get,
	enOperateStatus_Refresh
};

////////////////////////////////////////////////////////////////////////////////////

//自定义头像
class CDlgMoneyManager : public CDialog//, CSkinWndObject
{
	//变量定义
protected:
	CDlgStatus					m_DlgStatus;					//状态窗口
	CBrush						m_brBkground;						//背景刷子
	enOperateMoneyStatus		m_enOperateStatus;					//状态变量
	
	LONGLONG					m_lTotalMoney;						//总钱数
	LONGLONG					m_lRoomMoney;						//房间钱数
public:
		
	//控件变量
protected:

	ITCPSocket					* m_pIClientSocket;					//网络组件
	//tagUserData					* m_pMeUserData;					//用户信息
public:
	CComboBox					m_RoomSelect;
	CSkinButton					m_btSave;						//存
	CSkinButton					m_btGet;						//取
	CSkinButton					m_btRefresh;						//刷新
	CMyComboBox					m_chkRoom;

	//设置状态
	void SetStatus(enOperateMoneyStatus enOperateMoneyStatus){m_enOperateStatus = enOperateMoneyStatus;}
	//函数定义
public:
	//构造函数
	CDlgMoneyManager(CWnd* pParent = NULL);
	//虚构函数
	virtual ~CDlgMoneyManager();

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
	//销毁消息
	afx_msg void OnClose();
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
	//网络链接
	bool ConnectServer();
	//更新控件
	void UpdateControls();
	//格式化数据
	CString LongToString(LONGLONG lScore);
	//
	INT64 GetDlgItemInt64(UINT nID);


	afx_msg void OnBnClickedRefresh();

	//消息函数
protected:
	
	//绘画函数
	afx_msg void OnPaint();
	//颜色处理
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//显示消息
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedGet();

	//密码房间
	afx_msg void OnEnChangeRoom();
	//
	afx_msg void OnEnChangeNum();


	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif
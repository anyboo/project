#ifndef USER_PASS_FILE
#define USER_PASS_FILE

#pragma once
////////////////////////////////////////////////////////////////////////////////////

enum enOperateUserPassStatus
{
	enOperateUserPassStatus_NULL,
	enOperateUserPassStatus_Save,
	
};

////////////////////////////////////////////////////////////////////////////////////

//自定义头像
class CDlgUserPass : public CDialog//, CSkinWndObject
{
	//变量定义
protected:
	
	CBrush						m_brBkground;					//背景刷子
	enOperateUserPassStatus		m_enOperateStatus;					//状态变量
	CSkinButton					m_btEdit;						//编辑框
	

public:
		
	//控件变量
protected:

	ITCPSocket					* m_pIClientSocket;					//网络组件
	//tagUserData					* m_pMeUserData;					//用户信息
public:
	

	//设置状态
	void SetStatus(enOperateUserPassStatus enOperateUserPassStatus){m_enOperateStatus = enOperateUserPassStatus_NULL;}
	//函数定义
public:
	//构造函数
	CDlgUserPass(CWnd* pParent = NULL);
	//虚构函数
	virtual ~CDlgUserPass();

	//函数定义
public:
	//发送数据
	bool SendData();
	//初始化房间
	bool InitRoomName();
	

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

	afx_msg void OnOK();


	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif
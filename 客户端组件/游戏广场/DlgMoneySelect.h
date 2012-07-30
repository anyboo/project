#ifndef MONEY_SELECT_FILE
#define MONEY_SELECT_FILE

#pragma once
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////

//自定义头像
class CDlgMoneySelect : public CDialog//, CSkinWndObject
{
	//变量定义
protected:
	
	CBrush						m_brBkground;						//背景刷子

public:
	CMyComboBox					m_boxTimeSecect;
	CSkinButton					m_btEdit;
	//控件变量
protected:

	//ITCPSocket					* m_pIClientSocket;					//网络组件
	//tagUserData					* m_pMeUserData;					//用户信息
public:
	
		//是否选中
	bool IsButtonSelected(UINT uButtonID);
	//选中按钮
	void SetButtonSelected(UINT uButtonID, bool bSelected);
	
	//函数定义
public:
	//构造函数
	CDlgMoneySelect(CWnd* pParent = NULL);
	//虚构函数
	virtual ~CDlgMoneySelect();

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
	//销毁消息
	afx_msg void OnClose();

	//消息函数
protected:
	
	//绘画函数
	afx_msg void OnPaint();
	//颜色处理
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//显示消息
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);


	afx_msg void OnBnClickedEdit();

	afx_msg void OnBnClickedQuery();



	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif
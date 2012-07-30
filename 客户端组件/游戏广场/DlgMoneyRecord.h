#ifndef MONEY_RECORD_FILE
#define MONEY_RECORD_FILE

#pragma once
////////////////////////////////////////////////////////////////////////////////////

enum enOperateMoneyRecordStatus
{
	enOperateMoneyRecordStatus_NULL,
	enOperateRecordStatus_Save,
};


////////////////////////////////////////////////////////////////////////////////////

//自定义头像
class CDlgMoneyRecord : public CDialog//, CSkinWndObject
{
	//变量定义
protected:
	
	CBrush						m_brBkground;						//背景刷子
	int							nPageNum;							//显示总页数
	int							nCurrentPage;						//当前页

	CSkinListCtrl				m_RecordListCtrl;		
	enOperateMoneyRecordStatus	m_enOperateStatus;					//状态变量
public:
	int								m_nCount;						//记录数
	tagUserGiveRecord				GiveRecord[200];				//记录

public:
		
	//控件变量
protected:

	ITCPSocket					* m_pIClientSocket;					//网络组件
	//tagUserData					* m_pMeUserData;					//用户信息
public:
	
	CSkinButton					m_btFirstPage;
	CSkinButton					m_btEndPage;
	CSkinButton					m_btLastPage;
	CSkinButton					m_btNextPage;
	CSkinButton					m_btQuery;
	//函数定义
public:
	//构造函数
	CDlgMoneyRecord(CWnd* pParent = NULL);
	//虚构函数
	virtual ~CDlgMoneyRecord();

	//函数定义
public:
	//发送数据
	bool SendData();
	//初始化房间
	bool InitRoomName();

		//设置状态
	void SetStatus(enOperateMoneyRecordStatus enOperateMoneyRecordStatus){m_enOperateStatus = enOperateMoneyRecordStatus;}

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

	void InitTable();

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


	afx_msg void OnBnClickedGet();

	afx_msg void OnBnClickedFirst();
	afx_msg void OnBnClickedLast();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedEnd();



	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif
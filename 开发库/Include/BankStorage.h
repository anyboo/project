#ifndef BANKER_STORAGE_HEAD_FILE
#define BANKER_STORAGE_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

class CPageDlg : public CDialog
{
protected:
	CString m_strTitle;
	CString m_strButtonText;
	const UINT m_nIDTemplate;

	static CBrush m_brBack;

public:
	CPageDlg(UINT uIDTemplate, LPCTSTR pszTitleText, LPCTSTR pszButtonText);
	virtual ~CPageDlg();

protected:
	virtual void OnOK() { GetParent()->PostMessage(WM_COMMAND,IDOK,0); };
	virtual void OnCancel() { GetParent()->PostMessage(WM_COMMAND,IDCANCEL,0); };

public:
	virtual BOOL CreatePage(CWnd *pParent);
	virtual LPCTSTR GetTitleText() { return m_strTitle; }
	virtual LPCTSTR GetButtonText() { return m_strButtonText; }
	INT64 GetDlgItemInt64(UINT nID);

public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

protected:
	//重画函数
	afx_msg void OnPaint();
	//背景消息
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//颜色消息
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#define ALLOW_SAVE_PAGE				0x00000001L
#define ALLOW_DRAW_PAGE		        0x00000002L
#define ALLOW_TRANSFER_PAGE			0x00000004L
#define ALLOW_SECURITY_PAGE		    0x00000008L
#define ALLOW_ALL_PAGE				(ALLOW_SAVE_PAGE|ALLOW_DRAW_PAGE|ALLOW_TRANSFER_PAGE|ALLOW_SECURITY_PAGE)


//兑换金币 
class PROPERTY_MODULE_CLASS CBankStorage : public CSkinDialogEx
{
	//变量定义
public:
	tagUserData						*m_pMeUserData;					//用户信息
	LONGLONG						m_lGameGold;					//银行数目
	LONGLONG						m_lStorageGold;					//存储数目
	const BYTE						m_bGameAction;
	LONGLONG						m_lGameGoldTmp;
	LONGLONG						m_lStorageGoldTmp;

	CString							m_strInCount;


	//控件变量
protected:
	CSkinButton						m_btOK;							//确定按钮
	CSkinButton						m_btCancel;						//取消按钮
	CSkinButton                     m_btPage[4];

protected:
	CPageDlg                        * m_pPageDlg[4];
	UINT                            m_nSelPage;
	CRect                           m_rcPage;
	DWORD							m_dwOption;

	//接口变量
protected:	
	IClientKernel					* m_pIClientKernel;				//内核接口
	ITCPSocket						* m_pClientSocket;				//网络接口

	//基础函数
public:
	//构造函数
	CBankStorage(const BYTE bGameAction); 
	//析构函数
	virtual ~CBankStorage();

	//辅助函数
public:
	//创建窗口
	BOOL CreateBankWindow(CWnd *pParent, UINT nSelPage, DWORD dwOption=ALLOW_ALL_PAGE);
	//更新界面
	void UpdateBank();
	//设置按钮
	void SetWaiting(BOOL bWait);
	//设置组件
	void SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserDatas);
	//发送信息
	void SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange* pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定按钮
	afx_msg void OnBnClickedOk();
	//取消按钮
	afx_msg void OnBnClickedCancel();
	//鼠标信息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//鼠标信息
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//命令信息
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//时间消息
	afx_msg void OnTimer(UINT nIDEvent);
	//输入信息
	//afx_msg void OnEnChangeInCount();
	
	afx_msg void OnClickedPageButton(UINT nCtrlID);
	
	afx_msg void OnBnClickedInAll();
	afx_msg void OnBnClickedOutAll();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeInCount();
	afx_msg void OnEnChangeOutCount();
};

//////////////////////////////////////////////////////////////////////////
#endif
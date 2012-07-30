#ifndef LOCK_MACHINE_FILE_HEAD
#define LOCK_MACHINE_FILE_HEAD
#pragma once

class CDlgLockMachine : public CSkinDialogEx
{
	//变量定义
public:
	CString								m_strPassword;
	CString								m_strDescribe;
	bool								m_bLock;
    // CImage							    m_ImageBuffer;						//缓冲位图
	//CSkinImage                          m_ImageKuang;                   //登陆框图
	 CSkinEdit							m_edUserPassword;				//用户密码
TCHAR								m_szPassword1[PASS_LEN];			//游戏密码
	//控件变量
private:
	CSkinButton							m_btOK;
	CSkinButton							m_btCancel;
	

public:
	CDlgLockMachine(CWnd* pParent = NULL);
	virtual ~CDlgLockMachine();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
          
public:
	//重画消息
	//afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
		DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CDlgLockMachine)
protected:
	virtual void OnOK();
};

#endif
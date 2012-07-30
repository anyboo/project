#ifndef BANK_PASS_FILE_HEAD
#define BANK_PASS_FILE_HEAD
#pragma once

class CDlgBankPass : public CSkinDialogEx
{
	//变量定义
public:
	CString								m_strPassword;
	CString								m_strDescribe;
	CImage							    m_ImageBuffer;					//缓冲位图
	CSkinImage                          m_ImageKuang;                   //登陆框图
	bool								m_bIsLeave;						//是否离开注销
	TCHAR								m_szPassword1[PASS_LEN];		//游戏密码

	CPasswordControl					m_edUserPassword;				//用户密码
	CTime								m_tmLogin;						//登录银行时间
	//控件变量
private:
	CSkinButton							m_btOK;
	CSkinButton							m_btCancel;



public:
	CDlgBankPass(CWnd* pParent = NULL);
	virtual ~CDlgBankPass();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
          
public:
	//重画消息
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
		//取消消息
	afx_msg void OnCancel();
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CDlgBankPass)
protected:
	virtual void OnOK();
};

#endif
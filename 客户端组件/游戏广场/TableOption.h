#pragma once

#include "stdafx.h"
#include "Resource.h"
#include "SystemOption.h"

//设置参数
struct tagTableParameter
{
	enInviteMode						enInviteMode;					//邀请模式
	enCallMode							enCallMode;						//邀请模式
	TCHAR								szTablePass[PASS_LEN];			//桌子密码
	COLORREF							crFriend;						//好友颜色
	COLORREF							crMe;							//自己颜色
	bool								bIsTablePass;					//是否桌子密码
};


class CTableOption : public CSkinDialogEx
{
//	DECLARE_DYNAMIC(CTableOption)
public:
	tagTableParameter					m_TableParameter;				//桌子规则
	CMyComboBox							m_IpNum;						//现在的IP人数
	CServerOption						* m_pServerOption;				//房间设置
	COLORREF							m_crOwnColor;			//自己颜色
	COLORREF							m_crFriendColor;			//朋友颜色
	CSkinButton							m_btOk;
	CSkinButton							m_btCancle;	
	CSkinButton							m_btEdit;
	CSkinButton							m_btEdit1;

public:
	CTableOption(CWnd* pParent = NULL,CServerOption * pServerOption=NULL);   // standard constructor
	virtual ~CTableOption();

// Dialog Data
	enum { IDD = IDD_TABLE_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	//是否选中
	bool IsButtonSelected(UINT uButtonID);
	//选中按钮
	void SetButtonSelected(UINT uButtonID, bool bSelected);
	//保存设置
	void SaveSetting();
protected:
	//保存
	afx_msg void OnOK();

	afx_msg  void OnBnClickedEdit();

	afx_msg void OnBnClickedEdit1();
	//
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//绘画背景
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()
};

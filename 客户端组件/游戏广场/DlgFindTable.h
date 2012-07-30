#pragma once
#include "Stdafx.h"
#include "Resource.h"
// CDlgFindTable 对话框

class CDlgFindTable : public CSkinDialogEx
{

public:
	CWnd							* m_pParentWnd;						//父窗口
	WORD							m_wFindTableID;
	WORD							m_wChairID;							//

	CSkinButton						m_btFind;
	CSkinButton						m_btSitDown;

public:
	CDlgFindTable(CWnd * pParentWnd);   // 标准构造函数
	virtual ~CDlgFindTable();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	//查找桌子
	afx_msg void OnFindTable();
	//初始化函数
	virtual BOOL OnInitDialog();
	//加入
	afx_msg void OnSitDown();

	DECLARE_MESSAGE_MAP()
};

#ifndef MONEY_BANK_FILE
#define MONEY_BANK_FILE

#include "DlgMoneyManager.h"
#include "DlgMoneyGive.h"
#include "DlgMoneyRecord.h"
#include "DlgMoneyPass.h"
#include "DlgMoneySelect.h"
#include "DlgMoneyClose.h"

#pragma once
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////

//自定义头像
class CDlgMoneyBank : public CSkinDialogEx1//, CSkinWndObject
{
	//变量定义
protected:
	CBrush						m_brBkground;						//背景刷子
	CPngImage					m_pngImageBack;						//背景图片	

public:
	//CTreeCtrl					m_tree;
	CSkinButton					m_btOk;		
	CSkinButton					m_btCancle;							//关闭按钮
	CSkinButton					m_btGive;		
	CSkinButton					m_btRecord;		
	CSkinButton					m_btSave;		
	CSkinButton					m_btPass;		
	CSkinButton					m_btSelect;		
	
	CDlgMoneyManager			m_DlgMoneyManager;				//欢乐豆提取		//新加功能
	CDlgMoneyGive				m_DlgMoneyGive;					//欢乐豆赠送		//新加功能
	CDlgMoneyRecord				m_DlgMoneyRecord;
	CDlgMoneySelect				m_DlgMoneySelect;
	CDlgMoneyPass				m_DlgMoneyPass;

	CDlgMoneyClose				m_DlgMoneyClose;
	//函数定义
public:
	//构造函数
	CDlgMoneyBank(CWnd* pParent = NULL);
	//虚构函数
	virtual ~CDlgMoneyBank();

	//函数定义
protected:
	//交换函数
	virtual void DoDataExchange(CDataExchange* pDX);
	//初始函数
	virtual BOOL OnInitDialog();

	//消息函数
protected:
	
	//绘画函数
	afx_msg void OnPaint();
	//颜色处理
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

//	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	
	//销毁消息
	afx_msg void OnClose();

	afx_msg void OnGive();
	afx_msg void OnRecord();
	afx_msg void OnSave();
	afx_msg void OnPass();
	afx_msg void OnSelect();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void OnClose1();

	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif
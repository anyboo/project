#ifndef MONEY_CLOSE_FILE
#define MONEY_CLOSE_FILE

#pragma once
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////

//自定义头像
class CDlgMoneyClose : public CDialog//, CSkinWndObject
{
	//变量定义
protected:
	
	CBrush						m_brBkground;						//背景刷子

public:
	CMyComboBox					m_boxTimeSecect;
	//控件变量
protected:


public:
	
		//是否选中
	bool IsButtonSelected(UINT uButtonID);
	//选中按钮
	void SetButtonSelected(UINT uButtonID, bool bSelected);
	
	//函数定义
public:
	//构造函数
	CDlgMoneyClose(CWnd* pParent = NULL);
	//虚构函数
	virtual ~CDlgMoneyClose();

	//函数定义
public:
	
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

	afx_msg void OnOk();

	

	



	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif
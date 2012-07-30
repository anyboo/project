#pragma once

//////////////////////////////////////////////////////////////////////////



//兑换欢乐豆 
class CBankStorage1 : public CSkinDialogEx1
{
	//变量定义

	//控件变量
protected:
	CSkinButton						m_btOK;							//确定按钮
    CWebBrowser							m_BrowerAD;						//浏览窗口
    CImage							    m_ImageBuffer;						//缓冲位图
	CSkinImage                          m_ImageKuang;                   //登陆框图
	//基础函数
public:
	//构造函数
	CBankStorage1(const BYTE bGameAction); 
	//析构函数
	virtual ~CBankStorage1();
	

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange* pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
public:
	void Initm_BrowerAD();
	//消息函数
public:
	//重画消息
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
    
	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////

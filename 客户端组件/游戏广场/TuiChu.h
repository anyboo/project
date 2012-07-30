#pragma once

#include "Stdafx.h"
#include "Resource.h"
//////////////////////////////////////////////////////////////////////////

//兑换游戏币 
class  CDTuiChu : public CSkinDialogEx
{
	//变量定义
public:
    ITCPSocket					  * m_pIClientSocket;				//网络组件
	LONGLONG						m_lGameGold;					//银行数目
	LONG							m_lGameBank;					//变量数目
	LONGLONG						m_lStorageGold;					//存储数目
	LONGLONG						m_lInCount;						//输入游戏币
    DWORD                           UserID;                         //对方用户ID号
	TCHAR                           szPassword1[PASS_LEN];
	bool                            KongZhi;
	CWebBrowser							m_BrowerAD;						//浏览窗口
	//控件变量
protected:
	CSkinButton						m_btOK;							//确定按钮
	CSkinButton						m_btCancel;						//取消按钮
	CSkinButton						m_btTQ;							//提取按钮
	CSkinButton						m_btCR;							//存入按钮
	CSkinButton						m_btZZ;							//转账按钮
	CSkinButton						m_btPB;							//银行密码按钮
	bool                            tiqu;                           //提取
	bool                            cunchu;                         //存入
	bool                            zhuanzhang;                     //转账
	bool                             mima;                          //银行密码修改
    CString							TCText;				            //提取和存入比率
	CString 					    ZZText;				            //提取和存入比率
	CString 					    GGText;				            //提取和存入比率
	CString 					    BankText;				        //提取和存入比率
   
	//接口变量
protected:	
	IClientKernel					* m_pIClientKernel;				//内核接口
	ITCPSocket						* m_pClientSocket;				//网络接口
    protected:
	IPurchaseInfo					* m_pIPurchaseInfo;			    //信息接口
	//基础函数
public:
	//构造函数
	CDTuiChu(CWnd* pParent = NULL); 
	//析构函数
	virtual ~CDTuiChu();

	//辅助函数
public:
	//更新界面
	void UpdateView();
	bool ConnectServer();

	//重载函数
//确定按钮
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangOk1();
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange* pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定按钮
	//afx_msg void OnBnClickedOk();
	//提取按扭
    afx_msg void OnBnTiQuOk();
	//存入按钮 
    afx_msg void OnBnCunRuOk();
	//转账按钮 
    afx_msg void OnZhuanZhangOk();
	//银行密码
    afx_msg void OnPassBankOk();
	//鼠标信息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//鼠标信息
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//命令信息
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//时间消息
	afx_msg void OnTimer(UINT nIDEvent);
	//输入信息
	afx_msg void OnEnChangeInCount();
	afx_msg void OnPaint();
	//输入信息
	afx_msg void OnEnChangeInID();

	afx_msg void OnSize(UINT nType, int cx, int cy);

	//功能函数
public:
	///设置消息接口
	void SetPurchaseInfoSink(IPurchaseInfo *pIPurchaseInfo);

	//是否选中
	bool IsButtonSelected(UINT uButtonID);
	//选中按钮
	void SetButtonSelected(UINT uButtonID, bool bSelected);
	//设置组件
	void SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket);
	//发送信息
	void SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);
   //绘制字体
	void DrawTextString();
	DECLARE_MESSAGE_MAP()
};
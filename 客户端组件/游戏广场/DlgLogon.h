#ifndef DLG_LOGON_HEAD_FILE
#define DLG_LOGON_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//登录模式
enum enLogonMode
{
	LogonMode_Accounts,			//帐号登录
	LogonMode_UserID,			//I D 登录
};

//注册表项名
#define REG_OPTION_LOGON		TEXT("OptionLogon")
#define REG_USER_INFO			TEXT("Software\\7x78Game\\GamePlaza\\UserInfo")
#define REG_LOGON_SERVER		TEXT("Software\\7x78Game\\GamePlaza\\LogonServer")


//////////////////////////////////////////////////////////////////////////

//登录 TAB 控件
class CTabCtrlLogon : public CTabCtrl
{
	//函数定义
public:
	//构造函数
	CTabCtrlLogon();
	//析够函数
	virtual ~CTabCtrlLogon();

	//消息函数
protected:
	//重画函数	
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//用户注册
class CDlgRegister : public CSkinDialogEx
{
	friend class CDlgLogon;

	//登录信息
public:
	WORD								m_wFaceID;						//头像标识
	BYTE								m_cbGender;						//用户性别
	TCHAR								m_szSpreader[NAME_LEN];			//电子邮箱
	TCHAR								m_szAccounts[NAME_LEN];			//游戏帐号
	TCHAR								m_szPassword[PASS_LEN];			//游戏密码
	TCHAR								m_szEmail[NAME_LEN];			//电子邮箱
	TCHAR								m_szIdno[NAME_LEN];			    //身份证号
	TCHAR								m_szBankpass[PASS_LEN];			//银行密码
	TCHAR								m_szMobile[PASS_LEN];				//qq号码
//	TCHAR								m_szAnQuan[PASS_LEN];			//安全码


	//控件变量
public:
	CImageList							m_ImageList;
	CComboBoxEx							m_FaceSelect;
	CSkinButton							m_btLogon;						//登录按钮
	CSkinButton							m_btCancel;						//取消按钮
	CSkinButton							m_btCancel1;						//取消按钮
	CSkinHyperLink						m_LineMainPage;					//游戏主页
	CSkinHyperLink						m_LinePassWord;					//密码保护

    CSkinImage                          m_ImageKuang;                   //登陆框图
	 CPngImage						    m_ImageLogo;			        //进入条图片

	//位置变量
protected:
	INT									m_nFullWidth;					//全部宽度
	INT									m_nFullHeight;					//全部高度
	bool								m_bNetOption;					//网络设置
	CRect								m_rcNormalFrame;				//框架位置
    CImage							    m_ImageBuffer;						//缓冲位图

		CSkinEdit							m_edRegPassword;				//用户密码
		CSkinEdit							m_edRegPassword1;				//用户密码
		CSkinEdit							m_edRegPassword2;				//用户密码
		CSkinEdit							m_edRegPassword3;				//用户密码
		CSkinEdit							m_edRegPassword4;				//用户密码
		CSkinEdit							m_edRegPassword5;				//用户密码
		CSkinEdit							m_edRegPassword6;				//用户密码
		CMyComboBox							m_edRegPassword7;				//用户密码
		CSkinEdit							m_edRegPassword8;				//用户密码
		CSkinEdit							m_edRegPassword9;				//用户密码
		CSkinEdit							m_edRegPassword10;				//用户密码
		CSkinEdit							m_edRegPassword11;				//用户密码
	//函数定义
public:
	//构造函数
	CDlgRegister();
	//析构函数
	virtual ~CDlgRegister();

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual void OnOK();
	//取消消息
	virtual void OnCancel();
	//重画消息
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//登录对话框
class CDlgLogon : public CSkinDialogEx
{
	friend class CRoomViewItem;
	friend class CPlazaViewItem;

	//登录信息
protected:
	WORD								m_wFaceID;						//头像标识
	BYTE								m_cbGender;						//用户性别
	DWORD								m_dwUserID;						//用户 I D
	TCHAR								m_szSpreader[NAME_LEN];			//推广人名
	TCHAR								m_szAccounts[NAME_LEN];			//游戏帐号
	TCHAR								m_szPassword[PASS_LEN];			//游戏密码
	TCHAR								m_szEmail[NAME_LEN];			//电子邮箱
	TCHAR								m_szIdno[NAME_LEN];			    //身份证号
	TCHAR								m_szBankpass[PASS_LEN];			//银行密码
	TCHAR								m_szMobile[PASS_LEN];
//    TCHAR								m_szAnQuan[PASS_LEN];			//安全码

	//位置变量
protected:
	INT									m_nFullWidth;					//全部宽度
	INT									m_nFullHeight;					//全部高度
	bool								m_bNetOption;					//网络设置
	CRect								m_rcNormalFrame;				//框架位置
    CImage							    m_ImageBuffer;						//缓冲位图
	//登录模式
protected:
	enLogonMode							m_LogonMode;					//登录模式

	//辅助信息
protected:
	bool								m_bRegister;					//注册标志
	bool								m_bChangePassWord;				//改变标志
    bool                                m_bJinRu;                       //是否绘画进入跳
	//连接信息
protected:
	CString								m_strLogonServer;				//服务器地址
	bool                                LeftRight;                      //控制进入条的左右移动
	//按钮变量
public:
	CSkinButton							m_btLogon;						//登录按钮
	CSkinButton							m_btQuit;						//退出按钮
	CSkinButton							m_btCancel;						//取消按钮
	CGdiButton							m_btDelete;						//删除按钮
	CSkinButton							m_btJiZhuMiMa;					//记录密码
	CGdiButton							m_btRegister;					//注册按钮
	CSkinButton							m_btNetOption;					//网络按钮
	CSkinButton							m_btProxyTest;					//测试按钮

	CSkinButton							m_tAbsel1;						//取消按钮
	CSkinButton							m_tAbsel2;						//删除按钮

	//CSkinButton							m_btUseAccount;					//用户名登录
	//CSkinButton							m_btUseId;						//ID登录

	//控件变量
public:
	CWebBrowser							m_BrowerAD;						//浏览窗口
	CTabCtrlLogon						m_TabLogonMode;					//登录选择
	CSkinHyperLink						m_LineTiaokuan;					//游戏主页
	CSkinHyperLink						m_LinePassWord;					//密码保护
	CSkinHyperLink						m_LineGetPassWord;				//取回密码
	CSkinHyperLink						m_LineRegWeb;					//网站注册
	CSkinHyperLink						m_LineZhiNan;					//游戏指南
    //CSkinImage                          m_ImageKuang;                   //登陆框图
	//CPngImage						    m_ImageLogo;			        //进入条图片
	//CPngImage							m_ImageBack;	
	//登录背景
	CSkinImage							m_ImageBack;					//登录背景
	CBrush								m_brBkground;					//背景画刷
	CFont*								m_boxFont;						//COMBOX字体
	CPngImage							m_pngBack;						//PNG背景
	CPngImage							m_pngStatus;					//滚动状态
	UINT								m_iLoadingPos;					//百分比位置
	UINT								m_iLoaddingMax;					//最大长度
	CRect								m_rcLoading;					//加载条位置

	//编辑控件
protected:
	CSkinEdit							m_edProxyPort;					//代理端口
	CSkinEdit							m_edProxyServer;				//代理地址
	CSkinEdit							m_edProxyUserName;				//代理用户
	CPasswordControl							m_edProxyPassword;				//代理密码
	CMyComboBox							m_cmProxyServerType;			//代理类型
	CMyComboBox							m_boxUserName;					//昵称字
	CPasswordControl					m_edUserPassword;				//用户密码
    CMyComboBox							m_boxUserIP;					//昵称字
	CMyComboBox							m_boxUserID;					//昵称字
	CCheckButton						m_chkRem;						//记住密码
	CCheckButton						m_chkTiaoKuan;					//同意条款
	CGdiButton							m_btPay;						//快速充值

	//函数定义
public:
	//构造函数
	CDlgLogon();
	//析构函数
	virtual ~CDlgLogon();

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual void OnOK();
	//取消消息
	virtual void OnCancel();
   
	//功能函数
public:
	//获取服务器
	LPCTSTR GetLogonServer() { return m_strLogonServer; }
	//发送登录包
	bool SendLogonPacket(ITCPSocket * pIClientSocke);
	//登录成功处理
	bool OnLogonSuccess();

	void  HuiFu();
	//内部函数
private:
	//加载服务器
	void LoadLogonServer();
	//读取帐号
	void LoadAccountsInfo();
	//代理信息
	void LoadProxyServerInfo();
	//效验输入
	bool CheckLogonInput(bool bShowError);
	//设置模式
	void SetLogonMode(enLogonMode LogonMode);

	//辅助函数
private:
	//更新密码
	void UpdateUserPassWord(DWORD dwUserDBID);
	//代理模式
	void SwitchNetOption(bool bNetOption);
	//更新选择
	void UpdateUserComboBox(UINT uComboBoxID);
	//查找字符
	int ComboBoxFindString(CMyComboBox * pComboBox, LPCTSTR pszFindString);
	//获取代理
	void GetProxyInfo(enProxyServerType &ProxyServerType, tagProxyServerInfo &ProxyServerInfo);
	//代理判断
	bool EnableProxy() { return m_bNetOption;}

	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//消息函数
public:
	//注册帐号
	afx_msg void OnRegisterAccounts();
	//删除用户
	afx_msg void OnDeleteAccounts();
	//网络设置
	afx_msg void OnBnClickedNetOption();
	//代理测试
	afx_msg void OnBnClickedProxyTest();
	//密码改变
	afx_msg void OnEnChangePassword();
	//选择改变
	afx_msg void OnSelchangeAccounts();
	//选择改变
	afx_msg void OnSelchangeUserID();

	afx_msg void OnJiZhuPassword();
	//类型改变
	afx_msg void OnTcnSelchangeLogonType();
	//类型改变
	//afx_msg void OnTcnSelchangeLogonType1(NMHDR * pNMHDR, LRESULT * pResult);
		//类型改变
	afx_msg void OnTcnSelchangeLogonType1();
	//使用ID
	afx_msg void OnCheckUseid();
	afx_msg void OnLogonUseAccount();
	afx_msg void OnLogonUseId();
	afx_msg void OnOption();
	//背景消息
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//重画消息
	afx_msg void OnPaint();
	//定时器消息
	afx_msg void OnTimer(UINT nIDEvent);
	//调整大小
	afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif

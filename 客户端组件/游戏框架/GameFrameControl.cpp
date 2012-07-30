#include "Stdafx.h"
#include "Resource.h"
#include "GameFrameApp.h"
#include "GlobalOption.h"
#include "GameFrameControl.h"
#include ".\gameframecontrol.h"

//////////////////////////////////////////////////////////////////////////

//广告时间
#define IDI_BROWER_AD				2000								//广告定时器
#define IDI_SWITCH_USER				2001								//切换定时器
#define TIME_BROWER_AD				15000								//广告定时器
#define TIME_SWITCH_USER			15000								//切换定时器

//菜单命令
#define	IDM_UM_COPY_NAME			WM_USER+100							//拷贝名字
#define	IDM_UM_SET_CHAT				WM_USER+101							//设置聊天
#define	IDM_UM_CANECL_CHAT			WM_USER+102							//取消聊天
#define	IDM_UM_ALLOW_LOOKON			WM_USER+103							//旁观控制
#define	IDM_UM_ENJOIN_LOOKON		WM_USER+104							//旁观控制
#define	IDM_UM_KICK_TABLE_USER		WM_USER+105							//踢走用户

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameControl, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_SOUND, OnBnClickedSound)
	ON_BN_CLICKED(IDC_LOOKON, OnBnClickedLookon)
	ON_BN_CLICKED(IDC_GAME_RULE, OnBnClickedRule)
	ON_BN_CLICKED(IDC_GAME_QUIT, OnBnClickedQuit)
	ON_BN_CLICKED(IDC_GAME_OPTION, OnBnClickedOption)
	ON_BN_CLICKED(IDC_SEND_CHAT, OnBnClickedSendChat)
	ON_BN_CLICKED(IDC_EXPRESSION, OnBnClickedExpression)
	ON_BN_CLICKED(IDC_COLOR_SET, OnBnClickedColorSet)
	ON_BN_CLICKED(IDC_CLEAN_SCREEN, OnBnClickedCleanScreen)
	ON_BN_CLICKED(IDC_BUGLE,	OnBnClickedBugle)
	ON_MESSAGE(WM_HIT_EXPMESSTION,OnHitExpression)
	ON_NOTIFY(NM_RCLICK, IDC_USER_LIST, OnNMRclickUserList)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_USER_LIST, OnNMclickUserList)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameControl::CGameFrameControl(CGameFrameDlg * pGameFrameDlg) : CDialog(IDD_FRAME_CONTROL), m_pGameFrameDlg(pGameFrameDlg)
{
	//设置变量
	m_bShowAd=false;
	m_bRectify=false;
	m_bInitDialog=false;
	m_dwLimitChatTimes=0;

	//接口指针
	m_pUserData=NULL;
	m_pIClientKernel=NULL;

	return;
}

//析构函数
CGameFrameControl::~CGameFrameControl()
{
	//销毁控件
	m_PropertyBar.DestroyBar();//?
}

//接口查询
void * CGameFrameControl::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IPurchaseInfo,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IPurchaseInfo,Guid,dwQueryVer);
	return NULL;
}

//房间类型
WORD CGameFrameControl::GetGameGenre()
{
	tagServerAttribute const *pServerAttribute = m_pIClientKernel->GetServerAttribute();
	return pServerAttribute->wGameGenre;
}

//枚举玩家
tagUserData * CGameFrameControl::EnumLookonUser(WORD wIndex)
{
	return m_pIClientKernel->EnumLookonUser(wIndex);
}

//查找用户
tagUserData * CGameFrameControl::SearchUserItem(DWORD dwUserID)
{
	return m_pIClientKernel->SearchUserItem(dwUserID);
}

//获取玩家
const tagUserData * CGameFrameControl::GetUserInfo(WORD wChairID)
{
	return m_pIClientKernel->GetUserInfo(wChairID);
}

//获取自己
const tagUserData * CGameFrameControl::GetMeUserInfo()
{
	return m_pIClientKernel->GetMeUserInfo();
}

//发送信息
void CGameFrameControl::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pData, wDataSize);
	
	return;
}

//控件绑定
void CGameFrameControl::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_PHRASE, m_btPhrase);
	DDX_Control(pDX, IDC_COLOR_SET, m_btColorSet);
	DDX_Control(pDX, IDC_SEND_CHAT, m_btSendChat);
	DDX_Control(pDX, IDC_EXPRESSION, m_btExpression);
	DDX_Control(pDX, IDC_CLEAN_SCREEN, m_btCleanScreen);
	DDX_Control(pDX, IDC_BUGLE, m_btBugle);
	DDX_Control(pDX, IDC_CHAT_INPUT, m_ChatInput);
	DDX_Control(pDX, IDC_CHAT_MESSAGE, m_ChatMessage);
	DDX_Control(pDX, IDC_USER_LIST, m_UserListView);
	DDX_Control(pDX, IDC_SOUND, m_btSound);
	DDX_Control(pDX, IDC_LOOKON, m_btLookOn);
	DDX_Control(pDX, IDC_GAME_RULE, m_btGameRule);
	DDX_Control(pDX, IDC_GAME_QUIT, m_btGameQuit);
	DDX_Control(pDX, IDC_GAME_OPTION, m_btGameOption);
	DDX_Control(pDX, IDC_CHAT_OBJECT, m_ChatObject);
}

//初始化函数
BOOL CGameFrameControl::OnInitDialog()
{
	__super::OnInitDialog();
	//个人信息
	m_UserInfoView.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, 98, NULL);

	//设置控件
	m_UserListView.InitUserListView();
	m_ChatMessage.SetBackgroundColor(FALSE,RGB(35,35,35));

	SkinSB_Init(m_UserListView, g_hScroll);
	SkinSB_Init(m_ChatMessage, g_hScroll);
	//广告控件
//	m_BrowerAD.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,100,NULL);
//	m_BrowerAD.Navigate(TEXT("http://www.7x78.com"),NULL,NULL,NULL,NULL);

//	m_BrowerAD1.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,100,NULL);
//	m_BrowerAD1.Navigate(TEXT("http://www.7x78.com"),NULL,NULL,NULL,NULL);
	//加载资源
	UpdateSkinResource();

	//聊天短语
	m_ChatInput.AddString(TEXT("快点吧我等的花儿也谢了"));
	m_ChatInput.AddString(TEXT("大家好，很高兴见到各位！"));
	m_ChatInput.AddString(TEXT("怎么又断线了，网络太差了..."));
	m_ChatInput.AddString(TEXT("不要走，决战到天亮。"));
	m_ChatInput.AddString(TEXT("和你合作真是太愉快了..."));
	m_ChatInput.AddString(TEXT("我们交个朋友，能告诉我你的联系方式吗？"));
	m_ChatInput.AddString(TEXT("你是MM,还是GG?"));
	m_ChatInput.AddString(TEXT("快点，花儿都凋谢了！"));
	m_ChatInput.AddString(TEXT("痛快，痛快，来十斤牛肉，八碗好酒！"));
	m_ChatInput.AddString(TEXT("不要吵了，有什么好吵的..."));
	m_ChatInput.AddString(TEXT("再见了，我会想念大家的！BYE"));
	m_ChatInput.LimitText(MAX_CHAT_LEN-1);

	//建立提示控件
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_EXPRESSION),TEXT("表情"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_COLOR_SET),TEXT("颜色"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_CLEAN_SCREEN),TEXT("清屏"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_SEND_CHAT),TEXT("发送"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BUGLE),TEXT("小喇叭"));

	//聊天对象
	m_ChatObject.AddString(TEXT("所有人"));
	m_ChatObject.SetCurSel(0);

	//道具控件
	m_PropertyBar.SetSkinResource(g_GlobalOption.m_PlatformResourceModule->GetResInstance(),&g_GlobalOption.m_PropertyViewImage);
	m_PropertyBar.CreateBar(this);
	m_PropertyBar.SetPurchaseInfoSink(this);

	//设置变量
	m_bInitDialog=true;
  //  m_BrowerAD.ShowWindow(0);
	m_btBugle.ShowWindow(0);
	//广告时间
	SetTimer(IDI_BROWER_AD,TIME_BROWER_AD,NULL);

	return TRUE;
}

//消息过虑
BOOL CGameFrameControl::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN))
	{
		OnBnClickedSendChat();
		return TRUE;
	}
	m_ToolTipCtrl.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//菜单命令
BOOL CGameFrameControl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_UM_COPY_NAME:		//拷贝名字
		{
			//寻找玩家
			if ((m_pUserData==NULL)||(m_pUserData->dwUserID==0)) return TRUE;

			//打开剪切板
			if (OpenClipboard()==FALSE) return TRUE;
			if (EmptyClipboard()==FALSE) 
			{
				CloseClipboard();
				return TRUE;
			}

			//复制数据
			HANDLE hData=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,NAME_LEN);
			if (hData==NULL) 
			{
				CloseClipboard();
				return TRUE;
			}
			LPTSTR szMemName=(LPTSTR)GlobalLock(hData);
			lstrcpy(szMemName,m_pUserData->szGroupName);
			SetClipboardData(CF_TEXT,hData);
			GlobalUnlock(hData);
			CloseClipboard();

			//显示名字
			m_ChatInput.Paste();
			m_ChatInput.SetFocus();

			return TRUE;
		}
	case IDM_UM_SET_CHAT:		//设置聊天
		{
			//寻找玩家
			if ((m_pUserData==NULL)||(m_pUserData->dwUserID==0)) return TRUE;

			int nCount=m_ChatObject.GetCount();
			for (int i=1;i<nCount;i++)
			{
				if (m_ChatObject.GetItemData(i)==m_pUserData->dwUserID)
				{
					m_ChatObject.SetCurSel(i);
					break;
				}
			}
			m_ChatInput.SetFocus();

			return TRUE;
		}
	case IDM_UM_CANECL_CHAT:		//取消聊天
		{
			//寻找玩家
			if ((m_pUserData==NULL)||(m_pUserData->dwUserID==0)) return TRUE;

			if (m_ChatObject.GetCurSel()!=LB_ERR)
			{
				DWORD dwUserID=(DWORD)m_ChatObject.GetItemData(m_ChatObject.GetCurSel());
				if (dwUserID==m_pUserData->dwUserID)
				{
					m_ChatObject.SetCurSel(0);
					break;
				}
			}
			m_ChatInput.SetFocus();

			return TRUE;
		}
	case IDM_UM_ALLOW_LOOKON:		//允许旁观
	case IDM_UM_ENJOIN_LOOKON:		//禁止旁观
		{
			//寻找玩家
			if ((m_pUserData==NULL)||(m_pUserData->dwUserID==0)) return TRUE;

			//设置控制
			bool bAllowLookon=(LOWORD(wParam)==IDM_UM_ENJOIN_LOOKON);
			m_pIClientKernel->AllowUserLookon(m_pUserData->dwUserID,bAllowLookon);

			return TRUE;
		}
	case IDM_UM_KICK_TABLE_USER:	//踢走用户
		{
			//寻找玩家
			if ((m_pUserData==NULL)||(m_pUserData->dwUserID==0)) return TRUE;

			//发送消息
			CMD_GF_KickTableUser KickTableUser;
			KickTableUser.dwUserID=m_pUserData->dwUserID;
			m_pGameFrameDlg->m_ClientKernelHelper->SendSocketData(MDM_GF_FRAME,SUB_GF_KICK_TABLE_USER,&KickTableUser,sizeof(KickTableUser));

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//设置内核
void CGameFrameControl::SetClientKernel(IClientKernel * pIClientKernel)
{
	m_pIClientKernel=pIClientKernel;
	m_UserInfoView.SetClientKernel(m_pIClientKernel);

	return;
}

//设置接口
bool CGameFrameControl::SetUserFaceRes(IUnknownEx * pIUnknownEx)
{
	return m_UserInfoView.SetUserFaceRes( pIUnknownEx);
}

//更新资源
bool CGameFrameControl::UpdateSkinResource()
{
	//获取资源
	HINSTANCE hInstance=g_GlobalOption.m_PlatformResourceModule->GetResInstance();
	tagGameFrameImage &GameFrameImage=g_GlobalOption.m_GameFrameImage;
	
	//列表区域
	m_EncircleList.ImageML.SetLoadInfo(GameFrameImage.uUserListML,hInstance);
	m_EncircleList.ImageMR.SetLoadInfo(GameFrameImage.uUserListMR,hInstance);
	m_EncircleList.ImageTL.SetLoadInfo(GameFrameImage.uUserListTL,hInstance);
	m_EncircleList.ImageTM.SetLoadInfo(GameFrameImage.uUserListTM,hInstance);
	m_EncircleList.ImageTR.SetLoadInfo(GameFrameImage.uUserListTR,hInstance);
	m_EncircleList.ImageBL.SetLoadInfo(GameFrameImage.uUserListBL,hInstance);
	m_EncircleList.ImageBM.SetLoadInfo(GameFrameImage.uUserListBM,hInstance);
	m_EncircleList.ImageBR.SetLoadInfo(GameFrameImage.uUserListBR,hInstance);

	/*m_ImageTitle1.SetLoadInfo(GameFrameImage.uUserListBL,hInstance);
	m_ImageTitle2.SetLoadInfo(GameFrameImage.uUserListBM,hInstance);
	m_ImageTitle3.SetLoadInfo(GameFrameImage.uUserListBR,hInstance);*/
		
    
	

	//聊天区域
	m_EncircleChat.ImageML.SetLoadInfo(GameFrameImage.uChatML,hInstance);
	m_EncircleChat.ImageMR.SetLoadInfo(GameFrameImage.uChatMR,hInstance);
	m_EncircleChat.ImageTL.SetLoadInfo(GameFrameImage.uChatTL,hInstance);
	m_EncircleChat.ImageTM.SetLoadInfo(GameFrameImage.uChatTM,hInstance);
	m_EncircleChat.ImageTR.SetLoadInfo(GameFrameImage.uChatTR,hInstance);
	m_EncircleChat.ImageBL.SetLoadInfo(GameFrameImage.uChatBL,hInstance);
	m_EncircleChat.ImageBM.SetLoadInfo(GameFrameImage.uChatBM,hInstance);
	m_EncircleChat.ImageBR.SetLoadInfo(GameFrameImage.uChatBR,hInstance);

	//获取信息
	CSkinAide::GetEncircleInfo(m_ImageInfoChat,m_EncircleChat);
	CSkinAide::GetEncircleInfo(m_ImageInfoList,m_EncircleList);

	//设置按钮
	//m_btPhrase.SetButtonImage(IDB_BT_EXPESSION,hInstance,false);
	m_btExpression.SetButtonImage(GameFrameImage.uBtExpression,hInstance,false);
	m_btColorSet.SetButtonImage(GameFrameImage.uBtColorSet,hInstance,false);
	m_btCleanScreen.SetButtonImage(GameFrameImage.uBtCleanScreen,hInstance,false);
	m_btBugle.SetButtonImage(GameFrameImage.uBtBugle,hInstance,false);
	m_btSendChat.SetButtonImage(GameFrameImage.uBtSendChat,hInstance,false);

	//游戏规则
	m_btSound.SetButtonImage(GameFrameImage.uBtGameSound,hInstance,false);
	m_btLookOn.SetButtonImage(GameFrameImage.uBtGameLookOn,hInstance,false);
	m_btGameRule.SetButtonImage(GameFrameImage.uBtGameRule,hInstance,false);
	//m_btGameQuit.SetButtonImage(IDB_BT_GAME_QUIT,hInstance,false);
	m_btGameOption.SetButtonImage(GameFrameImage.uBtGameOption,hInstance,false);

	return true;
}

//重置控制
void CGameFrameControl::ResetGameFrameControl()
{
	//聊天对象
	int nChatCount=m_ChatObject.GetCount();
	for (int i=1;i<nChatCount;i++) m_ChatObject.DeleteString(1);

	//清理列表
	m_UserListView.DeleteAllItems();

	return;
}

//加入用户
bool CGameFrameControl::InsertUserItem(tagUserData * pUserData)
{
	if ( m_pIClientKernel->GetMeUserInfo()->dwUserID == pUserData->dwUserID)
	{
		m_UserInfoView.SetUserViewInfo(pUserData);
		m_UserListView.SetMeUserData(pUserData);
	}

	return m_UserListView.InsertUserItem(pUserData);
}

//更新用户
bool CGameFrameControl::UpdateUserItem(tagUserData * pUserData)
{
	m_UserInfoView.UpdateUserInfo();

	return m_UserListView.UpdateUserItem(pUserData);
}

//删除用户
bool CGameFrameControl::DeleteUserItem(tagUserData * pUserData)
{
	tagUserData const *pCurrentUserData = m_UserInfoView.GetCurrentUser();
	if ( pCurrentUserData != NULL && pUserData->dwUserID == pCurrentUserData->dwUserID )
		m_UserInfoView.SetUserViewInfo(m_pIClientKernel->GetMeUserInfo());

	return m_UserListView.DeleteUserItem(pUserData);
}

//绘画消息
void CGameFrameControl::OnPaint()
{
	CPaintDC dc(this);

	//绘画背景
	//DrawLeftViewFrame(&dc);

	CRect rcClient;
	GetClientRect(&rcClient);

	CImageHandle HandleTitleL(&m_ImageTitle1);
	CImageHandle HandleTitleM(&m_ImageTitle2);
	CImageHandle HandleTitleR(&m_ImageTitle3);

	CRect rcChat;
	rcChat.left=0;
	rcChat.right=rcClient.Width();
	rcChat.top=0;
	rcChat.bottom=rcClient.Height();
    
	m_ImageTitle1.BitBlt(dc,rcChat.left,rcChat.top);
    
   for(INT nXPos=rcChat.top+m_ImageTitle1.GetHeight();nXPos<rcClient.bottom-m_ImageTitle3.GetHeight();nXPos+=m_ImageTitle2.GetHeight())
	{
	
		m_ImageTitle2.BitBlt(dc,rcChat.left,nXPos);
		
	}
	m_ImageTitle3.BitBlt(dc,rcChat.left,rcClient.bottom-m_ImageTitle3.GetHeight());

	return;
}

//设置按钮
void CGameFrameControl::OnBnClickedOption()
{
	m_pGameFrameDlg->OnGameOptionSet();
	return;
}

//游戏规则
void CGameFrameControl::OnBnClickedRule()
{
	TCHAR szRuleUrl[128];
	_snprintf(szRuleUrl,sizeof(szRuleUrl),TEXT("http://www.7x78.com/GameRule.asp?KindID=%ld"),m_UserListView.m_wKindID);
	ShellExecute(NULL,TEXT("open"),szRuleUrl,NULL,NULL,SW_SHOWDEFAULT);

	return;
}

//游戏退出
void CGameFrameControl::OnBnClickedQuit()
{
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
	return;
}

//游戏声音
void CGameFrameControl::OnBnClickedSound()
{
	if (m_pGameFrameDlg->IsEnableSound()==true)
	{
		m_pGameFrameDlg->EnableSound(false);
	}
	else
	{
		m_pGameFrameDlg->EnableSound(true);
	}

	return;
}

//游戏旁观
void CGameFrameControl::OnBnClickedLookon()
{
	if (m_pGameFrameDlg->IsAllowUserLookon()==true)
	{
		m_pGameFrameDlg->AllowUserLookon(0L,false);
	}
	else
	{
		m_pGameFrameDlg->AllowUserLookon(0L,true);
	}

	return;
}

//绘画背景
BOOL CGameFrameControl::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//位置消息
void CGameFrameControl::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//表情消息
LRESULT CGameFrameControl::OnHitExpression(WPARAM wParam, LPARAM lParam)
{
	CExpressionItem * pExpressionItem=m_Expression.GetExpressItem((WORD)wParam);
	if (pExpressionItem!=NULL)
	{
		CString strBuffer;
		m_ChatInput.GetWindowText(strBuffer);
		strBuffer+=pExpressionItem->m_szTrancelate;
		m_ChatInput.SetWindowText(strBuffer);
		m_ChatInput.SetFocus();
		m_ChatInput.SetEditSel(strBuffer.GetLength(),strBuffer.GetLength());
	}

	return 0;
}

//发送按钮
void CGameFrameControl::OnBnClickedSendChat()
{
	//聊天信息
	TCHAR szChatMessage[MAX_CHAT_LEN]=TEXT("");
	m_ChatInput.GetWindowText(szChatMessage,CountArray(szChatMessage));
	if (szChatMessage[0]!=0)
	{
		//限时聊天
		DWORD dwCurTimes = GetTickCount();
		if(m_dwLimitChatTimes==0 || dwCurTimes-m_dwLimitChatTimes>=LIMIT_CHAT_TIMES)
		{
			m_dwLimitChatTimes = dwCurTimes;
			DWORD dwTargetUserID=0L;
			int nItem=m_ChatObject.GetCurSel();
			if ((nItem!=LB_ERR)&&(nItem!=0)) 
			{
				dwTargetUserID=(DWORD)m_ChatObject.GetItemData(nItem);
			}
			m_ChatInput.SetWindowText(TEXT(""));
			m_pIClientKernel->SendChatMessage(dwTargetUserID,szChatMessage,g_GlobalOption.m_crChatTX);
		}
	}

	//设置界面
	m_ChatInput.SetFocus();

	return;
}

//表情按钮
void CGameFrameControl::OnBnClickedExpression()
{
	//建立表情窗口
	if (m_Expression.GetSafeHwnd()==NULL) m_Expression.CreateExpression(this);

	//移动窗口
	CRect rcButton;
	CSize ExpressSize;
	m_Expression.GetFixSize(ExpressSize);
	m_btExpression.GetWindowRect(&rcButton);
	m_Expression.MoveWindow(rcButton.right-ExpressSize.cx,rcButton.top-ExpressSize.cy,ExpressSize.cx,ExpressSize.cy);
	m_Expression.ShowWindow(SW_SHOW);
	m_Expression.SetFocus();

	return;
}

//颜色按钮
void CGameFrameControl::OnBnClickedColorSet()
{
	//设置颜色
	CColorDialog ColorDialog(g_GlobalOption.m_crChatTX,CC_FULLOPEN);
	if (ColorDialog.DoModal()==IDOK) g_GlobalOption.m_crChatTX=ColorDialog.GetColor();

	//设置界面
	m_ChatInput.SetFocus();

	return;
}

//清屏按钮
void CGameFrameControl::OnBnClickedCleanScreen()
{
	m_ChatMessage.CleanScreen();
	return;
}

//喇叭按钮
void CGameFrameControl::OnBnClickedBugle()
{
	if(m_pIClientKernel==NULL)return;

	//设置对话框
	ShowBugleDlg(m_pIClientKernel,NULL,(tagUserData *)m_pIClientKernel->GetMeUserInfo());

	return ;
}

//左视图区
void CGameFrameControl::DrawLeftViewFrame(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//调整大小
	rcClient.right-=5;
	rcClient.bottom-=10;

	//变量定义
	HDC hDC=pDC->m_hDC;
	int nXPos=0,nYPos=0;
	int nComminutePos=rcClient.Height()*5/9;

	//按钮区域
	CRect rcButton;
	rcButton.left=0;
	rcButton.right=rcClient.Width();
	rcButton.top=nComminutePos;
	rcButton.bottom=nComminutePos+37;
	pDC->FillSolidRect(rcButton,RGB(161,216,255));

	//绘画聊框
	CRect rcChat;
	rcChat.left=0;
	rcChat.right=rcClient.Width();
	rcChat.top=nComminutePos+37;
	rcChat.bottom=rcClient.Height();
	CSkinAide::DrawEncircleFrame(pDC,rcChat,m_EncircleChat);

	//绘画列表
	CRect rcList;
	rcList.left=0;
	rcList.right=rcClient.Width();
	rcList.top=0;
	rcList.bottom=nComminutePos-m_PropertyBar.GetHeight();
	CSkinAide::DrawEncircleFrame(pDC,rcList,m_EncircleList);


return;
}

//调整控件
void CGameFrameControl::RectifyControl(int nWidth, int nHeight)
{
	//状态判断
	if (m_bInitDialog==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//变量定义
	const int nComminutePos=nHeight*4/8;
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//移动控件
	HDWP hDwp=BeginDeferWindowPos(20);


	DeferWindowPos(hDwp,m_UserListView,NULL,8,180,
		215-17, 169,uFlags);

	//道具区域
	m_PropertyBar.ShowWindow(SW_SHOW);
	DeferWindowPos(hDwp,m_PropertyBar,NULL,m_ImageInfoList.nLBorder-2,nComminutePos-m_PropertyBar.GetHeight()+3+50+5,
		231+45+17,82-6,uFlags);

    //m_BrowerAD1.ShowWindow(0);

	//广告区域
//	if(!m_bShowAd) DeferWindowPos(hDwp,m_BrowerAD,NULL,m_ImageInfoList.nLBorder,7,0,0,uFlags);
//	else  DeferWindowPos(hDwp,m_BrowerAD,NULL,m_ImageInfoList.nLBorder+7,8,231,m_ImageInfoList.nTBorder-8,uFlags);

// 	DeferWindowPos(hDwp,m_BrowerAD,NULL,m_ImageInfoList.nLBorder+7,nComminutePos-m_PropertyBar.GetHeight()+3+50+5,
// 		240,52,uFlags);

    CRect rcButton;
	m_btBugle.GetWindowRect(&rcButton);
	//聊天区域
	DeferWindowPos(hDwp,m_ChatMessage,NULL,8,nHeight-349,215-17,348,uFlags);
    
	//聊天区域
	
// 	CButton * pButtonArray[]={&m_btExpression,&m_btColorSet,&m_btCleanScreen};
// 	for (int i=0;i<CountArray(pButtonArray);i++)
// 	{
// 		DeferWindowPos(hDwp,pButtonArray[i]->m_hWnd,NULL,m_ImageInfoList.nLBorder+(rcButton.Width()+5)*i+9,nHeight-53,rcButton.Width(),rcButton.Height(),uFlags);
// 	}
// 	m_btSendChat.GetWindowRect(&rcButton);
// 	DeferWindowPos(hDwp,m_btSendChat,NULL,nWidth-rcButton.Width()-10,nHeight-33,rcButton.Width(),rcButton.Height(),uFlags);
// 	DeferWindowPos(hDwp,m_ChatObject,NULL,10,nHeight-33,70,200,uFlags);
// 	DeferWindowPos(hDwp,m_ChatInput,NULL,82,nHeight-33,nWidth-rcButton.Width()-93,19,uFlags);

    
	//功能按钮
	m_btSound.GetWindowRect(&rcButton);
	int nBeginPos=14,nEndPos=nWidth;
	int nButtonSpace=(nEndPos-nBeginPos-4*rcButton.Width())/4,nYPosButton=nComminutePos-m_PropertyBar.GetHeight()+9+132;
	DeferWindowPos(hDwp,m_btSound,NULL,nBeginPos,nYPosButton,rcButton.Width(),rcButton.Height(),uFlags);
	DeferWindowPos(hDwp,m_btLookOn,NULL,nBeginPos+15+rcButton.Width(),nYPosButton,rcButton.Width(),rcButton.Height(),uFlags);
	DeferWindowPos(hDwp,m_btGameRule,NULL,nBeginPos+15*2+rcButton.Width()*2,nYPosButton,rcButton.Width(),rcButton.Height(),uFlags);
	DeferWindowPos(hDwp,m_btGameOption,NULL,nBeginPos+15*3+rcButton.Width()*3,nYPosButton,rcButton.Width(),rcButton.Height(),uFlags);
	DeferWindowPos(hDwp,m_btGameQuit,NULL,nBeginPos+15*4+rcButton.Width()*4,nYPosButton,rcButton.Width(),rcButton.Height(),uFlags);
	m_btGameQuit.ShowWindow(SW_HIDE);
	//////////////////////////////////////////////////////////////////////////
	m_btColorSet.ShowWindow(0);m_btExpression.ShowWindow(0);m_btCleanScreen.ShowWindow(0);
	m_btSound.ShowWindow(0);m_btLookOn.ShowWindow(0);m_btGameRule.ShowWindow(0);m_btGameOption.ShowWindow(0);
	//////////////////////////////////////////////////////////////////////////

	//用户区域
	//DeferWindowPos(hDwp,m_UserInfoView,NULL,m_ImageInfoList.nLBorder,7,nWidth-m_ImageInfoList.nLBorder-m_ImageInfoList.nRBorder,145,uFlags);

	EndDeferWindowPos(hDwp);
	//m_UserListView.ShowWindow(0);
	//m_ChatMessage.ShowWindow(0);
	m_PropertyBar.ShowWindow(0);

	m_ChatInput.ShowWindow(0);m_ChatObject.ShowWindow(0);m_btSendChat.ShowWindow(0);

	//重画界面
	Invalidate(FALSE);
	UpdateWindow();

	return;
}

//右键列表
void CGameFrameControl::OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	if (pListNotify->iItem!=-1)
	{
		//获取位置
		CPoint MousePoint;
		GetCursorPos(&MousePoint);

		//获取用户
		TCHAR szBuffer[512]=TEXT("");
		m_pUserData=(tagUserData *)m_UserListView.GetItemData(pListNotify->iItem);
		if(m_pUserData==NULL) return;

		//预先计算
		double dFleeRate=0.0;
		LONG lPlayCount=m_pUserData->lWinCount+m_pUserData->lLostCount+m_pUserData->lDrawCount+m_pUserData->lFleeCount;
		if (m_pUserData->lFleeCount>0)
		{
			dFleeRate=(double)(m_pUserData->lFleeCount*100)/(double)lPlayCount;
			if (dFleeRate<0.01) dFleeRate=0.0;
		}

		//构造菜单
		CMenu UserInfoMenu;
		UserInfoMenu.CreatePopupMenu();

		//功能菜单
		UserInfoMenu.AppendMenu(0,IDM_UM_COPY_NAME,TEXT("拷贝昵称"));
		UserInfoMenu.AppendMenu(MF_SEPARATOR);
		
		//用户称谓
		LPCTSTR pszMemberOrder[]={TEXT("普通会员"),TEXT("中级会员"),TEXT("高级贵宾")};
		if ((m_pUserData->cbMemberOrder>0)&&(m_pUserData->cbMemberOrder<CountArray(pszMemberOrder)))
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("昵称：%s [ %s ]"),m_pUserData->szGroupName,pszMemberOrder[m_pUserData->cbMemberOrder]);
		}
		else _snprintf(szBuffer,sizeof(szBuffer),TEXT("昵称：%s"),m_pUserData->szGroupName);
		UserInfoMenu.AppendMenu(0,0,szBuffer);

		//用户 ID
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("游戏 ID：%ld"),m_pUserData->dwGameID);
		UserInfoMenu.AppendMenu(0,0,szBuffer);

		//用户经验
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("经验值：%ld"),m_pUserData->lExperience);
		UserInfoMenu.AppendMenu(0,0,szBuffer);

		//用户积分
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("积分：%I64d  总局数：%ld  逃跑率：%5.2f%%"),m_pUserData->lScore,lPlayCount,dFleeRate);
		UserInfoMenu.AppendMenu(0,0,szBuffer);

		//获取用户
		DWORD dwCurUserID=0L;
		const tagUserData * pMeUserData=m_pIClientKernel->GetMeUserInfo();
		if (m_ChatObject.GetCurSel()!=LB_ERR)
		{
			dwCurUserID=(DWORD)m_ChatObject.GetItemData(m_ChatObject.GetCurSel());
		}

		//聊天对象
		if (pMeUserData->dwUserID!=m_pUserData->dwUserID) UserInfoMenu.AppendMenu(MF_SEPARATOR);
		if ((dwCurUserID!=m_pUserData->dwUserID)&&(dwCurUserID!=pMeUserData->dwUserID)&&(pMeUserData->dwUserID!=m_pUserData->dwUserID))
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("与 [ %s ] 聊天"),m_pUserData->szGroupName);
			UserInfoMenu.AppendMenu(0,IDM_UM_SET_CHAT,szBuffer);
		}
		if (dwCurUserID==m_pUserData->dwUserID)
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("取消与 [ %s ] 的聊天"),m_pUserData->szGroupName);
			UserInfoMenu.AppendMenu(0,IDM_UM_CANECL_CHAT,szBuffer);
		}

		//旁观控制
		if ((pMeUserData!=m_pUserData)&&(pMeUserData->cbUserStatus!=US_LOOKON)&&(m_pUserData->wChairID==pMeUserData->wChairID))
		{
			//分隔菜单
			UserInfoMenu.AppendMenu(MF_SEPARATOR);

			//允许旁观
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("允许 [ %s ] 观看您游戏"),m_pUserData->szGroupName);
			UserInfoMenu.AppendMenu(0,IDM_UM_ALLOW_LOOKON,szBuffer);

			//禁止旁观
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("禁止 [ %s ] 观看您游戏"),m_pUserData->szGroupName);
			UserInfoMenu.AppendMenu(0,IDM_UM_ENJOIN_LOOKON,szBuffer);
		}

		//获取属性
		tagServerAttribute const* pServerAttribute = m_pIClientKernel->GetServerAttribute();

		if ( pServerAttribute->wChairCount <= MAX_CHAIR_NORMAL || m_pIClientKernel->GetMeUserInfo()->cbMasterOrder > 0 )
		{
			//踢人菜单
			if (m_pIClientKernel->GetMeUserInfo()->dwUserID!=m_pUserData->dwUserID)
			{
				//控制菜单
				bool bEnable=false;
				if (bEnable==false) bEnable=(pMeUserData->cbMasterOrder>m_pUserData->cbMasterOrder);
				if (bEnable==false) bEnable=(pMeUserData->cbMemberOrder>m_pUserData->cbMemberOrder);
				if ( pMeUserData->cbMasterOrder<m_pUserData->cbMasterOrder ) bEnable = false;

				//插入菜单
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("把 [ %s ] 踢出游戏房间"),m_pUserData->szGroupName);
				UserInfoMenu.AppendMenu((bEnable==true)?MF_ENABLED:MF_GRAYED,IDM_UM_KICK_TABLE_USER,szBuffer);
			}
		}
	
		//显示菜单
		UserInfoMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,MousePoint.x,MousePoint.y,this);
	}

	return;
}

//选中列表
void CGameFrameControl::OnNMclickUserList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	if (pListNotify->iItem!=-1)
	{
		//获取用户
		tagUserData *pUserData=(tagUserData *)m_UserListView.GetItemData(pListNotify->iItem);
		if(pUserData==NULL) return;

		//更新信息
		m_UserInfoView.SetUserViewInfo(pUserData);

		//选择用户
		m_PropertyBar.SetUserDataInfo(pUserData);

		//定时切换
		if(pUserData->dwUserID!=m_pIClientKernel->GetMeUserInfo()->dwUserID)SetTimer(IDI_SWITCH_USER,TIME_SWITCH_USER,NULL);
	}
	return;
}

//时间消息
void CGameFrameControl::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case IDI_BROWER_AD:			//广告时间
		{
			m_bShowAd=!m_bShowAd;

			//调整控件
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

			return;
		}
	case IDI_SWITCH_USER:		//切换信息
		{
			//删除时间
			KillTimer(IDI_SWITCH_USER);

			//更新信息
			m_UserInfoView.SetUserViewInfo(m_pIClientKernel->GetMeUserInfo());

			return;
		}
	}

	CDialog::OnTimer(nIDEvent);
}
//////////////////////////////////////////////////////////////////////////

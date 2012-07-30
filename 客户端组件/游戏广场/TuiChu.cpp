#include "Stdafx.h"
#include "resource.h"
#include "GlobalUnits.h"
#include "TuiChu.h"


//////////////////////////////////////////////////////////////////////////

//定时器I D
#define IDI_CHARMVALUE_UPDATE_VIEW		1								//更新界面
#define TIME_CHARMVALUE_UPDATE_VIEW		200								//更新界面

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDTuiChu, CSkinDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()	
	ON_BN_CLICKED(IDOK,OnBnClickedOk)   //确定
	ON_BN_CLICKED(IDC_TUICHU_GOLG, OnChangOk1)
	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CDTuiChu::CDTuiChu(CWnd* pParent):CSkinDialogEx(IDD_QUIT, pParent)
{

}

//析构函数
CDTuiChu::~CDTuiChu()
{
}
void CDTuiChu::OnSize(UINT nType, int cx, int cy) 
{
	__super::OnSize(nType, cx, cy);

//	CRgn m_rgn;
// CRect rc;
// GetWindowRect(&rc); //获得窗口矩形
// rc -= rc.TopLeft();
//m_rgn.CreateRoundRectRgn(rc.left, rc.top, rc.right+1, rc.bottom+1, 5, 5); //根据窗口矩形创建一个圆角矩形
//SetWindowRgn(m_rgn,true);

// 	CDC* dc=GetDC();
//// 
//// 	//获取位置
//// 	CRect rcClient;
//// 	GetClientRect(&rcClient);
// 	//加载图标
// 	HICON hIcon=LoadIcon(NULL,IDI_QUESTION);
// 
//	//绘画图标
// 	DrawIconEx(dc->m_hDC,50,50,hIcon,32,32,0,NULL,DI_NORMAL);
// 	AfxMessageBox("xxxx");

	return;
}
//控件绑定
void CDTuiChu::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	 DDX_Control(pDX, IDC_TUICHU_GOLG, m_btTQ);
	 DDX_Control(pDX, IDCANCLE, m_btCancel);

}
//提取按钮
void CDTuiChu::OnBnClickedOk()
{
	
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
    //AfxMessageBox("弹出来了");
//	AfxGetMainWnd()->PostMessage(IDC_BT_SWITCH_ACCOUNTS,0,0);
	//CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	//pGameFrame->QieHuanZhangHao();
	//ShowWindow(SW_HIDE);
   return;
}
void CDTuiChu::OnChangOk1()
{
	
	//AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
//	AfxGetMainWnd()->PostMessage(IDC_BT_SWITCH_ACCOUNTS,0,0);
	CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	pGameFrame->QieHuanZhangHao();
	ShowWindow(SW_HIDE);
   return;
}

void CDTuiChu::SetPurchaseInfoSink(IPurchaseInfo *pIPurchaseInfo)
{
   return;
}

	//提取按扭
 void CDTuiChu::OnBnTiQuOk()             
 {
   return;
 }
	//存入按钮 
 void CDTuiChu::OnBnCunRuOk()
 {  
   return;
 }
	//转账按钮 
 void CDTuiChu::OnZhuanZhangOk()
 {
   return;
 }
	//银行密码
 void CDTuiChu::OnPassBankOk()
 {

 return;
 }

//初始化函数
BOOL CDTuiChu::OnInitDialog()
{
	CSkinDialogEx::OnInitDialog();
	
	//m_btOK.SetButtonImage(IDB_GAME_TUICHUI,AfxGetInstanceHandle(),false);
	//m_btTQ.SetButtonImage(IDB_GAME_QIEHUAN  ,AfxGetInstanceHandle(),false);
	//m_btTQ.SetWindowText("xxxx");
	SetWindowText("退出确认");
	return TRUE;
}

//更新界面
void CDTuiChu::UpdateView()
{

}

//鼠标信息
void CDTuiChu::OnLButtonDown(UINT nFlags, CPoint point)
{
	CSkinDialogEx::OnLButtonDown(nFlags, point);
}

//鼠标信息
void CDTuiChu::OnMouseMove(UINT nFlags, CPoint point)
{
	CSkinDialogEx::OnMouseMove(nFlags, point);
}

//命令信息
BOOL CDTuiChu::PreTranslateMessage(MSG* pMsg)
{

	////过滤回车
	//if(pMsg->message==WM_KEYDOWN)
	//{
	//	BYTE bMesValue = (BYTE)(pMsg->wParam);
	//	if(bMesValue==VK_RETURN) return TRUE;
	//}

	return __super::PreTranslateMessage(pMsg);
}

//时间消息
void CDTuiChu::OnTimer(UINT nIDEvent)
{

	__super::OnTimer(nIDEvent);
}

//输入信息
void CDTuiChu::OnEnChangeInCount()
{


	return ;
}

void CDTuiChu::OnEnChangeInID()
{

	return ;
}

//是否选中
bool CDTuiChu::IsButtonSelected(UINT uButtonID)
{
	return ((CButton *)GetDlgItem(uButtonID))->GetCheck()==BST_CHECKED;
}

void CDTuiChu::DrawTextString()
{
	
	return;
}
//选中按钮
void CDTuiChu::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	
	return;
}

//设置信息
void CDTuiChu::SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket)
{
	
    
	return;
}

//发送信息
void CDTuiChu::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{

	

	return ;
}

bool CDTuiChu::ConnectServer()
{
	


	return true;
}

void CDTuiChu::OnPaint()
{

	CPaintDC dc(this);

	//绘画界面
	DrawCaption(&dc);
	DrawBackGround(&dc);
	DrawBorder(&dc);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);
	//加载图标
	HICON hIcon=LoadIcon(NULL,IDI_QUESTION);

	//绘画图标
	DrawIconEx(dc,10,35,hIcon,32,32,0,NULL,DI_NORMAL);
	//DrawSkinView(&dc);
	 //CSkinDialogEx::OnPaint();
	DrawBackGround(&dc);

	CRect rcClient1;
	GetWindowRect(&rcClient1);
	int nWidth1=rcClient1.Width();
	int nHeight1=rcClient1.Height();
	//调整判断
	if ((nWidth1!=0)&&(nHeight1!=0))
	{
		//框架位置
		CRect rcFrame;
		rcFrame.SetRect(0,0,nWidth1,nHeight1);

		//窗口区域
		CRgn RegionWindow;
		RegionWindow.CreateRoundRectRgn(rcFrame.left,rcFrame.top,rcFrame.right+1,rcFrame.bottom+1,5,5);

		//设置区域
		SetWindowRgn(RegionWindow,TRUE);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

//兑换魅力


//////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include"resource.h"
#include "DlgUserManager.h"
#include "GlobalUnits.h"


////////////////////////////////////////////////////////////////////////////////////
//宏定义
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//默认颜色
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgUserManage, CSkinDialogEx1)
	ON_WM_PAINT()
	//ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CLOSE, OnClose1)

	ON_BN_CLICKED(IDC_SAMPLE, OnSample)
	ON_BN_CLICKED(IDC_XX_INFO, OnXX)
	ON_BN_CLICKED(IDC_LOCK, OnLock)
	ON_BN_CLICKED(IDC_PASS, OnPass)
//	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree1)

END_MESSAGE_MAP()

//交换函数
void CDlgUserManage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CLOSE, m_btOk);

	DDX_Control(pDX, IDC_SAMPLE, m_btSample);
	DDX_Control(pDX, IDC_XX_INFO, m_btXX);
	DDX_Control(pDX, IDC_LOCK, m_btLock);
	DDX_Control(pDX, IDC_PASS, m_btPass);
	
	//DDX_Control(pDX, IDC_TREE, m_tree);
	
}

////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgUserManage::CDlgUserManage(): CSkinDialogEx1(IDD_USER_MANAGER)//, CSkinWndObject(this)
{
	if (!m_pngImageBack.IsNull())
	{
		m_pngImageBack.DestroyImage();
	}
	m_pngImageBack.LoadImage(szResPath("BankDlg\\BankDlgBack.png"));
}

//虚构函数
CDlgUserManage::~CDlgUserManage()
{
	if (m_brBkground.m_hObject != NULL ) m_brBkground.DeleteObject();
	if (!m_pngImageBack.IsNull())
	{
		m_pngImageBack.DestroyImage();
	}
}



//初始函数
BOOL CDlgUserManage::OnInitDialog()
{
	__super::OnInitDialog();

	//创建刷子
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//调整窗体
	SetWindowPos(NULL, 0,0,m_pngImageBack.GetWidth(), m_pngImageBack.GetHeight(),SWP_NOZORDER);
	CenterWindow();
	CRect rcClient;
	GetWindowRect(&rcClient);
	ScreenToClient(&rcClient);

	//调整框架
	CRect rcUserInfoIni;
	CGameFrame::GetRectFromIni(szResPath("UserInfo\\UserInfo.ini"), TEXT("UserInfo"), TEXT("ItemRect"), rcUserInfoIni, rcUserInfoIni);
	GetDlgItem(IDC_SHOW)->SetWindowPos(NULL, rcUserInfoIni.left, rcUserInfoIni.top, rcUserInfoIni.Width(),rcUserInfoIni.Height(),SWP_NOZORDER);

	m_btOk.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_CANCLE.bmp"), false);
	m_btOk.SetWindowPos(NULL, rcClient.Width()-30, 3, 0, 0, SWP_NOSIZE);
	m_btOk.SetWindowText("");

	static CRect rect;
	if(rect.IsRectNull())
	{
		//分隔符窗口
		CRect rectH;  //水平
		CRect rectV;  //垂直
		GetDlgItem(IDC_SHOW)->GetWindowRect(&rectH);
		
		//GetDlgItem(IDC_SEP2)->GetWindowRect(&rectV);
		rect.left= rectH.left+10;
		rect.bottom=rectH.bottom;
		rect.right=rectH.right;
		rect.top=rectH.top+15;

		ScreenToClient(&rect);
	}

	UINT xPos=0;//GetXExcursionPos()+10;
	UINT yPox =32; //GetYExcursionPos()+10;
	CRect rcBtn;
	m_btSample.GetWindowRect(&rcBtn);
	UINT uStype = 37;
	m_btSample.SetWindowPos(NULL, xPos, yPox, 0, 0,SWP_NOSIZE);
	m_btXX.SetWindowPos(NULL, xPos, yPox+uStype, 0, 0,SWP_NOSIZE);
	m_btLock.SetWindowPos(NULL, xPos, yPox+uStype*2, 0, 0,SWP_NOSIZE);
	m_btPass.SetWindowPos(NULL, xPos, yPox+uStype*3, 0,0 ,SWP_NOSIZE);
	
	m_DlgUserSample.Create(IDD_USER_JD_INFO,this);
	m_DlgUserSample.MoveWindow(rect);
	m_DlgUserSample.ShowWindow(SW_SHOW);   //初始化为第一个对话框

	m_DlgUserXX.Create(IDD_USER_XX_INFO,this);
	m_DlgUserXX.MoveWindow(rect);

	m_DlgUserLock.Create(IDD_USER_LOCK,this);
	m_DlgUserLock.MoveWindow(rect);

	m_DlgUserPass.Create(IDD_USER_PASS,this);
	m_DlgUserPass.MoveWindow(rect);
	
	GetDlgItem(IDC_NAME)->SetWindowText("基本信息");

	//HTREEITEM  hti1 = m_tree.InsertItem ( _T("欢乐豆赠送"),0,1);
	//HTREEITEM  hti2 = m_tree.InsertItem ( _T("赠送日志"),0,1);
	//HTREEITEM  hti3 = m_tree.InsertItem ( _T("游戏存取"),0,1);
	//HTREEITEM  hti4 = m_tree.InsertItem ( _T("密码修改"),0,1);
	//HTREEITEM  hti5 = m_tree.InsertItem ( _T("选择"),0,1);

	//m_tree.SetItemHeight(20);
	//


	//设置标题
	//SetWindowText(TEXT("欢乐豆赠送"));

	//更新控件
	//UpdateControls();

	//初始化设置
	//__super::InitSkinObject();

	return TRUE;  
}

//绘画函数
void CDlgUserManage::OnPaint()
{

	CPaintDC dc(this);
	CRect rcClient;
	GetClientRect(&rcClient);
	//建立缓冲
	CBitmap ImageBuffer;
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//创建 DC
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(&dc);
	BufferDC.SelectObject(&ImageBuffer);

	CDC* pDc = GetDC();
	m_pngImageBack.DrawImage(&BufferDC, 0,0);
	//int iPox=m_pngStatus.GetHeight();
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);
	//绘画界面
	BufferDC.DeleteDC();
	ImageBuffer.DeleteObject();
	return;
	//绘画标题
//	__super::DrawSkinView(&dc);
}

//颜色处理
HBRUSH CDlgUserManage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
		
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(BGROUND_COLOR);
			return m_brBkground;

			//RGB(239,247,255)
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}


//销毁消息
void CDlgUserManage::OnClose()
{
	

	CDialog::OnClose();
}


//销毁消息
void CDlgUserManage::OnClose1()
{
	CDialog::OnCancel();
}

//void CDlgMoneyBank::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
//	// TODO: Add your control notification handler code here
//	//Invalidate();
//	//SetRedraw (TRUE);
//	
//	HTREEITEM hItem = m_tree.GetSelectedItem();
//    //得到它的内容
//    CString Text = m_tree.GetItemText(hItem);
//	if(Text=="欢乐豆赠送")
//	{
//		m_DlgMoneyGive.ShowWindow(SW_SHOW);
//		m_DlgMoneyManager.ShowWindow(SW_HIDE);
//	}
//	else if(Text=="游戏存取")
//	{
//		m_DlgMoneyGive.ShowWindow(SW_HIDE);
//		m_DlgMoneyManager.ShowWindow(SW_SHOW);
//	}
//
//
//	*pResult = 0;
//}


void CDlgUserManage::OnSample()
{
	m_DlgUserSample.ShowWindow(SW_SHOW);
	m_DlgUserXX.ShowWindow(SW_HIDE);
	m_DlgUserLock.ShowWindow(SW_HIDE);
	m_DlgUserPass.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NAME)->SetWindowText("基本信息");
	m_btSample.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_ACTIVE.BMP"), false);
	m_btXX.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btLock.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btPass.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
}

void CDlgUserManage::OnXX()
{
	m_DlgUserSample.ShowWindow(SW_HIDE);
	m_DlgUserXX.ShowWindow(SW_SHOW);
	m_DlgUserLock.ShowWindow(SW_HIDE);
	m_DlgUserPass.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NAME)->SetWindowText("详细信息");
	m_btSample.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btXX.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_ACTIVE.BMP"), false);
	m_btLock.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btPass.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
}

void CDlgUserManage::OnLock()
{
	m_DlgUserSample.ShowWindow(SW_HIDE);
	m_DlgUserXX.ShowWindow(SW_HIDE);
	m_DlgUserLock.ShowWindow(SW_SHOW);
	m_DlgUserPass.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NAME)->SetWindowText("主机绑定");
	m_btSample.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btXX.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btLock.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_ACTIVE.BMP"), false);
	m_btPass.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
}

void CDlgUserManage::OnPass()
{
	m_DlgUserSample.ShowWindow(SW_HIDE);
	m_DlgUserXX.ShowWindow(SW_HIDE);
	m_DlgUserLock.ShowWindow(SW_HIDE);
	m_DlgUserPass.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NAME)->SetWindowText("登陆密码");
	m_btSample.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btXX.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btLock.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btPass.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_ACTIVE.BMP"), false);

}

void CDlgUserManage::InitRoomName()
{
	m_DlgUserSample.ShowWindow(SW_HIDE);
	m_DlgUserXX.ShowWindow(SW_HIDE);
	m_DlgUserLock.ShowWindow(SW_SHOW);
	m_DlgUserPass.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NAME)->SetWindowText("主机绑定");
	m_btSample.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btXX.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btLock.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_ACTIVE.BMP"), false);
	m_btPass.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
}

BOOL CDlgUserManage::OnEraseBkgnd( CDC* pDC )
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
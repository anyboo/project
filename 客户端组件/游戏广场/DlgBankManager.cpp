#include "stdafx.h"
#include"resource.h"
#include "DlgBankManager.h"
#include "GlobalUnits.h"


////////////////////////////////////////////////////////////////////////////////////
//宏定义
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//默认颜色
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgMoneyBank, CSkinDialogEx1)
	ON_WM_PAINT()
	//ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CLOSE, OnClose1)
	ON_BN_CLICKED(IDC_GIVE_MONEY, OnGive)
	ON_BN_CLICKED(IDC_RECORD, OnRecord)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_PASS, OnPass)
	ON_BN_CLICKED(IDC_SELECT, OnSelect)
//	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree1)

END_MESSAGE_MAP()

//交换函数
void CDlgMoneyBank::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CLOSE, m_btOk);
	DDX_Control(pDX, IDC_GIVE_MONEY, m_btGive);
	DDX_Control(pDX, IDC_RECORD, m_btRecord);
	DDX_Control(pDX, IDC_SAVE, m_btSave);
	DDX_Control(pDX, IDC_PASS, m_btPass);
	DDX_Control(pDX, IDC_SELECT, m_btSelect);
	//DDX_Control(pDX, IDCANCLE, m_btCancle);
	//DDX_Control(pDX, IDC_TREE, m_tree);
	
}

////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgMoneyBank::CDlgMoneyBank(CWnd* pParent): CSkinDialogEx1(IDD_BANK_MANAGER, pParent)//, CSkinWndObject(this)
{
	if (!m_pngImageBack.IsNull())
	{
		m_pngImageBack.DestroyImage();
	}
	m_pngImageBack.LoadImage(szResPath("BankDlg\\BankDlgBack.png"));
}

//虚构函数
CDlgMoneyBank::~CDlgMoneyBank()
{
	if (m_brBkground.m_hObject != NULL ) m_brBkground.DeleteObject();

	if (!m_pngImageBack.IsNull())
	{
		m_pngImageBack.DestroyImage();
	}
}

//初始函数
BOOL CDlgMoneyBank::OnInitDialog()
{
	__super::OnInitDialog();

	if(m_brBkground.m_hObject != NULL) m_brBkground.DeleteObject();

	//创建刷子
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);
	//m_SkinAttribute.m_brBackGround.CreateSolidBrush(RGB(218,222,223));;

	//m_brBackFrame.CreateSolidBrush(BGROUND_COLOR);;

	SetWindowPos(NULL, 0,0,m_pngImageBack.GetWidth(), m_pngImageBack.GetHeight(),SWP_NOZORDER);
	CenterWindow();
	CRect rcClient;
	GetWindowRect(&rcClient);
	ScreenToClient(&rcClient);

	CRect rcBankDlgIni;
	CGameFrame::GetRectFromIni(szResPath("BankDlg\\BankDlg.ini"), TEXT("BankDlg"), TEXT("ItemRect"), rcBankDlgIni, rcBankDlgIni);
	GetDlgItem(IDC_SHOW)->SetWindowPos(NULL, rcBankDlgIni.left, rcBankDlgIni.top, rcBankDlgIni.Width(),rcBankDlgIni.Height(),SWP_NOZORDER);

	static CRect rect;
	if(rect.IsRectNull())
	{
		//分隔符窗口
		CRect rectH;  //水平
		CRect rectV;  //垂直
		GetDlgItem(IDC_SHOW)->GetWindowRect(&rectH);
		
		//GetDlgItem(IDC_SEP2)->GetWindowRect(&rectV);
		rect.left= rectH.left+10;
		rect.bottom=rectH.bottom-10;
		rect.right=rectH.right-10;
		rect.top=rectH.top+15;

		ScreenToClient(&rect);
	}

	CPoint	ptBtnIni;
	CGameFrame::GetPointFromIni(szResPath("BankDlg\\BankDlg.ini"), TEXT("BankDlg"), TEXT("ptBt"), ptBtnIni, ptBtnIni);
	UINT xPos=ptBtnIni.x;//GetXExcursionPos()+10;
	UINT yPox =ptBtnIni.y; //GetYExcursionPos()+10;
	CRect rcBtn;
	m_btGive.GetWindowRect(&rcBtn);
	UINT uStype = 37;
	m_btGive.SetWindowPos(NULL, xPos, yPox, 0, 0,SWP_NOSIZE);
	m_btRecord.SetWindowPos(NULL, xPos, yPox+uStype, 0, 0,SWP_NOSIZE);
	m_btSave.SetWindowPos(NULL, xPos, yPox+uStype*2, 0, 0,SWP_NOSIZE);
	m_btPass.SetWindowPos(NULL, xPos, yPox+uStype*3, 0,0 ,SWP_NOSIZE);
	m_btSelect.SetWindowPos(NULL, xPos, yPox+uStype*4, 0,0 ,SWP_NOSIZE);

	m_btOk.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_CANCLE.bmp"), false);
	m_btOk.SetWindowPos(NULL, rcClient.Width()-30, 3, 0, 0, SWP_NOSIZE);
	m_btOk.SetWindowText("");

//	CString strPoint;
//	strPoint.Format(_T("left=%d,top=%d"),rect.left,rect.top);
//	AfxMessageBox(strPoint);
	
	m_DlgMoneyGive.Create(IDD_MONEY_GIVE,this);
	m_DlgMoneyGive.InitRoomName();
	m_DlgMoneyGive.MoveWindow(rect);
//	m_DlgMoneyGive.ShowWindow(SW_SHOW);   //初始化为第一个对话框

	m_DlgMoneyManager.Create(IDD_MONEY_MANAGER,this);
//	m_DlgMoneyManager.InitRoomName();
	m_DlgMoneyManager.MoveWindow(rect);

	m_DlgMoneyRecord.Create(IDD_RECORD,this);
	m_DlgMoneyRecord.MoveWindow(rect);

	m_DlgMoneySelect.Create(IDD_SELECT,this);
	m_DlgMoneySelect.MoveWindow(rect);
	
	m_DlgMoneyPass.Create(IDD_CHANGE_PASS,this);
	m_DlgMoneyPass.MoveWindow(rect);

	//默认为第一个
	OnGive();

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
void CDlgMoneyBank::OnPaint()
{
//	CPaintDC dc(this);

	//绘画标题
//	__super::DrawSkinView(&dc);
	//绘画边框
// 	DrawBorder(&dc);
// 	//绘画标题
// 	DrawCaption(&dc);
// 	//绘画背景
// 	DrawBackGround(&dc);

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
}

//颜色处理
HBRUSH CDlgMoneyBank::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
		
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(BGROUND_COLOR);
			return m_brBkground;
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}


//销毁消息
void CDlgMoneyBank::OnClose()
{
	if(!g_GlobalOption.m_bCloseQuery)
	{
		//if(!g_GlobalOption.m_BankCloseTip)
			m_DlgMoneyClose.DoModal();
	}

	m_DlgMoneyGive.OnClose();
	m_DlgMoneyManager.OnClose();
	m_DlgMoneyRecord.OnClose();
	m_DlgMoneyPass.OnClose();
	m_DlgMoneySelect.OnClose();
	__super::OnClose();
	DestroyWindow();
}


//销毁消息
void CDlgMoneyBank::OnClose1()
{
	__super::OnCancel();
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


void CDlgMoneyBank::OnGive()
{
	m_DlgMoneyGive.ShowWindow(SW_SHOW);
	m_DlgMoneyManager.ShowWindow(SW_HIDE);
	m_DlgMoneyRecord.ShowWindow(SW_HIDE);
	m_DlgMoneyPass.ShowWindow(SW_HIDE);
	m_DlgMoneySelect.ShowWindow(SW_HIDE);
	m_btGive.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_ACTIVE.BMP"), false);
	m_btRecord.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btSelect.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btSave.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btPass.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
}

void CDlgMoneyBank::OnRecord()
{
	m_DlgMoneyGive.ShowWindow(SW_HIDE);
	m_DlgMoneyManager.ShowWindow(SW_HIDE);
	m_DlgMoneyRecord.ShowWindow(SW_SHOW);
	m_DlgMoneyPass.ShowWindow(SW_HIDE);
	m_DlgMoneySelect.ShowWindow(SW_HIDE);
	m_btGive.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btRecord.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_ACTIVE.BMP"), false);
	m_btSelect.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btSave.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btPass.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
}

void CDlgMoneyBank::OnSave()
{
	m_DlgMoneyGive.ShowWindow(SW_HIDE);
	m_DlgMoneyManager.ShowWindow(SW_SHOW);
	m_DlgMoneyManager.InitRoomName();
	m_DlgMoneyRecord.ShowWindow(SW_HIDE);
	m_DlgMoneyPass.ShowWindow(SW_HIDE);
	m_DlgMoneySelect.ShowWindow(SW_HIDE);
	m_btGive.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btRecord.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btSelect.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btSave.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_ACTIVE.BMP"), false);
	m_btPass.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
}

void CDlgMoneyBank::OnPass()
{
	m_DlgMoneyGive.ShowWindow(SW_HIDE);
	m_DlgMoneyManager.ShowWindow(SW_HIDE);
	m_DlgMoneyRecord.ShowWindow(SW_HIDE);
	m_DlgMoneyPass.ShowWindow(SW_SHOW);
	m_DlgMoneySelect.ShowWindow(SW_HIDE);
	m_btGive.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btRecord.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btSelect.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btSave.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btPass.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_ACTIVE.BMP"), false);
}
//销毁消息
void CDlgMoneyBank::OnSelect()
{
	m_DlgMoneyGive.ShowWindow(SW_HIDE);
	m_DlgMoneyManager.ShowWindow(SW_HIDE);
	m_DlgMoneyRecord.ShowWindow(SW_HIDE);
	m_DlgMoneyPass.ShowWindow(SW_HIDE);
	m_DlgMoneySelect.ShowWindow(SW_SHOW);
	m_btGive.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btRecord.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btSelect.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_ACTIVE.BMP"), false);
	m_btSave.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btPass.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
}

BOOL CDlgMoneyBank::OnEraseBkgnd( CDC* pDC )
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////
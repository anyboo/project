#include "stdafx.h"
#include "resource.h"
#include "DlgLockMachine.h"
#include ".\dlglockmachine.h"


IMPLEMENT_DYNAMIC(CDlgLockMachine, CSkinDialogEx)
CDlgLockMachine::CDlgLockMachine(CWnd* pParent /*=NULL*/)
	: CSkinDialogEx(IDD_LOCK, pParent)
{
	m_bLock=true;
}

CDlgLockMachine::~CDlgLockMachine()
{
}

void CDlgLockMachine::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX,IDOK,m_btOK);
	DDX_Control(pDX,IDCANCEL,m_btCancel);
	//DDX_Text(pDX,IDC_PASSWORD,m_strPassword);
	DDX_Control(pDX, IDC_PASSWORD, m_edUserPassword);
	DDX_Text(pDX,IDC_DESCRIBE,m_strDescribe);		
}


BEGIN_MESSAGE_MAP(CDlgLockMachine, CSkinDialogEx)
	ON_WM_CTLCOLOR()
	//ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgLockMachine message handlers

HBRUSH CDlgLockMachine::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
	//if (CTLCOLOR_STATIC==nCtlColor && pWnd->GetSafeHwnd()!=GetDlgItem(IDC_BAK_PASSWORD)->GetSafeHwnd())
	//{
	//	pDC->SetTextColor(RGB(0,0,0));
	//	return m_SkinAttribute.m_brBackGround;
	//}
	
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CDlgLockMachine::OnInitDialog()
{
	__super::OnInitDialog();

	if (m_bLock) 
	{
		SetWindowText(TEXT("绑定操作"));
		SetDlgItemText(IDOK,TEXT("锁定机器"));
	}
	else
	{
		SetWindowText(TEXT("解锁操作"));
		SetDlgItemText(IDOK,TEXT("解锁机器"));
	}

	//设置变量
	m_strPassword=TEXT("");

	//更新变量
	UpdateData(FALSE);
    

	//设置焦点
	GetDlgItem(IDC_PASSWORD)->SetFocus();

	//m_ImageKuang.LoadFromResource(AfxGetInstanceHandle(),IDB_LOCK_KUANG);
	CRect rcFrame;
	GetWindowRect(&rcFrame);
	MoveWindow(&rcFrame,FALSE);
    
   // m_btCancel.SetButtonImage(IDB_FACE_CLOSE,AfxGetInstanceHandle(),false);
	//SetWindowPos(NULL,0,0,290,200,SWP_NOMOVE|SWP_NOZORDER|SWP_NOCOPYBITS);
	//m_ImageBuffer.Destroy();
    //m_ImageBuffer.Create(290,200,16);
    
	//HDWP hDwp=BeginDeferWindowPos(0);
    
	//DeferWindowPos(hDwp,m_btCancel,NULL,249,1,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);
    
	//EndDeferWindowPos(hDwp);
	m_btCancel.ShowWindow(0);

	return TRUE;
}

void CDlgLockMachine::OnOK()
{
	UpdateData(TRUE);

	GetDlgItemText(IDC_PASSWORD,m_szPassword1,CountArray(m_szPassword1));

	
	m_strPassword.Format(TEXT("%s"),m_szPassword1);
	if (m_strPassword.IsEmpty())
	{
		MessageBox(TEXT("请输入银行密码"),TEXT("系统提示"));
		GetDlgItem(IDC_PASSWORD)->SetFocus();
		return ;
	}
    
	CSkinDialogEx::OnOK();
}
//重画消息
// void CDlgLockMachine::OnPaint()
// {
// 	//创建 DC
// 	CPaintDC dc(this);
//     
// 	//获取区域
// // 	CRect rcClient,ClipRect;
// // 	dc.GetClipBox(&ClipRect);
// // 	GetClientRect(&rcClient);
// //     
// // 	CDC * pDC=CDC::FromHandle(m_ImageBuffer.GetDC());
// // 
// // 	pDC->SetBkMode(TRANSPARENT);
// // 	m_ImageKuang.AlphaDrawImage(pDC,rcClient.left,rcClient.top,RGB(255,0,255));
// // 	m_ImageBuffer.BitBlt(dc,rcClient.left,rcClient.top,547,349,0,0);
// // 	
// //     m_ImageBuffer.ReleaseDC();
// 	CSkinDialogEx::OnPaint();
// 	
// }

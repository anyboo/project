#include "stdafx.h"
#include "resource.h"
#include "DlgBankPass.h"
#include "GlobalUnits.h"
#include "GameFrame.h"
#include "DlgOption.h"
#include ".\dlglockmachine.h"


IMPLEMENT_DYNAMIC(CDlgBankPass, CSkinDialogEx1)
CDlgBankPass::CDlgBankPass(CWnd* pParent /*=NULL*/)
	: CSkinDialogEx(IDD_BANK_PASS, pParent)
{
	m_tmLogin = NULL;
	m_bIsLeave =false;
}

CDlgBankPass::~CDlgBankPass()
{

}

void CDlgBankPass::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX,IDOK,m_btOK);
	DDX_Control(pDX,IDCANCEL,m_btCancel);
//	DDX_Control(pDX,IDC_CANCEL2,m_btCancel2);
	//DDX_Text(pDX,IDC_PASSWORD,m_strPassword);
	DDX_Control(pDX, IDC_PASSWORD, m_edUserPassword);

//	DDX_Text(pDX,IDC_DESCRIBE,m_strDescribe);		
}


BEGIN_MESSAGE_MAP(CDlgBankPass, CSkinDialogEx)
	//ON_WM_CTLCOLOR()
	//ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CANCEL2, OnCancel)
END_MESSAGE_MAP()


// CDlgLockMachine message handlers

HBRUSH CDlgBankPass::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

BOOL CDlgBankPass::OnInitDialog()
{
	__super::OnInitDialog();

	SetWindowText(TEXT("保险箱密码"));

	if(m_tmLogin == NULL) 
		m_tmLogin = CTime::GetCurrentTime();

	switch(g_GlobalOption.m_BankCloseMode)
	{
	case enBankRule_LK:
		break;
	case enBankRule_ZD:
		{
			CTimeSpan ts = CTime::GetCurrentTime() - m_tmLogin;
			int ipass=ts.GetSeconds();
			if( ipass <= g_GlobalOption.m_BankCloseTime)
			{
				m_tmLogin = CTime::GetCurrentTime();
				__super::OnOK();
				return TRUE;
			}
			break;
		}	
		
	case enBankRule_LEAVE:		//离开大厅时注销
		CGameFrame* pFrame = (CGameFrame*)AfxGetMainWnd();
		if(pFrame->m_bIsLogBank == true)
		{
			__super::OnOK();
			return TRUE;
		}
		break;
	}
	
	//设置变量
	m_strPassword=TEXT("");

	//更新变量
	UpdateData(FALSE);
    
	//设置焦点
	GetDlgItem(IDC_PASSWORD)->SetFocus();

	if(!m_ImageKuang.IsNull())
		m_ImageKuang.Destroy();
	m_ImageKuang.LoadFromResource(AfxGetInstanceHandle(),IDB_LOCK_KUANG);
	CRect rcFrame;
	GetWindowRect(&rcFrame);
	MoveWindow(&rcFrame,FALSE);
    
  //  m_btCancel.SetButtonImage(IDB_FACE_CLOSE,AfxGetInstanceHandle(),false);
	//m_btCancel.load
	SetWindowPos(NULL,0,0,290,200,SWP_NOMOVE|SWP_NOZORDER|SWP_NOCOPYBITS);
	m_ImageBuffer.Destroy();
    m_ImageBuffer.Create(290,200,16);

	//
	//m_btCancel2.LoadStdImage((L"skin/bt_close.png"));
    
	HDWP hDwp=BeginDeferWindowPos(0);
    
//	DeferWindowPos(hDwp,m_btCancel2,NULL,249,1,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);
    
	EndDeferWindowPos(hDwp);

	return TRUE;
}

void CDlgBankPass::OnOK()
{
	UpdateData(TRUE);

	//GetDlgItemText(IDC_PASSWORD,m_strPassword,CountArray(m_strPassword));
	m_edUserPassword.GetUserPassword(m_szPassword1);
	
	m_strPassword.Format(TEXT("%s"),m_szPassword1);
	if (m_strPassword.IsEmpty())
	{
		MessageBox(TEXT("请输入保险箱密码"),TEXT("系统提示"));
		GetDlgItem(IDC_PASSWORD)->SetFocus();
		return ;
	}

	tagGlobalUserData &GlobalUserData=g_GlobalUnits.GetGolbalUserData();
	TCHAR szBankpass[PASS_LEN]={0};

	CMD5Encrypt::EncryptData(m_strPassword,szBankpass);

				//	if(szBankpass==GlobalUserData.szBankPass)
				//		{}
	if (lstrcmp(GlobalUserData.szBankPass,szBankpass)!=0)
	{
			//	ShowMessageBox(_T("对不起您输入的银行密码不对。请重新输入"),MB_ICONINFORMATION);
				ShowInformationEx(_T("对不起您输入的保险箱密码不对。请重新输入"),0,MB_ICONINFORMATION,TEXT("游戏大厅"));
				return ;
	}

    m_tmLogin = CTime::GetCurrentTime();
	__super::OnOK();
}

void CDlgBankPass::OnCancel()
{
	__super::OnCancel();	
}
//重画消息
void CDlgBankPass::OnPaint()
{
	//创建 DC
	CPaintDC dc(this);
    
	//获取区域
	CRect rcClient,ClipRect;
	dc.GetClipBox(&ClipRect);
	GetClientRect(&rcClient);
    
	CDC * pDC=CDC::FromHandle(m_ImageBuffer.GetDC());

	pDC->SetBkMode(TRANSPARENT);
	m_ImageKuang.AlphaDrawImage(pDC,rcClient.left,rcClient.top,RGB(255,0,255));

//	m_btCancel2.SetBkGnd(pDC);
	m_ImageBuffer.BitBlt(dc,rcClient.left,rcClient.top,547,349,0,0);
	
    m_ImageBuffer.ReleaseDC();
	
}

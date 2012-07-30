#include "stdafx.h"
#include"resource.h"
#include "DlgSafeTip.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"

////////////////////////////////////////////////////////////////////////////////////
//宏定义
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//默认颜色
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgSafeTip, CSkinDialogEx)
	//ON_WM_PAINT()
	//ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDC_SET, OnSet)
	ON_BN_CLICKED(IDOK, OnOk)
	
	
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgSafeTip::CDlgSafeTip(CWnd* pParent): CSkinDialogEx(IDD_SAFE_SET, pParent)//, CSkinWndObject(this)
{
	//设置变量

}

//虚构函数
CDlgSafeTip::~CDlgSafeTip()
{
	if (m_brBkground.m_hObject != NULL ) m_brBkground.DeleteObject();
}

//交换函数
void CDlgSafeTip::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_BtOk);
	DDX_Control(pDX, IDCANCLE, m_btCancle);
	DDX_Control(pDX, IDC_SET, m_btSet);
}

//初始函数
BOOL CDlgSafeTip::OnInitDialog()
{
	__super::OnInitDialog();

	//创建刷子
	if (m_brBkground.m_hObject != NULL ) m_brBkground.DeleteObject();

	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//更新控件
	UpdateControls();

	//初始化设置
	//__super::InitSkinObject();

	return TRUE;  
}

//绘画函数
void CDlgSafeTip::OnPaint()
{
	CPaintDC dc(this);

	
	//绘画标题
//	__super::DrawSkinView(&dc);
}


//显示消息
void CDlgSafeTip::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("安全提示"));

	return;
}



//更新控件
void CDlgSafeTip::UpdateControls()
{
	//变量定义
	UpdateData(false);

}

//颜色处理
HBRUSH CDlgSafeTip::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

//显示消息
void CDlgSafeTip::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//更新控件
	if ( bShow ) UpdateControls();
}

//销毁消息
void CDlgSafeTip::OnClose()
{


	CDialog::OnClose();
}


void CDlgSafeTip::OnOk()
{
	//if ( m_DlgUserManager1.m_hWnd == NULL )
	//{
	//	m_DlgUserManager1.Create(IDD_USER_MANAGER, GetParent());
	//}
	////m_DlgUserManager.DoModal();
	//m_DlgUserManager1.CenterWindow();
	//m_DlgUserManager1.ShowWindow(SW_SHOW);
	//m_DlgUserManager1.SetActiveWindow();
	//m_DlgUserManager1.SetForegroundWindow();
	//m_DlgUserManager1.InitRoomName();
	//CDialog::OnOK();
	::SendMessage(GetParent()->m_hWnd,WM_COMMAND,IDC_SHOW_TIP,0);
}


void CDlgSafeTip::OnSet()
{

//m_DlgUserManager1.DoModal();
	//CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	//
	//if ( pGameFrame->m_DlgUserManager.m_hWnd == NULL )
	//{
	//	 pGameFrame->m_DlgUserManager.Create(IDD_USER_MANAGER, this);
	//}
	////m_DlgUserManager.DoModal();
	// pGameFrame->m_DlgUserManager.CenterWindow();
	// pGameFrame->m_DlgUserManager.ShowWindow(SW_SHOW);
	// pGameFrame->m_DlgUserManager.SetActiveWindow();
	// pGameFrame->m_DlgUserManager.SetForegroundWindow();
	// pGameFrame->m_DlgUserManager.InitRoomName();
	::SendMessage(GetParent()->m_hWnd,WM_COMMAND,IDC_SHOW_TIP,0);
	//CDialog::OnOK();
}

////////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include"resource.h"
#include "DlgMoneySelect.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"

////////////////////////////////////////////////////////////////////////////////////
//宏定义
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//默认颜色
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgMoneySelect, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDC_EDIT, OnBnClickedEdit)
	ON_BN_CLICKED(IDC_QUERY, OnBnClickedQuery)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgMoneySelect::CDlgMoneySelect(CWnd* pParent): CDialog(IDD_SELECT, pParent)//, CSkinWndObject(this)
{
	//设置变量
//	m_pIClientSocket = NULL;
}

//虚构函数
CDlgMoneySelect::~CDlgMoneySelect()
{
	if(m_brBkground.GetSafeHandle()) m_brBkground.DeleteObject();
}

//交换函数
void CDlgMoneySelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SELECT, m_boxTimeSecect);
	DDX_Control(pDX, IDC_EDIT, m_btEdit);
	
}

//初始函数
BOOL CDlgMoneySelect::OnInitDialog()
{
	__super::OnInitDialog();

	//创建刷子
	if(m_brBkground.GetSafeHandle()) m_brBkground.DeleteObject();
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//设置标题
	SetWindowText(TEXT("欢乐豆赠送"));



	m_boxTimeSecect.Clear();
	m_boxTimeSecect.InsertString(0,"1");
	m_boxTimeSecect.InsertString(1,"3");
	m_boxTimeSecect.InsertString(2,"5");
	m_boxTimeSecect.InsertString(3,"10");
	m_boxTimeSecect.InsertString(4,"30");
	m_boxTimeSecect.InsertString(5,"60");
	m_boxTimeSecect.SetCurSel(0);

	SetButtonSelected(IDC_QUERY,g_GlobalOption.m_bCloseQuery);
	if(!g_GlobalOption.m_bCloseQuery)
	{
		GetDlgItem(IDC_LK)->EnableWindow(false);	
		GetDlgItem(IDC_ZD)->EnableWindow(false);	
		GetDlgItem(IDC_LEAVE)->EnableWindow(false);	
		GetDlgItem(IDC_SELECT)->EnableWindow(false);	
		GetDlgItem(IDC_DESC)->EnableWindow(false);	
	}
	else
	{
		if(g_GlobalOption.m_BankCloseMode==enBankRule_LK)
		{
			SetButtonSelected(IDC_LK,true);
		}
		else if(g_GlobalOption.m_BankCloseMode==enBankRule_ZD)
		{
			SetButtonSelected(IDC_ZD,true);
			//m_boxTimeSecect.SelectString(0, )
			CString strTime;
			strTime.Format("%d", g_GlobalOption.m_BankCloseTime);
			m_boxTimeSecect.SelectString(0, strTime);
			//g_GlobalOption.m_BankCloseTime

		}
		else
		{
			SetButtonSelected(IDC_LEAVE,true);
		}
		GetDlgItem(IDC_LK)->EnableWindow(TRUE);	
		GetDlgItem(IDC_ZD)->EnableWindow(TRUE);	
		GetDlgItem(IDC_LEAVE)->EnableWindow(TRUE);	
		GetDlgItem(IDC_SELECT)->EnableWindow(TRUE);	
		GetDlgItem(IDC_DESC)->EnableWindow(TRUE);	
	}

	//更新控件
	UpdateControls();

	//初始化设置
	//__super::InitSkinObject();

	return TRUE;  
}

//绘画函数
void CDlgMoneySelect::OnPaint()
{
	CPaintDC dc(this);

	
	//绘画标题
//	__super::DrawSkinView(&dc);
}


//显示消息
void CDlgMoneySelect::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("保险箱管理"));
	return;
}

//网络链接
bool CDlgMoneySelect::ConnectServer()
{
	//if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateMoneyStatus_NULL )
	//{
	//	ShowMessage(TEXT("操作正在进行，请稍后！"));
	//	return false;
	//}

	//if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT )//?GetSocketStatus
	//{
	//	SendData();
	//	return true;
	//}

	////获取大厅
	//CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	//CPlazaViewItem * pPlazaViewItem = &(pGameFrame->m_DlgGamePlaza) ;
	//if ( m_pIClientSocket == NULL )
	//{

	//	//合法判断
	//	if ( pPlazaViewItem->m_ClientSocket.GetInterface() == NULL ) 
	//	{
	//		ShowMessage(TEXT("网络组件还没有创建，请重新登录！"));
	//		return false;
	//	}

	//	//设置变量
	//	m_pIClientSocket = (ITCPSocket*)pPlazaViewItem->m_ClientSocket.GetInterface();
	//}

	////地址解释
	//CRegKey RegServer;
	//LPCTSTR pszServerIP=pPlazaViewItem->m_DlgLogon.GetLogonServer();
	//TCHAR szRegServer[256]=TEXT(""),szServerAddr[64]=TEXT("");
	//_snprintf(szRegServer,sizeof(szRegServer),TEXT("%s\\%s"),REG_LOGON_SERVER,pszServerIP);

	//if (RegServer.Open(HKEY_CURRENT_USER,szRegServer,KEY_READ)==ERROR_SUCCESS)
	//{
	//	TCHAR szReadData[1024]=TEXT("");
	//	DWORD dwReadData=0L,dwDataType=0L,dwDataSize=sizeof(szReadData);
	//	LONG lErrorCode=RegServer.QueryValue(TEXT("ServerAddr"),&dwDataType,szReadData,&dwDataSize);
	//	if (lErrorCode==ERROR_SUCCESS) 
	//	{
	//		CXOREncrypt::CrevasseData(szReadData,szServerAddr,sizeof(szServerAddr));
	//		pszServerIP=szServerAddr;
	//	}
	//}

	////连接服务器
	//try
	//{
	//	//连接服务器
	//	m_pIClientSocket->CloseSocket();
	//	if (m_pIClientSocket->Connect(pszServerIP,PORT_LOGON_SERVER)!=ERROR_SUCCESS)
	//	{
	//		throw TEXT("服务器连接错误，可能是你的系统还没有成功连接上网络！");
	//	}
	//}
	//catch (LPCTSTR pszError)
	//{
	//	ShowMessage((TCHAR*)pszError);
	//	return false;
	//}

	return true;
}


//发送数据
bool CDlgMoneySelect::SendData()
{
	
	////状态判断
	//ASSERT(m_enOperateStatus != enOperateMoneyGiveStatus_NULL);
	//if ( m_enOperateStatus == enOperateMoneyGiveStatus_NULL ) return false;

	//if ( m_pIClientSocket == NULL || m_pIClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT ) return false;

	//if ( m_enOperateStatus == enOperateGiveStatus_Save )
	//{
	//	CMD_GP_MoneyGive GiveMoney;
	//	ZeroMemory(&GiveMoney, sizeof(GiveMoney));

	//	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	//	GiveMoney.dwUserID = GlobalUserData.dwUserID;
	//	GiveMoney.lScore = GetDlgItemInt(IDC_SCORE);
	//	CString strNickName;
	//	GetDlgItem(IDC_USER)->GetWindowText(strNickName);
	//	strcpy(GiveMoney.szNickname,strNickName);
	//	GiveMoney.cbType = m_UserSelect.GetCurSel();

	//	//投递消息
	//	m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_GIVE_MONEY, &GiveMoney, sizeof(GiveMoney));        

	//	//更新界面
	//	InvalidateRect(NULL);
	//}
	//
	//else if ( m_enOperateStatus == enOperateGiveStatus_Refresh )
	//{
	//	CMD_GP_MoneyOper GetMoney;
	//	ZeroMemory(&GetMoney, sizeof(GetMoney));

	//	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	//	GetMoney.dwUserID = GlobalUserData.dwUserID;
	//	GetMoney.cbType = 2;
	//	GetMoney.lScore = 0;
	//	GetMoney.dwKindID = 0;
	//	//投递消息
	//	m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_OPEN_MONEY, &GetMoney, sizeof(GetMoney));        

	//	//更新界面
	//	InvalidateRect(NULL);
	//}

	////设置状态
	//m_enOperateStatus = enOperateMoneyGiveStatus_NULL ;

	return true;
}

//更新控件
void CDlgMoneySelect::UpdateControls()
{
	//变量定义
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	
	UpdateData(false);

}

//颜色处理
HBRUSH CDlgMoneySelect::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CDlgMoneySelect::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//更新控件
	if ( bShow ) UpdateControls();
}

//销毁消息
void CDlgMoneySelect::OnClose()
{
// 	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateMoneyStatus_NULL )
// 	{
// 		m_pIClientSocket->CloseSocket();
// 	}

	CDialog::OnClose();
		DestroyWindow();
}


//初始化房间
bool CDlgMoneySelect::InitRoomName()
{
	

	return true;
}


////////////////////////////////////////////////////////////////////////////////////
//是否选中
bool CDlgMoneySelect::IsButtonSelected(UINT uButtonID)
{
	return ((CButton *)GetDlgItem(uButtonID))->GetCheck()==BST_CHECKED;
}

//选中按钮
void CDlgMoneySelect::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	CButton * pButton=(CButton *)GetDlgItem(uButtonID);
	if (bSelected) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);
	return;
}


void CDlgMoneySelect::OnBnClickedEdit()
{
	g_GlobalOption.m_bCloseQuery=IsButtonSelected(IDC_QUERY);

	//邀请模式
	if (IsButtonSelected(IDC_LK)) 
	{
		g_GlobalOption.m_BankCloseMode=enBankRule_LK;
	}
	else if (IsButtonSelected(IDC_ZD)) 
	{
		g_GlobalOption.m_BankCloseMode=enBankRule_ZD;
		CString strTime;
		GetDlgItemText(IDC_SELECT,strTime);
		g_GlobalOption.m_BankCloseTime = atoi(strTime.GetBuffer(0));
	}
	else 
		g_GlobalOption.m_BankCloseMode=enBankRule_LEAVE;

	ShowMessage(TEXT("保存成功"));
}

void CDlgMoneySelect::OnBnClickedQuery()
{
	if(IsButtonSelected(IDC_QUERY))
	{
		GetDlgItem(IDC_LK)->EnableWindow(true);	
		GetDlgItem(IDC_ZD)->EnableWindow(true);	
		GetDlgItem(IDC_LEAVE)->EnableWindow(true);	
		GetDlgItem(IDC_SELECT)->EnableWindow(true);	
		GetDlgItem(IDC_DESC)->EnableWindow(true);		
	}
	else
	{
		GetDlgItem(IDC_LK)->EnableWindow(false);	
		GetDlgItem(IDC_ZD)->EnableWindow(false);	
		GetDlgItem(IDC_LEAVE)->EnableWindow(false);	
		GetDlgItem(IDC_SELECT)->EnableWindow(false);	
		GetDlgItem(IDC_DESC)->EnableWindow(false);		
	}
}
#include "stdafx.h"
#include"resource.h"
#include "DlgMoneyPass.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"

////////////////////////////////////////////////////////////////////////////////////
//宏定义
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//默认颜色
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgMoneyPass, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDC_EDIT, OnBnClickedEdit)

END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgMoneyPass::CDlgMoneyPass(CWnd* pParent): CDialog(IDD_CHANGE_PASS, pParent)//, CSkinWndObject(this)
{
	//设置变量

	m_pIClientSocket = NULL;
	m_enOperateStatus=enOperateMoneyPassStatus_NULL;
	
}

//虚构函数
CDlgMoneyPass::~CDlgMoneyPass()
{
	if(m_brBkground.GetSafeHandle()) m_brBkground.DeleteObject();
}

//交换函数
void CDlgMoneyPass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, m_btEdit);
}

//初始函数
BOOL CDlgMoneyPass::OnInitDialog()
{
	__super::OnInitDialog();

	//创建刷子
	if(m_brBkground.GetSafeHandle()) m_brBkground.DeleteObject();
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//设置标题
	SetWindowText(TEXT("欢乐豆赠送"));

	//更新控件
	UpdateControls();

	//初始化设置
	//__super::InitSkinObject();

	return TRUE;  
}

//绘画函数
void CDlgMoneyPass::OnPaint()
{
	CPaintDC dc(this);

	
	//绘画标题
//	__super::DrawSkinView(&dc);
}


//显示消息
void CDlgMoneyPass::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("赠送欢乐豆"));

	return;
}

//网络链接
bool CDlgMoneyPass::ConnectServer()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateMoneyPassStatus_NULL )
	{
		ShowMessage(TEXT("操作正在进行，请稍后！"));
		return false;
	}

	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT )//?GetSocketStatus
	{
		SendData();
		return true;
	}

	//获取大厅
	CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	CPlazaViewItem * pPlazaViewItem = &(pGameFrame->m_DlgGamePlaza) ;
	if ( m_pIClientSocket == NULL )
	{

		//合法判断
		if ( pPlazaViewItem->m_ClientSocket.GetInterface() == NULL ) 
		{
			ShowMessage(TEXT("网络组件还没有创建，请重新登录！"));
			return false;
		}

		//设置变量
		m_pIClientSocket = (ITCPSocket*)pPlazaViewItem->m_ClientSocket.GetInterface();
	}

	//地址解释
	CRegKey RegServer;
	LPCTSTR pszServerIP=pPlazaViewItem->m_DlgLogon.GetLogonServer();
	TCHAR szRegServer[256]=TEXT(""),szServerAddr[64]=TEXT("");
	_snprintf(szRegServer,sizeof(szRegServer),TEXT("%s\\%s"),REG_LOGON_SERVER,pszServerIP);

	if (RegServer.Open(HKEY_CURRENT_USER,szRegServer,KEY_READ)==ERROR_SUCCESS)
	{
		TCHAR szReadData[1024]=TEXT("");
		DWORD dwReadData=0L,dwDataType=0L,dwDataSize=sizeof(szReadData);
		LONG lErrorCode=RegServer.QueryValue(TEXT("ServerAddr"),&dwDataType,szReadData,&dwDataSize);
		if (lErrorCode==ERROR_SUCCESS) 
		{
			CXOREncrypt::CrevasseData(szReadData,szServerAddr,sizeof(szServerAddr));
			pszServerIP=szServerAddr;
		}
	}

	//连接服务器
	try
	{
		//连接服务器
		m_pIClientSocket->CloseSocket();
		if (m_pIClientSocket->Connect(pszServerIP,PORT_LOGON_SERVER)!=ERROR_SUCCESS)
		{
			throw TEXT("服务器连接错误，可能是你的系统还没有成功连接上网络！");
		}
	}
	catch (LPCTSTR pszError)
	{
		ShowMessage((TCHAR*)pszError);
		return false;
	}

	return true;
}


//发送数据
bool CDlgMoneyPass::SendData()
{
	
	//状态判断
	ASSERT(m_enOperateStatus != enOperateMoneyPassStatus_NULL);
	if ( m_enOperateStatus == enOperateMoneyPassStatus_NULL ) return false;

	if ( m_pIClientSocket == NULL || m_pIClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT ) return false;

	if ( m_enOperateStatus == enOperateEditStatus_Save )
	{
		CMD_GP_EditBankPass EditBankPass;
		ZeroMemory(&EditBankPass, sizeof(EditBankPass));

		tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
		EditBankPass.dwUserID = GlobalUserData.dwUserID;
		
		CString strPass;
		GetDlgItem(IDC_OLD_PASSWORD)->GetWindowText(strPass);

		TCHAR szBankpass[33];
		CMD5Encrypt::EncryptData(strPass,szBankpass);

		strcpy(EditBankPass.szPassword,szBankpass);

		GetDlgItem(IDC_NEW_PASSWORD)->GetWindowText(strPass);

		CMD5Encrypt::EncryptData(strPass,szBankpass);

		strcpy(EditBankPass.szPassword1,szBankpass);
		//投递消息
		m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_EDIT_BANK_PASS, &EditBankPass, sizeof(EditBankPass));        

		//更新界面
		InvalidateRect(NULL);
	}
	
	
	////设置状态
	m_enOperateStatus = enOperateMoneyPassStatus_NULL ;

	return true;
}

//更新控件
void CDlgMoneyPass::UpdateControls()
{
	//变量定义
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	
	UpdateData(false);

}

//颜色处理
HBRUSH CDlgMoneyPass::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CDlgMoneyPass::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//更新控件
	if ( bShow ) UpdateControls();
}

//销毁消息
void CDlgMoneyPass::OnClose()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateMoneyStatus_NULL )
	{
		m_pIClientSocket->CloseSocket();
	}
	CDialog::OnClose();
	DestroyWindow();
}


//初始化房间
bool CDlgMoneyPass::InitRoomName()
{
	

	return true;
}

void CDlgMoneyPass::OnBnClickedEdit()
{
	CString strBuffer;
	
	TCHAR		m_szPassword1[PASS_LEN];
	TCHAR		m_szPassword2[PASS_LEN];

	GetDlgItemText(IDC_NEW_PASSWORD,m_szPassword1,CountArray(m_szPassword1));
	GetDlgItemText(IDC_NEW_PASSWORD1,m_szPassword2,CountArray(m_szPassword2));

	if (lstrcmp(m_szPassword1,m_szPassword2)!=0)
	{
		ShowInformation(TEXT("游戏密码与确认密码不一致，请重新输入确认密码！"),0,MB_ICONQUESTION);
		GetDlgItem(IDC_NEW_PASSWORD1)->SetFocus();
		return;
	}

	m_enOperateStatus = enOperateEditStatus_Save;
	//链接网络
	if ( ConnectServer() == false )
	{
		//设置状态
		m_enOperateStatus = enOperateMoneyPassStatus_NULL;
		return;
	}
}


////////////////////////////////////////////////////////////////////////////////////
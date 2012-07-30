#include "stdafx.h"
#include"resource.h"
#include "DlgUserInfoLock.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"
#include "DlgBankPass.h"

////////////////////////////////////////////////////////////////////////////////////
//宏定义
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//默认颜色
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgUserLock, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDC_BIND, OnOK)

END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgUserLock::CDlgUserLock(CWnd* pParent): CDialog(IDD_USER_LOCK, pParent)//, CSkinWndObject(this)
{
	//设置变量

	m_pIClientSocket = NULL;
	m_enOperateStatus=enOperateUserLockStatus_NULL;
	
}

//虚构函数
CDlgUserLock::~CDlgUserLock()
{
	if (m_brBkground.m_hObject != NULL ) m_brBkground.DeleteObject();
}

//交换函数
void CDlgUserLock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BIND, m_btEdit);
}

//初始函数
BOOL CDlgUserLock::OnInitDialog()
{
	__super::OnInitDialog();

	//创建刷子
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//设置标题
//	SetWindowText(TEXT("欢乐豆赠送"));

	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	if(GlobalUserData.JiQi==1)
	{
		GetDlgItem(IDC_BIND)->SetWindowTextA("解锁");
		GetDlgItem(IDC_STATUS)->SetWindowTextA("当前状态: 锁定状态");
	}
	else
	{
		GetDlgItem(IDC_BIND)->SetWindowTextA("锁定");
		GetDlgItem(IDC_STATUS)->SetWindowTextA("当前状态: 解锁状态");
	}

	//更新控件
	UpdateControls();

	//初始化设置
	//__super::InitSkinObject();

	return TRUE;  
}

//绘画函数
void CDlgUserLock::OnPaint()
{
	CPaintDC dc(this);

	
	//绘画标题
//	__super::DrawSkinView(&dc);
}


//显示消息
void CDlgUserLock::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("赠送欢乐豆"));

	return;
}

//网络链接
bool CDlgUserLock::ConnectServer()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateUserLockStatus_NULL )
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
bool CDlgUserLock::SendData()
{
	
	////状态判断
	ASSERT(m_enOperateStatus != enOperateUserLockStatus_NULL);
	if ( m_enOperateStatus == enOperateUserLockStatus_NULL ) return false;

	if ( m_pIClientSocket == NULL || m_pIClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT ) return false;

	if ( m_enOperateStatus == enOperateUserLockStatus_Save )
	{
		tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
		if(GlobalUserData.JiQi==1)
		{
			CMD_GP_SafUnbind UnBind;
			ZeroMemory(&UnBind, sizeof(UnBind));

			tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
			UnBind.dwUserID = GlobalUserData.dwUserID;
			lstrcpy(UnBind.szPassWord, GlobalUserData.szPassWord);

			//投递消息
			m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_SAFE_UNBIND, &UnBind, sizeof(UnBind)); 
		}
		else
		{
			CMD_GP_SafeBind SafeBind;
			ZeroMemory(&SafeBind, sizeof(SafeBind));

			tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
			SafeBind.dwUserID = GlobalUserData.dwUserID;
			lstrcpy(SafeBind.szPassWord, GlobalUserData.szPassWord);
			g_GlobalUnits.GetClientSerial(SafeBind.ClientSerial);
					
			//投递消息
			m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_SAFE_BIND, &SafeBind, sizeof(SafeBind)); 
		}


		//更新界面
		InvalidateRect(NULL);
	}
	
	//设置状态
	m_enOperateStatus = enOperateUserLockStatus_NULL ;

	return true;
}

//更新控件
void CDlgUserLock::UpdateControls()
{
	//变量定义
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	UpdateData(false);
}

//颜色处理
HBRUSH CDlgUserLock::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CDlgUserLock::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//更新控件
	if ( bShow ) UpdateControls();
}

//销毁消息
void CDlgUserLock::OnClose()
{


	CDialog::OnClose();
}


//初始化房间
bool CDlgUserLock::InitRoomName()
{
	/*tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	
	m_RoomSelect.ResetContent();

	int nIndex=0;
	CListKind * pListKind=NULL;
	do
	{
		pListKind=g_GlobalUnits.m_ServerListManager.EnumKindItem(nIndex++);
		if (pListKind==NULL) break;
		m_RoomSelect.AddString(pListKind->m_GameKind.szKindName);
	} while (true);*/

	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	if(GlobalUserData.JiQi==1)
	{
		GetDlgItem(IDC_BIND)->SetWindowTextA("解锁");
		GetDlgItem(IDC_STATUS)->SetWindowTextA("当前状态: 锁定状态");
	}
	else
	{
		GetDlgItem(IDC_BIND)->SetWindowTextA("锁定");
		GetDlgItem(IDC_STATUS)->SetWindowTextA("当前状态: 解锁状态");
	}

	return true;
}

void CDlgUserLock::OnOK()
{
	CDlgBankPass DlgBankPass;

	if( DlgBankPass.DoModal()==IDOK)
	{
		m_enOperateStatus = enOperateUserLockStatus_Save;
		//链接网络
		if ( ConnectServer() == false )
		{
			//设置状态
			m_enOperateStatus = enOperateUserLockStatus_NULL;
			return;
		}
	}

}


////////////////////////////////////////////////////////////////////////////////////
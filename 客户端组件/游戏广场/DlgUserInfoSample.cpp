#include "stdafx.h"
#include"resource.h"
#include "DlgUserInfoSample.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"

////////////////////////////////////////////////////////////////////////////////////
//宏定义
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//默认颜色
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgUserSample, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDOK, OnOK)

END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgUserSample::CDlgUserSample(CWnd* pParent): CDialog(IDD_USER_JD_INFO, pParent)//, CSkinWndObject(this)
{
	//设置变量

	m_pIClientSocket = NULL;
	m_enOperateStatus=enOperateUserSampleStatus_NULL;
	
}

//虚构函数
CDlgUserSample::~CDlgUserSample()
{
	if (m_brBkground.m_hObject != NULL ) m_brBkground.DeleteObject();
}

//交换函数
void CDlgUserSample::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_GENDER, m_UserSelect);
	DDX_Control(pDX, IDOK, m_btEdit);
	
}

//初始函数
BOOL CDlgUserSample::OnInitDialog()
{
	__super::OnInitDialog();

	//创建刷子
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//设置标题
//	SetWindowText(TEXT("欢乐豆赠送"));

	m_UserSelect.InsertString(0,"男");
	m_UserSelect.InsertString(1,"女");
	m_UserSelect.InsertString(2,"未知");

	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();

	GetDlgItem(IDC_ACCOUNTS)->SetWindowText(GlobalUserInfo.szAccounts);

	CString strTemp;
	strTemp.Format("%ld",GlobalUserInfo.dwGameID);
	GetDlgItem(IDC_GAME_ID)->SetWindowText(strTemp);

	if(GlobalUserInfo.cbGender==0)
		m_UserSelect.SetCurSel(0);
	else if(GlobalUserInfo.cbGender==1)
		m_UserSelect.SetCurSel(1);
	else
		m_UserSelect.SetCurSel(2);

	GetDlgItem(IDC_DESC)->SetWindowText(GlobalUserInfo.szUnderWrite);

	//更新控件
	UpdateControls();

	//初始化设置
	//__super::InitSkinObject();

	return TRUE;  
}

//绘画函数
void CDlgUserSample::OnPaint()
{
	CPaintDC dc(this);

	
	//绘画标题
//	__super::DrawSkinView(&dc);
}


//显示消息
void CDlgUserSample::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("修改用户信息"));

	return;
}

//网络链接
bool CDlgUserSample::ConnectServer()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateUserSampleStatus_NULL )
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
bool CDlgUserSample::SendData()
{
	
	//状态判断
	ASSERT(m_enOperateStatus != enOperateUserSampleStatus_NULL);
	if ( m_enOperateStatus == enOperateUserSampleStatus_NULL ) return false;

	if ( m_pIClientSocket == NULL || m_pIClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT ) return false;

	if ( m_enOperateStatus == enOperateUserSampleStatus_Save )
	{
		CMD_GP_ModifyUserWrite ModifyWrite;
		ZeroMemory(&ModifyWrite, sizeof(ModifyWrite));

		tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
		ModifyWrite.dwUserID = GlobalUserData.dwUserID;
		
		CString strDesc;
		GetDlgItem(IDC_DESC)->GetWindowText(strDesc);
		lstrcpyn(ModifyWrite.szUnderWrite,strDesc, CountArray(ModifyWrite.szUnderWrite));
		
		//投递消息
		m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_MODIFY_WRITE, &ModifyWrite, sizeof(ModifyWrite));        

		//更新界面
		InvalidateRect(NULL);
	}
	
	
	//设置状态
	m_enOperateStatus = enOperateUserSampleStatus_NULL ;

	return true;
}

//更新控件
void CDlgUserSample::UpdateControls()
{
	//变量定义
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	UpdateData(false);
}

//颜色处理
HBRUSH CDlgUserSample::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CDlgUserSample::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//更新控件
	if ( bShow ) UpdateControls();
}

//销毁消息
void CDlgUserSample::OnClose()
{


	CDialog::OnClose();
}


//初始化房间
bool CDlgUserSample::InitRoomName()
{
	

	return true;
}

void CDlgUserSample::OnOK()
{
	m_enOperateStatus = enOperateUserSampleStatus_Save;
	//链接网络
	if ( ConnectServer() == false )
	{
		//设置状态
		m_enOperateStatus = enOperateUserSampleStatus_NULL;
		return;
	}
}

////////////////////////////////////////////////////////////////////////////////////
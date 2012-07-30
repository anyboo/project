#include "stdafx.h"
#include"resource.h"
#include "DlgUserInfoXX.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"

////////////////////////////////////////////////////////////////////////////////////
//宏定义
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//默认颜色
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgUserXX, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDOK, OnOK)

END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgUserXX::CDlgUserXX(CWnd* pParent): CDialog(IDD_USER_XX_INFO, pParent)//, CSkinWndObject(this)
{
	//设置变量

	m_pIClientSocket = NULL;
	m_enOperateStatus=enOperateUserXXStatus_NULL;
	
}

//虚构函数
CDlgUserXX::~CDlgUserXX()
{
	if (m_brBkground.m_hObject != NULL ) m_brBkground.DeleteObject();
}

//交换函数
void CDlgUserXX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SCHOOL, m_ComboSelect);
	DDX_Control(pDX,IDOK, m_btEdit);
	
}

//初始函数
BOOL CDlgUserXX::OnInitDialog()
{
	__super::OnInitDialog();

	//创建刷子
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//设置标题
//	SetWindowText(TEXT("欢乐豆赠送"));

	m_ComboSelect.AddString("本科");
	m_ComboSelect.AddString("大专");
	m_ComboSelect.AddString("高中");
	
	((CButton *)GetDlgItem(IDC_BH_INFO))->SetCheck(BST_CHECKED);
 	//GetDlgItem(IDC_TRUE_NAME)->SetWindowText("********");
	SetDlgItemText(IDC_TRUE_NAME, "********");
//  	GetDlgItem(IDC_MOBILE)->SetWindowText("********");
//  	GetDlgItem(IDC_IDNO)->SetWindowText("********");
//  	GetDlgItem(IDC_QQ)->SetWindowText("********");
	SetDlgItemText(IDC_MOBILE, "********");
	SetDlgItemText(IDC_IDNO, "********");
	SetDlgItemText(IDC_QQ, "********");

	GetDlgItem(IDC_BH_INFO)->EnableWindow(false);
	GetDlgItem(IDC_TRUE_NAME)->EnableWindow(false);
	GetDlgItem(IDC_MOBILE)->EnableWindow(false);
	GetDlgItem(IDC_IDNO)->EnableWindow(false);
	GetDlgItem(IDC_QQ)->EnableWindow(false);

	GetDlgItem(IDC_1)->EnableWindow(false);
	GetDlgItem(IDC_3)->EnableWindow(false);
	GetDlgItem(IDC_4)->EnableWindow(false);
	GetDlgItem(IDC_12)->EnableWindow(false);

	m_ComboSelect.SetCurSel(0);

	//更新控件
	UpdateControls();

	//初始化设置
	//__super::InitSkinObject();

	return TRUE;  
}

//绘画函数
void CDlgUserXX::OnPaint()
{
	CPaintDC dc(this);

	
	//绘画标题
//	__super::DrawSkinView(&dc);
}


//显示消息
void CDlgUserXX::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("赠送欢乐豆"));

	return;
}

//网络链接
bool CDlgUserXX::ConnectServer()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateUserXXStatus_NULL )
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
bool CDlgUserXX::SendData()
{
	
	//状态判断
	ASSERT(m_enOperateStatus != enOperateUserXXStatus_NULL);
	if ( m_enOperateStatus == enOperateUserXXStatus_NULL ) return false;

	if ( m_pIClientSocket == NULL || m_pIClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT ) return false;

	if ( m_enOperateStatus == enOperateUserXXStatus_Save )
	{
		CMD_GP_ModifyUserInfo UserInfo;
		ZeroMemory(&UserInfo, sizeof(UserInfo));

		tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
		UserInfo.dwUserID = GlobalUserData.dwUserID;
		CString strTemp;
		GetDlgItem(IDC_ADDRESS)->GetWindowTextA(strTemp);
		strcpy(UserInfo.szAddress,strTemp);

		GetDlgItem(IDC_SCHOOL)->GetWindowTextA(strTemp);
		strcpy(UserInfo.szSchool,strTemp);

		GetDlgItem(IDC_EMAIL)->GetWindowTextA(strTemp);
		strcpy(UserInfo.szEmail,strTemp);

		GetDlgItem(IDC_URL)->GetWindowTextA(strTemp);
		strcpy(UserInfo.szUrl,strTemp);

		GetDlgItem(IDC_TELPHONE)->GetWindowTextA(strTemp);
		strcpy(UserInfo.szTel,strTemp);

		GetDlgItem(IDC_MSN)->GetWindowTextA(strTemp);
		strcpy(UserInfo.szMsn,strTemp);
		
	//	strcpy(UserInfo.szNickname,strNickName);
	//	GiveMoney.cbType = m_UserSelect.GetCurSel();

		//投递消息
		m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_EDIT_USER_INFO, &UserInfo, sizeof(UserInfo));        

		//更新界面
		InvalidateRect(NULL);
	}
	


	//设置状态
	m_enOperateStatus = enOperateUserXXStatus_NULL ;

	return true;
}

//更新控件
void CDlgUserXX::UpdateControls()
{
	//变量定义
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	UpdateData(false);
}

//颜色处理
HBRUSH CDlgUserXX::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CDlgUserXX::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//更新控件
	if ( bShow ) UpdateControls();
}

//销毁消息
void CDlgUserXX::OnClose()
{


	CDialog::OnClose();
}


//初始化房间
bool CDlgUserXX::InitRoomName()
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

	return true;
}

void CDlgUserXX::OnOK()
{
	
	m_enOperateStatus = enOperateUserXXStatus_Save;
	//链接网络
	if ( ConnectServer() == false )
	{
		//设置状态
		m_enOperateStatus = enOperateUserXXStatus_NULL;
		return;
	}
}



////////////////////////////////////////////////////////////////////////////////////
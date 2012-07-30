#include "stdafx.h"
#include "resource.h"
#include "DlgMoneyGive.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"

////////////////////////////////////////////////////////////////////////////////////
//宏定义
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//默认颜色
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgMoneyGive, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDC_SAVE, OnBnClickedSave)
	ON_BN_CLICKED(IDC_REFRESH, OnBnClickedRefresh)
	ON_CBN_SELCHANGE(IDC_USER_SELECT, OnEnChangeUser)
	ON_EN_CHANGE(IDC_SCORE, OnEnChangeNum)

END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgMoneyGive::CDlgMoneyGive(CWnd* pParent): CDialog(IDD_MONEY_GIVE, pParent)//, CSkinWndObject(this)
{
	//设置变量
	m_lTotalMoney = 0;
	m_lRoomMoney = 0;
	m_pIClientSocket = NULL;
	m_enOperateStatus=enOperateMoneyGiveStatus_NULL;
	m_bIsOk= false;
	
}

//虚构函数
CDlgMoneyGive::~CDlgMoneyGive()
{
	if(m_brBkground.GetSafeHandle()) m_brBkground.DeleteObject();
}

//交换函数
void CDlgMoneyGive::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_USER_SELECT, m_UserSelect);
	DDX_Control(pDX, IDC_SAVE, m_btSave);
	DDX_Control(pDX, IDC_REFRESH, m_btRefresh);
	//DDX_Control(pDX, IDC_USER_SELECT, m_chkUserID);	
}

//初始函数
BOOL CDlgMoneyGive::OnInitDialog()
{
	__super::OnInitDialog();

	if(m_brBkground.GetSafeHandle()) m_brBkground.DeleteObject();
	//创建刷子
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
void CDlgMoneyGive::OnPaint()
{
	CPaintDC dc(this);

	
	//绘画标题
//	__super::DrawSkinView(&dc);
}


//显示消息
void CDlgMoneyGive::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("赠送欢乐豆"));

	return;
}

//网络链接
bool CDlgMoneyGive::ConnectServer()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateMoneyStatus_NULL )
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
bool CDlgMoneyGive::SendData()
{
	
	//状态判断
	ASSERT(m_enOperateStatus != enOperateMoneyGiveStatus_NULL);
	if ( m_enOperateStatus == enOperateMoneyGiveStatus_NULL ) return false;

	if ( m_pIClientSocket == NULL || m_pIClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT ) return false;

	if ( m_enOperateStatus == enOperateGiveStatus_Save )
	{
		CMD_GP_MoneyGive GiveMoney;
		ZeroMemory(&GiveMoney, sizeof(GiveMoney));

		tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
		GiveMoney.dwUserID = GlobalUserData.dwUserID;
		CString str;
//		GetDlgItem(IDC_SCORE)->GetWindowText(str);
		GiveMoney.lScore = GetDlgItemInt64(IDC_SCORE);
		CString strNickName;
		GetDlgItem(IDC_USER)->GetWindowText(strNickName);
		strcpy(GiveMoney.szNickname,strNickName);
		GiveMoney.cbType = m_UserSelect.GetCurSel();
		if(m_bIsOk == false)
			GiveMoney.cbQueryInfo = true;
		else
			SetDlgItemText(IDC_SCORE, "0");

		//投递消息
		m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_GIVE_MONEY, &GiveMoney, sizeof(GiveMoney));        

		//更新界面
		InvalidateRect(NULL);
	}
	
	else if ( m_enOperateStatus == enOperateGiveStatus_Refresh )
	{
		CMD_GP_MoneyOper GetMoney;
		ZeroMemory(&GetMoney, sizeof(GetMoney));

		tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
		GetMoney.dwUserID = GlobalUserData.dwUserID;
		GetMoney.cbType = 2;
		GetMoney.lScore = 0;
		GetMoney.dwKindID = 0;
		//投递消息
		m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_OPEN_MONEY, &GetMoney, sizeof(GetMoney));        

		//更新界面
		InvalidateRect(NULL);
	}

	//设置状态
	m_enOperateStatus = enOperateMoneyGiveStatus_NULL ;

	return true;
}

//更新控件
void CDlgMoneyGive::UpdateControls()
{
	//变量定义
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	//CString strTemp;
	//strTemp.Format("%I64d",m_lTotalMoney);
	GetDlgItem(IDC_TOTAL_MONEY)->SetWindowText(LongToString(m_lTotalMoney));
	
	m_DlgStatus.DestroyStatusWnd(this);
	UpdateData(false);

}

//颜色处理
HBRUSH CDlgMoneyGive::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CDlgMoneyGive::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//更新控件
	if ( bShow ) UpdateControls();
}

//销毁消息
void CDlgMoneyGive::OnClose()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateMoneyStatus_NULL )
	{
		m_pIClientSocket->CloseSocket();
	}
	DestroyWindow();
	CDialog::OnClose();
}


//初始化房间
bool CDlgMoneyGive::InitRoomName()
{
	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	
	CString strBuffer;
	strBuffer.Format(TEXT("正在获得用户保险箱，请稍候..."));
	m_DlgStatus.ShowStatusMessage(strBuffer,this);
	m_DlgStatus.SetWindowPos(&wndTopMost,0,0,0, 0,SWP_NOMOVE|SWP_NOSIZE   ); 
	m_enOperateStatus = enOperateGiveStatus_Refresh;
	if ( ConnectServer() == false )
	{
		//设置状态
		m_enOperateStatus = enOperateMoneyGiveStatus_NULL;
		
	}
	/*
	m_RoomSelect.ResetContent();

	
	int nIndex=0;
	CListKind * pListKind=NULL;
	do
	{
		pListKind=g_GlobalUnits.m_ServerListManager.EnumKindItem(nIndex++);
		if (pListKind==NULL) break;
		m_RoomSelect.AddString(pListKind->m_GameKind.szKindName);
	} while (true);*/
	m_UserSelect.Clear();
	m_UserSelect.InsertString(0,"用户名");
	m_UserSelect.InsertString(1,"用户ID");

	m_UserSelect.SetCurSel(0);

	return true;
}

DWORD CDlgMoneyGive::GetGameKind(CString m_strKindName)
{
	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	
	DWORD m_dwGameKind=0;
	/*int nIndex=0;
	CListKind * pListKind=NULL;
	do
	{
		pListKind=g_GlobalUnits.m_ServerListManager.EnumKindItem(nIndex++);
		if (pListKind==NULL) break;
		if(pListKind->m_GameKind.szKindName==m_strKindName)
			{
			m_dwGameKind = pListKind->m_GameKind.wKindID;
			break;
			}
		m_RoomSelect.AddString(pListKind->m_GameKind.szKindName);
	} while (true);*/

	return m_dwGameKind;
}

void CDlgMoneyGive::OnBnClickedSave()
{
	if(GetDlgItemInt64(IDC_SCORE) <=0)
	{
		ShowMessage("欢乐豆不能少于0！");
		return;
	}

	m_enOperateStatus = enOperateGiveStatus_Save;

	m_bIsOk = false;
	//链接网络
	if ( ConnectServer() == false )
	{
		//设置状态
		m_enOperateStatus = enOperateMoneyGiveStatus_NULL;
		return;
	}
}

void CDlgMoneyGive::OnBnClickedRefresh()
{
	m_enOperateStatus = enOperateGiveStatus_Refresh;
	//链接网络
	if ( ConnectServer() == false )
	{
		//设置状态
		m_enOperateStatus = enOperateMoneyGiveStatus_NULL;
		return;
	}		
}

void CDlgMoneyGive::SetMoney(LONGLONG TotalMoney,LONGLONG RoomMoney)
{
	m_lTotalMoney = TotalMoney;
	m_lRoomMoney = RoomMoney;
}
//密码房间
void CDlgMoneyGive::OnEnChangeUser()
{
	//m_lTotalMoney = 0;
	//m_lRoomMoney = 0;

	UpdateControls();
}

void CDlgMoneyGive::ShowOk( LPCTSTR szTargetAccount,DWORD dwTargetUserID,LONGLONG lScore)
{
	//TCHAR szMsg[512];
	//_snprintf(szMs)
	CString strMsg;
	strMsg.Format("请确认对方用户信息！\n\n用户名:%s\n用户ID:%d\n转出金币:%I64d", szTargetAccount ,dwTargetUserID, lScore);
	if(ShowInformationEx(strMsg,0,MB_ICONQUESTION| MB_YESNO|MB_DEFBUTTON1,TEXT("赠送欢乐豆")) == IDYES)
	{
		m_enOperateStatus = enOperateGiveStatus_Save;

		m_bIsOk = true;
		//链接网络
		if ( ConnectServer() == false )
		{
			//设置状态
			m_enOperateStatus = enOperateMoneyGiveStatus_NULL;
			return;
		}
	}

}

CString CDlgMoneyGive::LongToString( LONGLONG lScore )
{
	CString strText,strTmpText;
	strText = TEXT("");
	INT64 lGold;
	lGold = lScore;
	int NumCount=0;
	int lenCount=0;
	if(lGold==0) {strText.Format("0");};
	if(lGold>0)
	{
		strTmpText.Format(TEXT("%I64d"),lGold);
		if(strTmpText.GetLength() > 3)
		{
			for(int i=0;i<strTmpText.GetLength();i++)
			{
				if(i%4==0)
				{
					strTmpText.Insert(strTmpText.GetLength()-i,',');
				}
			}
		}
		int nFind = strTmpText.ReverseFind(',');
		if(-1==nFind)
		{
			strText=strTmpText;
		}
		else
		{
			strText = strTmpText.Left(strTmpText.ReverseFind(','));
		}
	}

	return strText;
}

INT64 CDlgMoneyGive::GetDlgItemInt64( UINT nID )
{
	CString strText;
	if (GetDlgItemText(nID,strText)==0)
		return 0;
	strText.Remove(',');

	return _tstoi64(strText);
}

void CDlgMoneyGive::OnEnChangeNum()
{
	CString strText,strTmpText;
	strText = TEXT("");
	INT64 lGold;
	lGold = GetDlgItemInt64(IDC_SCORE);
	int NumCount=0;
	int lenCount=0;
	if(lGold==0) strText=TEXT("0");
	if(lGold>0)
	{
		strTmpText.Format(TEXT("%I64d"),lGold);
		if(strTmpText.GetLength() > 3)
		{
			for(int i=0;i<strTmpText.GetLength();i++)
			{
				if(i%4==0)
				{
					strTmpText.Insert(strTmpText.GetLength()-i,',');
				}
			}
		}
		int nFind = strTmpText.ReverseFind(',');
		if(-1==nFind)
		{
			strText=strTmpText;
		}
		else
		{
			strText = strTmpText.Left(strTmpText.ReverseFind(','));
		}
		GetDlgItem(IDC_SCORE)->SetWindowText(strText);
		((CEdit*)GetDlgItem(IDC_SCORE))->SetSel(strText.GetLength(),-1);
	}
}

////////////////////////////////////////////////////////////////////////////////////
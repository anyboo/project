#include "Stdafx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "SuoJi.h"


SuoJi::SuoJi()
{
	m_pIClientSocket=NULL;
	m_LpSuoJi=0;
	
if(ConnectServer()==true)
{

	
}

};

SuoJi::~SuoJi()
{
m_pIClientSocket->CloseSocket();

};
void SuoJi::SenDdateAnlock()
{

	CMD_GP_LockJiQi lockJiQi;
    
	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();

	lockJiQi.dwGameUserID=GlobalUserData.dwGameID;
	lockJiQi.dwLockLP=false;
	
	if(ConnectServer()==true)
	{
     
		
	CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	CPlazaViewItem * pPlazaViewItem = &(pGameFrame->m_DlgGamePlaza) ;

	int iResult=pPlazaViewItem->ShowMessageBox("点击“确定”解锁您的机器,您的账号将可以在其他机器上登录!",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);

	if(iResult==IDYES)
	{
	
//	m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_CUSTOM_LOCK_JIQI, &lockJiQi, sizeof(lockJiQi));
	m_LpSuoJi=2;
	//TCHAR szPath[MAX_PATH]=TEXT("");
	//GetCurrentDirectory(sizeof(szPath),szPath);
	//_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\LockMoss.ini"),szPath);
	////移动窗口
	//
    //
    //m_LpSuoJi=2;
	//WritePrivateProfileString(TEXT("BankerCondition"),TEXT("Score"),TEXT("0"),m_szConfigFileName);
	}else
	{
	
	}
    
	return;
	}

};
void SuoJi::SenDdatelock()
{

	CMD_GP_LockJiQi lockJiQi;
    
	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();

	lockJiQi.dwGameUserID=GlobalUserData.dwGameID;
	lockJiQi.dwLockLP=true;
	
	if(ConnectServer()==true)
	{
     
		
	CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	CPlazaViewItem * pPlazaViewItem = &(pGameFrame->m_DlgGamePlaza) ;

	int iResult=pPlazaViewItem->ShowMessageBox("点击“确定”锁定您的机器,您的账号将不能在其他机器上登录!",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);

	if(iResult==IDYES)
	{
//	m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_CUSTOM_LOCK_JIQI, &lockJiQi, sizeof(lockJiQi));
     m_LpSuoJi=1;
	//TCHAR szPath[MAX_PATH]=TEXT("");
	//GetCurrentDirectory(sizeof(szPath),szPath);
	//_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\LockMoss.ini"),szPath);
	////移动窗口
	//
 //
 //   m_LpSuoJi=1;
	//WritePrivateProfileString(TEXT("BankerCondition"),TEXT("Score"),TEXT("1"),m_szConfigFileName);
	}else
	{
	
	}
	return;
 }


return;
}

bool SuoJi::ConnectServer()
{
	
   
	//if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT )//?GetSocketStatus
	//{   

	//	return true;
	//}

	//获取大厅
	CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	CPlazaViewItem * pPlazaViewItem = &(pGameFrame->m_DlgGamePlaza) ;
	if ( m_pIClientSocket == NULL )
	{
		
		//合法判断
		if ( pPlazaViewItem->m_ClientSocket.GetInterface() == NULL ) 
		{
			
			
//			ShowMessage(TEXT("网络组件还没有创建，请重新登录！"));
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
	catch (...)
	{
//		ShowMessage((TCHAR*)pszError);
		return false;
	}

	return true;



};
#include "stdafx.h"
#include"resource.h"
#include "DlgMoneyRecord.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"

#define PageNum   9
////////////////////////////////////////////////////////////////////////////////////
//宏定义
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//默认颜色
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgMoneyRecord, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDC_QUERY, OnBnClickedGet)
	ON_BN_CLICKED(IDC_FIRST, OnBnClickedFirst)
	ON_BN_CLICKED(IDC_LAST, OnBnClickedLast)
	ON_BN_CLICKED(IDC_NEXT, OnBnClickedNext)
	ON_BN_CLICKED(IDC_END, OnBnClickedEnd)
END_MESSAGE_MAP()

//控件绑定
void CDlgMoneyRecord::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_RecordListCtrl);

	DDX_Control(pDX, IDC_FIRST, m_btFirstPage);
	DDX_Control(pDX, IDC_LAST, m_btLastPage);
	DDX_Control(pDX, IDC_NEXT, m_btNextPage);
	DDX_Control(pDX, IDC_END, m_btEndPage);
	DDX_Control(pDX, IDC_QUERY, m_btQuery);
	
	
}

////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgMoneyRecord::CDlgMoneyRecord(CWnd* pParent): CDialog(IDD_RECORD, pParent)//, CSkinWndObject(this)
{
	//设置变量

	m_pIClientSocket = NULL;
	m_enOperateStatus=enOperateMoneyRecordStatus_NULL;
	
	
}

//虚构函数
CDlgMoneyRecord::~CDlgMoneyRecord()
{
	//if (m_brBkground.m_hObject != NULL ) m_brBkground.DeleteObject();
	if(m_brBkground.GetSafeHandle()) m_brBkground.DeleteObject();
}



//初始函数
BOOL CDlgMoneyRecord::OnInitDialog()
{
	__super::OnInitDialog();

	//创建刷子
	if(m_brBkground.GetSafeHandle()) m_brBkground.DeleteObject();
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//设置标题
	SetWindowText(TEXT("欢乐豆赠送"));

	m_RecordListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES); 

	
	m_RecordListCtrl.InsertColumn(0,"编号",LVCFMT_LEFT,40);
	m_RecordListCtrl.InsertColumn(1,"对方用户名",LVCFMT_LEFT,80);
	m_RecordListCtrl.InsertColumn(2,"类型",LVCFMT_LEFT,40);
	m_RecordListCtrl.InsertColumn(3,"赠送数量",LVCFMT_LEFT,100);
	m_RecordListCtrl.InsertColumn(4,"赠送时间",LVCFMT_LEFT,150);

	m_nCount=0;

	nPageNum = m_nCount/8;

	if(m_nCount%8==0)
		nPageNum=nPageNum+1;

	CString strPage;
	strPage.Format("共 %d 页",nPageNum);;

	GetDlgItem(IDC_PAGE)->SetWindowText(strPage);


	nCurrentPage = 1;
	m_btFirstPage.EnableWindow(0);
	m_btEndPage.EnableWindow(0);
	m_btLastPage.EnableWindow(0);
	m_btNextPage.EnableWindow(0);
	
	
	
	//更新控件
	UpdateControls();

	//初始化设置
	//__super::InitSkinObject();

	return TRUE;  
}

//绘画函数
void CDlgMoneyRecord::OnPaint()
{
	CPaintDC dc(this);

	
	//绘画标题
//	__super::DrawSkinView(&dc);
}


//显示消息
void CDlgMoneyRecord::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("赠送欢乐豆"));

	return;
}

//网络链接
bool CDlgMoneyRecord::ConnectServer()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateMoneyRecordStatus_NULL )
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
bool CDlgMoneyRecord::SendData()
{
	
	//状态判断
	ASSERT(m_enOperateStatus != enOperateMoneyRecordStatus_NULL);
	if ( m_enOperateStatus == enOperateMoneyRecordStatus_NULL ) return false;

	if ( m_pIClientSocket == NULL || m_pIClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT ) return false;

	if ( m_enOperateStatus == enOperateRecordStatus_Save )
	{
		CMD_GP_BankRecord GiveRecord;
		ZeroMemory(&GiveRecord, sizeof(GiveRecord));

		tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
		GiveRecord.dwUserID = GlobalUserData.dwUserID;
		//投递消息
		m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_GIVE_RECORD, &GiveRecord, sizeof(GiveRecord));        

		//更新界面
		InvalidateRect(NULL);
	}
	
	//设置状态
	m_enOperateStatus = enOperateMoneyRecordStatus_NULL ;

	return true;
}

//更新控件
void CDlgMoneyRecord::UpdateControls()
{
	//变量定义
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	
	UpdateData(false);

}

//颜色处理
HBRUSH CDlgMoneyRecord::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CDlgMoneyRecord::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//更新控件
	if ( bShow ) UpdateControls();
}

//销毁消息
void CDlgMoneyRecord::OnClose()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateMoneyStatus_NULL )
	{
		m_pIClientSocket->CloseSocket();
	}
	CDialog::OnClose();
		DestroyWindow();
}


//初始化房间
bool CDlgMoneyRecord::InitRoomName()
{
	

	return true;
}


////////////////////////////////////////////////////////////////////////////////////
void CDlgMoneyRecord::OnBnClickedGet()
{
	

	m_enOperateStatus = enOperateRecordStatus_Save;
	//链接网络
	if ( ConnectServer() == false )
	{
		//设置状态
		m_enOperateStatus = enOperateMoneyRecordStatus_NULL;
		return;
	}
}

void CDlgMoneyRecord::InitTable()
{
	m_RecordListCtrl.DeleteAllItems();

	int nShowPage=0;
	if(m_nCount>=PageNum)
	{
		nShowPage = PageNum;

		nCurrentPage = 1;
		m_btFirstPage.EnableWindow(0);
		m_btEndPage.EnableWindow(1);
		m_btLastPage.EnableWindow(0);
		m_btNextPage.EnableWindow(1);

		for(int i=0;i<PageNum;i++)
		{
			CString str;
			str.Format("%d",GiveRecord[i].dwRecordID);
			m_RecordListCtrl.InsertItem(i,str);
			if(strstr( GiveRecord[i].szGiveName,g_GlobalUnits.GetGolbalUserData().szAccounts) != NULL)
			{
				m_RecordListCtrl.SetItemText(i,1,GiveRecord[i].szTargetUserName);
				m_RecordListCtrl.SetItemText(i,2,"转出");
			}
			else
			{
				m_RecordListCtrl.SetItemText(i,1,GiveRecord[i].szGiveName);
				m_RecordListCtrl.SetItemText(i,2,"转入");
			}

			str.Format("%I64d",GiveRecord[i].lGiveScore);
			m_RecordListCtrl.SetItemText(i,3,str);
			m_RecordListCtrl.SetItemText(i,4,GiveRecord[i].szTime);
		}

	
	}
	else
	{
		for(int i=0;i<m_nCount;i++)
		{
			CString str;
			str.Format("%d",GiveRecord[i].dwRecordID);
			m_RecordListCtrl.InsertItem(i,str);
			if(strstr( GiveRecord[i].szGiveName,g_GlobalUnits.GetGolbalUserData().szAccounts) != NULL)
			{
				m_RecordListCtrl.SetItemText(i,1,GiveRecord[i].szTargetUserName);
				m_RecordListCtrl.SetItemText(i,2,"转出");
			}
			else
			{
				m_RecordListCtrl.SetItemText(i,1,GiveRecord[i].szGiveName);
				m_RecordListCtrl.SetItemText(i,2,"转入");
			}
			str.Format("%I64d",GiveRecord[i].lGiveScore);
			m_RecordListCtrl.SetItemText(i,3,str);
			m_RecordListCtrl.SetItemText(i,4,GiveRecord[i].szTime);
		}
	}

	nPageNum = m_nCount/PageNum;
	

	if(m_nCount%PageNum!=0)
		nPageNum=nPageNum+1;

	CString strPage;
	strPage.Format("共 %d 页",nPageNum);

	GetDlgItem(IDC_PAGE)->SetWindowText(strPage);
}

void CDlgMoneyRecord::OnBnClickedFirst()
	{
		m_RecordListCtrl.DeleteAllItems();
		nCurrentPage = 1;
		m_btFirstPage.EnableWindow(0);
		m_btEndPage.EnableWindow(1);
		m_btLastPage.EnableWindow(0);
		m_btNextPage.EnableWindow(1);
		
		for(int i=0;i<PageNum;i++)
		{
			CString str;
			str.Format("%d",GiveRecord[i].dwRecordID);
			m_RecordListCtrl.InsertItem(i,str);
			if(strstr( GiveRecord[i].szGiveName,g_GlobalUnits.GetGolbalUserData().szAccounts) != NULL)
			{
				m_RecordListCtrl.SetItemText(i,1,GiveRecord[i].szTargetUserName);
				m_RecordListCtrl.SetItemText(i,2,"转出");
			}
			else
			{
				m_RecordListCtrl.SetItemText(i,1,GiveRecord[i].szGiveName);
				m_RecordListCtrl.SetItemText(i,2,"转入");
			}
			str.Format("%i64",GiveRecord[i].lGiveScore);
			m_RecordListCtrl.SetItemText(i,3,str);
			m_RecordListCtrl.SetItemText(i,4,GiveRecord[i].szTime);
		}

		CString strPage;
		strPage.Format("第 %d 页",nCurrentPage);

		GetDlgItem(IDC_PAGE)->SetWindowText(strPage);

	
	}
void CDlgMoneyRecord::OnBnClickedLast()
	{
		m_RecordListCtrl.DeleteAllItems();
		nCurrentPage --;

		if(nCurrentPage==1)
		{
			m_btFirstPage.EnableWindow(0);
			m_btEndPage.EnableWindow(1);
			m_btLastPage.EnableWindow(0);
			m_btNextPage.EnableWindow(1);
		}
		else
		{
			m_btFirstPage.EnableWindow(1);
			m_btEndPage.EnableWindow(1);
			m_btLastPage.EnableWindow(1);
			m_btNextPage.EnableWindow(1);
		}
		
		/*if(nCurrentPage==nPageNum)
			{
				for(int i=0;i<m_nCount-PageNum*(nPageNum-1);i++)
				{
					CString str;
					str.Format("%d",i);
					m_RecordListCtrl.InsertItem(i,str);	
					m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szGiveName);

					str.Format("%d",GiveRecord[i+PageNum*(nPageNum-1)].cbType);
					m_RecordListCtrl.SetItemText(i,2,str);
					str.Format("%ld",GiveRecord[i+PageNum*(nPageNum-1)].lGiveScore);
					m_RecordListCtrl.SetItemText(i,3,str);
					m_RecordListCtrl.SetItemText(i,4,GiveRecord[i+PageNum*(nPageNum-1)].szTime);
				}
			}
		else
			{*/
				
				for(int i=0;i<PageNum;i++)
				{
					CString str;
					str.Format("%d",GiveRecord[i+PageNum*(nCurrentPage-1)].dwRecordID);
					m_RecordListCtrl.InsertItem(i,str);
					if(strstr( GiveRecord[i+PageNum*(nCurrentPage-1)].szGiveName,g_GlobalUnits.GetGolbalUserData().szAccounts) != NULL)
					{
						m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nCurrentPage-1)].szTargetUserName);
						m_RecordListCtrl.SetItemText(i,2,"转出");
					}
					else
					{
						m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nCurrentPage-1)].szGiveName);
						m_RecordListCtrl.SetItemText(i,2,"转入");
					}
// 					CString str;
// 					str.Format("%d",i+PageNum*(nCurrentPage-1));
// 					m_RecordListCtrl.InsertItem(i,str);	
// 					m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nCurrentPage-1)].szGiveName);
// 					str.Format("%d",GiveRecord[i+PageNum*(nCurrentPage-1)].cbType);
// 					m_RecordListCtrl.SetItemText(i,2,str);
					str.Format("%I64d",GiveRecord[i+PageNum*(nCurrentPage-1)].lGiveScore);
					m_RecordListCtrl.SetItemText(i,3,str);
					m_RecordListCtrl.SetItemText(i,4,GiveRecord[i+PageNum*(nCurrentPage-1)].szTime);
				}
	
			
			
		//	}
		CString strPage;
		strPage.Format("第 %d 页",nCurrentPage);

		GetDlgItem(IDC_PAGE)->SetWindowText(strPage);

	
	
	}
void CDlgMoneyRecord::OnBnClickedNext()
	{
		m_RecordListCtrl.DeleteAllItems();
		nCurrentPage ++;

		if(nCurrentPage==nPageNum)
		{
			m_btFirstPage.EnableWindow(1);
			m_btEndPage.EnableWindow(0);
			m_btLastPage.EnableWindow(1);
			m_btNextPage.EnableWindow(0);
		}
		else
		{
			m_btFirstPage.EnableWindow(1);
			m_btEndPage.EnableWindow(1);
			m_btLastPage.EnableWindow(1);
			m_btNextPage.EnableWindow(1);
		}
		
		if(nCurrentPage==nPageNum)
			{
				for(int i=0;i<m_nCount-PageNum*(nPageNum-1);i++)
				{
// 					CString str;
// 					str.Format("%d",i+PageNum*(nPageNum-1));
// 					m_RecordListCtrl.InsertItem(i,str);	
// 					m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szGiveName);
// 
// 					str.Format("%d",GiveRecord[i+PageNum*(nPageNum-1)].cbType);
// 					m_RecordListCtrl.SetItemText(i,2,str);
					CString str;
					str.Format("%d",GiveRecord[i+PageNum*(nPageNum-1)].dwRecordID);
					m_RecordListCtrl.InsertItem(i,str);
					if(strstr( GiveRecord[i+PageNum*(nPageNum-1)].szGiveName,g_GlobalUnits.GetGolbalUserData().szAccounts) != NULL)
					{
						m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szTargetUserName);
						m_RecordListCtrl.SetItemText(i,2,"转出");
					}
					else
					{
						m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szGiveName);
						m_RecordListCtrl.SetItemText(i,2,"转入");
					}

					str.Format("%I64d",GiveRecord[i+PageNum*(nPageNum-1)].lGiveScore);
					m_RecordListCtrl.SetItemText(i,3,str);
					m_RecordListCtrl.SetItemText(i,4,GiveRecord[i+PageNum*(nPageNum-1)].szTime);
				}
			}
		else
			{
				
				for(int i=0;i<PageNum;i++)
				{
					CString str;
					str.Format("%d",GiveRecord[i+PageNum*(nPageNum-1)].dwRecordID);
					m_RecordListCtrl.InsertItem(i,str);
					if(strstr( GiveRecord[i+PageNum*(nPageNum-1)].szGiveName,g_GlobalUnits.GetGolbalUserData().szAccounts) != NULL)
					{
						m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szTargetUserName);
						m_RecordListCtrl.SetItemText(i,2,"转出");
					}
					else
					{
						m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szGiveName);
						m_RecordListCtrl.SetItemText(i,2,"转入");
					}

					str.Format("%I64d",GiveRecord[i+PageNum*(nCurrentPage-1)].lGiveScore);
					m_RecordListCtrl.SetItemText(i,3,str);
					m_RecordListCtrl.SetItemText(i,4,GiveRecord[i+PageNum*(nCurrentPage-1)].szTime);
				}
	
			
			
			}
		CString strPage;
		strPage.Format("第 %d 页",nCurrentPage);

		GetDlgItem(IDC_PAGE)->SetWindowText(strPage);

	
	
	}
void CDlgMoneyRecord::OnBnClickedEnd()
{
		m_RecordListCtrl.DeleteAllItems();
		nCurrentPage = nPageNum;
		m_btFirstPage.EnableWindow(1);
		m_btEndPage.EnableWindow(1);
		m_btLastPage.EnableWindow(1);
		m_btNextPage.EnableWindow(0);
		
		for(int i=0;i<m_nCount-PageNum*(nPageNum-1);i++)
		{
// 			CString str;
// 			str.Format("%d",i+PageNum*(nPageNum-1));
// 			m_RecordListCtrl.InsertItem(i,str);	
// 			m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szGiveName);
// 
// 			str.Format("%d",GiveRecord[i+PageNum*(nPageNum-1)].cbType);
// 			m_RecordListCtrl.SetItemText(i,2,str);
			CString str;
			str.Format("%d",GiveRecord[i+PageNum*(nPageNum-1)].dwRecordID);
			m_RecordListCtrl.InsertItem(i,str);
			if(strstr( GiveRecord[i+PageNum*(nPageNum-1)].szGiveName,g_GlobalUnits.GetGolbalUserData().szAccounts) != NULL)
			{
				m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szTargetUserName);
				m_RecordListCtrl.SetItemText(i,2,"转出");
			}
			else
			{
				m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szGiveName);
				m_RecordListCtrl.SetItemText(i,2,"转入");
			}

			str.Format("%I64d",GiveRecord[i+PageNum*(nPageNum-1)].lGiveScore);
			m_RecordListCtrl.SetItemText(i,3,str);
			m_RecordListCtrl.SetItemText(i,4,GiveRecord[i+PageNum*(nPageNum-1)].szTime);
		}

		CString strPage;
		strPage.Format("第 %d 页",nCurrentPage);

		GetDlgItem(IDC_PAGE)->SetWindowText(strPage);

}

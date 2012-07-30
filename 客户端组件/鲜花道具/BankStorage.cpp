#include "Stdafx.h"
#include "resource.h"
#include "BankStorage.h"
#include ".\bankstorage.h"

//////////////////////////////////////////////////////////////////////////

//定时器I D
#define IDI_CHARMVALUE_UPDATE_VIEW		1								//更新界面
#define TIME_CHARMVALUE_UPDATE_VIEW		200								//更新界面


//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CPageDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

CBrush CPageDlg::m_brBack;

CPageDlg::CPageDlg(UINT uIDTemplate , LPCTSTR pszTitleText, LPCTSTR pszButtonText) 
	: CDialog(uIDTemplate),
	m_nIDTemplate(uIDTemplate)
{
	m_strTitle=pszTitleText;
	m_strButtonText=pszButtonText;

	if (m_brBack.GetSafeHandle()==NULL)
	{
		m_brBack.CreateSolidBrush(RGB(229,243,255));
	}
}

CPageDlg::~CPageDlg()
{
}

BOOL CPageDlg::CreatePage( CWnd *pParent )
{
	return Create(m_nIDTemplate, pParent);
}

INT64 CPageDlg::GetDlgItemInt64(UINT nID)
{
	CString strText;
	if (GetDlgItemText(nID,strText)==0)
		return 0;
	strText.Remove(',');

	return _tstoi64(strText);
}


void CPageDlg::OnPaint()
{
	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(&rcClient);

	dc.Draw3dRect(&rcClient, RGB(85,132,174),RGB(85,132,174));
}

BOOL CPageDlg::OnEraseBkgnd(CDC * pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CPageDlg::OnCtlColor( CDC * pDC, CWnd * pWnd, UINT nCtlColor )
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(CSkinDialog::m_SkinAttribute.m_crBackGround);
			pDC->SetTextColor(CSkinDialog::m_SkinAttribute.m_crControlTXColor);
			return m_brBack;
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

BOOL CPageDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	CWnd *pParentWnd=GetParent();
	ASSERT(pParentWnd);

	if (pParentWnd->OnCmdMsg(nID,nCode,pExtra,pHandlerInfo))
		return TRUE;

	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CBankStorage, CSkinDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK,OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL,OnBnClickedCancel)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_PAGE_BUTTON1, IDC_PAGE_BUTTON5, OnClickedPageButton)
	ON_BN_CLICKED(IDC_IN_ALL, OnBnClickedInAll)
	ON_BN_CLICKED(IDC_OUT_ALL, OnBnClickedOutAll)
	ON_EN_CHANGE(IDC_IN_COUNT, OnEnChangeInCount)
	ON_EN_CHANGE(IDC_OUT_COUNT, OnEnChangeOutCount)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CBankStorage::CBankStorage(const BYTE bGameAction) : CSkinDialogEx(IDD_BANK_STORAGE, NULL),
	m_bGameAction(bGameAction)
	
{
	m_lGameGold=0;
	m_lStorageGold=0;
	m_pMeUserData=NULL;
	m_pIClientKernel=NULL;
	m_pClientSocket=NULL;
	m_lGameGoldTmp=0;
	m_lStorageGoldTmp=0;

	m_strInCount=TEXT("");

	m_nSelPage=0;
	m_dwOption=0;

	//分配内存
	m_pPageDlg[0]=new CPageDlg(IDD_PAGE_DIALOG1,TEXT("存入欢乐豆"),TEXT("确认存入"));
	m_pPageDlg[1]=new CPageDlg(IDD_PAGE_DIALOG2,TEXT("取出欢乐豆"),TEXT("确认取出"));
	m_pPageDlg[2]=new CPageDlg(IDD_PAGE_DIALOG3,TEXT("欢乐豆转帐"),TEXT("确认转出"));
	m_pPageDlg[3]=new CPageDlg(IDD_PAGE_DIALOG4,TEXT("密码修改"),TEXT("确认修改"));
	m_pPageDlg[4]=new CPageDlg(IDD_PAGE_DIALOG5,TEXT("转账记录"),TEXT("下一页"));

}

//析构函数
CBankStorage::~CBankStorage()
{
	for (int i=0; i < 5; i++)
	{
		delete m_pPageDlg[i];
	}
}

//控件绑定
void CBankStorage::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_PAGE_BUTTON1, m_btPage[0]);
	DDX_Control(pDX, IDC_PAGE_BUTTON2, m_btPage[1]);
	DDX_Control(pDX, IDC_PAGE_BUTTON3, m_btPage[2]);
	DDX_Control(pDX, IDC_PAGE_BUTTON4, m_btPage[3]);
	DDX_Control(pDX, IDC_PAGE_BUTTON5, m_btPage[4]);
}


//初始化函数
BOOL CBankStorage::OnInitDialog()
{
	CSkinDialogEx::OnInitDialog();

	UpdateData(FALSE);

	SetWindowText(TEXT("银行"));

	HINSTANCE hInstance=GetModuleHandle(PROPERTY_MODULE_DLL_NAME);

	for (int i = 0; i < 5; i++)
	{
		m_pPageDlg[i]->CreatePage(this);
		m_btPage[i].SetButtonImage(IDB_TAB_BUTTON,hInstance,false);
		m_btPage[i].SetTextColor(RGB(0,0,0));
	}

	CRect rcClient;
	GetClientRect(&rcClient);
	m_rcPage.SetRect(rcClient.left+15+90, rcClient.top+35, rcClient.right-15, rcClient.bottom-50);

	m_pPageDlg[m_nSelPage]->MoveWindow(&m_rcPage);
	m_pPageDlg[m_nSelPage]->ShowWindow(SW_SHOW);
	m_btPage[m_nSelPage].SetButtonImage(IDB_TAB_BUTTON_EX,hInstance,false);
	m_btPage[m_nSelPage].SetTextColor(RGB(0,73,146));

 	TCHAR szUrl[256];
 	_snprintf(szUrl, 256, TEXT("http://www.7x78.com/MiniInsureRecord.asp?Id=%d&P=%s"), m_pMeUserData->dwUserID, m_szPassWord);

	//广告控件
	m_BrowerAD.Create(NULL,NULL,WS_VISIBLE,CRect(0,0,0,0),this,100,NULL);
	m_BrowerAD.Navigate(szUrl,NULL,NULL,NULL,NULL);

	//广告位置
	m_BrowerAD.MoveWindow(rcClient.left+15+90+5, rcClient.top+37,100, 100);

	//定时更新
	SetTimer(IDI_CHARMVALUE_UPDATE_VIEW,TIME_CHARMVALUE_UPDATE_VIEW,NULL);

	return TRUE;
}

//创建窗口
BOOL CBankStorage::CreateBankWindow(CWnd *pParent, UINT nSelPage, DWORD dwOption)
{
	ASSERT(pParent!=NULL);
	ASSERT(nSelPage<5);
	m_nSelPage=nSelPage;
	m_dwOption=dwOption;
	return Create(IDD_BANK_STORAGE, pParent);
}

//更新界面
void CBankStorage::UpdateBank()
{
	UpdateData(TRUE);

	ASSERT(m_pMeUserData!=NULL);
	if(m_pMeUserData==NULL)return;

	//设置信息
	m_lGameGold=((m_pMeUserData->lGameGold<0)?0:m_pMeUserData->lGameGold);
	m_lStorageGold=m_pMeUserData->lInsureScore;

	CString static strGameGold=TEXT(""), strStorageGold=TEXT(""),strTmpNumber1,strTmpNumber2;
	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strGameGold.Empty();
	strStorageGold.Empty();
	if(m_lGameGold==0) strGameGold=TEXT("0");
	int nNumberCount=0;
	int count=0;
	//if(m_lGameGold/1000==0) return;
	while(m_lGameGold>0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),m_lGameGold%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if(nNumberCount==3)
		{
			count++;
			if(count==1)
			{
				strTmpNumber2 +=strGameGold;
			}
			else
			{
				strTmpNumber2 += (TEXT(",") +strGameGold);
			}
			strGameGold=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		m_lGameGold/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		if(strGameGold.IsEmpty()==FALSE)
			strTmpNumber2 += (TEXT(",") +strGameGold);
		strGameGold=strTmpNumber2;
	}

	strTmpNumber1.Empty();
	strTmpNumber2.Empty();

	if(m_lStorageGold==0) strStorageGold=TEXT("0");
	nNumberCount=0;
	count=0;
	//if(m_lStorageGold/1000==0) return;
	while(m_lStorageGold>0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),m_lStorageGold%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if(nNumberCount==3)
		{
			count++;
			if(count==1)
			{
				strTmpNumber2 +=strStorageGold;
			}
			else
			{
				strTmpNumber2 += (TEXT(",") +strStorageGold);
			}
			strStorageGold=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		m_lStorageGold/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		if(strStorageGold.IsEmpty()==FALSE)
			strTmpNumber2 += (TEXT(",") +strStorageGold);
		strStorageGold=strTmpNumber2;
	}

	if (m_nSelPage<3)
	{
		//CString strText;
		//strText.Format(TEXT("%I64d"), m_lStorageGold);
		m_pPageDlg[m_nSelPage]->SetDlgItemText(IDC_BANK_GOLD, strStorageGold);

		//strText.Format(TEXT("%I64d"), m_lGameGold);
		m_pPageDlg[m_nSelPage]->SetDlgItemText(IDC_GAME_GOLD, strGameGold);
	}

	m_lGameGold=((m_pMeUserData->lGameGold<0)?0:m_pMeUserData->lGameGold);
	m_lStorageGold=m_pMeUserData->lInsureScore;

	UpdateData(FALSE);
}

//设置按钮
void CBankStorage::SetWaiting(BOOL bWait)
{
	if (bWait)
	{
		//GetDlgItem(IDOK)->EnableWindow(FALSE);
		AfxGetApp()->BeginWaitCursor();
		
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		AfxGetApp()->EndWaitCursor();

		CPageDlg *pSelPage=m_pPageDlg[m_nSelPage];

		switch(m_nSelPage)
		{
		case 0:
			{
				pSelPage->SetDlgItemText(IDC_IN_COUNT,TEXT(""));
				pSelPage->GetDlgItem(IDC_IN_COUNT)->EnableWindow(TRUE);
				((CButton*)pSelPage->GetDlgItem(IDC_IN_ALL))->SetCheck(0);
				break;
			}
		case 1:
			{
				//pSelPage->SetDlgItemText(IDC_OUT_COUNT,TEXT(""));
				pSelPage->SetDlgItemText(IDC_BANK_PASSWORD,TEXT(""));
				pSelPage->GetDlgItem(IDC_OUT_COUNT)->EnableWindow(TRUE);
				((CButton*)pSelPage->GetDlgItem(IDC_OUT_ALL))->SetCheck(0);
				break;
			}
		case 2:
			{
				//pSelPage->SetDlgItemText(IDC_USERGAME_ID,TEXT(""));
				//pSelPage->SetDlgItemText(IDC_OUT_COUNT,TEXT(""));
				pSelPage->SetDlgItemText(IDC_BANK_PASSWORD,TEXT(""));
				pSelPage->GetDlgItem(IDC_OUT_COUNT)->EnableWindow(TRUE);
				((CButton*)pSelPage->GetDlgItem(IDC_OUT_ALL))->SetCheck(0);
				break;
			}
		case 3:
			{
				pSelPage->SetDlgItemText(IDC_BANK_PASSWORD,TEXT(""));
				pSelPage->SetDlgItemText(IDC_NEW_PASSWORD,TEXT(""));
				pSelPage->SetDlgItemText(IDC_CONFIRM_PASSWORD,TEXT(""));
				break;
			}
		case 4:
			{
				break;
			}
		}
	}
}

//鼠标信息
void CBankStorage::OnLButtonDown(UINT nFlags, CPoint point)
{
	CSkinDialogEx::OnLButtonDown(nFlags, point);
}

//鼠标信息
void CBankStorage::OnMouseMove(UINT nFlags, CPoint point)
{
	CSkinDialogEx::OnMouseMove(nFlags, point);
}

//命令信息
BOOL CBankStorage::PreTranslateMessage(MSG* pMsg)
{
	//过滤字母
	//if(GetFocus()==GetDlgItem(IDC_IN_COUNT)&& pMsg->message==WM_CHAR)
	//{
	//	BYTE bMesValue = (BYTE)(pMsg->wParam);
	//	BYTE bTemp = bMesValue-'0';
	//	if((bTemp<0 || bTemp>9) && bMesValue!=VK_BACK) return TRUE;
	//}

	////过滤回车
	//if(pMsg->message==WM_KEYDOWN)
	//{
	//	BYTE bMesValue = (BYTE)(pMsg->wParam);
	//	if(bMesValue==VK_RETURN) return TRUE;
	//}


	//if((WM_CHAR == pMsg->message) && (pMsg->hwnd == GetDlgItem(IDC_IN_COUNT)->GetSafeHwnd()))
	//{
	//	if(pMsg->wParam >=_T('0') && pMsg->wParam <= _T('9'))
	//	{
	//		CString strText(_T(""));
	//		GetDlgItemText(IDC_IN_COUNT, strText);
	//		CString strLeft(_T(""));
	//		CString strRight(_T(""));

	//		if(strText.GetLength() >= 3)
	//		{
	//			int nFlag = strText.ReverseFind(_T(','));
	//			if(-1 != nFlag)
	//			{
	//				strLeft = strText.Left(nFlag + 1);
	//				strRight = strText.Mid(nFlag + 1);
	//				if(0 == strRight.GetLength()%3)
	//				{
	//					strRight += _T(",");
	//				}
	//				strText = strLeft + strRight;
	//			}
	//			else
	//			{
	//				strText += _T(",");
	//			}
	//			SetDlgItemText(IDC_IN_COUNT, strText);
	//			((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(strText.GetLength(), -1);
	//		}
	//	}
	//}

	return __super::PreTranslateMessage(pMsg);
}

//时间消息
void CBankStorage::OnTimer(UINT nIDEvent)
{
	//时间消息
	switch(nIDEvent)
	{
	case IDI_CHARMVALUE_UPDATE_VIEW:		//更新界面
		{
			//更新界面
			UpdateBank();
			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

void CBankStorage::OnClickedPageButton(UINT nCtrlID)
{
	int nPage=nCtrlID-IDC_PAGE_BUTTON1;

	ASSERT(nPage>=0 && nPage<5);

	if (nPage!=m_nSelPage)
	{
		HINSTANCE hInstance=GetModuleHandle(PROPERTY_MODULE_DLL_NAME);
		m_pPageDlg[nPage]->MoveWindow(&m_rcPage);
		m_pPageDlg[nPage]->ShowWindow(SW_SHOW);
		m_btPage[nPage].SetButtonImage(IDB_TAB_BUTTON_EX,hInstance,false);
		m_btPage[nPage].SetTextColor(RGB(0,73,146));

		m_pPageDlg[m_nSelPage]->ShowWindow(SW_HIDE);
		m_btPage[m_nSelPage].SetButtonImage(IDB_TAB_BUTTON,hInstance,false);
		m_btPage[m_nSelPage].SetTextColor(RGB(0,0,0));
		m_nSelPage=nPage;
	}
	CRect rcClient;
	GetClientRect(&rcClient);
	if(m_nSelPage==4)
	{
		//m_BrowerAD.ShowWindow(SW_SHOW);
		::SetWindowPos(m_BrowerAD.m_hWnd, HWND_TOP, rcClient.left+15+90+5, rcClient.top+38, 320, 250, SWP_SHOWWINDOW);
		m_BrowerAD.Refresh();
	}
	else
		::SetWindowPos(m_BrowerAD.m_hWnd, HWND_TOP, rcClient.left+15+90+5, rcClient.top+38, 320, 250, SWP_HIDEWINDOW);
	
}

//设置信息
void CBankStorage::SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserData, LPCTSTR PassWord)
{
	ASSERT((pClientKernel==NULL && pClientSocket!=NULL)||(pClientKernel!=NULL && pClientSocket==NULL));

	//设置信息
	m_pIClientKernel = pClientKernel;
	m_pClientSocket= pClientSocket;
	m_pMeUserData= pMeUserData;
	lstrcpyn( m_szPassWord, PassWord,256);
	return;
}

//发送信息
void CBankStorage::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	ASSERT(m_pIClientKernel!=NULL || m_pClientSocket!=NULL);

	//发送信息
	if(m_pClientSocket!=NULL)m_pClientSocket->SendData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);
	else m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);

	return ;
}

//void CBankStorage::OnEnChangeInCount()
//{
//	if(m_nSelPage==0)
//	{
//		CString strPageIn;
//		int strPutNumber=0;
//		int insertCount=0;
//		strPutNumber=m_pPageDlg[m_nSelPage]->GetDlgItemText(IDC_IN_COUNT,strPageIn);
//		if(strPutNumber>3)
//		{
//			if(strPutNumber%3==0) insertCount=strPutNumber/3-1;
//			else insertCount=strPutNumber/3;
//			for(int i=0;i<insertCount;i++) strPageIn.Insert(i*3,',');
//		}
//		m_pPageDlg[m_nSelPage]->SetDlgItemText(IDC_IN_COUNT,strPageIn);
//	}
//}

//全部存入
void CBankStorage::OnBnClickedInAll()
{
	if (m_nSelPage==0)
	{
		CButton *pCheckBox=(CButton*)m_pPageDlg[m_nSelPage]->GetDlgItem(IDC_IN_ALL);
		ASSERT(pCheckBox);

		if (pCheckBox->GetCheck()==BST_CHECKED)
		{
			//SetDlgItemText设置窗口或对话框拥有的标题或是控件文本。
			CString strText;
			strText.Format(TEXT("%I64d"), m_lGameGold);
			m_pPageDlg[m_nSelPage]->SetDlgItemText(IDC_IN_COUNT, strText);
			m_pPageDlg[m_nSelPage]->GetDlgItem(IDC_IN_COUNT)->EnableWindow(FALSE);
		}
		else
		{
			m_pPageDlg[m_nSelPage]->SetDlgItemText(IDC_IN_COUNT, TEXT(""));
			m_pPageDlg[m_nSelPage]->GetDlgItem(IDC_IN_COUNT)->EnableWindow(TRUE);
			m_pPageDlg[m_nSelPage]->GetDlgItem(IDC_IN_COUNT)->SetFocus();
		}
	}
}

//全部取出/转出
void CBankStorage::OnBnClickedOutAll()
{
	if (m_nSelPage==1)
	{
		CButton *pCheckBox=(CButton*)m_pPageDlg[m_nSelPage]->GetDlgItem(IDC_OUT_ALL);
		ASSERT(pCheckBox);

		if (pCheckBox->GetCheck()==BST_CHECKED)
		{
			CString strText;
			strText.Format(TEXT("%I64d"), m_lStorageGold);
			m_pPageDlg[m_nSelPage]->SetDlgItemText(IDC_OUT_COUNT, strText);
			m_pPageDlg[m_nSelPage]->GetDlgItem(IDC_OUT_COUNT)->EnableWindow(FALSE);
		}
		else
		{
			m_pPageDlg[m_nSelPage]->SetDlgItemText(IDC_OUT_COUNT, TEXT(""));
			m_pPageDlg[m_nSelPage]->GetDlgItem(IDC_OUT_COUNT)->EnableWindow(TRUE);
			m_pPageDlg[m_nSelPage]->GetDlgItem(IDC_OUT_COUNT)->SetFocus();
		}
	}
	else if (m_nSelPage==2)
	{
		CButton *pCheckBox=(CButton*)m_pPageDlg[m_nSelPage]->GetDlgItem(IDC_OUT_ALL);
		ASSERT(pCheckBox);

		if (pCheckBox->GetCheck()==BST_CHECKED)
		{
			CString strText;
			strText.Format(TEXT("%I64d"), m_lStorageGold);
			m_pPageDlg[m_nSelPage]->SetDlgItemText(IDC_OUT_COUNT, strText);
			m_pPageDlg[m_nSelPage]->GetDlgItem(IDC_OUT_COUNT)->EnableWindow(FALSE);
		}
		else
		{
			m_pPageDlg[m_nSelPage]->SetDlgItemText(IDC_OUT_COUNT, TEXT(""));
			m_pPageDlg[m_nSelPage]->GetDlgItem(IDC_OUT_COUNT)->EnableWindow(TRUE);
			m_pPageDlg[m_nSelPage]->GetDlgItem(IDC_OUT_COUNT)->SetFocus();
		}
	}
}

//提取按钮
void CBankStorage::OnBnClickedOk()
{
	CPageDlg *pSelPage=m_pPageDlg[m_nSelPage];
	ASSERT(pSelPage!=NULL);

	INT64 lGoldCount=0;

	switch( m_nSelPage )
	{
	case 0: //存入欢乐豆
		{
			lGoldCount=pSelPage->GetDlgItemInt64(IDC_IN_COUNT);

			if (lGoldCount <= 0 || lGoldCount > m_lGameGold)
			{
				CString strMessage;
				if(m_lGameGold>0)
					strMessage.Format(TEXT("你输入的欢乐豆值必须在1和%I64d之间"), m_lGameGold);
				else 
					strMessage.Format(TEXT("你的当前欢乐豆数目为0,不能进行存储操作!"));

				ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("系统消息"));
				//GetDlgItem函数获得对话框或其它窗口中指定控件或子窗口的指针
				//SetFocus函数要求得到输入焦点。输入焦点将随后的所有键盘输入定向到这个窗口。
				//原来拥有输入焦点的任何窗口都将失去它。SetFocus成员函数项失去输入焦点的窗口发送一条WM_KILLFOCUS消息，
				//并向接收输入焦点的窗口发送一条WM_SETFOCUS消息。它还激活该窗口或它的父窗口。
				//如果当前窗口是激活的，但是不具有输入焦点（这意味着，没有窗口具有输入焦点），则任何按下的键都将产生WM_SYSCHAR，WM_SYSKEYDOWN或WM_SYSKEYUP消息。
				pSelPage->GetDlgItem(IDC_IN_COUNT)->SetFocus();
				((CEdit*)pSelPage->GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
				return;
			}
			break;
		}
	case 1: //取出欢乐豆
		{
			lGoldCount=pSelPage->GetDlgItemInt64(IDC_OUT_COUNT);
			if (lGoldCount <= 0 || lGoldCount > m_lStorageGold)
			{
				CString strMessage;
				if(m_lStorageGold>0)
					strMessage.Format(TEXT("你输入的欢乐豆值必须在1和%I64d之间"),m_lStorageGold);
				else 
					strMessage.Format(TEXT("你的存储欢乐豆数目为0,不能进行提取操作!"));

				ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("系统消息"));
				pSelPage->GetDlgItem(IDC_OUT_COUNT)->SetFocus();
				((CEdit*)pSelPage->GetDlgItem(IDC_OUT_COUNT))->SetSel(0,-1);
				return;
			}
			break;
		}
	case 2: //转出欢乐豆
		{

			lGoldCount=pSelPage->GetDlgItemInt64(IDC_OUT_COUNT);
			DWORD dwTagGameID=pSelPage->GetDlgItemInt(IDC_USERGAME_ID);

			if (dwTagGameID==0)
			{
				ShowInformationEx(TEXT("接收用户ID不正确"),0 ,MB_ICONINFORMATION,TEXT("系统消息"));
				pSelPage->GetDlgItem(IDC_USERGAME_ID)->SetFocus();
				return;
			}

			if (dwTagGameID==m_pMeUserData->dwGameID)
			{
				ShowInformationEx(TEXT("接收用户ID不能是自已，请重新输入"),0 ,MB_ICONINFORMATION,TEXT("系统消息"));
				pSelPage->GetDlgItem(IDC_USERGAME_ID)->SetFocus();
				return;
			}

			if (lGoldCount <= 0 || lGoldCount > m_lStorageGold)
			{
				CString strMessage;
				if (m_lStorageGold>0)
					strMessage.Format(TEXT("你输入的欢乐豆值必须在1和%I64d之间"),m_lStorageGold);
				else 
					strMessage.Format(TEXT("你的存储欢乐豆数目为0,不能进行转帐操作!"));
				ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("系统消息"));
				pSelPage->GetDlgItem(IDC_OUT_COUNT)->SetFocus();
				((CEdit*)pSelPage->GetDlgItem(IDC_OUT_COUNT))->SetSel(0,-1);
				return;
			}

			if ((m_lStorageGold-lGoldCount) < 0)
			{
				ShowInformationEx(TEXT("您的银行必须最少保留 0 欢乐豆，不能全部转出！"),0,MB_ICONINFORMATION,TEXT("系统消息"));
				((CEdit*)pSelPage->GetDlgItem(IDC_OUT_COUNT))->SetSel(0,-1);
				return;
			}
			break;
		}
	case 3: //银行密码
		{
			TCHAR szOldPassword[PASS_LEN]=TEXT("");
			TCHAR szPassword1[PASS_LEN]=TEXT("");
			TCHAR szPassword2[PASS_LEN]=TEXT("");

			pSelPage->GetDlgItemText(IDC_BANK_PASSWORD,szOldPassword,CountArray(szOldPassword));
			pSelPage->GetDlgItemText(IDC_NEW_PASSWORD,szPassword1,CountArray(szPassword1));
			pSelPage->GetDlgItemText(IDC_CONFIRM_PASSWORD,szPassword2,CountArray(szPassword2));

			if (szPassword1[0]==0)
			{
				ShowInformationEx(TEXT("新密码不能为空，请重新填写"),0 ,MB_ICONINFORMATION,TEXT("系统消息"));
				pSelPage->GetDlgItem(IDC_NEW_PASSWORD)->SetFocus();
				return;
			}

			if (lstrlen(szPassword1) < 6)
			{
				ShowInformationEx(TEXT("为了您的密码安全，密码长度必须在5位以上"),0 ,MB_ICONINFORMATION,TEXT("系统消息"));
				pSelPage->GetDlgItem(IDC_NEW_PASSWORD)->SetFocus();
				return;
			}

			if (szPassword2[0]==0)
			{
				ShowInformationEx(TEXT("确认密码不能为空，请重新填写"),0 ,MB_ICONINFORMATION,TEXT("系统消息"));
				pSelPage->GetDlgItem(IDC_CONFIRM_PASSWORD)->SetFocus();
				return;
			}

			if (lstrlen(szPassword2) < 6)
			{
				ShowInformationEx(TEXT("为了您的密码安全，密码长度必须在5位以上"),0 ,MB_ICONINFORMATION,TEXT("系统消息"));
				pSelPage->GetDlgItem(IDC_CONFIRM_PASSWORD)->SetFocus();
				return;
			}

			if (strcmp(szPassword1,szPassword2)!=0)
			{
				ShowInformationEx(TEXT("你输入的两次密码不一致，请重新填写"),0 ,MB_ICONINFORMATION,TEXT("系统消息"));
				pSelPage->GetDlgItem(IDC_CONFIRM_PASSWORD)->SetFocus();
				return;
			}

			if (strcmp(szOldPassword,szPassword1)==0)
			{
				ShowInformationEx(TEXT("您输入的新密码和旧密相同，请重新填写"),0 ,MB_ICONINFORMATION,TEXT("系统消息"));
				pSelPage->GetDlgItem(IDC_NEW_PASSWORD)->SetFocus();
				return;
			}
			break;
		}
	case  4:
		{
			return;
		}
	}

	//密码效验
	TCHAR szPassword[PASS_LEN]=TEXT("");
	
	if (m_nSelPage > 0)
	{
		pSelPage->GetDlgItemText(IDC_BANK_PASSWORD, szPassword, CountArray(szPassword));

		if (szPassword[0]==0)
		{
			ShowInformationEx(TEXT("银行密码不能为空，请重新输入密码！"),0,MB_ICONINFORMATION,TEXT("系统消息")) ;
			pSelPage->GetDlgItem(IDC_BANK_PASSWORD)->SetWindowText("");
			pSelPage->GetDlgItem(IDC_BANK_PASSWORD)->SetFocus();
			return;
		}

		TCHAR szTempPassword[PASS_LEN]=TEXT("");
		CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
		CMD5Encrypt::EncryptData(szTempPassword,szPassword);
	}


	switch( m_nSelPage )
	{
	case 0: //存入欢乐豆
		{
			//发送信息
			CMD_GF_BankStorage BankStorage;
			ZeroMemory(&BankStorage, sizeof(BankStorage));
			BankStorage.lStorageValue = lGoldCount;
			BankStorage.cbGameAction=m_bGameAction;
			CopyMemory(BankStorage.szPassword,szPassword,sizeof(BankStorage.szPassword));
			SendData(MDM_GF_BANK, SUB_GF_BANK_STORAGE, &BankStorage, sizeof(BankStorage));
			break;
		}
	case 1: //取出欢乐豆
		{
			//发送信息
			CMD_GF_BankGet BankGetGold;
			ZeroMemory(&BankGetGold, sizeof(BankGetGold));
			BankGetGold.lGetValue = lGoldCount;
			BankGetGold.cbGameAction=m_bGameAction;
			CopyMemory(BankGetGold.szPassword,szPassword,sizeof(BankGetGold.szPassword));
			SendData(MDM_GF_BANK, SUB_GF_BANK_GET, &BankGetGold, sizeof(BankGetGold));
			break;
		}
	case 2: //转入欢乐豆
		{
			CString static strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
			strTmpNumber1.Empty();
			strTmpNumber2.Empty();
			strNumber.Empty();
			if(lGoldCount==0) strNumber=TEXT("0");
			int nNumberCount=0;
			int count=0;
			INT64 lTmpNumber=lGoldCount;
			while(lGoldCount>0)
			{
				strTmpNumber1.Format(TEXT("%I64d"),lGoldCount%10);
				nNumberCount++;
				strTmpNumber2 = strTmpNumber1+strTmpNumber2;

				if(nNumberCount==3)
				{
					count++;
					if(count==1)
					{
						strTmpNumber2 +=strNumber;
					}
					else
					{
						strTmpNumber2 += (TEXT(",") +strNumber);
					}
					strNumber=strTmpNumber2;
					nNumberCount=0;
					strTmpNumber2=TEXT("");
				}
				lGoldCount/=10;
			}

			if (strTmpNumber2.IsEmpty()==FALSE)
			{
				strTmpNumber2 += (TEXT(",") +strNumber);
				strNumber=strTmpNumber2;
			}

			DWORD dwTagGameID=pSelPage->GetDlgItemInt(IDC_USERGAME_ID);

			INT64 lRevenue=lTmpNumber * 0.02;
			TCHAR szMessage[512]=TEXT("");
			sprintf(szMessage, TEXT("您是否确定要转 %s 欢乐豆（扣税 %I64d 欢乐豆）给用户ID为［%d］的用户？"),
				strNumber,lRevenue,dwTagGameID);
			
			if (ShowInformationEx(szMessage,0,MB_YESNO,TEXT("转帐确认"))!=IDYES) return;
	
			CMD_GF_BankTransfer BankTransfer;
			ZeroMemory(&BankTransfer,sizeof(BankTransfer));
			BankTransfer.dwTagGameID=dwTagGameID;
			BankTransfer.lTransferValue=lTmpNumber;
			BankTransfer.cbGameAction=m_bGameAction;
			CopyMemory(BankTransfer.szPassword,szPassword,sizeof(BankTransfer.szPassword));
			SendData(MDM_GF_BANK, SUB_GF_BANK_TRANSFER, &BankTransfer, sizeof(BankTransfer));

			break;
		}
	case 3: //修改密码
		{
			TCHAR szNewPassword[PASS_LEN]=TEXT("");
			TCHAR szEncrypt[PASS_LEN]=TEXT("");
			pSelPage->GetDlgItemText(IDC_NEW_PASSWORD, szNewPassword, CountArray(szNewPassword));

			CMD_GF_BankSecurity BankSecurity;
			ZeroMemory(&BankSecurity,sizeof(BankSecurity));

			BankSecurity.cbGameAction=m_bGameAction;
			CMD5Encrypt::EncryptData(szNewPassword,szEncrypt);
			CopyMemory(BankSecurity.szPassword,szPassword,sizeof(BankSecurity.szPassword));
			CopyMemory(BankSecurity.szNewPassword,szEncrypt,sizeof(BankSecurity.szNewPassword));

			SendData(MDM_GF_BANK, SUB_GF_BANK_SECURITY, &BankSecurity, sizeof(BankSecurity));
			break;
		}
	case 4:
		break;
	}

	SetWaiting(TRUE);
}

//取消按钮
void CBankStorage::OnBnClickedCancel()
{
	KillTimer(IDI_CHARMVALUE_UPDATE_VIEW);
	DestroyWindow();
}

void CBankStorage::OnEnChangeInCount()
{
	CPageDlg *pSelPage=m_pPageDlg[m_nSelPage];
	ASSERT(pSelPage!=NULL);

	//CString strText, strLeft, strRight;
	//pSelPage->GetDlgItem(IDC_IN_COUNT)->GetWindowText(strText);
	//int len = strText.GetLength();
	//if ( len > 3)
	//{
	//	int nFind = strText.ReverseFind(',');
	//	
	//	if ( nFind == -1 )
	//	{
	//		strText.Insert(3,',');
	//		pSelPage->GetDlgItem(IDC_IN_COUNT)->SetWindowText(strText);
	//		((CEdit*)pSelPage->GetDlgItem(IDC_IN_COUNT))->SetSel(strText.GetLength(),-1);
	//	}
	//	else
	//	{
	//		strLeft = strText.Left(nFind+1);//带逗号
	//		strRight = strText.Right(strText.GetLength()-nFind-1);
	//		if ( strRight.GetLength() > 3 )
	//		{
	//			strRight.Insert(3,',');

	//			strText = strLeft + strRight;
	//			pSelPage->GetDlgItem(IDC_IN_COUNT)->SetWindowText(strText);
	//			((CEdit*)pSelPage->GetDlgItem(IDC_IN_COUNT))->SetSel(strText.GetLength(),-1);
	//		}
	//	}
	//}    

	//过滤回车

	CString strText,strTmpText;
	strText = TEXT("");
	INT64 lGold;
	lGold = pSelPage->GetDlgItemInt64(IDC_IN_COUNT);
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
		pSelPage->GetDlgItem(IDC_IN_COUNT)->SetWindowText(strText);
		((CEdit*)pSelPage->GetDlgItem(IDC_IN_COUNT))->SetSel(strText.GetLength(),-1);
	}
}

void CBankStorage::OnEnChangeOutCount()
{
	CPageDlg *pSelPage=m_pPageDlg[m_nSelPage];
	ASSERT(pSelPage!=NULL);

	switch(m_nSelPage)
	{
	case 1:
		{
			CString strText,strTmpText;
			strText = TEXT("");
			INT64 lGold;
			lGold = pSelPage->GetDlgItemInt64(IDC_OUT_COUNT);
			int NumCount=0;
			int lenCount=0;
			if(lGold==0) strText=TEXT("");
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
				pSelPage->GetDlgItem(IDC_OUT_COUNT)->SetWindowText(strText);
				((CEdit*)pSelPage->GetDlgItem(IDC_OUT_COUNT))->SetSel(strText.GetLength(),-1);
			}
			break;
		}
	case 2:
		{
			CString strText,strTmpText;
			strText = TEXT("");
			INT64 lGold;
			lGold = pSelPage->GetDlgItemInt64(IDC_OUT_COUNT);
			int NumCount=0;
			int lenCount=0;
			if(lGold==0) strText=TEXT("");
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
				pSelPage->GetDlgItem(IDC_OUT_COUNT)->SetWindowText(strText);
				((CEdit*)pSelPage->GetDlgItem(IDC_OUT_COUNT))->SetSel(strText.GetLength(),-1);
			}
			break;
		}
	}
}

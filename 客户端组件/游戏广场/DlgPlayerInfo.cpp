#include "Stdafx.h"
#include "resource.h"
#include "GlobalUnits.h"
#include "DlgPlayerInfo.h"


//////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgPlayerInfo, CSkinDialogEx)
	//ON_WM_PAINT()
	//ON_WM_CTLCOLOR()
	//ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDOK,OnBnClickedOk)   //确定
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CDlgPlayerInfo::CDlgPlayerInfo(CWnd* pParent, tagUserData* pUserData):CSkinDialogEx(IDD_PLAYER_INFO, pParent)
{
	m_pUserData = pUserData;
}

//析构函数
CDlgPlayerInfo::~CDlgPlayerInfo()
{
}
//控件绑定
void CDlgPlayerInfo::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	//DDX_Control(pDX, IDOK, m_btOK);

}
//提取按钮
void CDlgPlayerInfo::OnBnClickedOk()
{

	//AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
	return;
}

//初始化函数
BOOL CDlgPlayerInfo::OnInitDialog()
{
	__super::OnInitDialog();

	SetWindowText("用户信息");
	if (m_pUserData == NULL)
	{
		//AfxMessageBox("用户信息不存在，退出！");
		DestroyWindow();
		return FALSE;
	}
	//SetDlgItemText(IDD_PLAYER_INFO)

	
	//游戏等级
	m_GameRankManager.CreateInstance();
	ASSERT(m_GameRankManager.GetInterface()!=NULL);//pszGameName
	//bool bSuccess=m_GameRankManager->LoadGameLevel("ox", GAME_GENRE_GOLD);

	UpdataUserInfo();
	return TRUE;
}

LPCTSTR CDlgPlayerInfo::GetUserStatus( tagUserData* pUserData )
{
	static TCHAR szBuff[128]={0};
	if(pUserData != NULL)
	{
		switch(pUserData->cbUserStatus)
		{
		case US_PLAY:
			_snprintf(szBuff,CountArray(szBuff), TEXT("正在游戏中[桌号:%d]"), pUserData->wTableID);
			break;
		case US_SIT:
			_snprintf(szBuff,CountArray(szBuff), TEXT("用户坐下[桌号:%d]"), pUserData->wTableID);
			break;
		case US_READY:
			_snprintf(szBuff,CountArray(szBuff), TEXT("准备中[桌号:%d]"), pUserData->wTableID);
			break;
		default:
			lstrcpy(szBuff, TEXT("空闲中"));
			break;
		}
	}
	return szBuff;
}

void CDlgPlayerInfo::UpdataUserData( tagUserData* pUserData )
{
	if(m_hWnd!=NULL && pUserData != NULL)
		m_pUserData = pUserData;
}

void CDlgPlayerInfo::UpdataUserInfo()
{
	//填写用户基本资料
	TCHAR szBuff[256];
	SetDlgItemText(IDC_DLGPLAYERINFO_USERNAME, m_pUserData->szName);
	_snprintf(szBuff, CountArray(szBuff), TEXT("%d"), m_pUserData->dwGameID);
	SetDlgItemText(IDC_DLGPLAYERINFO_ID, szBuff);

	_snprintf(szBuff, CountArray(szBuff), TEXT("%s"), GetUserStatus(m_pUserData));
	SetDlgItemText(IDC_DLGPLAYERINFO_STATUS, szBuff);
	SetDlgItemText(IDC_DLGPLAYERINFO_UNWRITE, m_pUserData->szUnderWrite);

	//游戏资料
	_snprintf(szBuff, CountArray(szBuff), TEXT("%d"), m_pUserData->lExperience);
	SetDlgItemText(IDC_DLGPLAYERINFO_JIFEN, szBuff);

	_snprintf(szBuff, CountArray(szBuff), TEXT("%I64d"), m_pUserData->lScore);
	SetDlgItemText(IDC_DLGPLAYERINFO_SCORE, szBuff);

	TCHAR szLevel[128];
	if (m_GameRankManager.GetInterface()!=NULL)
	{
		LPCTSTR pszGameRank=m_GameRankManager->GetLevelDescribe(m_pUserData->lScore);
		lstrcpyn(szLevel,pszGameRank,sizeof(szLevel));
	}else szLevel[0] = 0;
	SetDlgItemText(IDC_DLGPLAYERINFO_LEVER, szLevel);

	_snprintf(szBuff, CountArray(szBuff), TEXT("赢: %ld,输: %ld,和: %ld,逃: %ld"), m_pUserData->lWinCount, m_pUserData->lLostCount, m_pUserData->lDrawCount, m_pUserData->lFleeCount);
	SetDlgItemText(IDC_DLGPLAYERINFO_ZHANJI, szBuff);
	UpdateData(FALSE);
}

// BOOL CDlgPlayerInfo::OnEraseBkgnd( CDC * pDC )
// {
// 	Invalidate(FALSE);
// 	UpdateWindow();
// 	return TRUE;
// }
// 
// void CDlgPlayerInfo::OnPaint()
// {
// // 	CPaintDC dc(this);
// // 	//绘画边框
// // 	DrawBorder(&dc);
// // 
// // 	//绘画背景
// // 	DrawBackGround(&dc);
// 
// 	CPaintDC dc(this);
// 
// 	//获取位置
// 	CRect rcRect;
// 	GetClientRect(&rcRect);
// // 
// // 	//加载资源
// // 	CImageHandle ImageHotHandle(&m_ImageHot);
// // 	CImageHandle ImageActiveHandle(&m_ImageActive);
// // 	CImageHandle ImageNormalHandle(&m_ImageNormal);
// // 	CImageHandle m_ImageBackGroundHandle(&m_ImageBackGround);
// 
// 	//建立缓冲
// 	CDC BufferDC;
// 	CBitmap BufferBmp;
// 	BufferDC.CreateCompatibleDC(&dc);
// 	BufferBmp.CreateCompatibleBitmap(&dc,rcRect.Width(),rcRect.Height());
// 	BufferDC.SelectObject(&BufferBmp);
// 
// 	//绘画背景
// 	CBrush BackBrush(RGB(255,255,254));
// 	CPen BorderPen(PS_SOLID,1,CSkinWndObject::m_SkinAttribute.m_crInsideBorder);
// 	CPen * pOldPen=BufferDC.SelectObject(&BorderPen);
// 	CBrush * pOldBrush=BufferDC.SelectObject(&BackBrush);
// 	BufferDC.FillSolidRect(&rcRect,CSkinWndObject::m_SkinAttribute.m_crBackGround);
// 	BufferDC.RoundRect(&rcRect,CPoint(10,10));
// 	BufferDC.SelectObject(pOldPen);
// 	BufferDC.SelectObject(pOldBrush);
// 	BackBrush.DeleteObject();
// 	BorderPen.DeleteObject();
// 
// 	//绘画按钮
// 	BufferDC.SetBkMode(TRANSPARENT);
// 	BufferDC.SelectObject(CSkinResourceManager::GetDefaultFont());
// 
// 
// 	//tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
// 	if( m_pUserData->dwUserID != 0)
// 	{
// 		CRect m_rcFace = CRect(27-4,27-4,27+48+4,27+48+4);
// 
// 		//用户头像
// 		g_GlobalUnits.m_UserFaceRes->DrawNormalFace(&BufferDC,27,27,m_pUserData->wFaceID,m_pUserData->dwUserID,m_pUserData->dwCustomFaceVer,48,48);
// 	}
// 
// 
// 	//绘画界面
// 	dc.BitBlt(0,0,rcRect.Width(),rcRect.Height(),&BufferDC,0,0,SRCCOPY);
// 
// 	//清理资源
// 	BufferDC.DeleteDC();
// 	BufferBmp.DeleteObject();
// 
// 	DrawBorder(&dc);
// 	//绘画标题
// 	DrawCaption(&dc);
// 	//绘画背景
// 	DrawBackGround(&dc);
// 
// }
// 
// //颜色消息
// HBRUSH  CDlgPlayerInfo::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
// {
// 
// 	if(pWnd->GetDlgCtrlID()==IDC_GROUP1||pWnd->GetDlgCtrlID()==IDC_GROUP2)
// 	{
// 		pDC->SetBkColor(CSkinDialog::m_SkinAttribute.m_crBackGround);
// 		pDC->SetTextColor(CSkinDialog::m_SkinAttribute.m_crControlTXColor);
// 
// 		HBRUSH hr = CreateSolidBrush(CSkinDialog::m_SkinAttribute.m_crBackGround);
// 		return hr;
// 
// 	}
// 
// 	switch (nCtlColor)
// 	{
// 	case CTLCOLOR_DLG:
// 	case CTLCOLOR_STATIC:
// 		{
// 			pDC->SetBkMode(TRANSPARENT);
// 			pDC->SetBkColor(CSkinDialog::m_SkinAttribute.m_crBackGround);
// 			pDC->SetTextColor(CSkinDialog::m_SkinAttribute.m_crControlTXColor);
// 			return CSkinDialog::m_SkinAttribute.m_brBackGround;
// 		}
// 	}
// 	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
// }



#include "Stdafx.h"
#include "AfxPriv.h"
#include "SkinDialog.h"

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSkinDialog, CDialog)

BEGIN_MESSAGE_MAP(CSkinDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETTINGCHANGE()
	ON_WM_WINDOWPOSCHANGED()
	ON_MESSAGE(WM_SETTEXT,OnSetTextMesage)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CSkinDialog1, CDialog)

BEGIN_MESSAGE_MAP(CSkinDialog1, CDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETTINGCHANGE()
	ON_WM_WINDOWPOSCHANGED()
	ON_MESSAGE(WM_SETTEXT,OnSetTextMesage)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CSkinDialogEx, CSkinDialog)

BEGIN_MESSAGE_MAP(CSkinDialogEx,CSkinDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CSkinDialogEx1, CSkinDialog1)

BEGIN_MESSAGE_MAP(CSkinDialogEx1,CSkinDialog1)
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CRgnDialog, CDialog)

BEGIN_MESSAGE_MAP(CRgnDialog,CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CSkinDialog::CSkinDialog(UINT nIDTemplate, CWnd * pParentWnd) 
	: CDialog(nIDTemplate,pParentWnd), CSkinWndObject(this)
{
}

//析构函数
CSkinDialog::~CSkinDialog()
{
}

//初始化消息
BOOL CSkinDialog::OnInitDialog()
{
	__super::OnInitDialog();

	//初始化
	InitSkinObject();
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
    

	return TRUE;
}

//激活消息
void CSkinDialog::OnActivate(UINT nState, CWnd * pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);

	m_bActive=(nState!=WA_INACTIVE);
	DrawSkinView(NULL);

	return;
}

//激活消息
void CSkinDialog::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	__super::OnActivateApp(bActive,dwThreadID);

	m_bActive=bActive?true:false;
	DrawSkinView(NULL);

	return;
}

//鼠标消息
void CSkinDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags,point);
	OnLMouseDoubleEvent(point);
	return;
}

//鼠标消息
void CSkinDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);
	OnLMouseDownEvent(point);
	return;
}

//鼠标消息
void CSkinDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
	__super::OnLButtonUp(nFlags,point);
	OnLMouseUpEvent(point);
	return;
}

//鼠标消息
void CSkinDialog::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnRButtonDblClk(nFlags,point);
	OnRMouseDoubleEvent(point);
	return;
}

//鼠标消息
void CSkinDialog::OnRButtonDown(UINT nFlags, CPoint point)
{
	__super::OnRButtonDown(nFlags,point);
	OnRMouseDownEvent(point);
	return;
}

//鼠标消息
void CSkinDialog::OnRButtonUp(UINT nFlags, CPoint point)
{
	__super::OnRButtonUp(nFlags,point);
	OnRMouseUpEvent(point);
	return;
}

//鼠标消息
void CSkinDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	__super::OnMouseMove(nFlags,point);
	OnMouseMoveEvent(point);
	return;
}

//绘画消息 
void CSkinDialog::OnPaint()
{
	CPaintDC dc(this);
	DrawSkinView(&dc);
	return;
}

//位置改变
void CSkinDialog::OnWindowPosChanged(WINDOWPOS * lpwndpos)
{
	__super::OnWindowPosChanged(lpwndpos);

	//调整位置
	CRect rcClient;
	m_cbNowHotButton=255;
	m_cbNowDownButton=255;
	GetClientRect(&rcClient);
	AdjustTitleButton(rcClient.Width(),rcClient.Height());

	//重画界面
	DrawSkinView(NULL);

	return;
}

//设置改变
void CSkinDialog::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags, lpszSection);

	//调整框架大小
	CSkinWndObject::OnSettingChange();

	return;
}

//绘画背景
BOOL CSkinDialog::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//控件颜色
HBRUSH CSkinDialog::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(m_SkinAttribute.m_crBackGround);
			pDC->SetTextColor(m_SkinAttribute.m_crControlTXColor);
			return m_SkinAttribute.m_brBackGround;
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//标题消息
LRESULT	CSkinDialog::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult=DefWindowProc(WM_SETTEXT,wParam,lParam);
	DrawSkinView(NULL);
	return lResult;
}

//////////////////////////////////////////////////////////////////////////
//构造函数
CSkinDialog1::CSkinDialog1(UINT nIDTemplate1, CWnd * pParentWnd1) 
	: CDialog(nIDTemplate1,pParentWnd1), CSkinWndObject(this)
{
}

//析构函数
CSkinDialog1::~CSkinDialog1()
{
}

//初始化消息
BOOL CSkinDialog1::OnInitDialog()
{
	__super::OnInitDialog();

	//初始化
	InitSkinObject();
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return TRUE;
}

//激活消息
void CSkinDialog1::OnActivate(UINT nState, CWnd * pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);

	m_bActive=(nState!=WA_INACTIVE);
	DrawSkinView1(NULL);

	return;
}

//激活消息
void CSkinDialog1::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	__super::OnActivateApp(bActive,dwThreadID);

	m_bActive=bActive?true:false;
	DrawSkinView1(NULL);

	return;
}

//鼠标消息
void CSkinDialog1::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags,point);
	OnLMouseDoubleEvent(point);
	return;
}

//鼠标消息
void CSkinDialog1::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);
	OnLMouseDownEvent1(point);
	return;
}

//鼠标消息
void CSkinDialog1::OnLButtonUp(UINT nFlags, CPoint point)
{
	__super::OnLButtonUp(nFlags,point);
	OnLMouseUpEvent1(point);
	return;
}

//鼠标消息
void CSkinDialog1::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnRButtonDblClk(nFlags,point);
	OnRMouseDoubleEvent(point);
	return;
}

//鼠标消息
void CSkinDialog1::OnRButtonDown(UINT nFlags, CPoint point)
{
	__super::OnRButtonDown(nFlags,point);
	OnRMouseDownEvent(point);
	return;
}

//鼠标消息
void CSkinDialog1::OnRButtonUp(UINT nFlags, CPoint point)
{
	__super::OnRButtonUp(nFlags,point);
	OnRMouseUpEvent(point);
	return;
}

//鼠标消息
void CSkinDialog1::OnMouseMove(UINT nFlags, CPoint point)
{
	__super::OnMouseMove(nFlags,point);
	OnMouseMoveEvent1(point);
	return;
}

//绘画消息 
void CSkinDialog1::OnPaint()
{
	CPaintDC dc(this);
	DrawSkinView1(&dc);
	return;
}

//位置改变
void CSkinDialog1::OnWindowPosChanged(WINDOWPOS * lpwndpos)
{
	__super::OnWindowPosChanged(lpwndpos);

	//调整位置
	CRect rcClient;
	m_cbNowHotButton=255;
	m_cbNowDownButton=255;
	GetClientRect(&rcClient);
	AdjustTitleButton(rcClient.Width(),rcClient.Height());

	//重画界面
	DrawSkinView1(NULL);

	return;
}

//设置改变
void CSkinDialog1::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags, lpszSection);

	//调整框架大小
	CSkinWndObject::OnSettingChange();

	return;
}

//绘画背景
BOOL CSkinDialog1::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//控件颜色
HBRUSH CSkinDialog1::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(m_SkinAttribute.m_crBackGround);
			pDC->SetTextColor(m_SkinAttribute.m_crControlTXColor);
			return m_SkinAttribute.m_brBackGround;
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//标题消息
LRESULT	CSkinDialog1::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult=DefWindowProc(WM_SETTEXT,wParam,lParam);
	DrawSkinView1(NULL);
	return lResult;
}
//////////////////////////////////////////////////////////////////////////
//构造函数
CSkinDialogEx::CSkinDialogEx(UINT nIDTemplate, CWnd * pParentWnd) : CSkinDialog(nIDTemplate,pParentWnd)
{
}

//析构函数
CSkinDialogEx::~CSkinDialogEx()
{
}

//绘画边框
void CSkinDialogEx::DrawBorder(CDC * pDC)
{
	//获取位置
	CRect ClientRect;
	GetClientRect(&ClientRect);
	int nXExcursion=GetXExcursionPos();
	int nYExcursion=GetYExcursionPos();

	//创建笔画
	CPen BorderPen(PS_SOLID,1,m_SkinAttribute.m_crBackGround);
	CPen * pOldPen=pDC->SelectObject(&BorderPen);

	CImageHandle TitleLeft1(&m_SkinAttribute.m_FramLeft);
	if (m_SkinAttribute.m_FramLeft.IsNull()) return ;
	//绘画内框
	CRect rcDrawRect(nXExcursion,nYExcursion,ClientRect.Width()-nXExcursion,ClientRect.Height());
	pDC->SelectObject(m_SkinAttribute.m_brBackGround);

// 	if (m_bMaxSize==true) rcDrawRect.DeflateRect(4,4,4,4);
// 	else rcDrawRect.DeflateRect(4,4,4,4);
	pDC->RoundRect(&rcDrawRect,CPoint(0,0));
    
	//	CImageHandle TitleLeft1(&m_SkinAttribute.m_TitleLeft1);
	//if (m_SkinAttribute.m_TitleLeft1.IsNull()) return ;
	//CImageHandle TitleMid1(&m_SkinAttribute.m_TitleMid1);
	//if (m_SkinAttribute.m_TitleMid1.IsNull()) return ;
	//CImageHandle TitleRight1(&m_SkinAttribute.m_TitleRight1);
	//if (m_SkinAttribute.m_TitleRight1.IsNull()) return ;

	//int nLeftWidth=TitleLeft1->GetHeight();
	//int nMidWidth=TitleMid1->GetWidth();
	//int nRightWidth=TitleRight1->GetWidth();

	////for (int i=rcDrawRect.top;i<rcDrawRect.bottom;i+=nLeftWidth)
	//TitleLeft1->BitBlt(pDC->m_hDC,rcDrawRect.left,rcDrawRect.top);
	//pDC->Draw3dRect(&rcDrawRect,RGB(255,0,0),RGB(255,0,0));

	//绘制背景
// 	CImageHandle BackGroundT(&m_SkinAttribute.m_BackGroundTop);
// 	CImageHandle BackGroundM(&m_SkinAttribute.m_BackGroundMid);
// 	CImageHandle BackGroundB(&m_SkinAttribute.m_BackGroundBtm);
// 	
// 	for (int xPox = rcDrawRect.left; xPox < rcDrawRect.right; xPox+=m_SkinAttribute.m_BackGroundMid.GetWidth())
// 	{
// 		m_SkinAttribute.m_BackGroundTop.BitBlt(pDC->GetSafeHdc(), xPox, rcDrawRect.top);
// 		for (int yMid=rcDrawRect.top+m_SkinAttribute.m_BackGroundTop.GetHeight(); 
// 			 yMid < rcDrawRect.bottom-m_SkinAttribute.m_BackGroundBtm.GetHeight(); yMid+=m_SkinAttribute.m_BackGroundMid.GetHeight())
// 		{
// 			m_SkinAttribute.m_BackGroundMid.BitBlt(pDC->GetSafeHdc(), xPox, yMid);
// 		}
// 		m_SkinAttribute.m_BackGroundBtm.BitBlt(pDC->GetSafeHdc(), xPox, rcDrawRect.bottom-m_SkinAttribute.m_BackGroundBtm.GetHeight());
// 	}

	//清理资源
	pDC->SelectObject(pOldPen);
	BorderPen.DeleteObject();

	return ;
}

//绘画标题
void CSkinDialogEx::DrawCaption(CDC * pDC)
{
	//绘画标题
	DrawSkinView(pDC);

	return ;
}

//绘画背景
void CSkinDialogEx::DrawBackGround(CDC * pDC)
{
	//获取位置
	CRect ClientRect;
	GetClientRect(&ClientRect);

	//绘画背景
	int nXExcursion1=GetXExcursionPos();
	int nYExcursion1=GetYExcursionPos();
	CImageHandle TitleLeft1(&m_SkinAttribute.m_FramLeft);
	if (m_SkinAttribute.m_FramLeft.IsNull()) return ;
	CImageHandle TitleMid1(&m_SkinAttribute.m_BottomM);
	if (m_SkinAttribute.m_BottomM.IsNull()) return ;
	CImageHandle TitleRight1(&m_SkinAttribute.m_FramRight);
	if (m_SkinAttribute.m_FramRight.IsNull()) return ;

	CImageHandle TitleLeftL1(&m_SkinAttribute.m_BottomL);
	if (m_SkinAttribute.m_FramRight.IsNull()) return ;

	CImageHandle TitleRightR1(&m_SkinAttribute.m_BottomR);
	if (m_SkinAttribute.m_FramRight.IsNull()) return ;


	int nLeftWidth=TitleLeft1->GetHeight();
	int nMidWidth=TitleMid1->GetWidth();
	int nRightWidth=TitleRight1->GetHeight();
    
	for (int i=ClientRect.top+26;i<ClientRect.bottom-TitleLeftL1->GetHeight();i+=nLeftWidth)
	TitleLeft1->BitBlt(pDC->GetSafeHdc(),nXExcursion1,i);

	for (int i=ClientRect.top+26;i<ClientRect.bottom-TitleRightR1->GetHeight();i+=nRightWidth)
		TitleRight1->BitBlt(pDC->GetSafeHdc(),ClientRect.Width()-nXExcursion1-TitleRight1->GetWidth(),i);

	for (int i=nXExcursion1+TitleLeftL1->GetWidth();i<ClientRect.Width()-TitleRight1->GetWidth()-nXExcursion1;i+=nMidWidth)
		TitleMid1->BitBlt(pDC->GetSafeHdc(),i,ClientRect.Height()-TitleMid1->GetHeight());

    TitleLeftL1->BitBlt(pDC->GetSafeHdc(),nXExcursion1,ClientRect.Height()-TitleLeftL1->GetHeight());

	TitleRightR1->BitBlt(pDC->GetSafeHdc(),ClientRect.Width()-TitleRightR1->GetWidth()-nXExcursion1,ClientRect.Height()-TitleRightR1->GetHeight());
	return;
}

//重画消息
void CSkinDialogEx::OnPaint()
{
	CPaintDC dc(this);

	//绘画背景
	//DrawBackGround(&dc);

	//绘画边框
	DrawBorder(&dc);
	//绘画标题
	DrawCaption(&dc);
	//绘画背景
	DrawBackGround(&dc);

// 	CRect rcClient1;
// 	GetWindowRect(&rcClient1);
// 	int nWidth1=rcClient1.Width();
// 	int nHeight1=rcClient1.Height();
// 	//调整判断
// 	if ((nWidth1!=0)&&(nHeight1!=0))
// 	{
// 		//框架位置
// 		CRect rcFrame;//=rcClient1;
// 		rcFrame.SetRect(0,0,nWidth1,nHeight1);
// 
// 		//窗口区域
// 		CRgn RegionWindow;
// 		RegionWindow.CreateRoundRectRgn(rcFrame.left,rcFrame.top,rcFrame.right+1,rcFrame.bottom+1,5,5);
// 
// 		//设置区域
// 		SetWindowRgn(RegionWindow,TRUE);
// 	}
	///////////////////////////////////////////////////////////////////////////


	return;
}

//位置消息
void CSkinDialogEx::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	/////////////////////////////////////////////////////////////////////////////


	//刷新界面
	Invalidate(FALSE);

	return;
}

//////////////////////////////////////////////////////////////////////////
CSkinDialogEx1::CSkinDialogEx1(UINT nIDTemplate1, CWnd * pParentWnd1) : CSkinDialog1(nIDTemplate1,pParentWnd1)
{
}

//析构函数
CSkinDialogEx1::~CSkinDialogEx1()
{
}

//绘画边框
void CSkinDialogEx1::DrawBorder(CDC * pDC)
{
	//获取位置
	CRect ClientRect;
	GetClientRect(&ClientRect);
	int nXExcursion=GetXExcursionPos();
	int nYExcursion=GetYExcursionPos();

	//创建笔画
	CPen BorderPen(PS_SOLID,1,m_SkinAttribute.m_crInsideBorder);
	CPen * pOldPen=pDC->SelectObject(&BorderPen);

	//绘画内框
	CRect rcDrawRect(nXExcursion,nYExcursion,ClientRect.Width()-2*nXExcursion,ClientRect.Height());
	pDC->SelectObject(m_SkinAttribute.m_brBackGround);
	if (m_bMaxSize==true) rcDrawRect.DeflateRect(3,3,3,2);
	else rcDrawRect.DeflateRect(3,4,2,4);
	pDC->RoundRect(&rcDrawRect,CPoint(8,8));

	//清理资源
	pDC->SelectObject(pOldPen);
	BorderPen.DeleteObject();

	return ;
}

//绘画标题
void CSkinDialogEx1::DrawCaption(CDC * pDC)
{
	//绘画标题
	DrawSkinView1(pDC);

	return ;
}

//绘画背景
void CSkinDialogEx1::DrawBackGround(CDC * pDC)
{
	//获取位置
	CRect ClientRect;
	GetClientRect(&ClientRect);

	//绘画背景
	int nXExcursion=GetXExcursionPos();
	int nYExcursion=GetYExcursionPos();
	pDC->FillSolidRect(nXExcursion,nYExcursion,ClientRect.Width()-2*nXExcursion,4,m_SkinAttribute.m_crBackFrame);
	pDC->FillSolidRect(nXExcursion,ClientRect.Height()-8,ClientRect.Width()-2*nXExcursion,7,m_SkinAttribute.m_crBackFrame);
	pDC->FillSolidRect(nXExcursion,nYExcursion+4,12,ClientRect.Height()-nYExcursion-9,m_SkinAttribute.m_crBackFrame);
	pDC->FillSolidRect(ClientRect.Width()-nXExcursion-12,nYExcursion+4,12,ClientRect.Height()-nYExcursion-9,m_SkinAttribute.m_crBackFrame);

	return;
}

//重画消息
void CSkinDialogEx1::OnPaint()
{
	CPaintDC dc(this);
   
	//绘画标题
	//DrawCaption(&dc);

	////绘画背景
	//DrawBackGround(&dc);

	////绘画边框
	//DrawBorder(&dc);

	return;
}

//位置消息
void CSkinDialogEx1::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog1::OnSize(nType, cx, cy);

	//刷新界面
	Invalidate(FALSE);
	/////////////////////////////////////////////////////////////////////////////
	//CRect rcClient;
	//GetWindowRect(&rcClient);
	//int nWidth=rcClient.Width();
	//int nHeight=rcClient.Height();
	////调整判断
	//if ((nWidth!=0)&&(nHeight!=0))
	//{


	//	//框架位置
	//	CRect rcFrame;
	//	rcFrame.SetRect(5,5,nWidth-5,nHeight-5);

	//	//窗口区域
	//	CRgn RegionWindow;
	//	RegionWindow.CreateRoundRectRgn(rcFrame.left,rcFrame.top,rcFrame.right+1,rcFrame.bottom+1,5,5);

	//	//设置区域
	//	SetWindowRgn(RegionWindow,TRUE);
	//}
	///////////////////////////////////////////////////////////////////////////
	return;
}

///////////////////////////////////////////////////////
//构造函数
CRgnDialog::CRgnDialog(UINT uTemplate, CWnd * pParent) : CDialog(uTemplate,pParent)
{
}

//析构函数
CRgnDialog::~CRgnDialog()
{
}

//初始化函数
BOOL CRgnDialog::OnInitDialog()
{
	__super::OnInitDialog();

	//删除窗口标题和获取属性
	ModifyStyle(WS_CAPTION,0,SWP_DRAWFRAME);
	ModifyStyleEx(WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return TRUE;
}

//重画函数
void CRgnDialog::OnPaint()
{
	CPaintDC dc(this);

	//绘画底图
	CRect ClientRect;
	GetClientRect(&ClientRect);
	CImageHandle ImageHandle(&m_ImageBack);
	if (m_ImageBack.IsNull()==false) m_ImageBack.BitBlt(dc,0,0);

	return;
}

//加载位图
bool CRgnDialog::LoadRgnImage(HINSTANCE hInstance, UINT uBitmapID, COLORREF crTrans)
{
	m_ImageBack.SetLoadInfo(uBitmapID,hInstance);
	return CreateControlRgn(crTrans);
}

//加载位图
bool CRgnDialog::LoadRgnImage(LPCTSTR pszFileName, COLORREF crTrans)
{
	m_ImageBack.SetLoadInfo(pszFileName);
	return CreateControlRgn(crTrans);
}

//创建区域
bool CRgnDialog::CreateControlRgn(COLORREF crTrans)
{
	//加载资源
	CImageHandle ImageHandle(&m_ImageBack);

	//创建区域
	CRgn RgnControl;
	m_ImageBack.CreateImageRegion(RgnControl,crTrans);

	//设置窗口
	if (RgnControl.GetSafeHandle()!=NULL)
	{
		//移动窗口
		SetWindowRgn(RgnControl,TRUE);
		SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE);

		//删除区域
		m_DialogRgn.DeleteObject();
		m_DialogRgn.Attach(RgnControl.Detach());

		return true;
	}

	return false;
}

//鼠标右键按下消息
void CRgnDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags, point);

	//模拟按标题
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
}

//背景函数
BOOL CRgnDialog::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

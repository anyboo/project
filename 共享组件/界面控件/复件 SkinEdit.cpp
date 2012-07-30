#include "Stdafx.h"
#include "SkinEdit.h"

//////////////////////////////////////////////////////////////////////////

//静态变量
CSkinEditAttribute					CSkinEdit::m_SkinAttribute;				//界面属性

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSkinEdit, CEdit)

//BEGIN_MESSAGE_MAP(CSkinEdit, CEdit)
//	ON_WM_SETFOCUS()
//	ON_WM_KILLFOCUS()
//	ON_WM_PAINT()
////	ON_WM_CTLCOLOR_REFLECT()
//	ON_WM_MOUSEMOVE()
//	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
//END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CSkinEdit, CEdit)
	ON_WM_NCPAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CSkinEditEx, CSkinEdit)
    ON_WM_NCPAINT()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CSkinEditAttribute::CSkinEditAttribute() 
{
	//初始化变量
	m_crFocusTX=RGB(0,0,0);
	m_crFocusBK=RGB(0,0,0);
	m_crNoFocusTX=RGB(0,0,0);
	m_crNoFocusBK=RGB(0,0,0);
	m_crDisFocusTX=RGB(0,0,0);
	m_crDisFocusBK=RGB(0,0,0);

	return;
}

//析构函数
CSkinEditAttribute::~CSkinEditAttribute() 
{
}

//加载配置
bool CSkinEditAttribute::UpdateSkinResource(ISkinResource * pISkinResource)
{
	//获取信息
	tagSkinEditResource SkinEditResource;
	if (pISkinResource!=NULL) pISkinResource->GetResourceInfo(SkinEditResource);

	//设置变量
	m_crFocusTX=SkinEditResource.crFocusTX;
	m_crFocusBK=SkinEditResource.crFocusBK;
	m_crNoFocusTX=SkinEditResource.crNoFocusTX;
	m_crNoFocusBK=SkinEditResource.crNoFocusBK;
	m_crDisFocusTX=SkinEditResource.crDisFocusTX;
	m_crDisFocusBK=SkinEditResource.crDisFocusBK;
	m_ImageBottom.SetLoadInfo(SkinEditResource.m_ImageBottom.uResourceID,SkinEditResource.m_ImageBottom.hResourceDLL);
	//创建画刷
	m_brFocus.DeleteObject();
	m_brNoFocus.DeleteObject();
	m_brDisbale.DeleteObject();
	m_brFocus.CreateSolidBrush(m_crFocusBK);
	m_brNoFocus.CreateSolidBrush(m_crNoFocusBK);

	return true;
}

//////////////////////////////////////////////////////////////////////////

////构造函数
//CSkinEdit::CSkinEdit()
//{
//	m_bFocus=false;
//	m_bHovering=false;
//}
//
////析构函数
//CSkinEdit::~CSkinEdit()
//{
//}
//
////绘画控件
////HBRUSH CSkinEdit::CtlColor(CDC * pDC, UINT nCtlColor)
////{
////	pDC->SetBkMode(OPAQUE);
////	if (IsWindowEnabled())
////	{
////		if (m_bFocus) 
////		{
////			pDC->SetTextColor(RGB(255,255,255));
////			pDC->SetBkColor(RGB(255,255,255));
////			return m_SkinAttribute.m_brFocus;
////		}
////		pDC->SetTextColor(RGB(255,255,255));
////		pDC->SetBkColor(RGB(255,255,255));
////		return m_SkinAttribute.m_brNoFocus;
////	}
////	pDC->SetTextColor(RGB(255,255,255));
////	pDC->SetBkColor(RGB(255,255,255));
////	return m_SkinAttribute.m_brDisbale;
////}
//
////得到焦点
//void CSkinEdit::OnSetFocus(CWnd * pOldWnd)
//{
//	m_bFocus=true;
//	Invalidate(TRUE);
//	__super::OnSetFocus(pOldWnd);
//}
//
////失去焦点
//void CSkinEdit::OnKillFocus(CWnd * pNewWnd)
//{
//	m_bFocus=false;
//	Invalidate(TRUE);
//	__super::OnKillFocus(pNewWnd);
//}
//void CSkinEdit::OnMouseMove(UINT nFlags, CPoint point)
//{
//
//	if (m_bHovering==false)
//	{
//		//注册消息
//		m_bHovering=true;
//		Invalidate(FALSE);
//		TRACKMOUSEEVENT TrackMouseEvent;
//		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);
//		TrackMouseEvent.dwFlags=TME_LEAVE;
//		TrackMouseEvent.hwndTrack=GetSafeHwnd();
//		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
//		_TrackMouseEvent(&TrackMouseEvent);
//	}
//
//	CEdit::OnMouseMove(nFlags, point);
//}
////鼠标离开消息
//LRESULT CSkinEdit::OnMouseLeave(WPARAM wparam, LPARAM lparam)
//{
//	m_bHovering=false;
//	Invalidate(FALSE);
//	return 0;
//}
////绘画
//void CSkinEdit::OnPaint()
//{
//	CEdit::OnPaint();
//	CDC *pDC = GetWindowDC();
//	//获取位置
//
//	CRect rect;
//	GetWindowRect(&rect); //获得窗口矩形
//	rect -= rect.TopLeft();
//	CPen LinePen(PS_SOLID,1,RGB(81,197,254));
//	CPen LinePen1(PS_SOLID,1,RGB(255,255,255));
//	rect.right-=1;
//	rect.bottom-=1;
//	pDC->SelectObject(&LinePen);
//	//pDC->Rectangle( rect.right - 6,rect.bottom - 6,rect.right,rect.bottom );
//	pDC->MoveTo(rect.left,rect.top);
//	pDC->LineTo(rect.left,rect.bottom+1);
//	pDC->MoveTo(rect.left,rect.top);
//	pDC->LineTo(rect.right,rect.top);
//	pDC->MoveTo(rect.right,rect.top);
//	pDC->LineTo(rect.right,rect.bottom);
//	pDC->MoveTo(rect.right,rect.bottom);
//	pDC->LineTo(rect.left,rect.bottom);
//	pDC->MoveTo(rect.right,rect.bottom);
//	pDC->LineTo(rect.left,rect.bottom);
//	pDC->SelectObject(&LinePen1);
//	pDC->MoveTo(rect.left+1,rect.top+1);
//	pDC->LineTo(rect.left+1,rect.bottom-1);
//	pDC->MoveTo(rect.left+1,rect.top+1);
//	pDC->LineTo(rect.right,rect.top+1);
//	LinePen.DeleteObject();
//	LinePen1.DeleteObject();
//	ReleaseDC( pDC );
//	
//}

//构造函数
CSkinEdit::CSkinEdit()
{
	m_bFocus=false;
}

//析构函数
CSkinEdit::~CSkinEdit()
{
}

//绘画控件
HBRUSH CSkinEdit::CtlColor(CDC * pDC, UINT nCtlColor)
{
	pDC->SetBkMode(OPAQUE);
	if (IsWindowEnabled())
	{
		if (m_bFocus) 
		{
			pDC->SetTextColor(m_SkinAttribute.m_crFocusTX);
			pDC->SetBkColor(m_SkinAttribute.m_crFocusBK);
			return m_SkinAttribute.m_brFocus;
		}
		pDC->SetTextColor(m_SkinAttribute.m_crNoFocusTX);
		pDC->SetBkColor(m_SkinAttribute.m_crNoFocusBK);
		return m_SkinAttribute.m_brNoFocus;
	}
	pDC->SetTextColor(m_SkinAttribute.m_crDisFocusBK);
	pDC->SetBkColor(m_SkinAttribute.m_crDisFocusBK);
	return m_SkinAttribute.m_brDisbale;
}

//得到焦点
void CSkinEdit::OnSetFocus(CWnd * pOldWnd)
{
	m_bFocus=true;
	Invalidate(TRUE);
	__super::OnSetFocus(pOldWnd);
}

//失去焦点
void CSkinEdit::OnKillFocus(CWnd * pNewWnd)
{
	m_bFocus=false;
	Invalidate(TRUE);
	__super::OnKillFocus(pNewWnd);
}

//绘画控件
VOID CSkinEdit::DrawControlView(CDC * pDC, COLORREF crColorBorad,  COLORREF crColorBK)
{
	//获取位置
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	ScreenToClient(&rcWindow);

	//绘画边框
	if (GetExStyle()&WS_EX_CLIENTEDGE)
	{
		pDC->Draw3dRect(&rcWindow,crColorBorad,crColorBorad);
		pDC->Draw3dRect(rcWindow.left+1,rcWindow.top+1,rcWindow.Width()-2,rcWindow.Height()-2,crColorBK,crColorBK);
	}
	else
	{
		pDC->Draw3dRect(&rcWindow,crColorBK,crColorBK);
		pDC->Draw3dRect(rcWindow.left+1,rcWindow.top+1,rcWindow.Width()-2,rcWindow.Height()-2,crColorBK,crColorBK);
	}

	return;
}

//重画消息
VOID CSkinEdit::OnNcPaint()
{
	//状态变量
	bool bDisable=(IsWindowEnabled()==FALSE);
	COLORREF crColorBK=(bDisable==false)?RGB(255,255,255):RGB(125,125,125);
	COLORREF crColorBorad=(bDisable==false)?RGB(92,100,105):RGB(255,255,255);

	//绘画边框
	CClientDC ClientDC(this);
	DrawControlView(&ClientDC,crColorBorad,crColorBK);

	return;
}


//构造函数
CSkinEditEx::CSkinEditEx()
{
	//背景颜色
	m_crEnableBK=RGB(255,255,255);;
	m_crDisableBK=RGB(125,125,125);;

	//字体颜色
	m_crEnableText=RGB(10,10,10);
	m_crDisableText=RGB(50,50,50);

	//边框颜色
	m_crEnableBorad=RGB(92,100,105);
	m_crDisableBorad=RGB(255,255,255); //

	return;
}

//析构函数
CSkinEditEx::~CSkinEditEx()
{
}

//设置颜色
VOID CSkinEditEx::SetEnableColor(COLORREF crEnableText, COLORREF crEnableBK, COLORREF crEnableBorad)
{
	//设置颜色
	m_crEnableBK=crEnableBK;
	m_crEnableText=crEnableText;
	m_crEnableBorad=crEnableBorad;

	//创建画刷
	m_brEnable.DeleteObject();
	m_brEnable.CreateSolidBrush(m_crEnableBK);

	//更新界面
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//设置颜色
VOID CSkinEditEx::SetDisableColor(COLORREF crDisableText, COLORREF crDisableBK, COLORREF crDisableBorad)
{
	//设置颜色
	m_crDisableBK=crDisableBK;
	m_crDisableText=crDisableText;
	m_crDisableBorad=crDisableBorad;

	//创建画刷
	m_brDisable.DeleteObject();
	m_brDisable.CreateSolidBrush(RGB(255,255,255));

	//更新界面
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//重画消息
VOID CSkinEditEx::OnNcPaint()
{
	//状态变量
	bool bDisable=(IsWindowEnabled()==FALSE);
	COLORREF crColorBK=(bDisable==false)?m_crEnableBK:m_crDisableBK;
	COLORREF crColorBorad=(bDisable==false)?m_crEnableBorad:m_crDisableBorad;

	//绘画边框
	CClientDC ClientDC(this);
	DrawControlView(&ClientDC,crColorBorad,crColorBK);

	return;
}

//绘画控件
HBRUSH CSkinEditEx::CtlColor(CDC * pDC, UINT nCtlColor)
{
	//设置环境
	if (IsWindowEnabled()==FALSE)
	{
		//禁用状态
		pDC->SetBkColor((m_crDisableBK==CLR_INVALID)?RGB(125,125,125):m_crDisableBK);
		pDC->SetTextColor((m_crDisableText==CLR_INVALID)?RGB(50,50,50):m_crDisableText);
		return (m_brDisable.GetSafeHandle()==NULL)?m_SkinAttribute.m_brDisbale:m_brDisable;
	}
	else
	{
		//启用状态
		pDC->SetBkColor((m_crEnableBK==CLR_INVALID)?RGB(255,255,255):m_crEnableBK);
		pDC->SetTextColor((m_crEnableText==CLR_INVALID)?RGB(10,10,10):m_crEnableText);
		return (m_brEnable.GetSafeHandle()==NULL)?m_SkinAttribute.m_brFocus:m_brEnable;
	}

	return NULL;
}



//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CMyComboBox, CComboBox)

CMyComboBox::CMyComboBox()
{
	m_bFocus=0x00;
	CImageHandle ImageHandleBottom(&CSkinEdit::m_SkinAttribute.m_ImageBottom);
	iWidth=CSkinEdit::m_SkinAttribute.m_ImageBottom.GetWidth()/3;
}

CMyComboBox::~CMyComboBox()
{
}


BEGIN_MESSAGE_MAP(CMyComboBox, CComboBox)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()



void CMyComboBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
// 	CImageHandle ImageHandleBottom(&CSkinEdit::m_SkinAttribute.m_ImageBottom);
// 	int iWidth=CSkinEdit::m_SkinAttribute.m_ImageBottom.GetWidth()/5;
// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (point.x>=rect.right-iWidth)
	{
		m_bFocus=0x01;
	}
	CComboBox::OnLButtonDown(nFlags, point);
}
void CMyComboBox::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
 	/*CImageHandle ImageHandleBottom(&CSkinEdit::m_SkinAttribute.m_ImageBottom);
 	int iWidth=CSkinEdit::m_SkinAttribute.m_ImageBottom.GetWidth()/5;*/
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (point.x>=rect.right-iWidth-1)
	{
		if (m_bFocus!=0x02)
		{
			m_bFocus=0x02;
			Invalidate(FALSE);
			TRACKMOUSEEVENT TrackMouseEvent;
			TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);
			TrackMouseEvent.dwFlags=TME_LEAVE;
			TrackMouseEvent.hwndTrack=GetSafeHwnd();
			TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
			_TrackMouseEvent(&TrackMouseEvent);
		}

	}
	else if(m_bFocus!=0)
	{
		Invalidate(FALSE);
		m_bFocus=0x00;
	}
	CComboBox::OnMouseMove(nFlags, point);
}

//鼠标离开消息
LRESULT CMyComboBox::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bFocus=0x01;
	Invalidate(FALSE);
	return 0;
}
void CMyComboBox::OnPaint()
{
	//	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CComboBox::OnPaint()
	CComboBox::OnPaint();
	CDC *pDC = GetWindowDC();
	//获取位置
	CImageHandle ImageHandleBottom(&CSkinEdit::m_SkinAttribute.m_ImageBottom);
	int iWidth=CSkinEdit::m_SkinAttribute.m_ImageBottom.GetWidth()/4,iHeight=CSkinEdit::m_SkinAttribute.m_ImageBottom.GetHeight();
//if(!CSkinEdit::m_SkinAttribute.m_ImageBottom.IsNull())
//{
//	AfxMessageBox(1);
//}
	CRect rect;
	GetWindowRect(&rect); //获得窗口矩形
	rect -= rect.TopLeft();
	CPen LinePen(PS_SOLID,1,RGB(81,197,254));
	CPen LinePen1(PS_SOLID,1,RGB(255,255,255));
	rect.right-=1;
	rect.bottom-=1;
	//pDC->SelectObject(&LinePen);
	//pDC->Rectangle( rect.right - 6,rect.bottom - 6,rect.right,rect.bottom );
// 	pDC->MoveTo(rect.left,rect.top);
// 	pDC->LineTo(rect.left,rect.bottom+1);
// 	pDC->MoveTo(rect.left,rect.top);
// 	pDC->LineTo(rect.right,rect.top);
// 	pDC->MoveTo(rect.right,rect.top);
// 	pDC->LineTo(rect.right,rect.bottom);
// 	pDC->MoveTo(rect.right,rect.bottom);
// 	pDC->LineTo(rect.left,rect.bottom);
// 	pDC->MoveTo(rect.right,rect.bottom);
// 	pDC->LineTo(rect.left,rect.bottom);
// 	pDC->SelectObject(&LinePen1);
// 	pDC->MoveTo(rect.left+1,rect.top+1);
// 	pDC->LineTo(rect.left+1,rect.bottom-1);
// 	pDC->MoveTo(rect.left+1,rect.top+1);
// 	pDC->LineTo(rect.right,rect.top+1);
// 	LinePen.DeleteObject();
// 	LinePen1.DeleteObject();

	pDC->Draw3dRect(0,0,rect.Width(),rect.Height(),RGB(81,197,254),RGB(81,197,254));

	//获取字符
	CString strString;
	GetWindowText(strString);

	//获取字符
	CRect rcText;
	GetClientRect(&rcText);

	//设置环境
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(GetFont());

	//绘画字符
	rcText.left+=4;
	pDC->DrawText(strString,&rcText,DT_VCENTER|DT_SINGLELINE);

	if (m_bFocus&0x01)
	{
		CSkinEdit::m_SkinAttribute.m_ImageBottom.AlphaDrawImage(pDC,rect.right-iWidth-2,2,iWidth,iHeight,iWidth*2,0,RGB(0,0,0));
	}
	else if(m_bFocus&0x02)
	{
		CSkinEdit::m_SkinAttribute.m_ImageBottom.AlphaDrawImage(pDC,rect.right-iWidth-2,2,iWidth,iHeight,iWidth*1,0,RGB(0,0,0));
	}
	else
	{	
		//AfxMessageBox(1);
		CSkinEdit::m_SkinAttribute.m_ImageBottom.AlphaDrawImage(pDC,rect.right-iWidth-2,2,iWidth,iHeight,iWidth*1,0,RGB(0,0,0));
		//CSkinEdit::m_SkinAttribute.m_ImageBottom.AlphaDrawImage(pDC,rect.right-iWidth-2,2,iWidth,iHeight,0,0,RGB(0,0,0));
	//	CSkinEdit::m_SkinAttribute.m_ImageBottom.AlphaDrawImage(pDC,0,0,iWidth,iHeight,0,0,RGB(255,255,255));
	}
	ReleaseDC( pDC );
}

void CMyComboBox::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{

	return;

}



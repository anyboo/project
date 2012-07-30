// GradientProgressCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "GradientProgressCtrl.h"
#include "MemDC1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGradientProgressCtrl

CGradientProgressCtrl::CGradientProgressCtrl()
{
	m_nLower=0;
	m_nUpper=100;
	m_nCurrentPosition=0;
	m_nStep=10;

	//Initial Color of show
	m_clrStart=	COLORREF(RGB(255,0,0));
	m_clrEnd=	COLORREF(RGB(0,0,255));
	m_clrBkGround=::GetSysColor(COLOR_3DFACE);
	m_clrText=COLORREF(RGB(255,255,255));
	//Not show Word
	m_bShowPercent=FALSE;
}

CGradientProgressCtrl::~CGradientProgressCtrl()
{
}


BEGIN_MESSAGE_MAP(CGradientProgressCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CGradientProgressCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGradientProgressCtrl message handlers

void CGradientProgressCtrl::SetRange(int nLower,int nUpper)
{
	//This Function is to Set Range of the progress 
	m_nLower=nLower;
	m_nUpper=nUpper;
	m_nCurrentPosition=nLower;
	
	CProgressCtrl::SetRange(nLower,nUpper);
}

int CGradientProgressCtrl::SetStep(int nStep)
{
	m_nStep=nStep;
	return (CProgressCtrl::SetStep(nStep));
}

BOOL CGradientProgressCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;//CProgressCtrl::OnEraseBkgnd(pDC);
}

void CGradientProgressCtrl::DrawGradient(CPaintDC *pDC, const RECT &rectClient, const int &nMaxWidth)
{
	RECT	rectFill;		//显示区域
	float	fStep;			//每一步的幅宽
	CBrush	brush;			//显示的颜色画刷
	CMemDC1	memDC(pDC);	

	int		r,g,b;
	float	rStep,gStep,bStep;
	//得到不同颜色并相减，返回颜色之间的最大差值
	r=(GetRValue(m_clrEnd)-GetRValue(m_clrStart));
	g=(GetGValue(m_clrEnd)-GetGValue(m_clrStart));
	b=(GetBValue(m_clrEnd)-GetBValue(m_clrStart));
	//使进程条显示的总数 等于最大的颜色差值
	int		nSteps=max(abs(r),max(abs(g),abs(b)));
	//确定每一颜色填充多大的矩形区域
	fStep=(float)rectClient.right/(float)nSteps;
	//设置每一颜色填充的步数
	rStep=r/(float)nSteps;
	gStep=g/(float)nSteps;
	bStep=b/(float)nSteps;

	r=GetRValue(m_clrStart);
	g=GetGValue(m_clrStart);
	b=GetBValue(m_clrStart);
	//绘制颜色渐变的进程条
	for(int iOnBand=0;iOnBand<nSteps;iOnBand++)
	{
		::SetRect(&rectFill,
			//以下为填充矩形区域的左上角x,y和右下角x,y
							(int)(iOnBand*fStep),
							0,
							(int)((iOnBand+1)*fStep),
							rectClient.bottom);
		
		VERIFY(brush.CreateSolidBrush(RGB(r+rStep*iOnBand,g+gStep*iOnBand,b+bStep*iOnBand)));
		memDC.FillRect(&rectFill,&brush);
		VERIFY(brush.DeleteObject());
		//在结束绘制之前，使用背景色填充乘下的的客户区域
		if(rectFill.right>nMaxWidth)
		{
			::SetRect(&rectFill,rectFill.right,0,rectClient.right,rectClient.bottom);
			VERIFY(brush.CreateSolidBrush(m_clrBkGround));
			memDC.FillRect(&rectFill,&brush);
			VERIFY(brush.DeleteObject());
			return;
		}
	}
}

void CGradientProgressCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(m_nCurrentPosition<=m_nLower||m_nCurrentPosition>=m_nUpper)
	{
		CRect rect;
		GetClientRect(rect);
		CBrush brush;
		brush.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
		dc.FillRect(&rect,&brush);
		VERIFY(brush.DeleteObject());
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient);
	float maxWidth((float)m_nCurrentPosition/(float)m_nUpper*(float)rectClient.right);
/*	//绘制
	DrawGradient(&dc,rectClient,(int)maxWidth);
	//显示进程条进度文字
	if(m_bShowPercent)
	{
		CString percent;
		percent.Format("%d%%",(int)(100*(float)m_nCurrentPosition/m_nUpper));
		dc.SetTextColor(m_clrText);
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(percent,&rectClient,DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	}
*/
	// Do not call CProgressCtrl::OnPaint() for painting messages
	/////////////////////////////
    
	CPen * pWhitePen=CPen::FromHandle((HPEN)GetStockObject(WHITE_PEN));
//	CPen * pBlackPen=CPen::FromHandle((HPEN)GetStockObject(BLACK_PEN));

	dc.SelectObject(pWhitePen);
	//创建笔画
	CPen BorderPen1(PS_SOLID,1, RGB( 0xff, 0xcf, 0xa7) );
	CPen * pOldPen1= dc.SelectObject(&BorderPen1);

	CBrush brush,*oldbrush;
	CBrush brush1;

	brush1.CreateSolidBrush(RGB( 0xff, 0xcf, 0xa7));

	oldbrush=dc.SelectObject(&brush1);
	dc.Rectangle( 0, 0, rectClient.right, rectClient.bottom);
	dc.SelectObject(oldbrush);
	dc.SelectObject(pOldPen1);
	BorderPen1.DeleteObject();

	brush.CreateSolidBrush(RGB(255,255,255));		// 白色
	oldbrush=dc.SelectObject(&brush);
	dc.Ellipse( 0, 0, rectClient.bottom, rectClient.bottom);
	dc.Ellipse( rectClient.right - rectClient.Height(), 0, rectClient.right, rectClient.bottom);
	dc.Rectangle( rectClient.Height()/2, 0, rectClient.right - rectClient.Height()/2, rectClient.bottom);

	dc.SelectObject(oldbrush);

//	CBrush brush2;
//--------------------------------------

	if ( maxWidth > rectClient.Height())
	{
	//	CPen * pRedPen=CPen::FromHandle((HPEN)GetStockObject(DKGRAY_BRUSH));
		//创建笔画
		CPen BorderPen(PS_SOLID,1, RGB(128, 0, 0) );
		CPen * pOldPen= dc.SelectObject(&BorderPen);

//		dc.SelectObject(pRedPen);
		CBrush brush2;
		brush2.CreateSolidBrush(RGB(128, 0, 0));
		oldbrush=dc.SelectObject(&brush2);
		dc.Ellipse( 0, 0, rectClient.bottom, rectClient.bottom);
		dc.Rectangle( rectClient.Height()/2, 0, maxWidth, rectClient.bottom);
		dc.SelectObject(oldbrush);
		
		//清理资源
		VERIFY(brush2.DeleteObject());
		dc.SelectObject(pOldPen);
		BorderPen.DeleteObject();
	}
//--------------------------------------
	VERIFY(brush1.DeleteObject());
	VERIFY(brush.DeleteObject());

}

int CGradientProgressCtrl::SetPos(int nPos)
{
	//Set the Position of the Progress
	m_nCurrentPosition=nPos;
	return (CProgressCtrl::SetPos(nPos));
}

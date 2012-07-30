#include "Stdafx.h"
#include "resource.h"
#include "BankStorage.h"
/////////////////////////////////////////////////////////////////////////

//定时器I D
#define IDI_CHARMVALUE_UPDATE_VIEW		1								//更新界面
#define TIME_CHARMVALUE_UPDATE_VIEW		200								//更新界面


//////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CBankStorage1, CSkinDialogEx1)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK,OnBnClickedOk)   //确定
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CBankStorage1::CBankStorage1(const BYTE bGameAction) : CSkinDialogEx1(IDD_BANK_STORAGE)
{
	

	
}

//析构函数
CBankStorage1::~CBankStorage1()
{
	
}

//控件绑定
void CBankStorage1::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	
}


//初始化函数
BOOL CBankStorage1::OnInitDialog()
{
	__super::OnInitDialog();

	UpdateData(FALSE);

	SetWindowText(TEXT("银行"));
    
    //广告控件
	m_BrowerAD.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,100,NULL);
	m_BrowerAD.Navigate(TEXT("http://www.7x78.com/GameInterFace/Insure/bankLogin.asp"),NULL,NULL,NULL,NULL);

	//广告位置
    int nXExcursion=GetXExcursionPos();
	int nYExcursion=GetYExcursionPos();
	m_BrowerAD.MoveWindow(nXExcursion+8,nYExcursion,529,300);

	CRect rcFrame;
	GetWindowRect(&rcFrame);
	MoveWindow(&rcFrame,FALSE);
    
	SetWindowPos(NULL,0,0,547,349,SWP_NOMOVE|SWP_NOZORDER|SWP_NOCOPYBITS);
	m_ImageBuffer.Destroy();
    m_ImageBuffer.Create(547,349,16);
    
	HDWP hDwp=BeginDeferWindowPos(0);
    
	DeferWindowPos(hDwp,m_btOK,NULL,510,5,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);
    
	EndDeferWindowPos(hDwp);
    
	return TRUE;
}
void CBankStorage1::Initm_BrowerAD()
{

	m_BrowerAD.Navigate(TEXT("http://www.7x78.com/GameInterFace/Insure/bankLogin.asp"),NULL,NULL,NULL,NULL);

}
//重画消息
void CBankStorage1::OnPaint()
{
	//创建 DC
	CPaintDC dc(this);
    
	//获取区域
	CRect rcClient,ClipRect;
	dc.GetClipBox(&ClipRect);
	GetClientRect(&rcClient);
    
	CDC * pDC=CDC::FromHandle(m_ImageBuffer.GetDC());

	pDC->SetBkMode(TRANSPARENT);
	m_ImageKuang.AlphaDrawImage(pDC,rcClient.left,rcClient.top,RGB(255,0,255));
	m_ImageBuffer.BitBlt(dc,rcClient.left,rcClient.top,547,349,0,0);
	
    m_ImageBuffer.ReleaseDC();
	
}

//提取按钮
void CBankStorage1::OnBnClickedOk()
{
	
   //关闭窗口
   DestroyWindow();
   return;
}

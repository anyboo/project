// TableOption.cpp : implementation file
//

#include "stdafx.h"
#include "TableOption.h"


// CTableOption dialog


BEGIN_MESSAGE_MAP(CTableOption, CSkinDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_EDIT, OnBnClickedEdit)
	ON_BN_CLICKED(IDC_EDIT1, OnBnClickedEdit1)
END_MESSAGE_MAP()

//IMPLEMENT_DYNAMIC(CTableOption, CSkinDialogEx)
CTableOption::CTableOption(CWnd* pParent /*=NULL*/, CServerOption * pServerOption)
	: CSkinDialogEx(CTableOption::IDD, pParent)
{
	m_pServerOption=pServerOption;
	//颜色设置
	 m_crFriendColor = g_GlobalOption.m_crFriendColor;
	 m_crOwnColor =g_GlobalOption.m_crOwnColor;
}

CTableOption::~CTableOption()
{
}

void CTableOption::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NUM, m_IpNum);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCLE, m_btCancle);
	DDX_Control(pDX, IDC_EDIT, m_btEdit);
	DDX_Control(pDX, IDC_EDIT1, m_btEdit1);
}

BOOL CTableOption::OnInitDialog()
{
	__super::OnInitDialog();

	SetWindowText("设置");
	//初始化IP列表
	m_IpNum.Clear();
	m_IpNum.InsertString(0,"1");
	m_IpNum.InsertString(1,"2");
	m_IpNum.InsertString(2,"3");
	m_IpNum.SetCurSel(0);
	
	//呼叫模式
	if(g_GlobalOption.m_CallMode==enCallMode_All)
		SetButtonSelected(IDC_TABOPTION_RDO_CALLALL, true);
	else if (g_GlobalOption.m_CallMode==enCallMode_Friend)
		SetButtonSelected(IDC_TABOPTION_RDO_CALLFRIEDN, true);
	else
		SetButtonSelected(IDC_TABOPTION_RDO_CALLNONE, true);

	//邀请模式
	if(g_GlobalOption.m_InviteMode == enInviteMode_All)
		SetButtonSelected(IDC_TABOPTION_RDO_YQ_ALL, true);
	else if(g_GlobalOption.m_InviteMode == enInviteMode_Friend)
		SetButtonSelected(IDC_TABOPTION_RDO_YQ_FRIEND, true);
	else 
		SetButtonSelected(IDC_TABOPTION_RDO_YQ_NONE, true);

	//桌子密码
	if (g_GlobalOption.m_bPassword==true)
	{
		SetButtonSelected(IDC_BRING_TABLE_PASSWORD,true);
	}
	else
		SetButtonSelected(IDC_BRING_TABLE_PASSWORD,false);

	GetDlgItem(IDC_TABLE_PASSWORD)->SetWindowText(g_GlobalOption.m_szPassword);

	if (g_GlobalOption.m_bSameIp==true)
	{
		SetButtonSelected(IDC_LIMIT_IP,true);
		if(g_GlobalOption.m_wSameIpNum==1)
			m_IpNum.SetCurSel(0);
		else if(g_GlobalOption.m_wSameIpNum==2)
			m_IpNum.SetCurSel(1);
		else if(g_GlobalOption.m_wSameIpNum==3)
			m_IpNum.SetCurSel(2);
	}
	else
	{
		SetButtonSelected(IDC_LIMIT_IP,false);
		m_IpNum.SetCurSel(0);
	}

	return TRUE;
}



void CTableOption:: OnBnClickedEdit()
{
	//设置颜色
	CColorDialog ColorDialog(m_crOwnColor,CC_FULLOPEN);
	if (ColorDialog.DoModal()==IDOK) m_crOwnColor = ColorDialog.GetColor();
	GetDlgItem(IDC_OWNER)->Invalidate();

	return ;
}
void CTableOption:: OnBnClickedEdit1()
{
	//设置颜色
	CColorDialog ColorDialog(m_crFriendColor,CC_FULLOPEN);
	if (ColorDialog.DoModal()==IDOK) m_crFriendColor = ColorDialog.GetColor();
	GetDlgItem(IDC_FRIEND)->Invalidate();

	return ;
}

//是否选中
bool CTableOption::IsButtonSelected(UINT uButtonID)
{
	return ((CButton *)GetDlgItem(uButtonID))->GetCheck()==BST_CHECKED;
}

//选中按钮
void CTableOption::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	CButton * pButton=(CButton *)GetDlgItem(uButtonID);
	if (bSelected) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);
	return;
}

HBRUSH CTableOption::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	if(pWnd->GetDlgCtrlID()==IDC_OWNER)
	{
		HBRUSH hr = CreateSolidBrush(m_crOwnColor);

		return hr;  

	}
	else if(pWnd->GetDlgCtrlID()==IDC_FRIEND)
	{
		HBRUSH hr = CreateSolidBrush(m_crFriendColor);

		return hr;  

	}

	if(pWnd->GetDlgCtrlID()==IDC_GROUP1||pWnd->GetDlgCtrlID()==IDC_GROUP2||pWnd->GetDlgCtrlID()==IDC_GROUP3||pWnd->GetDlgCtrlID()==IDC_GROUP4)
	{
		pDC->SetBkColor(CSkinDialog::m_SkinAttribute.m_crBackGround);
		pDC->SetTextColor(CSkinDialog::m_SkinAttribute.m_crControlTXColor);

		HBRUSH hr = CreateSolidBrush(CSkinDialog::m_SkinAttribute.m_crBackGround);

		return hr;

	}
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(CSkinDialog::m_SkinAttribute.m_crBackGround);
			pDC->SetTextColor(CSkinDialog::m_SkinAttribute.m_crControlTXColor);
			return CSkinDialog::m_SkinAttribute.m_brBackGround;
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}
void CTableOption::OnOK()
{
	SaveSetting();
	__super::OnOK();
}
void CTableOption::SaveSetting()
{

	//呼叫模式
	if(IsButtonSelected(IDC_TABOPTION_RDO_CALLALL))
		g_GlobalOption.m_CallMode=enCallMode_All;
	else if (IsButtonSelected(IDC_TABOPTION_RDO_CALLFRIEDN))
		g_GlobalOption.m_CallMode=enCallMode_Friend;
	else
		g_GlobalOption.m_CallMode=enCallMode_None;

	//邀请模式
	if(IsButtonSelected(IDC_TABOPTION_RDO_YQ_ALL))
		g_GlobalOption.m_InviteMode=enInviteMode_All;
	else if (IsButtonSelected(IDC_TABOPTION_RDO_YQ_FRIEND))
		g_GlobalOption.m_InviteMode=enInviteMode_Friend;
	else
		g_GlobalOption.m_InviteMode=enInviteMode_None;

	//桌子密码
	if (IsButtonSelected(IDC_BRING_TABLE_PASSWORD))
	{
		g_GlobalOption.m_bPassword=true;
		m_pServerOption->m_bPassword = true;

		CString strPass;
		GetDlgItemText(IDC_TABLE_PASSWORD,strPass);
		lstrcpyn(g_GlobalOption.m_szPassword,strPass,CountArray(g_GlobalOption.m_szPassword));
		lstrcpyn(m_pServerOption->m_szPassword ,strPass,CountArray(m_pServerOption->m_szPassword));
	}
	else
	{
		g_GlobalOption.m_bPassword=false;
		m_pServerOption->m_bPassword=false;
		ZeroMemory(g_GlobalOption.m_szPassword,sizeof(g_GlobalOption.m_szPassword));
		ZeroMemory(m_pServerOption->m_szPassword,sizeof(m_pServerOption->m_szPassword));
	}


	//相同IP
	if (IsButtonSelected(IDC_LIMIT_IP))
	{
		g_GlobalOption.m_bSameIp=true;
		g_GlobalOption.m_wSameIpNum = GetDlgItemInt(IDC_COMBO_NUM);
	}
	else
	{
		g_GlobalOption.m_bSameIp=false;
		g_GlobalOption.m_wSameIpNum = 0;
	}

	//颜色设置
	g_GlobalOption.m_crFriendColor = m_crFriendColor;
	g_GlobalOption.m_crOwnColor = m_crOwnColor;
}

BOOL CTableOption::OnEraseBkgnd( CDC * pDC )
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////


// CTableOption message handlers

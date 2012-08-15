// AdminControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AdminControlDlg.h"
#include ".\admincontroldlg.h"


// CAdminControlDlg 对话框

IMPLEMENT_DYNAMIC(CAdminControlDlg, CDialog)
CAdminControlDlg::CAdminControlDlg(CWnd* pParent /*=NULL*/)
: CDialog(CAdminControlDlg::IDD, pParent)
{
}

CAdminControlDlg::~CAdminControlDlg()
{
}

void CAdminControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CAdminControlDlg::ReSetAdminWnd()
{
	((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(m_cbControlStyle==CS_BANKER_WIN?1:0);
	((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(m_cbControlStyle==CS_BANKER_LOSE?1:0);

	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->SetCurSel(m_cbExcuteTimes>0?(m_cbExcuteTimes-1):-1);
}

//申请结果
bool CAdminControlDlg::ReqResult(const void * pBuffer)
{
	const CMD_S_CommandResult*pResult=(CMD_S_CommandResult*)pBuffer;
	CString str;
	switch(pResult->cbAckType)
	{
	case ACK_SET_WIN_AREA:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str="输赢控制命令已经接受！";
				SetTimer(1,1000,0);
			}
			else
			{
				str.Format("请求失败!");
				m_cbExcuteTimes=0;
				m_cbControlStyle=0;
				SetTimer(1,1000,0);
			}
			break;
		}
	case ACK_RESET_CONTROL:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				m_cbControlStyle=0;
				m_cbExcuteTimes=0;
				ReSetAdminWnd();
				str="重置请求已接受!";
				SetTimer(1,1000,0);
			}
			break;
		}
	case ACK_PRINT_SYN:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str="服务器同步请求已接受!";

				tagAdminReq*pAdminReq=(tagAdminReq*)pResult->cbExtendData;
				m_cbExcuteTimes=pAdminReq->m_cbExcuteTimes;
				m_cbControlStyle=pAdminReq->m_cbControlStyle;
				ReSetAdminWnd();
				SetTimer(1,1000,0);
			}
			else
			{
				str.Format("请求失败!");
			}
			break;
		}

	default: break;
	}

	SetWindowText(str);
	return true;
}


BEGIN_MESSAGE_MAP(CAdminControlDlg, CDialog)
	ON_BN_CLICKED(IDC_BT_RESET,OnReSet)
	ON_BN_CLICKED(IDC_BT_CANCEL,OnCancel)
	ON_BN_CLICKED(IDC_BT_EXCUTE,OnExcute)
	ON_BN_CLICKED(IDC_BT_CURSET,OnRefresh)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CAdminControlDlg 消息处理程序
void CAdminControlDlg::OnReSet()
{
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType=RQ_RESET_CONTROL;
	AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}
void CAdminControlDlg::OnRefresh()
{
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType=RQ_PRINT_SYN;
	AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}
void CAdminControlDlg::OnExcute()
{
	BYTE cbIndex=0;
	bool bFlags=false;
	m_cbControlStyle=0;
	m_cbExcuteTimes=0;

	//控制自己
	if(((CButton*)GetDlgItem(IDC_RADIO_WIN))->GetCheck())
	{
		m_cbControlStyle=CS_BANKER_WIN;
		bFlags=true;
	}
	else
	{
		if(((CButton*)GetDlgItem(IDC_RADIO_LOSE))->GetCheck())
		{
			m_cbControlStyle=CS_BANKER_LOSE;
			bFlags=true;
		}
	}

	m_cbExcuteTimes=(BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel()+1;
	//获取执行次数
	if(m_cbExcuteTimes<=0)
	{
		bFlags=false;
	}


	if(bFlags) //参数有效
	{
		CMD_C_AdminReq adminReq;
		adminReq.cbReqType=RQ_SET_WIN_AREA;
		tagAdminReq*pAdminReq=(tagAdminReq*)adminReq.cbExtendData;
		pAdminReq->m_cbExcuteTimes=m_cbExcuteTimes;
		pAdminReq->m_cbControlStyle=m_cbControlStyle;
		AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
	}
	else
	{
		AfxMessageBox("请选择受控次数以及受控方式!");
		OnRefresh();
	}
}
void CAdminControlDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

BOOL CAdminControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	// TODO:  在此添加额外的初始化
	SetDlgItemText(IDC_BT_RESET,"取消控制");	
	SetDlgItemText(IDC_BT_CURSET,"当前设置");
	SetDlgItemText(IDC_BT_EXCUTE,"执行");	
	SetDlgItemText(IDC_BT_CANCEL,"取消");	
	SetDlgItemText(IDC_RADIO_WIN,"自己赢");	
	SetDlgItemText(IDC_RADIO_LOSE,"自己输");
	SetDlgItemText(IDC_STATIC_TIMES	,"控制次数");	
	SetDlgItemText(IDC_STATIC_CHOICE,"控制选项");	
	SetDlgItemText(IDC_STATIC_NOTICE,"说明");
	SetDlgItemText(IDC_STATIC_NOTICE1,"1.区域输赢控制比游戏库存控制策略优先。");	

	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("1");
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("2");
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("3");
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("4");
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("5");

	((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAdminControlDlg::PrintCurStatus()
{
	CString str;
	switch(m_cbControlStyle)
	{
	case CS_BANKER_WIN:
		{
			str.Format("自己受控,受控方式:赢,执行次数:%d",m_cbExcuteTimes);break;
		}
	case CS_BANKER_LOSE:
		{
			str.Format("自己受控,受控方式:输,执行次数:%d",m_cbExcuteTimes);break;
		}
	default: str="非控制状态";break;
	}	

	SetWindowText(str);
}

VOID CAdminControlDlg::OnAllowControl(bool bEnable)
{
	GetDlgItem(IDC_BT_EXCUTE)->EnableWindow(bEnable);
}
void CAdminControlDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer(1);
	PrintCurStatus();
	CDialog::OnTimer(nIDEvent);
}

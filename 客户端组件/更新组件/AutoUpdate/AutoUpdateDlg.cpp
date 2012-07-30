// AutoUpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoUpdate.h"
#include "AutoUpdateDlg.h"
#include "TransparentHelper.h"
#include "..\MD5Checksum\MD5Checksum.h"

#import <msxml3.dll>
using namespace MSXML2;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define HTTP_PROTOCOL	_T("http://")

// CAutoUpdateDlg dialog


CAutoUpdateDlg::CAutoUpdateDlg(CWnd* pParent): CDialog(IDD_AUTOUPDATE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_ulTotalLength = 0L;
	m_ulCurentLenght = 0L;
	m_pMemDC = NULL;
	m_pOldBitmap = NULL;
	m_pBitmapMem = NULL;
	m_bmBack.LoadBitmap(IDB_DIALOG_BACK);
}

CAutoUpdateDlg::~CAutoUpdateDlg()
{
	if (m_pMemDC!=NULL)
	{
		m_pMemDC->SelectObject(m_pOldBitmap);
		delete m_pMemDC;
		delete m_pBitmapMem;
	}
}

void CAutoUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CURRENT_PROGRESS, m_Progress1);
	DDX_Control(pDX, IDC_TOTAL_PROGRESS, m_Progress2);
	DDX_Control(pDX, IDC_STATUS_TEXT, m_StatusText);
	DDX_Control(pDX, IDC_COUNT_TEXT, m_CountText);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	
}

BEGIN_MESSAGE_MAP(CAutoUpdateDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_MESSAGE( WM_TRANSPARENT_BK, &CAutoUpdateDlg::OnTransaprentBk)
	ON_BN_CLICKED(IDCANCEL, &CAutoUpdateDlg::OnBnClickedCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CAutoUpdateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_StatusText.SetTextColor(RGB(255,255,255));
	m_CountText.SetTextColor(RGB(255,255,255));

	HWND hNotifyWindow = NULL;
	TCHAR szModuleFile[MAX_PATH]={0};
	CString strSiteURL;
	CString strTempDir;

	//查找窗口

	hNotifyWindow = ::FindWindow(_T("pjqpGamePlaza"), NULL);
//#ifndef _DEBUG
//	if (hNotifyWindow==NULL)
//	{
//		PostMessage(WM_CLOSE);
//		return TRUE;
//	}
//#endif

	GetModuleFileName(AfxGetInstanceHandle(), szModuleFile, MAX_PATH);
	PathRemoveFileSpec(szModuleFile);
	PathAddBackslash(szModuleFile);
	m_strCurrentDir = szModuleFile;

	//读取配置参数
	strSiteURL = AfxGetApp()->GetProfileString(_T("Download"), _T("site1"), _T("192.168.0.222/down"));
	strTempDir = AfxGetApp()->GetProfileString(_T("Setup"), _T("TempDir"), _T("Update"));

	//设置URL路径
	if( strSiteURL.Left(7) != HTTP_PROTOCOL )
		strSiteURL = HTTP_PROTOCOL + strSiteURL;

	strSiteURL += _T("/update/");
	m_DownloadMgr.Initialize((IDownloadSink*)this, strSiteURL, m_strCurrentDir+strTempDir, m_strCurrentDir);

	BITMAP bm;
	m_bmBack.GetBitmap(&bm);
	SetWindowPos(NULL,0,0,bm.bmWidth,bm.bmHeight,SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOMOVE|SWP_NOZORDER);

	//检查版本
	if (!CheckVersionUpdate(strSiteURL + _T("update.xml")))
	{
		ASSERT(hNotifyWindow!=NULL);
		::SendMessage(hNotifyWindow, WM_USER+102, 0, 0);
		PostMessage(WM_CLOSE);
		return TRUE;	
	}
	else
	{
		::SendMessage(hNotifyWindow, WM_USER+102, 1, 0);
	}

	//设置范围
	m_Progress2.SetRange32(0, m_ulTotalLength/1024);

	// Send WM_NCCALCSIZE messages to the window
	m_Progress1.SetWindowPos(NULL, 0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	m_Progress2.SetWindowPos(NULL, 0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	ShowWindow(SW_SHOW);
	BringWindowToTop();
	UpdateWindow();

	//启动下载
	m_DownloadMgr.Start();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAutoUpdateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rtClient;
		GetClientRect(&rtClient);

		if ( m_pMemDC )
		{
			dc.BitBlt( 0,0,rtClient.Width(), rtClient.Height(), m_pMemDC, 0,0, SRCCOPY);
		}
	}
}

BOOL CAutoUpdateDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CAutoUpdateDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	BuildBkDC();
	Invalidate(FALSE);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAutoUpdateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAutoUpdateDlg::OnStatusCallback(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCTSTR pszStatusText)
{
	switch( ulStatusCode )
	{
	case DMS_SENDREQUEST:
		{
			break;
		}
	case DMS_BEGINDOWNLOAD:
		{
			m_ulCurentLenght = ulProgressMax;
			m_Progress1.SetRange32(0, ulProgressMax/1024);
			m_Progress1.SetPos(0);
			break;
		}
	case DMS_DOWNLOADDATA:
		{
			CString strText;
			CString strText2;
			strText.Format(TEXT("正在下载 %s "), pszStatusText);
			strText2.Format(TEXT("大小 %d KB  已下载 %d KB"), m_ulCurentLenght/1024, ulProgress/1024);
			m_StatusText.SetWindowText(strText);
			m_CountText.SetWindowText(strText2);

			m_Progress1.SetPos(ulProgress/1024);
			m_Progress2.SetPos(ulProgressMax/1024);
			break;
		}
	case DMS_ENDDOWNLOADDATA:
		{
			m_ulCurentLenght = 0;
			//m_Progress1.SetPos(0);
			break;
		}
	case DMS_FINISHDOWNLOAD:
		{
			m_btCancel.EnableWindow(FALSE);
			break;
		}
	case DMS_CANCELDOWNLOAD:
		{
			SendMessage(WM_CLOSE);
			break;
		}
	case DMS_ENDSETUP:
		{
			//启动程序
			TCHAR szModuleName[MAX_PATH]={0};
			GetModuleFileName(AfxGetInstanceHandle(), szModuleName, MAX_PATH);
			PathRemoveFileSpec(szModuleName);
			strcat(szModuleName, _T("\\GamePlaza.exe -noupdate"));
			WinExec(szModuleName, SW_SHOWDEFAULT);

			SendMessage(WM_CLOSE);
			break;
		}
	case DMS_ERROR:
		{
			CString strError = AfxGetApp()->GetProfileString(_T("messages"), _T("ErrorMsg"), _T("更新失败，请到www.jxhqp.com下载最新版本"));
			MessageBox(strError, _T("提示"), MB_OK|MB_ICONINFORMATION);
			SendMessage(WM_CLOSE);
			break;
		}
	}
}

BOOL CAutoUpdateDlg::CheckVersionUpdate(CString& strUpdateURL)
{
	BOOL bHasUpdate = FALSE;

	HRESULT hr = S_OK;
	IXMLHTTPRequestPtr pHttpRequest;
	IDispatchPtr pDispatch;
	MSXML2::IXMLDOMDocumentPtr pXmlDoc;
	MSXML2::IXMLDOMNodeListPtr pList;
	MSXML2::IXMLDOMElementPtr pChild;

	UINT nFileSize;
	CString strFileName, strFileVer, strMD5String;
	LONG lElementCount = 0L;

	//LPCTSTR lpUrl="www.baidu.com";
	AfxMessageBox(strUpdateURL);

	try
	{
		hr = pHttpRequest.CreateInstance(TEXT("Msxml2.XMLHTTP.3.0"));
		if( FAILED(hr) )
			_com_issue_error(hr);

		hr = pHttpRequest->open(TEXT("GET"), (_bstr_t)strUpdateURL, false);
		if( FAILED(hr) )
			_com_issue_error(hr);

		hr = pHttpRequest->send();
		if( FAILED(hr) )
			_com_issue_error(hr);

		if (pHttpRequest->Getstatus() != 200)
			throw (0);
		pDispatch = pHttpRequest->GetresponseXML();
		hr = pDispatch->QueryInterface(pXmlDoc.GetIID(), (void**)&pXmlDoc);
		if( FAILED(hr) )
			_com_issue_error(hr);

		pList = pXmlDoc->selectNodes((_bstr_t)"/manifest/filelist/file");
		pXmlDoc-> save(_bstr_t("c:\\2.txt"));

		lElementCount = pList->Getlength();
		for( LONG i = 0; i < lElementCount; i++ )
		{
			pChild = pList->Getitem(i);
			strFileName = pChild->getAttribute("filename");
			nFileSize = pChild->getAttribute("filesize");
			strFileVer = pChild->getAttribute("fileversion");
			strMD5String = pChild->getAttribute("md5");

			//如果本地文件存在则效验文件，不存在就下载这个文件
			if (PathFileExists(m_strCurrentDir+strFileName))
			{
				//效验文件,如果MD5码不相同则重新下载
				if (CMD5Checksum::GetMD5(m_strCurrentDir+strFileName) != strMD5String)
				{
					m_ulTotalLength += nFileSize;
					m_DownloadMgr.AddTask(strFileName);
					bHasUpdate = TRUE;
				}	
			}
			else
			{
				m_ulTotalLength += nFileSize;
				m_DownloadMgr.AddTask(strFileName);
				bHasUpdate = TRUE;
			}
		}

		return bHasUpdate;
	}
#ifdef _DEBUG
	catch(_com_error& e)
	{
		CString strError;
		strError.Format(_T("Error code:%d\nMessage:%s\nDescrption:%s"), (int)e.WCode(), e.ErrorMessage(), (TCHAR*)e.Description());
		MessageBox(strError, _T("提示"), MB_OK|MB_ICONWARNING);
		return FALSE;
	}
#endif
	catch(...)
	{
		return FALSE;
	}
}

void CAutoUpdateDlg::OnBnClickedCancel()
{
	if ( m_DownloadMgr.GetStatus() == DMS_FINISHDOWNLOAD || m_DownloadMgr.GetStatus() == DMS_ERROR )
	{
		DestroyWindow();
	}
	else
	{
		m_DownloadMgr.CancelTask();
		m_btCancel.EnableWindow(FALSE);
	}
}

void CAutoUpdateDlg::OnClose()
{
	DestroyWindow();
}

LRESULT CAutoUpdateDlg::OnTransaprentBk( WPARAM wParam, LPARAM lParam )
{
	HDC hdc = ( HDC)wParam;
	HWND hwnd = ( HWND)lParam;
	CTransparentHelper::OnTranparentControl( m_pMemDC->GetSafeHdc(), (WPARAM)hdc, (LPARAM)hwnd);
	return TRUE;
}

void CAutoUpdateDlg::BuildBkDC()
{
	CDC* pDC;
	CDC MemDC;
	CBitmap* pOldBitmap;
	CRect rtClient;

	pDC = GetDC();
	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = MemDC.SelectObject( &m_bmBack );

	GetClientRect(&rtClient);

	if ( m_pMemDC )
	{
		m_pMemDC->SelectObject( m_pOldBitmap);
		delete m_pBitmapMem;
		m_pBitmapMem = NULL;
		delete m_pMemDC;
		m_pMemDC = NULL;
	}

	if ( m_pMemDC == NULL )
	{
		m_pMemDC = new CDC;
		m_pMemDC->CreateCompatibleDC(pDC);

		m_pBitmapMem = new CBitmap;
		m_pBitmapMem->CreateCompatibleBitmap(pDC, rtClient.Width(), rtClient.Height());
		m_pOldBitmap = (CBitmap*)m_pMemDC->SelectObject( m_pBitmapMem);
	}

	m_pMemDC->BitBlt( 0,0, rtClient.Width(), rtClient.Height(), &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject( pOldBitmap );
	ReleaseDC(pDC);

	//when the parent dialog's background is rebuild, notify the child which has an transparent tag.
	CTransparentHelper::NotifyTransparentChild( GetSafeHwnd());
}
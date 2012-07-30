// UpdateBuilderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UpdateBuilder.h"
#include "UpdateBuilderDlg.h"
#include "..\MD5Checksum\MD5Checksum.h"

//#include "..\zlib-1.2.3\zlib.h"
//#import "c:\winnt\system32\msxml.dll" 

#import <msxml3.dll>
using namespace MSXML2;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define MAX_BUFFER	40960
//#define CHUNK			16384


// CUpdateBuilderDlg dialog

CUpdateBuilderDlg::CUpdateBuilderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateBuilderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//邦定控件
void CUpdateBuilderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_LIST, m_FileListCtrl);
}

BEGIN_MESSAGE_MAP(CUpdateBuilderDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_LOAD_FILES, &CUpdateBuilderDlg::OnBnClickedLoadFiles)
	ON_BN_CLICKED(IDC_REMOVE_FILE, &CUpdateBuilderDlg::OnBnClickedRemoveFile)
	ON_BN_CLICKED(IDC_SAVE_ITEM, &CUpdateBuilderDlg::OnBnClickedSaveItem)
	ON_BN_CLICKED(IDC_LOAD_ITEM, &CUpdateBuilderDlg::OnBnClickedLoadItem)
	ON_BN_CLICKED(IDC_UPDATE_ITEM, &CUpdateBuilderDlg::OnBnClickedUpdateItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CUpdateBuilderDlg message handlers

//初始化
BOOL CUpdateBuilderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_FileListCtrl.SetExtendedStyle(m_FileListCtrl.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_FileListCtrl.InsertColumn(0, _T("文件名"), LVCFMT_LEFT, 120);
	m_FileListCtrl.InsertColumn(1, _T("文件大小"), LVCFMT_LEFT, 80);
	m_FileListCtrl.InsertColumn(2, _T("文件版本"), LVCFMT_LEFT, 80);
	m_FileListCtrl.InsertColumn(3, _T("MD5编码"), LVCFMT_LEFT, 220);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

//文件尺寸
ULONG CUpdateBuilderDlg::GetFileSize(LPCTSTR pszFileName)
{
	ASSERT(pszFileName!=NULL);
	if( pszFileName == NULL )
		return 0L;

	CFile file;
	ULONG nFileSize = 0L;
	if( !file.Open(pszFileName, CFile::modeRead) )
		return 0L;

	nFileSize=(ULONG)file.GetLength();
	file.Close();

	return nFileSize;
}

//是否存在
BOOL CUpdateBuilderDlg::FindExist(LPCTSTR pszFileName)
{
	ASSERT(pszFileName!=NULL);
	if( pszFileName == NULL )
		return FALSE;

	LVFINDINFO fi;
	ZeroMemory(&fi, sizeof(fi));
	fi.flags = LVFI_STRING;
	fi.psz = pszFileName;
	if( m_FileListCtrl.FindItem(&fi) != -1 )
		return TRUE;

	return FALSE;
}

//保存项目
BOOL CUpdateBuilderDlg::SaveToXml(LPCTSTR pszFilePath)
{
	ASSERT(pszFilePath!=NULL);
	if (pszFilePath == NULL)
		return FALSE;

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMDocumentPtr	pXmlDoc;
	MSXML2::IXMLDOMElementPtr pRoot, pFileList, pFileNode;
	_variant_t vFileName, vFileSize, vFileVersion, vMD5Key;

	try
	{
		hr = pXmlDoc.CreateInstance(__uuidof(MSXML2::DOMDocument30));
		if( FAILED( hr ) ) 
			_com_issue_error(hr);

		pXmlDoc->PutpreserveWhiteSpace(VARIANT_TRUE);
		pXmlDoc->Putasync(VARIANT_FALSE);

		pXmlDoc->appendChild(pXmlDoc->createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'"));

		pRoot = pXmlDoc->createElement("manifest");
		pRoot->setAttribute("version", "1.0");
		pRoot->setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema");
		pXmlDoc->appendChild(pRoot);

		pFileList = pXmlDoc->createElement("filelist");
		pFileList->setAttribute("checksummode", "default");
		
		for( int i = 0; i < m_FileListCtrl.GetItemCount(); i++ )
		{
			vFileName = m_FileListCtrl.GetItemText(i, 0);
			vFILEVERSION 2,0,0,0
			vMD5Key = m_FileListCtrl.GetItemText(i, 3);
			vFileSize = (ULONG)m_FileListCtrl.GetItemData(i);

			pFileNode = pXmlDoc->createElement("file");
			pFileNode->setAttribute("filename", vFileName);
			pFileNode->setAttribute("filesize", vFileSize);
			pFileNode->setAttribute("fileversion", vFileVersion);
			pFileNode->setAttribute("md5", vMD5Key);
			pFileList->appendChild(pFileNode);
		}

		pRoot->appendChild(pFileList);

		hr = pXmlDoc->save(pszFilePath);
		if( FAILED( hr ) )
			_com_issue_error(hr);

		return TRUE;
		
	}
	catch(_com_error& e)
	{
		CString strError;
		strError.Format(_T("Error code:%d\nError Message:%s\nError Description:%s"), 
			(int)e.WCode(), e.ErrorMessage(), (char*)e.Description());
		MessageBox(strError, _T("错误"),MB_OK|MB_ICONSTOP);
	}

	return FALSE;
}

//文件版本
CString CUpdateBuilderDlg::GetFileVersion(LPCTSTR pszFileName)
{
	ASSERT(pszFileName!=NULL);
	if (pszFileName==NULL)
		return _T("");

	BYTE buffer[4096];
	DWORD dwFileHandle = NULL;
	DWORD dwLenght = 0L;
	UINT nQuerySize = 0;
	VS_FIXEDFILEINFO* pFixedFileInfo = NULL;
	TCHAR szText[32] = {0};

	ZeroMemory(buffer, sizeof(buffer));
	dwLenght = GetFileVersionInfoSize(pszFileName, &dwFileHandle);
	ASSERT(dwLenght <= 4096);
	if (dwLenght == 0 || dwLenght > 4096)
		return _T("");

	if( !GetFileVersionInfo(pszFileName, dwFileHandle, dwLenght, buffer) )
		return _T("");

	if( !VerQueryValue(buffer, _T("\\"), (void**)&pFixedFileInfo, &nQuerySize) )
		return _T("");

	sprintf(szText, _T("%d.%d.%d.%d"), 
		HIWORD(pFixedFileInfo->dwFileVersionMS), 
		LOWORD(pFixedFileInfo->dwFileVersionMS),
		HIWORD(pFixedFileInfo->dwFileVersionLS), 
		LOWORD(pFixedFileInfo->dwFileVersionLS));

	return szText;
}

//插入文件
BOOL CUpdateBuilderDlg::InsertFileItem(LPCTSTR pszFileName)
{
	ASSERT(pszFileName!=NULL);
	if (pszFileName==NULL)
		return FALSE;

	LPCTSTR pszFileTitle = PathFindFileName(pszFileName);
	if (FindExist(pszFileTitle) )
		return FALSE;

	char buff[32];
	int nItem = m_FileListCtrl.InsertItem(0, PathFindFileName(pszFileTitle));
	ULONG ulFileSize = GetFileSize(pszFileName);
	_ltoa(ulFileSize, buff, 10);
	m_FileListCtrl.SetItemText(nItem, 1, buff);
	m_FileListCtrl.SetItemText(nItem, 2, GetFileVersion(pszFileName));
	m_FileListCtrl.SetItemText(nItem, 3, CMD5Checksum::GetMD5(pszFileName));
	m_FileListCtrl.SetItemData(nItem, ulFileSize);

	return TRUE;
}

//更新文件
BOOL CUpdateBuilderDlg::UpdateFileItem(int nItem)
{
	ASSERT(nItem != -1);
	if (nItem == -1)
		return FALSE;

	CString strFileName;
	ULONG ulFileSize;
	char buff[32];

	strFileName = m_FileListCtrl.GetItemText(nItem, 0);
	if (!PathFileExists(strFileName))
	{
		return FALSE;
	}

	ulFileSize = GetFileSize(strFileName);
	_ltoa(ulFileSize, buff, 10);
	m_FileListCtrl.SetItemText(nItem, 1, buff);
	m_FileListCtrl.SetItemText(nItem, 2, GetFileVersion(strFileName));
	m_FileListCtrl.SetItemText(nItem, 3, CMD5Checksum::GetMD5(strFileName));
	m_FileListCtrl.SetItemData(nItem, ulFileSize);

	return TRUE;
}

//绘制消息
void CUpdateBuilderDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//查询图标
HCURSOR CUpdateBuilderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//文件拖放
void CUpdateBuilderDlg::OnDropFiles(HDROP hDropInfo)
{
	UINT nFileCount;
	TCHAR szFileName[MAX_PATH];

	nFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	for (UINT i = 0; i < nFileCount; i++)
	{
		DragQueryFile(hDropInfo, i, szFileName, MAX_PATH);
		InsertFileItem(szFileName);
		UpdateWindow();
		WaitMessage();
	}

	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}

//装入文件
void CUpdateBuilderDlg::OnBnClickedLoadFiles()
{
	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFileNames[MAX_PATH*32];
	ZeroMemory(szFileNames, sizeof(szFileNames));

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetSafeHwnd();
	ofn.lpstrFile = szFileNames;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFileNames);
	ofn.lpstrFilter = "All Files(*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT|OFN_EXPLORER;

	// Display the Open dialog box. 
	if (GetOpenFileName(&ofn)==TRUE) 
	{
		CWaitCursor wait;
		TCHAR* pFileTitle = szFileNames + ofn.nFileOffset;
		while( *pFileTitle )
		{
			InsertFileItem(pFileTitle);
			UpdateWindow();
			WaitMessage();
			pFileTitle += lstrlen(pFileTitle) + 1;
		}

		GetDlgItem(IDC_UPDATE_ITEM)->EnableWindow(TRUE);
	}

}

//移除文件
void CUpdateBuilderDlg::OnBnClickedRemoveFile()
{
	POSITION pos = m_FileListCtrl.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		int nItem = m_FileListCtrl.GetNextSelectedItem(pos);
		if (nItem != -1)
		{
			m_FileListCtrl.DeleteItem(nItem);
		}
	}
}

//保存项目
void CUpdateBuilderDlg::OnBnClickedSaveItem()
{
	CFileDialog dlg(FALSE, _T("*.xml"), _T("update.xml"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("XML File(*.xml)|*.xml||"));

	if( dlg.DoModal() == IDOK )
	{
		SaveToXml(dlg.GetPathName());
	}
}

//装入项目
void CUpdateBuilderDlg::OnBnClickedLoadItem()
{
	CFileDialog dlg(TRUE, _T("*.xml"), _T("update.xml"), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("XML File(*.xml)|*.xml||"));
	if (dlg.DoModal() == IDOK)
	{
		HRESULT hr = S_OK;
		MSXML2::IXMLDOMDocumentPtr	pXmlDoc;
		MSXML2::IXMLDOMNodeListPtr pNodeList;
		MSXML2::IXMLDOMElementPtr pElement;

		try
		{
			hr = pXmlDoc.CreateInstance(__uuidof(MSXML2::DOMDocument30));
			if( FAILED( hr ) ) 
				_com_issue_error(hr);

			pXmlDoc->load(CComVariant(dlg.GetPathName()));
			
			pNodeList = pXmlDoc->selectNodes("/manifest/filelist/file");
			
			m_FileListCtrl.DeleteAllItems();
			LONG lItemCount = pNodeList->Getlength();
			int nItem; 

			for (LONG lIndex = 0; lIndex < lItemCount; lIndex++)
			{
				pElement = pNodeList->Getitem(lIndex);
				nItem = m_FileListCtrl.InsertItem(0, CString(pElement->getAttribute("filename")));
				m_FileListCtrl.SetItemText(nItem, 1, CString(pElement->getAttribute("filesize")));
				m_FileListCtrl.SetItemText(nItem, 2, CString(pElement->getAttribute("fileversion")));
				m_FileListCtrl.SetItemText(nItem, 3, CString(pElement->getAttribute("md5")));
				m_FileListCtrl.SetItemData(nItem, (ULONG)pElement->getAttribute("filesize"));
			}

			SetWindowText(dlg.GetPathName());
			GetDlgItem(IDC_UPDATE_ITEM)->EnableWindow(TRUE);
		}
		catch(_com_error& e)
		{
			CString strError;
			strError.Format(_T("Error code:%d\nError Message:%s\nError Description:%s"), 
				(int)e.WCode(), e.ErrorMessage(), (char*)e.Description());
			MessageBox(strError, _T("错误"),MB_OK|MB_ICONSTOP);
		}
	}
}

//更新项目
void CUpdateBuilderDlg::OnBnClickedUpdateItem()
{
	CWaitCursor wait;
	for (int i = 0; i < m_FileListCtrl.GetItemCount(); i++)
	{
		if (!UpdateFileItem(i))
		{
			goto fail;
		}
		UpdateWindow();
		WaitMessage();
	}

	return;

fail:
	CString strError;
	TCHAR szCurrentPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurrentPath);
	strError.Format(_T("在以下'%s'路径中没有找到这些文件。"), szCurrentPath);
	MessageBox(strError, _T("错误提示"),MB_OK|MB_ICONERROR);
}


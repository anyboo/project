// UpdateBuilderDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CUpdateBuilderDlg dialog
class CUpdateBuilderDlg : public CDialog
{
	// Implementation
protected:
	HICON		m_hIcon;
	CListCtrl	m_FileListCtrl;

// Construction
public:
	CUpdateBuilderDlg(CWnd* pParent = NULL);

// Dialog Data
	enum { IDD = IDD_UPGRADEBUILDER_DIALOG };

protected:
	//邦定控件
	virtual void DoDataExchange(CDataExchange* pDX);
	//初始化
	virtual BOOL OnInitDialog();

	//助手函数
protected:
	//文件尺寸
	ULONG GetFileSize(LPCTSTR pszFileName);
	//是否存在
	BOOL FindExist(LPCTSTR pszFileName);
	//保存项目
	BOOL SaveToXml(LPCTSTR pszFilePath);
	//文件版本
	CString GetFileVersion(LPCTSTR pszFileName);
	//插入文件
	BOOL InsertFileItem(LPCTSTR pszFileName);
	//更新文件
	BOOL UpdateFileItem(int nItem);

	//消息处理
protected:
	//绘制消息
	afx_msg void OnPaint();
	//查询图标
	afx_msg HCURSOR OnQueryDragIcon();
	//文件拖放
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//装入文件
	afx_msg void OnBnClickedLoadFiles();
	//移除文件
	afx_msg void OnBnClickedRemoveFile();
	//保存项目
	afx_msg void OnBnClickedSaveItem();
	//装入项目
	afx_msg void OnBnClickedLoadItem();
	//更新项目
	afx_msg void OnBnClickedUpdateItem();
	
	DECLARE_MESSAGE_MAP()
};

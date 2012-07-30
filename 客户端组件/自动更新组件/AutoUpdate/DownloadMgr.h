#ifndef __DOWNLOADMGR_H_H
#define __DOWNLOADMGR_H_H

#pragma once
#include <afxinet.h>
#include <afxtempl.h>


//////////////////////////////////////////////////////////////////////////

// Download status
#define DMS_SENDREQUEST					0x00000001L			//发送请求
#define DMS_READYDOWNLOAD				0x00000002L         //准备下载
#define DMS_BEGINDOWNLOAD				0x00000003L			//开始下载
#define DMS_DOWNLOADDATA				0x00000004L			//下载数据
#define DMS_ENDDOWNLOADDATA				0x00000005L			//结束下载
#define DMS_FINISHDOWNLOAD				0x00000006L			//完成下载
#define DMS_CANCELDOWNLOAD				0x00000007L			//取消下载
#define DMS_BEGINSETUP					0x00000008L			//开始安装
#define DMS_ENDSETUP					0x00000009L         //结束安装
#define DMS_ERROR						0x00000020L			//错误信息


///////////////////////////////////////////////////////////////////////////////
// Download task structure
typedef struct tagDownloadTask {
	DWORD	dwTaskId;
	DWORD   dwFileSize;
	BOOL	bFinishd;
	TCHAR	szRemoteFile[32];
	TCHAR	szLocalFile[32];
} DOWNLOADTASK;

///////////////////////////////////////////////////////////////////////////////
// Status callback interface
interface IDownloadSink
{
	virtual void OnStatusCallback(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCTSTR pszStatusText) = 0;
};

///////////////////////////////////////////////////////////////////////////////
// CDownloadMgr class
class CDownloadMgr
{
	//任务管理
protected:
	UINT					m_nTaskCount;			//任务计数
	ULONG					m_ulTotalProgress;		//整体进度
	UINT					m_nStatus;				//当前状态
	CArray<DOWNLOADTASK*>   m_DownloadList;			//任务列表
	
	//线程变量
protected:
	HANDLE					m_hThread;				//线程句柄
	HANDLE					m_hEvent;				//事件句柄
	BOOL					m_bCancel;				//取消任务

	//配置变量
protected:
	IDownloadSink*			m_pDownloadSink;		//回调接口
	CString					m_strDownloadURL;		//下载地址
	CString					m_strTempPath;			//存放路径
	CString					m_strSetupPath;			//安装路径
	CInternetSession*		m_pInteSession;

public:
	static CRITICAL_SECTION	m_Critical;				//临界对象

public:
	class CThreadLockHandle
	{
	public:
		CThreadLockHandle()  
		{ 
			EnterCriticalSection(&CDownloadMgr::m_Critical);
		}

		~CThreadLockHandle() 
		{ 
			LeaveCriticalSection(&CDownloadMgr::m_Critical);
		}
	};
	
public:
	//构造函数
	CDownloadMgr();
	//析构函数
	~CDownloadMgr();

	//初始下载
	BOOL Initialize(IDownloadSink* pDownloadSink, LPCTSTR pszDownloadURL, LPCTSTR pszTempPath, LPCTSTR pszSetupPath);
	//得到状态
	UINT GetStatus() const { return m_nStatus; }

	//操作函数
public:
	//添加任务
	BOOL AddTask(LPCTSTR pszRemoteFile, LPCTSTR pszLocalFile=NULL);
	//启动任务
	void Start();
	//移除任务
	void RemoveAllTask();
	//澈消任务
	void CancelTask() { m_bCancel=TRUE; }

protected:
	//线程函数
	static DWORD WINAPI ThreadProc(LPVOID lpParam);
	//回调函数
	virtual DWORD OnDownloadProc();
	//安装文件
	virtual DWORD OnSetupFilesProc();
};

#endif
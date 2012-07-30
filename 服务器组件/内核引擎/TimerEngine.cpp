#include "StdAfx.h"
#include "TimerEngine.h"
#include "TraceService.h" //

//宏定义
#define NO_TIME_LEFT						DWORD(-1)				//不响应时间

//////////////////////////////////////////////////////////////////////////

//构造函数
CTimerThread::CTimerThread(void)
{
	m_pTimerEngine = NULL;
}

//析构函数
CTimerThread::~CTimerThread(void)
{
}

//配置函数
bool CTimerThread::InitThread(CTimerEngine * pTimerEngine)
{
	if (pTimerEngine == NULL) return false;

	//设置变量
	m_pTimerEngine = pTimerEngine;

	return true;
}

//运行函数
bool CTimerThread::OnEventThreadRun()
{
	ASSERT(m_pTimerEngine != NULL);
	Sleep(TIMER_SPACE);
	m_pTimerEngine->OnTimerThreadSink();
	return true;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CTimerEngine::CTimerEngine(void)
{
	m_bService = false;
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEFT;
}

//析构函数
CTimerEngine::~CTimerEngine(void)
{
	//停止服务
	ConcludeService();

	//清理内存
	tagTimerItem * pTimerItem = NULL;
	for (INT_PTR i = 0; i < m_TimerItemFree.GetCount(); i++)
	{
		pTimerItem = m_TimerItemFree[i];
		ASSERT(pTimerItem != NULL);
		SafeDelete(pTimerItem);
	}
	for (INT_PTR i = 0; i < m_TimerItemActive.GetCount(); i++)
	{
		pTimerItem = m_TimerItemActive[i];
		ASSERT(pTimerItem != NULL);
		SafeDelete(pTimerItem);
	}
	m_TimerItemFree.RemoveAll();
	m_TimerItemActive.RemoveAll();

	return;
}

//接口查询
void * __cdecl CTimerEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITimerEngine, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITimerEngine, Guid, dwQueryVer);
	return NULL;
}

//设置定时器
bool __cdecl CTimerEngine::SetTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter)
{
	//锁定资源
	CThreadLock lock(m_CriticalSection);//

	//效验参数
	ASSERT(dwRepeat > 0L);
	if (dwRepeat == 0) return false;
	dwElapse=(dwElapse+TIMER_SPACE-1)/TIMER_SPACE*TIMER_SPACE;

	//查找定时器
	bool bTimerExist = false;
	tagTimerItem * pTimerItem = NULL;
	for (INT_PTR i = 0; i < m_TimerItemActive.GetCount(); i++)
	{
		pTimerItem = m_TimerItemActive[i];
		ASSERT(pTimerItem != NULL);
		if (pTimerItem->wTimerID == dwTimerID)
		{
			bTimerExist = true;
			break;
		}
	}

	//创建定时器
	if (bTimerExist == false)
	{
		INT_PTR nFreeCount = m_TimerItemFree.GetCount();
		if (nFreeCount > 0)
		{
			pTimerItem = m_TimerItemFree[nFreeCount-1];
			ASSERT(pTimerItem != NULL);
			m_TimerItemFree.RemoveAt(nFreeCount - 1);
		}
		else
		{
			try
			{
				pTimerItem = new tagTimerItem;
				ASSERT(pTimerItem != NULL);
				if (pTimerItem == NULL) return false;
			}
			catch (...)
			{
				return false;
			}
		}
	}

	//设置参数
	ASSERT(pTimerItem != NULL);
	pTimerItem->wTimerID = dwTimerID;
	pTimerItem->wBindParam = dwBindParameter;
	pTimerItem->dwElapse = dwElapse;
	pTimerItem->dwRepeatTimes = dwRepeat;
	pTimerItem->dwTimeLeave=dwElapse+m_dwTimePass;

	//激活定时器
	m_dwTimeLeave=__min(m_dwTimeLeave,dwElapse);
	if (bTimerExist == false) m_TimerItemActive.Add(pTimerItem);

	return true;
}

//删除定时器
bool __cdecl CTimerEngine::KillTimer(DWORD dwTimerID)
{
	//锁定资源
	CThreadLock lock(m_CriticalSection);//

	//查找定时器
	tagTimerItem * pTimerItem = NULL;
	for (INT_PTR i = 0; i < m_TimerItemActive.GetCount(); i++)
	{
		pTimerItem = m_TimerItemActive[i];
		ASSERT(pTimerItem != NULL);
		if (pTimerItem->wTimerID == dwTimerID)
		{
			m_TimerItemActive.RemoveAt(i);
			m_TimerItemFree.Add(pTimerItem);
			if (m_TimerItemActive.GetCount()==0) 
			{
				m_dwTimePass=0L;
				m_dwTimeLeave=NO_TIME_LEFT;
			}
			return true;;
		}
	}

	return false;
}

//删除定时器
bool __cdecl CTimerEngine::KillAllTimer()
{
	//锁定资源
	CThreadLock lock(m_CriticalSection);//

	//删除定时器
	m_TimerItemFree.Append(m_TimerItemActive);
	m_TimerItemActive.RemoveAll();

	//设置变量
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEFT;

	return true;
}

//开始服务
bool __cdecl CTimerEngine::StartService()
{
	//效验状态
	if (m_bService == true)
	{
		CTraceService::TraceString(TEXT("定时器引擎重复启动，启动操作忽略"), TraceLevel_Warning);
		return true;
	}

	//设置变量
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEFT;

	//设置变量
	if (m_TimerThread.InitThread(this) == false)
	{
		CTraceService::TraceString(TEXT("定时器引擎线程服务初始化失败"), TraceLevel_Exception);
		return false;
	}

	//启动服务
	if (m_TimerThread.StartThread() == false)
	{
		CTraceService::TraceString(TEXT("定时器引擎线程服务启动失败"), TraceLevel_Exception);
		return false;
	}

	SetThreadPriority(m_TimerThread.GetThreadHandle(), REALTIME_PRIORITY_CLASS);


	//设置变量
	m_bService = true;

	return true;
}

//停止服务
bool __cdecl CTimerEngine::ConcludeService()
{
	//设置变量
	m_bService = false;

	//停止线程
	m_TimerThread.ConcludeThread(INFINITE);

	//设置变量
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEFT;
	m_TimerItemFree.Append(m_TimerItemActive);
	m_TimerItemActive.RemoveAll();

	//停止请求队列
	m_QueueServiceEvent.SetQueueServiceSink(NULL);

	return true;
}

//设置接口
bool __cdecl CTimerEngine::SetTimerEngineEvent(IUnknownEx * pIUnknownEx)
{
	//效验参数
	ASSERT(pIUnknownEx != NULL);
	ASSERT(m_bService == false);
	if (m_bService == true) return false;
	if (pIUnknownEx == NULL) return false;

	 return m_QueueServiceEvent.SetQueueServiceSink(pIUnknownEx);
}

//定时器通知
void CTimerEngine::OnTimerThreadSink()
{
	//缓冲锁定
	CThreadLock lock(m_CriticalSection);//

	//倒计时间
	if (m_dwTimeLeave==NO_TIME_LEFT) 
	{
		ASSERT(m_TimerItemActive.GetCount()==0);
		return;
	}

	//减少时间
	ASSERT(m_dwTimeLeave>=TIMER_SPACE);
	m_dwTimeLeave-=TIMER_SPACE;
	m_dwTimePass+=TIMER_SPACE;

	//查询定时器
	if (m_dwTimeLeave==0)
	{
		bool bKillTimer=false;
		tagTimerItem * pTimerItem=NULL;
		DWORD dwTimeLeave=NO_TIME_LEFT;
		for (INT_PTR i=0;i<m_TimerItemActive.GetCount();)
		{
			//效验参数
			pTimerItem=m_TimerItemActive[i];
			ASSERT(pTimerItem!=NULL);
			ASSERT(pTimerItem->dwTimeLeave>=m_dwTimePass);

			//定时器处理
			bKillTimer=false;
			pTimerItem->dwTimeLeave-=m_dwTimePass;
			if (pTimerItem->dwTimeLeave==0L)
			{
				//发送通知
				m_QueueServiceEvent.PostTimerEvent(pTimerItem->wTimerID, pTimerItem->wBindParam);

				//设置次数
				if (pTimerItem->dwRepeatTimes!=TIMES_INFINITY)
				{
					ASSERT(pTimerItem->dwRepeatTimes>0);
					if (pTimerItem->dwRepeatTimes==1L)
					{
						bKillTimer=true;
						m_TimerItemActive.RemoveAt(i);
						m_TimerItemFree.Add(pTimerItem);
					}
					else 
					{
						pTimerItem->dwRepeatTimes--;
					}
				}

				//设置时间
				if (bKillTimer==false) pTimerItem->dwTimeLeave=pTimerItem->dwElapse;
			}

			//增加索引
			if (bKillTimer==false) 
			{
				i++;
				dwTimeLeave=__min(dwTimeLeave,pTimerItem->dwTimeLeave);
				ASSERT(dwTimeLeave%TIMER_SPACE==0);
			}
		}

		//设置响应
		m_dwTimePass=0L;
		m_dwTimeLeave=dwTimeLeave;
	}

}

//////////////////////////////////////////////////////////////////////////

//建立对象函数
extern "C" __declspec(dllexport) void * __cdecl CreateTimerEngine(const GUID & Guid, DWORD dwInterfaceVer)
{
	//建立对象
	CTimerEngine * pTimerEngine = NULL;
	try
	{
		pTimerEngine = new CTimerEngine();
		if (pTimerEngine == NULL) throw TEXT("创建失败");
		void * pObject = pTimerEngine->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("接口查询失败");
		return pObject;
	}
	catch (...) {}

	//清理对象
	SafeDelete(pTimerEngine);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

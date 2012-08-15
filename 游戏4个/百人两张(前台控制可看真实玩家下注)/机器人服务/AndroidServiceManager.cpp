#include "StdAfx.h"
#include "AndroidServiceManager.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidService::CAndroidService()
{
}

//析构函数
CAndroidService::~CAndroidService()
{
}

//接口查询
void * __cdecl CAndroidService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidService,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidService,Guid,dwQueryVer);
	return NULL;
}

//创建机器人
VOID* __cdecl CAndroidService::CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)
{
	//变量定义
	CAndroidUserItemSink * pAndroidUserItemSink=NULL;

	try
	{
		//建立对象
		pAndroidUserItemSink=new CAndroidUserItemSink();
		if (pAndroidUserItemSink==NULL) throw TEXT("创建失败");

		//查询接口
		VOID * pObject=pAndroidUserItemSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("接口查询失败");

		return pObject;
	}
	catch (...) {}

	//删除对象
	SafeDelete(pAndroidUserItemSink);

	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//建立对象
DECLARE_CREATE_MODULE(AndroidService)

//////////////////////////////////////////////////////////////////////////

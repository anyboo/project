#ifndef ANDROID_SERVICE_HEAD_FILE
#define ANDROID_SERVICE_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//游戏服务器管理类
class CAndroidService : public IAndroidService
{
	//函数定义
public:
	//构造函数
	CAndroidService(void);
	//析构函数
	virtual ~CAndroidService(void);

	//基础接口
public:
	//释放对象
	virtual VOID __cdecl Release() { delete this; }
	//接口查询
	virtual VOID * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//功能接口
public:
	//创建机器人
	virtual VOID* __cdecl CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer);
};

//////////////////////////////////////////////////////////////////////////

#endif

#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//牌型
#define	CT_ERROR					0									//错误类型
#define	CT_POINT					1									//点数类型
#define	CT_SPECIAL_11				2									//特殊类型
#define	CT_SPECIAL_10				3									//特殊类型
#define	CT_SPECIAL_9				4									//特殊类型
#define	CT_SPECIAL_8				5									//特殊类型
#define	CT_SPECIAL_7				6									//特殊类型
#define	CT_SPECIAL_6				7									//特殊类型
#define	CT_SPECIAL_5				8									//特殊类型
#define	CT_SPECIAL_4				9									//特殊类型
#define	CT_SPECIAL_3				10									//特殊类型
#define	CT_SPECIAL_2				11									//特殊类型
#define	CT_SPECIAL_1				12									//特殊类型

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//排序类型
#define	ST_VALUE					1									//数值排序
#define	ST_LOGIC					2									//逻辑排序

//扑克数目
#define CARD_COUNT					32									//扑克数目
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////	
//调试函数
static void MyMsgBox(LPCSTR lpszFormat, ...)
{
	va_list args;
	int		nBuf;
	TCHAR	szBuffer[512];

	va_start(args, lpszFormat);
	nBuf = _vsntprintf(szBuffer, sizeof(szBuffer)-sizeof(TCHAR), lpszFormat, args);
#ifdef _DEBUG
	AfxMessageBox(szBuffer);
#else
	TCHAR	szFormat[512];
	_stprintf(szFormat, _T("[JJ]: %s"), szBuffer);
	OutputDebugString(szFormat);
#endif

	va_end(args);
}
extern TCHAR g_RoomName[128];
static bool MyDebug(LPCSTR lpszFormat, ...)
{
	va_list args;
	int		nBuf;
	TCHAR	szBuffer[512];
	TCHAR	szFormat[512];

	va_start(args, lpszFormat);
	nBuf = _vsntprintf(szBuffer, sizeof(szBuffer)-sizeof(TCHAR), lpszFormat, args);
	_stprintf(szFormat, _T(" %s"), szBuffer);
	OutputDebugString(szFormat);

	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	_snprintf(szBuffer, 512, "[%02d%02d:%02d%02d%02d,%d]\t%s", SystemTime.wMonth, SystemTime.wDay, 
		SystemTime.wHour,SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds, szFormat);


	try
	{
		//设置文件名
		TCHAR szPath[MAX_PATH]=TEXT("");
		TCHAR szFileName[MAX_PATH];
		GetCurrentDirectory(sizeof(szPath),szPath);
		_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\百人两张-%s.txt"),szPath, g_RoomName);

		//打开文件
		CFile FileRecord;
		if (FileRecord.Open(szFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
		{
			FileRecord.SeekToEnd();
			FileRecord.Write(szBuffer,lstrlen(szBuffer));
			FileRecord.Write(TEXT("\r\n"),lstrlen(TEXT("\r\n")));

			FileRecord.Close();
		}
	}
	catch(...){}

	va_end(args);


	return true;
}
//////////////////////////////////////////////////////////////////////////

//游戏逻辑
class CGameLogic
{
	//变量定义
private:
	static const BYTE				m_cbCardListData[CARD_COUNT];		//扑克定义

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return (cbCardData&LOGIC_MASK_COLOR)>>4; }

	//控制函数
public:
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//排列扑克
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);

	//逻辑函数
public:
	//获取牌点
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	//获取牌型
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//大小比较
	int CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount);
	//逻辑大小
	BYTE GetCardLogicValue(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////

#endif

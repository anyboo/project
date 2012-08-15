#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS					2									//最少时间

//游戏时间
#define TIME_USER_CALL_BANKER		2									//叫庄定时器
#define TIME_USER_START_GAME		3									//开始定时器
#define TIME_USER_ADD_SCORE			3									//下注定时器
#define TIME_USER_OPEN_CARD			12									//摊牌定时器

//游戏时间
#define IDI_START_GAME			(IDI_ANDROID_ITEM_SINK+0)			//开始时间
#define IDI_CALL_BANKER			(IDI_ANDROID_ITEM_SINK+1)			//叫庄时间
#define IDI_USER_ADD_SCORE	(IDI_ANDROID_ITEM_SINK+2)			//下注时间
#define IDI_OPEN_CARD		(IDI_ANDROID_ITEM_SINK+3)			//开牌时间
#define IDI_USER_LEFT		(IDI_ANDROID_ITEM_SINK+4)			//用户推出


//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_lTurnMaxScore = 0;
	ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

	//接口变量
	m_pIAndroidUserItem=NULL;

	return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
void * __cdecl CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool __cdecl CAndroidUserItemSink::InitUserItemSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//重置接口
bool __cdecl CAndroidUserItemSink::RepositUserItemSink()
{
	m_lTurnMaxScore = 0;
	ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

	return true;
}

//时间消息
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_USER_LEFT:
			m_pIAndroidUserItem->SendMainUser(SUB_GR_USER_LEFT_GAME_REQ, NULL, 0);
			return true;
	case IDI_START_GAME:		//开始定时器
		{

			//发送准备
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			return true;
		}
	case IDI_CALL_BANKER:		//叫庄定时
		{
			//设置变量
			CMD_C_CallBanker CallBanker;
			CallBanker.bBanker = rand()%2==0?0:1;

			//发送信息
			m_pIAndroidUserItem->SendSocketData(SUB_C_CALL_BANKER,&CallBanker,sizeof(CallBanker));

			return true;
		}
	case IDI_USER_ADD_SCORE:	//加注定时
		{
			//可下注筹码
			LONGLONG lUserMaxScore[GAME_PLAYER];
			ZeroMemory(lUserMaxScore,sizeof(lUserMaxScore));
			LONGLONG lTemp=m_lTurnMaxScore;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(i>0)lTemp/=2;
				lUserMaxScore[i]=__max(lTemp,1L);
			}

			////发送消息
			//CMD_C_AddScore AddScore;
			//
			//int countinfor = rand()%100;
			//if (countinfor > 100)
			//{
			//	AddScore.lScore=lUserMaxScore[0];
			//}
			//else if (countinfor > 85)
			//{
			//	AddScore.lScore=lUserMaxScore[1];
			//}			
			////else if (countinfor > 60)
			////{
			////	AddScore.lScore=lUserMaxScore[2];
			////}
			////else
			////{
			////	AddScore.lScore=lUserMaxScore[3];
			////}

			//m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
						//发送消息
			CMD_C_AddScore AddScore;
			AddScore.lScore=lUserMaxScore[rand()%2];
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			return true;	
		}
	case IDI_OPEN_CARD:			//开牌定时
		{
			//发送消息
		   COLORREF Color;
			CMD_C_OxCard OxCard;
			OxCard.bOX=(m_GameLogic.GetCardType(m_HandCardData,MAX_COUNT)>0)?TRUE:FALSE;
			m_pIAndroidUserItem->SendSocketData(SUB_C_OPEN_CARD,&OxCard,sizeof(OxCard));

			return true;	
		}

	}

	return false;
}

//游戏消息
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_CALL_BANKER:	//用户叫庄
		{
			//消息处理
			return OnSubCallBanker(pData,wDataSize);
		}
	case SUB_S_GAME_START:	//游戏开始
		{
			//消息处理
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_ADD_SCORE:	//用户下注
		{
			//消息处理
			return OnSubAddScore(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:	//发牌消息
		{
			//消息处理
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_OPEN_CARD:	//用户摊牌
		{
			//消息处理
			return OnSubOpenCard(pData,wDataSize);
		}
	case SUB_S_PLAYER_EXIT:	//用户强退
		{
			//消息处理
			return OnSubPlayerExit(pData,wDataSize);
		}
	case SUB_S_GAME_END:	//游戏结束
		{
			//消息处理
			return OnSubGameEnd(pData,wDataSize);
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//游戏消息
bool __cdecl CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool __cdecl CAndroidUserItemSink::OnEventGameScene(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_FREE:		//空闲状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			//////////////////////////////////////////////////////////////////////////
			memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));
			//lstrcpyn(m_szRoomName, "对杀牛牛房间", 128);
			//////////////////////////////////////////////////////////////////////////
			ReadConfigInformation(m_szRoomName);

			m_iPlayCount = 0;

			//开始时间
			UINT nElapse=rand()%(2)+TIME_LESS;
			if(rand()%60==0)
				nElapse+=20;

			//获取对家状态
			LONGLONG lUserScore = pStatusFree->lOtherScore;
			//////////////////////////////////////////////////////////////////////////
			//真人和机器人相差太大，就不开始
			if (lUserScore > 0)
			{
				LONGLONG lAndroid=m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()->lScore;
				WORD iSub = lAndroid/lUserScore;
				if(iSub > 10)
				{
					if(rand()%20!=0)
					{
						m_pIAndroidUserItem->SetGameTimer(IDI_USER_LEFT,nElapse+2);
						return true;
					}
					else
						m_iPlayCount = 3 + rand()%5;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

			return true;
		}
	case GS_TK_CALL:	// 叫庄状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusCall)) return false;
			CMD_S_StatusCall * pStatusCall=(CMD_S_StatusCall *)pData;

			//始叫用户
			if(pStatusCall->wCallBanker==m_pIAndroidUserItem->GetChairID())
			{
				//叫庄时间
				UINT nElapse=rand()%(2)+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER,nElapse);
			}

			return true;
		}
	case GS_TK_SCORE:	//下注状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;
			CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pData;

			//设置变量
			m_lTurnMaxScore=pStatusScore->lTurnMaxScore;
			WORD wMeChairId = m_pIAndroidUserItem->GetChairID();

			//设置筹码
			if (pStatusScore->lTurnMaxScore>0L && pStatusScore->lTableScore[wMeChairId]==0L)
			{
				//下注时间
				UINT nElapse=rand()%(3)+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
			}

			return true;
		}
	case GS_TK_PLAYING:	//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//设置变量
			m_lTurnMaxScore=pStatusPlay->lTurnMaxScore;
			WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
			CopyMemory(m_HandCardData,pStatusPlay->cbHandCardData[wMeChiarID],MAX_COUNT);

			//控件处理
			if(pStatusPlay->bOxCard[wMeChiarID]==0xff)
			{
				//开牌时间
				UINT nElapse=rand()%(2)+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);
			}

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//用户进入
void __cdecl CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
void __cdecl CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
void __cdecl CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
void __cdecl CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
void __cdecl CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户叫庄
bool CAndroidUserItemSink::OnSubCallBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_CallBanker)) return false;
	CMD_S_CallBanker * pCallBanker=(CMD_S_CallBanker *)pBuffer;

	//始叫用户
	if(pCallBanker->wCallBanker==m_pIAndroidUserItem->GetChairID())
	{
		//叫庄时间
		UINT nElapse=rand()%(2)+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER,nElapse);
	}

	return true;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//设置变量
	m_lTurnMaxScore=pGameStart->lTurnMaxScore;

	//设置筹码
	if (pGameStart->lTurnMaxScore>0)
	{
		//下注时间
		UINT nElapse=rand()%(2)+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
	}

	return true;
}

//用户下注
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	return true;
}

//发牌消息
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//设置数据
	WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
	CopyMemory(m_HandCardData,pSendCard->cbCardData[wMeChiarID],sizeof(m_HandCardData));

	//开牌时间
	UINT nElapse=rand()%(2)+TIME_LESS;
	m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);

	return true;
}

//用户摊牌
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_Open_Card)) return false;
	CMD_S_Open_Card * pOpenCard=(CMD_S_Open_Card *)pBuffer;

	return true;
}

//用户强退
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_PlayerExit)) return false;
	CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//删除定时器
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
	m_pIAndroidUserItem->KillGameTimer(IDI_OPEN_CARD);
	srand(time(NULL)+m_pIAndroidUserItem->GetChairID());

	UINT nElapse=rand()%2+TIME_LESS;
	if(rand()%60 ==0)
		nElapse +=rand()%20;

	//获取对家状态
	LONGLONG lUserScore =pGameEnd->lOtherScore;

	if(rand()%30 == 0)
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_LEFT,nElapse);
		return true;
	}

	if(m_iPlayCount -- > 0)
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	//真人和机器人相差太大，就不开始
	if (lUserScore > 0)
	{
		LONGLONG lAndroid=m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()->lScore;
		WORD iSub = lAndroid/lUserScore;
		if(iSub > 10)
		{
			if(rand()%20!=0)
			{
				m_pIAndroidUserItem->SetGameTimer(IDI_USER_LEFT,nElapse+2);
				return true;
			}
			else
				m_iPlayCount = 3 + rand()%5;
		}
	}

	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

	//清理变量
	m_lTurnMaxScore = 0;
	ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

	ReadConfigInformation(m_szRoomName);

	return true;
}

//读取配置
void CAndroidUserItemSink::ReadConfigInformation(TCHAR szRoomName[])
{
	//设置文件名
	TCHAR szPath[MAX_PATH] = TEXT("");
	TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	TCHAR OutBuf[255] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	_snprintf(szConfigFileName, sizeof(szConfigFileName), _T("%s\\%s"), szPath, TEXT("OxConfig.ini"));


	//机器人存取款信息
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotMaxMoney"), _T("10000000"), OutBuf, 255, szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotMaxMoney);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotMinMoney"), _T("100"), OutBuf, 255, szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotMinMoney);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotStorageMoneyMin"), _T("100"), OutBuf, 255, szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotStorageMoneyMin);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotStorageMoneyMax"), _T("100"), OutBuf, 255, szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotStorageMoneyMax);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotGETMoneyMin"), _T("100"), OutBuf, 255, szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotGETMoneyMin);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotGETMoneyMax"), _T("100"), OutBuf, 255, szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotGETMoneyMax);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotStoreagePrecision"), _T("100000"), OutBuf, 255, szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotStoragePrecision);
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotGetPrecision"), _T("100000"), OutBuf, 255, szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotGetPrecision);

	//MyDebug(_T("机器人 读取配置 [%I64d %I64d %d %d] %d [%d %d %d] 下注 %d 下降 %d"), m_lRobotJettonLimit[0], m_lRobotJettonLimit[1],
	//	m_nRobotBetTimeLimit, m_nRobotBetTimeLimit, m_bRobotBanker, m_nRobotBankerCount, m_nRobotWaitBanker, m_nRobotApplyBanker, m_bReduceJettonLimit);
}
//////////////////////////////////////////////////////////////////////////

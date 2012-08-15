#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink
{
	//游戏变量
protected:
	WORD							m_wBankerUser;							//庄家用户
	WORD							m_wFisrtCallUser;						//始叫用户
	WORD							m_wCurrentUser;							//当前用户
	LONGLONG						m_lExitScore;							//强退分数

	//用户数据
protected:
	BYTE							m_bPlayStatus[GAME_PLAYER];				//游戏状态
	BYTE							m_bCallStatus[GAME_PLAYER];				//叫庄状态
	BYTE							m_bOxCard[GAME_PLAYER];					//牛牛数据
	LONGLONG						m_lTableScore[GAME_PLAYER];				//下注数目
	BYTE							m_bIsUserLeave[GAME_PLAYER];			//玩家离开

	//扑克变量
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//桌面扑克

	//下注信息
protected:
	LONGLONG						m_lTurnMaxScore[GAME_PLAYER];			//最大下注

	//AI变量
protected:
	CHAR							m_szGameRoomName[SERVER_LEN];
	CHAR							m_szConfigFileName[MAX_PATH];
	LONGLONG						m_lStockScore;							//总输赢分
	LONGLONG						m_lStorageDeduct;							//回扣变量
	//玩家控制						
protected:
	BYTE							m_cbExcuteTimes;								//执行次数
	BYTE							m_cbControlStyle;								//控制方式
	WORD							m_wAdminUser;							//赢的ID

	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame						* m_pITableFrame;						//框架接口
	const tagGameServiceOption		* m_pGameServiceOption;					//配置参数

	//属性变量
protected:
	static const WORD				m_wPlayerCount;							//游戏人数
	static const enStartMode		m_GameStartMode;						//开始模式

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID __cdecl Release() { }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual void __cdecl RepositTableFrameSink();

	//信息接口
public:
	//开始模式
	virtual enStartMode __cdecl GetGameStartMode();
	//游戏状态
	virtual bool __cdecl IsUserPlaying(WORD wChairID);

	//游戏事件
public:
	//游戏开始
	virtual bool __cdecl OnEventGameStart();
	//游戏结束
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//事件接口
public:
	//定时器事件
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//游戏消息处理
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//游戏事件
protected:
	//叫庄事件
	bool OnUserCallBanker(WORD wChairID, BYTE bBanker);
	//加注事件
	bool OnUserAddScore(WORD wChairID, LONGLONG lScore);
	//摊牌事件
	bool OnUserOpenCard(WORD wChairID, BYTE bOx);
	//管理员命令
	bool OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer);

	//功能函数
protected:
	void AnalyseAndroidCard();
	//扑克分晓
	void AnalyseUserCard();
	//扑克分晓
	void AnalyseAdminCard(WORD dwAdminChairID, bool bIsWin);
	//交换扑克
	bool ChangeCard(WORD Winer, WORD Loser);
	//对方玩家
	WORD GetOtherUserChair(WORD wUserChair);
};

//////////////////////////////////////////////////////////////////////////

#endif

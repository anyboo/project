#include "StdAfx.h"
#include "TableFrameSink.h"
#include "debuglog.h"

//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_AllReady;	//开始模式

//////////////////////////////////////////////////////////////////////////

#define	IDI_SO_OPERATE							12							//代打定时器
#define	TIME_SO_OPERATE							33333						//代打定时器

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//游戏变量	
	m_lExitScore=0;	
	m_wBankerUser=INVALID_CHAIR;
	m_wFisrtCallUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wAdminUser=INVALID_CHAIR;
	m_cbExcuteTimes=0;
	m_cbControlStyle=0;

	//用户状态
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));
	ZeroMemory(m_bCallStatus,sizeof(m_bCallStatus));
	for(BYTE i=0;i<m_wPlayerCount;i++){m_bOxCard[i]=0xff;m_bIsUserLeave[i] = FALSE;}

	//扑克变量
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//下注信息
	ZeroMemory(m_lTurnMaxScore,sizeof(m_lTurnMaxScore));

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//接口查询--检测相关信息版本
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//读取配置
	
	memcpy(m_szGameRoomName, m_pGameServiceOption->szGameRoomName, sizeof(m_szGameRoomName));

	//设置文件名
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\OxConfig.ini"),szPath);

	TCHAR OutBuf[255];
	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("StorageStart"),"0",OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lStockScore);

	return true;
}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//游戏变量
	m_lExitScore=0;	
	m_wCurrentUser=INVALID_CHAIR;
	//m_wOperaCount=0;

	//用户状态
	//ZeroMemory(m_wWinCount,sizeof(m_wWinCount));		
	//ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));
	ZeroMemory(m_bCallStatus,sizeof(m_bCallStatus));
	for(BYTE i=0;i<m_wPlayerCount;i++){m_bOxCard[i]=0xff;m_bIsUserLeave[i]= FALSE;}

	//扑克变量
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//下注信息
	ZeroMemory(m_lTurnMaxScore,sizeof(m_lTurnMaxScore));

	return;
}

//开始模式
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//游戏状态
bool __cdecl CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	ASSERT(wChairID<m_wPlayerCount && m_bPlayStatus[wChairID]==TRUE);
	if(wChairID<m_wPlayerCount && m_bPlayStatus[wChairID]==TRUE)return true;
	return false;
}

//游戏开始
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//设置状态
	m_pITableFrame->SetGameStatus(GS_TK_CALL);

	//用户状态
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//获取用户
		IServerUserItem *pIServerUser=m_pITableFrame->GetServerUserItem(i);

		if(pIServerUser==NULL)
		{
			m_bPlayStatus[i]=FALSE;
		}
		else
		{
			m_bPlayStatus[i]=TRUE;
		}
	}

	//首局随机始叫
	if(m_wFisrtCallUser==INVALID_CHAIR)
	{
		m_wFisrtCallUser=rand()%m_wPlayerCount;
	}
	else
	{
		//m_wFisrtCallUser=(m_wFisrtCallUser+1)%m_wPlayerCount;
		//////////////////////////////////////////////////////////////////////////
		//
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]==TRUE) 
			{
				if (i == m_wFisrtCallUser) 
					continue;
				else
				{
					m_wFisrtCallUser = i;
					break;
				}
			}
				
		}
		//////////////////////////////////////////////////////////////////////////
		 
	}

	//始叫用户
	while(m_bPlayStatus[m_wFisrtCallUser]!=TRUE)
	{
		m_wFisrtCallUser=(m_wFisrtCallUser+1)%m_wPlayerCount;
	}

	//当前用户
	m_wCurrentUser=m_wFisrtCallUser;

	//设置变量
	CMD_S_CallBanker CallBanker;
	CallBanker.wCallBanker=m_wCurrentUser;
	CallBanker.bFirstTimes=true;

	//发送数据
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_bPlayStatus[i]!=TRUE)continue;
		m_pITableFrame->SendTableData(i,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
	m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);

	return true;
}

//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			WORD m_wWinTimes[GAME_PLAYER],m_wWinCount[GAME_PLAYER];
			ZeroMemory(m_wWinCount,sizeof(m_wWinCount));
			ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));

			//保存扑克
			BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
			CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

			//庄家倍数
			ASSERT(m_bOxCard[m_wBankerUser]<2);
			if(m_bOxCard[m_wBankerUser]==TRUE)
				m_wWinTimes[m_wBankerUser]=m_GameLogic.GetTimes(cbUserCardData[m_wBankerUser],MAX_COUNT);
			else m_wWinTimes[m_wBankerUser]=1;

			//对比玩家
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;

				ASSERT(m_bOxCard[i]<2);

				//对比扑克
				if (m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[m_wBankerUser],MAX_COUNT,m_bOxCard[i],m_bOxCard[m_wBankerUser])) 
				{
					m_wWinCount[i]++;
					//获取倍数
					if(m_bOxCard[i]==TRUE)
						m_wWinTimes[i]=m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT);
					else m_wWinTimes[i]=1;
				}
				else
				{
					m_wWinCount[m_wBankerUser]++;
				}
			}

			//统计得分
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;

				WORD j=i;
				if(m_wWinCount[j]>0)	//闲家胜利
				{
					GameEnd.lGameScore[j]=m_lTableScore[j]*m_wWinTimes[j];
					GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameScore[j];
					m_lTableScore[j]=0;
				}
				else					//庄家胜利
				{
					GameEnd.lGameScore[j]=(-1)*m_lTableScore[j]*m_wWinTimes[m_wBankerUser];
					GameEnd.lGameScore[m_wBankerUser]+=(-1)*GameEnd.lGameScore[j];
					m_lTableScore[j]=0;
				}
			}

			//闲家强退分数	
			GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;

			//处理负数
			if(GameEnd.lGameScore[m_wBankerUser]<0L)
			{
				//庄家积分
				IServerUserItem *pIServerUser=m_pITableFrame->GetServerUserItem(m_wBankerUser);
				LONGLONG lBankerScore=pIServerUser->GetUserScore()->lScore+pIServerUser->GetUserData()->lStorageScore;

				//不够赔偿,客户要求
				if(lBankerScore+GameEnd.lGameScore[m_wBankerUser]<0L)
				{
					//玩家得分情况
					LONGLONG lBankerGetScore=lBankerScore;

					//闲家总得分
					LONGLONG lIdlerGetScore=(-1)*GameEnd.lGameScore[m_wBankerUser];

					//庄家总得分
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;
						else if(GameEnd.lGameScore[i]<0L)		
						{
							lBankerGetScore+=(-1)*GameEnd.lGameScore[i];
						}
					}

					//失分概率 1-（庄家本钱+赢闲家的钱）/∑(赢的闲家下注数*闲家倍数)
					ASSERT(lIdlerGetScore>lBankerGetScore);

					//统计得分
					LONGLONG dTemp = 0;
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(GameEnd.lGameScore[i]>0L)
						{
							dTemp = GameEnd.lGameScore[i]-GameEnd.lGameScore[i]*(lIdlerGetScore-lBankerGetScore)/lIdlerGetScore;
							GameEnd.lGameScore[i] = dTemp;
						}
					}

					//扣除总分
					GameEnd.lGameScore[m_wBankerUser]=(-1)*lBankerScore;
				}
			}

			//离开用户
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_lTableScore[i]>0)GameEnd.lGameScore[i]=-m_lTableScore[i];
			}

			//扣税变量
			WORD cbRevenue=m_pGameServiceOption->wRevenue;

			//积分税收
			for(WORD i=0;i<m_wPlayerCount;i++)
			{
				if(GameEnd.lGameScore[i]>0L)
				{
					GameEnd.lGameTax[i]=LONGLONG(LONGLONG(GameEnd.lGameScore[i])*LONGLONG(cbRevenue)/1000L);
					GameEnd.lGameScore[i]-=GameEnd.lGameTax[i];
				}
			}

			//对家积分
			IServerUserItem * pIOtherUserItem =NULL;
			for (WORD i=0;i<m_wPlayerCount;i++){
				pIOtherUserItem = m_pITableFrame->GetServerUserItem(i);
				if(pIOtherUserItem != NULL && pIOtherUserItem->IsAndroidUser() == FALSE)
				{
					GameEnd.lOtherScore = pIOtherUserItem->GetUserScore()->lScore;
					break;
				}
			}

			//发送信息
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_bPlayStatus[i]==FALSE)continue;

				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//修改积分
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_bPlayStatus[i]==FALSE)continue;
				enScoreKind nScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
				m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],GameEnd.lGameTax[i],nScoreKind);
			}

			//库存统计
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//获取用户
				IServerUserItem * pIServerUserIte=m_pITableFrame->GetServerUserItem(i);
				if ((pIServerUserIte==NULL)||(pIServerUserIte->IsAndroidUser()==false)) continue;
			}

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//用户强退
		{

			//效验参数
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount && m_bPlayStatus[wChairID]==TRUE);

			//设置状态
			//m_bPlayStatus[wChairID]=FALSE;

			//定义变量
			CMD_S_PlayerExit PlayerExit;
			PlayerExit.wPlayerID=wChairID;

			//发送信息
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==wChairID || m_bPlayStatus[i]==FALSE)continue;
				m_pITableFrame->SendTableData(i,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));

			//m_bIsUserLeave[wChairID]=TRUE;
			WORD wOtherChair=GetOtherUserChair(wChairID);

			//看是否下注
			if(m_lTableScore[wChairID]>0L || m_lTableScore[wOtherChair]>0L)
			{
				if(m_bOxCard[wChairID]==0xff)
				{
					BYTE bOx = (m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>0)?TRUE:FALSE;
					OnUserOpenCard(wChairID,bOx);
				}
				if(m_bOxCard[wOtherChair]==0xff)
				{
					BYTE bOx = (m_GameLogic.GetCardType(m_cbHandCardData[wOtherChair],MAX_COUNT)>0)?TRUE:FALSE;
					OnUserOpenCard(wOtherChair,bOx);
				}

				return TRUE;
			}
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//发送信息
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if( m_bPlayStatus[i]==FALSE)continue;
				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			m_pITableFrame->ConcludeGame();	
			return TRUE;

/*
			//效验参数
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount && m_bPlayStatus[wChairID]==TRUE);

			//设置状态
			m_bPlayStatus[wChairID]=FALSE;

			//定义变量
			CMD_S_PlayerExit PlayerExit;
			PlayerExit.wPlayerID=wChairID;

			//发送信息
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==wChairID || m_bPlayStatus[i]==FALSE)continue;
				m_pITableFrame->SendTableData(i,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));

			WORD m_wWinTimes[GAME_PLAYER];
			if (m_pITableFrame->GetGameStatus()>GS_TK_CALL)
			{
				if (wChairID==m_wBankerUser)	//庄家强退
				{
					//定义变量
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd,sizeof(GameEnd));
					ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));

					BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
					CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

					//得分倍数
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;
						m_wWinTimes[i]=(m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING)?(1):(m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT));
					}

					//统计得分 已下或没下
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;
						GameEnd.lGameScore[i]=m_lTableScore[i]*m_wWinTimes[i];
						GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameScore[i];
						m_lTableScore[i]=0;
					}

					//闲家强退分数 
					GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;

					//处理负数
					if(GameEnd.lGameScore[m_wBankerUser]<0L)
					{
						//庄家积分
						IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wBankerUser);
						LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore+pIServerUserItem->GetUserData()->lStorageScore;

						//不够赔偿,客户要求
						if(lBankerScore+GameEnd.lGameScore[m_wBankerUser]<0L)
						{
							//玩家得分情况
							LONGLONG lBankerGetScore=lBankerScore;
							LONGLONG lIdlerGetScore=0;
							for (WORD i=0;i<m_wPlayerCount;i++)
							{
								if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;
								if(GameEnd.lGameScore[i]>0L)			//闲家总得分
								{
									lIdlerGetScore+=GameEnd.lGameScore[i];
								}
							}

							//失分概率 1-（庄家本钱+赢闲家的钱）/∑(赢的闲家下注数*闲家倍数)
							ASSERT(lIdlerGetScore>lBankerGetScore);

							//统计得分
							for (WORD i=0;i<m_wPlayerCount;i++)
							{
								if(GameEnd.lGameScore[i]>0L)
								{
									GameEnd.lGameScore[i]-=(GameEnd.lGameScore[i]*
										(lIdlerGetScore-lBankerGetScore)/lIdlerGetScore);
								}
							}

							//扣除总分
							GameEnd.lGameScore[m_wBankerUser]=(-1)*lBankerScore;
						}
					}

					//离开用户
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_lTableScore[i]>0)GameEnd.lGameScore[i]=-m_lTableScore[i];
					}

					//扣税变量
					WORD cbRevenue=m_pGameServiceOption->wRevenue;

					//积分税收
					for(WORD i=0;i<m_wPlayerCount;i++)
					{
						if(GameEnd.lGameScore[i]>0L)
						{
							GameEnd.lGameTax[i]=LONGLONG(LONGLONG(GameEnd.lGameScore[i])*LONGLONG(cbRevenue)/1000L);
							GameEnd.lGameScore[i]-=GameEnd.lGameTax[i];
						}
					}

					//发送信息
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;
						m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					}
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

					//修改积分
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_bPlayStatus[i]==FALSE && i!=m_wBankerUser)continue;
						enScoreKind nScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
						m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],GameEnd.lGameTax[i],nScoreKind);
					}

					//获取用户
					IServerUserItem * pIServerUserIte=m_pITableFrame->GetServerUserItem(m_wBankerUser);
			
					//结束游戏
					m_pITableFrame->ConcludeGame();
				}
				else						//闲家强退
				{
					//已经下注
					if (m_lTableScore[wChairID]>0L)
					{
						ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));

						//用户扑克
						BYTE cbUserCardData[MAX_COUNT];
						CopyMemory(cbUserCardData,m_cbHandCardData[m_wBankerUser],MAX_COUNT);

						//用户倍数
						m_wWinTimes[m_wBankerUser]=(m_pITableFrame->GetGameStatus()==GS_TK_SCORE)?(1):(m_GameLogic.GetTimes(cbUserCardData,MAX_COUNT));

						//修改积分
						LONGLONG lScore=-m_lTableScore[wChairID]*m_wWinTimes[m_wBankerUser];
						m_lExitScore+=(-1*lScore);
						m_lTableScore[wChairID]=(-1*lScore);
						m_pITableFrame->WriteUserScore(wChairID,lScore,0,enScoreKind_Lost);

						//获取用户
						IServerUserItem * pIServerUserIte=m_pITableFrame->GetServerUserItem(wChairID);
					}

					//玩家人数
					WORD wUserCount=0;
					for (WORD i=0;i<m_wPlayerCount;i++)if(m_bPlayStatus[i]==TRUE)wUserCount++;

					//结束游戏
					if(wUserCount==1)
					{
						//定义变量
						CMD_S_GameEnd GameEnd;
						ZeroMemory(&GameEnd,sizeof(GameEnd));
						ASSERT(m_lExitScore>=0L); 

						//扣税变量
						WORD cbRevenue=m_pGameServiceOption->wRevenue;

						//统计得分
						GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;
						GameEnd.lGameTax[m_wBankerUser]=LONGLONG(LONGLONG(GameEnd.lGameScore[m_wBankerUser])*LONGLONG(cbRevenue)/1000L);
						GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameTax[m_wBankerUser];

						//离开用户
						for (WORD i=0;i<m_wPlayerCount;i++)
						{
							if(m_lTableScore[i]>0)GameEnd.lGameScore[i]=-m_lTableScore[i];
						}

						//发送信息
						m_pITableFrame->SendTableData(m_wBankerUser,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
						m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

						WORD Zero=0;
						for (;Zero<m_wPlayerCount;Zero++)if(m_lTableScore[Zero]!=0)break;
						if(Zero!=m_wPlayerCount)
						{
							//修改积分
							LONGLONG lRevenue = GameEnd.lGameTax[m_wBankerUser];
							LONGLONG lScore=GameEnd.lGameScore[m_wBankerUser];
							m_pITableFrame->WriteUserScore(m_wBankerUser,lScore,lRevenue,enScoreKind_Win);

							//获取用户
							IServerUserItem * pIServerUserIte=m_pITableFrame->GetServerUserItem(wChairID);
						}

						//结束游戏
						m_pITableFrame->ConcludeGame();		
					}
					else if	(m_pITableFrame->GetGameStatus()==GS_TK_SCORE && m_lTableScore[wChairID]==0L)
					{
						OnUserAddScore(wChairID,0);
					}
					else if (m_pITableFrame->GetGameStatus()==GS_TK_PLAYING && m_bOxCard[wChairID]==0xff)
					{
						OnUserOpenCard(wChairID,0);
					}
				}
			}
			else 
			{
				//玩家人数
				WORD wUserCount=0;
				for (WORD i=0;i<m_wPlayerCount;i++)if(m_bPlayStatus[i]==TRUE)wUserCount++;

				//结束游戏
				if(wUserCount==1)
				{
					//定义变量
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd,sizeof(GameEnd));

					//发送信息
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;
						m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					}
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

					//结束游戏
					m_pITableFrame->ConcludeGame();			
				}
				else if(m_wCurrentUser==wChairID)OnUserCallBanker(wChairID,0);
			}
*/
			return true;
		}
	}

	return false;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_FREE:		//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//对家积分
			IServerUserItem * pIOtherUserItem =NULL;
			for (WORD i=0;i<m_wPlayerCount;i++){
				pIOtherUserItem = m_pITableFrame->GetServerUserItem(i);
				if(pIOtherUserItem ==NULL || pIOtherUserItem== pIServerUserItem) continue;
				if(!pIOtherUserItem->IsAndroidUser())
					StatusFree.lOtherScore = pIOtherUserItem->GetUserScore()->lScore;
				break;
			}

			//房间名称
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szGameRoomName, sizeof(StatusFree.szGameRoomName));
			//设置变量
			StatusFree.lCellScore=0L;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_TK_CALL:	//叫庄状态
		{
			//构造数据
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			//设置变量
			StatusCall.wCallBanker=m_wCurrentUser;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusCall,sizeof(StatusCall));
		}
	case GS_TK_SCORE:	//下注状态
		{
			//构造数据
			CMD_S_StatusScore StatusScore;
			memset(&StatusScore,0,sizeof(StatusScore));

			//加注信息
			StatusScore.lTurnMaxScore=m_lTurnMaxScore[wChiarID];
			StatusScore.wBankerUser=m_wBankerUser;

			//设置积分
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_bPlayStatus[i]==FALSE)continue;
				StatusScore.lTableScore[i]=m_lTableScore[i];
			}

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
		}
	case GS_TK_PLAYING:	//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//设置信息
			StatusPlay.lTurnMaxScore=m_lTurnMaxScore[wChiarID];
			StatusPlay.wBankerUser=m_wBankerUser;
			CopyMemory(StatusPlay.bOxCard,m_bOxCard,sizeof(StatusPlay.bOxCard));

			//设置扑克
			for (WORD i=0;i< m_wPlayerCount;i++)
			{
				if(m_bPlayStatus[i]==FALSE)continue;
				WORD j= i;
				StatusPlay.lTableScore[j]=m_lTableScore[j];
				CopyMemory(StatusPlay.cbHandCardData[j],m_cbHandCardData[j],MAX_COUNT);
			}

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}
	//效验错误
	ASSERT(FALSE);

	return false;
}

//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	switch(wTimerID)
	{
	case IDI_SO_OPERATE:
		{
			//删除时间
			m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

			//游戏状态
			switch( m_pITableFrame->GetGameStatus() )
			{
			case GS_TK_CALL:			//用户叫庄
				{
					OnUserCallBanker(m_wCurrentUser, 0);
					break;
				}
			case GS_TK_SCORE:			//下注操作
				{
					for(WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_lTableScore[i]>0L || m_bPlayStatus[i]==FALSE || i==m_wBankerUser)continue;
						OnUserAddScore(i,m_lTurnMaxScore[i]/8);
					}

					break;
				}
			case GS_TK_PLAYING:			//用户开牌
				{
					for(WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_bOxCard[i]<2 || m_bPlayStatus[i]==FALSE)continue;
						OnUserOpenCard(i, 0);
					}

					break;
				}
			default:
				{
					break;
				}
			}

			if(m_pITableFrame->GetGameStatus()!=GS_TK_FREE)
				m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);
			return true;
		}
	}

	return false;
}

//游戏消息处理 
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	bool bResult=false;
	switch (wSubCmdID)
	{
	case SUB_C_CALL_BANKER:			//用户叫庄
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_CallBanker));
			if (wDataSize!=sizeof(CMD_C_CallBanker)) return false;

			//变量定义
			CMD_C_CallBanker * pCallBanker=(CMD_C_CallBanker *)pDataBuffer;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//状态判断
			ASSERT(IsUserPlaying(pUserData->wChairID));
			if (!IsUserPlaying(pUserData->wChairID)) return true;

			//消息处理
			bResult=OnUserCallBanker(pUserData->wChairID,pCallBanker->bBanker);
			break;
		}
	case SUB_C_ADD_SCORE:			//用户加注
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) return false;

			//变量定义
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pDataBuffer;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//状态判断
			ASSERT(IsUserPlaying(pUserData->wChairID));
			if (!IsUserPlaying(pUserData->wChairID)) return true;

			//消息处理
			bResult=OnUserAddScore(pUserData->wChairID,pAddScore->lScore);
			break;
		}
	case SUB_C_OPEN_CARD:			//用户摊牌
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_OxCard));
			if (wDataSize!=sizeof(CMD_C_OxCard)) return false;

			//变量定义
			CMD_C_OxCard * pOxCard=(CMD_C_OxCard *)pDataBuffer;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//状态判断
			ASSERT(m_bPlayStatus[pUserData->wChairID]!=FALSE);
			if(m_bPlayStatus[pUserData->wChairID]==FALSE)return true;

			//消息处理
			bResult=OnUserOpenCard(pUserData->wChairID,pOxCard->bOX);
			break;

		}
	case SUB_C_AMDIN_COMMAND:
		{
			ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
			if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;
			return OnSubAmdinCommand(pIServerUserItem,pDataBuffer);
		}

	}

	//操作定时器
	if(bResult)
	{
		m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);
		return true;
	}

	return false;
}

//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}


//叫庄事件
bool CTableFrameSink::OnUserCallBanker(WORD wChairID, BYTE bBanker)
{
	//状态效验
	ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_CALL);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_CALL) return true;
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) return true;

	//设置变量
	m_bCallStatus[wChairID]=TRUE;

	//////////////////////////////////////////////////////////////////////////
	//叫庄人数
	WORD wCallUserCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_bPlayStatus[i]==TRUE && m_bCallStatus[i]==TRUE) 
			wCallUserCount++;
// 		else if(m_bPlayStatus[i]!=TRUE) 
// 			wCallUserCount++;
	}
	//////////////////////////////////////////////////////////////////////////


	//下注开始
	if(bBanker==TRUE || wCallUserCount== 2/*m_wPlayerCount*/ )
	{

		//////////////////////////////////////////////////////////////////////////
		//这里处理最后一次取消坐庄
		if(wCallUserCount == 2 && (bBanker != TRUE))
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_bPlayStatus[i]==TRUE) 
				{
					if (i == wChairID) 
						continue;
					else
					{
						m_wBankerUser = i;
						break;
					}
				}

			}
		}
		else
			m_wBankerUser=wChairID;
		//////////////////////////////////////////////////////////////////////////
		//始叫用户
		//m_wBankerUser=wChairID;
		m_wCurrentUser=INVALID_CHAIR;

		//过滤最后一个叫庄用户强退情况
		while(m_bPlayStatus[m_wBankerUser]==FALSE)
		{
			m_wBankerUser=(m_wBankerUser+1)%GAME_PLAYER;
		}

		//设置状态
		m_pITableFrame->SetGameStatus(GS_TK_SCORE);

		//庄家积分
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wBankerUser);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//玩家人数
		WORD wUserCount=0;
		for (WORD i=0;i<m_wPlayerCount;i++)if(m_bPlayStatus[i]==TRUE )wUserCount++;

		//最大下注
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]!=TRUE || i==m_wBankerUser)continue;

			//获取用户
			pIServerUserItem=m_pITableFrame->GetServerUserItem(i);

			//获取积分
			const tagUserScore * pUserScore=pIServerUserItem->GetUserScore();

			//金币检验
			ASSERT(pUserScore->lScore>=m_pGameServiceOption->lCellScore);

			//下注变量 客户要求
			m_lTurnMaxScore[i]=__min(lBankerScore/(wUserCount-1)/5,pUserScore->lScore/5);
		}

		//设置变量
		//m_wOperaCount=0;
		CMD_S_GameStart GameStart;
		GameStart.wBankerUser=m_wBankerUser;
		GameStart.lTurnMaxScore=0;

		//发送数据
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]!=TRUE)continue;
			GameStart.lTurnMaxScore=m_lTurnMaxScore[i];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	else		 //用户叫庄
	{
		//查找下个玩家
		do{
			m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		}while(m_bPlayStatus[m_wCurrentUser]==FALSE);

		//设置变量
		CMD_S_CallBanker CallBanker;
		CallBanker.wCallBanker=m_wCurrentUser;
		CallBanker.bFirstTimes=false;

		//发送数据
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]!=TRUE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
	}
	return true;
}

//获取对方玩家
WORD CTableFrameSink::GetOtherUserChair( WORD wUserChair )
{
	WORD wOtherUser=INVALID_CHAIR;

	for (WORD i=0;i<m_wPlayerCount;i++){
		if(m_bPlayStatus[i]==TRUE) {
			if (i == wUserChair) 
				continue;
			else{
				wOtherUser = i;
				break;
			}
		}
	}

	return wOtherUser;
}


//加注事件
bool CTableFrameSink::OnUserAddScore(WORD wChairID, LONGLONG lScore)
{
	//状态效验
	ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_SCORE);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_SCORE) return true;

	//金币效验
	if(m_bPlayStatus[wChairID]==TRUE)
	{
		ASSERT(lScore>0 && lScore<=m_lTurnMaxScore[wChairID]);
		if (lScore<=0 || lScore>m_lTurnMaxScore[wChairID]) return false;
	}
	else //没下注玩家强退
	{
		ASSERT(lScore==0);
		if (lScore!=0) return false;
	}

	if(lScore>0L)
	{
		//下注金币
		m_lTableScore[wChairID]=lScore;

		//构造数据
		CMD_S_AddScore AddScore;
		AddScore.wAddScoreUser=wChairID;
		AddScore.lAddScoreCount=m_lTableScore[wChairID];

		//发送数据
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]==FALSE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
	}

	//下注人数
	BYTE bUserCount=0;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_lTableScore[i]>0L && m_bPlayStatus[i]==TRUE)bUserCount++;
		else if(m_bPlayStatus[i]==FALSE || i==m_wBankerUser)bUserCount++;
	}

	//闲家全到
	if(bUserCount==m_wPlayerCount)
	{
		//设置状态
		m_pITableFrame->SetGameStatus(GS_TK_PLAYING);

		//构造数据
		CMD_S_SendCard SendCard;
		ZeroMemory(SendCard.cbCardData,sizeof(SendCard.cbCardData));

		//获取扑克
		BYTE bTempArray[GAME_PLAYER*MAX_COUNT];
		m_GameLogic.RandCardList(bTempArray,sizeof(bTempArray));
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]==FALSE)continue;

			//派发扑克
			CopyMemory(m_cbHandCardData[i],&bTempArray[i*MAX_COUNT],MAX_COUNT);
		}
		
		int nWiner = GetPrivateProfileInt(m_szGameRoomName,TEXT("WinerGL"), 50, m_szConfigFileName);
		srand( GetTickCount());
		
		//////////////////////////////////////////////////////////////////////////
		
		if (m_cbExcuteTimes > 0 && m_wAdminUser != INVALID_CHAIR)
		{
			WORD wOtherUser=GetOtherUserChair(m_wAdminUser);

			if(m_cbControlStyle == CS_BANKER_WIN)
				ChangeCard(m_wAdminUser, wOtherUser);//AnalyseAdminCard(m_wAdminUser, true);
			else
				ChangeCard(wOtherUser ,m_wAdminUser);//AnalyseAdminCard(m_wAdminUser, false);
			m_cbExcuteTimes--;
		}else
		{
			//作弊账户控制优先于库存控制
			if ((rand()%100)>=(100-nWiner))
				AnalyseUserCard();
			else
				AnalyseAndroidCard();
		}

		int i0=GetPrivateProfileInt(m_szGameRoomName,TEXT("Card0"), 0, m_szConfigFileName);
		if(i0!=0)
		{
			m_cbHandCardData[0][0]=i0;
			m_cbHandCardData[0][1]=GetPrivateProfileInt(m_szGameRoomName,TEXT("Card1"), 0, m_szConfigFileName);
			m_cbHandCardData[0][2]=GetPrivateProfileInt(m_szGameRoomName,TEXT("Card2"), 0, m_szConfigFileName);
			m_cbHandCardData[0][3]=GetPrivateProfileInt(m_szGameRoomName,TEXT("Card3"), 0, m_szConfigFileName);
			m_cbHandCardData[0][4]=GetPrivateProfileInt(m_szGameRoomName,TEXT("Card4"), 0, m_szConfigFileName);
			//m_cbHandCardData[0][0]=GetPrivateProfileInt(m_szGameRoomName,TEXT("Card0"), 0, m_szConfigFileName);
		}

		//发送扑克
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]==FALSE)continue;

			//派发扑克
			CopyMemory(SendCard.cbCardData[i],m_cbHandCardData[i],MAX_COUNT);
		}

		//发送数据
		for (WORD i=0;i< m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]==FALSE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	}

	return true;
}

//摊牌事件
bool CTableFrameSink::OnUserOpenCard(WORD wChairID, BYTE bOx)
{
	//状态效验
	ASSERT (m_pITableFrame->GetGameStatus()==GS_TK_PLAYING);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING) return true;
	if (m_bOxCard[wChairID]!=0xff) return true;

	//效验数据
	ASSERT(bOx<=TRUE);
	if(bOx>TRUE)return false;

	//////////////////////////////////////////////////////////////////////////
	bOx = (m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>0)?TRUE:FALSE;
	//////////////////////////////////////////////////////////////////////////

	//效验数据
	if(bOx)
	{
		ASSERT(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>0);
		if(!(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>0))return false;
	}
	else if(m_bPlayStatus[wChairID]==TRUE)
	{
		if(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>=OX_THREE_SAME)bOx=TRUE;
	}

	//牛牛数据
	m_bOxCard[wChairID] = bOx;

	//摊牌人数
	BYTE bUserCount=0;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_bOxCard[i]<2 && m_bPlayStatus[i]==TRUE)bUserCount++;
		else if(m_bPlayStatus[i]==FALSE)bUserCount++;
	}

	 //构造变量
	CMD_S_Open_Card OpenCard;
	ZeroMemory(&OpenCard,sizeof(OpenCard));

	//设置变量
	OpenCard.bOpen=bOx;
	OpenCard.wPlayerID=wChairID;

//	&& m_bIsUserLeave[1] == false && m_bIsUserLeave[0] == false

	bool bUserLeave=false;
	//发送数据
	for (WORD i=0;i< m_wPlayerCount;i++)
	{
		if(m_bIsUserLeave[i] == TRUE) bUserCount=true;
		if(m_bPlayStatus[i]==FALSE)continue;
		m_pITableFrame->SendTableData(i,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));	

	
	//结束游戏
	if(bUserCount == m_wPlayerCount && bUserLeave==false)
	{
		return OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);
	}

	return true;
}

//扑克分析
void CTableFrameSink::AnalyseAndroidCard()
{
	//机器人数
	bool bIsAiBanker = false;
	WORD wAiCount = 0;
	WORD wPlayerCount = 0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem!=NULL)
		{
			ASSERT(m_bPlayStatus[i]==TRUE);
			if(pIServerUserItem->IsAndroidUser()) 
			{
				wAiCount++ ;
				if(!bIsAiBanker && i==m_wBankerUser)bIsAiBanker = true;
			}
			wPlayerCount++; 
		}
	}

	//全部机器
	if(wPlayerCount == wAiCount || wAiCount==0)return;
//////////////////////////////////////////////////////////////////////////

	//是否继续
	bool bIsAndroidWin= false;
	int nWiner = GetPrivateProfileInt(m_szGameRoomName,TEXT("AndroidWinerGL"), 50, m_szConfigFileName);
	srand( GetTickCount()+m_pITableFrame->GetPlayTimeCount());
	if ((rand()%100)>=(100-nWiner))		
		bIsAndroidWin = true;
//////////////////////////////////////////////////////////////////////////

	WORD wOtherChair=GetOtherUserChair(m_wBankerUser);

	//根据赢率设置输赢
	if(bIsAndroidWin)
	{
		if(!bIsAiBanker)
		{
			ChangeCard(wOtherChair ,m_wBankerUser);
		}
		else
		{
			ChangeCard(m_wBankerUser ,wOtherChair);	
		}
	}else
	{
		if(bIsAiBanker)
			ChangeCard(wOtherChair ,m_wBankerUser);
		else
			ChangeCard(m_wBankerUser ,wOtherChair);

	}
	return ;
//////////////////////////////////////////////////////////////////////////

	//扑克变量
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

	//牛牛数据
	BOOL bUserOxData[GAME_PLAYER];
	ZeroMemory(bUserOxData,sizeof(bUserOxData));
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_bPlayStatus[i]!=TRUE)continue;
		bUserOxData[i] = (m_GameLogic.GetCardType(cbUserCardData[i],MAX_COUNT)>0)?TRUE:FALSE;
	}

	//排列扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_GameLogic.SortCardList(cbUserCardData[i],MAX_COUNT);
	}

	//变量定义
	LONGLONG lAndroidScore=0;

	//倍数变量
	BYTE cbCardTimes[GAME_PLAYER];
	ZeroMemory(cbCardTimes,sizeof(cbCardTimes));

	//查找倍数
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_bPlayStatus[i]==TRUE)
		{
			cbCardTimes[i]=m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT);
		}
	}

	//机器庄家
	if(bIsAiBanker)
	{
		//对比扑克
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//用户过滤
			if ((i==m_wBankerUser)||(m_bPlayStatus[i]==FALSE)) continue;

			//获取用户
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);

			//机器过滤
			if ((pIServerUserItem!=NULL)&&(pIServerUserItem->IsAndroidUser())) continue;

			//对比扑克
			if (m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[m_wBankerUser],MAX_COUNT,bUserOxData[i],bUserOxData[m_wBankerUser])==true)
			{
				lAndroidScore-=cbCardTimes[i]*m_lTableScore[i];
			}
			else
			{
				lAndroidScore+=cbCardTimes[m_wBankerUser]*m_lTableScore[i];
			}
		}
	}
	else//用户庄家
	{
		//对比扑克
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//获取用户
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);

			//用户过滤
			if ((i==m_wBankerUser)||(pIServerUserItem==NULL)||!(pIServerUserItem->IsAndroidUser())) continue;

			//对比扑克
			if (m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[m_wBankerUser],MAX_COUNT,bUserOxData[i],bUserOxData[m_wBankerUser])==true)
			{
				lAndroidScore+=cbCardTimes[i]*m_lTableScore[i];
			}
			else
			{
				lAndroidScore-=cbCardTimes[m_wBankerUser]*m_lTableScore[i];
			}
		}
	}

	//库存判断
	if ((m_lStockScore+lAndroidScore)<0)
	{
		//变量定义
		WORD wWinUser=INVALID_CHAIR;

		//判断最大
		bool bCardMax = (bIsAiBanker)?true:false;

		//查找数据
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//用户过滤
			if (m_bPlayStatus[i]==FALSE) continue;

			//设置用户
			if (wWinUser==INVALID_CHAIR)
			{
				wWinUser=i;
				continue;
			}

			//对比扑克
			if (m_GameLogic.CompareCard(cbUserCardData[i],m_cbHandCardData[wWinUser],MAX_COUNT,bUserOxData[i],bUserOxData[wWinUser])==bCardMax)
			{
				wWinUser=i;
			}
		}

		//交换数据
		BYTE cbTempData[MAX_COUNT];
		CopyMemory(cbTempData,m_cbHandCardData[m_wBankerUser],MAX_COUNT);
		CopyMemory(m_cbHandCardData[m_wBankerUser],m_cbHandCardData[wWinUser],MAX_COUNT);
		CopyMemory(m_cbHandCardData[wWinUser],cbTempData,MAX_COUNT);
	}

	return;
}


void CTableFrameSink::AnalyseUserCard()
{
	//限制处理
	bool bBreak = false;
	bool bIsAiBanker = false;
	WORD wWinIndex = INVALID_CHAIR;
	WORD wLoserIndex = INVALID_CHAIR;
	int nAccountsCount = 0;
	//扑克变量
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

	//牛牛数据
	BOOL bUserOxData[GAME_PLAYER];
	ZeroMemory(bUserOxData,sizeof(bUserOxData));
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_bPlayStatus[i]!=TRUE)continue;
		bUserOxData[i] = (m_GameLogic.GetCardType(cbUserCardData[i],MAX_COUNT)>0)?TRUE:FALSE;
	}

	//作弊
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		if(m_bPlayStatus[i]==FALSE)continue;
		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		nAccountsCount = GetPrivateProfileInt(m_szGameRoomName,TEXT("AccountsCount"), 0, m_szConfigFileName);
	
		if (nAccountsCount<=0) break;
		for (int j= 0; j < nAccountsCount; j++)
		{
			TCHAR szAccounts[NAME_LEN] = TEXT("");
			TCHAR szTmp[128] = TEXT("");
			_snprintf(szTmp, sizeof(szTmp), TEXT("Accounts%d"), j+1);
			GetPrivateProfileString(m_szGameRoomName,szTmp,TEXT(""),szAccounts,sizeof(szAccounts),m_szConfigFileName);

			if (lstrcmp(pIServerUserItem->GetAccounts(),szAccounts)==0)
			{
				if(!bIsAiBanker && i==m_wBankerUser) bIsAiBanker = true;
				//胜利玩家
				wWinIndex=i;
				wLoserIndex=wWinIndex==0?1:0;
				bBreak = true;
				break;
			}
		}
		if (bBreak) break;
	}

	//交换扑克
	if (nAccountsCount>0 && bBreak)
	{
		//变量定义
		WORD wWinUser=INVALID_CHAIR;

		//判断最大
		bool bCardMax = (bIsAiBanker)?true:false;

		//查找数据
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//用户过滤
			if (m_bPlayStatus[i]==FALSE) continue;

			//设置用户
			if (wWinUser==INVALID_CHAIR)
			{
				wWinUser=i;
				continue;
			}

			//对比扑克
			if (m_GameLogic.CompareCard(cbUserCardData[i],m_cbHandCardData[wWinUser],MAX_COUNT,bUserOxData[i],bUserOxData[wWinUser])==bCardMax)
			{
				wWinUser=i;
			}
		}

		//交换数据
		BYTE cbTempData[MAX_COUNT];
		CopyMemory(cbTempData,m_cbHandCardData[m_wBankerUser],MAX_COUNT);
		CopyMemory(m_cbHandCardData[m_wBankerUser],m_cbHandCardData[wWinUser],MAX_COUNT);
		CopyMemory(m_cbHandCardData[wWinUser],cbTempData,MAX_COUNT);
	}
	else
		AnalyseAndroidCard();
	return;
}

//交换扑克
bool CTableFrameSink::ChangeCard(WORD Winer, WORD Loser)
{
	//BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	//CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

	if(Winer == INVALID_CHAIR || Loser == INVALID_CHAIR) return false;

	//牛牛数据
	BOOL bUserOxData[GAME_PLAYER];
	ZeroMemory(bUserOxData,sizeof(bUserOxData));
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_bPlayStatus[i]!=TRUE)continue;
		bUserOxData[i] = (m_GameLogic.GetCardType(m_cbHandCardData[i],MAX_COUNT)>0)?TRUE:FALSE;
	}

	//排列扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_GameLogic.SortCardList(m_cbHandCardData[i],MAX_COUNT);
	}

	//对比扑克 作弊用户牌小
	if (m_GameLogic.CompareCard(m_cbHandCardData[Winer],m_cbHandCardData[Loser],MAX_COUNT,bUserOxData[Winer],bUserOxData[Loser])==false)
	{
		//交换数据
		BYTE cbTempData[MAX_COUNT];
		CopyMemory(cbTempData,m_cbHandCardData[Winer],MAX_COUNT);
		CopyMemory(m_cbHandCardData[Winer],m_cbHandCardData[Loser],MAX_COUNT);
		CopyMemory(m_cbHandCardData[Loser],cbTempData,MAX_COUNT);
	}

	return true;
}

bool CTableFrameSink::OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer)
{

	//如果不具有管理员权限 则返回错误
	if((pIServerUserItem->GetUserData()->dwUserRight&UR_GAME_CONTROL)==0)
	{
		return false;
	}

	m_wAdminUser=pIServerUserItem->GetChairID();

	const CMD_C_AdminReq* AdminReq=static_cast<const CMD_C_AdminReq*>(pDataBuffer);
	switch(AdminReq->cbReqType)
	{
	case RQ_RESET_CONTROL:
		{	
			m_cbControlStyle=0;
			m_cbExcuteTimes=0;

			CMD_S_CommandResult cResult;
			cResult.cbResult=CR_ACCEPT;
			cResult.cbAckType=ACK_RESET_CONTROL;
			m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
			break;
		}
	case RQ_SET_WIN_AREA:
		{

			const tagAdminReq*pAdminReq=reinterpret_cast<const tagAdminReq*>(AdminReq->cbExtendData);
			switch(pAdminReq->m_cbControlStyle)
			{
			case CS_BANKER_LOSE:	//庄家输牌
				{
					m_cbControlStyle=pAdminReq->m_cbControlStyle;
					m_cbExcuteTimes=pAdminReq->m_cbExcuteTimes;
					CMD_S_CommandResult cResult;
					cResult.cbResult=CR_ACCEPT;
					cResult.cbAckType=ACK_SET_WIN_AREA;
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
					
					break;
				}
			case CS_BANKER_WIN:		//庄家赢牌
				{
					
					m_cbControlStyle=pAdminReq->m_cbControlStyle;
					m_cbExcuteTimes=pAdminReq->m_cbExcuteTimes;
					CMD_S_CommandResult cResult;
					cResult.cbResult=CR_ACCEPT;
					cResult.cbAckType=ACK_SET_WIN_AREA;
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
					break;
				}
			default:	//拒绝请求
				{
					CMD_S_CommandResult cResult;
					cResult.cbResult=CR_REFUSAL;
					cResult.cbAckType=ACK_SET_WIN_AREA;
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
					break;
				}
			}

			break;
		}
	case RQ_PRINT_SYN:
		{
			CMD_S_CommandResult cResult;
			cResult.cbResult=CR_ACCEPT;
			cResult.cbAckType=ACK_PRINT_SYN;
			tagAdminReq*pAdminReq=reinterpret_cast<tagAdminReq*>(cResult.cbExtendData);
			pAdminReq->m_cbControlStyle=m_cbControlStyle;
			pAdminReq->m_cbExcuteTimes=m_cbExcuteTimes;
			m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
			break;
		}
	default:
		{
			break;
		}


	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

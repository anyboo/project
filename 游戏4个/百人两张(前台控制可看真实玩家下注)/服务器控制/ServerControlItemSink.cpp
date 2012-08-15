#include "StdAfx.h"
#include "servercontrolitemsink.h"


CServerControlItemSink::CServerControlItemSink(void)
{
	m_cbExcuteTimes = 0;								
	m_cbControlStyle = 0;	
	m_cbWinAreaCount = 0;
	ZeroMemory(m_bWinArea, sizeof(m_bWinArea));	
	ZeroMemory(m_nCompareCard, sizeof(m_nCompareCard));
	ZeroMemory(m_cbTableCardArray, sizeof(m_cbTableCardArray));
	ZeroMemory(m_cbTableCard, sizeof(m_cbTableCard));
	ZeroMemory(m_lAllJettonScore, sizeof(m_lAllJettonScore));
}

CServerControlItemSink::~CServerControlItemSink( void )
{

}

//服务器控制
bool __cdecl CServerControlItemSink::ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame)
{
	//效验数据
	if ( (pIServerUserItem->GetUserData()->dwMasterRight&UR_CAN_SERVER_OPTION) == 0 )
		return false;

	ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
	if (wDataSize!=sizeof(CMD_C_AdminReq)) 
		return false;

	CMD_C_AdminReq* AdminReq = (CMD_C_AdminReq*)pDataBuffer;
	switch(AdminReq->cbReqType)
	{
	case RQ_RESET_CONTROL:
		{	
			m_cbControlStyle=0;
			m_cbWinAreaCount=0;
			m_cbExcuteTimes=0;
			ZeroMemory(m_bWinArea,sizeof(m_bWinArea));

			CMD_S_CommandResult cResult;
			cResult.cbResult=CR_ACCEPT;
			cResult.cbAckType=ACK_RESET_CONTROL;
			pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
			break;
		}
	case RQ_SET_WIN_AREA:
		{

			const tagAdminReq*pAdminReq=reinterpret_cast<const tagAdminReq*>(AdminReq->cbExtendData);
			switch(pAdminReq->m_cbControlStyle)
			{
			case CS_BET_AREA:	//区域设置
				{
					m_cbControlStyle=pAdminReq->m_cbControlStyle;
					m_cbExcuteTimes=pAdminReq->m_cbExcuteTimes;
					m_cbWinAreaCount=0;
					BYTE cbIndex=0;
					for(cbIndex=0;cbIndex<CONTROL_AREA;cbIndex++)
					{
						m_bWinArea[cbIndex]=pAdminReq->m_bWinArea[cbIndex];
						if(m_bWinArea[cbIndex])
							m_cbWinAreaCount++;
					}
					CMD_S_CommandResult cResult;
					cResult.cbResult=CR_ACCEPT;
					cResult.cbAckType=ACK_SET_WIN_AREA;
					pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
					break;
				}
			case CS_BANKER_LOSE:	//庄家输牌
				{
					m_cbControlStyle=pAdminReq->m_cbControlStyle;
					m_cbExcuteTimes=pAdminReq->m_cbExcuteTimes;
					CMD_S_CommandResult cResult;
					cResult.cbResult=CR_ACCEPT;
					cResult.cbAckType=ACK_SET_WIN_AREA;
					pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));

					break;
				}
			case CS_BANKER_WIN:		//庄家赢牌
				{
					m_cbControlStyle=pAdminReq->m_cbControlStyle;
					m_cbExcuteTimes=pAdminReq->m_cbExcuteTimes;
					CMD_S_CommandResult cResult;
					cResult.cbResult=CR_ACCEPT;
					cResult.cbAckType=ACK_SET_WIN_AREA;
					pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
					break;
				}
			default:	//拒绝请求
				{
					CMD_S_CommandResult cResult;
					cResult.cbResult=CR_REFUSAL;
					cResult.cbAckType=ACK_SET_WIN_AREA;
					pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
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
			memcpy(pAdminReq->m_bWinArea,m_bWinArea,sizeof(m_bWinArea));
			pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
			break;
		}
	default:
		{
			break;
		}
	}

	return true;
}

//需要控制
bool __cdecl CServerControlItemSink::NeedControl()
{
	if(m_cbControlStyle > 0 && m_cbExcuteTimes > 0)
	{
		return true;
	}

	return false;
}

//满足控制
bool __cdecl CServerControlItemSink::MeetControl(tagControlInfo ControlInfo)
{
	return true;
}

//完成控制
bool __cdecl CServerControlItemSink::CompleteControl()
{
	if (m_cbExcuteTimes > 0)
		m_cbExcuteTimes--;	

	if ( m_cbExcuteTimes == 0)
	{
		m_cbExcuteTimes = 0;								
		m_cbControlStyle = 0;	
		m_cbWinAreaCount = 0;
		ZeroMemory(m_bWinArea, sizeof(m_bWinArea));	
		ZeroMemory(m_nCompareCard, sizeof(m_nCompareCard));
		ZeroMemory(m_cbTableCardArray, sizeof(m_cbTableCardArray));
		ZeroMemory(m_cbTableCard, sizeof(m_cbTableCard));
		ZeroMemory(m_lAllJettonScore, sizeof(m_lAllJettonScore));
	}

	return true;
}

//设置是否能成功
bool CServerControlItemSink::IsSettingSuccess( BYTE cbControlArea[MAX_INDEX] )
{
	return true;
}

//返回控制区域
bool __cdecl CServerControlItemSink::ReturnControlArea( tagControlInfo& ControlInfo )
{
	return true;
}

//控制牌型
void __cdecl CServerControlItemSink::GetSuitResult( BYTE cbTableCardArray[][MAX_CARD], BYTE cbTableCard[], LONGLONG lAllJettonScore[] )
{
	//填充变量
	memcpy(m_cbTableCardArray, cbTableCardArray, sizeof(m_cbTableCardArray));
	memcpy(m_cbTableCard, cbTableCard, sizeof(m_cbTableCard));
	memcpy(m_lAllJettonScore, lAllJettonScore, sizeof(m_lAllJettonScore));

	//排序扑克
	BYTE cbSuitStack[MAX_CARDGROUP] = {};

	//构建牌型
	BuildCardGroup();

	//自动组合
	GetSuitCardCombine(cbSuitStack);

	//重新设置纸牌排列
	BYTE UserCard[MAX_CARDGROUP][MAX_CARD] = {};
	memcpy(UserCard,m_cbTableCardArray,sizeof(UserCard));
	BYTE cbIndex=0;
	for(cbIndex=0;cbIndex<MAX_CARDGROUP;cbIndex++)
		memcpy(cbTableCardArray[cbIndex],UserCard[cbSuitStack[cbIndex]],sizeof(BYTE)*MAX_CARD);
	memcpy(cbTableCard, m_cbTableCard, sizeof(m_cbTableCard));
}

//是否是有效组合
bool CServerControlItemSink::GetSuitCardCombine(BYTE cbStack[])
{
	switch(m_cbControlStyle)
	{
	case CS_BET_AREA:
		{
			AreaWinCard(cbStack);
			break;
		}
	case CS_BANKER_WIN:
		{
			BankerWinCard(true,cbStack);
			break;
		}
	case CS_BANKER_LOSE:
		{
			BankerWinCard(false,cbStack);
			break;
		}

	}

	return true;
}

bool CServerControlItemSink::AreaWinCard(BYTE cbStack[])
{
	bool bIsUser[MAX_CARDGROUP]={0};	//是否已经使用
	BYTE cbStackCount=0;	//栈中元素数量
	BYTE cbIndex=0;

	//比较牌之间关系
	for(cbIndex=0;cbIndex<MAX_CARDGROUP;cbIndex++)
	{
		for(BYTE j=cbIndex;j<MAX_CARDGROUP;j++)
		{
			m_nCompareCard[j][cbIndex]=m_GameLogic.CompareCard(m_cbTableCardArray[cbIndex],MAX_CARD,m_cbTableCardArray[j],MAX_CARD);
			m_nCompareCard[cbIndex][j]=-m_nCompareCard[j][cbIndex];
		}
	}

	//查找派牌组合
	for(cbIndex=0;cbIndex<=MAX_CARDGROUP;)
	{
		if(cbIndex<MAX_CARDGROUP)
		{
			if(bIsUser[cbIndex]) cbIndex++;
			else
			{
				cbStack[cbStackCount]=cbIndex;
				bIsUser[cbIndex]=true;
				cbStackCount++;
				if(cbStackCount==MAX_CARDGROUP)	//如果已经找到一种组合
				{

					if(m_bWinArea[0]==(m_nCompareCard[cbStack[1]][cbStack[0]]>0?true:false) \
						&&m_bWinArea[1]==(m_nCompareCard[cbStack[2]][cbStack[0]]>0?true:false) \
						&&m_bWinArea[2]==(m_nCompareCard[cbStack[3]][cbStack[0]]>0?true:false))
						break;
					cbIndex=MAX_CARDGROUP;
				}
				else
				{
					cbIndex=0;
					continue;
				}
			}
		}
		else
		{
			if(cbStackCount>0)
			{
				cbIndex=cbStack[--cbStackCount];
				bIsUser[cbIndex]=false;
				cbIndex++;
				continue;
			}
			else break;
		}
	}
	return true;
}

LONGLONG CServerControlItemSink::GetBankerWinScore(bool bWinArea[])
{
	//庄家总量
	LONGLONG lBankerWinScore = 0;
	//区域倍率
	BYTE static const cbMultiple[] = {0, 1, 1, 1, 1, 1, 1};
	//胜利标识
	bool static bWinFlag[AREA_COUNT];
	bWinFlag[0]=false;
	bWinFlag[ID_DUI_MEN]=bWinArea[1];
	bWinFlag[ID_DAO_MEN]=bWinArea[2];
	bWinFlag[ID_SHUN_MEN]=bWinArea[0];
	bWinFlag[ID_QIAO]=bWinArea[0]&&bWinArea[2];
	bWinFlag[ID_JIAO_L]=bWinArea[0]&&bWinArea[1];
	bWinFlag[ID_JIAO_R]=bWinArea[2]&&bWinArea[1];

	for(BYTE cbIndex=1;cbIndex<=AREA_COUNT;cbIndex++)
	{
		lBankerWinScore+=m_lAllJettonScore[cbIndex]*(bWinFlag[cbIndex]?(-cbMultiple[cbIndex]):1);
	}
	return lBankerWinScore;
}

void CServerControlItemSink::BankerWinCard(bool bIsWin,BYTE cbStack[])
{
	bool bIsUser[MAX_CARDGROUP]={0};	//是否已经使用
	BYTE cbStackCount=0;	//栈中元素数量
	BYTE cbIndex=0;

	//比较牌之间关系
	for(cbIndex=0;cbIndex<MAX_CARDGROUP;cbIndex++)
	{
		for(BYTE j=cbIndex;j<MAX_CARDGROUP;j++)
		{
			m_nCompareCard[j][cbIndex]=m_GameLogic.CompareCard(m_cbTableCardArray[cbIndex],MAX_CARD,m_cbTableCardArray[j],MAX_CARD);
			m_nCompareCard[cbIndex][j]=-m_nCompareCard[j][cbIndex];
		}
	}

	//查找派牌组合
	for(cbIndex=0;cbIndex<=MAX_CARDGROUP;)
	{
		if(cbIndex<MAX_CARDGROUP)
		{
			if(bIsUser[cbIndex]) cbIndex++;
			else
			{
				cbStack[cbStackCount]=cbIndex;
				bIsUser[cbIndex]=true;
				cbStackCount++;
				if(cbStackCount==MAX_CARDGROUP)	//如果已经找到一种组合
				{
					bool cbWinArea[3];
					cbWinArea[0]=m_nCompareCard[cbStack[1]][cbStack[0]]>0?true:false;
					cbWinArea[1]=m_nCompareCard[cbStack[2]][cbStack[0]]>0?true:false;
					cbWinArea[2]=m_nCompareCard[cbStack[3]][cbStack[0]]>0?true:false;
					if(bIsWin&&GetBankerWinScore(cbWinArea)>=0)
						break;
					if(!bIsWin&&GetBankerWinScore(cbWinArea)<0)
						break;
					cbIndex=MAX_CARDGROUP;
				}
				else
				{
					cbIndex=0;
					continue;
				}
			}
		}
		else
		{
			if(cbStackCount>0)
			{
				cbIndex=cbStack[--cbStackCount];
				bIsUser[cbIndex]=false;
				cbIndex++;
				continue;
			}
			else break;
		}
	}
}


void CServerControlItemSink::BuildCardGroup()
{
	//重新洗牌
	ZeroMemory(m_cbTableCard, sizeof(m_cbTableCard));
	ZeroMemory(m_cbTableCardArray, sizeof(m_cbTableCardArray));

	while (true)
	{
		m_GameLogic.RandCardList(m_cbTableCard,CountArray(m_cbTableCard));
		memcpy(m_cbTableCardArray, m_cbTableCard, sizeof(m_cbTableCardArray));

		bool bNeedReDispatch = false;
		for (int i = 0; i < 3; i++)
		{
			for (int j = i+1; j < 4; j++)
			{
				if (m_GameLogic.CompareCard(m_cbTableCardArray[i],MAX_CARD,m_cbTableCardArray[j],MAX_CARD) == 0)
				{
					bNeedReDispatch = true;
					break;
				}
			}

			if (bNeedReDispatch)	break;
		}

		//符合要求
		if (!bNeedReDispatch)	break;
	}
}


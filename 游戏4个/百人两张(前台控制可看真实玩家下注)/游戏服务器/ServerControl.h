#pragma once

//游戏控制基类
class IServerControl
{
public:
	IServerControl(void){};
	virtual ~IServerControl(void){};

public:
	//服务器控制
	virtual bool __cdecl ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame) = NULL;

	//需要控制
	virtual bool __cdecl NeedControl() = NULL;

	//满足控制
	virtual bool __cdecl MeetControl(tagControlInfo ControlInfo) = NULL;

	//完成控制
	virtual bool __cdecl CompleteControl() = NULL;

	//返回控制区域
	virtual bool __cdecl ReturnControlArea(tagControlInfo& ControlInfo) = NULL;

	//开始控制
	virtual void __cdecl GetSuitResult( BYTE cbTableCardArray[][MAX_CARD], BYTE cbTableCard[], LONGLONG lAllJettonScore[] ) = NULL;
};

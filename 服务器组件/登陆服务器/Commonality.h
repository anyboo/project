#ifndef COMMONALITY_HEAD_FILE
#define COMMONALITY_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//数据库对象定义

//数据库请求标识
#define	DBR_GP_LOGON_BY_ACCOUNTS	1									//帐号登录
#define	DBR_GP_LOGON_BY_USERID		2									//I D 登录
#define DBR_GP_REGISTER_ACCOUNTS	3									//注册帐号
#define DBR_GP_USER_UPLOAD_FACE		4									//上传头像
#define DBR_GP_USER_LEAVE			5									//玩家离开
#define DBR_GP_DOWNLOAD_FACE		6									//下载头像
#define DBR_GP_DELETE_FACE			7									//删除头像
#define DBR_GP_MODIFY_INDIVIDUAL	8									//修改资料
#define DBR_GP_SAFE_BIND			9									//绑定帐号
#define DBR_GP_SAFE_UNBIND			10									//解除绑定
#define DBR_GP_OPER_MONEY			11									//操作钱数  新增功能 2012-4-10
#define DBR_GP_GIVE_MONEY			12									//操作钱数  新增功能 2012-4-10
#define DBR_GP_BANK_PASS			13									//银行密码  新增功能 2012-4-10
#define DBR_GP_GIVE_RECORD			14									//赠送记录  新增功能 2012-4-10
#define DBR_GP_USER_PASS			15									//银行密码  新增功能 2012-4-10
#define DBR_GP_USER_WRITE			16									//银行密码  新增功能 2012-4-10
#define DBR_GP_USER_INFO			17									//银行密码  新增功能 2012-4-10

//数据库输出标识
#define DBR_GP_LOGON_SUCCESS		100									//登录成功
#define DBR_GP_LOGON_ERROR			101									//登录失败
#define DBR_GP_DOWNLOADFACE_SUCCESS	102									//下载成功
#define DBR_GP_UPLOAD_FACE_RESULT	103									//上传结果
#define DBR_GP_DELETE_FACE_RESULT	104									//删除结果
#define DBR_GP_MODIFY_RESULT		105									//修改结果

#define DBR_GP_OPERMONEY_RESULT		106									//操作钱数 新增功能 2012-4-10
#define DBR_GP_REFRESHEMONEY_RESULT	107									//修改钱数 新增功能 2012-4-10

#define DBR_GP_GIVEMONEY_RESULT		108									//操作钱数 新增功能 2012-4-10

#define DBR_GP_BANKPASS_RESULT		109									//操作钱数 新增功能 2012-4-10
#define DBR_GP_GIVERECORD_RESULT	110								//操作钱数 新增功能 2012-4-10

//绑定帐号
struct DBR_GP_SafeBind
{
	DWORD							dwUserID;							//玩家ID
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//机器序列
	TCHAR								szPassWord[PASS_LEN];			//登录密码
};

//解除绑定
struct DBR_GP_SafeUnbind
{
	DWORD							dwUserID;							//玩家ID
	TCHAR								szPassWord[PASS_LEN];			//登录密码
};
//解除绑定
struct DBR_GP_BindResult
{
	BYTE								cbSuccess;						//玩家ID
	//TCHAR								szErrorDescribe[128];			//登录密码
};
//帐号登录
struct DBR_GP_LogonByAccounts
{
	DWORD							dwClientIP;							//连接地址
	TCHAR							szAccounts[NAME_LEN];				//登录帐号
	TCHAR							szPassWord[PASS_LEN];				//登录密码
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//机器序列
};

//ID 登录
struct DBR_GP_LogonByUserID
{
	DWORD							dwClientIP;							//连接地址
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassWord[PASS_LEN];				//登录密码
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//机器序列
};

//帐号注册
struct DBR_GP_RegisterAccounts
{
	WORD							wFaceID;							//注册头像
	BYTE							cbGender;							//用户性别
	DWORD							dwClientIP;							//连接地址
	TCHAR							szSpreader[NAME_LEN];				//推荐人名
	TCHAR							szAccounts[NAME_LEN];				//登录帐号
	TCHAR							szPassWord[PASS_LEN];				//登录密码
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//机器序列
    TCHAR							szBankpass[PASS_LEN];				//银行密码
	TCHAR							szIdno[NAME_LEN];				//用户身份证号
    TCHAR							szEmail[NAME_LEN];				//电子邮件
	TCHAR							szMobile[NAME_LEN];				//手机
};

//登录成功
struct DBR_GP_LogonSuccess
{
	//属性资料
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	DWORD							dwExperience;						//经验数值
	TCHAR							szPassword[PASS_LEN];				//登录密码
	TCHAR							szAccounts[NAME_LEN];				//登录帐号
	TCHAR							szGroupName[GROUP_LEN];				//社团名字

	//用户资料
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwCustomFaceVer;					//头像版本
	BYTE							cbMember;							//会员等级
	TCHAR							szUnderWrite[UNDER_WRITE_LEN];		//个性签名
     DWORD                            JiQi;                              //锁机标示
	//描述信息
	TCHAR							szDescribeString[128];				//描述消息
	TCHAR							szBankPassword[PASS_LEN];				//登录密码
};

//登录失败
struct DBR_GP_LogonError
{
	LONG							lErrorCode;							//错误代码
	TCHAR							szErrorDescribe[128];				//错误消息
};

//上传头像
struct DBR_GP_UploadCustomFace
{
	DWORD							dwUserID;							//玩家 ID
};

//上传结果
struct DBR_GP_UploadFaceResult
{
	TCHAR							szDescribeMsg[128];					//描述信息
	DWORD							dwFaceVer;							//头像版本
	bool							bOperateSuccess;					//成功标识
};

//删除结果
struct DBR_GP_DeleteFaceResult
{
	TCHAR							szDescribeMsg[128];					//描述信息
	DWORD							dwFaceVer;							//头像版本
	bool							bOperateSuccess;					//成功标识
};

//下载头像
struct DBR_GP_DownloadCustomFace
{
	DWORD							dwUserID;							//玩家 ID
};
struct DBR_GP_DownLockJiQi
{
	DWORD								dwGameUserID;						//玩家 ID
	bool								dwLockLP;					//最后标识
};
//下载成功
struct DBR_GP_DownloadFaceSuccess
{
	DWORD							dwToltalSize;						//总共大小
	DWORD							dwCurrentSize;						//当前大小
	DWORD							dwUserID;							//玩家 ID
	BYTE							bFaceData[2048];					//头像数据
};

//删除消息
struct DBR_GP_CustomFaceDelete
{
	DWORD							dwUserID;							//玩家 ID
	DWORD							dwFaceVer;							//头像版本
};

//个人资料
struct DBR_GP_ModifyIndividual
{
	DWORD							dwUserID;							//玩家 ID
	TCHAR							szNickname[NAME_LEN];				//玩家昵称
	int								nGender;							//玩家性别
	int								nAge;								//玩家年龄
	TCHAR							szAddress[64];						//玩家地址
	TCHAR							szUnderWrite[32];					//个性签名
	TCHAR							szPassword[33];						//玩家密码
};

//修改结果
struct DBR_GP_ModifyIndividualResult
{
	TCHAR							szDescribeMsg[128];					//描述信息
	bool							bOperateSuccess;					//成功标识
};


struct DBR_GP_OperMoney
{
	DWORD							dwUserID;							//玩家 ID
	DWORD							dwKindID;							//房间标识
	LONGLONG						lScore;								//钱数
	BYTE							cbType;								//类型  0:SAVE 1:GET  3：REFRESH
};

//修改结果
struct DBR_GP_MoneyOperResult
{
	TCHAR							szDescribeMsg[128];					//描述信息
	bool							bOperateSuccess;					//成功标识
};

struct DBR_GP_MoneyRefreshResult
{
	LONGLONG						lTotalScore;						//保险箱钱数
	LONGLONG						lRoomScore;							//房间钱数
};

struct DBR_GP_GiveMoney
{
	DWORD							dwClientIP;							//连接地址
	DWORD							dwUserID;							//玩家 ID
	TCHAR							szNickname[NAME_LEN];				//玩家昵称
	LONGLONG						lScore;								//钱数
	LONGLONG						lRevenue;							//税收
	BYTE							cbType;								//类型  0:用户名 1:用户ID
	BYTE							cbQueryInfo;						//查询用户信息 1查询
};

//修改结果
struct DBR_GP_GiveMoneyResult
{
	TCHAR							szDescribeMsg[128];					//描述信息
	TCHAR							szTargeAccount[NAME_LEN];			//用户信息
	DWORD							dwTargetUserId;						//对方ID
	LONGLONG						lRevenue;							//税收
	LONGLONG						lSwapScore;							//税后
	bool							bOperateSuccess;					//成功标识
	bool							bQueryInfo;							//查寻用户
};

struct DBR_GP_BankPass
{
	DWORD							dwUserID;							//玩家 ID
	TCHAR							szPassword[33];						//玩家密码
	TCHAR							szPassword1[33];						//玩家密码

};

//修改结果
struct DBR_GP_BankPassResult
{
	TCHAR							szDescribeMsg[128];					//描述信息
	bool							bOperateSuccess;					//成功标识
};

struct DBR_GP_GiveRecord
{
	DWORD							dwUserID;							//玩家 ID
};

//修改结果
struct DBR_GP_GiveRecordResult
{
	int								nCount;								//数量															
	bool							bOperateSuccess;					//成功标识
	TCHAR							szDescribeMsg[128];					//描述信息
	tagUserGiveRecord				GiveRecord[200];					//记录
};

struct DBR_GP_UserWrite
{
	DWORD							dwUserID;							//玩家 ID
	TCHAR							szUnderWrite[32];					//个性签名
};

struct DBR_GP_ModifyUserInfo
{
	DWORD							dwUserID;							//玩家 ID
	TCHAR							szAddress[64];						//玩家昵称
	TCHAR							szSchool[32];						//玩家地址
	TCHAR							szEmail[32];						//EMAIL
	TCHAR							szUrl[64];							//个人主页
	TCHAR							szTel[32];							//电话
	TCHAR							szMsn[32];							//MSN
};

//////////////////////////////////////////////////////////////////////////

#endif
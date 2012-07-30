#ifndef CMD_PLAZA_HEAD_FILE
#define CMD_PLAZA_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//广场版本
#define VER_PLAZA_LOW					1								//广场版本
#define VER_PLAZA_HIGH					2								//广场版本
#define VER_PLAZA_FRAME					MAKELONG(VER_PLAZA_LOW,VER_PLAZA_HIGH)

//////////////////////////////////////////////////////////////////////////
//登录错误标识

#define ERC_GP_LOGON_SUCCESS			0								//登陆成功
#define ERC_GP_ACCOUNTS_NOT_EXIST		1								//帐号不存在
#define ERC_GP_LONG_NULLITY				2								//禁止登录
#define ERC_GP_PASSWORD_ERCOR			3								//密码错误

//////////////////////////////////////////////////////////////////////////
//登录命令码

#define MDM_GP_LOGON					1								//广场登录

#define SUB_GP_LOGON_ACCOUNTS			1								//帐号登录
#define SUB_GP_LOGON_USERID				2								//I D 登录
#define SUB_GP_REGISTER_ACCOUNTS		3								//注册帐号
#define SUB_GP_UPLOAD_CUSTOM_FACE		4								//定义头像

#define SUB_GP_LOGON_SUCCESS			100								//登陆成功
#define SUB_GP_LOGON_ERROR				101								//登陆失败
#define SUB_GP_LOGON_FINISH				102								//登陆完成

//帐号登录
struct CMD_GP_LogonByAccounts
{
	DWORD								dwPlazaVersion;					//广场版本
	TCHAR								szAccounts[NAME_LEN];			//登录帐号
	TCHAR								szPassWord[PASS_LEN];			//登录密码
};

//I D 登录
struct CMD_GP_LogonByUserID
{
	DWORD								dwPlazaVersion;					//广场版本
	DWORD								dwUserID;						//用户 I D
	TCHAR								szPassWord[PASS_LEN];			//登录密码
};

//注册帐号
struct CMD_GP_RegisterAccounts
{
	WORD								wFaceID;						//头像标识
	BYTE								cbGender;						//用户性别
	DWORD								dwPlazaVersion;					//广场版本
	TCHAR								szSpreader[NAME_LEN];			//推广人名
	TCHAR								szAccounts[NAME_LEN];			//登录帐号
	TCHAR								szPassWord[PASS_LEN];			//登录密码
    TCHAR							szBankpass[PASS_LEN];				//银行密码
	TCHAR							szIdno[NAME_LEN];				//用户身份证号
    TCHAR							szEmail[NAME_LEN];				//电子邮件
	TCHAR							szMobile[NAME_LEN];				//手机
};

//登陆成功
struct CMD_GP_LogonSuccess
{
	WORD								wFaceID;						//头像索引
	BYTE								cbGender;						//用户性别
	BYTE								cbMember;						//会员等级
	DWORD								dwUserID;						//用户 I D
	DWORD								dwGameID;						//游戏 I D
	DWORD								dwExperience;					//用户经验
	DWORD                               JiQi;                           //锁定机器
	//扩展信息
	DWORD								dwCustomFaceVer;				//头像版本
};

//登陆失败
struct CMD_GP_LogonError
{
	LONG								lErrorCode;						//错误代码
	TCHAR								szErrorDescribe[128];			//错误消息
};

//////////////////////////////////////////////////////////////////////////
//游戏列表命令码

#define MDM_GP_SERVER_LIST				2								//列表信息

#define SUB_GP_LIST_TYPE				100								//类型列表
#define SUB_GP_LIST_KIND				101								//种类列表
#define SUB_GP_LIST_STATION				102								//站点列表
#define SUB_GP_LIST_SERVER				103								//房间列表
#define SUB_GP_LIST_FINISH				104								//发送完成
#define SUB_GP_LIST_CONFIG				105								//列表配置

//列表配置
struct CMD_GP_ListConfig
{
	BYTE								bShowOnLineCount;				//显示人数
};

//////////////////////////////////////////////////////////////////////////
//系统命令码

#define MDM_GP_SYSTEM					3								//系统命令

#define SUB_GP_VERSION					100								//版本通知
#define SUB_SP_SYSTEM_MSG				101								//系统消息

//版本通知
struct CMD_GP_Version
{
	BYTE								bNewVersion;					//更新版本
	BYTE								bAllowConnect;					//允许连接
};

//////////////////////////////////////////////////////////////////////////

#define MDM_GP_USER						4								//用户信息

#define SUB_GP_USER_UPLOAD_FACE			100								//上传头像
#define SUB_GP_USER_DOWNLOAD_FACE		101								//下载头像
#define SUB_GP_UPLOAD_FACE_RESULT		102								//上传结果
#define SUB_GP_DELETE_FACE_RESULT		103								//删除结果
#define SUB_GP_CUSTOM_FACE_DELETE		104								//删除头像
#define SUB_GP_MODIFY_INDIVIDUAL		105								//个人资料
#define SUB_GP_MODIFY_INDIVIDUAL_RESULT	106								//修改结果

#define SUB_GP_SAFE_BIND				107								//玩家绑定
#define SUB_GP_SAFE_UNBIND				108								//解除绑定

#define SUB_GP_OPEN_MONEY				109								//修改钱数  新增功能 2012-4-10
#define SUB_GP_REFRESH_MONEY			110								//刷新钱数  新增功能 2012-4-10
#define SUB_GP_GIVE_MONEY				111								//赠送钱数  新增功能 2012-4-12
#define SUB_GP_EDIT_BANK_PASS			112								//修改银行密码  新增功能 2012-4-12
#define SUB_GP_GIVE_RECORD				113								//赠送记录  新增功能 2012-4-12
#define SUB_GP_MODIFY_WRITE				114								//赠送记录  新增功能 2012-4-12
#define SUB_GP_EDIT_USER_PASS			115								//修改银行密码  新增功能 2012-4-12
#define SUB_GP_EDIT_USER_INFO			116								//用户信息  新增功能 2012-4-12


//绑定帐号
struct CMD_GP_SafeBind
{
	DWORD								dwUserID;						//玩家ID
	tagClientSerial						ClientSerial;					//机器序列
	TCHAR								szPassWord[PASS_LEN];			//登录密码
};

//解除绑定
struct CMD_GP_SafUnbind
{
	DWORD								dwUserID;						//玩家ID
	TCHAR								szPassWord[PASS_LEN];			//登录密码
};
//解除绑定
struct CMD_GP_BindResult
{
	BYTE								cbSuccess;						//玩家ID
	//TCHAR								szErrorDescribe[128];			//登录密码
};
//个人资料
struct CMD_GP_ModifyIndividual
{
	DWORD							dwUserID;							//玩家 ID
	TCHAR							szNickname[NAME_LEN];				//玩家昵称
	int								nGender;							//玩家性别
	int								nAge;								//玩家年龄
	TCHAR							szAddress[64];						//玩家地址
	TCHAR							szUnderWrite[32];					//个性签名
	TCHAR							szPassword[33];						//玩家密码
};

//定义头像
struct CMD_GP_UploadCustomFace
{
	DWORD								dwUserID;						//玩家 ID
	WORD								wCurrentSize;					//当前大小
	bool								bLastPacket;					//最后标识
	bool								bFirstPacket;					//第一个标识
	BYTE								bFaceData[2048];				//头像数据
};
//锁定机器
struct CMD_GP_LockJiQi
{
	DWORD								dwGameUserID;				  //玩家 ID
	bool								dwLockLP;					  //最后标识

};
//下载成功
struct CMD_GP_DownloadFaceSuccess
{
	DWORD							dwToltalSize;						//总共大小
	DWORD							dwCurrentSize;						//当前大小
	DWORD							dwUserID;							//玩家 ID
	BYTE							bFaceData[2048];					//头像数据
};

//下载头像
struct CMD_GP_DownloadFace
{
	DWORD							dwUserID;							//玩家 ID
};

//上传结果
struct CMD_GP_UploadFaceResult
{
	TCHAR							szDescribeMsg[128];					//描述信息
	DWORD							dwFaceVer;							//头像版本
	bool							bOperateSuccess;					//成功标识
};

//删除结果
struct CMD_GP_DeleteFaceResult
{
	TCHAR							szDescribeMsg[128];					//描述信息
	DWORD							dwFaceVer;							//头像版本
	bool							bOperateSuccess;					//成功标识
};

//删除消息
struct CMD_GP_CustomFaceDelete
{
	DWORD							dwUserID;							//玩家 ID
	DWORD							dwFaceVer;							//头像版本
};

//修改结果
struct CMD_GP_ModifyIndividualResult
{
	TCHAR							szDescribeMsg[128];					//描述信息
	bool							bOperateSuccess;					//成功标识
};

//修改结果
struct CMD_GP_MoneyOper
{
	DWORD							dwUserID;							//玩家 ID
	DWORD							dwKindID;							//房间标识
	LONGLONG						lScore;								//钱数
	BYTE							cbType;								//类型  0:SAVE 1:GET  3：REFRESH
};

//修改结果
struct CMD_GP_MoneyOperResult
{
	TCHAR							szDescribeMsg[128];					//描述信息
	bool							bOperateSuccess;					//成功标识
};

struct CMD_GP_MoneyRefreshResult
{
	LONGLONG						lTotalScore;						//保险箱钱数
	LONGLONG						lRoomScore;							//房间钱数
};
//赠送钱数
//赠送钱数
struct CMD_GP_MoneyGive
{
	DWORD							dwUserID;							//玩家 ID
	TCHAR							szNickname[NAME_LEN];				//玩家昵称
	LONGLONG						lScore;								//钱数
	BYTE							cbType;								//类型  0:用户名 1:用户ID 
	BYTE							cbQueryInfo;						//查询用户信息 1查询
};


//修改结果
//修改结果
struct CMD_GP_MoneyGiveResult
{
	TCHAR							szDescribeMsg[128];					//描述信息
	TCHAR							szTargeAccount[NAME_LEN];			//用户信息
	DWORD							dwTargetUserId;						//对方ID
	LONGLONG						lRevenue;							//税收
	LONGLONG						lSwapScore;							//税后
	bool							bOperateSuccess;					//成功标识
	bool							bQueryInfo;							//查寻用户
};

//修改银行密码
struct CMD_GP_EditBankPass
{
	DWORD							dwUserID;							//玩家 ID
	TCHAR							szPassword[33];						//玩家旧密码
	TCHAR							szPassword1[33];					//玩家新密码
};

//修改结果
struct CMD_GP_EditBankPassResult
{
	TCHAR							szDescribeMsg[128];					//描述信息
	bool							bOperateSuccess;					//成功标识
};

//获得银行记录
struct CMD_GP_BankRecord
{
	DWORD							dwUserID;							//玩家 ID
};

//修改结果
struct CMD_GP_BankRecordResult
{
	int								nCount;								//数量															
	bool							bOperateSuccess;					//成功标识
	TCHAR							szDescribeMsg[128];					//描述信息
	tagUserGiveRecord				GiveRecord[200];					//记录
};

//个人资料
struct CMD_GP_ModifyUserWrite
{
	DWORD							dwUserID;							//玩家 ID
	TCHAR							szUnderWrite[32];					//个性签名
};


//用户密码
struct CMD_GP_ModifyUserPass
{
	DWORD							dwUserID;							//玩家 ID
	TCHAR							szPassword[33];						//玩家旧密码
	TCHAR							szPassword1[33];					//玩家新密码
};

//个人资料
struct CMD_GP_ModifyUserInfo
{
	DWORD							dwUserID;							//玩家 ID
	TCHAR							szAddress[64];						//玩家昵称
	TCHAR							szSchool[32];						//玩家地址
	TCHAR							szEmail[32];						//EMAIL
	TCHAR							szUrl[64];							//个人主页
	TCHAR							szTel[32];						//电话
	TCHAR							szMsn[32];						//MSN
};
//////////////////////////////////////////////////////////////////////////

#endif
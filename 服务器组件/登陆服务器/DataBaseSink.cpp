#include "StdAfx.h"
#include "Commonality.h"
#include "DataBaseSink.h"
#include "LogonServer.h"


//声明变量
extern CLogonServerApp theApp;
//////////////////////////////////////////////////////////////////////////

//构造函数
CDataBaseSink::CDataBaseSink()
{
	//设置变量
	m_pInitParamter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return;
}

//析构函数
CDataBaseSink::~CDataBaseSink()
{
}

//接口查询
VOID * __cdecl CDataBaseSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
	return NULL;
}

//模块启动
bool __cdecl CDataBaseSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//创建对象
	if ((m_AccountsDBModule.GetInterface()==NULL)&&(m_AccountsDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	try
	{
		//连接用户数据库
		m_AccountsDBModule->SetConnectionInfo(m_pInitParamter->m_szUserDataBaseAddr,m_pInitParamter->m_wUserDataBasePort,
			m_pInitParamter->m_szUserDataBaseName,m_pInitParamter->m_szUserDataBaseUser,m_pInitParamter->m_szUserDataBasePass);
		m_AccountsDBModule->OpenConnection();

		//发起连接
		m_AccountsDBModule->OpenConnection();
		m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		return false;
	}

	return true;
}

//模块关闭
bool __cdecl CDataBaseSink::OnDataBaseEngineStop(IUnknownEx * pIUnknownEx)
{
	//设置对象
	m_AccountsDBAide.SetDataBase(NULL);

	//关闭连接
	if (m_AccountsDBModule.GetInterface()!=NULL)
	{
		m_AccountsDBModule->CloseConnection();
		m_AccountsDBModule.CloseInstance();
	}

	//组件变量
	m_pInitParamter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return true;
}

//数据操作处理
bool __cdecl CDataBaseSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBR_GP_LOGON_BY_ACCOUNTS:		//帐号登录
		{
			return OnRequestLogon(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOGON_BY_USERID:		//I D 登录
		{
			return OnRequestLogon(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_REGISTER_ACCOUNTS:		//注册帐号
		{
			return OnRequestLogon(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_UPLOAD_FACE:		//上传头像
		{
			return OnRequestUploadFace(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_DOWNLOAD_FACE:			//下载头像
		{
			return OnRequestDownloadFace(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_DELETE_FACE:			//删除消息
		{
			return OnRequestCustomFaceDelete(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_INDIVIDUAL:		//修改资料
		{
			return OnRequestModifyIndividual(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_SAFE_UNBIND:			//解除绑定
		{
			return OnRequestSafeUnbind(wRequestID,dwContextID,pData,wDataSize);
		}	
	case DBR_GP_SAFE_BIND:				//绑定帐号
		{
			return OnRequestSafeBind(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_OPER_MONEY:				//操作保险柜	22012-4-10
		{
			return OnRequestOperMoney(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_GIVE_MONEY:				//操作保险柜	22012-4-10
		{
			return OnRequestGiveMoney(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_BANK_PASS:				//保险柜密码	22012-4-10
		{
			return OnRequestBankPass(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_GIVE_RECORD:				//赠送记录	22012-4-10
		{
			return OnRequestGiveRecord(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_PASS:
		{
			return OnRequestUserPass(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_WRITE:
		{
			return OnRequestUserWrite(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_INFO:
		{
			return OnRequestUserInfo(wRequestID,dwContextID,pData,wDataSize);
		}

	}

	return false;
}

//登录请求处理
bool CDataBaseSink::OnRequestLogon(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//变量定义
		LONG lReturnValue=-1;
		LPCTSTR pszPassword=NULL;

		//登录处理
		switch (wRequestID)
		{
		case DBR_GP_LOGON_BY_ACCOUNTS:		//帐号登录
			{
				//效验参数
				ASSERT(wDataSize==sizeof(DBR_GP_LogonByAccounts));
				if (wDataSize!=sizeof(DBR_GP_LogonByAccounts)) return false;

				//执行查询
				DBR_GP_LogonByAccounts * pLogonByAccounts=(DBR_GP_LogonByAccounts *)pData;
				lReturnValue=SPLogonByAccounts(pLogonByAccounts->szAccounts,pLogonByAccounts->szPassWord,pLogonByAccounts->dwClientIP,pLogonByAccounts->szComputerID);
				pszPassword=pLogonByAccounts->szPassWord;

				break;
			}
		case DBR_GP_LOGON_BY_USERID:		//I D 登录
			{
				//效验参数
				ASSERT(wDataSize==sizeof(DBR_GP_LogonByUserID));
				if (wDataSize!=sizeof(DBR_GP_LogonByUserID)) return false;

				//执行查询
				DBR_GP_LogonByUserID * pLogonByUserID=(DBR_GP_LogonByUserID *)pData;
				lReturnValue=SPLogonByUserID(pLogonByUserID->dwUserID,pLogonByUserID->szPassWord,pLogonByUserID->dwClientIP,pLogonByUserID->szComputerID);
				pszPassword=pLogonByUserID->szPassWord;

				break;
			}
		case DBR_GP_REGISTER_ACCOUNTS:		//注册帐号
			{
				//效验参数
				ASSERT(wDataSize==sizeof(DBR_GP_RegisterAccounts));
				if (wDataSize!=sizeof(DBR_GP_RegisterAccounts)) return false;

				//执行查询
				DBR_GP_RegisterAccounts * pRegisterAccounts=(DBR_GP_RegisterAccounts *)pData;
				lReturnValue=SPRegisterAccounts(pRegisterAccounts->szAccounts,pRegisterAccounts->szPassWord,pRegisterAccounts->szSpreader,
					pRegisterAccounts->wFaceID,pRegisterAccounts->cbGender,pRegisterAccounts->dwClientIP,pRegisterAccounts->szComputerID,pRegisterAccounts->szBankpass,pRegisterAccounts->szIdno,pRegisterAccounts->szEmail,pRegisterAccounts->szMobile);
				pszPassword=pRegisterAccounts->szPassWord;

				break;
			}
		default :	//默认处理
			{
				ASSERT(FALSE); 
				return false;
			}
		}

		//结果判断
		if (lReturnValue==0L)
		{
			//登录成功
			DBR_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//读取变量
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMember=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwCustomFaceVer=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomFaceVer"));
			LogonSuccess.JiQi=m_AccountsDBAide.GetValue_DWORD(TEXT("MoorMachine"));//锁定机器
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));
			m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),LogonSuccess.szDescribeString,CountArray(LogonSuccess.szDescribeString));
            //m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szGroupName,CountArray(LogonSuccess.szGroupName));
            m_AccountsDBAide.GetValue_String(TEXT("BankPass"),LogonSuccess.szBankPassword,CountArray(LogonSuccess.szBankPassword));  //修改
			
			//设置密码
			CopyMemory(LogonSuccess.szPassword,pszPassword,sizeof(LogonSuccess.szPassword));

			//投递结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_LOGON_SUCCESS,dwContextID,&LogonSuccess,sizeof(LogonSuccess));
		}
		else
		{
			//登录失败
			DBR_GP_LogonError LogonError;
			ZeroMemory(&LogonError,sizeof(LogonError));

			//设置变量
			LogonError.lErrorCode=lReturnValue;
			m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),LogonError.szErrorDescribe,CountArray(LogonError.szErrorDescribe));

			//投递结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_LOGON_ERROR,dwContextID,&LogonError,sizeof(LogonError));
		}
	}
	catch (IDataBaseException * pIException)
	{
		//登录失败
		DBR_GP_LogonError LogonError;
		ZeroMemory(&LogonError,sizeof(LogonError));

		//输出错误
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//操作失败
		LogonError.lErrorCode=-1;
		lstrcpyn(LogonError.szErrorDescribe,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),CountArray(LogonError.szErrorDescribe));

		//投递结果
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_LOGON_ERROR,dwContextID,&LogonError,sizeof(LogonError));
	}

	//关闭记录集
	m_AccountsDBModule->CloseRecordset();

	return true;
}

//上传头像
bool CDataBaseSink::OnRequestUploadFace(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//数据验证
	ASSERT(wDataSize==sizeof(DBR_GP_UploadCustomFace));
	if (wDataSize!=sizeof(DBR_GP_UploadCustomFace)) return false;

	//数据转换
	DBR_GP_UploadCustomFace *pUploadCustomFace = (DBR_GP_UploadCustomFace*)pData;

	//文件定义
	CFile fileCustomFace;
	TCHAR szFileName[128];
	_snprintf(szFileName, CountArray(szFileName), TEXT("%s\\UploadFile_%ld.zip"), theApp.m_szDirWork, pUploadCustomFace->dwUserID);

	try
	{
		if ( fileCustomFace.Open( szFileName, CFile::modeRead|CFile::typeBinary) )
		{
			BYTE cbCustomFaceData[1024*30];

			//读取文件
			UINT uReadSize = fileCustomFace.Read(cbCustomFaceData, CountArray(cbCustomFaceData));

			//关闭文件
			fileCustomFace.Close();

			LONG lRet=-1;
			try
			{
				lRet = SPUploadFace(pUploadCustomFace->dwUserID, cbCustomFaceData, uReadSize);
			}
			catch (IDataBaseException * pIException)
			{
				//错误信息
				LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
				CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
			}

			//成功判断
			if ( lRet != -1 )
			{
				DBR_GP_UploadFaceResult UploadFaceResult;
				ZeroMemory(&UploadFaceResult, sizeof(UploadFaceResult));
				lstrcpyn(UploadFaceResult.szDescribeMsg, TEXT("头像上传成功！"), sizeof(UploadFaceResult.szDescribeMsg));
				UploadFaceResult.dwFaceVer=lRet;
				UploadFaceResult.bOperateSuccess=true;

				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_UPLOAD_FACE_RESULT,dwContextID,
					&UploadFaceResult,sizeof(UploadFaceResult));
			}
			else
			{
				//操作失败
				DBR_GP_UploadFaceResult UploadFaceResult;
				ZeroMemory(&UploadFaceResult, sizeof(UploadFaceResult));
				lstrcpyn(UploadFaceResult.szDescribeMsg, TEXT("由于数据库操作异常，请重新上传！"), sizeof(UploadFaceResult.szDescribeMsg));
				UploadFaceResult.bOperateSuccess=false;

				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_UPLOAD_FACE_RESULT,dwContextID,
					&UploadFaceResult,sizeof(UploadFaceResult));
			}

			//删除文件
			CFile::Remove(szFileName);
		}
	}catch(...){}	

	return true;
}
///////////
//绑定帐号
bool CDataBaseSink::OnRequestSafeBind(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//参数校验
	ASSERT(wDataSize == sizeof(DBR_GP_SafeBind) );
	if ( wDataSize != sizeof(DBR_GP_SafeBind) ) return false;

	try
	{
		DBR_GP_SafeBind *pSafeBind = (DBR_GP_SafeBind*)pData;

		
		//执行存储过程
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pSafeBind->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pSafeBind->szComputerID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pSafeBind->szPassWord);		

		LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_SafeBind"),true);
        
		if(lReturnValue != 0)
		{
			TCHAR szError[128];
			m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),szError,CountArray(szError));
		}
		DBR_GP_BindResult BindResult={0};
		BindResult.cbSuccess=BYTE(lReturnValue);
        
		//投递消息
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_SAFE_BIND, dwContextID, &BindResult, sizeof(BindResult));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	return true;
}

//解除绑定
bool CDataBaseSink::OnRequestSafeUnbind(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
		//参数校验
	ASSERT(wDataSize == sizeof(DBR_GP_SafeUnbind) );
	if ( wDataSize != sizeof(DBR_GP_SafeUnbind) ) return false;

	try
	{
		DBR_GP_SafeUnbind *pSafeUnbind = (DBR_GP_SafeUnbind*)pData;
        
		//执行存储过程
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pSafeUnbind->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pSafeUnbind->szPassWord);
		
		LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_SafeUnBind"),false);

		DBR_GP_BindResult BindResult={0};
		BindResult.cbSuccess=BYTE(lReturnValue);

		//投递消息
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_SAFE_UNBIND, dwContextID, &BindResult, sizeof(BindResult));
       

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	return true;
}
///////////
//下载头像
bool CDataBaseSink::OnRequestDownloadFace(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//校验参数
	ASSERT(wDataSize == sizeof(DBR_GP_DownloadCustomFace));
	if ( wDataSize != sizeof(DBR_GP_DownloadCustomFace)) return false;

	//类型转换
	DBR_GP_DownloadCustomFace *pDownloadCustomFace = (DBR_GP_DownloadCustomFace*)pData;

	LONG lReturnValue=-1;
	long lSize=0;
	bool bRet = false;
	try
	{
		lReturnValue=SPDownloadFace(pDownloadCustomFace->dwUserID);

		//读取数据
		if ( lReturnValue==0 )
		{	
			DWORD dwImageSize=0;
			dwImageSize=m_AccountsDBAide.GetValue_DWORD(TEXT("ImgSize"));
			
			//读取数据
			IDataBase * pIDataBase=m_AccountsDBModule.GetInterface();
			_variant_t vtFld;
			pIDataBase->GetRecordsetValue(TEXT("CustomFaceImage"), vtFld);
			BYTE * pcbBuf=NULL;
			SafeArrayAccessData(vtFld.parray,(LPVOID *)&pcbBuf);

			//投递请求
			DBR_GP_DownloadFaceSuccess DownloadFaceSuccess;
			ZeroMemory(&DownloadFaceSuccess, sizeof(DownloadFaceSuccess));
			DownloadFaceSuccess.dwToltalSize = dwImageSize;
			DownloadFaceSuccess.dwUserID = pDownloadCustomFace->dwUserID;
			DWORD dwSendSize = 0;
			while( dwSendSize <dwImageSize )
			{
				//设置数据
				if ( 1024 < dwImageSize - dwSendSize ) DownloadFaceSuccess.dwCurrentSize = 1024;
				else DownloadFaceSuccess.dwCurrentSize = dwImageSize - dwSendSize ;
				CopyMemory(DownloadFaceSuccess.bFaceData, pcbBuf+dwSendSize, DownloadFaceSuccess.dwCurrentSize);

				//投递消息
				WORD wPostSize = WORD(sizeof(DownloadFaceSuccess)-sizeof(DownloadFaceSuccess.bFaceData)+DownloadFaceSuccess.dwCurrentSize);
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_DOWNLOADFACE_SUCCESS, dwContextID, &DownloadFaceSuccess, wPostSize);

				//递增变量
				dwSendSize += DownloadFaceSuccess.dwCurrentSize;
			}

			//释放资源
			SafeArrayUnaccessData(vtFld.parray);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}


	return true;
}

//删除消息
bool CDataBaseSink::OnRequestCustomFaceDelete(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//参数验证
	ASSERT(wDataSize == sizeof(DBR_GP_CustomFaceDelete));
	if ( wDataSize != sizeof(DBR_GP_CustomFaceDelete)) return false;

	//类型转换
	DBR_GP_CustomFaceDelete *pCustomFaceDelete = (DBR_GP_CustomFaceDelete*)pData;

	//执行存储过程
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPDeleteCustomFace(pCustomFaceDelete->dwUserID);
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//成功判断
	if ( lReturnValue == 0 )
	{
		DBR_GP_DeleteFaceResult DeleteFaceResult;
		ZeroMemory(&DeleteFaceResult, sizeof(DeleteFaceResult));
		lstrcpyn(DeleteFaceResult.szDescribeMsg, TEXT("头像删除成功！"), sizeof(DeleteFaceResult.szDescribeMsg));
		DeleteFaceResult.dwFaceVer=pCustomFaceDelete->dwFaceVer;
		DeleteFaceResult.bOperateSuccess = true;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_DELETE_FACE_RESULT,dwContextID,
			&DeleteFaceResult,sizeof(DeleteFaceResult));
	}
	else
	{
		DBR_GP_DeleteFaceResult DeleteFaceResult;
		ZeroMemory(&DeleteFaceResult, sizeof(DeleteFaceResult));
		lstrcpyn(DeleteFaceResult.szDescribeMsg, TEXT("由于数据库操作异常，删除失败！"), sizeof(DeleteFaceResult.szDescribeMsg));
		DeleteFaceResult.dwFaceVer=pCustomFaceDelete->dwFaceVer;
		DeleteFaceResult.bOperateSuccess = false;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_DELETE_FACE_RESULT,dwContextID,
			&DeleteFaceResult,sizeof(DeleteFaceResult));
	}

	return true;
}
//锁定机器
bool CDataBaseSink::OnRequestLockJiQi(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
 ASSERT(wDataSize == sizeof(DBR_GP_DownLockJiQi));
	if ( wDataSize != sizeof(DBR_GP_DownLockJiQi)) return false;
 
	//类型转换
 DBR_GP_DownLockJiQi *DB_LockJiQi = (DBR_GP_DownLockJiQi*)pData;

 m_AccountsDBAide.ResetParameter();
 m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),DB_LockJiQi->dwGameUserID);
 m_AccountsDBAide.AddParameter(TEXT("@dwMoorMachine"),DB_LockJiQi->dwLockLP);


  return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_MoorMachineByGameID"),false);
}
//修改资料
bool CDataBaseSink::OnRequestModifyIndividual(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//参数验证
	ASSERT(wDataSize == sizeof(DBR_GP_ModifyIndividual));
	if ( wDataSize != sizeof(DBR_GP_ModifyIndividual)) return false;

	//类型转换
	DBR_GP_ModifyIndividual *pModifyIndividual = (DBR_GP_ModifyIndividual*)pData;

	//执行存储过程
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPModifyIndividual(pModifyIndividual->dwUserID, pModifyIndividual->szNickname, pModifyIndividual->nGender,
			pModifyIndividual->nAge, pModifyIndividual->szAddress, pModifyIndividual->szUnderWrite, pModifyIndividual->szPassword);
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//成功判断
	if ( lReturnValue == 0 )
	{
		DBR_GP_ModifyIndividualResult ModifyIndividualResult;
		ZeroMemory(&ModifyIndividualResult, sizeof(ModifyIndividualResult));
		lstrcpyn(ModifyIndividualResult.szDescribeMsg, TEXT("资料修改成功！"), sizeof(ModifyIndividualResult.szDescribeMsg));
		ModifyIndividualResult.bOperateSuccess = true;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_MODIFY_RESULT,dwContextID,
			&ModifyIndividualResult,sizeof(ModifyIndividualResult));
	}
	else
	{
		DBR_GP_ModifyIndividualResult ModifyIndividualResult;
		ZeroMemory(&ModifyIndividualResult, sizeof(ModifyIndividualResult));
		lstrcpyn(ModifyIndividualResult.szDescribeMsg, TEXT("由于数据库操作异常，修改失败！"), sizeof(ModifyIndividualResult.szDescribeMsg));
		ModifyIndividualResult.bOperateSuccess = false;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_MODIFY_RESULT,dwContextID,
			&ModifyIndividualResult,sizeof(ModifyIndividualResult));
	}
	return true;
}

//帐号存储过程
LONG CDataBaseSink::SPLogonByAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID)
{
	//效验参数
	ASSERT(pszAccounts!=NULL);
	ASSERT(pszPassword!=NULL);

	//转化地址
	TCHAR szClientIP[16]=TEXT("");
	BYTE * pClientIP=(BYTE *)&dwClientIP;
	_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

	//执行存储过程
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pszAccounts);
	m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pszPassword);
	m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pszComputerID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyAccounts"),true);
}

//更新存储过程
LONG CDataBaseSink::SPRegisterAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, LPCTSTR pszSpreader, WORD wFaceID, BYTE cbGender, DWORD dwClientIP, LPCTSTR pszComputerID, LPCTSTR pszBankpass, LPCTSTR pszIdno, LPCTSTR pszEmail, LPCTSTR pszMobile)
{
	//效验参数
	ASSERT(pszAccounts!=NULL);
	ASSERT(pszPassword!=NULL);

	//转化地址
	TCHAR szClientIP[16]=TEXT("");
	BYTE * pClientIP=(BYTE *)&dwClientIP;
	_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

	//执行存储过程
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pszAccounts);
	m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pszPassword);
	m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pszSpreader);
	m_AccountsDBAide.AddParameter(TEXT("@nFaceID"),wFaceID);
	m_AccountsDBAide.AddParameter(TEXT("@cbGender"),cbGender);
	m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pszComputerID);
    m_AccountsDBAide.AddParameter(TEXT("@strBankpass"),pszBankpass);
	m_AccountsDBAide.AddParameter(TEXT("@strPassWordCode"),pszIdno);
	m_AccountsDBAide.AddParameter(TEXT("@strEmail"),pszEmail);
	m_AccountsDBAide.AddParameter(TEXT("@strMobile"),pszMobile);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccounts"),true);
}

//I D 存储过程
LONG CDataBaseSink::SPLogonByUserID(DWORD dwGameID, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID)
{
	//效验参数
	ASSERT(dwGameID!=0L);
	ASSERT(pszPassword!=NULL);

	//转化地址
	TCHAR szClientIP[16]=TEXT("");
	BYTE * pClientIP=(BYTE *)&dwClientIP;
	_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

	//执行存储过程
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),dwGameID);
	m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pszPassword);
	m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pszComputerID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyGameID"),true);
}

//上传存储过程
LONG CDataBaseSink::SPUploadFace(DWORD dwUserID, BYTE *bFaceData, DWORD dwFaceDataSize)
{
	//效验参数
	ASSERT(dwUserID!=0L);
	ASSERT(dwFaceDataSize!=0L);
	ASSERT(bFaceData!=NULL);

	//效验参数
	ASSERT(dwUserID!=0L);
	ASSERT(dwFaceDataSize!=0L);
	ASSERT(bFaceData!=NULL);

	//执行存储过程
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);

	VARIANT varChunk;
	SAFEARRAY *psa=NULL;
	SAFEARRAYBOUND rgsabound[1];
	rgsabound[0].cElements = dwFaceDataSize;
    rgsabound[0].lLbound = 0;
    psa = SafeArrayCreate(VT_UI1,1,rgsabound);
    for (long index =0; index < long(dwFaceDataSize); index++ )
	{
		SafeArrayPutElement(psa,&index,&bFaceData[index]);
	}
    varChunk.vt = VT_ARRAY|VT_UI1;
    varChunk.parray = psa;

	IDataBase * pIDataBase=m_AccountsDBModule.GetInterface();
	pIDataBase->AddParameter(TEXT("@imgCustomFaceImage"),adLongVarBinary,adParamInput,dwFaceDataSize,_variant_t(varChunk));

	LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_CF_InsertCustomFace"),false);

	//释放资源
	SafeArrayDestroy(psa);

	return lReturnValue;
}

//加载存储过程
LONG CDataBaseSink::SPDownloadFace(DWORD dwUserID)
{
	//效验参数
	ASSERT(dwUserID!=0L);

	//执行存储过程
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);


	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_CF_LoadCustomFace"),true);
}

//删除存储过程
LONG CDataBaseSink::SPDeleteCustomFace(DWORD dwUserID)
{
	//效验参数
	ASSERT(dwUserID!=0L);

	//执行存储过程
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_CF_DeleteCustomFace"),true);
}

//修改存储过程
LONG CDataBaseSink::SPModifyIndividual(DWORD dwUserID, TCHAR szNickname[NAME_LEN], int nGender, int nAge, TCHAR szAddress[64], TCHAR szUnderWrite[32], TCHAR szPassword[32])
{
	//执行存储过程
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),szPassword);
	m_AccountsDBAide.AddParameter(TEXT("@strNickName"),szNickname);
	m_AccountsDBAide.AddParameter(TEXT("@nGender"),nGender);
	m_AccountsDBAide.AddParameter(TEXT("@nAge"),nAge);
	m_AccountsDBAide.AddParameter(TEXT("@strAddress"),szAddress);
	m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"),szUnderWrite);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_CF_ModifyIndividual"),false);
}
//绑定帐号
bool CDataBaseSink::OnRequestOperMoney(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//参数校验
	ASSERT(wDataSize == sizeof(DBR_GP_OperMoney) );
	if ( wDataSize != sizeof(DBR_GP_OperMoney) ) return false;

	try
	{
		DBR_GP_OperMoney *pOperMoney = (DBR_GP_OperMoney*)pData;

		if(pOperMoney->cbType==2)
		{
			////执行存储过程
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pOperMoney->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@nType"),pOperMoney->cbType);
			m_AccountsDBAide.AddParameter(TEXT("@dwKindID"),pOperMoney->dwKindID);
			m_AccountsDBAide.AddParameter(TEXT("@lScore"),pOperMoney->lScore);	

			LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_OPER_MONEY"),true);

			if(lReturnValue==0)
			{
				DBR_GP_MoneyRefreshResult MoneyOperResult;
				ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));

				MoneyOperResult.lTotalScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("a"));
				MoneyOperResult.lRoomScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("b"));

				//投递消息
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_REFRESHEMONEY_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
			}

		}
		else
		{

			////执行存储过程
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pOperMoney->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@nType"),pOperMoney->cbType);
			m_AccountsDBAide.AddParameter(TEXT("@dwKindID"),pOperMoney->dwKindID);
			m_AccountsDBAide.AddParameter(TEXT("@lScore"),pOperMoney->lScore);	

			LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_OPER_MONEY"),true);


			if(lReturnValue==0)
			{
				DBR_GP_MoneyOperResult MoneyOperResult;
				ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
				if(pOperMoney->cbType==0)
					lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("保存成功！"), sizeof(MoneyOperResult.szDescribeMsg));
				else
					lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("提取成功！"), sizeof(MoneyOperResult.szDescribeMsg));
				MoneyOperResult.bOperateSuccess = true;

				//投递消息
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_OPERMONEY_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
			}
			else
			{
				DBR_GP_MoneyOperResult MoneyOperResult;
				ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
				m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),MoneyOperResult.szDescribeMsg,CountArray(MoneyOperResult.szDescribeMsg));
				//lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("资料修改成功！"), sizeof(MoneyOperResult.szDescribeMsg));
				MoneyOperResult.bOperateSuccess = false;

				//投递消息
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_OPERMONEY_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
			}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		DBR_GP_MoneyOperResult MoneyOperResult;
		ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
		//m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),MoneyOperResult.szDescribeMsg,CountArray(MoneyOperResult.szDescribeMsg));
		lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("操作失败，请联系管理员！"), sizeof(MoneyOperResult.szDescribeMsg));
		MoneyOperResult.bOperateSuccess = false;

		//投递消息
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_OPERMONEY_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));

	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
//绑定帐号
bool CDataBaseSink::OnRequestGiveMoney(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//参数校验
	ASSERT(wDataSize == sizeof(DBR_GP_GiveMoney) );
	if ( wDataSize != sizeof(DBR_GP_GiveMoney) ) return false;

	try
	{
		DBR_GP_GiveMoney *pOperMoney = (DBR_GP_GiveMoney*)pData;

		
		{
			TCHAR szClientIP[16]=TEXT("");
			BYTE * pClientIP=(BYTE *)&(pOperMoney->dwClientIP);
			_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);
			////执行存储过程
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pOperMoney->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@nType"),pOperMoney->cbType);
			m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pOperMoney->szNickname);
			m_AccountsDBAide.AddParameter(TEXT("@wGameID"),atoi(pOperMoney->szNickname));
			m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
			m_AccountsDBAide.AddParameter(TEXT("@bQueryInfo"),pOperMoney->cbQueryInfo);	
			m_AccountsDBAide.AddParameter(TEXT("@lScore"),pOperMoney->lScore);	
			m_AccountsDBAide.AddParameter(TEXT("@lRevenue"), pOperMoney->lRevenue);

			LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GIVE_MONEY"),true);
	       
			if (lReturnValue==10)
			{
				DBR_GP_GiveMoneyResult MoneyOperResult;
				ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
				//lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("赠送成功！"), sizeof(MoneyOperResult.szDescribeMsg));
				m_AccountsDBAide.GetValue_String(TEXT("TargetAccount"),MoneyOperResult.szTargeAccount,CountArray(MoneyOperResult.szTargeAccount));
// 				CString str;
// 				DWORD dwuserid =  m_AccountsDBAide.GetValue_DWORD(TEXT("TargetGameID"));
// 				str.Format("userId%d", dwuserid);
// 				AfxMessageBox(str);
				MoneyOperResult.dwTargetUserId = m_AccountsDBAide.GetValue_DWORD(TEXT("TargetGameID"));
				//MoneyOperResult.lRevenue= m_AccountsDBAide.GetValue_LONGLONG(TEXT(""));
				MoneyOperResult.lSwapScore= m_AccountsDBAide.GetValue_LONGLONG(TEXT("SwapScore"));
				MoneyOperResult.bOperateSuccess = true;
				MoneyOperResult.bQueryInfo = true;
				//投递消息
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GIVEMONEY_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));

			}
			else if(lReturnValue==0)
			{
					DBR_GP_GiveMoneyResult MoneyOperResult;
					ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
					lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("赠送成功！"), sizeof(MoneyOperResult.szDescribeMsg));
					MoneyOperResult.bOperateSuccess = true;
			        
					//投递消息
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GIVEMONEY_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
			}else
			{
					DBR_GP_GiveMoneyResult MoneyOperResult;
					ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
					m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),MoneyOperResult.szDescribeMsg,CountArray(MoneyOperResult.szDescribeMsg));
					//lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("资料修改成功！"), sizeof(MoneyOperResult.szDescribeMsg));
					MoneyOperResult.bOperateSuccess = false;
			        
					//投递消息
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GIVEMONEY_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
				
			}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		DBR_GP_GiveMoneyResult MoneyOperResult;
		ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
		//m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),MoneyOperResult.szDescribeMsg,CountArray(MoneyOperResult.szDescribeMsg));
		lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("赠送失败"), sizeof(MoneyOperResult.szDescribeMsg));
		MoneyOperResult.bOperateSuccess = false;
			        
					//投递消息
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GIVEMONEY_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
	}
	return true;
}


//绑定帐号
bool CDataBaseSink::OnRequestBankPass(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//参数校验
	ASSERT(wDataSize == sizeof(DBR_GP_BankPass) );
	if ( wDataSize != sizeof(DBR_GP_BankPass) ) return false;

	try
	{
		DBR_GP_BankPass *pBankPass = (DBR_GP_BankPass*)pData;

		
		{
		
			////执行存储过程
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pBankPass->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pBankPass->szPassword);
			m_AccountsDBAide.AddParameter(TEXT("@strPassword1"),pBankPass->szPassword1);
			
			LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_BANK_PASS"),true);
	       
			
			if(lReturnValue==0)
				{
					DBR_GP_BankPassResult MoneyOperResult;
					ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
					lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("保存成功！"), sizeof(MoneyOperResult.szDescribeMsg));
					MoneyOperResult.bOperateSuccess = true;
			        
					//投递消息
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_BANKPASS_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
				}
			else
				{
					DBR_GP_BankPassResult MoneyOperResult;
					ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
					m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),MoneyOperResult.szDescribeMsg,CountArray(MoneyOperResult.szDescribeMsg));
					//lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("资料修改成功！"), sizeof(MoneyOperResult.szDescribeMsg));
					MoneyOperResult.bOperateSuccess = false;
			        
					//投递消息
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_BANKPASS_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
				
				}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	return true;
}

//绑定帐号
bool CDataBaseSink::OnRequestGiveRecord(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//参数校验
	ASSERT(wDataSize == sizeof(DBR_GP_GiveRecord) );
	if ( wDataSize != sizeof(DBR_GP_GiveRecord) ) return false;

	try
	{
		DBR_GP_GiveRecord *pGiveRecord = (DBR_GP_GiveRecord*)pData;

		
		{
		
			////执行存储过程
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pGiveRecord->dwUserID);
			
			LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GIVE_RECORD"),true);
	       
			
			if(lReturnValue==0)
				{
					DBR_GP_GiveRecordResult MoneyOperResult;
					ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
				//	lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("保存成功！"), sizeof(MoneyOperResult.szDescribeMsg));
				//	MoneyOperResult.bOperateSuccess = true;
					
					int num=0;
				
					//读取记录
					if (m_AccountsDBModule->IsRecordsetEnd()==false) 
					{
						m_AccountsDBModule->MoveToFirst();
						while ( m_AccountsDBModule->IsRecordsetEnd() == false )
						{
							
							MoneyOperResult.GiveRecord[num].dwRecordID=m_AccountsDBAide.GetValue_DWORD(TEXT("RecordID"));
							m_AccountsDBAide.GetValue_String(TEXT("SourceUserName"),MoneyOperResult.GiveRecord[num].szGiveName,CountArray(MoneyOperResult.GiveRecord[num].szGiveName));
							m_AccountsDBAide.GetValue_String(TEXT("TargetUserName"),MoneyOperResult.GiveRecord[num].szTargetUserName,CountArray(MoneyOperResult.GiveRecord[num].szTargetUserName));
							MoneyOperResult.GiveRecord[num].lGiveScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("SwapScore"));
							MoneyOperResult.GiveRecord[num].cbType=m_AccountsDBAide.GetValue_LONG(TEXT("TradeType"));
							SYSTEMTIME m_time;
							m_AccountsDBAide.GetValue_SystemTime(TEXT("CollectDate"),m_time);
							
							CString strTime;
							strTime.Format("%d-%d-%d %02d:%02d:%02d",m_time.wYear,m_time.wMonth,m_time.wDay,m_time.wHour,m_time.wMinute,m_time.wSecond);
							lstrcpy(MoneyOperResult.GiveRecord[num].szTime,strTime.GetBuffer());
							num++;
							MoneyOperResult.nCount = num;
						
							//移动记录
							m_AccountsDBModule->MoveToNext();
						}
					}

			        MoneyOperResult.bOperateSuccess = true;
					//投递消息
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GIVERECORD_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
				}
			else
				{
					DBR_GP_GiveRecordResult MoneyOperResult;
					ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
					m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),MoneyOperResult.szDescribeMsg,CountArray(MoneyOperResult.szDescribeMsg));
					//lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("资料修改成功！"), sizeof(MoneyOperResult.szDescribeMsg));
					MoneyOperResult.bOperateSuccess = false;
			        
					//投递消息
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GIVERECORD_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
				
				}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

			DBR_GP_GiveRecordResult MoneyOperResult;
			ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
		//	m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),MoneyOperResult.szDescribeMsg,CountArray(MoneyOperResult.szDescribeMsg));
			lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("获取失败！"), sizeof(MoneyOperResult.szDescribeMsg));
			MoneyOperResult.bOperateSuccess = false;
			        
					//投递消息
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GIVERECORD_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
	}
	return true;
}

//绑定帐号
bool CDataBaseSink::OnRequestUserPass(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//参数校验
	ASSERT(wDataSize == sizeof(DBR_GP_BankPass) );
	if ( wDataSize != sizeof(DBR_GP_BankPass) ) return false;

	try
	{
		DBR_GP_BankPass *pUserPass = (DBR_GP_BankPass*)pData;

		
		{
		
			////执行存储过程
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pUserPass->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pUserPass->szPassword);
			m_AccountsDBAide.AddParameter(TEXT("@strPassword1"),pUserPass->szPassword1);
			
			LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_USER_PASS"),true);
	       
			
			if(lReturnValue==0)
				{
					DBR_GP_ModifyIndividualResult OperResult;
					ZeroMemory(&OperResult, sizeof(OperResult));
					lstrcpyn(OperResult.szDescribeMsg, TEXT("保存成功！"), sizeof(OperResult.szDescribeMsg));
					OperResult.bOperateSuccess = true;
			        
					//投递消息
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(SUB_GP_MODIFY_INDIVIDUAL_RESULT, dwContextID, &OperResult, sizeof(OperResult));
				}
			else
				{
					DBR_GP_ModifyIndividualResult OperResult;
					ZeroMemory(&OperResult, sizeof(OperResult));
					m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),OperResult.szDescribeMsg,CountArray(OperResult.szDescribeMsg));
					//lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("资料修改成功！"), sizeof(MoneyOperResult.szDescribeMsg));
					OperResult.bOperateSuccess = false;
			        
					//投递消息
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(SUB_GP_MODIFY_INDIVIDUAL_RESULT, dwContextID, &OperResult, sizeof(OperResult));
				
				}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	return true;
}

//绑定帐号
bool CDataBaseSink::OnRequestUserWrite(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//参数校验
	ASSERT(wDataSize == sizeof(DBR_GP_UserWrite) );
	if ( wDataSize != sizeof(DBR_GP_UserWrite) ) return false;

	try
	{
		DBR_GP_UserWrite *pUserWrite = (DBR_GP_UserWrite*)pData;
		{
		
			////执行存储过程
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pUserWrite->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@szUnderWrite"),pUserWrite->szUnderWrite);
			
			
			LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_USER_UNDERWRITE"),true);
	       
			
			if(lReturnValue==0)
				{
					DBR_GP_ModifyIndividualResult OperResult;
					ZeroMemory(&OperResult, sizeof(OperResult));
					lstrcpyn(OperResult.szDescribeMsg, TEXT("保存成功！"), sizeof(OperResult.szDescribeMsg));
					OperResult.bOperateSuccess = true;
			        
					//投递消息
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(SUB_GP_MODIFY_INDIVIDUAL_RESULT, dwContextID, &OperResult, sizeof(OperResult));
				}
			else
				{
					DBR_GP_ModifyIndividualResult OperResult;
					ZeroMemory(&OperResult, sizeof(OperResult));
					m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),OperResult.szDescribeMsg,CountArray(OperResult.szDescribeMsg));
					//lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("资料修改成功！"), sizeof(MoneyOperResult.szDescribeMsg));
					OperResult.bOperateSuccess = false;
			        
					//投递消息
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(SUB_GP_MODIFY_INDIVIDUAL_RESULT, dwContextID, &OperResult, sizeof(OperResult));
				
				}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	return true;
}

//绑定帐号
bool CDataBaseSink::OnRequestUserInfo(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//参数校验
	ASSERT(wDataSize == sizeof(DBR_GP_ModifyUserInfo) );
	if ( wDataSize != sizeof(DBR_GP_ModifyUserInfo) ) return false;

	try
	{
		DBR_GP_ModifyUserInfo *pUserInfo = (DBR_GP_ModifyUserInfo*)pData;
		{
			////执行存储过程
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pUserInfo->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@szAddress"),pUserInfo->szAddress);
			m_AccountsDBAide.AddParameter(TEXT("@szSchool"),pUserInfo->szSchool);
			m_AccountsDBAide.AddParameter(TEXT("@szEmail"),pUserInfo->szEmail);
			m_AccountsDBAide.AddParameter(TEXT("@szUrl"),pUserInfo->szUrl);
			m_AccountsDBAide.AddParameter(TEXT("@szMsn"),pUserInfo->szMsn);
			m_AccountsDBAide.AddParameter(TEXT("@szTel"),pUserInfo->szTel);
			
			
			LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_USER_INFO"),true);
	       
			
			if(lReturnValue==0)
				{
					DBR_GP_ModifyIndividualResult OperResult;
					ZeroMemory(&OperResult, sizeof(OperResult));
					lstrcpyn(OperResult.szDescribeMsg, TEXT("保存成功！"), sizeof(OperResult.szDescribeMsg));
					OperResult.bOperateSuccess = true;
			        
					//投递消息
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(SUB_GP_MODIFY_INDIVIDUAL_RESULT, dwContextID, &OperResult, sizeof(OperResult));
				}
			else
				{
					DBR_GP_ModifyIndividualResult OperResult;
					ZeroMemory(&OperResult, sizeof(OperResult));
					m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),OperResult.szDescribeMsg,CountArray(OperResult.szDescribeMsg));
					//lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("资料修改成功！"), sizeof(MoneyOperResult.szDescribeMsg));
					OperResult.bOperateSuccess = false;
			        
					//投递消息
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(SUB_GP_MODIFY_INDIVIDUAL_RESULT, dwContextID, &OperResult, sizeof(OperResult));
				
				}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	return true;
}

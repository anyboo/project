#pragma once

#include "Stdafx.h"
#include "Resource.h"


class SuoJi
{
public:

	SuoJi();
	~SuoJi();
public:
	 
	ITCPSocket					  * m_pIClientSocket;					//网络组件

	TCHAR							m_szConfigFileName[MAX_PATH];			//配置文件

	int                            m_LpSuoJi;
public:
   
  bool	ConnectServer();

  void SenDdatelock();
  void SenDdateAnlock();

int ShowMessageBox(LPCTSTR pszMessage, UINT nType);





};




















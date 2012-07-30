#ifndef __BINDSTATUSCALLBACK_H_H
#define __BINDSTATUSCALLBACK_H_H

#pragma once
#include "resource.h"
#include "urlmon.h"







//回调接口
class CBindStatusCallback : public IBindStatusCallback
{
private:
	LONG		m_cRef;
	HWND		m_hWnd;

public:
	CBindStatusCallback(HWND hWnd) : m_cRef(0), m_hWnd(hWnd)
	{
	}

	virtual ~CBindStatusCallback()
	{
	}

	STDMETHOD(OnStartBinding)(DWORD dwReserved,IBinding __RPC_FAR *pib)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(GetPriority)(LONG __RPC_FAR *pnPriority)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnLowResource)(DWORD reserved)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnProgress)(ULONG ulProgress,ULONG ulProgressMax,ULONG ulStatusCode,LPCWSTR wszStatusText)
	{
		return S_OK;
	}

	STDMETHOD(OnStopBinding)(HRESULT hresult,LPCWSTR szError)
	{ 
		return E_NOTIMPL; 
	}

	STDMETHOD(GetBindInfo)(DWORD __RPC_FAR *grfBINDF, BINDINFO __RPC_FAR *pbindinfo)
	{ 
		if( !grfBINDF || !pbindinfo || grfBINDF==NULL )
			return E_POINTER;

		*grfBINDF |= BINDF_GETNEWESTVERSION | BINDF_NOWRITECACHE;

		if( pbindinfo->cbSize < sizeof(BINDINFO) )
			return E_INVALIDARG;

		if( pbindinfo->dwBindVerb!=BINDVERB_POST && pbindinfo->dwBindVerb!=BINDVERB_GET )
			return E_UNEXPECTED;

		return S_OK;
	}

	STDMETHOD(OnDataAvailable)(DWORD grfBSCF,DWORD dwSize,FORMATETC __RPC_FAR *pformatetc,STGMEDIUM __RPC_FAR *pstgmed)
	{ 
		return E_NOTIMPL;
	}

	STDMETHOD(OnObjectAvailable)(REFIID riid,IUnknown __RPC_FAR *punk)
	{ 
		return E_NOTIMPL;
	}

	STDMETHOD_(ULONG,AddRef)()
	{
		return InterlockedIncrement(&m_cRef);
	}

	STDMETHOD_(ULONG,Release)()
	{ 
		InterlockedDecrement(&m_cRef);
		if( m_cRef == 0L )
		{
			delete this;
		}
		return m_cRef; 
	}

	STDMETHOD(QueryInterface)(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		if( riid == IID_IUnknown )
		{
			*ppvObject=reinterpret_cast<PVOID>(this);
			AddRef();
			return S_OK;
		}

		if( riid == IID_IBindStatusCallback )
		{
			*ppvObject=reinterpret_cast<PVOID>(this);
			AddRef();
			return S_OK;
		}

		return E_NOINTERFACE; 
	}
};


#endif
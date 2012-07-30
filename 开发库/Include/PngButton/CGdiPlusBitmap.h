#ifndef GDIPLUS_BITMAP_HEAD_FILE
#define GDIPLUS_BITMAP_HEAD_FILE


#pragma once

//#include "Stdafx.h"
//////////////////////////////////////////////////////////////////////////

//位图类
class CGdiPlusBitmap
{
	//变量定义
public:
	Gdiplus::Bitmap*							m_pBitmap;					//位图变量

	//函数定义
public:
	//构造函数
	CGdiPlusBitmap()
	{ 
		m_pBitmap = NULL; 
	}
	//构造函数
	CGdiPlusBitmap(LPCWSTR pFile)
	{ 
		m_pBitmap = NULL; 
		LoadFromFile(pFile); 
	}
	//析构函数
	virtual ~CGdiPlusBitmap()
	{ 
		Empty(); 
	}
	
	//功能函数
public:
	//清空函数
	void Empty()
	{ 
		delete m_pBitmap; 
		m_pBitmap = NULL; 
	}
	//加载位图
	bool LoadFromFile(LPCWSTR pFile)
	{
		Empty();
		m_pBitmap = Gdiplus::Bitmap::FromFile(pFile);
		return m_pBitmap->GetLastStatus() == Gdiplus::Ok;
	}
	//重载操作函数
	operator Gdiplus::Bitmap*() const
	{ 
		return m_pBitmap; 
	}
};

//位图资源类
class CGdiPlusBitmapResource : public CGdiPlusBitmap
{
	//变量定义
protected:
	HGLOBAL										m_hBuffer;					//缓存

	//函数定义
public:
	//构造函数
	CGdiPlusBitmapResource()					
	{ 
		m_hBuffer = NULL; 
	}
	//构造函数
	CGdiPlusBitmapResource(LPCTSTR pName, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL)
	{ 
		m_hBuffer = NULL; 
		Load(pName, pType, hInst); 
	}
	//构造函数
	CGdiPlusBitmapResource(UINT id, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL)
	{ 
		m_hBuffer = NULL; 
		Load(id, pType, hInst); 
	}
	//构造函数
	CGdiPlusBitmapResource(UINT id, UINT type, HMODULE hInst = NULL)
	{ 
		m_hBuffer = NULL; 
		Load(id, type, hInst); 
	}
	//析构函数
	virtual ~CGdiPlusBitmapResource()
	{
		Empty(); 
	}

	//功能函数
public:
	//清空函数
	void Empty();
	//加载资源
	bool Load(LPCTSTR pName, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL);
	//加载资源
	bool Load(UINT id, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL)
	{ 
		return Load(MAKEINTRESOURCE(id), pType, hInst); 
	}
	//加载资源
	bool Load(UINT id, UINT type, HMODULE hInst = NULL)
	{ 
		return Load(MAKEINTRESOURCE(id), MAKEINTRESOURCE(type), hInst); 
	}
};

//内联函数
inline void CGdiPlusBitmapResource::Empty()
{
	CGdiPlusBitmap::Empty();
	if (m_hBuffer)
	{
		::GlobalUnlock(m_hBuffer);
		::GlobalFree(m_hBuffer);
		m_hBuffer = NULL;
	} 
}

//内联函数
inline bool CGdiPlusBitmapResource::Load(LPCTSTR pName, LPCTSTR pType, HMODULE hInst)
{
	Empty();

	HRSRC hResource = ::FindResource(hInst, pName, pType);
	if (!hResource)		return false;
	
	DWORD imageSize = ::SizeofResource(hInst, hResource);
	if (!imageSize)		return false;

	const void* pResourceData = ::LockResource(::LoadResource(hInst, hResource));
	if (!pResourceData)		return false;

	m_hBuffer  = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
	if (m_hBuffer)
	{
		void* pBuffer = ::GlobalLock(m_hBuffer);
		if (pBuffer)
		{
			CopyMemory(pBuffer, pResourceData, imageSize);

			IStream* pStream = NULL;
			if (::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
			{
				m_pBitmap = Gdiplus::Bitmap::FromStream(pStream);
				pStream->Release();
				if (m_pBitmap)
				{ 
					if (m_pBitmap->GetLastStatus() == Gdiplus::Ok)	return true;

					delete m_pBitmap;
					m_pBitmap = NULL;
				}
			}
			::GlobalUnlock(m_hBuffer);
		}
		::GlobalFree(m_hBuffer);
		m_hBuffer = NULL;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
#endif
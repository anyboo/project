// MemDC.h: interface for the CMemDC1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMDC_H__A68B97E6_DDB4_4B4F_8A18_429844C3659C__INCLUDED_)
#define AFX_MEMDC_H__A68B97E6_DDB4_4B4F_8A18_429844C3659C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMemDC1 : public CDC  
{
public:
	CMemDC1(CDC* pDC);
	CMemDC1* operator->(){return this;}
	operator CMemDC1*(){return this;}
	virtual ~CMemDC1();
private:
	CBitmap	m_bitmap;
	CBitmap*	m_pOldBitmap;
	CDC*	m_pDC;
	CRect	m_rect;
	BOOL	m_bMemDC;

};

#endif // !defined(AFX_MEMDC_H__A68B97E6_DDB4_4B4F_8A18_429844C3659C__INCLUDED_)

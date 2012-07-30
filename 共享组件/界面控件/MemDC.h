#ifndef __MEMDC_H__
#define __MEMDC_H__
#pragma once

class CMemDC : public CDC
{
public:
	CMemDC(CDC* pDC, LPRECT lpRect)
	{
		ASSERT( pDC );

		if( lpRect == NULL )
			pDC->GetClipBox(&m_rect);
		else
			m_rect.CopyRect(lpRect);


		VERIFY(CreateCompatibleDC(pDC));
		SetMapMode(pDC->GetMapMode());
		VERIFY(m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height()));
		m_pOldBitmap = SelectObject(&m_bitmap);
		FillSolidRect(lpRect, RGB(255,255,255));
	}

	virtual ~CMemDC()
	{
		DeleteDC();
	}

	BOOL DeleteDC()
	{
		if( m_pOldBitmap && m_hDC )
		{
			ASSERT(m_hDC);
			SelectObject(m_pOldBitmap);
		}
		m_bitmap.DeleteObject();

		return CDC::DeleteDC();
	}

#ifdef _DEBUG
	virtual void AssertValid() const
	{
		CDC::AssertValid();
		ASSERT( m_bitmap.m_hObject );
	}
#endif

protected:
	CBitmap   m_bitmap;
	CBitmap*  m_pOldBitmap;
	CRect     m_rect;
};

#endif
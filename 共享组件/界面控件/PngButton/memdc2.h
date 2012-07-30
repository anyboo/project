#ifndef MEMORY_DC_HEAD_FILE
#define MEMORY_DC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//内存DC类
class CMemDC2 : public CDC 
{
	//变量定义
private:
	CBitmap						m_bitmap;										// 屏幕外位图
	CBitmap*					m_oldBitmap;									// 最初位图对象
	CDC*						m_pDC;											// 保存经过构造CDC
	CRect						m_rect;											// 长方形绘制区域
	BOOL						m_bMemDC;										// 是否为内存DC

	//函数定义
public:
	//构造函数定义
	CMemDC2(CDC* pDC, CRect rect = CRect(0,0,0,0), BOOL bCopyFirst = FALSE) : CDC(), m_oldBitmap(NULL), m_pDC(pDC)
	{
		ASSERT(m_pDC != NULL);			// 断言参数不为空
		
		m_bMemDC = !pDC->IsPrinting();
		
		if (m_bMemDC)
		{
			//创建一个内存DC
			CreateCompatibleDC(pDC);
			if ( rect == CRect(0,0,0,0) )
			{
				pDC->GetClipBox(&m_rect);
			}
			else
			{
				m_rect = rect;
			}
			//创建兼容位图
			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			//保留旧位图对象
			m_oldBitmap = SelectObject(&m_bitmap);
			//移动窗口原点
			SetWindowOrg(m_rect.left, m_rect.top);
			//初次复制
			if(bCopyFirst)
			{
				this->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),m_pDC, m_rect.left, m_rect.top, SRCCOPY);
			}
		} 
		else
		{
			//创建一个跟当前DC绘制有关的副本
			m_bPrinting = pDC->m_bPrinting;
			m_hDC = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
	}
	
	//析构函数
	~CMemDC2()
	{
		if (m_bMemDC) 
		{
			//复制屏幕外位图到屏幕内
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),	this, m_rect.left, m_rect.top, SRCCOPY);
			//交换原来的位图
			SelectObject(m_oldBitmap);
		} 
		else
		{
			m_hDC = m_hAttribDC = NULL;
		}
	}
	
	// 允许使用指针
	CMemDC2* operator->() {return this;}
	
	// 允许使用指针
	operator CMemDC2*() {return this;}
};

#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


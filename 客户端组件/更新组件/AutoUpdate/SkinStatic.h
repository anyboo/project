#pragma once


// CSkinStatic

class CSkinStatic : public CStatic
{
	DECLARE_DYNAMIC(CSkinStatic)

protected:
	CTransparentHelper m_objTransparent;
	COLORREF m_crTextColor;

public:
	CSkinStatic();
	virtual ~CSkinStatic();

	void SetTextColor(COLORREF crTextColor) { m_crTextColor=crTextColor; }
	COLORREF GetTextColor() const { return m_crTextColor; }

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};



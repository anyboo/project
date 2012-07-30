#pragma once

#ifndef SKIN_EDIT_HEAD_FILE
#define SKIN_EDIT_HEAD_FILE

#include "SkinControls.h"
#include "SkinResourceManager.h"
#include "SkinImage.h"
//属性名字
//#define SKIN_EDIT_CRFOCUSTX			TEXT("编辑框焦点字色")				// COLORREF 类型 （读写）
//#define SKIN_EDIT_CRFOCUSBK			TEXT("编辑框焦点背景色")			// COLORREF 类型 （读写）
//#define SKIN_EDIT_CRNOFOCUSTX		TEXT("编辑框正常字色")				// COLORREF 类型 （读写）
//#define SKIN_EDIT_CRNOFOCUSBK		TEXT("编辑框正常背景色")			// COLORREF 类型 （读写）
//#define SKIN_EDIT_CRDISFOCUSTX		TEXT("编辑框禁止字色")				// COLORREF 类型 （读写）
//#define SKIN_EDIT_CRDISFOCUSBK		TEXT("编辑框禁止背景色")			// COLORREF 类型 （读写）

//////////////////////////////////////////////////////////////////////////

//编辑框属性
class CSkinEditAttribute : public CSkinResourceManager
{
	friend class CSkinEdit;

	//设置部分
public:
	COLORREF							m_crFocusTX;					//字体颜色
	COLORREF							m_crFocusBK;					//背景颜色
	COLORREF							m_crNoFocusTX;					//字体颜色
	COLORREF							m_crNoFocusBK;					//背景颜色
	COLORREF							m_crDisFocusTX;					//字体颜色
	COLORREF							m_crDisFocusBK;					//背景颜色

	//GDI 对象
public:
	CBrush								m_brFocus;						//背景刷
	CBrush								m_brNoFocus;					//背景刷
	CBrush								m_brDisbale;					//背景刷
	CSkinImage                          m_ImageBottom;					//向下图片
	//函数定义
protected:
	//构造函数
	CSkinEditAttribute();
	//析构函数
	virtual ~CSkinEditAttribute();

	//接口函数
public:
	//加载配置
	bool UpdateSkinResource(ISkinResource * pISkinResource);
};

//////////////////////////////////////////////////////////////////////////

//编辑框
//class SKIN_CONTROL_CLASS CSkinEdit : public CEdit
//{
//	//变量定义
//protected:
//	bool							m_bFocus;						//焦点标志
//	bool							m_bHovering;					//盘旋标志
//	//静态变量
//public:
//	static CSkinEditAttribute		m_SkinAttribute;				//界面属性
//
//	//函数定义
//public:
//	//构造函数
//	CSkinEdit();
//	//析构函数
//	virtual ~CSkinEdit();
//
//	//查询函数
//public:
//	//获取属性接口
//	static CSkinEditAttribute * GetSkinAttribute() { return &m_SkinAttribute; }
//
//	//消息函数 
//protected:
//	//绘画控件
//	//afx_msg HBRUSH CtlColor(CDC * pDC, UINT nCtlColor);
//	//得到焦点
//	afx_msg void OnSetFocus(CWnd * pOldWnd);
//	//失去焦点
//	afx_msg void OnKillFocus(CWnd * pNewWnd);
//	//鼠标移动
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	//鼠标离开
//	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
//	afx_msg void OnPaint();
//	//消息映射
//protected:
//	DECLARE_MESSAGE_MAP()
//	DECLARE_DYNAMIC(CSkinEdit)
//};

//编辑框
class SKIN_CONTROL_CLASS CSkinEdit : public CEdit
{
	//变量定义
protected:
	bool							m_bFocus;						//焦点标志

	//静态变量
public:
	static CSkinEditAttribute		m_SkinAttribute;				//界面属性

	//函数定义
public:
	//构造函数
	CSkinEdit();
	//析构函数
	virtual ~CSkinEdit();

	//查询函数
public:
	//获取属性接口
	static CSkinEditAttribute * GetSkinAttribute() { return &m_SkinAttribute; }

	//消息函数 
protected:
	//绘画控件
	afx_msg HBRUSH CtlColor(CDC * pDC, UINT nCtlColor);
	//得到焦点
	afx_msg void OnSetFocus(CWnd * pOldWnd);
	//失去焦点
	afx_msg void OnKillFocus(CWnd * pNewWnd);
	//重画消息
    VOID OnNcPaint();

protected:
	//绘画控件
	VOID DrawControlView(CDC * pDC, COLORREF crColorBorad, COLORREF crColorBK);


	//消息映射
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinEdit)
};

//编辑框
class SKIN_CONTROL_CLASS CSkinEditEx : public CSkinEdit
{
	//画刷对象
public:
	CBrush							m_brEnable;							//画刷资源
	CBrush							m_brDisable;						//画刷资源

	//背景颜色
public:
	COLORREF						m_crEnableBK;						//背景颜色
	COLORREF						m_crDisableBK;						//背景颜色

	//字体颜色
public:
	COLORREF						m_crEnableText;						//字体颜色
	COLORREF						m_crDisableText;					//字体颜色

	//边框颜色
public:
	COLORREF						m_crEnableBorad;					//边框颜色
	COLORREF						m_crDisableBorad;					//边框颜色

	//函数定义
public:
	//构造函数
	CSkinEditEx();
	//析构函数
	virtual ~CSkinEditEx();

	//功能函数
public:
	//设置颜色
	VOID SetEnableColor(COLORREF crEnableText, COLORREF crEnableBK, COLORREF crEnableBorad);
	//设置颜色
	VOID SetDisableColor(COLORREF crDisableText, COLORREF crDisableBK, COLORREF crDisableBorad);

	
	//消息函数 
protected:
	//重画消息
    VOID OnNcPaint();
	//绘画控件
	HBRUSH CtlColor(CDC * pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};
//////////////////////////////////////////////////////////////////////////

class SKIN_CONTROL_CLASS CMyComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CMyComboBox)
	//变量定义
protected:
	BYTE							m_bFocus;						//焦点标志
//	static CSkinImage				m_ImageBottom;						//向下图片
	int								iWidth;
	int								iHeight;

public:
	CMyComboBox();
	virtual ~CMyComboBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	//	afx_msg void OnPaint();
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//鼠标离开
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

//////////////////////////////////////////////////////////////////////////

#endif
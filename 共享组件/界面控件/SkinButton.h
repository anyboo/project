#pragma once

#ifndef SKIN_BUTTON_HEAD_FILE
#define SKIN_BUTTON_HEAD_FILE

#include "SkinImage.h"
#include "SkinControls.h"
#include "SkinResourceManager.h"
#include "PngButton\\CGdiPlusBitmap.h"
#include "PngButton\\MemDC2.h"
#include "PngImage.h"
//////////////////////////////////////////////////////////////////////////

//宏定义
#define DEF_TEXT_FRAME_COLOR			RGB(255,255,255)				//默认颜色
#define DEF_TEXT_COLOR					RGB(10,10,10)					//默认颜色
#define DEF_TEXT_COLOR_EX				RGB(10,10,10)					//默认颜色

//属性名字
//#define SKIN_BUTTON_CRTEXTCOLOR			TEXT("按钮字体颜色")			// COLORREF 类型 （读写）
//#define SKIN_BUTTON_STRIMAGEPATH		TEXT("按钮背景图")				// CString 类型 （读写）

#define	WM_BT_LBUTTONDBLCLK				WM_USER+400

//////////////////////////////////////////////////////////////////////////

//按钮类属性
class CSkinButtonAttribute : public CSkinResourceManager
{
	friend class CSkinButton;

	//设置部分
public:
	COLORREF							m_crTextColor;					//字体颜色

	//GDI 对象
public:
	CSkinImage							m_ImageBack;					//按钮位图
	tagImageLoadInfo					LoadInfoPng;					//默认PNG按钮
	tagImageLoadInfo					LoadInfoCheck;					//默认选择

	//函数定义
protected:
	//构造函数
	CSkinButtonAttribute();
	//析构函数
	virtual ~CSkinButtonAttribute();

	//接口函数
public:
	//加载配置
	bool UpdateSkinResource(ISkinResource * pISkinResource);
};

//////////////////////////////////////////////////////////////////////////

//按钮类
class SKIN_CONTROL_CLASS CSkinButton : public CButton
{
	friend class CRgnButton;

	//变量定义
protected:
	bool								m_bShowTextFrame;				//字体边框
	bool								m_bExpand;						//是否拉伸
	bool								m_bHovering;					//盘旋标志
	HICON								m_hIcon;						//按钮图标
	COLORREF							m_crTextColor;					//字体颜色
	COLORREF							m_crTextFrameColor;				//字体颜色
	CSkinImage							m_ImageBack;					//按钮位图
	LOGFONT								m_BtnFont;						//按钮字休

	//接口指针
protected:
	IMouseEvent							* m_pIMouseEvent;				//鼠标事件

	//按钮类属性
public:
	static CSkinButtonAttribute			m_SkinAttribute;				//按钮属性
	

	//函数定义
public:
	//构造函数
	CSkinButton();
	//析构函数
	virtual ~CSkinButton();

	//重载函数
protected:
	//控件子类化
	virtual void PreSubclassWindow();
	//界面绘画函数
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//默认回调函数
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//功能函数
public:
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect);
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//获取鼠标回调接口
	IMouseEvent * GetMouseEventCallBack() { return m_pIMouseEvent; }
	//设置鼠标回调接口
	void SetMouseEventCallBack(IMouseEvent * pIMouseEvent) { m_pIMouseEvent=pIMouseEvent; };

	//设置函数
public:
	//调整位置
	bool FixButtonSize();
	//设置图标
	bool SetButtonIcon(HICON hIcon);
	//设置颜色
	bool SetTextColor(COLORREF crTextColor,COLORREF crTextFrameColor=DEF_TEXT_FRAME_COLOR,bool bShowFrame=false);
	//设置字体
	bool SetTextFont(CFont* pFont, COLORREF crTextColor=DEF_TEXT_COLOR,COLORREF crTextFrameColor=DEF_TEXT_FRAME_COLOR,bool bShowFrame=false);
	//加载位图
	bool SetButtonImage(LPCTSTR pszFileName, bool bExpandImage);
	//加载位图
	bool SetButtonImage(UINT uBitmapID, HINSTANCE hInstance, bool bExpandImage);
	//默认按钮
	void SetDefaultButton(bool bResetIcon=true, bool bResetColor=true, bool bResetImage=true);

	//查询函数
public:
	//获取图标
	HICON GetButtonIcon() { return m_hIcon; }
	//获取颜色
	COLORREF GetButtonColor() { return m_crTextColor; }
	//获取位图参数
	bool GetButtonLoadInfo(tagImageLoadInfo & LoadInfo);
	//获取属性接口
	static CSkinButtonAttribute * GetSkinAttribute() { return &m_SkinAttribute; }

	//消息函数
protected:
	//建立消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标移动
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//鼠标离开
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	//背景消息
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//双击事件
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinButton)
};

//////////////////////////////////////////////////////////////////////////

//区域按钮类
class SKIN_CONTROL_CLASS CRgnButton : public CButton
{
	//变量定义
protected:
	bool								m_bHovering;					//盘旋标志
	CRgn								m_ButtonRgn;					//窗口区域
	COLORREF							m_crTrans;						//透明颜色
	COLORREF							m_crTextColor;					//字体颜色
	CSkinImage							m_ImageBack;					//背景位图

	//接口指针
protected:
	IMouseEvent							* m_pIMouseEvent;				//鼠标事件

	//函数定义
public:
	//构造函数
	CRgnButton();
	//析构函数
	virtual ~CRgnButton();

	//重载函数
protected:
	//控件子类化
	virtual void PreSubclassWindow();
	//界面绘画函数
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//默认回调函数
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//功能函数
public:
	//加载位图
	bool LoadRgnImage(LPCTSTR pszFileName, COLORREF crTrans);
	//加载位图
	bool LoadRgnImage(HINSTANCE hInstance, UINT uBitmapID, COLORREF crTrans);
	//获取鼠标回调接口
	IMouseEvent * GetMouseEventCallBack() { return m_pIMouseEvent; }
	//设置鼠标回调接口
	void SetMouseEventCallBack(IMouseEvent * pIMouseEvent) { m_pIMouseEvent=pIMouseEvent; };

	//内部函数 
private:
	//创建区域
	bool CreateControlRgn(COLORREF crTrans);

	//消息函数
protected:
	//建立消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//背景消息
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//鼠标移动
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//鼠标离开
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CRgnButton)
};


// CZLButton1 类支持PNG格式按钮，支持从资源加载图片以及从外部文件加载图片
class SKIN_CONTROL_CLASS CPngButton : public CButton
{
	//成员变量
private:
	CDC										m_dcBk;							//背景DC
	CDC										m_dcStd;						//标准
	CDC										m_dcStdP;						//标准按下
	CDC										m_dcStdH;						//标准盘旋
	CDC										m_dcAlt;						//交替
	CDC										m_dcAltP;						//交替按下
	CDC										m_dcAltH;						//交替判断
	CDC										m_dcGS;							//灰色按钮
	CDC*									m_pCurBtn;						//当前按钮

	//成员变量
protected:
	BOOL									m_bHaveAltImage;				//切换图
	BOOL									m_bHaveBitmaps;					//位图
	BOOL									m_bIsDisabled;					//关闭
	BOOL									m_bIsToggle;					//开关
	BOOL									m_bIsHovering;					//鼠标盘旋
	BOOL									m_bIsTracking;					//鼠标跟踪
	int										m_nCurType;						//当前类型
	CGdiPlusBitmapResource					m_pAltImage;					//切换图
	CGdiPlusBitmapResource					m_pStdImage;					//标准图
	CString									m_tooltext;						//提示信息
	CToolTipCtrl							m_pToolTip;						//提示助手

	//函数定义
public:
	//构造函数
	CPngButton();
	//析构函数
	virtual ~CPngButton();

	//函数定义
public:
	//枚举类型
	enum	{
				STD_TYPE	= 0,		//标准类型
				ALT_TYPE,				//交替类型
				DIS_TYPE				//无效类型
			};

	//设置按钮类型[0为标准类型,1为切换类型,2为无效类型]
	void SetButtonType(int type);
	//从外部文件加载标准图片[传入路径文件名]
	BOOL LoadStdImage(LPCTSTR pFilePath,bool bFixSize=true); 
	//从外部文件加载切换图片[传入路径文件名]
	BOOL LoadAltImage(LPCTSTR pFilePath,bool bFixSize=true);
	//加载标准图片[资源ID,资源类型]
	BOOL LoadStdImage(UINT id, LPCTSTR pType);
	//加载切换图片[资源ID,资源类型]
	BOOL LoadAltImage(UINT id, LPCTSTR pType,HINSTANCE hInstance);
	//启用按钮[如果为false, 关闭按下状态和显示无效状态]
	void EnableButton(BOOL bEnable = TRUE);
	//设置开关模式[如果为TRUE,每次按下按钮后都在标准类型和切换类型中反复切换]
	void EnableToggle(BOOL bEnable = TRUE);
	//判断是否关闭[返回按钮是否关闭的状态,TRUE为关闭,FALSE为开启]
	BOOL IsDisabled(void);
	//绘制按钮
	void SetBkGnd(CDC* pDC);
	//设置按钮提示文字[提示文字,默认激活]
	void SetToolTipText(CString spText, BOOL bActivate = TRUE);
	//设置按钮提示文字[资源ID,默认激活]
	void SetToolTipText(UINT nId, BOOL bActivate = TRUE);
	//设置盘旋
	void SetHorizontal(bool ImagesAreLaidOutHorizontally = FALSE);
	//删除按钮提示
	void DeleteToolTip();
	//按钮宽度
	int GetWidth();
	//刷新按钮
	void UpdateBk();

	//功能函数
protected:
	//绘制背景
	inline void PaintBk(CDC* pDC);
	//绘制按钮
	inline void PaintBtn(CDC* pDC);
	//初始化提示
	void	InitToolTip();					
	//控件子类化
	virtual void PreSubclassWindow();
	//界面绘画函数
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//消息函数
protected:
	//控件颜色
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//擦除背景
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//鼠标移动
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//鼠标离开
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	//鼠标盘旋
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	//设置鼠标指针
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  
	//消息映射
	DECLARE_MESSAGE_MAP()
};
//////////////////////////////////////////////////////////////////////////
class  SKIN_CONTROL_CLASS CCheckButton:public CButton
{
protected:
	bool									m_bChecked;
	BOOL									m_bHovering;					//鼠标盘旋
	COLORREF								m_crTextColor;						//文本颜色
	COLORREF								m_crBackColor;					//背景颜色
	//CSkinImage								m_ImageBack;					//背景位图
	CPngImage								m_pngBack;						//北京PNG

public:
	static CSkinButtonAttribute				m_SkinAttribute;				//按钮属性

public:

	CCheckButton();
	virtual ~CCheckButton();

public: 

	//获取设置状态
	void SetCheck(int nCheck);
	int GetCheck();
	//设置颜色
	void SetTextColor(COLORREF crText, COLORREF crBack=DEF_TEXT_FRAME_COLOR);
	//加载位图
	bool SetCheckImage(LPCTSTR lpResName, HINSTANCE hInstance);
	//加载图片
	bool SetCheckImage(LPCTSTR pszFileName);

protected:
	//获取选中状态
	bool GetCheckStat() {return m_bChecked;}
	
protected:

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//控件子类化
	virtual void PreSubclassWindow();

	//建立消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标移动
	afx_msg void OnMouseMove(UINT, CPoint);
	//鼠标离开
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	//右键
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP();

};
class SKIN_CONTROL_CLASS CGdiButton:public CButton
{
	
	//变量定义
protected:
	bool								m_bShowTextFrame;				//字体边框
	bool								m_bExpand;						//是否拉伸
	bool								m_bHovering;					//盘旋标志
	HICON								m_hIcon;						//按钮图标
	COLORREF							m_crTextColor;					//字体颜色
	COLORREF							m_crTextFrameColor;				//字体颜色					//按钮位图
	CPngImage							m_pngBack;						//PNG按钮图
	LOGFONT								m_BtnFont;						//按钮字休
	CDC										m_dcBk;							//背景DC

	//接口指针
protected:
	IMouseEvent							* m_pIMouseEvent;				//鼠标事件

	//函数定义
public:
	//构造函数
	CGdiButton();
	//析构函数
	virtual ~CGdiButton();

	//重载函数
protected:
	//控件子类化
	virtual void PreSubclassWindow();
	//界面绘画函数
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//默认回调函数
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//功能函数
public:
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect);
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//设置函数
public:
	//调整位置
	bool FixButtonSize();
	//设置图标
	bool SetButtonIcon(HICON hIcon);
	//设置颜色
	bool SetTextColor(COLORREF crTextColor,COLORREF crTextFrameColor=DEF_TEXT_FRAME_COLOR,bool bShowFrame=false);
	//设置字体
	bool SetTextFont(CFont* pFont, COLORREF crTextColor=DEF_TEXT_COLOR,COLORREF crTextFrameColor=DEF_TEXT_FRAME_COLOR,bool bShowFrame=false);
	//加载位图
	bool SetButtonImage(LPCTSTR pszFileName, bool bExpandImage);
	//加载位图
	bool SetButtonImage(LPCTSTR lpResName, HINSTANCE hInstance, bool bExpandImage);
	//默认按钮
	void SetDefaultButton(bool bResetIcon=true, bool bResetColor=true, bool bResetImage=true);

	//获取鼠标回调接口
	IMouseEvent * GetMouseEventCallBack() { return m_pIMouseEvent; }
	//设置鼠标回调接口
	void SetMouseEventCallBack(IMouseEvent * pIMouseEvent) { m_pIMouseEvent=pIMouseEvent; };


	//查询函数
public:
	//获取图标
	HICON GetButtonIcon() { return m_hIcon; }
	//获取颜色
	COLORREF GetButtonColor() { return m_crTextColor; }
	//绘制背景
	inline void PaintBk(CDC* pDC);
	//绘制按钮
	void SetBkGnd(CDC* pDC);

	//消息函数
protected:
	//建立消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标移动
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//鼠标离开
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	//背景消息
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//双击事件
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);


	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CGdiButton)
};
#endif
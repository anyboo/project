#ifndef SERVER_ITEM_VIEW_HEAD_FILE
#define SERVER_ITEM_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//列表数据类型
enum enItemGenre
{
	ItemGenre_Type,						//游戏种类
	ItemGenre_Kind,						//游戏类型
	ItemGenre_Station,					//游戏站点
	ItemGenre_Server,					//游戏房间
	ItemGenre_Inside,					//内部信息
};

//房间状态
enum enServerState
{
	ServerState_Normal,					//普通状态
	ServerState_InUse,					//正在使用
	ServerState_EverUse,				//曾经使用
};

//内部结构
struct tagGameInside
{
	DWORD								dwSortID;						//排序号码
	int									iImageIndex;					//资源索引
	char								szDisplayName[64];				//显示名字
};

//图标索引
#define IND_ROOT						0								//顶项索引
#define IND_TYPE						1								//类型索引
#define IND_STATION						2								//站点索引
#define IND_KIND_NODOWN					3								//没下载索引
#define IND_KIND_UNKNOW					4								//未知类型索引
#define IND_SERVER_NORNAL				5								//普通房间索引
#define IND_SERVER_CURRENT				6								//当前登录索引
#define IND_SERVER_BEENIN				7								//曾经登录索引
#define IND_SERVER_LIKE					8								//喜爱房间索引
#define IND_SERVER_DISABLE				9								//禁止房间索引
#define IND_INSINDE						10								//内部索引

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define TREE_TYPE_DEF                   0
#define TREE_TYPE_BIG                   1

//类说明
class CListItem;
class CListType;
class CListKind;
class CListServer;
class CListStation;
class CListInside;
typedef CArrayTemplate<CListItem *> CListItemArray;
typedef CArrayTemplate<CListType *> CListTypeArray;
typedef CArrayTemplate<CListKind *> CListKindArray;
typedef CArrayTemplate<CListStation *> CListStationArray;
typedef CArrayTemplate<CListServer *> CListServerArray;
typedef CArrayTemplate<CListInside *> CListInsideArray;

//////////////////////////////////////////////////////////////////////////
//接口定义

//列表回调接口
interface IServerListSink
{
	//展开列表
	virtual bool __cdecl ExpandListItem(CListItem * pListItem)=NULL;
	//更新通知
	virtual void __cdecl OnListItemUpdate(CListItem * pListItem)=NULL;
	//插入通知
	virtual void __cdecl OnListItemInserted(CListItem * pListItem)=NULL;
};

//树列表接口
interface ITreeCtrlSink
{
	//左键单击
	virtual void __cdecl OnTreeLeftClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)=NULL;
	//右键单击
	virtual void __cdecl OnTreeRightClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)=NULL;
	//左键双击
	virtual void __cdecl OnTreeLeftDBClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)=NULL;
	//右键双击
	virtual void __cdecl OnTreeRightDBClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)=NULL;
	//选择改变
	virtual void __cdecl OnTreeSelectChanged(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)=NULL;
	//子项展开
	virtual void __cdecl OnTreeItemexpanded(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//服务器项
class CListItem
{
	friend class CServerListManager;

	//变量定义
protected:
	DWORD_PTR							m_dwData;					//绑定数据
    CListItem							* m_pParentItem;			//父项指针
	CListItemArray						m_ListItemArray;			//子项数组//?有用到吗
	const enItemGenre					m_ItemGenre;				//子项类型

	//静态变量
protected:
	static IServerListSink				* m_pIServerListSink;		//回调接口	

	//函数定义
protected:
	//构造函数
	CListItem(CListItem * pParentItem, enItemGenre ItemGenre) : m_ItemGenre(ItemGenre)
	{
		m_dwData=0L;
		m_pParentItem=pParentItem;
		if (m_pParentItem!=NULL) m_pParentItem->m_ListItemArray.Add(this);
	}
	//析构函数
	virtual ~CListItem();

	//功能函数
public:
	//获取类型
	enItemGenre GetItemGenre() { return m_ItemGenre; }
	//获取父项
	CListItem * GetParentItem() { return m_pParentItem; }
	//获取绑定数
	DWORD_PTR GetItemData() { return m_dwData; }
	//设置绑定数
	void SetItemData(DWORD_PTR dwData) { m_dwData=dwData; } 
	//获取子项数
	INT_PTR GetItemCount() { return m_ListItemArray.GetCount(); }
	//枚举子项
	CListItem * EnumChildItem(INT_PTR nIndex);

	//查找函数
public:
	//查找子项
	CListType * SearchTypeItem(WORD wTypeID);
	//查找子项
	CListKind * SearchKindItem(WORD wKindID);
	//查找子项
	CListStation * SearchStationItem(WORD wKindID, WORD wStationID, bool bDeepSearch);
	//查找子项
	CListServer * SearchServerItem(WORD wKindID, WORD wServerID, bool bDeepSearch);
};

//////////////////////////////////////////////////////////////////////////

//游戏类型
class CListType : public CListItem
{
	//变量定义
public:
	tagGameType						m_GameType;					//信息结构

	//函数定义
public:
	//构造函数
	CListType(CListItem * pParentItem) : CListItem(pParentItem,ItemGenre_Type)
	{
		memset(&m_GameType,0,sizeof(m_GameType));
	}
	//获取数据
	tagGameType * GetItemInfo() 
	{ 
		return & m_GameType; 
	}
};

//////////////////////////////////////////////////////////////////////////

//游戏类型
class CListKind : public CListItem
{
	//变量定义
public:
	bool							m_bInstall;					//安装标志
	tagGameKind						m_GameKind;					//信息结构

	//函数定义
public:
	//构造函数
	CListKind(CListItem * pParentItem) : CListItem(pParentItem,ItemGenre_Kind)
	{
		m_bInstall=false;
		memset(&m_GameKind,0,sizeof(m_GameKind));
	}
	//获取数据
	tagGameKind * GetItemInfo() 
	{ 
		return & m_GameKind; 
	}
};

//////////////////////////////////////////////////////////////////////////

//游戏站点
class CListStation : public CListItem
{
	//变量定义
public:
	CListKind						* m_pListKind;				//类型信息
	tagGameStation					m_GameStation;				//信息结构

	//函数定义
public:
	//构造函数
	CListStation(CListItem * pParentItem, CListKind * pListKind) : CListItem(pParentItem,ItemGenre_Station)
	{
		ASSERT(pListKind!=NULL);
		m_pListKind=pListKind;
		memset(&m_GameStation,0,sizeof(m_GameStation));
	}
	//获取数据
	tagGameStation * GetItemInfo() 
	{ 
		return & m_GameStation; 
	}
};

//////////////////////////////////////////////////////////////////////////

//游戏房间
class CListServer : public CListItem
{
	//变量定义
public:
	CListKind						* m_pListKind;				//类型信息
	tagGameServer					m_GameServer;				//信息结构

	//函数定义
public:
	//构造函数
	CListServer(CListItem * pParentItem, CListKind * pListKind) : CListItem(pParentItem,ItemGenre_Server)
	{
		ASSERT(pListKind!=NULL);
		m_pListKind=pListKind;
		memset(&m_GameServer,0,sizeof(m_GameServer));
	}
	//获取数据
	tagGameServer * GetItemInfo()
	{ 
		return & m_GameServer; 
	}
	//获取类型
	CListKind * GetListKind() { return m_pListKind; }
};

//////////////////////////////////////////////////////////////////////////

//内部数据
class CListInside : public CListItem
{
	//变量定义
public:
	tagGameInside					m_GameInside;				//信息结构

	//函数定义
public:
	//构造函数
	CListInside(CListItem * pParentItem) : CListItem(pParentItem,ItemGenre_Inside)
	{
		memset(&m_GameInside,0,sizeof(m_GameInside));
	}
	//获取数据
	tagGameInside * GetItemInfo() 
	{ 
		return & m_GameInside; 
	}
};

//////////////////////////////////////////////////////////////////////////

//房间列表管理类
class CServerListManager
{
	//变量定义
protected:
	CListTypeArray					m_PtrArrayType;					//类型指针
	CListKindArray					m_PtrArrayKind;					//游戏指针
	CListStationArray				m_PtrArrayStation;				//站点指针
	CListServerArray				m_PtrArrayServer;				//房间指针
	CListInsideArray				m_PtrArrayInside;				//数据指针

	//接口指针
protected:
	IServerListSink					* m_pIServerListSink;			//回调接口	

	//函数定义
public:
	//构造函数
	CServerListManager();
	//析构函数
	virtual ~CServerListManager();

	//功能函数
public:
	//展开列表
	bool ExpandListItem(CListItem * pListItem);

	bool ChangeTreeType(bool bType);

	//功能接口
public:
	//枚举类型项
	CListType * EnumTypeItem(INT_PTR nIndex);
	//枚举游戏项
	CListKind * EnumKindItem(INT_PTR nIndex);
	//枚举站点项
	CListStation * EnumStationItem(INT_PTR nIndex);
	//枚举房间项
	CListServer * EnumServerItem(INT_PTR nIndex);
	//枚举内部项
	CListInside * EnumInsideItem(INT_PTR nIndex);
	//初始化函数
	bool InitServerListManager(IServerListSink * pIServerListSink);

	//插入接口
public:
	//插入子项
	bool InsertTypeItem(tagGameType GameType[], WORD wCount);
	//插入子项
	bool InsertKindItem(tagGameKind GameKind[], WORD wCount);
	//插入子项
	bool InsertStationItem(tagGameStation GameStation[], WORD wCount);
	//插入子项
	bool InsertServerItem(tagGameServer GameServer[], WORD wCount);
	//插入子项
	bool InsertInsideItem(tagGameInside GameInside[], WORD wCount);

	//查找接口
public:
	//查找子项
	CListType * SearchTypeItem(WORD wTypeID);
	//查找子项
	CListKind * SearchKindItem(WORD wKindID);
	//查找子项
	CListStation * SearchStationItem(WORD wKindID, WORD wStationID);
	//查找子项
	CListServer * SearchServerItem(WORD wKindID, WORD wServerID);
	// 获取游戏种类管理指针（种类：斗地主、梭哈等等）
	CListKindArray *GetGameKinkArry()const { return (CListKindArray*)&m_PtrArrayKind; }

	//人数函数
public:
	//更新类型
	bool UpdateGameKind(WORD wKindID);
	//更新总数
	bool UpdateGameOnLineCount(DWORD dwOnLineCount);
	//类型人数
	bool UpdateGameKindOnLine(WORD wKindID, DWORD dwOnLineCount);
	//房间人数
	bool UpdateGameServerOnLine(CListServer * pListServer, DWORD dwOnLineCount);
};

//////////////////////////////////////////////////////////////////////////

//游戏资源信息
struct tagGameResourceInfo
{
	WORD								wKindID;						//类型标识
	DWORD								dwImageIndex;					//资源索引
};

//数组说明
typedef CArrayTemplate<tagGameResourceInfo> CGameResourceArray;

/////////////////////////////////////////////////////////////////////////////
// cTree window
extern void GradientFillRect( CDC *pDC, CRect &rect, COLORREF col_from, COLORREF col_to, bool vert_grad );
class cTree : public CTreeCtrl
{
// Construction
public:
	cTree();
public:
	void SetDefaultCursor() ;
public:
   
	enum BkMode { BK_MODE_BMP = 0, BK_MODE_GRADIENT, BK_MODE_FILL };
	HCURSOR cursor_hand ;
	HCURSOR cursor_arr	;
	HCURSOR cursor_no	;
	bool	isCursorArrow;
	protected:
	CImageList*	m_pDragImage;
	BOOL		m_bLDragging;
	HTREEITEM	m_hitemDrag,m_hitemDrop;

	CRect rtMouseMove;
	CRect rtSelete;

	BYTE MouseMoveIndex;
	BYTE LBtnDownIndex;

	COLORREF  m_ColorBk;

private:
	COLORREF m_gradient_bkgd_from;		// Gradient variables
	COLORREF m_gradient_bkgd_to;
	COLORREF m_gradient_bkgd_sel;
	bool     m_gradient_horz;			// horz or vertical gradient

	BkMode  m_bkgd_mode;				// Current Background mode
	CBitmap m_bmp_back_ground;			// Background bitmap image
	CBitmap m_bmp_tree_closed;			// Tree open image (marker)
	CBitmap m_bmp_tree_open;			// Tree closed image (marker)
	bool    m_bmp_tiled_mode;			// Tile background image

	CRect m_rect;						// The client rect when drawing
	int   m_h_offset;					// 0... -x (scroll offset)
	int   m_h_size;						// width of unclipped window
	int   m_v_offset;					// 0... -y (scroll offset)
	int   m_v_size;						// height of unclipped window

	HICON m_icon;

// Implementation
public:
	COLORREF memDC_bgColor_bitmap;
	bool	 isImageTiled;

	CString		RemoveBuddyCountFromName(CString name);
	int			GetChildCountInGroup(CString group_name);
	HTREEITEM   GetCurrentTreeItem();
		
	bool		DeleteChild(CString buddy_name);
	HTREEITEM	MoveChildItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter);
	HTREEITEM	GetBuddyTreeItem(CString buddy_name);
	HTREEITEM	AddChild(CString buddy_name, CString group_name);
	bool		DeleteGroup(CString group_name);
	HTREEITEM	GetGroupTreeItem( CString  grp_name);
	HTREEITEM	AddGroup(const CString & group_name);
	virtual		~cTree();

	BOOL SetBkImage(UINT nIDResource) ;
	BOOL SetBkImage(LPCTSTR lpszResourceName) ;
	void SetSkinBkColor(COLORREF color);

	void DrawBackGround( CDC* pDC );	// 画背景
	void DrawItems( CDC* pDC );  // 画单项
	virtual void  SetItemIcon( HICON icon ) { m_icon = icon; }; // Overridable
	virtual HICON GetItemIcon( HTREEITEM item ){return m_icon;} // Overridable
	void SetBkMode( BkMode bkgd_mode ) { m_bkgd_mode = bkgd_mode; }

	// Generated message map functions
protected:
	
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

protected:
	CPalette m_pal;
	CBitmap m_bitmap;
	int m_cxBitmap, m_cyBitmap ;
};

/////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//服务器列表视图
class CServerItemView : public cTree, public IServerListSink
{
	//配置变量
protected:
	bool								m_bShowOnLineCount;				//显示人数

	//变量信息
protected:
	CImageList							m_ImageList;					//图片资源
	CGameResourceArray					m_GameResourceArray;			//资源数组

	//接口变量
protected:
	ITreeCtrlSink						* m_pITreeCtrlSink;				//回调接口

public:
	CBitmap	                            m_BmpScrollBar	;
	CFont			m_Font14;		//根节点字体
	CFont			m_Font12;		//子节点字体
	
	bool            m_bTreeType;

	CRect            rtMouseMove;

	//函数定义
public:
	//构造函数
	CServerItemView();
	//析构函数
	virtual ~CServerItemView();

	//功能函数
public:
	//显示人数
	void ShowOnLineCount(bool bShowOnLineCount);
	//配置函数
	bool InitServerItemView(ITreeCtrlSink * pITreeCtrlSink);

	bool ChangeTreeType(bool bType);

	//通知接口
public:
	//展开列表
	virtual bool __cdecl ExpandListItem(CListItem * pListItem);
	//更新通知
	virtual void __cdecl OnListItemUpdate(CListItem * pListItem);
	//插入通知
	virtual void __cdecl OnListItemInserted(CListItem * pListItem);



	//辅助函数
private:
	//按钮测试
	HTREEITEM GetCurrentTreeItem();

	//图标函数
private:
	//获取图标
	DWORD GetGameImageIndex(CListKind * pListKind);
	//获取图标
	DWORD GetGameImageIndex(CListServer * pListServer);
	//获取图标
	DWORD GetGameImageIndex(LPCTSTR pszProcess, WORD wKindID);

	//标题函数
private:
	//获取标题
	LPCTSTR GetGameItemTitle(CListKind * pListKind, LPTSTR pszTitle, WORD wBufferSize);
	//获取标题
	LPCTSTR GetGameItemTitle(CListServer * pListServer, LPTSTR pszTitle, WORD wBufferSize);

	//消息映射
protected:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//左击列表
	afx_msg void OnNMClick(NMHDR * pNMHDR, LRESULT * pResult);
	//右击列表
	afx_msg void OnNMRclick(NMHDR * pNMHDR, LRESULT * pResult);
	//左键双击
	afx_msg void OnNMDblclk(NMHDR * pNMHDR, LRESULT * pResult);
	//右键双击
	afx_msg void OnNMRdblclk(NMHDR * pNMHDR, LRESULT * pResult);
	//选择改变
	afx_msg void OnTvnSelchanged(NMHDR * pNMHDR, LRESULT * pResult);
	//列表展开
	afx_msg void OnTvnItemexpanded(NMHDR * pNMHDR, LRESULT * pResult);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);


	//afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	//afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////

#endif
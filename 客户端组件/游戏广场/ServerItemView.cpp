#include "Stdafx.h"
#include "Resource.h"
#include "ServerItemView.h"
#include ".\serveritemview.h"

//宏定义
#define WN_SET_LINE_COLOR				(TV_FIRST+40)		//设置颜色消息
#define DEF_FRAME_COLOR                 RGB(0,0,200)

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CServerItemView, cTree)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
	ON_NOTIFY_REFLECT(NM_RDBLCLK, OnNMRdblclk)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnTvnItemexpanded)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_VSCROLL()	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//静态变量
IServerListSink		* CListItem::m_pIServerListSink=NULL;		//回调接口	

//析构函数
CListItem::~CListItem()
{
	m_dwData=0;
	m_pParentItem=NULL;
}

//枚举子项
CListItem * CListItem::EnumChildItem(INT_PTR nIndex)
{
	if (nIndex>=m_ListItemArray.GetCount()) return NULL;
	return m_ListItemArray[nIndex];
}

//查找子项
CListType * CListItem::SearchTypeItem(WORD wTypeID)
{
	for (INT_PTR i=0;i<m_ListItemArray.GetCount();i++)
	{
		CListItem * pListItem=m_ListItemArray[i];
		if (pListItem->GetItemGenre()==ItemGenre_Type)
		{
			CListType * pListType=(CListType *)pListItem;
			tagGameType * pGameType=pListType->GetItemInfo();
			if (pGameType->wTypeID==wTypeID) return pListType;
		}
	}

	return NULL;
}

//查找子项
CListKind * CListItem::SearchKindItem(WORD wKindID)
{
	for (INT_PTR i=0;i<m_ListItemArray.GetCount();i++)
	{
		CListItem * pListItem=m_ListItemArray[i];
		if (pListItem->GetItemGenre()==ItemGenre_Kind)
		{
			CListKind * pListKind=(CListKind *)pListItem;
			const tagGameKind * pGameKind=pListKind->GetItemInfo();
			if (pGameKind->wKindID==wKindID) return pListKind;
		}
	}

	return NULL;
}

//查找子项
CListStation * CListItem::SearchStationItem(WORD wKindID, WORD wStationID, bool bDeepSearch)
{
	//本层搜索
	for (INT_PTR i=0;i<m_ListItemArray.GetCount();i++)
	{
		CListItem * pListItem=m_ListItemArray[i];
		if (pListItem->GetItemGenre()==ItemGenre_Station)
		{
			CListStation * pListStation=(CListStation *)pListItem;
			const tagGameStation * pGameStation=pListStation->GetItemInfo();
			if ((pGameStation->wStationID==wStationID)&&(pGameStation->wKindID==wKindID)) return pListStation;
		}
	}

	//深度搜索
	if (bDeepSearch)
	{
		for (INT_PTR i=0;i<m_ListItemArray.GetCount();i++)
		{
			CListItem * pItemBase=m_ListItemArray[i];
			CListStation * pListStation=pItemBase->SearchStationItem(wKindID,wStationID,bDeepSearch);
			if (pListStation!=NULL) return pListStation;
		}
	}

	return NULL;
}

//查找子项
CListServer * CListItem::SearchServerItem(WORD wKindID, WORD wServerID, bool bDeepSearch)
{
	//本层搜索
	for (INT_PTR i=0;i<m_ListItemArray.GetCount();i++)
	{
		CListItem * pListItem=m_ListItemArray[i];
		if (pListItem->GetItemGenre()==ItemGenre_Server)
		{
			CListServer * pListServer=(CListServer *)pListItem;
			const tagGameServer * pGameServer=pListServer->GetItemInfo();
			if ((pGameServer->wServerID==wServerID)&&(pGameServer->wKindID==wKindID)) return pListServer;
		}
	}

	//深度搜索
	if (bDeepSearch)
	{
		for (INT_PTR i=0;i<m_ListItemArray.GetCount();i++)
		{
			CListItem * pItemBase=m_ListItemArray[i];
			CListServer * pListServer=pItemBase->SearchServerItem(wKindID,wServerID,bDeepSearch);
			if (pListServer!=NULL) return pListServer;
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CServerListManager::CServerListManager()
{
	m_pIServerListSink=NULL;
}

//析构函数
CServerListManager::~CServerListManager()
{
	INT_PTR nCount=m_PtrArrayType.GetCount();
	int i;
	for (i=0;i<nCount;i++) delete m_PtrArrayType[i];
	m_PtrArrayType.RemoveAll();

	nCount=m_PtrArrayKind.GetCount();
	for (i=0;i<nCount;i++) delete m_PtrArrayKind[i];
	m_PtrArrayKind.RemoveAll();

	nCount=m_PtrArrayStation.GetCount();
	for (i=0;i<nCount;i++) delete m_PtrArrayStation[i];
	m_PtrArrayStation.RemoveAll();

	nCount=m_PtrArrayServer.GetCount();
	for (i=0;i<nCount;i++) delete m_PtrArrayServer[i];
	m_PtrArrayServer.RemoveAll();

	return;
}

//初始化函数
bool CServerListManager::InitServerListManager(IServerListSink * pIServerListSink)
{
	//设置变量
	m_pIServerListSink=pIServerListSink;
	CListItem::m_pIServerListSink=pIServerListSink;

	//插入数据
	tagGameInside GameInside;
	memset(&GameInside,0,sizeof(GameInside));
	lstrcpyn(GameInside.szDisplayName,szProductName,sizeof(GameInside.szDisplayName));
	InsertInsideItem(&GameInside,1);

	return true;
}

//展开列表
bool CServerListManager::ExpandListItem(CListItem * pListItem)
{
	ASSERT(m_pIServerListSink!=NULL);
	return m_pIServerListSink->ExpandListItem(pListItem);
}

//枚举类型
CListType * CServerListManager::EnumTypeItem(INT_PTR nIndex)
{
	if (nIndex>=m_PtrArrayType.GetCount()) return NULL;
	return m_PtrArrayType[nIndex];
}

//枚举游戏
CListKind * CServerListManager::EnumKindItem(INT_PTR nIndex)
{
	if (nIndex>=m_PtrArrayKind.GetCount()) return NULL;
	return m_PtrArrayKind[nIndex];
}

//枚举站点
CListStation * CServerListManager::EnumStationItem(INT_PTR nIndex)
{
	if (nIndex>=m_PtrArrayStation.GetCount()) return NULL;
	return m_PtrArrayStation[nIndex];
}

//枚举房间
CListServer * CServerListManager::EnumServerItem(INT_PTR nIndex)
{
	if (nIndex>=m_PtrArrayServer.GetCount()) return NULL;
	return m_PtrArrayServer[nIndex];
}

//枚举内部项
CListInside * CServerListManager::EnumInsideItem(INT_PTR nIndex)
{
	if (nIndex>=m_PtrArrayInside.GetCount()) return NULL;
	return m_PtrArrayInside[nIndex];
}

//插入子项
bool CServerListManager::InsertTypeItem(tagGameType GameType[], WORD wCount)
{
	//效验参数
	ASSERT(m_PtrArrayInside.GetCount()>0);
	if (wCount==0) return false;

	//变量定义
	CListType * pListType=NULL;
	CListInside * pRootListInside=m_PtrArrayInside[0];

	try
	{
		//变量定义
		CListType * pListTypeTemp=NULL;

		for (WORD i=0;i<wCount;i++)
		{
			//查找存在
			pListTypeTemp=pRootListInside->SearchTypeItem(GameType[i].wTypeID);
			if (pListTypeTemp!=NULL) continue;

			//插入操作
			pListType=new CListType(pRootListInside);
			CopyMemory(pListType->GetItemInfo(),&GameType[i],sizeof(tagGameType));
			m_PtrArrayType.Add(pListType);
			m_pIServerListSink->OnListItemInserted(pListType);
		}
		return true;
	}
	catch (...) { SafeDelete(pListType); }

	return false;
}

///插入子项
bool CServerListManager::InsertKindItem(tagGameKind GameKind[], WORD wCount)
{
	CListKind * pListKind=NULL;
	try
	{
		//变量定义
		WORD wTypeID=0;
		CListType * pListType=NULL;
		CListKind * pListKindTemp=NULL;

		for (WORD i=0;i<wCount;i++)
		{
			//寻找父项
			if ((pListType==NULL)||(GameKind[i].wTypeID!=wTypeID))
			{
				wTypeID=GameKind[i].wTypeID;
				pListType=SearchTypeItem(wTypeID);
				ASSERT(pListType!=NULL);
				if (pListType==NULL) continue;
			}

			//查找存在
			pListKindTemp=pListType->SearchKindItem(GameKind[i].wKindID);
			if (pListKindTemp!=NULL) continue;

			//插入操作
			CWinFileInfo WinFileInfo;
			pListKind=new CListKind(pListType);
			CopyMemory(pListKind->GetItemInfo(),&GameKind[i],sizeof(tagGameKind));
			pListKind->m_bInstall=WinFileInfo.OpenWinFile(GameKind[i].szProcessName);
			m_PtrArrayKind.Add(pListKind);
			m_pIServerListSink->OnListItemInserted(pListKind);
		}
		return true;
	}
	catch (...) { SafeDelete(pListKind); }
	
	return false;
}

//插入子项
bool CServerListManager::InsertStationItem(tagGameStation GameStation[], WORD wCount)
{
	CListStation * pListStation=NULL;
	try
	{
		//变量定义
		WORD wKindID=0,wStationID=0;
		CListKind * pListKind=NULL;
		CListItem * pListParent=NULL;
		CListStation * pListStationJoin=NULL;
		CListStation * pListStationTemp=NULL;

		for (WORD i=0;i<wCount;i++)
		{
			//寻找种类
			if ((pListKind==NULL)||(GameStation[i].wKindID!=wKindID))
			{
				pListKind=SearchKindItem(GameStation[i].wKindID);
				ASSERT(pListKind!=NULL);
				if (pListKind==NULL) continue;
				wKindID=GameStation[i].wKindID;
				pListParent=pListKind;
			}

			//查找存在
			pListStationTemp=pListKind->SearchStationItem(GameStation[i].wKindID,GameStation[i].wStationID,true);
			if (pListStationTemp!=NULL) continue;

			//寻找站点
			if (GameStation[i].wJoinID!=0)
			{
				if ((pListStationJoin==NULL)||(GameStation[i].wJoinID!=wStationID))
				{
					pListStationJoin=pListKind->SearchStationItem(GameStation[i].wKindID,GameStation[i].wJoinID,true);
					ASSERT(pListStationJoin!=NULL);
					if (pListStationJoin==NULL) continue;
					wStationID=GameStation[i].wJoinID;
				}
				pListParent=pListStationJoin;
			}

			//插入操作
			pListStation=new CListStation(pListParent,pListKind);
			CopyMemory(pListStation->GetItemInfo(),&GameStation[i],sizeof(tagGameStation));
			m_PtrArrayStation.Add(pListStation);
			m_pIServerListSink->OnListItemInserted(pListStation);
		}
		return true;
	}
	catch (...) { SafeDelete(pListStation); }

	return false;
}

//插入子项
bool CServerListManager::InsertServerItem(tagGameServer GameServer[], WORD wCount)
{
	CListServer * pListServer=NULL;
	try
	{
		//变量定义
		WORD wKindID=0,wStationID=0;
		CListKind * pListKind=NULL;
		CListItem * pListParent=NULL;
		CListStation * pListStation=NULL;
		CListServer * pListServerTemp=NULL;

		for (WORD i=0;i<wCount;i++)
		{
			//寻找种类
			if ((pListKind==NULL)||(GameServer[i].wKindID!=wKindID))
			{
				pListKind=SearchKindItem(GameServer[i].wKindID);
				ASSERT(pListKind!=NULL);
				if (pListKind==NULL) continue;
				wKindID=GameServer[i].wKindID;
				pListParent=pListKind;
			}

			//查找存在
			pListServerTemp=pListKind->SearchServerItem(GameServer[i].wKindID,GameServer[i].wServerID,true);
			if (pListServerTemp!=NULL)
			{
				CopyMemory(pListServerTemp->GetItemInfo(),&GameServer[i],sizeof(tagGameServer));
				continue;
			}

			//寻找站点
			if (GameServer[i].wStationID!=0)
			{
				if ((pListStation==NULL)||(GameServer[i].wStationID!=wStationID))
				{
					pListStation=pListKind->SearchStationItem(GameServer[i].wKindID,GameServer[i].wStationID,true);
					if (pListStation==NULL) continue;
					wStationID=GameServer[i].wStationID;
				}
				pListParent=pListStation;
			}

			//插入操作
			pListServer=new CListServer(pListParent,pListKind);
			CopyMemory(pListServer->GetItemInfo(),&GameServer[i],sizeof(tagGameServer));
			m_PtrArrayServer.Add(pListServer);
			m_pIServerListSink->OnListItemInserted(pListServer);
		}
		return true;
	}
	catch (...) { SafeDelete(pListServer); }

	return false;
}

//插入子项
bool CServerListManager::InsertInsideItem(tagGameInside GameInside[], WORD wCount)
{
	CListInside * pListInside=NULL;
	try
	{
		for (WORD i=0;i<wCount;i++)
		{
			pListInside=new CListInside(NULL);
			CopyMemory(pListInside->GetItemInfo(),&GameInside[i],sizeof(tagGameInside));
			m_PtrArrayInside.Add(pListInside);
			m_pIServerListSink->OnListItemInserted(pListInside);
		}
		return true;
	}
	catch (...) { SafeDelete(pListInside); }

	return false;
}

//查找子项
CListType * CServerListManager::SearchTypeItem(WORD wTypeID)
{
	for (INT_PTR i=0;i<m_PtrArrayType.GetCount();i++)
	{
		CListType * pListType=m_PtrArrayType[i];
		tagGameType * pGameType=pListType->GetItemInfo();
		if (pGameType->wTypeID==wTypeID) return pListType;
	}
	return NULL;
}

//查找子项
CListKind * CServerListManager::SearchKindItem(WORD wKindID)
{
	for (INT_PTR i=0;i<m_PtrArrayKind.GetCount();i++)
	{
		CListKind * pListKind=m_PtrArrayKind[i];
		tagGameKind * pGameKind=pListKind->GetItemInfo();
		if (pGameKind->wKindID==wKindID)
		{
			return pListKind;
		}
	}
	return NULL;
}

//查找子项
CListStation * CServerListManager::SearchStationItem(WORD wKindID, WORD wStationID)
{
	for (INT_PTR i=0;i<m_PtrArrayStation.GetCount();i++)
	{
		CListStation * pListStation=m_PtrArrayStation[i];
		tagGameStation * pGameStation=pListStation->GetItemInfo();
		if ((pGameStation->wStationID==wStationID)&&(pGameStation->wKindID==wKindID)) return pListStation;
	}
	return NULL;
}

//查找子项
CListServer * CServerListManager::SearchServerItem(WORD wKindID, WORD wServerID)
{
	for (INT_PTR i=0;i<m_PtrArrayServer.GetCount();i++)
	{
		CListServer * pListServer=m_PtrArrayServer[i];
		tagGameServer * pGameServer=pListServer->GetItemInfo();
		if ((pGameServer->wServerID==wServerID)&&(pGameServer->wKindID==wKindID)) return pListServer;
	}
	return NULL;
}

//更新类型
bool CServerListManager::UpdateGameKind(WORD wKindID)
{
	CListKind * pListKind=SearchKindItem(wKindID);
	if (pListKind!=NULL)
	{
		CWinFileInfo WinFileInfo;
		tagGameKind * pGameKind=pListKind->GetItemInfo();
		pListKind->m_bInstall=WinFileInfo.OpenWinFile(pGameKind->szProcessName);
		m_pIServerListSink->OnListItemUpdate(pListKind);
		return true;
	}

	return false;
}
//更新总数
bool CServerListManager::UpdateGameOnLineCount(DWORD dwOnLineCount)
{
	if (m_PtrArrayInside.GetCount()>0)
	{
		CListInside * pListInside=m_PtrArrayInside[0];
		tagGameInside * pGameInside=pListInside->GetItemInfo();
		_snprintf(pGameInside->szDisplayName,sizeof(pGameInside->szDisplayName),TEXT("%s"),szProductName);
		m_pIServerListSink->OnListItemUpdate(pListInside);
		return true;
	}

	return false;
}

//类型人数
bool CServerListManager::UpdateGameKindOnLine(WORD wKindID, DWORD dwOnLineCount)
{
	//寻找类型
	CListKind * pListKind=SearchKindItem(wKindID);
	if (pListKind!=NULL)
	{
		tagGameKind * pGameKind=pListKind->GetItemInfo();
		pGameKind->dwOnLineCount=dwOnLineCount;
		m_pIServerListSink->OnListItemUpdate(pListKind);
		return true;
	}

	return false;
}

//房间人数
bool CServerListManager::UpdateGameServerOnLine(CListServer * pListServer, DWORD dwOnLineCount)
{
	//效验参数
	ASSERT(pListServer!=NULL);
	if (pListServer==NULL) return false;

	//更新信息
	tagGameServer * pGameServer=pListServer->GetItemInfo();
	pGameServer->dwOnLineCount=dwOnLineCount;
	m_pIServerListSink->OnListItemUpdate(pListServer);

	return true;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CServerItemView::CServerItemView()
{
	m_bShowOnLineCount=false;

	m_bTreeType=TREE_TYPE_DEF;

	m_Font12.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,			   // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		PROOF_QUALITY,             // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体") );              // lpszFacename

	m_Font14.CreateFont(
		13,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,			   // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		PROOF_QUALITY,             // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体") );              // lpszFacename
}

//析构函数
CServerItemView::~CServerItemView()
{
}

//显示人数
void CServerItemView::ShowOnLineCount(bool bShowOnLineCount)
{
	bShowOnLineCount=true;
	if (m_bShowOnLineCount!=bShowOnLineCount)
	{
		m_bShowOnLineCount=bShowOnLineCount;
	}

	return;
}

//配置函数
//配置函数
//配置函数
bool CServerItemView::InitServerItemView(ITreeCtrlSink * pITreeCtrlSink)
{	
	//设置控件
    SetItemHeight(20);
	SetTextColor(RGB(56,81,97));
	SetBkColor(RGB(248,247,247));
	ModifyStyle(0,TVS_HASBUTTONS);		
	SendMessage(WN_SET_LINE_COLOR,0,(LPARAM)RGB(72,79,63));
    
	//设置变量
	ASSERT(pITreeCtrlSink!=NULL);
	m_pITreeCtrlSink=pITreeCtrlSink;
    
	//加载图片
	if (m_ImageList.GetSafeHandle()==NULL)
	{
		CBitmap ServerImage;
		ServerImage.LoadBitmap(IDB_SERVER_LIST_IMAGE);
		m_ImageList.Create(18,18,ILC_COLOR16|ILC_MASK,0,0);
		m_ImageList.Add(&ServerImage,RGB(255,0,255));
		SetImageList(&m_ImageList,LVSIL_NORMAL);
	}   
    

	return true;
}




//展开列表
bool __cdecl CServerItemView::ExpandListItem(CListItem * pListItem)
{
	//效验参数
	ASSERT(pListItem!=NULL);

	//展开列表
	HTREEITEM hTreeItem=(HTREEITEM)pListItem->GetItemData();
	if (hTreeItem!=NULL) 
	{
		Expand(hTreeItem,TVE_EXPAND);
		return true;
	}

	return false;
}

//更新通知
void __cdecl CServerItemView::OnListItemUpdate(CListItem * pListItem)
{
	//效验参数
	ASSERT(pListItem!=NULL);

	//更新显示
	switch (pListItem->GetItemGenre())
	{
	case ItemGenre_Kind:		//游戏类型
		{
			//变量定义
			CListKind * pListKind=(CListKind *)pListItem;

			//获取信息
			TCHAR szItemTitle[128]=TEXT("");
			DWORD dwImageIndex=GetGameImageIndex(pListKind);
			GetGameItemTitle(pListKind,szItemTitle,sizeof(szItemTitle));

			//更新树项
			HTREEITEM hTreeItem=(HTREEITEM)pListKind->GetItemData();
			SetItem(hTreeItem,TVIF_IMAGE|TVIF_TEXT|TVIF_SELECTEDIMAGE,szItemTitle,dwImageIndex,dwImageIndex,0,0,0);

			return;
		}
	case ItemGenre_Server:		//游戏房间
		{
			//变量定义
			CListServer * pListServer=(CListServer *)pListItem;
			tagGameServer * pGameServer=pListServer->GetItemInfo();

			//获取信息
			TCHAR szItemTitle[128]=TEXT("");
			GetGameItemTitle(pListServer,szItemTitle,sizeof(szItemTitle));

			//更新显示
			HTREEITEM hTreeItem=(HTREEITEM)pListServer->GetItemData();

			////去掉
			//CString strKineName;
			//strKineName.Format(("%s"),pListServer->GetListKind()->GetItemInfo()->szKindName);

			//TCHAR szItemTitleTemp[128]=TEXT("");

			//CopyMemory(szItemTitleTemp,szItemTitle+strKineName.GetLength(),sizeof(szItemTitleTemp)-strKineName.GetLength());

			SetItemText(hTreeItem,szItemTitle);

			//更新类型
			INT_PTR nIndex=0;
			CListItem * pListItemTemp=NULL;
			CListKind * pListKind=pListServer->GetListKind();
			tagGameKind * pGameKind=pListKind->GetItemInfo();
			pGameKind->dwOnLineCount=0L;
			do
			{
				pListItemTemp=pListKind->EnumChildItem(nIndex++);
				if (pListItemTemp==NULL) break;
				if (pListItemTemp->GetItemGenre()==ItemGenre_Server)
				{
					pListServer=(CListServer *)pListItemTemp;
					pGameKind->dwOnLineCount+=pListServer->GetItemInfo()->dwOnLineCount;
				}
			} while (true);

			//更新显示
			GetGameItemTitle(pListKind,szItemTitle,sizeof(szItemTitle));
			hTreeItem=(HTREEITEM)pListKind->GetItemData();


			


			SetItemText(hTreeItem,szItemTitle);

			return;
		}
	case ItemGenre_Inside:	//内部类型
		{
			//变量定义
			CListInside * pListInside=(CListInside *)pListItem;
			tagGameInside * pGameInside=pListInside->GetItemInfo();

			//消息处理
			HTREEITEM hTreeItem=(HTREEITEM)pListInside->GetItemData();
			SetItemText(hTreeItem,pGameInside->szDisplayName);

			return;
		}
	}

	return;
}

//插入通知
void __cdecl CServerItemView::OnListItemInserted(CListItem * pListItem)
{
	//效验参数
	ASSERT(pListItem!=NULL);

	//变量定义
	TV_INSERTSTRUCT InsertInf;
	memset(&InsertInf,0,sizeof(InsertInf));

	//设置变量
	InsertInf.item.cchTextMax=128;
	InsertInf.hInsertAfter=TVI_LAST;
	InsertInf.item.lParam=(LPARAM)pListItem;
	InsertInf.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;

	//寻找父项
	CListItem * pListParent=pListItem->GetParentItem();
	if (pListParent!=NULL) InsertInf.hParent=(HTREEITEM)pListParent->GetItemData();

	//构造数据
	switch (pListItem->GetItemGenre())
	{
	case ItemGenre_Type:		//游戏类型
		{
			//变量定义
			CListType * pListType=(CListType *)pListItem;
			tagGameType * pGameType=pListType->GetItemInfo();

			//构造数据
			InsertInf.item.iImage=pGameType->wTypeID;
			InsertInf.item.iSelectedImage=pGameType->wTypeID;
			InsertInf.item.pszText=pGameType->szTypeName;
			pListType->SetItemData((DWORD_PTR)InsertItem(&InsertInf));
			SetItemState((HTREEITEM)pListType->GetItemData(),TVIS_BOLD,TVIS_BOLD);		
			
			break;
		}
	case ItemGenre_Kind:		//游戏种类
		{
			//变量定义
			CListKind * pListKind=(CListKind *)pListItem;
			tagGameKind * pGameKind=pListKind->GetItemInfo();

			//获取信息
			TCHAR szItemTitle[128]=TEXT("");
			DWORD dwImageIndex=GetGameImageIndex(pListKind);
			GetGameItemTitle(pListKind,szItemTitle,sizeof(szItemTitle));

			//设置数据
			InsertInf.item.pszText=szItemTitle;
			InsertInf.item.iImage=dwImageIndex;
			InsertInf.item.iSelectedImage=dwImageIndex;
			pListKind->SetItemData((DWORD_PTR)InsertItem(&InsertInf));
		

			break;
		}
	case ItemGenre_Station:		//游戏站点
		{
			//变量定义
			CListStation * pListStation=(CListStation *)pListItem;
			tagGameStation * pGameStation=pListStation->GetItemInfo();

			//设置数据
			InsertInf.item.iImage=IND_STATION;
			InsertInf.item.iSelectedImage=IND_STATION;
			InsertInf.item.pszText=pGameStation->szStationName;
			pListStation->SetItemData((DWORD_PTR)InsertItem(&InsertInf));

			break;
		}
	case ItemGenre_Server:		//游戏房间
		{
			//变量定义
			CListServer * pListServer=(CListServer *)pListItem;
			tagGameServer * pGameServer=pListServer->GetItemInfo();

			//设置位置
			InsertInf.hInsertAfter=TVI_FIRST;

			//寻找子项
			if (InsertInf.hParent!=NULL)
			{
				//变量定义
				WORD wSortID=pGameServer->wServerID;
				HTREEITEM hTreeItem=GetChildItem(InsertInf.hParent);

				//搜索位置
				if (hTreeItem!=NULL)//找到上一级节点
				{
					do
					{
						//获取数据
						CListServer * pItemTemp=(CListServer *)GetItemData(hTreeItem);
						if (pItemTemp!=NULL)
						{
							//获取位置
							WORD wItemSortID=1;
							if (pItemTemp->GetItemGenre()==ItemGenre_Server)
							{
								wItemSortID=((CListServer *)pItemTemp)->GetItemInfo()->wServerID;
							}

							//位置判断
							if (wSortID>wItemSortID) InsertInf.hInsertAfter=hTreeItem;
							else break;
						}
						else InsertInf.hInsertAfter=hTreeItem;

						//切换树项
						hTreeItem=GetNextItem(hTreeItem,TVGN_NEXT);

					} while (hTreeItem!=NULL);
				}
			}

			//构造标题
			TCHAR szItemTitle[128]=TEXT("");
			DWORD dwImageIndex=GetGameImageIndex(pListServer);
			GetGameItemTitle(pListServer,szItemTitle,sizeof(szItemTitle));
			//设置数据
			InsertInf.item.pszText=szItemTitle;
			InsertInf.item.iImage=dwImageIndex;
			InsertInf.item.iSelectedImage=dwImageIndex;			
			pListServer->SetItemData((DWORD_PTR)InsertItem(&InsertInf));

			break;
		}
	case ItemGenre_Inside:		//内部数据
		{
			//插入分析
			CListInside * pListInside=(CListInside *)pListItem;
			tagGameInside * pGameInside=pListInside->GetItemInfo();

			//设置数据
			InsertInf.item.iImage=pGameInside->iImageIndex;
			InsertInf.item.iSelectedImage=pGameInside->iImageIndex;
			InsertInf.item.pszText=pGameInside->szDisplayName;
			pListInside->SetItemData((DWORD_PTR)InsertItem(&InsertInf));
			SetItemState((HTREEITEM)pListInside->GetItemData(),TVIS_BOLD,TVIS_BOLD);

			break;
		}
	}
	
	return;
}

//左击列表
void CServerItemView::OnNMClick(NMHDR * pNMHDR, LRESULT * pResult)
{
	//变量定义
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=GetCurrentTreeItem();

	//展开列表
	if (hTreeItem!=NULL)
	{
		//Select(hTreeItem,TVGN_CARET);
		pListItem=(CListItem *)GetItemData(hTreeItem);
	}

	//发送消息
	ASSERT(m_pITreeCtrlSink!=NULL);
	m_pITreeCtrlSink->OnTreeLeftClick(pListItem,hTreeItem,this);

	return;
}

void CServerItemView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//SetFocus();
	//变量定义
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=GetCurrentTreeItem();
	CRect rtClient;
	GetClientRect(rtClient);	

	
	//展开列表
	if (hTreeItem!=NULL)
	{
		int ItemH=GetItemHeight();
		LBtnDownIndex=point.y/ItemH;
		rtSelete.SetRect(0,point.y/ItemH*ItemH,rtClient.Width(),(point.y/ItemH+1)*ItemH);
		Select(hTreeItem,TVGN_ROOT);
		//SetFocus();
		pListItem=(CListItem *)GetItemData(hTreeItem);

		//效验参数
		ASSERT(pListItem!=NULL);

		//展开列表
		HTREEITEM hTreeItem=(HTREEITEM)pListItem->GetItemData();
		if (hTreeItem!=NULL) 
		{
			UINT   nState   =   GetItemState(hTreeItem,   TVIS_EXPANDED);   
			if   (0   !=   (nState   &   TVIS_EXPANDED))   
			{   
				//   已经展开   
				Expand(hTreeItem,TVE_COLLAPSE);
			}   
			else   
			{   
				//   没展开  
				Expand(hTreeItem,TVE_EXPAND);
			}			

			GetClientRect(rtClient);
			this->InvalidateRect(rtClient,false);			

		}

		ASSERT(m_pITreeCtrlSink!=NULL);
		m_pITreeCtrlSink->OnTreeLeftClick(pListItem,hTreeItem,this);
		return ;
	}
	
	

}

//左击列表
void CServerItemView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//变量定义
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=GetCurrentTreeItem();

	//展开列表
	if (hTreeItem!=NULL)
	{
		//Select(hTreeItem,TVGN_CARET);
		pListItem=(CListItem *)GetItemData(hTreeItem);
	}

	//发送消息
	ASSERT(m_pITreeCtrlSink!=NULL);
	m_pITreeCtrlSink->OnTreeLeftDBClick(pListItem,hTreeItem,this);
	return;
}
//右击列表
void CServerItemView::OnNMRclick(NMHDR * pNMHDR, LRESULT * pResult)
{
	//变量定义
	//CListItem * pListItem=NULL;
	//HTREEITEM hTreeItem=GetCurrentTreeItem();

	////选择列表
	//if (hTreeItem!=NULL)
	//{
	//	Select(hTreeItem,TVGN_CARET);
	//	pListItem=(CListItem *)GetItemData(hTreeItem);
	//}

	////发送消息
	//ASSERT(m_pITreeCtrlSink!=NULL);
	//m_pITreeCtrlSink->OnTreeRightClick(pListItem,hTreeItem,this);

	return;
}

//左键双击
void CServerItemView::OnNMDblclk(NMHDR * pNMHDR, LRESULT * pResult)
{
	//变量定义
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=GetCurrentTreeItem();

	//展开列表
	if (hTreeItem!=NULL)
	{
		//Select(hTreeItem,TVGN_CARET);
		pListItem=(CListItem *)GetItemData(hTreeItem);
	}

	//发送消息
	ASSERT(m_pITreeCtrlSink!=NULL);
	m_pITreeCtrlSink->OnTreeLeftDBClick(pListItem,hTreeItem,this);

	return;
}

//右键双击
void CServerItemView::OnNMRdblclk(NMHDR * pNMHDR, LRESULT * pResult)
{
	//变量定义
	//CListItem * pListItem=NULL;
	//HTREEITEM hTreeItem=GetCurrentTreeItem();

	////展开列表
	//if (hTreeItem!=NULL)
	//{
	//	Select(hTreeItem,TVGN_CARET);
	//	pListItem=(CListItem *)GetItemData(hTreeItem);
	//}

	////发送消息
	//ASSERT(m_pITreeCtrlSink!=NULL);
	//m_pITreeCtrlSink->OnTreeRightDBClick(pListItem,hTreeItem,this);

	return;
}

//选择改变
void CServerItemView::OnTvnSelchanged(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMTREEVIEW pNMTreeView=reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	//变量定义
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=pNMTreeView->itemNew.hItem;
	if (hTreeItem!=NULL) pListItem=(CListItem *)pNMTreeView->itemNew.lParam;

	//发送消息
	ASSERT(m_pITreeCtrlSink!=NULL);
	m_pITreeCtrlSink->OnTreeSelectChanged(pListItem,hTreeItem,this);

	return;
}

//列表展开
void CServerItemView::OnTvnItemexpanded(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMTREEVIEW pNMTreeView=reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	//变量定义
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=pNMTreeView->itemNew.hItem;
	if (hTreeItem!=NULL) pListItem=(CListItem *)pNMTreeView->itemNew.lParam;

	//发送消息
	ASSERT(m_pITreeCtrlSink!=NULL);
	m_pITreeCtrlSink->OnTreeItemexpanded(pListItem,hTreeItem,this);

	CRect rtClient;
	GetClientRect(rtClient);
	

	return;
}

//按钮测试
HTREEITEM CServerItemView::GetCurrentTreeItem()
{
	TVHITTESTINFO HitTestInfo;
	memset(&HitTestInfo,0,sizeof(HitTestInfo));
	HitTestInfo.flags=TVHT_ONITEM;
	GetCursorPos(&HitTestInfo.pt);
	ScreenToClient(&HitTestInfo.pt);
	return TreeView_HitTest(m_hWnd,&HitTestInfo);
}

DWORD CServerItemView::GetGameImageIndex(CListKind * pListKind)
{
	//效验参数
	ASSERT(pListKind!=NULL);
	
	//安装判断
	if (pListKind->m_GameKind.dwMaxVersion==0L) return IND_KIND_UNKNOW;
	if ((pListKind->m_GameKind.dwMaxVersion!=0L)&&(pListKind->m_bInstall==false)) return IND_KIND_NODOWN;
    
	//获取图标
	tagGameKind * pGameKind=pListKind->GetItemInfo();
	DWORD dwImageIndex=GetGameImageIndex(pGameKind->szProcessName,pGameKind->wKindID);
    
	return dwImageIndex;
}

//获取图标
DWORD CServerItemView::GetGameImageIndex(CListServer * pListServer)
{
	return IND_SERVER_NORNAL;
}

//获取图标
DWORD CServerItemView::GetGameImageIndex(LPCTSTR pszProcess, WORD wKindID)
{
	//寻找现存
	tagGameResourceInfo * pGameResourceInfo=NULL;
	for (INT_PTR i=0;i<m_GameResourceArray.GetCount();i++)
	{
		pGameResourceInfo=&m_GameResourceArray[i];
		if (pGameResourceInfo->wKindID==wKindID) return pGameResourceInfo->dwImageIndex;
	}

	//加载资源
	HINSTANCE hInstance=AfxLoadLibrary(pszProcess);
	if (hInstance==NULL) return IND_KIND_UNKNOW;
	
	//加载标志
	CBitmap GameLogo;
	DWORD dwImagePos=0L;
	AfxSetResourceHandle(hInstance);
	if (GameLogo.LoadBitmap(TEXT("GAME_LOGO"))) dwImagePos=m_ImageList.Add(&GameLogo,RGB(255,0,255));
	AfxSetResourceHandle(GetModuleHandle(NULL));
	AfxFreeLibrary(hInstance);

	//保存信息
	if (dwImagePos!=0L)
	{
		tagGameResourceInfo GameResourceInfo;
		memset(&GameResourceInfo,0,sizeof(GameResourceInfo));
		GameResourceInfo.wKindID=wKindID;
		GameResourceInfo.dwImageIndex=dwImagePos;
		m_GameResourceArray.Add(GameResourceInfo);
		return GameResourceInfo.dwImageIndex;
	}
	
	return IND_KIND_UNKNOW;
}

//获取标题
LPCTSTR CServerItemView::GetGameItemTitle(CListKind * pListKind, LPTSTR pszTitle, WORD wBufferSize)
{
	//效验参数
	ASSERT(pszTitle!=NULL);
	ASSERT(pListKind!=NULL);

	//生成标题
	tagGameKind * pGameKind=pListKind->GetItemInfo();

	//特殊标题
	if (pGameKind->dwMaxVersion==0L)
	{
		lstrcpyn(pszTitle,pGameKind->szKindName,wBufferSize);
		return pszTitle;
	}

	//构造标题
// 	if (m_bShowOnLineCount==true)
// 	{
// 		if (pListKind->m_bInstall==true)
// 		{
// 			_snprintf(pszTitle,wBufferSize,TEXT("%s[%ld ]"),pGameKind->szKindName,pGameKind->dwOnLineCount);
// 		}
// 		else
// 		{
// 			_snprintf(pszTitle,wBufferSize,TEXT("%s[%ld ] （双击下载）"),pGameKind->szKindName,pGameKind->dwOnLineCount);
// 		}
// 	}
// 	else
	{
		if (pListKind->m_bInstall==true)
		{
			_snprintf(pszTitle,wBufferSize,TEXT("%s"),pGameKind->szKindName);
		}
		else
		{
			_snprintf(pszTitle,wBufferSize,TEXT("%s（双击下载）"),pGameKind->szKindName);
		}
	}

	return pszTitle;
}

//获取标题
LPCTSTR CServerItemView::GetGameItemTitle(CListServer * pListServer, LPTSTR pszTitle, WORD wBufferSize)
{
	//效验参数
	ASSERT(pszTitle!=NULL);
	ASSERT(pListServer!=NULL);

	//生成标题
	tagGameServer * pGameServer=pListServer->GetItemInfo();
	if (m_bShowOnLineCount==true)
	{
		DWORD dwOnline = pGameServer->dwOnLineCount;
		TCHAR szTemp[256]={0};
		_snprintf(szTemp, 256, TEXT("空闲"));
		if(dwOnline == 0)
			_snprintf(szTemp, 256, TEXT("空闲"));
		if(dwOnline  > 0 )
			_snprintf(szTemp, 256, TEXT("正常"));
		if(dwOnline >= 50)
			_snprintf(szTemp, 256, TEXT("繁忙"));
		if(dwOnline >= 90)
			_snprintf(szTemp, 256, TEXT("拥挤"));

		_snprintf(pszTitle,wBufferSize,TEXT("%s[%s]"),pGameServer->szServerName,szTemp);
	}
	else 
	{
		_snprintf(pszTitle,wBufferSize,TEXT("%s"),pGameServer->szServerName);
	}

	return pszTitle;
}

bool CServerItemView::ChangeTreeType(bool bType)
{
	m_bTreeType=bType;
	if(m_bTreeType==TREE_TYPE_DEF)
	{
		SetFont(&m_Font12);	//小	
		SetItemHeight(22);	
	}
	else
	{
		SetFont(&m_Font14);//大
		SetItemHeight(26);
	}

	CRect rtClient;
	GetClientRect(rtClient);
	this->InvalidateRect(rtClient);

	UpdateWindow();
	return false;
}
//void CServerItemView::OnMouseMove(UINT nFlags, CPoint point)
//{
//	CRect rtClient;
//	GetClientRect(rtClient);
//	int ItemH=GetItemHeight();
//	rtMouseMove.SetRect(0,point.y-point.y/ItemH,rtClient.Width(),point.y-point.y/ItemH+ItemH);
//}


/////////////////////////////////////////////////////////////////////////////
// cTree

cTree::cTree()
{
	memDC_bgColor_bitmap = RGB(255,255,255);

	m_pDragImage = NULL;
	m_bLDragging = FALSE; 

	isImageTiled = true ;

	SetDefaultCursor() ;

	m_ColorBk=DEF_FRAME_COLOR;


	m_gradient_bkgd_sel = RGB( 0x80, 0xa0, 0xff );	//Blueish
	m_gradient_bkgd_to = RGB( 0xff, 0xff, 0xff );	//White
	m_gradient_bkgd_from = RGB( 0xd5, 0xd5, 0xe0 );	//Light Greyish Blue

	//m_gradient_bkgd_sel = RGB( 0x80, 0xa0, 0xff );	//Blueish
	//m_gradient_bkgd_from = RGB( 71, 135, 167 );	//White
	//m_gradient_bkgd_to = RGB( 250, 250, 250 );	//Light Greyish Blue

	m_bkgd_mode = BK_MODE_GRADIENT;
	m_bmp_tiled_mode = false;
	m_gradient_horz = true;

   /* m_bmp_tree_closed.LoadBitmap( IDB_TREE_CLOSED )  ; 
    m_bmp_tree_closed.GetSafeHandle();
    m_bmp_tree_open.LoadBitmap( IDB_TREE_OPENED ); 
    m_bmp_tree_open.GetSafeHandle();*/
	m_icon = NULL;

	
}

cTree::~cTree()
{

	if (m_bmp_back_ground.GetSafeHandle())
        m_bmp_back_ground.DeleteObject();
    if (m_bmp_tree_closed.GetSafeHandle())
        m_bmp_tree_closed.DeleteObject();
    if (m_bmp_tree_open.GetSafeHandle())
        m_bmp_tree_open.DeleteObject();
}


BEGIN_MESSAGE_MAP(cTree, CTreeCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
//	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBeginDrag)
	ON_WM_MOUSEMOVE()	
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemExpanding)
	ON_WM_ERASEBKGND()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cTree message handlers

HTREEITEM cTree::AddGroup(const CString & group_name)
{
 	HTREEITEM added_group = 0;
	HTREEITEM groupItem ; 

		if ( (groupItem = GetGroupTreeItem(group_name)) != 0 )  return 0;

	HTREEITEM lastAddedGroup = 0;
		
		//insert the group into the tree
		added_group = InsertItem(group_name, 1,1,0,TVI_FIRST);
	    //Associates 32 bit number with this item
	//	SetItemData(added_group,(DWORD)newGroup);
	
		Expand(added_group,TVE_EXPAND);


	return added_group ;
}

HTREEITEM cTree::GetGroupTreeItem(CString grp_name)
{

	HTREEITEM answer = 0;
	
	HTREEITEM groupItem = GetRootItem()/*(TVI_ROOT, TVGN_NEXT)*/;
	while (groupItem != NULL && !answer)	//while there is still something in the tree
	{
	
		TVITEM item;
		TCHAR szText[1024];
		//CString szText = name;
		item.hItem = groupItem;
		//only get the text and the handle of the item
		item.mask = TVIF_TEXT | TVIF_HANDLE;	
		item.pszText = szText;
		item.cchTextMax = 1024;

		GetItem(&item);

		CString thisGroupName = item.pszText;

		if (thisGroupName == grp_name)
		{
			answer = groupItem;
			break ;
		}
		
		//get the next item for the sake of the while loop ending
		groupItem = GetNextItem(groupItem,TVGN_NEXT);
	}
	return answer;
}

bool cTree::DeleteGroup(CString group_name)
{
	if (group_name.IsEmpty() ) return false ;

	HTREEITEM group_item ;
	if ( (group_item = GetGroupTreeItem(group_name)) == 0 ) return false ;


		//delete every child of this group
		HTREEITEM currentItem = GetNextItem(group_item ,TVGN_CHILD);

		while (currentItem != NULL)
		{
			DeleteItem(currentItem);

			//get the next item for the sake of the while loop ending
			currentItem = GetNextItem(currentItem,TVGN_NEXT);
		}

		//delete the actual group now
		DeleteItem(group_item);

	return true ;
}

HTREEITEM cTree::AddChild(CString buddy_name, CString group_name)
{
 
	HTREEITEM buddy_that_was_added;

	//check if the buddy already exists
	if (GetBuddyTreeItem(buddy_name) != 0) return 0;


		HTREEITEM tree_group = GetGroupTreeItem(group_name);

		//if the group actually exists add the buddy to it
		if (tree_group != 0)
		{
			buddy_that_was_added = InsertItem(buddy_name,2, 2,tree_group,TVI_LAST);

		  //	SetItemData(buddy_that_was_added,(DWORD)newBuddy);
		}
		//create the group then add the buddy
		else	
		{
			tree_group = AddGroup(group_name);
			buddy_that_was_added = InsertItem(buddy_name, 2, 2,tree_group,TVI_LAST);
	
		//	SetItemData(buddyWasAdded,(DWORD)newBuddy);
		}
	

		//this just expands the group the buddy was added to
		HTREEITEM hParent = GetParentItem(buddy_that_was_added);
	
		if (hParent != NULL)	Expand(hParent, TVE_EXPAND);

	
	
	return buddy_that_was_added;
}

HTREEITEM cTree::GetBuddyTreeItem(CString buddy_name)
{
  	HTREEITEM answer = 0;
	//get the root group
	HTREEITEM groupItem = GetRootItem();
	while (groupItem != NULL && !answer)	//while there is still something in the tree
	{
	HTREEITEM currentItem = GetNextItem(groupItem,TVGN_CHILD);
	while (currentItem != NULL && !answer)	//while there is still something in the tree
	{
		TVITEM item;
		TCHAR szText[1024];
		//CString szText = name;
		item.hItem = currentItem;
		//only get the text and the handle of the item
		item.mask = TVIF_TEXT | TVIF_HANDLE;	
		item.pszText = szText;
		item.cchTextMax = 1024;

		/*BOOL answer = */
		GetItem(&item);

		CString thisBuddyName = item.pszText;
		if (thisBuddyName == buddy_name)
			answer = currentItem ;
		else
			answer = 0;

		//get the next item for the sake of the while loop ending
		currentItem = GetNextItem(currentItem,TVGN_NEXT);
	}
		groupItem = GetNextItem(groupItem,TVGN_NEXT);
	}
	return answer;
}

void cTree::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	//CTreeCtrl::OnLButtonDblClk(nFlags, point);
	//
	//HTREEITEM hItem = GetSelectedItem();
	//
	//// If this is a root node, return
	//if (GetParentItem(hItem) == NULL) return ;

}

void cTree::OnRButtonDown(UINT nFlags, CPoint point) 
{
	

	//CTreeCtrl::OnRButtonDown(nFlags, point);
}
bool isExpand=false;
void cTree::OnLButtonDown(UINT nFlags, CPoint point) 
{
	////SetFocus();
	////变量定义
	//CListItem * pListItem=NULL;
	//HTREEITEM hTreeItem=GetCurrentTreeItem();
	//CRect rtClient;
	//GetClientRect(rtClient);

	////展开列表
	//if (hTreeItem!=NULL)
	//{
	//	int ItemH=GetItemHeight();
	//	LBtnDownIndex=point.y/ItemH;
	//	rtSelete.SetRect(0,point.y/ItemH*ItemH,rtClient.Width(),(point.y/ItemH+1)*ItemH);
	//	Select(hTreeItem,TVGN_ROOT);
	//	//SetFocus();
	//	pListItem=(CListItem *)GetItemData(hTreeItem);

	//	//效验参数
	//	ASSERT(pListItem!=NULL);

	//	//展开列表
	//	HTREEITEM hTreeItem=(HTREEITEM)pListItem->GetItemData();
	//	if (hTreeItem!=NULL) 
	//	{
	//		UINT   nState   =   GetItemState(hTreeItem,   TVIS_EXPANDED);   
	//		if   (0   !=   (nState   &   TVIS_EXPANDED))   
	//		{   
	//			//   已经展开   
	//			Expand(hTreeItem,TVE_COLLAPSE);
	//		}   
	//		else   
	//		{   
	//			//   没展开  
	//			Expand(hTreeItem,TVE_EXPAND);
	//		}			
	//		
	//		GetClientRect(rtClient);
	//		this->InvalidateRect(rtClient,false);
	//		
	//		return ;
	//	}

	//	return ;
	//}
	//
	//this->InvalidateRect(rtClient);

	//CTreeCtrl::OnLButtonDown(nFlags, point);
}

//void cTree::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
//	*pResult = 0;
//	
//	// So user cant drag root node
//	if (GetParentItem(pNMTreeView->itemNew.hItem) == NULL) return ; 
//
//	// Item user started dragging ...
//	m_hitemDrag = pNMTreeView->itemNew.hItem;
//	m_hitemDrop = NULL;
//
//
//
//	m_pDragImage = CreateDragImage(m_hitemDrag);  // get the image list for dragging
//	// CreateDragImage() returns NULL if no image list
//	// associated with the tree view control
//	if( !m_pDragImage )
//		return;
//
//	m_bLDragging = TRUE;
//	m_pDragImage->BeginDrag(0, CPoint(-15,-15));
//	POINT pt = pNMTreeView->ptDrag;
//	ClientToScreen( &pt );
//	m_pDragImage->DragEnter(NULL, pt);
//	SetCapture();
//
//}

void cTree::OnMouseMove(UINT nFlags, CPoint point) 
{

	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=GetCurrentTreeItem();
	CRect rtClient;
	GetClientRect(rtClient);

	//展开列表
	if (hTreeItem==NULL) return ;
	
	
	if(point.x>=rtMouseMove.left&&
		point.x<=rtMouseMove.right&&
		point.y>=rtMouseMove.top&&
		point.y<=rtMouseMove.bottom)
	{

		
	}
	else
	{
		int ItemH=GetItemHeight();
		MouseMoveIndex=point.y/ItemH;	
		CRect rtClient;
		GetClientRect(rtClient);
		
		rtMouseMove.SetRect(0,(point.y/ItemH)*ItemH,rtClient.Width(),(point.y/ItemH+1)*ItemH);

		this->InvalidateRect(rtClient);
	}

	//HTREEITEM	hitem;
	//UINT		flags;

	//if (m_bLDragging)
	//{
	//	POINT pt = point;
	//	ClientToScreen( &pt );
	//	CImageList::DragMove(pt);
	//	if ((hitem = HitTest(point, &flags)) != NULL)
	//	{
	//			CImageList::DragShowNolock(FALSE);
	//			
	//		// Tests if dragged item is over another child !
	//		  if ( (GetParentItem(hitem) != NULL) && (cursor_no != ::GetCursor())) 
	//		  {
	//			  ::SetCursor(cursor_no);
	//			   // Dont want last selected target highlighted after mouse
	//			   // has moved off of it, do we now ?
	//			   SelectDropTarget(NULL);
	//		  }
	//		// Is item we're over a root node and not parent root node ?
	//		if ( (GetParentItem(hitem) == NULL) && (GetParentItem(m_hitemDrag) != hitem ) ) 
	//		{
	//			if (cursor_arr != ::GetCursor()) ::SetCursor(cursor_arr); 
	//			SelectDropTarget(hitem);
	//		}

	//		m_hitemDrop = hitem;
	//		CImageList::DragShowNolock(TRUE);
	//	}
	//}
	//else 
	//{
	//	// Set cursor to arrow if not dragged
	//	// Otherwise, cursor will stay hand or arrow depen. on prev setting
	//	::SetCursor(cursor_arr);
	//}
	//	


	//CTreeCtrl::OnMouseMove(nFlags, point);

}

void cTree::OnLButtonUp(UINT nFlags, CPoint point) 
{

	//CTreeCtrl::OnLButtonUp(nFlags, point);

	//if (m_bLDragging)
	//{
	//	m_bLDragging = FALSE;
	//	CImageList::DragLeave(this);
	//	CImageList::EndDrag();
	//	ReleaseCapture();

	//	if(m_pDragImage != NULL) 
	//	{ 
	//	delete m_pDragImage; 
	//	m_pDragImage = NULL; 
	//	} 

	//	// Remove drop target highlighting
	//	SelectDropTarget(NULL);

	//	if( m_hitemDrag == m_hitemDrop )
	//		return;

	//	HTREEITEM	hitem;
	//	// Make sure pt is over some item
	//	if ( ((hitem = HitTest(point, &nFlags)) == NULL)  ) return ;
	//	// Make sure dropped item isnt a child
	//	if (GetParentItem(hitem) != NULL) return ;

	//	// If Drag item is an ancestor of Drop item then return
	//	HTREEITEM htiParent = m_hitemDrop;
	//	while( (htiParent = GetParentItem( htiParent )) != NULL )
	//	{
	//		if( htiParent == m_hitemDrag ) return;
	//	}

	//	Expand( m_hitemDrop, TVE_EXPAND ) ;

	//	HTREEITEM htiNew = MoveChildItem( m_hitemDrag, m_hitemDrop, TVI_LAST );
	//	DeleteItem(m_hitemDrag);
	//	SelectItem( htiNew );
	//}

}

HTREEITEM cTree::MoveChildItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{

	TV_INSERTSTRUCT tvstruct;
	HTREEITEM hNewItem;
    CString sText;

    // get information of the source item
    tvstruct.item.hItem = hItem;
    tvstruct.item.mask = TVIF_CHILDREN | TVIF_HANDLE |     TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    GetItem(&tvstruct.item);  
    sText = GetItemText( hItem );
        
    tvstruct.item.cchTextMax = sText.GetLength();
    tvstruct.item.pszText = sText.LockBuffer();

    //insert the item at proper location
    tvstruct.hParent = htiNewParent;
    tvstruct.hInsertAfter = htiAfter;
    tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
    hNewItem = InsertItem(&tvstruct);
    sText.ReleaseBuffer();

    //now copy item data and item state.
    SetItemData(hNewItem,GetItemData(hItem));
    SetItemState(hNewItem,GetItemState(hItem,TVIS_STATEIMAGEMASK),TVIS_STATEIMAGEMASK);

	//now delete the old item
	DeleteItem(hItem);

    return hNewItem;
}


bool cTree::DeleteChild(CString buddy_name)
{
	if (buddy_name.IsEmpty()) return false;


	HTREEITEM buddyItem = GetBuddyTreeItem(buddy_name);

	if (buddyItem != 0)
	{
		DeleteItem(buddyItem);
		return true ; 
	}

	return false;
}

int cTree::GetChildCountInGroup(CString group_name)
{

	HTREEITEM group_item = GetGroupTreeItem(group_name);	//get the group
	
	if (group_item == 0)					return -1 ;
	if (!ItemHasChildren(group_item))		return  0 ;

		int total_in_group =0 ;
		
		//delete every child of this group
		HTREEITEM currentItem = GetNextItem(group_item ,TVGN_CHILD);

		while (currentItem != NULL)
		{
			++total_in_group ;

			//get the next item for the sake of the while loop ending
			currentItem = GetNextItem(currentItem,TVGN_NEXT);
		}

	return total_in_group ;
}

CString cTree::RemoveBuddyCountFromName(CString name)
{
	if (name.Find("(") > 0)
	{
		name = name.Left(name.Find("(") - 1);
	}
	else return name;

	return name;
}

/*
void cTree::CollapseAll()
{
        HTREEITEM hti = GetRootItem();
        do{
             Expand( hti, TVE_COLLAPSE ); //     CollapseBranch( hti );
        }while( (hti = GetNextSiblingItem( hti )) != NULL );
}

  */

void cTree::OnPaint() 
{



	CPaintDC dc(this);

	CRect rcClip, rcClient;
	dc.GetClipBox( &rcClip );
	GetClientRect(&rcClient);

	// Create a compatible memory DC 
	CDC memDC;
	memDC.CreateCompatibleDC( &dc );
	
	// Select a compatible bitmap into the memory DC
	CBitmap bitmap, bmpImage;
	bitmap.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
	memDC.SelectObject( &bitmap );
    
	
	// First let the control do its default drawing.
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );

	// Draw bitmap in the background if one has been set
	if( m_bitmap.m_hObject != NULL )
	{
		// Now create a mask
		CDC maskDC;
		maskDC.CreateCompatibleDC(&dc);
		CBitmap maskBitmap;

		// Create monochrome bitmap for the mask
		maskBitmap.CreateBitmap( rcClient.Width(), rcClient.Height(), 
						1, 1, NULL );
		maskDC.SelectObject( &maskBitmap );
		memDC.SetBkColor(RGB(255,255,255) /*::GetSysColor( COLOR_WINDOW )*/ );

		// Create the mask from the memory DC
		maskDC.BitBlt( 0, 0, rcClient.Width(), rcClient.Height(), &memDC, 
					rcClient.left, rcClient.top, SRCCOPY );

		
		CDC tempDC;
		tempDC.CreateCompatibleDC(&dc);
		tempDC.SelectObject( &m_bitmap );

		CDC imageDC;
		CBitmap bmpImage;
		imageDC.CreateCompatibleDC( &dc );
		bmpImage.CreateCompatibleBitmap( &dc, rcClient.Width(), 
						rcClient.Height() );
		imageDC.SelectObject( &bmpImage );
        
		if( dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE && m_pal.m_hObject != NULL )
		{
			dc.SelectPalette( &m_pal, FALSE );
			dc.RealizePalette();
            
			imageDC.SelectPalette( &m_pal, FALSE );
		}
        
		// Get x and y offset
		CRect rcRoot;
		GetItemRect( GetRootItem(), rcRoot, FALSE );
		rcRoot.left = -GetScrollPos( SB_HORZ );

	/////////////////////////////////////////////////
		if (isImageTiled)
		{
			// Draw bitmap in tiled manner to imageDC
			for( int i = rcRoot.left; i < rcClient.right; i += m_cxBitmap )
			for( int j = rcRoot.top; j < rcClient.bottom; j += m_cyBitmap )
				imageDC.BitBlt( i, j, m_cxBitmap, m_cyBitmap, &tempDC, 
							0, 0, SRCCOPY );
		}else
		{
			int x=0,y=0 ;
			(m_cxBitmap > rcClient.right) ? x=0:x=(rcClient.right - m_cxBitmap);
			(m_cyBitmap > rcClient.bottom)? y=0:y=(rcClient.bottom - m_cyBitmap);
			imageDC.BitBlt( x, y, m_cxBitmap, m_cyBitmap, &tempDC, 
							0, 0, SRCCOPY );
		}
		int ItemH=GetItemHeight();

		COLORREF b=((m_ColorBk&0x00ff0000)>>16)+220; if(b>=255)b=255;//R
		COLORREF g=((m_ColorBk&0x0000ff00)>>8)+220; if(g>=255)g=255; //G
		COLORREF r=(m_ColorBk&0x000000ff)+220; if(r>=255)r=255; //B
		
		//imageDC.FillSolidRect(rcClient,RGB(r,g,b));
		//GradientFillRect(&imageDC,rcClient,m_gradient_bkgd_from, m_gradient_bkgd_to, !m_gradient_horz );

		//GetDlgItemInt 
		////画线
		//int count=0;
		//for(int i=0;i<=rcClient.Height();i+=ItemH)
		//{
		//	count++;
		//	/*HPEN hPen = CreatePen(PS_SOLID,0,RGB(218,218,218));
		//	imageDC.SelectObject(hPen);		
		//	imageDC.MoveTo(0,i);
		//	imageDC.LineTo(rcClient.Width(),i);*/
  //          
		//	COLORREF ColorRt;
		//	ColorRt=RGB(230,230,230);
		//	imageDC.FillSolidRect(0,i,rcClient.right,1,ColorRt);
  //          
		//	//DeleteObject(hPen);
		//}
		int i=0;
		HTREEITEM	hItem = NULL;
		hItem = GetFirstVisibleItem();
		while ( hItem )
	    { 
             i+=ItemH;      
			COLORREF ColorRt;
			ColorRt=RGB(230,230,230);
			imageDC.FillSolidRect(0,i,rcClient.right,1,ColorRt);
		hItem = this->GetNextVisibleItem( hItem );
	    }// end of loop
        //画选择框		
	    rtMouseMove.SetRect(0,MouseMoveIndex*ItemH+1,rcClient.Width(),(MouseMoveIndex+1)*ItemH-1);
		GradientFillRect(&imageDC, rtMouseMove, RGB(215,236,254), RGB(231,250,254), FALSE );	
        
		rtSelete.SetRect(0,LBtnDownIndex*ItemH+1,rcClient.Width(),(LBtnDownIndex+1)*ItemH-1);
		//imageDC.FillRect(rtSelete,&CBrush(RGB(254,235,202)));
		GradientFillRect(&imageDC, rtSelete, RGB(254,235,202),RGB(254,250,220), FALSE );
		
		

		// Set the background in memDC to black. Using SRCPAINT with black and any other
		// color results in the other color, thus making black the transparent color
		memDC.SetBkColor( RGB(0,0,0)/*memDC_bgColor_bitmap*/);        
		memDC.SetTextColor(RGB(255,255,255));
		memDC.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &maskDC, 
				rcClip.left, rcClip.top, SRCAND);
      
		// Set the foreground to black. See comment above.
		imageDC.SetBkColor(RGB(255,255,255));
		imageDC.SetTextColor(RGB(0,0,0));
		imageDC.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &maskDC, 
				rcClip.left, rcClip.top, SRCAND);

		// Combine the foreground with the background
		imageDC.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), 
					&memDC, rcClip.left, rcClip.top,SRCPAINT);

	

		
		// Draw the final image to the screen		
		dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), 
					&imageDC, rcClip.left, rcClip.top, SRCCOPY );
		
	}
	else
	{
		dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(), 
				rcClip.Height(), &memDC, 
				rcClip.left, rcClip.top, SRCCOPY );
	}

}

// Draw TreeCtrl Background - 
void cTree::DrawBackGround( CDC* pDC )
{
	BkMode mode = m_bkgd_mode;

	if ( mode == BK_MODE_BMP )
	{
		if ( !m_bmp_back_ground.GetSafeHandle() )
			mode = BK_MODE_GRADIENT;
	}
	if ( mode == BK_MODE_GRADIENT )
	{
		GradientFillRect( pDC, 
			CRect( m_h_offset, m_v_offset, m_h_size + m_h_offset, m_v_size + m_v_offset ), 
			m_gradient_bkgd_from, m_gradient_bkgd_to, !m_gradient_horz );
	}
	else if ( mode == BK_MODE_FILL )
		pDC->FillSolidRect( m_rect, pDC->GetBkColor() ); 
	else if ( mode == BK_MODE_BMP )
	{
		BITMAP bm;
		CDC dcMem;
	      
		VERIFY(m_bmp_back_ground.GetObject(sizeof(bm), (LPVOID)&bm));
		dcMem.CreateCompatibleDC(NULL);
		CBitmap* bmp_old = (CBitmap*)dcMem.SelectObject( &m_bmp_back_ground ); 
		
		if ( m_bmp_tiled_mode ) 	// BitMap Tile Mode
		{
			for ( int y = 0; y <= (m_v_size / bm.bmHeight); y++ )
			{
				for ( int x = 0; x <= (m_h_size / bm.bmWidth); x++ )
				{
					pDC->BitBlt((x*bm.bmWidth) + m_h_offset, (y*bm.bmHeight) + m_v_offset,
						bm.bmWidth, bm.bmHeight, &dcMem, 0, 0, SRCCOPY);
				}
			}
		}
		else  // BITMAP Stretch Mode
		{
			pDC->StretchBlt( m_h_offset, m_v_offset, m_h_size, m_v_size, 
				&dcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY );
		}
		// CleanUp
		dcMem.SelectObject( bmp_old );
	}
	else
		ASSERT( 0 );  // Unknown BackGround Mode
}



// Draw TreeCtrl Items
void cTree::DrawItems( CDC *pDC )
{
	// draw items
	HTREEITEM show_item, parent;
	CRect rc_item;
	CString name;
	COLORREF color;
	DWORD tree_style;
	BITMAP bm;
	CDC dc_mem;
	CBitmap *button;
	int count = 0;
	int state;
	bool selected;
	bool has_children;

	show_item = GetFirstVisibleItem();
	if ( show_item == NULL )
		return;

	dc_mem.CreateCompatibleDC(NULL);
	color = pDC->GetTextColor();
	tree_style = ::GetWindowLong( m_hWnd, GWL_STYLE ); 

	do
	{
		state = GetItemState( show_item, TVIF_STATE );
		parent = GetParentItem( show_item );
		has_children = ItemHasChildren( show_item ) || parent == NULL;
		selected = (state & TVIS_SELECTED) && ((this == GetFocus()) || 
				(tree_style & TVS_SHOWSELALWAYS));

		if ( GetItemRect( show_item, rc_item, TRUE ) )
		{
			if ( has_children  || selected )
			{
				COLORREF from;
				CRect rect;
				// Show 
				if ( selected )
					from = m_gradient_bkgd_sel;
				else
					from = m_gradient_bkgd_to - (m_gradient_bkgd_from - m_gradient_bkgd_to);
				rect.top = rc_item.top;
				rect.bottom = rc_item.bottom;
				rect.right = m_h_size + m_h_offset;
				if ( !has_children )
					rect.left = rc_item.left + m_h_offset;
				else
					rect.left = m_h_offset;
				GradientFillRect( pDC, rect, from, m_gradient_bkgd_to, FALSE );
				pDC->SetTextColor( RGB( 0, 0, 255 ) );

				if ( has_children )
				{
					// Draw an Open/Close button
					if ( state & TVIS_EXPANDED )
						button = &m_bmp_tree_open;
					else
						button = &m_bmp_tree_closed;
					VERIFY(button->GetObject(sizeof(bm), (LPVOID)&bm));
					CBitmap *bmp_old = (CBitmap*)dc_mem.SelectObject(button); 
					pDC->BitBlt( rc_item.left - bm.bmWidth - 2, rc_item.top, bm.bmWidth, bm.bmHeight, 
						&dc_mem, 0, 0, SRCAND );
					// CleanUp
					dc_mem.SelectObject( bmp_old );
				}
			}
			if ( !has_children )
			{
				// lookup the ICON instance (if any) and draw it
				HICON icon;
				icon = GetItemIcon( show_item );
				if ( icon != NULL )
					DrawIconEx( pDC->m_hDC, rc_item.left - 18, rc_item.top, icon, 16, 16,0,0, DI_NORMAL );
			}
			name = GetItemText( show_item );
			rc_item.DeflateRect( 0,1,0,1 );
			if ( selected )
			{
				if ( !has_children  )
					pDC->SetTextColor( GetSysColor(COLOR_HIGHLIGHTTEXT) );
				COLORREF col = pDC->GetBkColor();
				pDC->SetBkColor( GetSysColor(COLOR_HIGHLIGHT) );
				pDC->DrawText( name, rc_item, DT_LEFT );
				pDC->SetTextColor( color );
				pDC->SetBkColor( col );
			}
			else
			{
				pDC->DrawText( name, rc_item, DT_LEFT );
				pDC->SetTextColor( color );
			}
			//if ( state & TVIS_BOLD )
			//	pDC->SelectObject( font );
		}
	} while ( (show_item = GetNextVisibleItem( show_item )) != NULL );
}


void cTree::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	//if( m_bitmap.m_hObject != NULL )
		InvalidateRect(NULL);
	
	*pResult = 0;

}

BOOL cTree::OnEraseBkgnd(CDC* pDC) 
{
	if( m_bitmap.m_hObject != NULL )	return TRUE;

	return CTreeCtrl::OnEraseBkgnd(pDC);

}

//BOOL cTree::OnQueryNewPalette() 
//{
//
//	CClientDC dc(this);
//	if( dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE && m_pal.m_hObject != NULL )
//	{
//		dc.SelectPalette( &m_pal, FALSE );
//		BOOL result = dc.RealizePalette();
//		if( result )
//			Invalidate();
//		return result;
//	}
//	
//	return CTreeCtrl::OnQueryNewPalette();
//}

//void cTree::OnPaletteChanged(CWnd* pFocusWnd) 
//{
//	CTreeCtrl::OnPaletteChanged(pFocusWnd);
//	
//	if( pFocusWnd == this )
//		return;
//
//	OnQueryNewPalette();
//
//}

BOOL cTree::SetBkImage(UINT nIDResource)
{
	return SetBkImage( (LPCTSTR)nIDResource );
}

void cTree::SetSkinBkColor(COLORREF color)
{
	m_ColorBk=color;
	if(GetSafeHwnd())
	{
		CRect rtWnd;
		GetClientRect(rtWnd);
		InvalidateRect(rtWnd);
	}
}

BOOL cTree::SetBkImage(LPCTSTR lpszResourceName)
{

	// If this is not the first call then Delete GDI objects
	if( m_bitmap.m_hObject != NULL )
		m_bitmap.DeleteObject();
	if( m_pal.m_hObject != NULL )
		m_pal.DeleteObject();
	
	
	HBITMAP hBmp = (HBITMAP)::LoadImage( AfxGetInstanceHandle(), 
			lpszResourceName, IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION );

	if( hBmp == NULL ) 
		return FALSE;

	m_bitmap.Attach( hBmp );
	BITMAP bm;
	m_bitmap.GetBitmap( &bm );
	m_cxBitmap = bm.bmWidth;
	m_cyBitmap = bm.bmHeight;


	// Create a logical palette for the bitmap
	DIBSECTION ds;
	BITMAPINFOHEADER &bmInfo = ds.dsBmih;
	m_bitmap.GetObject( sizeof(ds), &ds );

	int nColors = bmInfo.biClrUsed ? bmInfo.biClrUsed : 1 << bmInfo.biBitCount;

	// Create a halftone palette if colors > 256. 
	CClientDC dc(NULL);			// Desktop DC
	if( nColors > 256 )
		m_pal.CreateHalftonePalette( &dc );
	else
	{
		// Create the palette

		RGBQUAD *pRGB = new RGBQUAD[nColors];
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);

		memDC.SelectObject( &m_bitmap );
		::GetDIBColorTable( memDC, 0, nColors, pRGB );

		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

		pLP->palVersion = 0x300;
		pLP->palNumEntries = nColors;

		for( int i=0; i < nColors; i++)
		{
			pLP->palPalEntry[i].peRed = pRGB[i].rgbRed;
			pLP->palPalEntry[i].peGreen = pRGB[i].rgbGreen;
			pLP->palPalEntry[i].peBlue = pRGB[i].rgbBlue;
			pLP->palPalEntry[i].peFlags = 0;
		}

		m_pal.CreatePalette( pLP );

		delete[] pLP;
		delete[] pRGB;
	}
	Invalidate();

	return TRUE;
}


void cTree::SetDefaultCursor()
{
	   // Get the windows directory
        CString strWndDir;
        GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
        strWndDir.ReleaseBuffer();

        strWndDir += _T("\\winhlp32.exe");
        // This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
        HMODULE hModule = LoadLibrary(strWndDir);
        if (hModule) {
            HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
            if (hHandCursor)
			{
                cursor_hand = CopyCursor(hHandCursor);
			}
			      
        }
        FreeLibrary(hModule);

		cursor_arr	= ::LoadCursor(NULL, IDC_ARROW);
		cursor_no	= ::LoadCursor(NULL, IDC_NO) ;
}

//按钮测试
HTREEITEM cTree::GetCurrentTreeItem()
{
	TVHITTESTINFO HitTestInfo;
	memset(&HitTestInfo,0,sizeof(HitTestInfo));
	HitTestInfo.flags=TVHT_ONITEM;
	GetCursorPos(&HitTestInfo.pt);
	ScreenToClient(&HitTestInfo.pt);
	return TreeView_HitTest(m_hWnd,&HitTestInfo);
}

// Gradient Filling Helper Routine
void GradientFillRect( CDC *pDC, CRect &rect, COLORREF col_from, COLORREF col_to, bool vert_grad )
{
	TRIVERTEX        vert[2];
	GRADIENT_RECT    mesh;

	vert[0].x      = rect.left;
	vert[0].y      = rect.top;
	vert[0].Alpha  = 0x0000;
	vert[0].Blue   = GetBValue(col_from) << 8;
	vert[0].Green  = GetGValue(col_from) << 8;
	vert[0].Red    = GetRValue(col_from) << 8;

	vert[1].x      = rect.right;
	vert[1].y      = rect.bottom; 
	vert[1].Alpha  = 0x0000;
	vert[1].Blue   = GetBValue(col_to) << 8;
	vert[1].Green  = GetGValue(col_to) << 8;
	vert[1].Red    = GetRValue(col_to) << 8;

	mesh.UpperLeft  = 0;
	mesh.LowerRight = 1;
#if _MSC_VER >= 1300  // only VS7 and above has GradientFill as a pDC member
	pDC->GradientFill( vert, 2, &mesh, 1, vert_grad ? GRADIENT_FILL_RECT_V : 
		GRADIENT_FILL_RECT_H );
#else
	GradientFill( pDC->m_hDC, vert, 2, &mesh, 1, vert_grad ? GRADIENT_FILL_RECT_V : 
		GRADIENT_FILL_RECT_H );
#endif
}



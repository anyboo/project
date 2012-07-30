// SkinListProgressCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "Resource.h"
#include "SkinListProgressCtrl.h"


// CSkinListProgressCtrl

IMPLEMENT_DYNAMIC(CSkinListProgressCtrl, CSkinListCtrl)
CSkinListProgressCtrl::CSkinListProgressCtrl()
{
	m_listItemColor1 = RGB(255,255,254);
}

CSkinListProgressCtrl::~CSkinListProgressCtrl()
{

	CProgressEntry* pProgEntry=0;
	int Index=0;
	POSITION pos = m_ProgEntries.GetStartPosition();
	while (pos != NULL) {
		m_ProgEntries.GetNextAssoc(pos, Index, pProgEntry);
		if (pProgEntry) delete pProgEntry;
	}
	m_ProgEntries.RemoveAll();

	CButtonEntry* pButtonEntry=0;
	Index=0;
	
	pos = m_ButtonEntries.GetStartPosition();
	while (pos != NULL) 
	{
		m_ButtonEntries.GetNextAssoc(pos, Index, pButtonEntry);
		if (pButtonEntry) delete pButtonEntry;
	}
	m_ButtonEntries.RemoveAll();


	pos   =   m_lstItemColor.GetHeadPosition();  
    while   (pos)  
    {  
        delete   []m_lstItemColor.GetNext(pos);  
    }   
}


BEGIN_MESSAGE_MAP(CSkinListProgressCtrl, CSkinListCtrl)
END_MESSAGE_MAP()



// CSkinListProgressCtrl 消息处理程序



// CTwoListCtrl 消息处理程序
void CSkinListProgressCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS )
{
    // TODO: Add your message handler code here and/or call default
    CDC   *pDC = CDC::FromHandle(lpDIS->hDC);  
    int   nRows = m_lstItemColor.GetCount();  
    COLORREF *clrCol = NULL;  
    if   (nRows && (int)lpDIS->itemID < nRows)  
    {  
        clrCol = m_lstItemColor.GetAt(m_lstItemColor.FindIndex(lpDIS->itemID));  
    }  
    CRect  rcItem(lpDIS->rcItem);  
    LVCOLUMN lvColumn;  
    ZeroMemory(&lvColumn, sizeof(LVCOLUMN));  
    lvColumn.mask = LVCF_FMT | LVCF_WIDTH;  
    for (int nCol = 0; nCol < GetHeaderCtrl()->GetItemCount(); nCol++)  
    {  
        if (clrCol)  
        {
            pDC->SetBkColor(clrCol[nCol]);
            pDC->SetTextColor(clrCol[nCol]);  
        }
        GetColumn(nCol, &lvColumn);  
        UINT   uStyle = DT_SINGLELINE | DT_END_ELLIPSIS;  
        if   (lvColumn.fmt & LVCFMT_LEFT)  
            uStyle |= DT_LEFT;  
        if   (lvColumn.fmt   &   LVCFMT_RIGHT)  
            uStyle |= DT_RIGHT;  
        if   (lvColumn.fmt & LVCFMT_CENTER)  
            uStyle |= DT_CENTER;  
        rcItem.right = rcItem.left + lvColumn.cx;  
     //   COLORREF color;
     //   GetSysColor(color);
     //   pDC->FillSolidRect(rcItem, RGB(122,122,122));//::GetSysColor(COLOR_INACTIVECAPTION));     
		 pDC->FillSolidRect(rcItem, m_listItemColor1); 
        pDC->DrawText(GetItemText(lpDIS->itemID, nCol), rcItem, uStyle);
        rcItem.left += lvColumn.cx;  
    }   
//     CListCtrl::OnDrawItem(nIDCtl, lpDIS);

}


void CSkinListProgressCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(0,   LVS_OWNERDRAWFIXED | LVS_REPORT, 0);   

	CSkinListCtrl::PreSubclassWindow();
//	CListCtrl::PreSubclassWindow();
}


void CSkinListProgressCtrl::CreateProgress(int Index, int nSubItem, DWORD dwID )
{
  // can only create progress for an existing item
  if (Index >= GetItemCount())
    return;
  CProgressEntry* ProgEntry = new CProgressEntry(Index, nSubItem);
  CRect ItemRect;
  GetSubItemRect(Index, ProgEntry->m_SubIndex, LVIR_BOUNDS, ItemRect);

  int left = ItemRect.left;
  int top = ItemRect.top;
  int right = (ItemRect.Width() > 100)? (ItemRect.right - 60): ItemRect.right;
  int bottom = ItemRect.bottom;

  (ProgEntry->m_Prog).Create(PBS_SMOOTH | WS_CHILD | WS_VISIBLE, CRect(left, top, right, bottom), this, 1);
  (ProgEntry->m_Prog).SetRange(0, 100);
  (ProgEntry->m_Prog).SetPos(0);

  ///////////
  ProgEntry->dwMissionID = dwID;
	(ProgEntry->m_Prog).ShowPercent(FALSE);
	//	 (ProgEntry->m_Prog).SetStartColor(m_clrStart);
	//	 (ProgEntry->m_Prog).SetEndColor(m_clrEnd);
	(ProgEntry->m_Prog).SetBkColor(RGB( 10,100,108));
	(ProgEntry->m_Prog).SetPos(0);
  ///////////
  m_ProgEntries[Index] = ProgEntry;

  this->Invalidate();
}


void CSkinListProgressCtrl::SetProgress(int nItem, int prog)
{
//	CProgressEntry* ProgEntry;
//  if (m_ProgEntries.Lookup(Index, ProgEntry) == TRUE)
//    (ProgEntry->m_Prog).SetPos(prog);

	DWORD dwMissionID = GetItemData(nItem);

	CProgressEntry* pProgEntry = 0;
	POSITION pos = m_ProgEntries.GetStartPosition();
	int Index = 0;
	while (pos != NULL) 
	{
		m_ProgEntries.GetNextAssoc(pos, Index, pProgEntry);
	//	if (pButtonEntry) delete pButtonEntry;
		if (pProgEntry)
		{
			if (pProgEntry->dwMissionID == dwMissionID)
			{
				(pProgEntry->m_Prog).SetPos(prog);
				break;
			}
		}

	}


}

void CSkinListProgressCtrl::ResizeProg()
{
	CRect ItemRect;
	CProgressEntry* ProgEntry=0;
	int Index=0;
	POSITION pos = m_ProgEntries.GetStartPosition();
	while (pos != NULL) {
		m_ProgEntries.GetNextAssoc(pos, Index, ProgEntry);

		if ( ProgEntry->m_Index == -1 )
		{
			continue;
		}

		GetSubItemRect(ProgEntry->m_Index, ProgEntry->m_SubIndex, LVIR_BOUNDS, ItemRect);

		int left = ItemRect.left;
		int top = ItemRect.top;
		//   int right = ItemRect.right;
			int right = (ItemRect.Width() > 100)? (ItemRect.right - 60): ItemRect.right;
		int bottom = ItemRect.bottom;

		if ( IsWindow((ProgEntry->m_Prog).GetSafeHwnd()) )
		{
			(ProgEntry->m_Prog).MoveWindow(left, top, (right - left), (bottom - top));
			(ProgEntry->m_Prog).EnableWindow(TRUE);
		}
	}
}

void CSkinListProgressCtrl::ResizeButton()
{
	CRect ItemRect;
	CButtonEntry* ButtonEntry=0;
	int Index=0;
	POSITION pos = m_ButtonEntries.GetStartPosition();
	while (pos != NULL) {
		m_ButtonEntries.GetNextAssoc(pos, Index, ButtonEntry);

		if ( ButtonEntry->m_Index == -1 )
		{
			continue;
		}

		GetSubItemRect(ButtonEntry->m_Index, ButtonEntry->m_SubIndex, LVIR_BOUNDS, ItemRect);

		int left =  (ItemRect.Width() > 20)? (ItemRect.left + 2 ): ItemRect.left;
		int top = ItemRect.top;
		//   int right = ItemRect.right;
			int right = (ItemRect.Width() > 100)? (ItemRect.right - 2): ItemRect.right;
		int bottom = ItemRect.bottom;

		if ( IsWindow((ButtonEntry->m_Btn).GetSafeHwnd()) )
		{
			(ButtonEntry->m_Btn).MoveWindow(left, top, (right - left), (bottom - top));
			(ButtonEntry->m_Btn).EnableWindow(TRUE);
		}

	}
}

void CSkinListProgressCtrl::CreateButton(int Index, int nSubItem, HWND hWnd, DWORD dwID  )
{
	// can only create progress for an existing item
	if (Index >= GetItemCount())
		return;
	CButtonEntry* ButtonEntry = new CButtonEntry(Index, nSubItem, hWnd);
	CRect ItemRect;
	GetSubItemRect(Index, ButtonEntry->m_SubIndex, LVIR_BOUNDS, ItemRect);
	//	GetSubItemRect(Index, nSubItem, LVIR_BOUNDS, ItemRect);
	int left = (ItemRect.Width() > 20)? (ItemRect.left + 2 ): ItemRect.left;
	int top = ItemRect.top;
	int right = (ItemRect.Width() > 100)? (ItemRect.right - 2): ItemRect.right;
	int bottom = ItemRect.bottom;

//	(ButtonEntry->m_Btn).m_hWnd1 = hWnd;
	ButtonEntry->dwMissionID = dwID;
	(ButtonEntry->m_Btn).Create("取消", WS_CHILD | WS_VISIBLE, CRect(left, top, right, bottom), this, dwID );
	m_ButtonEntries[Index] = ButtonEntry;

}

void CSkinListProgressCtrl::Reflash()
{
	int nItemCount =  GetItemCount();

	if ( nItemCount <= 0)
	{
		return;
	}

	

	for ( int i = 0; i < nItemCount; i++)
	{
		DWORD dwMissionID;
		dwMissionID = GetItemData( i );

		CButtonEntry* pButtonEntry = 0;
		POSITION pos = m_ButtonEntries.GetStartPosition();
		int Index = 0;
		while (pos != NULL) 
		{
			m_ButtonEntries.GetNextAssoc(pos, Index, pButtonEntry);
		//	if (pButtonEntry) delete pButtonEntry;
			if (pButtonEntry)
			{
				if (pButtonEntry->dwMissionID == dwMissionID)
				{
					pButtonEntry->m_Index = i;
					break;
				}
			}

		}

		CProgressEntry* pProgEntry = 0;
		pos = m_ProgEntries.GetStartPosition();
		Index = 0;
		while (pos != NULL) 
		{
			m_ProgEntries.GetNextAssoc(pos, Index, pProgEntry);
		//	if (pButtonEntry) delete pButtonEntry;
			if (pProgEntry)
			{
				if (pProgEntry->dwMissionID == dwMissionID)
				{
					pProgEntry->m_Index = i;
					break;
				}
			}

		}

	}


}


BOOL CSkinListProgressCtrl::DeleteItem( int nItem )
{
	CButtonEntry* pButtonEntry=0;

	int Index = 0;

	DWORD dwMissionID;
	dwMissionID = GetItemData( nItem );

	POSITION pos = m_ButtonEntries.GetStartPosition();

	while (pos != NULL) 
	{
		m_ButtonEntries.GetNextAssoc(pos, Index, pButtonEntry);
	//	if (pButtonEntry) delete pButtonEntry;
		if (pButtonEntry)
		{
			if (pButtonEntry->dwMissionID == dwMissionID)
			{
			//	TRACE("ShowWindow(%d)\n", dwMissionID);
			//	pButtonEntry->m_Btn.DestroyWindow();
			//	pButtonEntry->m_Btn.ShowWindow(SW_HIDE);
			//	if (pButtonEntry) delete pButtonEntry;
			//	m_ProgEntries.RemoveKey(Index);

				TRACE("DeleteItemButton(%d): MissionID = %d\n", Index, dwMissionID );

				pButtonEntry->m_Btn.ShowWindow(SW_HIDE);

				pButtonEntry->m_Index = -1;
				pButtonEntry->dwMissionID = -1;

				break;
			}
		}

	}

	CProgressEntry* pProgEntry = 0;
	pos = m_ProgEntries.GetStartPosition();
	Index = 0;
	while (pos != NULL) 
	{
		m_ProgEntries.GetNextAssoc(pos, Index, pProgEntry);
	//	if (pButtonEntry) delete pButtonEntry;
		if (pProgEntry)
		{
			if (pProgEntry->dwMissionID == dwMissionID)
			{
				//	TRACE("m_Prog.ShowWindow(%d)\n", dwMissionID);
			//	pProgEntry->m_Prog.ShowWindow(SW_HIDE);
			//	pProgEntry->m_Prog.DestroyWindow();
			//	pButtonEntry->m_Btn.ShowWindow(SW_HIDE);
			//	if (pProgEntry) delete pProgEntry;
			//	m_ProgEntries.RemoveKey(Index);
				TRACE("DeleteItemProg(%d): MissionID = %d\n", Index, dwMissionID );

				pProgEntry->m_Prog.ShowWindow(SW_HIDE);

				pProgEntry->m_Index = -1;
				pProgEntry->dwMissionID = -1;			
				break;
			}
		}

	}


	BOOL bResult = CListCtrl::DeleteItem( nItem );

	if ( GetItemCount() > 0 )
	{
		Reflash();
		ResizeButton();
		ResizeProg();
	}

	this->Invalidate();
	return bResult;
}


///////////////////////////////


CSkinListProgressCtrl::CProgressEntry::CProgressEntry(int Index, int SubIndex) :
	m_Index(Index), m_SubIndex(SubIndex)
{
}

/*
CProgressListCtrl::CProgressEntry::CProgressEntry() :
  m_Index(-1), m_SubIndex(-1), m_Prog(0)
{
}
*/

CSkinListProgressCtrl::CProgressEntry::~CProgressEntry()
{
}

/*
CProgressListCtrl::CProgressEntry& CProgressListCtrl::CProgressEntry::operator=(const CProgressEntry& pe)
{
  m_Index = pe.m_Index;
  m_SubIndex = pe.m_SubIndex;
  m_Prog = pe.m_Prog;
  return *this;
}

*/

CSkinListProgressCtrl::CButtonEntry::CButtonEntry(int Index, int SubIndex, HWND hWnd) :
	m_Btn(hWnd), m_Index(Index), m_SubIndex(SubIndex)
{
}

/*
CProgressListCtrl::CProgressEntry::CProgressEntry() :
  m_Index(-1), m_SubIndex(-1), m_Prog(0)
{
}
*/

CSkinListProgressCtrl::CButtonEntry::~CButtonEntry()
{
}
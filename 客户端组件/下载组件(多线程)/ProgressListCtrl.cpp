// ProgressListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DownLoadService.h"
#include "ProgressListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressListCtrl

CProgressListCtrl::CProgressListCtrl()
{
}

CProgressListCtrl::~CProgressListCtrl()
{
  CProgressEntry* pProgEntry=0;
  int Index=0;
  POSITION pos = m_ProgEntries.GetStartPosition();
  while (pos != NULL) {
    m_ProgEntries.GetNextAssoc(pos, Index, pProgEntry);
    if (pProgEntry) delete pProgEntry;
  }
  m_ProgEntries.RemoveAll();
}


BEGIN_MESSAGE_MAP(CProgressListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CProgressListCtrl)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressListCtrl message handlers

void CProgressListCtrl::CreateColumns()
{
  LVCOLUMN Col;
  char pColStr[101] = "First Column";

  InitHdr();
  Col.mask = LVCF_TEXT | LVCF_WIDTH;
  Col.cx = 125;
  Col.pszText = pColStr;
  Col.cchTextMax = 100;

  InsertColumn(0, &Col);

  Col.mask |= LVCF_SUBITEM;
  Col.iSubItem = 1;
  sprintf(pColStr, "Second Column");
  InsertColumn(1, &Col);

  Col.iSubItem = 2;
  sprintf(pColStr, "Third Column");
  InsertColumn(2, &Col);
}



void CProgressListCtrl::CreateProgress(int Index, int nSubItem)
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
  m_ProgEntries[Index] = ProgEntry;
}

void CProgressListCtrl::InsertSomeItems()
{
  char pItemText[101] = "This is the first item";
  LVITEM Item;
  Item.mask = LVIF_TEXT | LVIF_PARAM;
  Item.iItem = 0;
  Item.iSubItem = 0;
  Item.pszText = pItemText;
  Item.cchTextMax = 100;
  Item.lParam = 12345;

  InsertItem(&Item);

  sprintf(pItemText, "Second item (Subitem)");
  SetItemText(0, 1, pItemText);

  sprintf(pItemText, "Another Item in list");
  Item.iItem = 1;
  Item.lParam = 98765;
  InsertItem(&Item);

}


void CProgressListCtrl::SetProgress(int Index, int prog)
{
  CProgressEntry* ProgEntry;
  if (m_ProgEntries.Lookup(Index, ProgEntry) == TRUE)
    (ProgEntry->m_Prog).SetPos(prog);
}

void CProgressListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
  ResizeProg();
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CProgressListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
  ResizeProg();	
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CProgressListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
  ResizeProg();
}

void CProgressListCtrl::ResizeProg()
{
  CRect ItemRect;
  CProgressEntry* ProgEntry=0;
  int Index=0;
  POSITION pos = m_ProgEntries.GetStartPosition();
  while (pos != NULL) {
    m_ProgEntries.GetNextAssoc(pos, Index, ProgEntry);
    GetSubItemRect(ProgEntry->m_Index, ProgEntry->m_SubIndex, LVIR_BOUNDS, ItemRect);
    int left = ItemRect.left;
    int top = ItemRect.top;
    int right = ItemRect.right;
    int bottom = ItemRect.bottom;

  	(ProgEntry->m_Prog).MoveWindow(left, top, (right - left), (bottom - top));
  }
}

void CProgressListCtrl::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CListCtrl::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
  ResizeProg();
}


void CProgressListCtrl::InitHdr()
{
  m_AdvHdr.Init(GetHeaderCtrl());
}

CProgressListCtrl::CProgressEntry::CProgressEntry(int Index, int SubIndex) :
m_Index(Index), m_SubIndex(SubIndex)
{
}

/*
CProgressListCtrl::CProgressEntry::CProgressEntry() :
  m_Index(-1), m_SubIndex(-1), m_Prog(0)
{
}
*/

CProgressListCtrl::CProgressEntry::~CProgressEntry()
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

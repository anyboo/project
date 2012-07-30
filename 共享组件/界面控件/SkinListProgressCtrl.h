#pragma once

#include <afxtempl.h>
#include "SkinImage.h"
#include "SkinControls.h"
//#include "SkinAttribute.h"
#include "SkinResourceManager.h"
// CSkinListProgressCtrl
#include "SkinListCtrl.h"
//#include "SkinButton.h"
#include "DownloadButton.h"
#include "GradientProgressCtrl.h"

class SKIN_CONTROL_CLASS CSkinListProgressCtrl : public CSkinListCtrl
{
	DECLARE_DYNAMIC(CSkinListProgressCtrl)

public:
	CSkinListProgressCtrl();
	virtual ~CSkinListProgressCtrl();

	void CreateProgress(int Index, int nSubItem, DWORD dwID );
	void ResizeProg();
	void SetProgress(int Index, int prog);

	void CreateButton(int Index, int nSubItem, HWND hWnd, DWORD dwID );
	void ResizeButton();

	void Reflash();
	virtual BOOL DeleteItem( int nItem );
protected:

  class CProgressEntry {
  public:
    CProgressEntry(int Index, int SubIndex);
    virtual ~CProgressEntry();
//    CProgressEntry();

//    CProgressEntry& operator=(const CProgressEntry& pe);
    int m_Index;
    int m_SubIndex;
	DWORD	dwMissionID;
  	CGradientProgressCtrl m_Prog;
  };

  CMap < int, int, CProgressEntry*, CProgressEntry*& > m_ProgEntries;

 class CButtonEntry {
  public:
    CButtonEntry(int Index, int SubIndex, HWND hWnd);
    virtual ~CButtonEntry();
//    CProgressEntry();

//    CProgressEntry& operator=(const CProgressEntry& pe);
    int m_Index;
    int m_SubIndex;
  	CDownloadButton m_Btn;
	DWORD	dwMissionID;
  };

  CMap < int, int, CButtonEntry*, CButtonEntry*& > m_ButtonEntries;

protected:
	DECLARE_MESSAGE_MAP()

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
protected:
	virtual void PreSubclassWindow();

private:  
    CList<COLORREF*,   COLORREF*>   m_lstItemColor;   
	COLORREF						m_listItemColor1;

};



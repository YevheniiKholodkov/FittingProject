#ifndef BOUNDSTAB
#define BOUNDSTAB
#include <Dialog.h>
#include <..\Originlab\DialogEx.h>
#include <MsgMap.h>
#include "MultiNLFitDialogRes.h"
#include "resource.h"
#include "NLMultiFitSettings.h"

class BoundsList : public GridListControl
{
public:
	BoundsList();
	
	void Init(int nID, PropertyPage& page,LPCSTR lpcszDlgName, NLMultiFitSettings* settings);
	void refresh();
	
	void BeforeEdit(Control flxControl, long nRow, long nCol, BOOL* pCancel);
	void AfterEdit(Control flxControl, int nRow, int nCol);
	void OnCellChecked(Control flxControl);
	
	void FillGrid();
	void SetColHeader();
	
private:
	NLMultiFitSettings* mSettings;
};


class BoundsTab : public PropertyPage
{
public:
	// Constructor for BoundsTab page
	BoundsTab(int nID) : PropertyPage(nID) {}

	void init(int id, NLMultiFitSettings*  settings)
	{
		SetID(id);
		mSettings = settings;
	}
	
	// Event handlers for BoundsTab page
	EVENTS_BEGIN
		PAGE_ON_INIT(OnInitPage)
		PAGE_ON_ACTIVE(OnActivatePage)
		ON_GRID_BEFORE_EDIT(IDC_GRIDBOUNDS, OnBeforeEditColList)
        ON_GRID_AFTER_EDIT(IDC_GRIDBOUNDS, OnAfterEditColList)
		ON_GRID_SEL_CHANGE(IDC_GRIDBOUNDS, OnCellChecked)
	EVENTS_END
	
	BOOL OnInitPage();
	BOOL OnActivatePage();
    void OnBeforeEditColList(Control flxControl, long nRow, long nCol, BOOL* pCancel);
	void OnAfterEditColList(Control flxControl,int nRow,int nCol);
	void OnCellChecked(Control flxControl);
private:
	NLMultiFitSettings* mSettings;
	BoundsList         m_ColList;
};
#endif //BOUNDSTAB
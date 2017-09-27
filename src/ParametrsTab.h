

#include <Origin.h>
////////////////////////////////////////////////////////////////////////////////////
#include <..\Originlab\DialogEx.h>
#include <Dialog.h>
#include <..\system\MsgMap.h>
#include "MultiNLFitDialogRes.h"
#include <..\FittingProject\src\resource.h>
#include <..\FittingProject\src\NLMultiFitSettings.h>

    
class ParametersList : public GridListControl
{
public:
	ParametersList();
	
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

class ParametersTab : public PropertyPage
{
public:
	// Constructor for PrametersTab page
	ParametersTab(int nID) : PropertyPage(nID) {}

	void init(int id, NLMultiFitSettings*  settings)
	{
		SetID(id);
		mSettings = settings;
	}
	
	
	// Event handlers for PrametersTab page
	EVENTS_BEGIN
		PAGE_ON_INIT(OnInitPage)
		PAGE_ON_ACTIVE(OnActivatePage)
		ON_GRID_BEFORE_EDIT(IDC_GRIDPARAMETRS, OnBeforeEditColList)
        ON_GRID_AFTER_EDIT(IDC_GRIDPARAMETRS, OnAfterEditColList)
		ON_GRID_SEL_CHANGE(IDC_GRIDPARAMETRS, OnCellChecked)
	//	ON_BN_CLICKED(IDC_FITBUTTON, OnFitButtonClicked);
	EVENTS_END
	
	BOOL OnInitPage();
	BOOL OnActivatePage();
    void OnBeforeEditColList(Control flxControl, long nRow, long nCol, BOOL* pCancel);
	void OnAfterEditColList(Control flxControl,int nRow,int nCol);
	void OnCellChecked(Control flxControl);
	    
private:
	NLMultiFitSettings* mSettings;
	ParametersList         m_ColList;
};

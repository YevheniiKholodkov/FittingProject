#ifndef ADDITIONALTAB
#define ADDITIONALTAB
#include <Dialog.h>
#include <..\system\MsgMap.h>
#include "MultiNLFitDialogRes.h"
#include <..\Originlab\DialogEx.h>
#include <..\FittingProject\src\NLFitAdditionalParams.h>
#include <..\FittingProject\src\resource.h>
#include <..\FittingProject\src\NLMultiFitSettings.h>

#define STR_DLG_NAME                "AdditionalParams"

enum{
	 COL_NAME    = 0,
	 COL_COMPUTE,	
	 COL_COMMENT,
	 COL_TOTAL
    };
    
    
// an example grid with multiple columns 
class AdditionalParamsList : public GridListControl
{
public:
	AdditionalParamsList()  
		{
		}
	
	void Init(int nID, PropertyPage& page,LPCSTR lpcszDlgName, NLMultiFitSettings* settings)
	{			
		GridListControl::Init(nID, page);
		SetCols(COL_TOTAL);
		SetRows(AddParamNames.GetSize());
		
		SetColHeader();		
		FillGrid();		//Set the column's properties
		
		mSettings = settings;
		
		if(mSettings->isSave())
		{
			for(int i = 0; i < AddParamNames.GetSize(); ++i)
			{
				if(mSettings->isAddParamChecked(i))
					SetCheck(i + 1, COL_COMPUTE, true);
			}
		}
		//SetEditable(flexEDKbd);		//set the column can be edited
		//SetSelection( flexSelectionFree );		//set the selection mode
	}
	
	void BeforeEdit(Control flxControl, long nRow, long nCol, BOOL* pCancel)
	{
		if( pCancel )
			*pCancel = (nCol != COL_COMPUTE);
	}
	
	void FillGrid()
	{
		SetCells(AddParamNames, COL_NAME);
		SetCells(AddParamComments, COL_COMMENT);
		
		vector<string> vsChks;
		vsChks.SetSize(AddParamNames.GetSize());
		SetCells(vsChks, COL_COMPUTE);
		
	}
	void SetColHeader()
	{
		SetColHeading(COL_NAME,"Param");
		SetColDataType(COL_COMPUTE,flexDTBoolean);//set the column type to check box
		SetColHeading(COL_COMPUTE,"Check");
		
		SetColHeading(COL_COMMENT,"Comment");
	}
	
	void AfterEdit(Control flxControl, int nRow, int nCol)
	{
		if(nCol!=COL_COMPUTE)
			return;
		
		bool bCheck=GetCheck(nRow, nCol);
		if(bCheck)
		{
			SetCheck(nRow, COL_COMPUTE, true);
		}
		mSettings->checkAddParam(nRow - 1, bCheck);
	}
	
private:
	NLMultiFitSettings* mSettings;
};

class AdditionalTab : public PropertyPage
{
public:
	// Constructor for AdditionalTab page
	AdditionalTab(int nID) : PropertyPage(nID) {}

	void init(int id, NLMultiFitSettings*  settings)
	{
		SetID(id);
		mSettings = settings;
	}
	
	
	// Event handlers for AdditionalTab page
	EVENTS_BEGIN
		PAGE_ON_INIT(OnInitPage)
		PAGE_ON_ACTIVE(OnActivatePage)
		ON_GRID_BEFORE_EDIT(IDC_ADDITIONALTAB, OnBeforeEditColList)
        ON_GRID_AFTER_EDIT(IDC_ADDITIONALTAB, OnAfterEditColList)
	//	ON_BN_CLICKED(IDC_FITBUTTON, OnFitButtonClicked);
	EVENTS_END

	BOOL OnInitPage()
	{
		m_ColList.Init(IDC_ADDITIONALTAB, *this, STR_DLG_NAME, mSettings);
		return TRUE;		
	}

	BOOL OnActivatePage()
	{
		return TRUE;		
	}
	
    void OnBeforeEditColList(Control flxControl, long nRow, long nCol, BOOL* pCancel)
    {   
		m_ColList.BeforeEdit(flxControl, nRow, nCol, pCancel);
    }
    
	void OnAfterEditColList(Control flxControl,int nRow,int nCol)
	{   
		m_ColList.AfterEdit(flxControl,nRow,nCol);
	}	
	    
private:
	AdditionalParamsList         m_ColList;
	ListBox mAdditionalParams;
	ListBox mCheckedParams;
	NLMultiFitSettings* mSettings;
};
#endif //ADDITIONALTAB
#include <Origin.h>
#include "ParametrsTab.h"

enum {
	 COL_NO   = 0,
	 COL_NAME,
	 COL_MEANING,
	 COL_SHARED,	
	 COL_FIXED,
	 COL_VALUE,
	 COL_TOTAL
};

ParametersList::ParametersList()  
{
	mCurrentData = 0;
}
	
void ParametersList::Init(int nID, PropertyPage& page,LPCSTR lpcszDlgName, NLMultiFitSettings* settings)
{			
	GridListControl::Init(nID, page);
	mSettings = settings;
	
	SetColHeader();		
	FillGrid();		//Set the column's properties
	
}

void ParametersList::BeforeEdit(Control flxControl, long nRow, long nCol, BOOL* pCancel)
{
}

void ParametersList::refresh()
{
	HideCol(COL_SHARED, mSettings->getReplicas() > 0 ? false : true);
	FillGrid();
}

void ParametersList::setCurrentData(int data) 
{
	mCurrentData = data; 
	refresh();
}

void ParametersList::FillGrid()
{
	vector<string> names;
	mSettings->getNames(names);
	if(names.GetSize() == 0)
		return;
	SetRows(names.GetSize());
	
	vector<string> numbers;
	vector<string> meanings;
	vector<double> values;
	vector<bool> shareds;
	vector<bool> fixeds;
	mSettings->getUnitNumbers(numbers);
	mSettings->getMeanings(meanings);
	mSettings->getValues(values, mCurrentData);
	mSettings->getShareds(shareds, mCurrentData);
	mSettings->getFixeds(fixeds, mCurrentData);
	
	vector<string> strValues(values.GetSize());
	for(int i = 0; i < values.GetSize(); ++i)
	{
		strValues[i] = values[i];
	}
	SetCells(numbers, COL_NO);
	SetCells(names, COL_NAME);
	SetCells(meanings, COL_MEANING);
	SetCells(strValues, COL_VALUE);
	
	vector<string> vsChks;
	vsChks.SetSize(names.GetSize());
	if(mSettings->getReplicas() == 0)
	{
		SetCells(vsChks, COL_SHARED);
	}
	SetCells(vsChks, COL_FIXED);
	for(i = 0; i < shareds.GetSize(); ++i)
	{
		if(mSettings->getReplicas() > 0 && shareds[i])
		{
			SetCheck(i + 1, COL_SHARED, true);
			vector<int> indexes;
			mSettings->getDublicateParamIndexes(indexes, i);
			for(int j = 0; j < indexes.GetSize(); ++j)
			{
				HideRow(indexes[j] + 1, true);
				SetCheck(indexes[j] + 1, COL_SHARED, true);
			}
		}
		else
		{
			SetCheck(i + 1, COL_SHARED, false);
			vector<int> indexes;
			mSettings->getDublicateParamIndexes(indexes, i);
			for(int j = 0; j < indexes.GetSize(); ++j)
			{
				HideRow(indexes[j] + 1, false);
				SetCheck(indexes[j] + 1, COL_SHARED, false);
			}
		}
	}
	for(i = 0; i < fixeds.GetSize(); ++i)
	{
		if(fixeds[i])
		{
			SetCheck(i + 1, COL_FIXED, true);
		}
	}
}
void ParametersList::SetColHeader()
{
	SetCols(COL_TOTAL);
	
	SetColHeading(COL_NO,"No.");
	SetColHeading(COL_NAME,"Name");
	SetColHeading(COL_MEANING,"Meaning");
	SetColDataType(COL_SHARED,flexDTBoolean);//set the column type to check box
	SetColHeading(COL_SHARED,"Share");
	SetColDataType(COL_FIXED,flexDTBoolean);
	SetColHeading(COL_FIXED,"Fixed");
	SetColHeading(COL_VALUE,"Value");
}

void ParametersList::OnCellChecked(Control flxControl)
{
	int nRow, nCol;
	GetMouseCell(nRow, nCol);
	if(nCol == COL_NAME || nCol == COL_MEANING )
	{
		SetEditable(flexEDNone);
	}
	else
	{
		SetEditable(flexEDKbdMouse);
	}
}
void ParametersList::AfterEdit(Control flxControl, int nRow, int nCol)
{
	if(nCol == COL_SHARED || nCol == COL_FIXED)
	{
		bool bCheck=GetCheck(nRow, nCol);
		if(bCheck)
		{
			if(nCol == COL_FIXED)
				SetCheck(nRow, COL_FIXED, true);
		}
		if(nCol == COL_SHARED)
		{
			int d = mSettings->getParamIndexWithoutReplica(nRow - 1) + 1;
			SetCheck(mSettings->getParamIndexWithoutReplica(nRow - 1) + 1, COL_SHARED, bCheck);
			mSettings->setShared(nRow - 1, mCurrentData, bCheck);
			vector<int> indexes;
			mSettings->getDublicateParamIndexes(indexes, nRow - 1);
			for(int i = 0; i < indexes.GetSize(); ++i)
			{
				HideRow(indexes[i] + 1, bCheck);
				SetCheck(indexes[i] + 1, COL_SHARED, bCheck);
			}
		}
		if(nCol == COL_FIXED)
			mSettings->setFixed(nRow - 1, mCurrentData, bCheck);
	}
	else if(nCol == COL_VALUE)
	{
		mSettings->setValue(nRow - 1, mCurrentData, atof(GetCell(nRow, nCol)));
	}
}


BOOL ParametersTab::OnInitPage()
{
	m_ColList.Init(IDC_GRIDPARAMETRS, *this, "Parameters", mSettings);
	
	mDataList = GetItem(IDC_DATALIST);
	for(int i = 0; i < mSettings->getDataNumber(); ++i)
	{
		string str = i;
		mDataList.AddString(str);
	}
	return TRUE;		
}

BOOL ParametersTab::OnActivatePage()
{
	mSettings->updateSessionParametrsIfNeeded();
	mSettings->updateParameterNamesIfNeeded();
	m_ColList.refresh();
	return TRUE;		
}

void ParametersTab::OnBeforeEditColList(Control flxControl, long nRow, long nCol, BOOL* pCancel)
{   
	m_ColList.BeforeEdit(flxControl, nRow, nCol, pCancel);
}

void ParametersTab::OnAfterEditColList(Control flxControl,int nRow,int nCol)
{   
	m_ColList.AfterEdit(flxControl,nRow,nCol);
}	

void ParametersTab::OnCellChecked(Control flxControl)
{
	m_ColList.OnCellChecked(flxControl);
}

void ParametersTab::OnListDataChecnged(Control flxControl)
{
	int data = mDataList.GetCurSel();
	m_ColList.setCurrentData(data);
}
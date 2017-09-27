#include <Origin.h>
#include <..\FittingProject\src\ParametrsTab.h>

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

void ParametersList::FillGrid()
{
	vector<string> names;
	mSettings->getNames(names);
	if(names.GetSize() == 0)
		return;
	SetRows(names.GetSize());
	
	vector<string> numbers;
	vector<string> meanings;
	vector<string> values;
	vector<bool> shareds;
	vector<bool> fixeds;
	mSettings->getUnitNumbers(numbers);
	mSettings->getMeanings(meanings);
	mSettings->getValues(values);
	mSettings->getShareds(shareds);
	mSettings->getFixeds(fixeds);

	SetCells(numbers, COL_NO);
	SetCells(names, COL_NAME);
	SetCells(meanings, COL_MEANING);
	SetCells(values, COL_VALUE);
	
	vector<string> vsChks;
	vsChks.SetSize(names.GetSize());
	if(mSettings->getReplicas() == 0)
	{
		SetCells(vsChks, COL_SHARED);
	}
	SetCells(vsChks, COL_FIXED);
	for(int i = 0; i < shareds.GetSize(); ++i)
	{
		if(mSettings->getReplicas() > 0 && shareds[i])
		{
			SetCheck(i, COL_SHARED, true);
		}
		if(fixeds[i])
		{
			SetCheck(i, COL_FIXED, true);
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
			if(nCol == COL_SHARED)
			{
				SetCheck(nRow, COL_SHARED, true);
			}
			if(nCol == COL_FIXED)
				SetCheck(nRow, COL_FIXED, true);
		}
		if(nCol == COL_SHARED)
		{
			mSettings->setShared(nRow - 1, bCheck);
			vector<int> indexes;
			mSettings->getDublicateParamIndexes(indexes, nRow - 1);
			for(int i = 0; i < indexes.GetSize(); ++i)
				HideRow(indexes[i] + 1, bCheck);
		}
		if(nCol == COL_FIXED)
			mSettings->setFixed(nRow - 1, bCheck);
	}
	else if(nCol == COL_VALUE)
	{
		mSettings->setValue(nRow - 1, atof(GetCell(nRow, nCol)));
	}
}


BOOL ParametersTab::OnInitPage()
{
	m_ColList.Init(IDC_GRIDPARAMETRS, *this, "Parameters", mSettings);
	return TRUE;		
}

BOOL ParametersTab::OnActivatePage()
{
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
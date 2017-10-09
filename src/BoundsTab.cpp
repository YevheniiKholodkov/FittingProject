
#include <Origin.h>
#include "BoundsTab.h"

enum {
	 COL_NO   = 0,
	 COL_MEANING,
	 COL_VALUE,	
	 COL_LOWERBOUNDS,
	 COL_SIGHT1,
	 COL_PARAM,
	 COL_SIGHT2,
	 COL_UPPERBOUNDS,
	 COL_TOTAL
};

BoundsList::BoundsList()  
{
}
	
void BoundsList::Init(int nID, PropertyPage& page,LPCSTR lpcszDlgName, NLMultiFitSettings* settings)
{			
	GridListControl::Init(nID, page);
	mSettings = settings;
	ResizeCols(80, 8, 3, 3, flexAutoSizeNoFixed);
	SetColHeader();		
	FillGrid();		//Set the column's properties
		SetSelection( flexSelectionByRow );	
	
}

void BoundsList::BeforeEdit(Control flxControl, long nRow, long nCol, BOOL* pCancel)
{
}

void BoundsList::refresh()
{
	FillGrid();
}

void BoundsList::FillGrid()
{
	vector<string> names;
	mSettings->getNames(names);
	if(names.GetSize() == 0)
		return;
	SetRows(names.GetSize());
	
	vector<string> meanings;
	vector<string> numbers;
	vector<double> values;
	vector 		   lowerBounds;
	vector<int>    lowerControls;
	vector 		   upperBounds;
	vector<int>    upperControls;
	mSettings->getMeanings(meanings);
	mSettings->getUnitNumbers(numbers);
	mSettings->getValues(values, 0);
	mSettings->getLowerBounds(lowerBounds);
	mSettings->getLowerLimitControls(lowerControls);
	mSettings->getUppperBounds(upperBounds);
	mSettings->getUpperLimitControls(upperControls);
	
	vector<string> strValues(values.GetSize());
	vector<string> lowerSight(values.GetSize());
	vector<string> upperSight(values.GetSize());
	vector<string> lowerStrValue(values.GetSize());
	vector<string> upperStrValue(values.GetSize());
	for(int i = 0; i < values.GetSize(); ++i)
	{
		strValues[i] = values[i];
		if(i < lowerBounds.GetSize())
		{
			lowerStrValue[i] = lowerBounds[i];
		}
		else
		{
			lowerStrValue[i] = "--";
		}
		if(i < upperBounds.GetSize())
		{
			upperStrValue[i] = upperBounds[i];
		}
		else
		{
			upperStrValue[i] = "--";
		}
		if(i < lowerControls.GetSize())
		{
			if(lowerControls[i] == LIMIT_EXCLUSIVELESS)
				lowerSight[i] = "<";
			else if(lowerControls[i] == LIMIT_LESS)
				lowerSight[i] = "<=";
		}
		if(i < upperControls.GetSize())
		{
			if(upperControls[i] == LIMIT_EXCLUSIVELESS)
				upperSight[i] = "<";
			else if(upperControls[i] == LIMIT_LESS)
				upperSight[i] = "<=";
		}
	}
	
	SetCells(numbers, COL_NO);
	SetCells(meanings, COL_MEANING);
	SetCells(strValues, COL_VALUE);
	SetCells(names, COL_PARAM);
	SetCells(lowerStrValue, COL_LOWERBOUNDS);
	SetCells(lowerSight, COL_SIGHT1);
	SetCells(upperStrValue, COL_UPPERBOUNDS);
	SetCells(upperSight, COL_SIGHT2);
	
	SetCell(1, flexcpAlignment, 1, COL_PARAM);
}
void BoundsList::SetColHeader()
{
	SetCols(COL_TOTAL);
	
	SetColHeading(COL_NO,"NO.");
	SetColHeading(COL_MEANING,"Meaning");
	SetColHeading(COL_VALUE,"Value");
	SetColHeading(COL_LOWERBOUNDS,"Lower Bounds");
	SetColDataType(COL_LOWERBOUNDS,flexDTDouble);
	SetColHeading(COL_SIGHT1,"< or <=");
	SetColHeading(COL_PARAM,"Param");
	SetColAlignment(COL_PARAM, flexAlignCenterCenter);
	SetColDataType(COL_PARAM,flexDTVariant);
	SetColComboList(COL_SIGHT1," |<|<=");
	//SetEditable(flexEDNone);
	
	SetColHeading(COL_SIGHT2,"< or <=");
	SetColComboList(COL_SIGHT2," |<|<=");
	SetColHeading(COL_UPPERBOUNDS,"Upper Bounds");
	SetColDataType(COL_UPPERBOUNDS,flexDTDouble);
}

void BoundsList::AfterEdit(Control flxControl, int nRow, int nCol)
{
	if(nCol == COL_SIGHT1)
	{
		string str = GetCell(nRow, nCol);
		if(str == " ")
		{
			mSettings->setLowerLimitControls(nRow - 1, LIMIT_OFF);
		}
		else if(str == "<")
		{
			mSettings->setLowerLimitControls(nRow - 1, LIMIT_EXCLUSIVELESS);
		}
		else if(str == "<=")
		{
			mSettings->setLowerLimitControls(nRow - 1, LIMIT_LESS);
		}
	}
	else if(nCol == COL_SIGHT2)
	{
		string str = GetCell(nRow, nCol);
		if(str == " ")
		{
			mSettings->setUpperLimitControls(nRow - 1, LIMIT_OFF);
		}
		else if(str == "<")
		{
			mSettings->setUpperLimitControls(nRow - 1, LIMIT_EXCLUSIVELESS);
		}
		else if(str == "<=")
		{
			mSettings->setUpperLimitControls(nRow - 1, LIMIT_LESS);
		}
	}
	else if(nCol == COL_LOWERBOUNDS)
	{
		double value = atof(GetCell(nRow, nCol));
		mSettings->setLowerBound(nRow - 1, value);
	}
	else if(nCol == COL_UPPERBOUNDS)
	{
		double value = atof(GetCell(nRow, nCol));
		mSettings->setUpperBound(nRow - 1, value);
	}
}

void BoundsList::OnCellChecked(Control flxControl)
{
	int nRow, nCol;
	GetMouseCell(nRow, nCol);
	string sight1 = GetCell(nRow, COL_SIGHT1);
	string sight2 = GetCell(nRow, COL_SIGHT2);
	if(nCol == COL_NO || nCol == COL_MEANING ||  nCol == COL_VALUE ||  nCol == COL_PARAM ||
	  (nCol == COL_LOWERBOUNDS && (sight1 == " " || sight1.IsEmpty())) ||
	  (nCol == COL_UPPERBOUNDS && (sight2 == " " || sight2.IsEmpty())))
	{
		SetEditable(flexEDNone);
	}
	else
	{
		SetEditable(flexEDKbdMouse);
	}
}

BOOL BoundsTab::OnInitPage()
{
	m_ColList.Init(IDC_GRIDBOUNDS, *this, "Bounds", mSettings);
	return TRUE;		
}

BOOL BoundsTab::OnActivatePage()
{
	mSettings->updateParameterNamesIfNeeded();
	mSettings->updateBoundsIfNeeded();
	m_ColList.refresh();
	return TRUE;		
}

void BoundsTab::OnBeforeEditColList(Control flxControl, long nRow, long nCol, BOOL* pCancel)
{   
	m_ColList.BeforeEdit(flxControl, nRow, nCol, pCancel);
}

void BoundsTab::OnAfterEditColList(Control flxControl,int nRow,int nCol)
{   
	m_ColList.AfterEdit(flxControl,nRow,nCol);
}	

void BoundsTab::OnCellChecked(Control flxControl)
{
	m_ColList.OnCellChecked(flxControl);
}
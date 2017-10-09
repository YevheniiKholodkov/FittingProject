/*------------------------------------------------------------------------------*
 * File Name:				 													*
 * Creation: 																	*
 * Purpose: OriginC Source C file												*
 * Copyright (c) ABCD Corp.	2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010		*
 * All Rights Reserved															*
 * 																				*
 * Modification Log:															*
 *------------------------------------------------------------------------------*/
 
////////////////////////////////////////////////////////////////////////////////////
// Including the system header file Origin.h should be sufficient for most Origin
// applications and is recommended. Origin.h includes many of the most common system
// header files and is automatically pre-compiled when Origin runs the first time.
// Programs including Origin.h subsequently compile much more quickly as long as
// the size and number of other included header files is minimized. All NAG header
// files are now included in Origin.h and no longer need be separately included.
//
// Right-click on the line below and select 'Open "Origin.h"' to open the Origin.h
// system header file.
#include <Origin.h>
////////////////////////////////////////////////////////////////////////////////////
#include <Dialog.h>

////////////////////////////////////////////////////////////////////////////////////
// Include your own header files here.
#include "MultiNLFitDialogRes.h" // resource DLL header
#include "MultiNLFitDialog.h" // dialog class header
#include <..\originlab\NLFitSession.h>

//////// ////////////////////////////////////////////////////////////////////////////
// Start your functions here.

	
BOOL MultiNLFitDlg::OnInitDialog()
{
	mSaveCheckBox = GetItem(IDC_SAVESETTINGSCHECK);
	mReverseCheckBox = GetItem(IDC_CHECKREVERSE);
	mEditFirst = GetItem(IDC_EDITFIRST);
	mEditLast = GetItem(IDC_EDITLAST);
	m_PlaceHolder.Create(IDC_TAB_PLACEHOLDER, *this);
	m_PlaceHolder.init(mSettings);
	m_PlaceHolder.InitMaps();
	
	mSaveCheckBox.Check = mSettings->isSave();
	mReverseCheckBox.Check = mSettings->getReverse();
	return TRUE;
}

BOOL MultiNLFitDlg::OnClose()
{
	mSettings->saveSettings();
	return TRUE;
}

BOOL MultiNLFitDlg::OnClickBuildFirstGraph(Control ctrl)
{
	mCurrentPeak = 1;
	mSettings->buildPeak(mCurrentPeak);
	return true;
}

BOOL MultiNLFitDlg::OnSystemCommand(int nCmd)
{
	if( SC_MINIMIZE == nCmd)
	{
		BOOL bClose = !IsRolledup();
			Enable = bClose;
		if(bClose)
		{
			if ( OM_RETURN_NOT_PRESENT == OptionalMessageStatus("NLFitMinimizeToModalless") )
				OptionalMessage("ClickMinimizeToRollDown", MB_OK);
			bool res = Rollup(bClose);
			ShowWindow(SW_HIDE);
			
			return FALSE;
		}
		else
			ShowWindow(SW_NORMAL);
	}
	return TRUE;
}

BOOL MultiNLFitDlg::OnClickBuildNextGraph(Control ctrl)
{
	mSettings->buildPeak(++mCurrentPeak);
	return true;
}

BOOL MultiNLFitDlg::OnClickBuildNGraphs(Control ctrl)
{
	int first = atoi(mEditFirst.Text);
	int last = atoi(mEditLast.Text);
	for(int i = first; i <= last; ++i)
		mSettings->buildPeak(i);
	return true;
}

BOOL MultiNLFitDlg::OnFitButtonClicked(Control ctrl)
{
	mSettings->fitAll();
	OnClose();
	Close();
	return true;
}

BOOL MultiNLFitDlg::OnClickSetReverse(Control ctrl)
{
	mSettings->setReverse(mReverseCheckBox.Check);
	return true;
}

BOOL MultiNLFitDlg::OnClickSetSave(Control ctrl)
{
	mSettings->saveSettings(mSaveCheckBox.Check);
	return true;
}

void MultiNLFitDlg::OnActivate(UINT nState, HWND hwndOther, BOOL bMinimized)
{
}
	
bool DoMultiNLFitDlg() 
{
	Worksheet wks = Project.ActiveLayer();
	if(!wks)
	{
		MessageBox(GetWindow(), "There are no active worksheet", "Error", MB_OK);
	    return false;
	}
	else
	{
		static MultiNLFitDlg myDlg;
		myDlg.Create(GetWindow()/*, DLG_HIDDEN| DLG_OPTIONS_SHIFT_SHOW_DEFAULT_TRUE | DLG_OPTIONS_USE_OPS */);
	}
	return true;
}

void Datasheet_SetReportTree_Ex2()
{
    int nID = 100; // Each node must have node ID and node ID must be unique    
    int nTableFormat = GETNBRANCH_OPEN | GETNBRANCH_HIDE_COL_HEADINGS| GETNBRANCH_HIDE_ROW_HEADINGS | GETNBRANCH_FIT_COL_WIDTH | GETNBRANCH_FIT_ROW_HEIGHT;
    
    // 1. Create report tree
    Tree tr;
    tr.Report.ID = nID++; 
    tr.Report.SetAttribute(STR_LABEL_ATTRIB, "Dest Stats Report"); //Table title
    // TREE_Table attribute is critical in getting the report to work so must be present in every table level. 
    // Can set this attribute as 0 without any format, but many bits GETNBRANCH_* defined in oc_const.h to set table display format.
    tr.Report.SetAttribute(TREE_Table, nTableFormat); 
 
    
    // 2. Prepare the 1st table Descriptive Statistics and show values got from one structure
    tr.Report.Table1.ID = nID++;
    tr.Report.Table1.SetAttribute(STR_LABEL_ATTRIB, "Descriptive Statistics"); // Table title. If not set this, will show as empty here
    tr.Report.Table1.SetAttribute(TREE_Table, nTableFormat); 
    
    tr.Report.Table1.C1.ID = nID++;
    tr.Report.Table1.C1.SetAttribute(STR_LABEL_ATTRIB, "Result");
    
    tagtTestDescStats stRes; 
    stRes.N = 100;
    stRes.Mean = 604.72;
    stRes.SD = 760.19;
    stRes.SEM = 76.02;  
    
    // Add nodes with values and IDs from structure to tree
    tr.Report.Table1.C1 += stRes;
    
    // !! Please some details of tagtTestDescStats in stats_types.h, already define ID when declaration this structure
    // If stRes is a user defined structure without ID, here need to assign node ID for each child nodes of trCol, for example:
    //foreach(TreeNode trN in tr.Report.Table1.C1.Children)
    //{
        //trN.ID = nID++;
    //}
    
    // Set label for each row
    tr.Report.Table1.C1.N.SetAttribute(STR_LABEL_ATTRIB, "N total");
    tr.Report.Table1.C1.Mean.SetAttribute(STR_LABEL_ATTRIB, "Mean");
    tr.Report.Table1.C1.SD.SetAttribute(STR_LABEL_ATTRIB, "Standard Deviation");
    tr.Report.Table1.C1.SEM.SetAttribute(STR_LABEL_ATTRIB, "SE of Mean");   
 
    
    // 3. Prepare the 2nd table and show values got from vectors
    tr.Report.Table2.ID = nID++;
    tr.Report.Table2.SetAttribute(STR_LABEL_ATTRIB, "Extreme Values"); //Table tile
    tr.Report.Table2.SetAttribute(TREE_Table, nTableFormat | GETNBRANCH_TRANSPOSE); 
    
    tr.Report.Table2.C1.ID = nID++;
    tr.Report.Table2.C1.SetAttribute(STR_LABEL_ATTRIB, "Max Values"); // Column label
    
    tr.Report.Table2.C2.ID = nID++;
    tr.Report.Table2.C2.SetAttribute(STR_LABEL_ATTRIB, "Min Values"); // Column label       
    
    // Put data from vector to table columns
    vector vMaxDatas = {100, 98, 95, 92, 84};
    vector vMinDatas = {5, 9, 13, 18, 21};
    tr.Report.Table2.C1.dVals = vMaxDatas; 
    tr.Report.Table2.C2.dVals = vMinDatas;  
    
    // 4. Put graph page inside report table
    GraphPage gp;
    gp.Create();
    tr.Report.Table3.ID = nID++;    
    tr.Report.Table3.SetAttribute(STR_LABEL_ATTRIB, "Graphs"); //Table tile
    tr.Report.Table3.SetAttribute(TREE_Table, nTableFormat | GETNBRANCH_TRANSPOSE); 
    
    tr.Report.Table3.C1.ID = nID++;
    tr.Report.Table3.C1.strVal = gp.GetUID(true);
    tr.Report.Table3.C1.SetAttribute(STR_LABEL_ATTRIB, "Graph1");
    tr.Report.Table3.C1.SetAttribute(TREE_Control, ONODETYPE_EMBED_GRAPH);
    
    GraphPage gp2;
    gp2.Create();
    tr.Report.Table3.C2.ID = nID++;
    tr.Report.Table3.C2.strVal = gp2.GetUID(true);
    tr.Report.Table3.C2.SetAttribute(STR_LABEL_ATTRIB, "Graph2");
    tr.Report.Table3.C2.SetAttribute(TREE_Control, ONODETYPE_EMBED_GRAPH);
    
    // 5. Prepare worksheet window to report
    WorksheetPage wksPage;
    wksPage.Create();   
    
    DWORD   dwOptions = WP_SHEET_HIERARCHY | CREATE_NO_DEFAULT_TEMPLATE;        
    string  strSheetName = "Report Sheet";
    int nn = wksPage.AddLayer(strSheetName, dwOptions);
    if( nn < 0 )    
        return; 
    
    Worksheet wksOut = wksPage.Layers(nn);
    wksPage.Layers(0).Delete(); //delete the first default layer    
    
    
    // 6. Do report
    if( wksOut.SetReportTree(tr.Report) < 0 ) // Returns last row number on successful exit and -1 on failure.
    {
        printf("Fail to set report tree.\n");
        return;
    }       
    wksOut.AutoSize();  
}

/*
void Datasheet_SetReportTree()
{
	string strGraphName = "Graph1";
	GraphPage gp(strGraphName);
	if(!gp)
	{
		printf("No graph named %s in project\n", strGraphName);
		return;
	}
	
	int nID = 100; // Each node must have node ID and node ID must be unique	
	int nTableFormat = GETNBRANCH_OPEN | GETNBRANCH_HIDE_COL_HEADINGS| GETNBRANCH_HIDE_ROW_HEADINGS | GETNBRANCH_FIT_COL_WIDTH | GETNBRANCH_FIT_ROW_HEIGHT;
	
	// 1. Create report tree
	Tree tr;
	tr.Report.ID = nID++; 
	tr.Report.SetAttribute(STR_LABEL_ATTRIB, "Report with Embedded Graph"); //Table title
	// TREE_Table attribute is critical in getting the report to work so must be present in every table level. 
	// Can set this attribute as 0 without any format, but many bits GETNBRANCH_* defined in oc_const.h to set table display format.
	tr.Report.SetAttribute(TREE_Table, nTableFormat); 	

	// 2. Put graph page inside report table
	tr.Report.Table1.ID = nID++;	
	tr.Report.Table1.strVal = gp.GetUID(true);
	tr.Report.Table1.SetAttribute(TREE_Control, ONODETYPE_EMBED_GRAPH);		

	// 3. Prepare worksheet window to report
	WorksheetPage wksPage;
	wksPage.Create("origin", CREATE_EMPTY);	
	wksPage.SetShow();	
	
	DWORD 	dwOptions = WP_SHEET_HIERARCHY | CREATE_NO_DEFAULT_TEMPLATE;		
	string	strSheetName = "Report Sheet";
	int nn = wksPage.AddLayer(strSheetName, dwOptions);
	if( nn < 0 )	
		return;		
	
	// 4. Do report
	Worksheet wksOut = wksPage.Layers(nn);
	if( wksOut.SetReportTree(tr.Report) < 0 ) // Returns last row number on successful exit and -1 on failure.
	{
		printf("Fail to set report tree.\n");
		return;
	}		
	wksPage.Rename("ReportSample");
	wksOut.AutoSize();	
}*/
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
	m_PlaceHolder.Create(IDC_TAB_PLACEHOLDER, *this);
	m_PlaceHolder.init(mSettings);
	m_PlaceHolder.InitMaps();
	
	mSaveCheckBox.Check = mSettings->isSave();
	return TRUE;
}

BOOL MultiNLFitDlg::OnClose()
{
	mSettings->saveSettings();
	return TRUE;
}

BOOL MultiNLFitDlg::OnCancelClosed(Control ctrl)
{
	OnClose();
	Close();
	return TRUE;
}

BOOL MultiNLFitDlg::OnFitButtonClicked(Control ctrl)
{
	mSettings->fit();
	Close();
	return true;
}

BOOL MultiNLFitDlg::OnClickSetSave(Control ctrl)
{
	mSettings->saveSettings(mSaveCheckBox.Check);
	return true;
}
	
bool MultiNLFitDlg::fit()
{
	//const int nMaxNumIterations = 500;
	Worksheet wks = Project.ActiveLayer();
	if(!wks)
	    return false;
	
	Worksheet wksOutput;
	wksOutput.Create();
	
	NLFitSession    FitSession;
	int             nDataIndex = 0; // only one set in our case
	DWORD           dwRules = DRR_GET_DEPENDENT | DRR_NO_FACTORS;
	
	// 1. Set fucntion
	string func = mSettings->getFunction();
	int rep = mSettings->getReplicas();
	if(!FitSession.SetFunction(mSettings->getFunction(), NULL/*FitSetting.mFunctionCategory*/,mSettings->getReplicas() + 1)) // set function, category name can be ignore
	    return error_report("invalid fit function");
	
  //  FitSession.SetIterationSettings(nMaxNumIterations);
	vector<string>  vsParamNames;
	int             nNumParamsInFunction = FitSession.GetParamNamesInFunction(vsParamNames);
	int 			nFitOutcome, nFitErr = -1;
	
	for(int nYCol = 1; nYCol < wks.GetNumCols(); nYCol++)
	{
	    DataRange   drInputData;
	    drInputData.Add(wks, 0, "X");
	    drInputData.Add(wks, nYCol, "Y");
	    int         nNumData = drInputData.GetNumData(dwRules);
	    ASSERT(1==nNumData);
	    
	    //2 set the dataset
	    vector  vX1, vY1;
	    drInputData.GetData( dwRules, nDataIndex, NULL, NULL, &vY1, &vX1 );    
        FitSession.SetSrcDataRange(drInputData); 
        
        
	    if(!FitSession.SetData(vY1, vX1, NULL, nDataIndex, nNumData))  
	        return error_report("err setting data");
	
	    FitSession.SetParamShare(1);
	 //   FitSession.SetParamShare(5);
	   /* FitSession.SetParamShare(5);
	    FitSession.SetParamShare(6);*/
	    
	/*vector			vParams(7);			// this vector should be initialized to the total number of paramaters
	// This vector will be used both to set initial paramater values, and
	// to receive the parameter values after fitting:
	vParams[0] = 1.66;				// y0, not shared 
	vParams[1] = 2.5;				// xc, not shared
	vParams[2] = 0.3;				// w, shared (group 1)
	vParams[3] = 50;				// A, shared (group 2)
	vParams[4] = 7.5;				// xc for the second peak, not shared
	vParams[5] = 0.3;				// w, shared (group 1)
	vParams[6] = 50;				// A, shared (group 2)
	int nn = FitSession.SetParamValues(vParams);
	    
	if ( 0 != nn )
	{
		out_str("Invalid paramater setting!");
		return false;
	}	*/
	    // 3. Call parameter init codes to init parameters
	    if(!FitSession.ParamsInitValues())
	        return error_report("err ParamsInitValues");
	    
	    vector<string> names;
	    vector paramValues;
	    vector<int> paramOffsets;
	    FitSession.GetParamValuesAndOffsets(paramValues, paramOffsets);
	    FitSession.GetParamNamesInFunction(names, 0, true);
	    for(int i =0; i < paramValues.GetSize(); ++i)
	    {
	    	printf("[%d] -> name - , param - %f \n ", i/*, names[i]*/, paramValues[i]); 
	    }
	    for(int a =0; a < names.GetSize(); ++a)
	    {
	    	printf("[%d] -> name - %s \n ", a, names[a]); 
	    }
	    ////////////////////////////////
		RegStats		fitStats1;
		FitParameter	params1[20];
		FitSession.GetFitResultsParams(params1, &fitStats1);
	int index1 = 0;
	for(; index1 < 8; index1++)
	{
		printf("param[%d]: LCL - %f\n", index1, params1[index1].LCL);
	}/////////////////////////////////////////////
	    // 4. Iterate with default settings
	    if(!FitSession.Fit(&nFitOutcome))
	    {
        	string strOutcome = FitSession.GetFitOutCome(nFitOutcome);
	        printf("fit failed:%d->%s\n", nFitErr, strOutcome);
	       // return false;
	    }
	    
		// 5. success, get results and put to wksOutput
		FitParameter	params[20];// whatever the max possible number of parameters in your function
		RegStats		fitStats;
		NLSFFitInfo		fitInfo;
		FitSession.GetFitResultsStats(&fitStats, &fitInfo, false, nDataIndex);
		FitSession.GetFitResultsParams(params, &fitStats);
		
	int index = 0;
	for(; index < 8; index++)
	{
		printf("param[%d]:%f\n", index, params[index].Value);
	}
		appendFitResults(wksOutput, vsParamNames, params, fitStats, fitInfo);
	}
	return true;
}
void MultiNLFitDlg::appendFitResults(Worksheet& wks, const vector<string>& vsParamNames, const FitParameter* pParams, const RegStats& fitStats, const NLSFFitInfo& fitInfo)
{
	Column	c0Time = wks.Columns(0);
	int		nR1 = c0Time.GetUpperBound();
	
	// check if empty, then setup cols
	if(nR1 < 0)
	{
		int nTotalCols = vsParamNames.GetSize() + mSettings->getCheckedAddParamCount();
		if(wks.GetNumCols() < nTotalCols)
		{
			wks.SetSize(0, nTotalCols);	
			vector<unsigned int> paramIndexes;
			mSettings->getCheckedAddIndexes(paramIndexes);
			
			int nCol = 0;
			int index = 0;
			while(nCol < nTotalCols)
			{
				Column column = wks.Columns(nCol);
				if(nCol < vsParamNames.GetSize())
				{
					column.SetLongName(vsParamNames[nCol]);
				}
				else
				{
					column.SetLongName(AddParamNames[paramIndexes[index]]);
					column.SetComments(AddParamComments[paramIndexes[index]]);
					++index;
				}
				++nCol;
			}
		}
	}
	nR1++; 
	//fit parameters
	int index = 0;
	for(; index < vsParamNames.GetSize(); index++)
	{
		wks.SetCell(nR1, index, pParams[index].Value);
	}
	int checkIndex = 0;
	// additional params from fit information and fit statistics
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.MuFinal); }
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.MuMin); }
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.DerivStep); }
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.ParaChange); }
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.Tolerance); }
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.Confidence); }
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.Iterations); }
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.Range1); }
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.Range2); }
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.Step); }
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.Constr); }
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.EffConstr); }
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.N); }
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.DOF); }
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.ReducedChiSq); }
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.SSR);}
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.Correlation);}
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.Rvalue);}
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.RSqCOD);}
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.AdjRSq);}
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.RMSESD);}
	if(mSettings->isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.NormResiduals); }
}

bool DoMultiNLFitDlg() 
{
	MultiNLFitDlg myDlg;
	myDlg.DoModal( GetWindow() );
	return true;
}

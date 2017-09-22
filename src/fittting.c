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
#include <..\Originlab\NLFitSession.h>

#define ERR_RETURN(_ERR_MSG) { \
    out_str(_ERR_MSG); \
    return; \
}
/*
void append_column_data(Worksheet& wks, vector& vData, LPCSTR lpcstrLongName)
{
    int nCol = wks.AddCol();
    if( NULL != lpcstrLongName )
        wks.Columns(nCol).SetLongName(lpcstrLongName);
 
    Dataset ds(wks, nCol);
    ds = vData;
} */
 
 
void show_params(NLFitSession& nlfSession, int nNumData, LPCSTR lpcscTitle)
{
    vector<double>  vParamValues;
    vector<int>     vParamOffsets;
    vector<string>  vsParamNames;
 
    //get parameters values and names
    nlfSession.GetParamValuesAndOffsets(vParamValues, vParamOffsets);
    nlfSession.GetParamNamesInFunction(vsParamNames);
 
    //output paramter values with names;
    for ( int iData = 0; iData  < nNumData; iData++ )
    {
        printf("%s, parameters of the Dataset%d:\n", lpcscTitle, iData + 1);
        for ( int nParam = vParamOffsets[iData], ii = 0; nParam < vParamOffsets[iData + 1]; nParam++, ii++ )
        {
            printf("%s = %lf\n", vsParamNames[ii], vParamValues[nParam]);
        }
    }
}
void test_NLFitSession_2()
{
    Worksheet   wks = Project.ActiveLayer();
    if( !wks )
        return; 
 
    XYRange drInput;
    drInput.Add(wks, 0, "X");
    drInput.Add(wks, 1, "Y");
    drInput.Add();
    drInput.Add(wks, 0, "X");
    drInput.Add(wks, 2, "Y");    
    int nNumData = drInput.GetNumData(DRR_GET_DEPENDENT);
 
    vector vX1, vY1, vX2, vY2;
    drInput.GetData(vY1, vX1, NULL, 0);
    drInput.GetData(vY2, vX2, NULL, 1);
 
    NLFitSession nlfSession;
 
    nlfSession.SetSrcDataRange(drInput);
    // Set Function
    if ( !nlfSession.SetFunction("Gauss") )
        ERR_RETURN("Fail to set function!");
 
    // Set Data and only need to set the last dataset with global fit mode, as to allow share parameters
    if ( !nlfSession.SetData(vY1, vX1, NULL, 0, nNumData) )
        ERR_RETURN("Fail to set data for the first dataset!")
 
    if ( !nlfSession.SetData(vY2, vX2, NULL, 1, nNumData, DATA_MODE_GLOBAL) )
        ERR_RETURN("Fail to set data for the second dataset!")
 
    // Parameter initialization
    if ( !nlfSession.ParamsInitValues() )
        ERR_RETURN("Fail to init parameters values!")
 
    // Share xc parameter
    int nSharedParamIndex = 1; // 1, the index of xc in function, must be DATA_MODE_GLOBAL datamode, or else will fail to share parameters
    nlfSession.SetParamShare(nSharedParamIndex);
 
    // Show parameters before fitting
    nlfSession.GetChiSqr(); // call GetChiSqr order to set parameter settings on internal fit object
    show_params(nlfSession, nNumData, "Before ftting");

    // Set lower bound for the second dataset "Y0" parameter - "Y0 >= 3"
    // NLFitSession::SetParamBounds supported from Origin 8.5.1 version
    int nParam = 0; // Y0
    int nLimitControl = 0; // 0 for "<="
    double dLimitValue = 3;     
    bool bIsLower = true; // lower bound
    int nDataset = 1; // the second dataset
    nlfSession.SetParamBounds(nParam, nLimitControl, dLimitValue, bIsLower, nDataset);
 
    // Do fit
    int nFitOutcome;
    bool bRet = nlfSession.Fit(&nFitOutcome); //if success    
 
    string strOutcome = nlfSession.GetFitOutCome(nFitOutcome);
    out_str("Outcome of the fitting session : " + strOutcome);
 
    if( bRet )
    {
        // Show parameters after fitting
        show_params(nlfSession, nNumData, "After ftting");
 
        // Get fit parameter results
        vector vFitY1(vX1.GetSize()), vFitY2(vX2.GetSize());
        nlfSession.GetYFromX(vX1, vFitY1, vX1.GetSize(), 0);
        nlfSession.GetYFromX(vX2, vFitY2, vX1.GetSize(), 1);  
 
        // Put fitting data into new column in worksheet
//        append_column_data(wks, vFitY1, "Fit 1");        
 //       append_column_data(wks, vFitY2, "Fit 2");  
    }
}
void test_NLFitSession()
{
    Worksheet   wks = Project.ActiveLayer();
    if( !wks )
        return; 
 
    XYRange drInput;
    int column_data_count = wks.Columns.Count();
    int index = 1;
    while(index <= column_data_count)
    {
		drInput.Add(wks, 0, "X");
		drInput.Add(wks, index, "Y");
		if(index < 2) 
			drInput.Add(); 
		index++;
    }
    int nNumData = drInput.GetNumData(DRR_GET_DEPENDENT);
 
  /*  XYRange drInput;
    drInput.Add(wks, 0, "X");
    drInput.Add(wks, 1, "Y");
    drInput.Add();
    drInput.Add(wks, 0, "X");
    drInput.Add(wks, 2, "Y");    
    int nNumData = drInput.GetNumData(DRR_GET_DEPENDENT);*/
 
    
    NLFitSession nlfSession;
 
    nlfSession.SetSrcDataRange(drInput);
    
    // Set Function
    if ( !nlfSession.SetFunction("Gauss") )
        ERR_RETURN("Fail to set function!");
 
    
   /* 
    vector vX1, vY1, vX2, vY2;
    drInput.GetData(vY1, vX1, NULL, 0);
    drInput.GetData(vY2, vX2, NULL, 1);
    
    if ( !nlfSession.SetData(vY1, vX1, NULL, 0, nNumData) )
        ERR_RETURN("Fail to set data for the first dataset!")
 
    if ( !nlfSession.SetData(vY2, vX2, NULL, 1, nNumData, DATA_MODE_GLOBAL) )
        ERR_RETURN("Fail to set data for the second dataset!")*/
    index = 0;
    while(index < nNumData)
    {
		vector vX1, vY1;
		drInput.GetData(vY1, vX1, NULL, index);
		// Set Data and only need to set the last dataset with global fit mode, as to allow share parameters
		if ( !nlfSession.SetData(vY1, vX1, NULL, index, nNumData, index == nNumData - 1 ? DATA_MODE_GLOBAL : INVALID_DATA_MODE) )
			ERR_RETURN("Fail to set data for the first dataset!")
			index++;
    }
    // Parameter initialization
    if ( !nlfSession.ParamsInitValues() )
        ERR_RETURN("Fail to init parameters values!")
 
    // Share xc parameter
    int nSharedParamIndex = 1; // 1, the index of xc in function, must be DATA_MODE_GLOBAL datamode, or else will fail to share parameters
    nlfSession.SetParamShare(nSharedParamIndex);
 
    // Show parameters before fitting
    nlfSession.GetChiSqr(); // call GetChiSqr order to set parameter settings on internal fit object
    show_params(nlfSession, nNumData, "Before ftting");

    // Set lower bound for the second dataset "Y0" parameter - "Y0 >= 3"
    // NLFitSession::SetParamBounds supported from Origin 8.5.1 version
    int nParam = 0; // Y0
    int nLimitControl = 0; // 0 for "<="
    double dLimitValue = 3;     
    bool bIsLower = true; // lower bound
    int nDataset = 1; // the second dataset
    nlfSession.SetParamBounds(nParam, nLimitControl, dLimitValue, bIsLower, nDataset);
 
    // Do fit
    int nFitOutcome;
    bool bRet = nlfSession.Fit(&nFitOutcome); //if success    
 
    string strOutcome = nlfSession.GetFitOutCome(nFitOutcome);
    out_str("Outcome of the fitting session : " + strOutcome);
 
    if( bRet )
    {
        // Show parameters after fitting
        show_params(nlfSession, nNumData, "After ftting");
 
        // Get fit parameter results
       /* vector vFitY1(vX1.GetSize()), vFitY2(vX2.GetSize());
        nlfSession.GetYFromX(vX1, vFitY1, vX1.GetSize(), 0);
        nlfSession.GetYFromX(vX2, vFitY2, vX1.GetSize(), 1);  */
 
        // Put fitting data into new column in worksheet
       // append_column_data(wks, vFitY1, "Fit 1");        
      //  append_column_data(wks, vFitY2, "Fit 2");  
    }
}
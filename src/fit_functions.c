#include <Origin.h>
#include <..\originlab\NLFitSession.h>
// assume active worksheet with XYYYY data, all with a single peak
// so we can fit all of them to gauss function and gnerate a new worksheet to hold the fitting results.
void fit()
{
	Worksheet wks = Project.ActiveLayer();
	if(!wks)
	    return;
	
	Worksheet wksOut;
	wksOut.Create();
	
	fitAllColumns(wks, wksOut);
}

static bool fitAllColumns(Worksheet& wks, Worksheet& wksOutput, string strFunc = "Gauss")
{
	NLFitSession    FitSession;
	int             nDataIndex = 0; // only one set in our case
	DWORD           dwRules = DRR_GET_DEPENDENT | DRR_NO_FACTORS;
	
	// 1. Set fucntion
	if(!FitSession.SetFunction(strFunc, NULL)) // set function, category name can be ignore
	    return error_report("invalid fit function");
	
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
	
	    // 3. Call parameter init codes to init parameters
	    if(!FitSession.ParamsInitValues())
	        return error_report("err ParamsInitValues");
	    
	    // 4. Iterate with default settings
	    if(!FitSession.Fit(&nFitOutcome))
	    {
        	string strOutcome = FitSession.GetFitOutCome(nFitOutcome);
	        printf("fit failed:%d->%s\n", nFitErr, strOutcome);
	        return false;
	    }
	    
		// 5. success, get results and put to wksOutput
		FitParameter	params[20];// whatever the max possible number of parameters in your function
		RegStats		fitStats;
		NLSFFitInfo		fitInfo;
		FitSession.GetFitResultsStats(&fitStats, &fitInfo, false, nDataIndex);
		FitSession.GetFitResultsParams(params, &fitStats);
		appendFitResults(wksOutput, vsParamNames, params, fitStats, fitInfo);
	}
	
	return true;
}
static void appendFitResults(Worksheet& wks, const vector<string>& vsParamNames, const FitParameter* pParams, const RegStats& fitStats, const NLSFFitInfo& fitInfo)
{
	Column	c0Time = wks.Columns(0);
	int		nR1 = c0Time.GetUpperBound();
	// check if empty, then setup cols
	if(nR1 < 0)
	{
		vector<string> vsExtraCols = {"MuFinal", "MuMin", "DerivStep", "ParaChange", "Tolerance", "Confidence", "Iterations", "Range1", "Range2", "Step", "Constr", "EffConstr", 
		                              "Adj Rsquare", "N", "DOF", "ReducedChiSq", "SSR", "Correlation", "Rvalue", "RSqCOD", "AdjRSq", "RMSESD", "NormResiduals"};
		vector<string> vsComments = { "", "", "", "", "Final MU value reached", "Smallest MU value reached", "Parameter change amount for calculating derivatives",
		                             "Parameters changed", "Tolerance value", "Confidence value used", "Iterations performed", "Beginning range",
		                             "Ending range", "Step value", "Number of constraints", "Number of effective constraints", "Number of points",
		                             "Degrees of freedom", "Chi^2/DOF", "Regression sum of squares, same as ChiSq", "Chi^2",
		                             "Correlation between factors", "R Value", "Coefficient of determination (R^2)", 
		                             "Adjusted residuel sum of squares", "Root-MSE (SD)", "Norm of Residuals, as the largest singular value of the residuals"};
		
		int nTotalCols = vsParamNames.GetSize() + vsExtraCols.GetSize();
		if(wks.GetNumCols() < nTotalCols)
		{
			wks.SetSize(0, nTotalCols);
			vector<string> vs;vs = vsParamNames;
			vs.Append(vsExtraCols);		
			ASSERT(vs.GetSize() == vsComments.GetSize());
			for(int nCol = 0;nCol < vs.GetSize();nCol++)
			{
				Column cc = wks.Columns(nCol);
				cc.SetLongName(vs[nCol]);
				cc.SetComments(vsComments[nCol]);
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
	// additional params from fit information and fit statistics
	wks.SetCell(nR1, index++, fitInfo.MuFinal);
	wks.SetCell(nR1, index++, fitInfo.MuMin);
	wks.SetCell(nR1, index++, fitInfo.DerivStep);
	wks.SetCell(nR1, index++, fitInfo.ParaChange);
	wks.SetCell(nR1, index++, fitInfo.Tolerance);
	wks.SetCell(nR1, index++, fitInfo.Confidence);
	wks.SetCell(nR1, index++, fitInfo.Iterations);
	wks.SetCell(nR1, index++, fitInfo.Range1);
	wks.SetCell(nR1, index++, fitInfo.Range2);
	wks.SetCell(nR1, index++, fitInfo.Step);
	wks.SetCell(nR1, index++, fitInfo.Constr);
	wks.SetCell(nR1, index++, fitInfo.EffConstr);
	wks.SetCell(nR1, index++, fitStats.N);
	wks.SetCell(nR1, index++, fitStats.DOF);
	wks.SetCell(nR1, index++, fitStats.ReducedChiSq);
	wks.SetCell(nR1, index++, fitStats.SSR);
	wks.SetCell(nR1, index++, fitStats.Correlation);
	wks.SetCell(nR1, index++, fitStats.Rvalue);
	wks.SetCell(nR1, index++, fitStats.RSqCOD);
	wks.SetCell(nR1, index++, fitStats.AdjRSq);
	wks.SetCell(nR1, index++, fitStats.RMSESD);
	wks.SetCell(nR1, index++, fitStats.NormResiduals);
}

void Datasheet_SetReportTree_Ex3()
{
	Worksheet wks = Project.ActiveLayer();
	
	DataRange dRange;
	dRange.Add(wks, 0, "X");
	dRange.Add(wks, 1, "Y");
	GraphPage gp;
	gp.Create("Sample");
	
	GraphLayer gl = gp.Layers();
	int plotIndex = gl.AddPlot(dRange, IDM_PLOT_SCATTER);
	
	if(plotIndex >= 0)
	{
		gl.Rescale();
	}
}
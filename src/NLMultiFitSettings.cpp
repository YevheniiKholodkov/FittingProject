#include <Origin.h>
#include <..\FittingProject\src\NLMultiFitSettings.h>
#include <..\system\FDFTree.h>
#include <..\OriginLab\nlsf_utils.h>

const string SettingFileName = "nlf_setting.xml";

bool isDigit (char c) {
    if ((c>='0') && (c<='9')) return true;
    return false;
}

void numbersFromStringTo(string str, vector<unsigned int>& indexes)
{
	const char* c = str.GetBuffer(64);
	while(*c){
		while(*c && !isDigit(*c) && *c!='-') c++;
		if(!(*c)) break;
		indexes.Add(atoi(c));
		while(*c && (isDigit(*c) || *c=='-')) c++;
	}
}

//////////////////////////////////////////NLMultiFitSettings///////////////////////////////////////////////////////////////
NLMultiFitSettings::NLMultiFitSettings()
{
	mSaveSettings = false;
}
/*
NLMultiFitSettings::NLMultiFitSettings(const NLMultiFitSettings& settings)
{
	mFunctionCategory = settings.mFunctionCategory;
	mFunction = settings.mFunction;
	mReplicas = settings.mReplicas;
	mCheckedParamCount = settings.mCheckedParamCount;
	mAddParamChecked = settings.mAddParamChecked;
}
	*/
void NLMultiFitSettings::checkAddParam(unsigned int paramIndex, bool check)
{
	if(paramIndex >= mAdditionalParameters.mAddParamChecked.GetSize())
		return;
	mAdditionalParameters.mCheckedAddParamCount += check ? 1 : -1;
	mAdditionalParameters.mAddParamChecked[paramIndex] = check;
}

bool NLMultiFitSettings::isAddParamChecked(unsigned int paramIndex)
{
	if(paramIndex >= mAdditionalParameters.mAddParamChecked.GetSize())
		return false;
	return mAdditionalParameters.mAddParamChecked[paramIndex];
}
	
void NLMultiFitSettings::getCheckedAddIndexes(vector<unsigned int>& indexes)
{
	for(int i = 0; i < mAdditionalParameters.mAddParamChecked.GetSize(); ++i)
	{
		if(isAddParamChecked(i))
			indexes.Add(i);
	}
}
unsigned int NLMultiFitSettings::getCheckedAddParamCount()
{
	return mAdditionalParameters.mCheckedAddParamCount;
}


bool NLMultiFitSettings::loadSettings()
{
	Tree treeIni;
    if(!treeIni.Load(SettingFileName))
    	return false;
    
	TreeNode treeNodeTmp = treeIni.FirstNode;
	while(treeNodeTmp.IsValid())
	{
		if( treeNodeTmp.tagName == "save")
		{
			if(treeNodeTmp.Text == "false")
			{
				mSaveSettings = false;
				return mSaveSettings;
			}
			mSaveSettings = true;
		}
		if( treeNodeTmp.tagName == "Category")
		{
			setCategory(treeNodeTmp.Text);
		}
		if( treeNodeTmp.tagName == "Function")
		{
			setFunction(treeNodeTmp.Text);
		}
		if( treeNodeTmp.tagName == "Replicas")
		{
			setReplicas(atoi(treeNodeTmp.Text));
		}
		if( treeNodeTmp.tagName == "checkedParamCount")
		{
			mAdditionalParameters.mCheckedAddParamCount = atoi(treeNodeTmp.Text);
		}
		if( treeNodeTmp.tagName == "indexes")
		{
			vector<unsigned int> paramIndexes;
			numbersFromStringTo(treeNodeTmp.Text, paramIndexes);
			for(unsigned int i = 0; i < paramIndexes.GetSize(); ++i)
			{
				mAdditionalParameters.mAddParamChecked[paramIndexes[i]] = true;
			}
		}
		treeNodeTmp = treeNodeTmp.NextNode;
	}
    return true;
}

bool NLMultiFitSettings::saveSettings()
{
	Tree treeSettings;
	if(!mSaveSettings)
	{
		treeSettings.AddTextNode("false", "save", 0);
		return false;
	}
	treeSettings.AddTextNode("true", "save", 0);
	
	//TreeNode general, additional, parametrs, bounds;
	///General
	//general = treeSettings.AddNode("General", 1);
	treeSettings.AddTextNode(getCategory(), "Category", 10);
	treeSettings.AddTextNode(getFunction(), "Function", 11);
	string replicas = getReplicas();
	treeSettings.AddTextNode(replicas, "Replicas", 12);
	
	//additional
	//additional = treeSettings.AddNode("Additional", 2);
	string checkedParamCount = mAdditionalParameters.mCheckedAddParamCount;
	treeSettings.AddTextNode(checkedParamCount, "checkedParamCount", 20);
	vector<unsigned int> paramIndexes;
	getCheckedAddIndexes(paramIndexes);
	
	string index;
	for(int i = 0; i < paramIndexes.GetSize() - 1; ++i)
	{
		index += paramIndexes[i];
		index += ',';
	}
	if(paramIndexes.GetSize() != 0) index += paramIndexes[paramIndexes.GetSize() - 1];
	treeSettings.AddTextNode(index, "indexes", 21);
	
	//parametrs
	//parametrs = treeSettings.AddNode("Parametrs", 3);
	//additional
	//bounds = treeSettings.AddNode("Bounds", 4);
	
	return treeSettings.Save(SettingFileName);
}

void NLMultiFitSettings::loadFunctionParameters()
{
	Tree trFDF;
 	string strFDFFile;
 			if(nlsf_load_FDF_to_tree(trFDF, getFunction(), &mGeneralSettings.mFunctionCategory) <= FDFTYPE_UNKNOWN)
 			return;
 			//nlsf_load_FDF_to_tree(trFDF, mFunction, &mFunctionCategory);
 			
    StringArray names;
    StringArray meanings;
    
    mFunctionSettings.mFuncNames.RemoveAll();
    mFunctionSettings.mMeanings.RemoveAll();
    mFunctionSettings.mDublicateOffset =0;
    mFunctionSettings.mDublicateUnit = 0;
 	
	TreeNode treeNodeTmp = trFDF.FirstNode;
	while(treeNodeTmp.IsValid())
	{
		if(treeNodeTmp.tagName == "GENERALINFORMATION")
		{
			TreeNode paramsTreeNodeTmp = treeNodeTmp.FirstNode;    	// Start enumerating from the first node
			while(paramsTreeNodeTmp.IsValid())		// Checks if the tnTmp node is a valid node or not
			{
				string str = paramsTreeNodeTmp.tagName;
				if(paramsTreeNodeTmp.tagName == "NumberOfParameters")
				{
					mFunctionSettings.mNumberOfParams = atoi(paramsTreeNodeTmp.Text);
				}
				paramsTreeNodeTmp = paramsTreeNodeTmp.NextNode;
			}
		}
		if(treeNodeTmp.tagName == "FITTINGPARAMETERS")
		{
			TreeNode paramsTreeNodeTmp = treeNodeTmp.FirstNode;    	// Start enumerating from the first node
			while(paramsTreeNodeTmp.IsValid())		// Checks if the tnTmp node is a valid node or not
			{
				string str = paramsTreeNodeTmp.tagName;
				if(paramsTreeNodeTmp.tagName == "Names")
				{
					paramsTreeNodeTmp.Text.GetTokens(names, ','); 
					mFunctionSettings.mFuncNames.Append(names);
				}
				else if(paramsTreeNodeTmp.tagName == "Meanings")
				{
					paramsTreeNodeTmp.Text.GetTokens(meanings, ','); 
					mFunctionSettings.mMeanings.Append(meanings);
				}
				paramsTreeNodeTmp = paramsTreeNodeTmp.NextNode;
			}
		}
		if(treeNodeTmp.tagName == "CONTROLS")
		{
			TreeNode paramsTreeNodeTmp = treeNodeTmp.FirstNode;    	// Start enumerating from the first node
			while(paramsTreeNodeTmp.IsValid())		// Checks if the tnTmp node is a valid node or not
			{
				string str = paramsTreeNodeTmp.tagName;
				if(paramsTreeNodeTmp.tagName == "DuplicateOffset")
				{
					mFunctionSettings.mDublicateOffset = atoi(paramsTreeNodeTmp.Text);
				}
				else if(paramsTreeNodeTmp.tagName == "DuplicateUnit")
				{
					mFunctionSettings.mDublicateUnit = atoi(paramsTreeNodeTmp.Text);
					}
				paramsTreeNodeTmp = paramsTreeNodeTmp.NextNode;
			}
		}
		treeNodeTmp = treeNodeTmp.NextNode;
	}
	if(mFunctionSettings.mDublicateOffset == mFunctionSettings.mDublicateUnit == 0)
}

string NLMultiFitSettings::getFunction()
{
	return mGeneralSettings.mFunction; 
}
void NLMultiFitSettings::setFunction(string function) 
{ 
	mGeneralSettings.mFunction = function; 
	loadFunctionParameters();
	updateSession();
}
string NLMultiFitSettings::getCategory() 
{ 
	return mGeneralSettings.mFunctionCategory; 
}
void NLMultiFitSettings::setCategory(string category)
{
	mGeneralSettings.mFunctionCategory = category;
}
unsigned int NLMultiFitSettings::getReplicas()
{ 
	return mGeneralSettings.mReplicas; 
}
void NLMultiFitSettings::setReplicas(unsigned int replicas) 
{
	mGeneralSettings.mReplicas = replicas; 
	updateSession();
}
void NLMultiFitSettings::saveSettings(bool save) 
{
	mSaveSettings = save;
 }
 
 
bool NLMultiFitSettings::fit()
{
	int numOfAllParams = mParametrs.mNumOfParams * (getReplicas() + 1);
	if(numOfAllParams != mParametrs.mValues.GetSize() + getReplicas())
	{
		return error_report("number of parametrs of fit session and settings are different!!!");
	}
    
    vector vParams(mParametrs.mValues.GetSize());
    for(int ii = 0; ii < mParametrs.mValues.GetSize(); ++ii)
    {
		vParams[ii] = atof(mParametrs.mValues[ii]);
    }
    
    
    int index = 0;
    for(index = 0; index < mParametrs.mIsFixeds.GetSize(); ++index){
    	bool r = mParametrs.mIsFixeds[index];
		mFitSession.SetParamFix(index, mParametrs.mIsFixeds[index]);
    }
    for(index = 0; index < mParametrs.mIsShareds.GetSize(); ++index){
    	bool r = mParametrs.mIsShareds[index];
		mFitSession.SetParamShare(index, mParametrs.mIsShareds[index]);
    }
    
    
	//const int nMaxNumIterations = 500;
	Worksheet wks = Project.ActiveLayer();
	if(!wks)
	    return error_report("There are no active worksheet");
	
	Worksheet wksOutput;
	wksOutput.Create();
	
	int             nDataIndex = 0; // only one set in our case
	DWORD           dwRules = DRR_GET_DEPENDENT | DRR_NO_FACTORS;

	int    nFitOutcome;
	string statusWithError;
	
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
        mFitSession.SetSrcDataRange(drInputData); 
        
        
	    if(!mFitSession.SetData(vY1, vX1, NULL, nDataIndex, nNumData))  
	    {
	        statusWithError = "err setting data";
	        printf("fit failed:%s\n", statusWithError);
	    }
	    else
	    {
			// 3. Call parameter init codes to init parameters
			/*if(!mFitSession.ParamsInitValues())
			{
				statusWithError = "err ParamsInitValues";
				printf("fit failed:%s\n", statusWithError);
			}
			else
			{*/
				int result = mFitSession.SetParamValues(vParams);
				if(result == -2)
					return error_report("Set params error.  fit function not ready");
				else if(result == -1)
					return error_report("Set params error.  too many parameters");
				else if(result == 1)
					return error_report("Set params error.  too few parameters");
				
				// 4. Iterate with default settings
				if(!mFitSession.Fit(&nFitOutcome))
				{
					statusWithError = mFitSession.GetFitOutCome(nFitOutcome);
					printf("fit failed:%s\n", statusWithError);
				}
			//}
	    }
	    
		// 5. success, get results and put to wksOutput
		FitParameter	params[100];// whatever the max possible number of parameters in your function
		RegStats		fitStats;
		NLSFFitInfo		fitInfo;
		mFitSession.GetFitResultsStats(&fitStats, &fitInfo, false, nDataIndex);
		int paramsNum = mFitSession.GetFitResultsParams(params, &fitStats);
		if(paramsNum == -1)
			return error_report("an error occuried while getting the result params");
		if(!appendFitResults(wksOutput, params, paramsNum, fitStats, fitInfo, statusWithError))
			return false;
	}
	
	return true;
}

void NLMultiFitSettings::updateSession()
{
	if(getFunction().IsEmpty())
		return;
	//const int nMaxNumIterations = 500;
	Worksheet wks = Project.ActiveLayer();
	if(!wks)
	    return;
	
	int             nDataIndex = 0; // only one set in our case
	DWORD           dwRules = DRR_GET_DEPENDENT | DRR_NO_FACTORS;
	
	// 1. Set fucntion
	int j = getReplicas();
	if(!mFitSession.SetFunction(getFunction(), NULL/*getCategory()*/,getReplicas() + 1)) // set function, category name can be ignore
	{
		printf("invalid fit function");
		return;// error_report("invalid fit function");
	}
	
  //  FitSession.SetIterationSettings(nMaxNumIterations);
  
	int 			nFitOutcome;
	///////////////////////////////////////////////////////////////////
	DataRange   drInputData;
	drInputData.Add(wks, 0, "X");
	drInputData.Add(wks, 1, "Y");
	int         nNumData = drInputData.GetNumData(dwRules);
	ASSERT(1==nNumData);
	
	//2 set the dataset
	vector  vX1, vY1;
	drInputData.GetData( dwRules, nDataIndex, NULL, NULL, &vY1, &vX1 );    
    mFitSession.SetSrcDataRange(drInputData); 
    
	if(!mFitSession.SetData(vY1, vX1, NULL, nDataIndex, nNumData))  
	{
		printf("err setting data");
	    return ;//error_report("err setting data");
	}
	
	// 3. Call parameter init codes to init parameters
	if(!mFitSession.ParamsInitValues())
	{
		printf("err ParamsInitValues");
	    return ;//error_report("err ParamsInitValues");
	}
	setParametrsNamesAndValues();
}

void NLMultiFitSettings::setParametrsNamesAndValues()
{
	if(mFunctionSettings.mNumberOfParams == 0)
		return;
	vector<string> paramNames;
	vector         paramValues;
	vector<int>    paramOffsets;
	mParametrs.mIsShareds.SetSize(mFunctionSettings.mNumberOfParams);
	
	mFitSession.GetParamValuesAndOffsets(paramValues, paramOffsets);
	mParametrs.cleanAll();
	
	for(int ii =0; ii < paramValues.GetSize(); ++ii)
	{
		printf("[%d] -> name - , param - %f \n ", ii/*, names[i]*/, paramValues[ii]); 
	}
	
	mParametrs.mNames.RemoveAll();
	mParametrs.mValues.RemoveAll();
	
	int paramNumWithReplica = mFunctionSettings.mDublicateOffset - 1 + mFunctionSettings.mDublicateUnit * (getReplicas() + 1);
	mParametrs.mNames.SetSize(paramNumWithReplica);
	mParametrs.mValues.SetSize(paramNumWithReplica);
	
	for(int i = 0; i < mFunctionSettings.mNumberOfParams; ++i) // set the names and values of params without replicas
	{
		mParametrs.mNames[i] = mFunctionSettings.mFuncNames[i];
		mParametrs.mValues[i] = paramValues[i];
	}
	
	// set names and values for params of replicas
	int unit = 1;	
	int nameIndex = mFunctionSettings.mDublicateOffset - 1;
	//																														                      y,xc,w,A y0 xc0 w0 A0
	int unitIndex = mFunctionSettings.mNumberOfParams +  mFunctionSettings.mDublicateOffset - 1; // begin index for replica params              0, 1,2,3,4, 5,  6,  7
	int endUnitIndex = unitIndex + mFunctionSettings.mDublicateUnit - 1; // end index for replica params  y and y0 are the same and they are not inside dublicate unit.5 - begUnitIndex, 7 - endUnitIndex.
	for(i = mFunctionSettings.mNumberOfParams; i < paramNumWithReplica; ++i)
	{
			string paramName;
			paramName.Format("%s__%d", mFunctionSettings.mFuncNames[nameIndex++], unit);
			mParametrs.mNames[i] = paramName;
			string value;
			value.Format("%f", paramValues[unitIndex]);
			mParametrs.mValues[i] = value;
			
			if(unitIndex++ == endUnitIndex)
			{
				nameIndex = mFunctionSettings.mDublicateOffset - 1;
				++unit;
				unitIndex = mFunctionSettings.mNumberOfParams * unit + mFunctionSettings.mDublicateOffset - 1;
				endUnitIndex = unitIndex + mFunctionSettings.mDublicateUnit - 1;
			}
	}
	mParametrs.mIsShareds.SetSize(mFunctionSettings.mNumberOfParams);
	mParametrs.mIsFixeds.SetSize(paramValues.GetSize());
}

bool NLMultiFitSettings::getDublicateParamIndexes(vector<int>& indexes, int paramIndex)
{
	if(paramIndex < mFunctionSettings.mDublicateOffset - 1)
		return false;
	paramIndex = paramIndex >= mFunctionSettings.mNumberOfParams ? paramIndex % mFunctionSettings.mNumberOfParams : paramIndex; 
	paramIndex += mFunctionSettings.mDublicateUnit;
	while(paramIndex < mParametrs.mValues.GetSize())
	{
		indexes.Add(paramIndex);
		paramIndex += mFunctionSettings.mDublicateUnit;
	}
	return true;
}

bool NLMultiFitSettings::setShared(int index, bool shared/* = true*/) 
{
	if(index >= mParametrs.mIsShareds.GetSize())
		return false;
	int i = index >= mFunctionSettings.mNumberOfParams ? index % mFunctionSettings.mNumberOfParams : index; 
	mParametrs.mIsShareds[i] = shared;
	return true;
}

bool NLMultiFitSettings::appendFitResults(Worksheet& wks, const FitParameter* pParams, int numOfParams, const RegStats& fitStats, const NLSFFitInfo& fitInfo, string statWithError)
{
	int j = 0;
	for(; j < 50; j++)
	{
		printf("param[%d]:%f\n", j, pParams[j].Value);
	}
	/*if(numOfParams != mParametrs.mNames.GetSize())
		return error_report("err. number of parameters of initialization and results are different.");*/
	
	Column	c0Time = wks.Columns(0);
	int		nR1 = c0Time.GetUpperBound();
	
	// check if empty, then setup cols
	if(nR1 < 0)
	{
		int nTotalCols = mParametrs.mNames.GetSize() + getCheckedAddParamCount();
		if(wks.GetNumCols() < nTotalCols)
		{
			wks.SetSize(0, nTotalCols);	
			vector<unsigned int> paramIndexes;
			getCheckedAddIndexes(paramIndexes);
			
			int nCol = 0;
			int index = 0;
			while(nCol < nTotalCols)
			{
				Column column = wks.Columns(nCol);
				if(nCol < mParametrs.mNames.GetSize())
				{
					column.SetLongName(mParametrs.mNames[nCol]);
				}
				else
				{
					column.SetLongName(AddParamNames[paramIndexes[index]]);
					column.SetComments(AddParamComments[paramIndexes[index]]);
					++index;
				}
				++nCol;
			}
			wks.AddCol("Status");
		}
	}
	nR1++; 
	//fit parameters
	int index = 0;
	for(; index < mParametrs.mNames.GetSize(); index++)
	{
		wks.SetCell(nR1, index, pParams[index].Value);
	}
	int checkIndex = 0;
	// additional params from fit information and fit statistics
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.MuFinal); }
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.MuMin); }
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.DerivStep); }
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.ParaChange); }
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.Tolerance); }
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.Confidence); }
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.Iterations); }
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.Range1); }
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.Range2); }
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.Step); }
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.Constr); }
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitInfo.EffConstr); }
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.N); }
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.DOF); }
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.ReducedChiSq); }
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.SSR);}
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.Correlation);}
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.Rvalue);}
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.RSqCOD);}
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.AdjRSq);}
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.RMSESD);}
	if(isAddParamChecked(checkIndex++)) { wks.SetCell(nR1, index++, fitStats.NormResiduals); }
		
	wks.SetCell(nR1, index, statWithError);
	return true;
}
#include <Origin.h>
#include <..\FittingProject\src\NLMultiFitSettings.h>
#include <..\system\FDFTree.h>
#include <..\OriginLab\nlsf_utils.h>

const string SettingFileName = "nlf_setting.xml";

bool isDigit (char c) {
    if ((c>='0') && (c<='9')) return true;
    return false;
}

void numbersFromStringTo(string str, vector<int>& indexes, char devider)
{
	vector<string> str_nums;
	str.GetTokens(str_nums, devider);
	for(int i = 0; i < str_nums.GetSize(); ++i)
	{
		indexes.Add(atoi(str_nums[i]));
	}
}
void numbersFromStringToBool(string str, vector<bool>& indexes, char devider)
{
	vector<string> str_nums;
	str.GetTokens(str_nums, devider);
	for(int i = 0; i < str_nums.GetSize(); ++i)
	{
		indexes.Add(atoi(str_nums[i]));
	}
}


void numbersFromStringToDouble(string str, vector<double>& indexes, char devider)
{
	vector<string> str_nums;
	str.GetTokens(str_nums, devider);
	for(int i = 0; i < str_nums.GetSize(); ++i)
	{
		indexes.Add(atof(str_nums[i]));
	}
	/*const char* c = str.GetBuffer(256);
	while(*c){
		while(*c && !isDigit(*c) && *c!='-' && *c!='.') c++;
		if(!(*c)) break;
		printf("%s\n", c);
		indexes.Add(atof(c));
		while(*c && (isDigit(*c) || *c=='-' || *c=='.')) c++;
	}*/
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
		else if( treeNodeTmp.tagName == "Category")
		{
			mGeneralSettings.mFunctionCategory = treeNodeTmp.Text;
		}
		else if( treeNodeTmp.tagName == "Function")
		{
			mGeneralSettings.mFunction = treeNodeTmp.Text; 
			loadFunctionParameters();
		}
		else if( treeNodeTmp.tagName == "Replicas")
		{
			mGeneralSettings.mReplicas = atoi(treeNodeTmp.Text);
		}
		else if( treeNodeTmp.tagName == "Iterations")
		{
			mGeneralSettings.mMaxNumberOfIter = atoi(treeNodeTmp.Text);
		}
		else if( treeNodeTmp.tagName == "checkedParamCount")
		{
			mAdditionalParameters.mCheckedAddParamCount = atoi(treeNodeTmp.Text);
		}
		else if( treeNodeTmp.tagName == "mAddParamChecked")
		{
			vector<bool> add_params;
			numbersFromStringToBool(treeNodeTmp.Text, add_params, ',');
			mAdditionalParameters.mAddParamChecked = add_params;
		}
		else if( treeNodeTmp.tagName == "mUnitNumbers")
		{
			treeNodeTmp.Text.GetTokens(mParametrs.mUnitNumbers, ',');
		}
		else if( treeNodeTmp.tagName == "mNames")
		{
			treeNodeTmp.Text.GetTokens(mParametrs.mNames, ',');
		}
		else if( treeNodeTmp.tagName == "mMeanings")
		{
			treeNodeTmp.Text.GetTokens(mParametrs.mMeanings, ',');
		}
		else if( treeNodeTmp.tagName == "mValues")
		{
			numbersFromStringToDouble(treeNodeTmp.Text, mParametrs.mValues, ',');
		}
		else if( treeNodeTmp.tagName == "mIsShareds")
		{
			numbersFromStringToBool(treeNodeTmp.Text, mParametrs.mIsShareds, ',');
		}
		else if( treeNodeTmp.tagName == "mIsFixeds")
		{
			numbersFromStringToBool(treeNodeTmp.Text, mParametrs.mIsFixeds, ',');
		}
		else if( treeNodeTmp.tagName == "mLowerBounds")
		{
			numbersFromStringToDouble(treeNodeTmp.Text, mParametrs.mLowerBounds, ',');
		}
		else if( treeNodeTmp.tagName == "mLowerLimitControl")
		{
			numbersFromStringTo(treeNodeTmp.Text, mParametrs.mLowerLimitControl, ',');
		}
		else if( treeNodeTmp.tagName == "mUpperBounds")
		{
			numbersFromStringToDouble(treeNodeTmp.Text, mParametrs.mUpperBounds, ',');
		}
		else if( treeNodeTmp.tagName == "mUpperLimitControl")
		{
			numbersFromStringTo(treeNodeTmp.Text, mParametrs.mUpperLimitControl, ',');
		}
		else if( treeNodeTmp.tagName == "mValuesNumber")
		{
			mParametrs.mValuesNumber = atoi(treeNodeTmp.Text);
		}
		treeNodeTmp = treeNodeTmp.NextNode;
	}
	updateSessionFunction();
    return true;
}

bool NLMultiFitSettings::saveSettings()
{
	Tree treeSettings;
	if(!mSaveSettings)
	{
		treeSettings.AddTextNode("false", "save", 0);
		return treeSettings.Save(SettingFileName);
	}
	treeSettings.AddTextNode("true", "save", 0);
	
	//TreeNode general, additional, parametrs, bounds;
	///General
	//general = treeSettings.AddNode("General", 1);
	treeSettings.AddTextNode(getCategory(), "Category", 1);
	treeSettings.AddTextNode(getFunction(), "Function", 2);
	string replicas = getReplicas();
	treeSettings.AddTextNode(replicas, "Replicas", 3);
	
	//additional
	string checkedParamCount = mAdditionalParameters.mCheckedAddParamCount;
	treeSettings.AddTextNode(checkedParamCount, "checkedParamCount", 4);
	
	string value;
	for(int i = 0; i < mAdditionalParameters.mAddParamChecked.GetSize() - 1; ++i)
	{
		value += mAdditionalParameters.mAddParamChecked[i];
		value += ',';
	}
	if(mAdditionalParameters.mAddParamChecked.GetSize() != 0) value += mAdditionalParameters.mAddParamChecked[mAdditionalParameters.mAddParamChecked.GetSize() - 1];
	treeSettings.AddTextNode(value, "mAddParamChecked", 5);
	
	//parametrs
	value.Empty();
	for(i = 0; i < mParametrs.mUnitNumbers.GetSize() - 1; ++i)
	{
		value += mParametrs.mUnitNumbers[i];
		value += ",";
	}
	if(mParametrs.mUnitNumbers.GetSize() != 0) value += mParametrs.mUnitNumbers[ mParametrs.mUnitNumbers.GetSize() - 1];
	treeSettings.AddTextNode(value, "mUnitNumbers", 6);
	//////////////////////////////////////////////////////////
	value.Empty();
	for(i = 0; i < mParametrs.mNames.GetSize() - 1; ++i)
	{
		value += mParametrs.mNames[i];
		value += ",";
	}
	if(mParametrs.mNames.GetSize() != 0) value += mParametrs.mNames[ mParametrs.mNames.GetSize() - 1];
	treeSettings.AddTextNode(value, "mNames", 7);
	//////////////////////////////////////////////////////////
	value.Empty();
	for(i = 0; i < mParametrs.mMeanings.GetSize() - 1; ++i)
	{
		value += mParametrs.mMeanings[i];
		value += ",";
	}
	if(mParametrs.mMeanings.GetSize() != 0) value += mParametrs.mMeanings[ mParametrs.mMeanings.GetSize() - 1];
	treeSettings.AddTextNode(value, "mMeanings", 8);
	//////////////////////////////////////////////////////////
	value.Empty();
	for(i = 0; i < mParametrs.mValues.GetSize() - 1; ++i)
	{
		value += mParametrs.mValues[i];
		value += ",";
	}
	if(mParametrs.mValues.GetSize() != 0) value += mParametrs.mValues[ mParametrs.mValues.GetSize() - 1];
	treeSettings.AddTextNode(value, "mValues", 9);
	//////////////////////////////////////////////////////////
	value.Empty();
	for(i = 0; i < mParametrs.mIsShareds.GetSize() - 1; ++i)
	{
		value += mParametrs.mIsShareds[i];
		value += ",";
	}
	if(mParametrs.mIsShareds.GetSize() != 0) value += mParametrs.mIsShareds[ mParametrs.mIsShareds.GetSize() - 1];
	treeSettings.AddTextNode(value, "mIsShareds", 10);
	//////////////////////////////////////////////////////////
	value.Empty();
	for(i = 0; i < mParametrs.mIsFixeds.GetSize() - 1; ++i)
	{
		value += mParametrs.mIsFixeds[i];
		value += ",";
	}
	if(mParametrs.mIsFixeds.GetSize() != 0) value += mParametrs.mIsFixeds[ mParametrs.mIsFixeds.GetSize() - 1];
	treeSettings.AddTextNode(value, "mIsFixeds", 11);
	//////////////////////////////////////////////////////////
	value.Empty();
	for(i = 0; i < mParametrs.mLowerBounds.GetSize() - 1; ++i)
	{
		value += mParametrs.mLowerBounds[i];
		value += ",";
	}
	if(mParametrs.mLowerBounds.GetSize() != 0) value += mParametrs.mLowerBounds[ mParametrs.mLowerBounds.GetSize() - 1];
	treeSettings.AddTextNode(value, "mLowerBounds", 12);
	//////////////////////////////////////////////////////////
	value.Empty();
	for(i = 0; i < mParametrs.mLowerLimitControl.GetSize() - 1; ++i)
	{
		value += mParametrs.mLowerLimitControl[i];
		value += ",";
	}
	if(mParametrs.mLowerLimitControl.GetSize() != 0) value += mParametrs.mLowerLimitControl[ mParametrs.mLowerLimitControl.GetSize() - 1];
	treeSettings.AddTextNode(value, "mLowerLimitControl", 13);
	//////////////////////////////////////////////////////////
	value.Empty();
	for(i = 0; i < mParametrs.mUpperBounds.GetSize() - 1; ++i)
	{
		value += mParametrs.mUpperBounds[i];
		value += ",";
	}
	if(mParametrs.mUpperBounds.GetSize() != 0) value += mParametrs.mUpperBounds[ mParametrs.mUpperBounds.GetSize() - 1];
	treeSettings.AddTextNode(value, "mUpperBounds", 14);
	//////////////////////////////////////////////////////////
	value.Empty();
	for(i = 0; i < mParametrs.mUpperLimitControl.GetSize() - 1; ++i)
	{
		value += mParametrs.mUpperLimitControl[i];
		value += ",";
	}
	if(mParametrs.mUpperLimitControl.GetSize() != 0) value += mParametrs.mUpperLimitControl[ mParametrs.mUpperLimitControl.GetSize() - 1];
	treeSettings.AddTextNode(value, "mUpperLimitControl", 15);
	value.Empty();
	
	///////////////////////////////////////////////////////////////
	value = getMaxNumOfIter();
	treeSettings.AddTextNode(value, "Iterations", 16);
	value = mParametrs.mValuesNumber;
	treeSettings.AddTextNode(value, "mValuesNumber", 17);
	
	return treeSettings.Save(SettingFileName);
}

void NLMultiFitSettings::loadFunctionParameters()
{
	Worksheet wks = Project.ActiveLayer();
	if(!wks)
	{
		printf("No Active Layer");
	    return;
	}
	mParametrs.mDataNumber = wks.GetNumCols();
	
	Tree trFDF;
 	string strFDFFile;
 			if(nlsf_load_FDF_to_tree(trFDF, getFunction(), NULL) <= FDFTYPE_UNKNOWN)
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
		string tag = treeNodeTmp.tagName;
		tag.MakeUpper();
		if(tag == "GENERALINFORMATION")
		{
			TreeNode paramsTreeNodeTmp = treeNodeTmp.FirstNode;    	// Start enumerating from the first node
			while(paramsTreeNodeTmp.IsValid())		// Checks if the tnTmp node is a valid node or not
			{
				tag = paramsTreeNodeTmp.tagName;
				tag.MakeUpper();
				if(tag == "NUMBEROFPARAMETERS")
				{
					int a = atoi(paramsTreeNodeTmp.Text);
					mFunctionSettings.mNumberOfParams = atoi(paramsTreeNodeTmp.Text);
				}
				paramsTreeNodeTmp = paramsTreeNodeTmp.NextNode;
			}
		}
		if(tag == "FITTINGPARAMETERS")
		{
			TreeNode paramsTreeNodeTmp = treeNodeTmp.FirstNode;    	// Start enumerating from the first node
			while(paramsTreeNodeTmp.IsValid())		// Checks if the tnTmp node is a valid node or not
			{
				tag = paramsTreeNodeTmp.tagName;
				tag.MakeUpper();
				if(tag == "NAMES")
				{
					paramsTreeNodeTmp.Text.GetTokens(names, ','); 
					mFunctionSettings.mFuncNames.Append(names);
				}
				else if(tag == "MEANINGS")
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
				tag = paramsTreeNodeTmp.tagName;
				tag.MakeUpper();
				if(tag == "DUPLICATEOFFSET")
				{
					mFunctionSettings.mDublicateOffset = atoi(paramsTreeNodeTmp.Text);
				}
				else if(tag == "DUPLICATEUNIT")
				{
					mFunctionSettings.mDublicateUnit = atoi(paramsTreeNodeTmp.Text);
					}
				paramsTreeNodeTmp = paramsTreeNodeTmp.NextNode;
			}
		}
		treeNodeTmp = treeNodeTmp.NextNode;
	}
	if(mFunctionSettings.mDublicateOffset == 0 && mFunctionSettings.mDublicateUnit == 0)
	{
		mFunctionSettings.mIsReplicaAllowed = false;
		setReplicas(0);
	}
	else
		mFunctionSettings.mIsReplicaAllowed = true;
	
	mParametrs.mValuesNumber = isReplicaAllowed() ? mFunctionSettings.mDublicateOffset - 1 + mFunctionSettings.mDublicateUnit * (getReplicas() + 1)
	                                              : mFunctionSettings.mNumberOfParams;
}

string NLMultiFitSettings::getFunction()
{
	return mGeneralSettings.mFunction; 
}
void NLMultiFitSettings::setFunction(string function) 
{ 
	mGeneralSettings.mFunction = function; 
	loadFunctionParameters();
}
string NLMultiFitSettings::getCategory() 
{ 
	return mGeneralSettings.mFunctionCategory; 
}
void NLMultiFitSettings::setCategory(string category)
{
	mGeneralSettings.mFunctionCategory = category;
}
int NLMultiFitSettings::getReplicas()
{ 
	return mGeneralSettings.mReplicas; 
}
void NLMultiFitSettings::setReplicas(int replicas) 
{
	mGeneralSettings.mReplicas = replicas; 
}
void NLMultiFitSettings::saveSettings(bool save) 
{
	mSaveSettings = save;
}
 
int NLMultiFitSettings::getMaxNumOfIter()
{
	return mGeneralSettings.mMaxNumberOfIter;
}

void NLMultiFitSettings::setMaxNumOfIter(int iterations)
{
	mGeneralSettings.mMaxNumberOfIter = iterations;
}
 
bool NLMultiFitSettings::fit()
{
	/*int numOfAllParams = mFunctionSettings.mNumberOfParams * (getReplicas() + 1);
	if(numOfAllParams != mParametrs.mValuesNumber + getReplicas())
	{
		return error_report("number of parametrs of fit session and settings are different!!!");
	}*/
    
    for(int ii = 0; ii < mParametrs.mValuesNumber; ++ii)
    {
		/*set bounds */
		if(ii < mParametrs.mLowerBounds.GetSize())
		{
			printf("lowerBounds[%d] ->  %f \n ", ii/*, names[i]*/, mParametrs.mLowerBounds[ii]); 
			printf("mLowerLimitControl[%d] ->  %d \n ", ii/*, names[i]*/,mParametrs.mLowerLimitControl[ii]); 
			mFitSession.SetParamBounds(ii,  mParametrs.mLowerLimitControl[ii], mParametrs.mLowerBounds[ii], true, -1);
		}
		if(ii < mParametrs.mUpperBounds.GetSize())
		{
			printf("mUpperBounds[%d] ->  %f \n ", ii/*, names[i]*/, mParametrs.mUpperBounds[ii]); 
			printf("mUpperLimitControl[%d] ->  %d \n ", ii/*, names[i]*/,mParametrs.mUpperLimitControl[ii]); 
			mFitSession.SetParamBounds(ii,  mParametrs.mUpperLimitControl[ii], mParametrs.mUpperBounds[ii], false, -1);
		}
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
    
    mFitSession.SetMaxNumIter(mGeneralSettings.mMaxNumberOfIter);
    
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
				vector<double> params;
				getValues(params, nYCol - 1);
				int result = mFitSession.SetParamValues(params);
				if(result == -2)
					return error_report("Set params error.  fit function not ready");
				else if(result == -1)
					return error_report("Set params error.  too many parameters");
				else if(result == 1)
					return error_report("Set params error.  too few parameters");
				
	// 3. Call parameter init codes to init parameters
	if(!mFitSession.ParamsInitValues())
	{
		printf("err ParamsInitValues");
	    return false;//error_report("err ParamsInitValues");
	}
				// 4. Iterate with default settings
				if(!mFitSession.Fit(&nFitOutcome))
				{
					statusWithError = mFitSession.GetFitOutCome(nFitOutcome);
					printf("fit failed:%s\n", statusWithError);
				}
				else
				{
					statusWithError = "Success";
				}
			//}
	    }
	    
        /*Curve crv(wks, 0, nYCol);
        
        double dxc, dWidth, dBaseline, dArea, dHeight, dCentroid;
        int nErr, nPeakDirection;
        dxc = peak_pos(crv, &dWidth, &dBaseline, &dArea, &dHeight, &dCentroid, TRUE, TRUE, &nErr, &nPeakDirection);
        
        printf("Peak's width is %.4lf, height is %.4lf, baseline is %.4lf, center is %.4lf\n", dWidth, dHeight, dBaseline, dxc);*/
        
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
	updateSessionFunction();
	updateSessionParametrs();
}

void NLMultiFitSettings::updateSessionFunction()
{
	if(getFunction().IsEmpty())
		return;
	//const int nMaxNumIterations = 500;
	Worksheet wks = Project.ActiveLayer();
	if(!wks)
	{
		printf("No Active Layer");
	    return;
	}
	
	// 1. Set fucntion
	if(!mFitSession.SetFunction(getFunction(), NULL/*getCategory()*/,getReplicas() + 1)) // set function, category name can be ignore
	{
		printf("invalid fit function");
		return;// error_report("invalid fit function");
	}
	
  //  FitSession.SetIterationSettings(nMaxNumIterations);
  
}

void NLMultiFitSettings::updateSessionParametrs()
{
	Worksheet wks = Project.ActiveLayer();
	if(!wks)
	{
		printf("No Active Layer");
	    return;
	}
	mParametrs.mDataNumber = wks.GetNumCols();
	mParametrs.mValues.RemoveAll();
	
	int o = wks.GetNumCols();
	int             nDataIndex = 0; // only one set in our case
	DWORD           dwRules = DRR_GET_DEPENDENT | DRR_NO_FACTORS;
	int 			nFitOutcome;
	///////////////////////////////////////////////////////////////////
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
			printf("err setting data");
			return ;//error_report("err setting data");
		}
		
		// 3. Call parameter init codes to init parameters
		if(!mFitSession.ParamsInitValues())
		{
			printf("err ParamsInitValues");
			return ;//error_report("err ParamsInitValues");
		}
		vector         paramValues;
		vector<int>    paramOffsets;
		mFitSession.GetParamValuesAndOffsets(paramValues, paramOffsets);
		for(int i = 0; i < mFunctionSettings.mNumberOfParams; ++i) // set the names and values of params without replicas
			mParametrs.mValues.Add(paramValues[i]);
		int unit = 0;
		for(i = mFunctionSettings.mNumberOfParams; i < paramValues.GetSize(); ++i) // set the names and values of params without replicas
		{
			if(unit >= mFunctionSettings.mDublicateOffset - 1)
				mParametrs.mValues.Add(paramValues[i]);
			if(++unit == mFunctionSettings.mNumberOfParams)
				unit = 0;
		}
	}
	setFunctionParametrs();
}

void NLMultiFitSettings::setFunctionParametrs()
{
	if(mFunctionSettings.mNumberOfParams == 0)
		return;
	vector<string> paramNames;
	mParametrs.mIsShareds.SetSize(mFunctionSettings.mNumberOfParams);
	
	mParametrs.mUnitNumbers.RemoveAll();
	mParametrs.mNames.RemoveAll();
	mParametrs.mMeanings.RemoveAll();
	mParametrs.mIsShareds.RemoveAll();
	mParametrs.mIsFixeds.RemoveAll();
	mParametrs.mLowerBounds.RemoveAll();
	mParametrs.mLowerLimitControl.RemoveAll();
	mParametrs.mUpperBounds.RemoveAll();
	mParametrs.mUpperLimitControl.RemoveAll(); 
	
	/*for(int ii =0; ii < paramValues.GetSize(); ++ii)
	{
		printf("[%d] -> name - , param - %f \n ", ii, paramValues[ii]); 
	}*/
	
	int paramNumWithReplica = isReplicaAllowed() ? mFunctionSettings.mDublicateOffset - 1 + mFunctionSettings.mDublicateUnit * (getReplicas() + 1)
	                                              : mFunctionSettings.mNumberOfParams;
	mParametrs.mNames.SetSize(paramNumWithReplica);
	mParametrs.mUnitNumbers.SetSize(paramNumWithReplica); // set the number of parameters units. For example, y0 - 0, xc -1, w - 1, A - 1, xc__1 - 2, w__1 - 2, A__1 - 2
	{
		int unit = 1, unit_num = mFunctionSettings.mDublicateUnit;
		for(int i = 0; i < mParametrs.mUnitNumbers.GetSize(); ++i)
		{
			if(i < mFunctionSettings.mDublicateOffset - 1 || getReplicas() == 0)
				mParametrs.mUnitNumbers[i] = 0;
			else
			{
				mParametrs.mUnitNumbers[i] = unit;
				if(--unit_num == 0)
				{
					++unit;
					unit_num = mFunctionSettings.mDublicateUnit;
				}
			}
		}
	}
	for(int i = 0; i < mFunctionSettings.mNumberOfParams; ++i) // set the names and values of params without replicas
	{
		mParametrs.mNames[i] = mFunctionSettings.mFuncNames[i];
	}
	// set names and values for params of replicas
	if(isReplicaAllowed())
	{
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
				
				if(unitIndex++ == endUnitIndex)
				{
					nameIndex = mFunctionSettings.mDublicateOffset - 1;
					++unit;
					unitIndex = mFunctionSettings.mNumberOfParams * unit + mFunctionSettings.mDublicateOffset - 1;
					endUnitIndex = unitIndex + mFunctionSettings.mDublicateUnit - 1;
				}
		}
	}
	mParametrs.mIsShareds.SetSize(mFunctionSettings.mNumberOfParams);
	mParametrs.mIsFixeds.SetSize(mParametrs.mValuesNumber);
	
	vector<bool>   ExclusiveLower;
	vector<bool>   OnLowerBounds;	
	vector<bool>   ExclusiveUpper;
	vector<bool>   OnUpperBounds;
	mFitSession.GetParamNumericValues(mParametrs.mLowerBounds, PARMAS_SETTING_LOWERBOUNDS);
	mFitSession.GetParamNumericValues(ExclusiveLower, PARMAS_SETTING_LOWERBOUNDSEXCLUSIVE);
	mFitSession.GetParamNumericValues(OnLowerBounds, PARMAS_SETTING_LOWERBOUNDSENABLE);	
	
	mParametrs.mLowerLimitControl.SetSize(mParametrs.mLowerBounds.GetSize());
	for(int ii =0; ii < mParametrs.mLowerBounds.GetSize(); ++ii)
	{
		int control = OnLowerBounds[ii] ? (ExclusiveLower[ii] ? LIMIT_EXCLUSIVELESS : LIMIT_LESS): LIMIT_OFF;
		mParametrs.mLowerLimitControl[ii] = control;
		
		/*printf("lowerBounds[%d] ->  %f \n ", ii, mParametrs.mLowerBounds[ii]); 
		printf("mLowerLimitControl[%d] ->  %d \n ", ii,mParametrs.mLowerLimitControl[ii]); */
	}
	
	mFitSession.GetParamNumericValues(mParametrs.mUpperBounds, PARMAS_SETTING_UPPERBOUNDS);
	mFitSession.GetParamNumericValues(ExclusiveUpper, PARMAS_SETTING_UPPERBOUNDSEXCLUSIVE);
	mFitSession.GetParamNumericValues(OnUpperBounds, PARMAS_SETTING_UPPERBOUNDSENABLE);	
	
	mParametrs.mUpperLimitControl.SetSize(mParametrs.mUpperBounds.GetSize());
	for(ii =0; ii < mParametrs.mUpperBounds.GetSize(); ++ii)
	{
		int control = OnUpperBounds[ii] ? (ExclusiveUpper[ii] ? LIMIT_EXCLUSIVELESS : LIMIT_LESS): LIMIT_OFF;
		mParametrs.mUpperLimitControl[ii] = control;
		/*printf("upperBounds[%d] ->  %f \n ", ii, mParametrs.mUpperBounds[ii]); 
		printf("mUpperLimitControl[%d] ->  %d \n ", ii,mParametrs.mUpperLimitControl[ii]); */
	}
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

bool NLMultiFitSettings::setValue(int index, int dataIndex, double value)
{
	int trueIndex = dataIndex * mParametrs.mValuesNumber + index;
	if(trueIndex >= mParametrs.mValues.GetSize())
		return false;
	mParametrs.mValues[trueIndex] = value;
	return true;
}

bool NLMultiFitSettings::setFixed(int index, bool fixed)
{
	if(index >= mParametrs.mIsFixeds.GetSize())
		return false;
	int i = index >= mFunctionSettings.mNumberOfParams ? index % mFunctionSettings.mNumberOfParams : index; 
	mParametrs.mIsFixeds[i] = fixed;
	return true;
}


bool NLMultiFitSettings::appendFitResults(Worksheet& wks, const FitParameter* pParams, int numOfParams, const RegStats& fitStats, const NLSFFitInfo& fitInfo, string statWithError)
{
	/*if(numOfParams != mParametrs.mNames.GetSize())
		return error_report("err. number of parameters of initialization and results are different.");*/
	
	Column	c0Time = wks.Columns(0);
	int		nR1 = c0Time.GetUpperBound();
	
	// check if empty, then setup cols
	if(nR1 < 0)
	{
		int n = getCheckedAddParamCount();
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
					//printf("index - %d = %s\n",paramIndexes[index], AddParamNames[paramIndexes[index]]);
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
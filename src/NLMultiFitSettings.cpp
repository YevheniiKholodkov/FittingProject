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
 	/*if ( !nlf_get_fdf_filename(mFunction, &mFunctionCategory, NULL, &strFDFFile) || 	
 		(FDFERR_NO_ERROR != nlsf_load_FDF_to_tree(strFDFFile, &trFDF, FDFT_CONVERT_BOOL_TO_01, true)))
 			///End STILL_NO_ERR_MSG_SHOW_WHEN_FAIL_TO_READ_FDFFILE*/
 			if(nlsf_load_FDF_to_tree(trFDF, getFunction(), &mGeneralSettings.mFunctionCategory) <= FDFTYPE_UNKNOWN)
 			return;
 			//nlsf_load_FDF_to_tree(trFDF, mFunction, &mFunctionCategory);
 			
    StringArray names;
    StringArray meanings;
 	
	TreeNode treeNodeTmp = trFDF.FirstNode;
	while(treeNodeTmp.IsValid())
	{
		if(treeNodeTmp.tagName == "FITTINGPARAMETERS")
		{
			TreeNode paramsTreeNodeTmp = treeNodeTmp.FirstNode;    	// Start enumerating from the first node
			while(paramsTreeNodeTmp.IsValid())		// Checks if the tnTmp node is a valid node or not
			{
				if(paramsTreeNodeTmp.tagName == "Names")
				{
					paramsTreeNodeTmp.Text.GetTokens(names, ','); 
					mParametrs.appendNames(names);
				}
				if(paramsTreeNodeTmp.tagName == "Meanings")
				{
					paramsTreeNodeTmp.Text.GetTokens(meanings, ','); 
					mParametrs.appendMeanings(meanings);
				}
				paramsTreeNodeTmp = paramsTreeNodeTmp.NextNode;
			}
		}
		treeNodeTmp = treeNodeTmp.NextNode;
	}
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
unsigned int NLMultiFitSettings::getReplicas()
{ 
	return mGeneralSettings.mReplicas; 
}
void NLMultiFitSettings::setReplicas(unsigned int replicas) 
{
	mGeneralSettings.mReplicas = replicas; 
}
void NLMultiFitSettings::saveSettings(bool save) 
{
	mSaveSettings = save;
 }
 
 
bool NLMultiFitSettings::fit()
{
	return true;
}

void NLMultiFitSettings::updateSession()
{
}
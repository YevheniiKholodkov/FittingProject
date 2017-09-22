#ifndef NLMULTIFITSETTINGS
#define NLMULTIFITSETTINGS
#include <Origin.h>
#include <..\originlab\NLFitSession.h>

	
static const vector<string> AddParamNames = {"MuFinal", "MuMin", "DerivStep", "ParaChange", "Tolerance", "Confidence", "Iterations", "Range1",
				"Range2", "Step", "Constr", "EffConstr", "N", "DOF", "ReducedChiSq", "SSR", 
				"Correlation", "Rvalue", "RSqCOD", "AdjRSq", "RMSESD", "NormResiduals"};
				
static  vector<string> AddParamComments = {"Final MU value reached", "Smallest MU value reached", "Parameter change amount for calculating derivatives",
				"Parameters changed", "Tolerance value", "Confidence value used", "Iterations performed", "Beginning range",
				"Ending range", "Step value", "Number of constraints", "Number of effective constraints", "Number of points",
				"Degrees of freedom", "Chi^2/DOF", "Regression sum of squares, same as ChiSq, Chi^2",
				"Correlation between factors", "R Value", "Coefficient of determination (R^2)", 
				"Adjusted residuel sum of squares", "Root-MSE (SD)", "Norm of Residuals, as the largest singular value of the residuals"};

struct NLMultiFitParam
{
public:
	NLMultiFitParam(string name, string meaning, string value, bool isShared, bool isFixed)
	{
		mName = name;
		mMeaning = meaning;
		mValue = value;
		mIsShared = isShared;
		mIsFixed = isFixed;
	}
	
	string mName;
	string mMeaning;
	string mValue;
	bool  mIsShared;
	bool  mIsFixed;
};

struct NLMultiFitParams
{
public:
	NLMultiFitParams() {	}
	void addParam(NLMultiFitParam& param) 
	{
		mNames.Add(param.mName);
		mMeanings.Add(param.mMeaning);
		mValues.Add(param.mValue);
		mIsShareds.Add(param.mIsShared);
		mIsFixeds.Add(param.mIsFixed);
	}
	
	NLMultiFitParam getParam(int index)
	{
		NLMultiFitParam param(mNames[index], mMeanings[index], mValues[index], mIsShareds[index], mIsFixeds[index]);
		return param;
	}
	
	void getNames(vector<string>& names) { names = mNames; }
	void getMeanings(vector<string>& meanings) { meanings = mMeanings; }
	void getValues(vector<string>& values) { values = mValues; }
	void getShareds(vector<bool>& shareds) { shareds = mIsShareds; }
	void getFixeds(vector<bool>& fixeds) { fixeds = mIsFixeds; }
	
	void setShared(int index, bool shared = true) { mIsShareds[index] = shared; }
	void setFixed(int index, bool fixed = true) { mIsFixeds[index] = fixed; }
	
	void appendNames(vector<string>& names) { mNames.Append(names); }
	void appendMeanings(vector<string>& meanings) { mMeanings.Append(meanings); }
private:
	vector<string> mNames;
	vector<string> mMeanings;
	vector<string> mValues;
	vector<bool>   mIsShareds;
	vector<bool>   mIsFixeds;
};

struct AdditionalParameters
{
	AdditionalParameters()
	{
		mCheckedAddParamCount = 0;
		mAddParamChecked.SetSize(AddParamNames.GetSize());
		for(int i = 0; i < AddParamNames.GetSize(); ++i)
			mAddParamChecked[i] = false;
	}

	vector<bool>   mAddParamChecked;
	unsigned int   mCheckedAddParamCount;
};

struct GeneralSetttings
{
	GeneralSetttings() 
	{
		mReplicas = 0;
	}
	string         mFunctionCategory;
	string         mFunction;
	unsigned int   mReplicas;
};
				
class NLMultiFitSettings
{
public:
	NLMultiFitSettings();
//	NLMultiFitSettings(const NLMultiFitSettings& settings);
	
	bool fit();
	void updateSession();
	
	/*general setting methods*/
	string getFunction();
	void setFunction(string function);
	string getCategory();
	void setCategory(string category);
	unsigned int getReplicas();
	void setReplicas(unsigned int replicas);
	
	/*parameters methods*/
	NLMultiFitParam  getParam(int index) { return mParametrs.getParam(index);  }
	void  addParam(NLMultiFitParam& param)
	{
		mParametrs.addParam(param); 
	}
	NLMultiFitParams* getParameters() { return &mParametrs; } 
	
	/*Additional settings methods*/
	void checkAddParam(unsigned int paramIndex, bool check);
	bool isAddParamChecked(unsigned int paramIndex);
	void getCheckedAddIndexes(vector<unsigned int>& indexes);
	unsigned int getCheckedAddParamCount();
	
	/*save and load methods*/
	void saveSettings(bool save);
	bool isSave() { return mSaveSettings; }
	
	bool loadSettings();
	bool saveSettings();
	
private:
	void loadFunctionParameters();
private:
	AdditionalParameters mAdditionalParameters;
	GeneralSetttings     mGeneralSettings;
	NLMultiFitParams     mParametrs;
	
	bool           mSaveSettings;
	
	NLFitSession    mFitSession;
};

#endif // NLMULTIFITSETTINGS
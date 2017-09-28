#ifndef NLMULTIFITSETTINGS
#define NLMULTIFITSETTINGS
#include <Origin.h>
#include <..\originlab\NLFitSession.h>

enum LimitControl
{
	LIMIT_LESS = 0,
	LIMIT_EXCLUSIVELESS = 1,
	LIMIT_OFF = 2
};
	
static const vector<string> AddParamNames = {"MuFinal", "MuMin", "DerivStep", "ParaChange", "Tolerance", "Confidence", "Iterations", "Range1",
				"Range2", "Step", "Constr", "EffConstr", "N", "DOF", "ReducedChiSq", "SSR", 
				"Correlation", "Rvalue", "RSqCOD", "AdjRSq", "RMSESD", "NormResiduals"};
				
static  vector<string> AddParamComments = {"Final MU value reached", "Smallest MU value reached", "Parameter change amount for calculating derivatives",
				"Parameters changed", "Tolerance value", "Confidence value used", "Iterations performed", "Beginning range",
				"Ending range", "Step value", "Number of constraints", "Number of effective constraints", "Number of points",
				"Degrees of freedom", "Chi^2/DOF", "Regression sum of squares, same as ChiSq, Chi^2",
				"Correlation between factors", "R Value", "Coefficient of determination (R^2)", 
				"Adjusted residuel sum of squares", "Root-MSE (SD)", "Norm of Residuals, as the largest singular value of the residuals"};

struct NLMultiFitParams
{
public:
	NLMultiFitParams() 
	{
	 mValuesNumber = 0;
	 mDataNumber = 0;
	}
	
	vector<string> mUnitNumbers;
	vector<string> mNames;
	vector<string> mMeanings;
	vector<double> mValues;
	vector<bool>   mIsShareds;
	vector<bool>   mIsFixeds;
	
	vector 		   mLowerBounds;
	vector<int>    mLowerLimitControl; //LimitControl
	vector 		   mUpperBounds;
	vector<int>    mUpperLimitControl; //LimitControl
	
	int mValuesNumber;
	int mDataNumber;
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
		mMaxNumberOfIter = 100;
	}
	string         mFunctionCategory;
	string         mFunction;
	int            mReplicas;
	int            mMaxNumberOfIter;
};

struct FunctionSettings
{
	FunctionSettings() 
	{
		mDublicateOffset = 0;
		mDublicateUnit = 0;
		mNumberOfParams = 0;
		mIsReplicaAllowed = false;
	}
	vector<string> mFuncNames;
	vector<string> mMeanings;
	int mDublicateOffset;
	int mDublicateUnit;
	int mNumberOfParams;
	bool mIsReplicaAllowed;
};
				
class NLMultiFitSettings
{
public:
	NLMultiFitSettings();
//	NLMultiFitSettings(const NLMultiFitSettings& settings);
	
	bool fit();
	
	/*general setting methods*/
	string getFunction();
	void setFunction(string function);
	string getCategory();
	void setCategory(string category);
	int getReplicas();
	void setReplicas(int replicas);
	int getMaxNumOfIter();
	void setMaxNumOfIter(int iterations);
	
	/*parameters methods*/	
	void getUnitNumbers(vector<string>& numbers) { numbers = mParametrs.mUnitNumbers; }
	void getNames(vector<string>& names) { names = mParametrs.mNames; }
	void getMeanings(vector<string>& meanings) { meanings = mFunctionSettings.mMeanings; }
	void getValues(vector<double>& values, int dataIndex) 
	{
		int begIndex = dataIndex * mParametrs.mValuesNumber;
		int endIndex = begIndex + mParametrs.mValuesNumber - 1;
		mParametrs.mValues.GetSubVector(values, begIndex, endIndex); 
	}
	void getShareds(vector<bool>& shareds) { shareds = mParametrs.mIsShareds; }
	void getFixeds(vector<bool>& fixeds) { fixeds = mParametrs.mIsFixeds; }
		
	void getLowerBounds(vector& lowerBound) { lowerBound = mParametrs.mLowerBounds; }
	void getLowerLimitControls(vector<int>& lowerControls) { lowerControls = mParametrs.mLowerLimitControl; }
	void getUppperBounds(vector& uppperBounds) { uppperBounds = mParametrs.mUpperBounds; }
	void getUpperLimitControls(vector<int>& upperControls) { upperControls = mParametrs.mUpperLimitControl; }
	
	int getValuesNumber() { return mParametrs.mValuesNumber; }
	int getDataNumber() { return mParametrs.mDataNumber; }
	
	void setLowerBound(int index, double value) { if(index < mParametrs.mLowerBounds.GetSize()) mParametrs.mLowerBounds[index] = value; }
	void setLowerLimitControls(int index, int value) { if(index < mParametrs.mLowerLimitControl.GetSize()) mParametrs.mLowerLimitControl[index] = value;}
	void setUpperBound(int index, double value) { if(index < mParametrs.mUpperBounds.GetSize()) mParametrs.mUpperBounds[index] = value;}
	void setUpperLimitControls(int index, int value) { if(index < mParametrs.mUpperLimitControl.GetSize()) mParametrs.mUpperLimitControl[index] = value;}
	
	bool setValue(int index, int dataIndex, double value);
	bool setFixed(int index, bool fixed);
	
	bool getDublicateParamIndexes(vector<int>& indexes, int paramIndex);
	bool setShared(int index, bool shared = true);
	
	bool isReplicaAllowed() { return mFunctionSettings.mIsReplicaAllowed; }
	
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
	
	void updateSession(); // updates all session parametrs 
private:
	void loadFunctionParameters();
	void updateSessionFunction();
	void updateSessionParametrs();
	void setFunctionParametrs();	
	bool appendFitResults(Worksheet& wks, const FitParameter* pParams, int numOfParams, const RegStats& fitStats, const NLSFFitInfo& fitInfo, string statWithError);
private:
	AdditionalParameters mAdditionalParameters;
	GeneralSetttings     mGeneralSettings;
	NLMultiFitParams     mParametrs;
	FunctionSettings     mFunctionSettings;
	
	bool           mSaveSettings;
	
	NLFitSession    mFitSession;
};

#endif // NLMULTIFITSETTINGS
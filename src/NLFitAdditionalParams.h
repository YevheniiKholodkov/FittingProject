#include <Origin.h>

struct NLFitAdditionalParams
{
public:
	NLFitAdditionalParams() 
	{
		
	    vector<string> paramNames = {"MuFinal", "MuMin", "DerivStep", "ParaChange", "Tolerance", "Confidence", "Iterations", "Range1",
					"Range2", "Step", "Constr", "EffConstr", "N", "DOF", "ReducedChiSq", "SSR", 
					"Correlation", "Rvalue", "RSqCOD", "AdjRSq", "RMSESD", "NormResiduals"};
					
		vector<string> paramComments = {"Final MU value reached", "Smallest MU value reached", "Parameter change amount for calculating derivatives",
					"Parameters changed", "Tolerance value", "Confidence value used", "Iterations performed", "Beginning range",
					"Ending range", "Step value", "Number of constraints", "Number of effective constraints", "Number of points",
					"Degrees of freedom", "Chi^2/DOF", "Regression sum of squares, same as ChiSq, Chi^2",
					"Correlation between factors", "R Value", "Coefficient of determination (R^2)", 
					"Adjusted residuel sum of squares", "Root-MSE (SD)", "Norm of Residuals, as the largest singular value of the residuals"};
		
		mAddParamNames = paramNames;
		mAddParamComments = paramComments;
		
		mAddParamChecked.SetSize(mAddParamNames.GetSize());
		for(int i = 0; i < mAddParamNames.GetSize(); ++i)
		{
			mAddParamChecked[i] = false;
		}
	}
	
	void check(unsigned int paramIndex, bool check)
	{
		if(paramIndex >= mAddParamChecked.GetSize())
			return;
		mCheckedParamCount += check ? 1 : -1;
		mAddParamChecked[paramIndex] = check;
	}
	bool isChecked(unsigned int paramIndex)
	{
		if(paramIndex >= mAddParamChecked.GetSize())
			return false;
		return mAddParamChecked[paramIndex];
	}
	
	void getCheckedIndexes(vector<unsigned int>& indexes)
	{
		for(int i = 0; i < mAddParamChecked.GetSize(); ++i)
		{
			if(isChecked(i))
			{
				indexes.Add(i);
			}
		}
	}
	
	unsigned int getCheckedParamCount()
	{
		return mCheckedParamCount;
	}
	
	vector<string> mAddParamNames;
	vector<string> mAddParamComments;
private:
	vector<bool>   mAddParamChecked;
	unsigned int mCheckedParamCount;
};
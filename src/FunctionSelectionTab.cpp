
#include <Origin.h>
#include <..\FittingProject\src\FunctionSelection.h>


string FunctionSelectionTab::getCategory()
{
	string categoty;
	int currentIndex = mCategoryComboBox.GetCurSel();
	mCategoryComboBox.GetLBText(currentIndex, categoty);
	return  categoty;
}

string FunctionSelectionTab::getFunction()
{
	string function;
	int currentIndex = mFunctionComboBox.GetCurSel();
	mFunctionComboBox.GetLBText(currentIndex, function);
	return  function;
}

unsigned int FunctionSelectionTab::getReplicas()
{
	string replicas;
	int currentIndex = mReplicasComboBox.GetCurSel();
	mReplicasComboBox.GetLBText(currentIndex, replicas);
	return  atoi(replicas.GetBuffer(64));
}

BOOL FunctionSelectionTab::OnInitPage()
{
	mCategoryComboBox = GetItem(IDC_CATEGORYCOMBO);
	mFunctionComboBox = GetItem(IDC_FUNCTIONCOMBO2);
	mReplicasComboBox = GetItem(IDC_REPLICASCOMBO);
		
	string pathToIni = nlf_get_ini_filepath(0, true);
	if(!mIniParser.readIni(pathToIni)) // read NLSF.ini file with categories and functions
	{
		out_str("Error. INI file was read!")
	}
		
	vector<string> categories;
	if(!mIniParser.getCategories(categories)) // get categories
	{
		out_str("Error. Categories was not found!");
	}
	
	for(int i = 0; i < categories.GetSize(); ++i)
	{
		mCategoryComboBox.AddString(categories[i]);
	}
	
	for(i = 0; i <= 10; ++i)
	{
		string str = i;
		mReplicasComboBox.AddString(str);
	}
	//Set values from saved settings
	if(mSettings->isSave())
	{
		if(!mSettings->getCategory().IsEmpty()) 
		{
			mCategoryComboBox.Text = mSettings->getCategory();
			fillComboBoxByFunctionsWith(mSettings->getCategory());
		}
		if(!mSettings->getFunction().IsEmpty()) 
		{
			mFunctionComboBox.Text = mSettings->getFunction();
		}
		if(!mSettings->isReplicaAllowed())
		{
			mReplicasComboBox.Text = 0;
			mReplicasComboBox.Enable = false;
		}
		else
		{
			mReplicasComboBox.Text = mSettings->getReplicas();
		}
	}
	else
	{
		mReplicasComboBox.Text = 0;
	}
	
	return TRUE;		
}

BOOL FunctionSelectionTab::OnCategoryChanged(ComboBox ctrl)
{
	fillComboBoxByFunctionsWith(getCategory());
	if(mSettings)
		mSettings->setCategory(getCategory());
	string str = mSettings->getCategory();
	return TRUE;
}

BOOL FunctionSelectionTab::OnFunctionChanged(ComboBox ctrl)
{
	if(mSettings) 
		mSettings->setFunction(getFunction());
	if(!mSettings->isReplicaAllowed())
	{
		mReplicasComboBox.Text = 0;
		mReplicasComboBox.Enable = false;
	}
	else if(mReplicasComboBox.Enable == false)
	{
		mReplicasComboBox.Enable = true;
	}
	return TRUE;
}

BOOL FunctionSelectionTab::OnReplicasChanged(ComboBox ctrl)
{
	if(mSettings) 
		mSettings->setReplicas(getReplicas());
	return TRUE;
}

void FunctionSelectionTab::fillComboBoxByFunctionsWith(string category)
{
	vector<string> functions;
	if(!mIniParser.getFunctions(category, functions)) // get categories
	{
		out_str("Error. Functions was not found!");
	}
	
	mFunctionComboBox.ResetContent();
	for(int i = 0; i < functions.GetSize(); ++i)
	{
		mFunctionComboBox.AddString(functions[i]);
	}
}

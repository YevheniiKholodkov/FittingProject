#ifndef FUNCTIONSELECTIONTAB
#define FUNCTIONSELECTIONTAB
#include <Dialog.h>
#include "NLSFIniPraser.h"
#include <MsgMap.h>
#include "MultiNLFitDialogRes.h"
#include "resource.h"
#include "NLMultiFitSettings.h"

class FunctionSelectionTab : public PropertyPage
{
public:
	// Constructor for FunctionSelectionTab page
	FunctionSelectionTab(int nID) : PropertyPage(nID) {}
	
	void init(int id, NLMultiFitSettings*  settings)
	{
		SetID(id);
		mSettings = settings;
	}
	
	string getCategory();
	string getFunction();
	int getReplicas();
	int getIterations();
	
	void setSettings(NLMultiFitSettings*  settings) {  } 

public:
	// Event handlers for FunctionSelectionTab page
	EVENTS_BEGIN
		PAGE_ON_INIT(OnInitPage)
		PAGE_ON_ACTIVE(OnActivatePage)
		ON_CBN_SELCHANGE(IDC_CATEGORYCOMBO, OnCategoryChanged)
		ON_CBN_SELCHANGE(IDC_FUNCTIONCOMBO2, OnFunctionChanged)
		ON_CBN_SELCHANGE(IDC_REPLICASCOMBO, OnReplicasChanged)
		ON_CBN_SELCHANGE(IDC_ITERCOMBO, OnIterationChanged)
	EVENTS_END

	BOOL OnInitPage();

	BOOL OnActivatePage()
	{
		return TRUE;		
	}
	
	BOOL OnCategoryChanged(ComboBox ctrl);
	BOOL OnFunctionChanged(ComboBox ctrl);
	BOOL OnReplicasChanged(ComboBox ctrl);
	BOOL OnIterationChanged(ComboBox ctrl);
private:
	void fillComboBoxByFunctionsWith(string category);
private:
	ComboBox mCategoryComboBox;
	ComboBox mFunctionComboBox;
	ComboBox mReplicasComboBox;
	ComboBox mIterationsComboBox;
	NLSFIniPraser mIniParser;
	NLMultiFitSettings* mSettings;
};

#endif //FUNCTIONSELECTIONTAB
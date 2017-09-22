#include <Dialog.h>
#include <..\FittingProject\src\resource.h>
#include <..\OriginLab\DialogEx.h>
#include <..\OriginLab\MenuBase.h>
#include <..\OriginLab\FunctionsMenu.h>
#include <..\OriginLab\NewFunctionPlotHelper.h>
#include <..\OriginLab\New2DFunctionPlotDlg.h>

#include "MultiNLFitDialogRes.h"

class NLSFIniPraser
{
public:
	NLSFIniPraser() {}
	
	bool readIni(string pathToIni);
	bool getCategories(vector<string> &categories);
	bool getFunctions(string category, vector<string>& functions);
private:
	Tree mTreeIni;
};

class MultiNLFitDialog : public Dialog
{
public:
	MultiNLFitDialog() : Dialog(IDD_MULTINLFITDLG, "MultiNLFitDialog")
	{
	}
	
protected:
	EVENTS_BEGIN
		ON_INIT(OnInitDialog)
		ON_OK(OnOk)
	EVENTS_END
	
	BOOL OnInitDialog();
	BOOL OnOk();
private:
	ComboBox mCategoryComboBox;
	NLSFIniPraser mIniParser;
 
};

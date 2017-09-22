#ifndef BOUNDSTAB
#define BOUNDSTAB
#include <Dialog.h>
#include <..\system\MsgMap.h>
#include "MultiNLFitDialogRes.h"
#include <..\FittingProject\src\resource.h>
#include <..\FittingProject\src\NLMultiFitSettings.h>

class BoundsTab : public PropertyPage
{
public:
	// Constructor for BoundsTab page
	BoundsTab(int nID) : PropertyPage(nID) {}

	void init(int id, NLMultiFitSettings*  settings)
	{
		SetID(id);
		mSettings = settings;
	}
	
	// Event handlers for BoundsTab page
	EVENTS_BEGIN
		PAGE_ON_INIT(OnInitPage)
		PAGE_ON_ACTIVE(OnActivatePage)
	EVENTS_END

	BOOL OnInitPage()
	{
		return TRUE;		
	}

	BOOL OnActivatePage()
	{
		return TRUE;		
	}
private:
	NLMultiFitSettings* mSettings;
};
#endif //BOUNDSTAB
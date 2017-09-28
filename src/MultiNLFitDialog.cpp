/*------------------------------------------------------------------------------*
 * File Name:				 													*
 * Creation: 																	*
 * Purpose: OriginC Source C file												*
 * Copyright (c) ABCD Corp.	2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010		*
 * All Rights Reserved															*
 * 																				*
 * Modification Log:															*
 *------------------------------------------------------------------------------*/
 
////////////////////////////////////////////////////////////////////////////////////
// Including the system header file Origin.h should be sufficient for most Origin
// applications and is recommended. Origin.h includes many of the most common system
// header files and is automatically pre-compiled when Origin runs the first time.
// Programs including Origin.h subsequently compile much more quickly as long as
// the size and number of other included header files is minimized. All NAG header
// files are now included in Origin.h and no longer need be separately included.
//
// Right-click on the line below and select 'Open "Origin.h"' to open the Origin.h
// system header file.
#include <Origin.h>
////////////////////////////////////////////////////////////////////////////////////
#include <Dialog.h>

////////////////////////////////////////////////////////////////////////////////////
// Include your own header files here.
#include "MultiNLFitDialogRes.h" // resource DLL header
#include "MultiNLFitDialog.h" // dialog class header
#include <..\originlab\NLFitSession.h>

//////// ////////////////////////////////////////////////////////////////////////////
// Start your functions here.

	
BOOL MultiNLFitDlg::OnInitDialog()
{
	mSaveCheckBox = GetItem(IDC_SAVESETTINGSCHECK);
	m_PlaceHolder.Create(IDC_TAB_PLACEHOLDER, *this);
	m_PlaceHolder.init(mSettings);
	m_PlaceHolder.InitMaps();
	
	mSaveCheckBox.Check = mSettings->isSave();
	return TRUE;
}

BOOL MultiNLFitDlg::OnClose()
{
	mSettings->saveSettings();
	return TRUE;
}

BOOL MultiNLFitDlg::OnCancelClosed(Control ctrl)
{
	OnClose();
	Close();
	return TRUE;
}

BOOL MultiNLFitDlg::OnFitButtonClicked(Control ctrl)
{
	mSettings->fit();
	OnClose();
	Close();
	return true;
}

BOOL MultiNLFitDlg::OnClickSetSave(Control ctrl)
{
	mSettings->saveSettings(mSaveCheckBox.Check);
	return true;
}

bool DoMultiNLFitDlg() 
{
	Worksheet wks = Project.ActiveLayer();
	if(!wks)
	{
		MessageBox(GetWindow(), "There are no active worksheet", "Error", MB_OK);
	    return false;
	}
	else
	{
		MultiNLFitDlg myDlg;
		myDlg.DoModal( GetWindow() );
	}
	return true;
}

/*------------------------------------------------------------------------------*
 * File Name:				 													*
 * Creation: 																	*
 * Purpose: OriginC Source C file												*
 * Copyright (c) ABCD Corp.	2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010		*
 * All Rights Reserved															*
 * 																				*
 * Modification Log:															*
 *------------------------------------------------------------------------------*/
#ifndef MULTINLFITDLG
#define MULTINLFITDLG

#include <Origin.h>
#include <Dialog.h>
#include <MsgMap.h>
#include "MultiNLFitDialogRes.h"
#include "resource.h"
#include "FunctionSelection.h"
#include "AdditionalTab.h"
#include "ParametrsTab.h"
#include "BoundsTab.h"
#include "NLMultiFitSettings.h"

class MultiNLFitDlgPlaceHolder : public PropertySheet
{
public:
	MultiNLFitDlgPlaceHolder()
	{
	}
	
	void init(NLMultiFitSettings* settings)
	{
		m_FunctionSelectionTab.init(IDD_FUNCTIONTAB, settings);
		m_PrametersTab.init(IDD_PARAMETERSTAB, settings);
		m_BoundsTab.init(IDD_BOUNDS, settings);
		m_AdditionalTab.init(IDD_ADDITIONALTAB, settings);
		
		AddPage(m_FunctionSelectionTab);
		AddPage(m_PrametersTab);
		AddPage(m_BoundsTab);
		AddPage(m_AdditionalTab);
	}

	void InitMaps()
	{
		m_FunctionSelectionTab.InitMsgMap();
		m_PrametersTab.InitMsgMap();
		m_BoundsTab.InitMsgMap();
		m_AdditionalTab.InitMsgMap();
	}
		
	string getFunction() { return m_FunctionSelectionTab.getFunction(); }

	FunctionSelectionTab m_FunctionSelectionTab;
	ParametersTab m_PrametersTab;
	BoundsTab m_BoundsTab;
	AdditionalTab m_AdditionalTab;
};


class MultiNLFitDlg : public Dialog
{
public:
	MultiNLFitDlg() : Dialog(IDD_MULTINLFITDLG, "MultiNLFitDialog")
	{
		mCurrentPeak = 1;
		Worksheet wks = Project.ActiveLayer();
		mSettings = new NLMultiFitSettings(wks);
		mSettings->loadSettings();
	}
	
	~MultiNLFitDlg()
	{
		delete mSettings;
	}
	BOOL Create(HWND hParent = NULL)
	{
		InitMsgMap();// will be called from internal later
		bool nRet = Dialog::Create(hParent, 0);
		return nRet;
	}
	
	int DoModal(HWND hParent = NULL)
	{
		InitMsgMap();// will be called from internal later
		int nRet = Dialog::DoModal(hParent, 0, "Multi NLFit");
		return nRet;
	}
	
	void test() {
		mSettings->buildGraph();
	}

protected:
///----------------- Message Map ----------------
	EVENTS_BEGIN
		ON_SYSCOMMAND(OnSystemCommand)
		PAGE_ON_INIT(OnInitDialog)
		ON_CANCEL(OnClose) 
		ON_BN_CLICKED(IDC_FITBUTTON, OnFitButtonClicked)
		ON_BN_CLICKED(IDC_SAVESETTINGSCHECK, OnClickSetSave)
		ON_BN_CLICKED(IDC_CHECKREVERSE, OnClickSetReverse)
		ON_BN_CLICKED(IDC_BUILDFST_BTN, OnClickBuildFirstGraph)
		ON_BN_CLICKED(IDC_BUILDNEXT_BTN, OnClickBuildNextGraph)
		ON_BN_CLICKED(IDC_BUILDN_BTN, OnClickBuildNGraphs)
		OnActivate(OnActivate)
	EVENTS_END
///----------------------------------------------
	
	
	void	OnActivate(UINT nState, HWND hwndOther, BOOL bMinimized);
	BOOL OnSystemCommand(int nCmd);
	
	BOOL OnInitDialog();
	BOOL OnClose();
	BOOL OnFitButtonClicked(Control ctrl);
	BOOL OnClickSetReverse(Control ctrl);
	BOOL OnClickSetSave(Control ctrl);
	BOOL OnClickBuildFirstGraph(Control ctrl);
	BOOL OnClickBuildNextGraph(Control ctrl);
	BOOL OnClickBuildNGraphs(Control ctrl);


	Edit   mEditFirst;
	Edit   mEditLast;
	Button mSaveCheckBox;
	Button mReverseCheckBox;
	MultiNLFitDlgPlaceHolder m_PlaceHolder;
	NLMultiFitSettings* mSettings;
	
	int mCurrentPeak;
};

#endif //MULTINLFITDLG
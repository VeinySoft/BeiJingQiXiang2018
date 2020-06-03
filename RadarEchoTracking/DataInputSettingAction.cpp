#include "StdAfx.h"
#include "DataInputSettingAction.h"
#include "ModeSettingDialogImps.h"
#include "MainWindow.h"

extern MainWindow* g_MainWindow;

ADD_MYACTION_CONTSTRUCTED_DECLARATION(DataInputSettingAction)

void DataInputSettingAction::MyAction()
{
	ModeSettingDialogImps dialog;
	int iR = dialog.exec();
	if(iR == 1)
	{
		g_MainWindow->FlushCubesList();
		g_MainWindow->StartTrack();
	}
}

void DataInputSettingAction::InitMyAction()
{
	
}

#include "StdAfx.h"
#include "TrackBoxSettingAction.h"
#include "MainWindow.h"
extern MainWindow* g_MainWindow;

ADD_MYACTION_CONTSTRUCTED_DECLARATION(TrackBoxSettingAction)

void TrackBoxSettingAction::MyAction()
{
	g_MainWindow->SwitchDrawRectMaipulator();
}

void TrackBoxSettingAction::InitMyAction()
{
	
}

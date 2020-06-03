#include "StdAfx.h"
#include "StopTrackingAction.h"
#include "MainWindow.h"
extern MainWindow* g_MainWindow;

ADD_MYACTION_CONTSTRUCTED_DECLARATION(StopTrackingAction)

void StopTrackingAction::MyAction()
{
	g_MainWindow->StopStrack();
	g_MainWindow->WaitSlaveThread();

	g_MainWindow->SelectLastedFileListItem();
	g_MainWindow->StartRealDisplay();
}

void StopTrackingAction::InitMyAction()
{
	
}

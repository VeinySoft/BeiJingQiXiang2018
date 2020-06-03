#include "StdAfx.h"
#include "NextFrameAction.h"
#include "MainWindow.h"
extern MainWindow* g_MainWindow;

ADD_MYACTION_CONTSTRUCTED_DECLARATION(NextFrameAction)

void NextFrameAction::MyAction()
{
	g_MainWindow->StopStrack();
	g_MainWindow->WaitSlaveThread();

	g_MainWindow->SelectLastedFileListItem();
	g_MainWindow->DefaultAction();
}

void NextFrameAction::InitMyAction()
{
	
}

#include "StdAfx.h"
#include "RestorePointerAction.h"
#include "MainWindow.h"
extern MainWindow* g_MainWindow;
ADD_MYACTION_CONTSTRUCTED_DECLARATION(RestorePointerAction)

void RestorePointerAction::MyAction()
{
	g_MainWindow->SwitchDefalultMaipulator();
}

void RestorePointerAction::InitMyAction()
{
	
}

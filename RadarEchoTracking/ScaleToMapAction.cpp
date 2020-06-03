#include "StdAfx.h"
#include "ScaleToMapAction.h"
#include "MainWindow.h"
extern MainWindow* g_MainWindow;

ADD_MYACTION_CONTSTRUCTED_DECLARATION(ScaleToMapAction)

void ScaleToMapAction::MyAction()
{
	g_MainWindow->ScaleToMap();
}

void ScaleToMapAction::InitMyAction()
{
	
}

#include "StdAfx.h"
#include "MyActionInterface.h"
#include "DirectStartStrackAcion.h"

#include "MainWindow.h"
extern MainWindow* g_MainWindow;

ADD_MYACTION_CONTSTRUCTED_DECLARATION(DirectStartStrackAcion)

void DirectStartStrackAcion::MyAction()
{
	g_MainWindow->StopRealDispaly();

	g_MainWindow->FillSelectList();
	if(g_MainWindow->StartTrack2() > 0)
	{
		this->setEnabled(false);
		g_MainWindow->EnabledReplay(false);
	}
	
}

void DirectStartStrackAcion::InitMyAction()
{
	
}
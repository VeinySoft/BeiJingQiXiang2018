#include "StdAfx.h"
#include "PlayHistoryTrackAction.h"
#include "ConfigAccesser.h"
#include <QtGlobal>
#include <QVector>
#include "MainWindow.h"

extern MainWindow* g_MainWindow;
extern ConfigAccesser g_GlobleConfig; 
ADD_MYACTION_CONTSTRUCTED_DECLARATION(PlayHistoryTrackAction)

void PlayHistoryTrackAction::MyAction()
{
	g_MainWindow->StopRealDispaly();

	g_MainWindow->ReplayStrack();
	g_MainWindow->FillSelectList(1);
	g_MainWindow->StartReplay();
	this->setEnabled(false);
	g_MainWindow->EnabledTrack(false);
}

void PlayHistoryTrackAction::InitMyAction()
{
	
}




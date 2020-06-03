#include "StdAfx.h"
#include "PauseTrackingAction.h"
#include "MainWindow.h"
extern MainWindow* g_MainWindow;

ADD_MYACTION_CONTSTRUCTED_DECLARATION(PauseTrackingAction)

void PauseTrackingAction::MyAction()
{
	if(m_bPlay)
	{
		m_bPlay = false;
		g_MainWindow->PaushStarck();
		this->setChecked(true);
	}
	else
	{
		m_bPlay = true;
		g_MainWindow->ResumeStarck();
		this->setChecked(false);
	}
}

void PauseTrackingAction::InitMyAction()
{
	m_bPlay = true;
}

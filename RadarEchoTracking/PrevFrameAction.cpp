#include "StdAfx.h"
#include "PrevFrameAction.h"
#include "MainWindow.h"
extern MainWindow* g_MainWindow;
ADD_MYACTION_CONTSTRUCTED_DECLARATION(PrevFrameAction)

void PrevFrameAction::MyAction()
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

void PrevFrameAction::InitMyAction()
{
	
}

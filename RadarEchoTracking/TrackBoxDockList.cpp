#include "StdAfx.h"
#include "TrackBoxDockList.h"


TrackBoxDockList::TrackBoxDockList(void)
{
	m_Setup.setupUi(this);
	this->setWindowTitle(QString::fromLocal8Bit("���ٿ�"));

	m_Setup.widget_2->setVisible(false);
}


TrackBoxDockList::~TrackBoxDockList(void)
{
}

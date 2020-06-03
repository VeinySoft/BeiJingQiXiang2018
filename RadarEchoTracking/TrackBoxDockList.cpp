#include "StdAfx.h"
#include "TrackBoxDockList.h"


TrackBoxDockList::TrackBoxDockList(void)
{
	m_Setup.setupUi(this);
	this->setWindowTitle(QString::fromLocal8Bit("¸ú×Ù¿ò"));

	m_Setup.widget_2->setVisible(false);
}


TrackBoxDockList::~TrackBoxDockList(void)
{
}

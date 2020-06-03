#include "StdAfx.h"
#include "FileNameDockList.h"


FileNameDockList::FileNameDockList(void)
{
	m_Setup.setupUi(this);
	this->setWindowTitle(QString::fromLocal8Bit("À×´ïÊý¾Ý"));
	m_Setup.widget->setVisible(false);
}


FileNameDockList::~FileNameDockList(void)
{
}

void FileNameDockList::SelectFile( const QString& fileName )
{

}

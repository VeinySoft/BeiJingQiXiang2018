#include "stdafx.h"
#include "MyActionInterface.h"

MyActionInterface::MyActionInterface( QObject * parent = 0 )
	: QAction(parent){}
MyActionInterface::MyActionInterface( const QString & text, QObject * parent  = 0)
	: QAction(text, parent){}
MyActionInterface::MyActionInterface( const QIcon & icon, const QString & text, QObject * parent  = 0)
	: QAction(icon, text, parent){}

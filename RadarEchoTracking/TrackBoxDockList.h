#pragma once
#include "ui_ListDockWidget.h"
class TrackBoxDockList :
	public QDockWidget
{
public:
	TrackBoxDockList(void);
	~TrackBoxDockList(void);
	Ui::DockList m_Setup;
};


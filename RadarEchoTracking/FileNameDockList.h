#pragma once
#include "ui_ListDockWidget.h"

class FileNameDockList :
	public QDockWidget
{
public:
	FileNameDockList(void);
	~FileNameDockList(void);
	Ui::DockList m_Setup;
	void SelectFile(const QString& fileName);
};


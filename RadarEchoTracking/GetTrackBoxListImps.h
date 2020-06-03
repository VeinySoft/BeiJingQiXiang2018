#pragma once
#include "gettrackboxlistinterface.h"

class MainWindow;

class GetTrackBoxListImps :
	public GetTrackBoxListInterface
{
public:
	GetTrackBoxListImps(MainWindow* pMW);
	~GetTrackBoxListImps(void);

	virtual void GetSelectTrackBoxList( QVector<cube_data>& boxList );
	MainWindow* m_pMainWindow;
};


#include "StdAfx.h"
#include "MainWindow.h"
#include "GetTrackBoxListImps.h"

GetTrackBoxListImps::GetTrackBoxListImps(MainWindow* pMW) : m_pMainWindow(pMW)
{
}

GetTrackBoxListImps::~GetTrackBoxListImps(void)
{
}

void GetTrackBoxListImps::GetSelectTrackBoxList( QVector<cube_data>& boxList )
{
	if(m_pMainWindow)
		m_pMainWindow->GetSelectTrackBoxList(boxList);
}

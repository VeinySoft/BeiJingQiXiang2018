#include "StdAfx.h"
#include "StartTrackingAction.h"
#include "ConfigAccesser.h"
#include <QtGlobal>
#include <QVector>
#include "MainWindow.h"
extern MainWindow* g_MainWindow;
extern ConfigAccesser g_GlobleConfig;
ADD_MYACTION_CONTSTRUCTED_DECLARATION(StartTrackingAction)
ControlorInterface* pContrlor = 0;

void StartTrackingAction::MyAction()
{
#if 0
	m_pMap = g_MainWindow->GetMap();
	if(g_MainWindow->CheckTrecReuslt()) 
	{
		g_MainWindow->slot_TrecDone(0);
		return;
	}

	goto_gis::GeosOnTheOsg* pGOTO = goto_gis::GeosOnTheOsg::Instantiate();
	goto_gis::Layer* pLayer = new goto_gis::VectorLayer(0, 0, 1);
	QString strFileName;
	if(g_GlobleConfig.ModeType() == 0)//
	{
		QString strPath = g_GlobleConfig.Path();
		QDir dir(strPath);
		//dir.setSorting(QDir::Time | QDir::Reversed);QDir::NoDotAndDotDot, QDir::Time | QDir::Reversed
		QFileInfoList fileList = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
		/*for(int i = 0; i < fileList.size(); i++)
		{
			qDebug()<<fileList.at(i).absoluteFilePath()<<"/r/n";
		}*/
		if(fileList.size() > 0)
		{
			strFileName = fileList.at(0).absoluteFilePath();
			
		}
	}
	else
	{
		strFileName = g_GlobleConfig.Path() + "/" + g_GlobleConfig.StartTime() + "<->" + g_GlobleConfig.EndTime() + ".nc";
	}
	pGOTO->OpenDataAsLayer(strFileName.toStdString(), pLayer);
	pLayer->CoordTrans(m_pMap->GetCoordinateTransform());
	pLayer->Visible(true);
	pLayer->LayerZ(10);

	pLayer->CreateLayerData();

	m_pMap->AddLayer("radar", pLayer);

	osg::Node* pNode = m_pMap->GetRootNode();

	SetCubesInterface* p_SetCubes = (SetCubesInterface*)(pLayer->GetDataProvider()->GetExtendInterface(0));
	pContrlor = (ControlorInterface*)(pLayer->GetDataProvider()->GetExtendInterface(1));

	QVector<cube_data> myCubeData;
	std::vector<CUBE_DATE> cubes;
	g_GlobleConfig.GetCubes(myCubeData);

	for(int i = 0; i < myCubeData.size(); i++)
	{
		CUBE_DATE cd;
		cube_data mycd = myCubeData.at(i);

		cd.fTargetFstLon = mycd.left_top_lon;
		cd.fTargetFstLat = mycd.right_bottom_lat;
		cd.fTargetLstLon = mycd.right_bottom_lon;
		cd.fTargetLstLat = mycd.left_top_lat;
		cd.iHeightDown = mycd.height_down.toInt();
		cd.iHeightUp = mycd.height_up.toInt();
		cd.iHeight = mycd.height.toInt();

		for(int i = 0; i< g_GlobleConfig.Displays()->size(); i++)
		{
			if(g_GlobleConfig.Displays()->at(i) > 0)
				cd.refs.push_back(g_GlobleConfig.GetValue(i));
		}
		
		cubes.push_back(cd);
	}
	

	p_SetCubes->SetCubes(cubes);
	pContrlor->runTrec(g_MainWindow->GetTrecCallback());
	printf("pNode");
#else
	g_GlobleConfig.ModeType(0);
	g_MainWindow->StartTrack();
	g_MainWindow->FlushCubesList();
	g_MainWindow->FileListScrollButton();
	g_MainWindow->SetFileListSelectMode(QListView::SingleSelection);

	g_MainWindow->DefaultAction();
#endif
}

void StartTrackingAction::InitMyAction()
{
	
}


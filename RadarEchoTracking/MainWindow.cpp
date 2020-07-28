#include "StdAfx.h"

#include "MainWindow.h"
#include "GetTrackBoxListImps.h"
#include "StartTrackingAction.h"
#include "StopTrackingAction.h"
#include "DataInputSettingAction.h"
#include "ExportImageAtion.h"
#include "TrackBoxSettingAction.h"
#include "DrawTrackingBoxPost.h"
#include "RestorePointerAction.h"
#include "ScaleToMapAction.h"
#include "PauseTrackingAction.h"
#include "PrevFrameAction.h"
#include "NextFrameAction.h"
#include "FileNameDockList.h"
#include "TrackBoxDockList.h"
#include "NcFileLayerDockCheckBox.h"
#include "PaoDianDockWidget.h"
#include "PaoDianListDialog.h"
#include "PlayHistoryTrackAction.h"
#include "DirectStartStrackAcion.h"
#include "DistroScaleDraw.h"
#include "QwtPlotDialogImps.h"
#include "ConfigAccesser.h"
#include "DrawClipLine.h"
#include "FlightAndRasterWindow.h"

#include <math.h>

#define distance(x1,x2,y1,y2) sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))

extern ConfigAccesser g_GlobleConfig;

int g_LayerHeight[] = {500, 1500, 2500, 3500, 4500, 5500, 6500, 7500
	, 8500, 9500, 10500, 11500, 12500, 13500, 14500, 15500
	, 16500, 17500
	, 18500, 19500};

int getHeightIndex(int iHeight)
{
	for(int i = 0; i < 20; i++)
	{
		if(iHeight == g_LayerHeight[i])
			return i;
	}

	return -1;
}

class SnapImageDrawCallback : public osg::Camera::DrawCallback 
{
public:
	SnapImageDrawCallback() 
	{ 
		_snapImageOnNextFrame = false; 
	}

	void setFileName(const std::string& filename) { _filename = filename; } 
	const std::string& getFileName() const { return _filename; }
	void setSnapImageOnNextFrame(bool flag) { _snapImageOnNextFrame = flag; } 
	bool getSnapImageOnNextFrame() const { return _snapImageOnNextFrame; }
	virtual void operator () (const ::osg::Camera& camera) const 
	{
		if (!_snapImageOnNextFrame) return;
		int x,y,width,height;
		x = camera.getViewport()->x();
		y = camera.getViewport()->y();
		width = camera.getViewport()->width();
		height = camera.getViewport()->height();
		::osg::ref_ptr< ::osg::Image> image = new ::osg::Image();
		image->readPixels(x,y,width,height,GL_RGB,GL_UNSIGNED_BYTE);
		osgDB::writeImageFile(*image,_filename);
		_snapImageOnNextFrame = false;
	}
protected:
	::std::string _filename; 
	mutable bool _snapImageOnNextFrame;

};

class CameraCallBack : public osg::NodeCallback
{
public:
	CameraCallBack() : m_pViewer(0), m_pText(0), m_pMatrixTransform(0){}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::Camera* pC = dynamic_cast<osg::Camera*>(node);
		if(pC)
		{
			osg::Viewport* vp = m_pViewer->getCamera()->getViewport();
			if(vp)
			{
				double dW = vp->width();
				double dH = vp->height();
				pC->setProjectionMatrixAsOrtho2D(0, dW,0, dH);

				if(m_pText)
					m_pText->setPosition(osg::Vec3(2, dH-20, 0));
			}
		}
		traverse(node,nv);
	}
	inline void setText(osg::ref_ptr<osgText::Text> pText){m_pText = pText;}
	inline void setViewer(osg::View* pV){m_pViewer = pV;}
	inline void setMatrix(const osg::ref_ptr<osg::MatrixTransform>& pM)
	{
		m_pMatrixTransform = pM;
	}
private:
	osg::View* m_pViewer;
	osg::ref_ptr<osgText::Text> m_pText;
	osg::ref_ptr<osg::MatrixTransform> m_pMatrixTransform;
};

class ScaleCallBack : public osg::NodeCallback
{
public:
	ScaleCallBack() : m_pViewer(0), m_pText(0){}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::Camera* pC = dynamic_cast<osg::Camera*>(node);
		if(pC)
		{
			osg::Vec3 v3In1(0,0,0), v3In2(100, 100, 0);
			osg::Vec3 v3Out1, v3Out2;
			double distance1, distance2;

			osg::Matrix VPW = pC->getViewMatrix() *
				pC->getProjectionMatrix() *
				pC->getViewport()->computeWindowMatrix();
			osg::Matrix inverseVPW;
			inverseVPW.invert(VPW);
			v3Out1 = v3In1  * inverseVPW;
			v3Out2 = v3In2  * inverseVPW;

			distance1 = distance(v3In1.x(), v3In2.x(), v3In1.y(), v3In2.y());
			distance2 = distance(v3Out1.x(), v3Out2.x(), v3Out1.z(), v3Out2.z());

			std::string txt = QString("  %1km").arg(QString::number(distance2/distance1 * 100 / 1000, 'f', 0)).toStdString();

			m_pText->setText(txt);
		}
		traverse(node,nv);
	}
	inline void setText(osg::ref_ptr<osgText::Text> pText){m_pText = pText;}
	inline void setViewer(osg::View* pV){m_pViewer = pV;}
private:
	osg::View* m_pViewer;
	osg::ref_ptr<osgText::Text> m_pText;
};


//#include <osg/Matrix>
class TransformCallBack : public osg::NodeCallback
{
public:
	TransformCallBack(int iTop = 30) : m_iTop(iTop), m_pViewer(0){}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::MatrixTransform* pMT = dynamic_cast<osg::MatrixTransform*>(node);
		if(pMT && m_pViewer)
		{
			osg::Viewport* pViewPort = m_pViewer->getCamera()->getViewport();
			osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(m_pViewer);

			double dWidth = pViewPort->width();
			double dHeight = pViewPort->height();
			if(dHeight >  253 + m_iTop)
			{
				osg::Vec3 v3(0, dHeight - (253 + m_iTop), 0);

				osg::Matrix ma;
				ma.makeTranslate(v3);
				pMT->setMatrix(ma);
			}
			
		}
		traverse(node,nv);
	}
	inline void setViewer(osg::View* pV){m_pViewer = pV;}
private:
	osg::View* m_pViewer;
	int m_iTop;
};

inline std::string QStringToStdString(const QString& inStr)
{
	std::string gbkString = "";
	QStdWString stdwstring = inStr.toStdWString();
	size_t len = inStr.length() * 2;
	
	char *p = new char[len];
	memset(p, 0, len);

	if (setlocale(LC_CTYPE, "") == NULL)
		printf("setlocale failed.\n");
	wcstombs(p,stdwstring.c_str(), len);

	gbkString = p;
	delete[] p;

	return gbkString;
}

MainWindow::MainWindow(void) : m_pViewerQT(0), m_pMap(0)
	, m_keySwitchMaipulator(new osgGA::KeySwitchMatrixManipulator)
	, m_pDrawRectMainpulator(new DrawGeometryManipulator(0))
	, m_pDrawLineMainpulator(new DrawGeometryManipulator(0))
	, m_pDefaultMainpulator(new DefaultManipulator)
	, m_pTrecCallbackImaps(new TrecCallbackImps)
	, m_SnapImageDrawCallback(new SnapImageDrawCallback)
	, m_pHUDCamera(new osg::Camera)
	, m_FileNameDockList(new FileNameDockList)
	, m_TrackBoxDockList(new TrackBoxDockList)
	, m_NcFileLayerDockCheckBox(new NcFileLayerDockCheckBox)
	, m_PaoDianDockWidget(new PaoDianDockWidget)
	, m_CurrentFileIndex(1)
	, m_CurrentRealFileIndex(1)
	, m_CurrentReplayFileIndex(1)
	, m_bAutoSaveResult(false)
	, m_pProgressDlg(nullptr)
	, m_pPaoDinaControlerInterface(nullptr)
{
	m_pActionGroup = new QActionGroup(this);
	m_pActionGroup2 = new QActionGroup(this);

	m_pStartTrackAction = new StartTrackingAction(QIcon("./icon/footReal.png"), QString::fromLocal8Bit("实时数据跟踪"), this);
	m_pStopTrackAction = new StopTrackingAction(QIcon("./icon/footStop.png"), QString::fromLocal8Bit("停止跟踪"), this);
	m_pTrackBoxSettingAction = new TrackBoxSettingAction(QIcon("./icon/boxSetting.png"), QString::fromLocal8Bit("跟踪框设置"), this);
	m_pDataInputSettingAction = new DataInputSettingAction(QIcon("./icon/DataInputSetting.png"), QString::fromLocal8Bit("数据源设置"), this);
	//m_pExprotImageAction = new ExportImageAtion(QIcon("./icon/export.png"), QString::fromLocal8Bit("保存配置"), this);
	m_pRestorePointerAction = new RestorePointerAction(QIcon("./icon/hand.png"), QString::fromLocal8Bit("恢复平移缩放"), this);
	m_pScaleToMapAction = new ScaleToMapAction(QIcon("./icon/allmap.png"), QString::fromLocal8Bit("缩放到全图"), this);
	m_pPauseTrackAction = new PauseTrackingAction(QIcon("./icon/footPause.png"), QString::fromLocal8Bit("暂停跟踪"), this);
	m_pPrevAction = new PrevFrameAction(QIcon("./icon/pause.png"), QString::fromLocal8Bit("暂停数据回放"), this);
	m_pNextAction = new NextFrameAction(QIcon("./icon/stop.png"), QString::fromLocal8Bit("停止数据回放"), this);
	m_pPlayHistoryTrackAction = new PlayHistoryTrackAction(QIcon("./icon/play.png"), QString::fromLocal8Bit("动画播放"), this);
	m_DirectStartTrackAction = new DirectStartStrackAcion(QIcon("./icon/foot.png"), QString::fromLocal8Bit("开始跟踪"), this);
	m_pLoadHidtoryAtion = new QAction(QIcon("./icon/Reload.png"), QString::fromLocal8Bit("历史数据回放"), this);
	m_pHelpAction = new QAction(/*QIcon("./icon/Reload.png"), */QString::fromLocal8Bit("使用说明"), this);
	m_pAboutAction = new QAction(/*QIcon("./icon/Reload.png"), */QString::fromLocal8Bit("关于"), this);
	m_pZoomInAction = new QAction(QIcon("./icon/zoomin.png"), QString::fromLocal8Bit("放大地图"), this);
	m_pZoomOutAction = new QAction(QIcon("./icon/zoomout.png"), QString::fromLocal8Bit("缩小地图"), this);
	m_pShutdownAction = new QAction(QIcon("./icon/shutdown.png"), QString::fromLocal8Bit("关闭系统"), this);
	m_pSaveResultAction = new QAction(QIcon("./icon/export.png"), QString::fromLocal8Bit("导出跟踪结果"), this);
	m_pSectionLineAction = new QAction(QIcon("./icon/section.png"), QString::fromLocal8Bit("任意剖面图"), this);
	m_pStrackBoxSectionAction = new QAction(QIcon("./icon/rect.png"), QString::fromLocal8Bit("跟踪框剖面图"), this);
	m_pDistanctTestAction = new QAction(QIcon("./icon/distance.png"), QString::fromLocal8Bit("测量直线距离"), this);
	m_pShowResultAction = new QAction(QIcon("./icon/show.png"), QString::fromLocal8Bit("察看结果"), this);
	m_pSaveSenceAtion = new QAction(QIcon("./icon/image.png"), QString::fromLocal8Bit("导出场景"), this);
	m_pAutoStartToTrack  = new QAction(QIcon("./icon/auto.png"), QString::fromLocal8Bit("自动开始跟踪"), this);
	m_pAccordingPaoDianToTrackBoxAtion = new QAction(QIcon("./icon/paodiancube.png"), QString::fromLocal8Bit("炮点生成跟踪框"), this);
	m_pMoveTrackBoxAction = new QAction(QIcon("./icon/copyBox.png"), QString::fromLocal8Bit("移动跟踪框"), this);
	m_pExportRegionDataAction = new QAction(QIcon("./icon/export_file.png"), QString::fromLocal8Bit("导出选定区域"), this);																					  
	m_pOpenFlightPathAction = new QAction(QIcon("./icon/flightPath.png"), QString::fromLocal8Bit("轨迹与固定点剖面"), this);

	m_pPauseTrackAction->setCheckable(true);
	m_pRestorePointerAction->setCheckable(true);
	m_pRestorePointerAction->setChecked(true);
	m_pSectionLineAction->setCheckable(true);
	//m_pCopyTrackBoxAction->setCheckable(true);
	m_pStrackBoxSectionAction->setCheckable(true);
	m_pDistanctTestAction->setCheckable(true);
	m_pPrevAction->setCheckable(true);
	m_pTrackBoxSettingAction->setCheckable(true);
	m_pMoveTrackBoxAction->setCheckable(true);

	m_pActionGroup->addAction(m_pStartTrackAction);
	m_pActionGroup->addAction(m_DirectStartTrackAction);
	m_pActionGroup->addAction(m_pStopTrackAction);
	m_pActionGroup2->addAction(m_pTrackBoxSettingAction);
	m_pActionGroup->addAction(m_pDataInputSettingAction);
	//m_pActionGroup->addAction(m_pExprotImageAction);
	m_pActionGroup2->addAction(m_pRestorePointerAction);
	m_pActionGroup->addAction(m_pScaleToMapAction);
	m_pActionGroup->addAction(m_pPauseTrackAction);
	m_pActionGroup->addAction(m_pPlayHistoryTrackAction);
	m_pActionGroup->addAction(m_pLoadHidtoryAtion);
	m_pActionGroup->addAction(m_pPrevAction);
	m_pActionGroup->addAction(m_pNextAction);
	m_pActionGroup->addAction(m_pZoomInAction);
	m_pActionGroup->addAction(m_pZoomOutAction);
	m_pActionGroup->addAction(m_pShutdownAction);
	m_pActionGroup->addAction(m_pSaveResultAction);
	m_pActionGroup->addAction(m_pSectionLineAction);
	m_pActionGroup->addAction(m_pStrackBoxSectionAction);
	m_pActionGroup->addAction(m_pDistanctTestAction);
	m_pActionGroup->addAction(m_pShowResultAction);
	m_pActionGroup->addAction(m_pSaveSenceAtion);
	m_pActionGroup->addAction(m_pAutoStartToTrack);
	m_pActionGroup->addAction(m_pAccordingPaoDianToTrackBoxAtion);
	m_pActionGroup->addAction(m_pMoveTrackBoxAction);
	m_pActionGroup->addAction(m_pExportRegionDataAction);
	m_pActionGroup->addAction(m_pOpenFlightPathAction);
	//////////////////////////////////////////////////////////////////////////
	addDockWidget(Qt::LeftDockWidgetArea, m_FileNameDockList);
	addDockWidget(Qt::LeftDockWidgetArea, m_TrackBoxDockList);
	addDockWidget(Qt::LeftDockWidgetArea, m_NcFileLayerDockCheckBox);
	addDockWidget(Qt::RightDockWidgetArea, m_PaoDianDockWidget);

	m_pFileListItemModel = new QStandardItemModel(m_FileNameDockList);
	m_pTrackBoxItemModel = new QStandardItemModel(m_TrackBoxDockList);
	
	m_FileNameDockList->m_Setup.listView->setModel(m_pFileListItemModel);
	m_TrackBoxDockList->m_Setup.listView->setModel(m_pTrackBoxItemModel);
	m_TrackBoxDockList->m_Setup.listView->setSelectionMode(QAbstractItemView::MultiSelection);
	//////////////////////////////////////////////////////////////////////////
	qRegisterMetaType<P_TREC_NC_MAP_OUT>("P_TREC_NC_MAP_OUT");
	qRegisterMetaType<QStringList>("QStringList&");
	qRegisterMetaType<QString>("QString&");
	connect(m_pTrecCallbackImaps, SIGNAL(signal_TrecDone( QStringList&, QString&, P_TREC_NC_MAP_OUT))
		, SLOT(slot_TrecDone(QStringList&, QString&, P_TREC_NC_MAP_OUT)), Qt::QueuedConnection);

	//////////////////////////////////////////////////////////////////////////

	//m_MySloveThread.m_pMainWindow = this;
	//m_MySloveThread.m_bToTracking = true;
	m_MySloveThread.setMainWindow(this);
	m_MySloveThread.setIsTracking(true);
	m_MyDisplayRealFile.m_pMainWindow = this;
	connect(&m_MySloveThread, SIGNAL(signal_TrackDone(const QString&)), SLOT(slot_TrackDone(const QString&)), Qt::QueuedConnection);
	connect(&m_MySloveThread, SIGNAL(signal_Replay(const QString&)), SLOT(slot_Replay(const QString&)), Qt::QueuedConnection);
	connect(&m_MySloveThread, SIGNAL(signal_ToDeleteOld(const QString&)), SLOT(slot_ToDeleteOld(const QString&)), Qt::QueuedConnection);
	connect(this, SIGNAL(signal_InsertFile(const QString&)), SLOT(slot_InsertFile(const QString&)), Qt::QueuedConnection);
	connect(&m_MySloveThread, SIGNAL(signal_StopTrack()), SLOT(slot_StopTrack()), Qt::QueuedConnection);
	connect(&m_MyDisplayRealFile, SIGNAL(signal_Replay(const QString&)), SLOT(slot_Replay(const QString&)), Qt::QueuedConnection);
	connect(m_NcFileLayerDockCheckBox->GetCheckBoxGroup(), SIGNAL(buttonClicked(int)), SLOT(slot_SelectLayer(int)));
	connect(m_FileNameDockList->m_Setup.listView, SIGNAL(doubleClicked( const QModelIndex&)), SLOT(slot_doubleClicked(const QModelIndex&)));
	connect(m_FileNameDockList->m_Setup.listView, SIGNAL(clicked( const QModelIndex&)), SLOT(slot_NameListClicked(const QModelIndex&)));
	connect(m_TrackBoxDockList->m_Setup.listView, SIGNAL(clicked(const QModelIndex&)), SLOT(slot_TrackListClicked(const QModelIndex&)));
	//////////////////////////////////////////////////////////////////////////
	connect(m_TrackBoxDockList->m_Setup.pushButton, SIGNAL(clicked(bool)), SLOT(slot_AddBox(bool)));
	connect(m_TrackBoxDockList->m_Setup.pushButton_2, SIGNAL(clicked(bool)), SLOT(slot_ModifyBox(bool)));
	connect(m_TrackBoxDockList->m_Setup.pushButton_3, SIGNAL(clicked(bool)), SLOT(slot_DeleteBox(bool)));
	connect(m_TrackBoxDockList->m_Setup.pushButton_6, SIGNAL(clicked(bool)), SLOT(slot_CopyBox(bool)));

	connect(m_FileNameDockList->m_Setup.pushButton_4, SIGNAL(clicked(bool)), SLOT(slot_PreviousFile(bool)));
	connect(m_FileNameDockList->m_Setup.pushButton_5, SIGNAL(clicked(bool)), SLOT(slot_NextFile(bool)));
}

MainWindow::~MainWindow(void)
{
}

void MainWindow::slot_ActionTriggered( QAction* action )
{
	MyActionInterface* pMAI = dynamic_cast<MyActionInterface*>(action);
	if(pMAI) pMAI->MyAction();

	if(action == m_pLoadHidtoryAtion)
	{
		StopRealDispaly();

		g_GlobleConfig.ModeType(1);
		SetFileListSelectMode(QListView::ExtendedSelection);
		this->StartTrack();
	}
	else if(action == m_pZoomInAction)
	{
		m_pDefaultMainpulator->MapMZoomIn();
	}
	else if(action == m_pZoomOutAction)
	{
		m_pDefaultMainpulator->MapMZoomOut();
	}
	else if(action == m_pShutdownAction)
	{
		this->close();
	}
	else if(action == m_pSaveResultAction)
	{
		if(m_QwtPlotDialogMap.size() == 0) return;

		QString strPath = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("保存跟踪结果"));
		if(strPath.size() != 0)
		{
			QList<QString> keys = m_QwtPlotDialogMap.keys();
			for(int i = 0; i < keys.size(); i++)
			{
				QwtPlotDialogImps* pQwtPlotDialog = m_QwtPlotDialogMap.value(keys[i]);
				pQwtPlotDialog->SaveReuslt(strPath + "\\" + keys[i] + "\\");
			}
		}
	}
	else if(action == m_pSectionLineAction)
	{
		static int iFirst = 0;

		bool bChecked = m_pSectionLineAction->isChecked();
		if(iFirst == 1)
		{
			iFirst = 0;
			if(bChecked == true)
				bChecked = false;
			else
				bChecked = true;
		}
		
		if(bChecked)
		{
			iFirst = 1;
			m_pDrawClipLine->m_pCI = m_pControlorInterface;
			m_pDrawClipLine->m_KeySwitch = m_keySwitchMaipulator;
			m_pDrawClipLine->SetLineUsage(DrawClipLine::NORMAL);
			m_keySwitchMaipulator->selectMatrixManipulator(2);
		}
		else
		{
			m_pSectionLineAction->setChecked(false);
			m_keySwitchMaipulator->selectMatrixManipulator(0);
			return;
		}
	}
	else if(action == m_pMoveTrackBoxAction)
	{
		static int iFirst3 = 0;

		bool bChecked = m_pMoveTrackBoxAction->isChecked();
		if(iFirst3 == 1)
		{
			iFirst3 = 0;
			if(bChecked == true)
				bChecked = false;
			else
				bChecked = true;
		}

		if(bChecked)
		{
			iFirst3 = 1;
			m_pDrawClipLine->m_pCI = m_pControlorInterface;
			m_pDrawClipLine->m_KeySwitch = m_keySwitchMaipulator;
			m_pDrawClipLine->SetLineUsage(DrawClipLine::MOVE);
			m_keySwitchMaipulator->selectMatrixManipulator(2);
		}
		else
		{
			m_pMoveTrackBoxAction->setChecked(false);
			m_keySwitchMaipulator->selectMatrixManipulator(0);
			return;
		}
	}
	else if(action == m_pStrackBoxSectionAction)
	{
		static int iFirst2 = 0;

		bool bChecked = m_pStrackBoxSectionAction->isChecked();
		if(iFirst2 == 1)
		{
			iFirst2 = 0;
			if(bChecked == true)
				bChecked = false;
			else
				bChecked = true;
		}

		if(bChecked)
		{
			iFirst2 = 1;
			m_pDrawClipLine->m_pCI = m_pControlorInterface;
			m_pDrawClipLine->m_KeySwitch = m_keySwitchMaipulator;
			m_pDrawClipLine->SetLineUsage(DrawClipLine::STACK);
			m_keySwitchMaipulator->selectMatrixManipulator(2);
		}
		else
		{
			m_pStrackBoxSectionAction->setChecked(false);
			m_keySwitchMaipulator->selectMatrixManipulator(0);
			return;
		}

	}
	else if(action == m_pDistanctTestAction)
	{
		static int iFirst2 = 0;

		bool bChecked = m_pDistanctTestAction->isChecked();
		if(iFirst2 == 1)
		{
			iFirst2 = 0;
			if(bChecked == true)
				bChecked = false;
			else
				bChecked = true;
		}

		if(bChecked)
		{
			iFirst2 = 1;
			m_pDrawClipLine->m_pCI = m_pControlorInterface;

			/*if(m_OpenFileName.size())
			{
				m_pDrawClipLine->m_FileName = m_OpenFileName;
			}
			else
			{
				if(m_FileList.size() > 1)
					m_pDrawClipLine->m_FileName 
					= m_FileBaseMapToFilePath.value(m_FileList.at(0));
				else
				{
					m_pDrawClipLine->m_FileName = "";
					return;
				}
			}*/

			m_pDrawClipLine->m_KeySwitch = m_keySwitchMaipulator;
			m_pDrawClipLine->SetLineUsage(DrawClipLine::DISTANCE);
			m_keySwitchMaipulator->selectMatrixManipulator(2);
		}
		else
		{
			m_pDistanctTestAction->setChecked(false);
			m_keySwitchMaipulator->selectMatrixManipulator(0);
			return;
		}
	}
	else if(action == m_pShowResultAction)
	{
		QList<QString> keys = m_QwtPlotDialogMap.keys();
		for(int i = 0; i < keys.size(); i++)
		{
			QwtPlotDialogImps* pQwtPlotDialog = m_QwtPlotDialogMap.value(keys[i]);

			bool bV = pQwtPlotDialog->IsVisible();
			if(!bV)
				pQwtPlotDialog->Show(keys[i]);
		}
	}
	else if(action == m_pSaveSenceAtion)
	{
		QString strFileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("导出图片"), "", "JPEG (*.jpg *.jpeg)");
		if(strFileName.size() > 0)
			ExportImage(strFileName);
	}
	else if(action == m_pAutoStartToTrack)
	{
		AutoStartToTrack();
	}
	else if(action == m_pAccordingPaoDianToTrackBoxAtion)
	{
		GenerateTrackBox();
	}
	else if(action == m_pExportRegionDataAction)
	{
		ExportRegionData();				
	}
	else if(m_pOpenFlightPathAction == action)
	{
		QStringList files = m_RasterNeedFiles;
		//files.append();
		if(files.size() == 0)
		{
			QStringList ncFilesList;
			ncFilesList.push_back(m_FileList[0]);
			ncFilesList.push_back(m_FileList[m_FileList.size() - 1]);
			m_pRasterWindow->slot_updateSelectNcFiles(ncFilesList);
		}
		else
		{
			QStringList ncFilesList;
			ncFilesList.push_back(files[0]);
			ncFilesList.push_back(m_FileList[m_FileList.size() - 1]);
			m_pRasterWindow->slot_updateSelectNcFiles(files);
		}
		m_pRasterWindow->show();
		//m_pRasterWindow->FillList();
	}
	else
	{
		return;
	}
}

void MainWindow::LoadBusinessFeature()
{
	MainControlFrame* pMainControlFrame 
		= MainControlFrame::GetMainControlFrameInstance();
	pMainControlFrame->LoadFramePlugins(std::string(""));

	m_pMap = pMainControlFrame->CreateMapFromFactory(0);
	osg::StateSet* pStateSet = m_pMap->GetRootNode()->getOrCreateStateSet();
	//pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	//pStateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
	//pStateSet->setMode(GL_POLYGON_SMOOTH, osg::StateAttribute::ON);
	//pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);

	m_pView = pMainControlFrame->CreateOsgView(m_pMap);
	//////////////////////////////////////开启Shader////////////////////////////////////
	/*osgViewer::Viewer::Windows windows;
	osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(m_pView);
	pViewer->getWindows(windows);
	for(osgViewer::Viewer::Windows::iterator itr = windows.begin();
		itr != windows.end();
		++itr)
	{
		(*itr)->getState()->setShaderCompositionEnabled(true);
	}*/
	//////////////////////////////////////////////////////////////////////////
	m_pView->setLightingMode(osg::View::NO_LIGHT);
	//osg::DisplaySettings::instance()->setNumMultiSamples(16);
	m_pMap->SetAttachView((osgViewer::View*)m_pView);
	osg::Camera* pC = m_pView->getCamera();
	pC->setClearColor(osg::Vec4(1, 1, 1, 1)/*osg::Vec4(238.0/255.0, 243.0/255.0, 250.0/255.0, 1)*/);
	pC->setPostDrawCallback(m_SnapImageDrawCallback);
	QString strMapConfig = QCoreApplication::applicationDirPath()
		+ QString::fromLocal8Bit("/Config/Default.prj");
	m_pMap->Load2DMapFromConfig(QStringToStdString(strMapConfig));

	m_pViewerQT = new ViewerQT((osgViewer::Viewer*)m_pView, this);
	
	m_pViewerQT->setParent(this);
	//QMdiArea* mdiArea = new QMdiArea(this);
	//mdiArea->addSubWindow(new QTextBrowser(mdiArea));
	this->setCentralWidget(m_pViewerQT);

	m_keySwitchMaipulator->addMatrixManipulator('1', "default", m_pDefaultMainpulator);
	m_keySwitchMaipulator->addMatrixManipulator('2', "drawRect", m_pDrawRectMainpulator);
	m_keySwitchMaipulator->addMatrixManipulator('3', "drawLine", m_pDrawLineMainpulator);
	//m_keySwitchMaipulator->selectMatrixManipulator('1');

	m_pView->setCameraManipulator(m_keySwitchMaipulator);
	//m_keySwitchMaipulator->home(m_pView->event, *m_pView);
	AddDrawRectManipulator();
	AddDrawLineManipulator();

	m_pRootNode = m_pMap->GetRootNode()->asGroup();

	/************************************************************************/
	/* 加载雷达跟踪显示图层                                                    */
	/************************************************************************/
	//CreateRadarDisplayLayer();
	/************************************************************************/
	/* 初始化dock列表                                                        */
	/************************************************************************/
	//FlushList();
	/************************************************************************/
	/* 初始化剖面窗口*/
	/************************************************************************/
	InitRasterWindow();	
}

void MainWindow::InitRasterWindow()
{
	m_pRasterWindow = new FlightAndRasterWindow(this);
	if(m_pRasterWindow != 0)
	{
		m_pRasterWindow->SetMapToFilePath(&m_FileBaseMapToFilePath);
		m_pRasterWindow->SetFileList(&m_FileList);
		connect(this, SIGNAL(signal_SelectFiles(const QStringList&)), m_pRasterWindow, SLOT(slot_updateSelectNcFiles(const QStringList&)));
	}
}

void MainWindow::TransformData()
{
	m_pMap->CreateMapData();

	//pMap->GetAttachView()->setCameraManipulator(new osgGA::TrackballManipulator);
	//m_pMap->GetAttachView()->getCamera()->setClearColor(osg::Vec4(0, 0, 0, 1));
	/*osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(pMap->GetAttachView());
	pViewer->run();*/
	//m_pMap->ScaleToLayer();
}

void MainWindow::SetUpActions( QToolBar* pToolBar )
{
	
	pToolBar->addAction(m_pStartTrackAction);
	pToolBar->addAction(m_DirectStartTrackAction);
	pToolBar->addAction(m_pPauseTrackAction);
	pToolBar->addAction(m_pStopTrackAction);
	pToolBar->addSeparator();
	pToolBar->addAction(m_pZoomInAction);
	pToolBar->addAction(m_pZoomOutAction);
	pToolBar->addAction(m_pRestorePointerAction);
	pToolBar->addAction(m_pScaleToMapAction);
	pToolBar->addSeparator();
	pToolBar->addAction(m_pLoadHidtoryAtion);
	pToolBar->addAction(m_pPlayHistoryTrackAction);
	pToolBar->addAction(m_pPrevAction);
	pToolBar->addAction(m_pNextAction);
	pToolBar->addSeparator();
	pToolBar->addAction(m_pAutoStartToTrack);
	pToolBar->addSeparator();
	pToolBar->addAction(m_pAccordingPaoDianToTrackBoxAtion);
	pToolBar->addAction(m_pTrackBoxSettingAction);
	pToolBar->addAction(m_pMoveTrackBoxAction);
	pToolBar->addSeparator();
	pToolBar->addAction(m_pSectionLineAction);
	pToolBar->addAction(m_pStrackBoxSectionAction);
	pToolBar->addAction(m_pDistanctTestAction);
	pToolBar->addSeparator();
	pToolBar->addAction(m_pDataInputSettingAction);
	pToolBar->addAction(m_pShowResultAction);
	pToolBar->addAction(m_pSaveResultAction);
	pToolBar->addAction(m_pSaveSenceAtion);
	pToolBar->addAction(m_pExportRegionDataAction);
	pToolBar->addSeparator();
	pToolBar->addAction(m_pOpenFlightPathAction);
	pToolBar->addSeparator();
	pToolBar->addAction(m_pShutdownAction);
	
	//pToolBar->addAction(m_pExprotImageAction);
	
	/*QComboBox* pComboBox = new QComboBox;
	pComboBox->addItem("111111111111");
	pComboBox->addItem("111");
	pComboBox->addItem("111");
	pComboBox->addItem("111");
	pComboBox->addItem("111");

	pToolBar->addWidget(pComboBox);*/

	connect(m_pActionGroup, SIGNAL(triggered ( QAction *)), this
		, SLOT(slot_ActionTriggered( QAction*)));
	connect(m_pActionGroup2, SIGNAL(triggered ( QAction *)), this
		, SLOT(slot_ActionTriggered( QAction*)));
}

void MainWindow::AddDrawRectManipulator()
{
	goto_gis::VectorLayer* pVectorLayer = new goto_gis::VectorLayer(0,0,1);
	pVectorLayer->LayerZ(0.f);
	pVectorLayer->Visible(true);

	m_pMap->AddLayer("DrawLayer", pVectorLayer);
	DrawTrackingBoxPost* pDrawResult = new DrawTrackingBoxPost;
	pDrawResult->m_DrawLayer = pVectorLayer;

	DrawRect* pDR = new DrawRect();
	pDR->SetDrawLayer(pVectorLayer);
	pDR->SetColor(osg::Vec4(1.0f, 0.f, 0.f, 1.0f));
	pDR->SetResultHandle(pDrawResult);

	m_pDrawRectMainpulator->SetDrawGeometryData(pDR);
}

void MainWindow::AddDrawLineManipulator()
{
	goto_gis::VectorLayer* pVectorLayer = new goto_gis::VectorLayer(0,0,1);
	pVectorLayer->LayerZ(0.f);
	pVectorLayer->Visible(true);
	pVectorLayer->CoordTrans(m_pMap->GetCoordinateTransform());

	m_pMap->AddLayer("DrawLineLayer", pVectorLayer);
	DrawTrackingBoxPost* pDrawResult = new DrawTrackingBoxPost;
	pDrawResult->m_DrawLayer = pVectorLayer;

	m_pDrawClipLine = new DrawClipLine();
	m_pDrawClipLine->SetBoxListInterface(new GetTrackBoxListImps(this));
	m_pDrawClipLine->SetDrawLayer(pVectorLayer);
	m_pDrawClipLine->SetColor(osg::Vec4(1.0f, 0.f, 0.f, 1.0f));
	m_pDrawClipLine->SetResultHandle(pDrawResult);

	m_pDrawLineMainpulator->SetDrawGeometryData(m_pDrawClipLine);
}

void MainWindow::ScaleToMap()
{
	m_keySwitchMaipulator->computeHomePosition();
	m_pDefaultMainpulator->home(0);
}


void MainWindow::slot_TrecDone( QStringList& fileList, QString& strCubeName, P_TREC_NC_MAP_OUT pOutMap )
{
	if(pOutMap->size() == 0)
	{
		delete pOutMap;
		return;
	}

	if(m_TrecOutData.size() <= 0)
	{
		m_TrecOutData = *pOutMap;
		delete pOutMap;
	}

	QwtPlotDialogImps qdi, qdi1;
	qdi.InitPlot();qdi1.InitBar();
	QPolygonF pf;
	QMap<int, QString> dataTimeList, dataTimeList2;
	QPolygonF pf2;
	QPolygonF pf3;
	QPolygonF pf4;

	int iXIntev = 0;

	QVector<QVector<double>> barValues;
	QVector<int> values;
	QVector<QColor> colors;
	std::vector<std::string> fileNames;

	m_pControlorInterface->GetFileNames(fileNames);

	bool bOne = true;
	for(size_t i = 0; i < fileNames.size(); i++)
	{
		void* pNcFilePointer = 0;
		m_pControlorInterface->FileToName(fileNames[i], &pNcFilePointer);
		TREC_NC_MAP_OUT::iterator be = m_TrecOutData.find((NcFile*)pNcFilePointer);

		if(be == m_TrecOutData.end()) continue;
		if(bOne && be != m_TrecOutData.end())
		{
			for(size_t i = 0; i < be->second.ratio.interval.size(); i++)
			{
				int iV = be->second.ratio.interval.at(i);
				values.push_back(iV);
				int iIndex = m_pControlorInterface->GetIndexFromValue(iV);
				colors.push_back(m_pControlorInterface->GetColor(iIndex));
			}

			bOne = false;
		}

		pf.push_back(QPointF(iXIntev, be->second.precipitation.precipitation));
		pf2.push_back(QPointF(iXIntev, be->second.precipitation.content));
		
		QVector<double> barValue;
		for(size_t ii = 0; ii < be->second.ratio.ratio.size(); ii++)
		{
			barValue.push_back(be->second.ratio.ratio.at(ii));
		}
		barValues.push_back(barValue);
		//pf3.push_back(QPointF(be->second.ratio.interval, be->second.ratio.ratio));
		
		float fTargetFstLon = be->second.speed.Elon;
		float fTargetFstLat = be->second.speed.Elat;
		float fTargetLstLon = be->second.speed.NLon;
		float fTargetLstLat = be->second.speed.Nlat;
		osg::Vec3 p1, p2, p3, p4;
		p1.set(fTargetFstLon, fTargetLstLat, 0);
		p2.set(fTargetLstLon, fTargetLstLat, 0);
		p3.set(fTargetLstLon, fTargetFstLat, 0);
		p4.set(fTargetFstLon, fTargetFstLat, 0);

		m_pControlorInterface->AddRect("", p1, p2, p3, p4, osg::Vec4(0.0, 1.0, 0.0, 1.0));
		std::string strName = fileNames[i];

		dataTimeList.insert(iXIntev, QString::fromStdString(strName));
		dataTimeList2.insert(i - 1, QString::fromStdString(strName));
		iXIntev+=10;
	}

	DistroScaleDraw* dsd = new DistroScaleDraw(Qt::Vertical, dataTimeList);

	qdi.DrawPlot(pf, 0, QColor(56, 121, 111), QString::fromLocal8Bit("降水量"));
	qdi.DrawPlot(pf2, dsd, QColor(123, 231, 111), QString::fromLocal8Bit("液体水含量"));
	
	qdi.AddMark(pf);
	qdi.AddMark(pf2);
	//qdi.DrawPlot(pf3, QColor(56, 121, 111), QString::fromLocal8Bit("反射率百分比"));
	qdi.Show(QString::fromLocal8Bit("液体水含量、降水量"), 0, 40);

	DistroScaleDraw* dsd2 = new DistroScaleDraw(Qt::Vertical, dataTimeList2);
	qdi1.DrawBar(values, colors, barValues, dsd2);
	qdi1.AddBarMark(barValues);
	qdi1.Show(QString::fromLocal8Bit("反射百分率"), 841, 40);
}

void MainWindow::ExportImage( const QString& fileName )
{
	m_SnapImageDrawCallback->setFileName(QStringToStdString(fileName));
	m_SnapImageDrawCallback->setSnapImageOnNextFrame(true);
}

void MainWindow::ExportRegionData()
{
	QItemSelectionModel* pISM = m_TrackBoxDockList->m_Setup.listView->selectionModel();
	
	QModelIndexList selectBoxes = pISM->selectedRows();
	if(selectBoxes.size() <= 0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("导出框"), QString::fromLocal8Bit("请选择导出框。（可多选）"));
		return;
	}

	QString strOutCSV = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("导出框内雷达数据"), "", "csv (*.csv)");
	if(strOutCSV.size() == 0) return;

	for(int i = 0; i < selectBoxes.size(); i++)
	{
		QStandardItem* item =  m_pTrackBoxItemModel->item(selectBoxes.at(i).row());
		cube_data cd = g_GlobleConfig.GetCubeFromName(item->text());
		int upHeight = getHeightIndex(cd.height_up.toInt());
		int downHeight = getHeightIndex(cd.height_down.toInt());
		QString strName = GetSelectFileName();
		QString strFilePath = m_FileBaseMapToFilePath.value(strName);
		//QString strOutCSV = strPath + QDir::separator() + QString::fromLocal8Bit("_") + cd.name + strName + QString::fromLocal8Bit(".csv");
		m_pControlorInterface->ExportPartNcFile(strFilePath, osg::Vec3(cd.left_top_lon, cd.left_top_lat, 0), osg::Vec3(cd.right_bottom_lon, cd.right_bottom_lat, 0), 0, upHeight, downHeight, strOutCSV);
	}
}

void MainWindow::AddFileTime()
{
	osg::ref_ptr<osg::Geode> pHUDGeode = new osg::Geode;
	modelview = new osg::MatrixTransform;
	
	pHgtTxt = new osgText::Text();
	pHgtTxt->setFont("Fonts/simhei.ttf");
	pHgtTxt->setText("0000-00-0 00:00:00 (BJT)"); 
	pHgtTxt->setBackdropColor(osg::Vec4(1, 1, 1, 1));
	pHgtTxt->setBoundingBoxColor(osg::Vec4(0, 0, 0, 1));
	pHgtTxt->setPosition(osg::Vec3(20, 20, 0));
	pHgtTxt->setAutoRotateToScreen(true);
	pHgtTxt->setCharacterSize(25);
	pHgtTxt->setBackdropType(osgText::Text::OUTLINE);
	pHgtTxt->setDrawMode(osgText::Text::TEXT | osgText::Text::BOUNDINGBOX);
	pHgtTxt->setCharacterSizeMode(osgText::TextBase::SCREEN_COORDS);
	osg::Vec4 m_markColor(.0, .0,.0,1);
	pHgtTxt->setColor(m_markColor);
	pHgtTxt->setAlignment(osgText::Text::LEFT_BOTTOM);

	pHUDGeode->addDrawable(pHgtTxt);

	osg::ref_ptr<CameraCallBack> pPCB = new CameraCallBack;
	pPCB->setMatrix(modelview);
	pPCB->setText(pHgtTxt);
	pPCB->setViewer(m_pView);
	m_pHUDCamera->setUpdateCallback(pPCB);
	m_pHUDCamera->addChild(pHUDGeode);
	m_pHUDCamera->setProjectionMatrixAsOrtho2D(0,1027,0,639);
	//m_pHUDCamera->setClearDepth(1.0);
	m_pHUDCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
	m_pHUDCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	m_pHUDCamera->setViewMatrix(osg::Matrix::identity());
	m_pHUDCamera->setRenderOrder(osg::Camera::POST_RENDER);
	m_pRootNode->addChild(m_pHUDCamera);


	AddScale(pHUDGeode);
}

void MainWindow::AddScale(const osg::ref_ptr<osg::Geode>& geode)
{
	osg::ref_ptr<osgText::Text> pScaleTxt = new osgText::Text();
	pScaleTxt->setFont("Fonts/simhei.ttf");
	pScaleTxt->setText("1km"); 
	pScaleTxt->setBackdropColor(osg::Vec4(1, 1, 1, 1));
	pScaleTxt->setBoundingBoxColor(osg::Vec4(0, 0, 0, 1));
	pScaleTxt->setPosition(osg::Vec3(10, 10, 0));
	pScaleTxt->setAutoRotateToScreen(true);
	pScaleTxt->setCharacterSize(25);
	//pScaleTxt->setBackdropType(osgText::Text::OUTLINE);
	pScaleTxt->setDrawMode(osgText::Text::TEXT);
	pScaleTxt->setCharacterSizeMode(osgText::TextBase::SCREEN_COORDS);
	osg::Vec4 m_markColor(.0, .0,.0,1);
	pScaleTxt->setColor(m_markColor);
	pScaleTxt->setAlignment(osgText::Text::LEFT_BOTTOM);

	osg::ref_ptr<osg::Geometry> pScaleGeom  = new osg::Geometry;
	///////////////////////////////////////////////////////////////////////
	osg::ref_ptr<osg::Vec3Array> pVertexArray = new osg::Vec3Array;
	pVertexArray->push_back(osg::Vec3(100,0, 0));
	pVertexArray->push_back(osg::Vec3(100,10,0));
	pVertexArray->push_back(osg::Vec3(0,10,0));
	pVertexArray->push_back(osg::Vec3(0,0,0));
	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;
	texcoords->push_back(osg::Vec2(1.0f,0.0f));
	texcoords->push_back(osg::Vec2(1.0f,1.0f));
	texcoords->push_back(osg::Vec2(0.0f,1.0f));
	texcoords->push_back(osg::Vec2(0.0f,0.0f));
	osg::ref_ptr<osg::Vec4Array> pColorArray = new osg::Vec4Array;
	pColorArray->push_back(osg::Vec4(1.0f, 1,1,1.0f));
	pColorArray->push_back(osg::Vec4(1.0f, 1,1,1.0f));
	pColorArray->push_back(osg::Vec4(1.0f, 1,1,1.0f));
	pColorArray->push_back(osg::Vec4(1.0f, 1,1,1.0f));
	osg::ref_ptr<osg::Vec3Array> pNormalArray = new osg::Vec3Array;
	pNormalArray->push_back(osg::Vec3(0, -1, 0));
	pNormalArray->push_back(osg::Vec3(0, -1, 0));
	pNormalArray->push_back(osg::Vec3(0, -1, 0));
	pNormalArray->push_back(osg::Vec3(0, -1, 0));

	pScaleGeom->setVertexArray(pVertexArray);
	pScaleGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
	pScaleGeom->setTexCoordArray(0, texcoords);
	pScaleGeom->setColorArray(pColorArray);
	pScaleGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::StateSet* pStateSet = pScaleGeom->getOrCreateStateSet();
	///////////////////////////////////设置纹理///////////////////////////////////////
	osg::Texture2D* pTexture2D = new osg::Texture2D;
	pTexture2D->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
	pTexture2D->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
	pTexture2D->setDataVariance(osg::Object::DYNAMIC);
	osg::Image* pImage = osgDB::readImageFile("./icon/scale.png");
	pTexture2D->setImage(pImage);
	pStateSet->setTextureAttributeAndModes(0,pTexture2D,osg::StateAttribute::ON);
	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);

	//////////////////////////////////////////////////////////////////////////
	geode->addDrawable(pScaleTxt);
	geode->addDrawable(pScaleGeom);
	osg::ref_ptr<ScaleCallBack> pSC = new ScaleCallBack;
	pSC->setText(pScaleTxt);

	m_pView->getCamera()->addUpdateCallback(pSC);
}
//extern short gValue1[];
//extern QColor gColorTable1[];

struct MyScalarPrinter: public osgSim::ScalarBar::ScalarPrinter
{
	std::string printScalar(float scalar)
	{
		//std::cout<<"In MyScalarPrinter::printScalar"<<std::endl;
		return ScalarPrinter::printScalar(scalar);
	}
};

void MainWindow::AddColorTable(bool vertical)
{
	osg::ref_ptr<osg::Geometry> pGeom  = new osg::Geometry;
	///////////////////////////////////////////////////////////////////////
	osg::ref_ptr<osg::Vec3Array> pVertexArray = new osg::Vec3Array;
	pVertexArray->push_back(osg::Vec3(90,0, 0));
	pVertexArray->push_back(osg::Vec3(90,253,0));
	pVertexArray->push_back(osg::Vec3(0,253,0));
	pVertexArray->push_back(osg::Vec3(0,0,0));
	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;
	texcoords->push_back(osg::Vec2(1.0f,0.0f));
	texcoords->push_back(osg::Vec2(1.0f,1.0f));
	texcoords->push_back(osg::Vec2(0.0f,1.0f));
	texcoords->push_back(osg::Vec2(0.0f,0.0f));
	osg::ref_ptr<osg::Vec4Array> pColorArray = new osg::Vec4Array;
	pColorArray->push_back(osg::Vec4(1.0f, 1,1,1.0f));
	pColorArray->push_back(osg::Vec4(1.0f, 1,1,1.0f));
	pColorArray->push_back(osg::Vec4(1.0f, 1,1,1.0f));
	pColorArray->push_back(osg::Vec4(1.0f, 1,1,1.0f));
	osg::ref_ptr<osg::Vec3Array> pNormalArray = new osg::Vec3Array;
	pNormalArray->push_back(osg::Vec3(0, -1, 0));
	pNormalArray->push_back(osg::Vec3(0, -1, 0));
	pNormalArray->push_back(osg::Vec3(0, -1, 0));
	pNormalArray->push_back(osg::Vec3(0, -1, 0));

	pGeom->setVertexArray(pVertexArray);
	pGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
	pGeom->setTexCoordArray(0, texcoords);
	pGeom->setColorArray(pColorArray);
	pGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::StateSet* pStateSet = pGeom->getOrCreateStateSet();
	///////////////////////////////////设置纹理///////////////////////////////////////
	osg::Texture2D* pTexture2D = new osg::Texture2D;
	pTexture2D->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
	pTexture2D->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
	pTexture2D->setDataVariance(osg::Object::DYNAMIC);
	osg::Image* pImage = osgDB::readImageFile("./icon/colorTable.png");
	pTexture2D->setImage(pImage);
	pStateSet->setTextureAttributeAndModes(0,pTexture2D,osg::StateAttribute::ON);
	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	
	//////////////////////////////////////////////////////////////////////////
	osg::ref_ptr<TransformCallBack> pTC = new TransformCallBack;
	pTC->setViewer(m_pView);

	osg::ref_ptr<osg::Geode> pGeode = new osg::Geode;
	pGeode->addDrawable(pGeom);
	
	osg::ref_ptr<osg::MatrixTransform> pCompassTransform = new osg::MatrixTransform;
	pCompassTransform->setUpdateCallback(pTC);
	pCompassTransform->addChild(pGeode);
	pCompassTransform->setMatrix(osg::Matrix::identity());
	pCompassTransform->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	osg::StateSet* pTransState = pCompassTransform->getOrCreateStateSet();
	pTransState->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
	pCompassTransform->setCullingActive(false);

	m_pHUDCamera->addChild(pCompassTransform);
}

void MainWindow::CreateRadarDisplayLayer()
{
	goto_gis::GeosOnTheOsg* pGOTO = goto_gis::GeosOnTheOsg::Instantiate();
	goto_gis::Layer* pLayer = new goto_gis::VectorLayer(0, 0, 1);
	QString strFileName("5.nc");
	
	pGOTO->OpenDataAsLayer(QStringToStdString(strFileName), pLayer);
	pLayer->CoordTrans(m_pMap->GetCoordinateTransform());
	pLayer->Visible(true);
	pLayer->LayerZ(10);

	pLayer->CreateLayerData();

	m_pMap->AddLayer("radar", pLayer);
	osg::Node* pNode = m_pMap->GetRootNode();

	m_pSetCubesInterface = (SetCubesInterface*)(pLayer->GetDataProvider()->GetExtendInterface(0));
	m_pControlorInterface = (ControlorInterface*)(pLayer->GetDataProvider()->GetExtendInterface(1));
}

void MainWindow::FlushList()
{
	//FlushCubesList();
	FlushFileList();
}

void MainWindow::GetTimeString(const QString& str1, QString& strTimeString)
{
	QString strBaseName = str1.left(15);
	strTimeString = strBaseName.remove("_");
}

void MainWindow::FillList()
{
	m_FileBaseMapToFilePath.clear();
	m_FileList.clear();
	//m_TrackFileList.clear();

	if(g_GlobleConfig.ModeType() == 0)
	{
		QString strPath = g_GlobleConfig.Path();
		QDir dir(strPath);
		QFileInfoList fileList = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot/*, QDir::Time*/);

		for(int i = 0; i < fileList.size(); i++)
		{
			if(fileList.at(i).isDir() || fileList.at(i).suffix() != "nc")
				continue;

			QString strFileName = fileList.at(i).absoluteFilePath();
			QFileInfo fileInfo(strFileName);
			QString strFilePathAndName = fileInfo.absoluteFilePath();
			QString strBaseName = fileInfo.baseName();
			
			GetTimeString(strBaseName, strBaseName);

			m_FileList.push_back(strBaseName);

			m_FileBaseMapToFilePath.insert(strBaseName
				, strFilePathAndName);
		}
		/*if(fileList.size() > 0)
		{
		QString strFileName = fileList.at(0).absoluteFilePath();
		QFileInfo fileInfo(strFileName);
		QString strFilePathAndName = fileInfo.absoluteFilePath();

		m_FileList.push_back(fileInfo.baseName());

		m_FileBaseMapToFilePath.insert(fileInfo.baseName()
		, strFilePathAndName);
		}*/
	}
	else
	{
		QString strFileName = g_GlobleConfig.HistoryPath() + "/" + g_GlobleConfig.StartTime() + "<->" + g_GlobleConfig.EndTime() + ".nc";
		if(strFileName.contains("<->"))
		{
			QFileInfo fileInfo(strFileName);
			QString FileName = fileInfo.baseName();
			QStringList Times = FileName.split("<->");

			QDateTime startTime = QDateTime::fromString(Times[0], "yyyy-MM-dd hh:mm:ss");
			QDateTime endTime = QDateTime::fromString(Times[1], "yyyy-MM-dd hh:mm:ss");
			QDir dir = fileInfo.absoluteDir();
			QFileInfoList infoList = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot/*, QDir::Time*/);
			//m_tniv.clear();
			for(int i = 0; i < infoList.size(); i++)
			{
				if(infoList.at(i).isDir() || infoList.at(i).suffix() != "nc")
					continue;

				QString strFilePath = infoList.at(i).fileName();
				QFileInfo info(strFilePath);
				QString strTemp = info.baseName();
				GetTimeString(strTemp, strTemp);
				//QDateTime currDateTime = QDateTime::fromString("20" + strTemp, "yyyyMMddhhmm");
				QDateTime currDateTime = QDateTime::fromString(strTemp, "yyyyMMddhhmmss");
				//加载雷达反射

				if(startTime <= currDateTime && currDateTime <= endTime)
				{
					QString strFilePathAndName = infoList.at(i).absoluteFilePath();
					QFileInfo fileInfo(strFilePathAndName);

					QString strBasename = fileInfo.baseName();

					GetTimeString(strBasename, strBasename);
					m_FileList.push_back(strBasename);

					m_FileBaseMapToFilePath.insert(strBasename
						, strFilePathAndName);

				}
			}
		}
	}
	//m_OldFileList = m_FileList;
}

int MainWindow::StartTrack2()
{
	/*if(m_CurrentFileIndex != 1)
	{
	QMessageBox::StandardButton but = QMessageBox::information(this, QString::fromLocal8Bit("跟踪记录")
	, QString::fromLocal8Bit("上一次跟踪未完成是否继续上一次的跟踪 ?"), QMessageBox::Yes, QMessageBox::No);

	if(but == QMessageBox::No)
	{
	m_CurrentFileIndex = 1;
	ClearTrackResult();
	}
	}*/
	m_CurrentRealFileIndex = 1;
	m_CurrentFileIndex = 1;
	if(m_CurrentFileIndex == 1) ClearTrackResult();

	for(int i = 0; i < m_pFileListItemModel->rowCount(); i++)
	{
		 QStandardItem* pItem = m_pFileListItemModel->item(i);
		 pItem->setBackground(QBrush(QColor(255, 255, 255)));
	}

	PlayStrack();
	m_pRestorePointerAction->MyAction();
	//SwitchDefalultMaipulator();
	QVector<cube_data> mycubes;
	//g_GlobleConfig.GetCubes(mycubes);
	QItemSelectionModel* pISM = m_TrackBoxDockList->m_Setup.listView->selectionModel();
	
	QModelIndexList selectBoxes = pISM->selectedRows();
	if(selectBoxes.size() <= 0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("跟踪框"), QString::fromLocal8Bit("请选择跟踪框。（可多选）"));
		return -1;
	}

	for(int i = 0; i < selectBoxes.size(); i++)
	{
		QStandardItem* item =  m_pTrackBoxItemModel->item(selectBoxes.at(i).row());
		cube_data cd = g_GlobleConfig.GetCubeFromName(item->text());

		if(m_iCurrentLayer >= 0 && m_iCurrentLayer < 20)
			cd.height = QString::number(g_LayerHeight[m_iCurrentLayer]);
		//else
			//cd.height = QString::number(-1);

		mycubes.push_back(cd);
	}
	//////////////////////////////////////////////////////////////////////////
	/*QStringList selectList;
	QItemSelectionModel* pSelectFileModel = m_TrackBoxDockList->m_Setup.listView->selectionModel();
	QModelIndexList selectFileIndexes = pISM->selectedRows();
	if(selectFileIndexes.size() <= 0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("跟踪框"), QString::fromLocal8Bit("请选择跟踪框。（可多选）"));
		return;
	}

	for(int i = 0; i < selectFileIndexes.size(); i++)
	{
		QStandardItem* item =  m_pTrackBoxItemModel->item(selectFileIndexes.at(i).row());
		selectList.push_back(item->text());
	}*/


	/*for(int i = 0; i < mycubes.size(); i++)
	{
		cube_data mycd = mycubes.at(i);
	}*/
	//m_MySloveThread.cubes = mycubes;
	m_MySloveThread.setCubes(mycubes);
	//m_MySloveThread.m_iMode = g_GlobleConfig.ModeType();
	m_MySloveThread.setMode(g_GlobleConfig.ModeType());
	m_MySloveThread.start();

	return 1;
#if 0
	for(int i = 0; i < m_FileList.size(); i++)
	{
		if(i == 0)
		{
			continue;
		}

		QString strFile1 = m_FileList.at(i - 1);
		QString strFile2 = m_FileList.at(i);
		QString strFilePath1 = m_FileBaseMapToFilePath.value(strFile1);
		QString strFilePath2 = m_FileBaseMapToFilePath.value(strFile2);

		for(int ii = 0; ii < mycubes.size(); ii++)
		{
			CUBE_DATE cd;
			cube_data mycd = mycubes.at(ii);

			cd.fTargetFstLon = mycd.left_top_lon;
			cd.fTargetFstLat = mycd.right_bottom_lat;
			cd.fTargetLstLon = mycd.right_bottom_lon;
			cd.fTargetLstLat = mycd.left_top_lat;
			cd.iHeightDown = mycd.height_down.toInt();
			cd.iHeightUp = mycd.height_up.toInt();
			cd.iHeight = mycd.height.toInt();
			cd.refs.push_back(20);
			cd.refs.push_back(30);
			cd.refs.push_back(40);
			cd.refs.push_back(50);

			P_TREC_NC_MAP_OUT pOut = /*m_pControlorInterface->runTrec*/Test(strFilePath1.toStdString()
				, strFilePath2.toStdString(), cd);

			/////////////////////////////////handle reslut/////////////////////////////////////////
			osg::Vec3 p1, p2, p3, p4;
			float fTargetFstLon = mycd.left_top_lon;
			float fTargetFstLat = mycd.right_bottom_lat;
			float fTargetLstLon = mycd.right_bottom_lon;
			float fTargetLstLat = mycd.left_top_lat;

			p1.set(fTargetFstLon, fTargetLstLat, 0);
			p2.set(fTargetLstLon, fTargetLstLat, 0);
			p3.set(fTargetLstLon, fTargetFstLat, 0);
			p4.set(fTargetFstLon, fTargetFstLat, 0);

			m_pControlorInterface->AddRect(mycd.name.toStdString(), p1, p2, p3, p4, osg::Vec4(1.0, 0.0, 0.0, 1.0));

			QMap<QString, QVector<sc>>::iterator finded1 = m_Result1.find(mycd.name);

			sc sc_value;
			sc_value.p = pOut->begin()->second.precipitation.precipitation;
			sc_value.c = pOut->begin()->second.precipitation.content;

			if(finded1 != m_Result1.end())
			{
				finded1.value().push_back(sc_value);
			}
			else
			{
				QVector<sc> rvector;
				rvector.push_back(sc_value);
				m_Result1.insert(mycd.name, rvector);
			}

			
			QMap<QString, QMap<int, QVector<ri>>>::Iterator finded2 = m_Result2.find(mycd.name);

			

			//先查跟踪框名，再差对应的反射值。

				
			if(finded2 != m_Result2.end())
			{
				for(size_t iii = 0; iii < pOut->begin()->second.ratio.interval.size(); iii++)
				{
					QPolygonF plotData3;

					float fRatio = pOut->begin()->second.ratio.ratio.at(iii);
					int iV = pOut->begin()->second.ratio.interval.at(iii);
					int iNum = pOut->begin()->second.ratio.number.at(iii);

					ri ri_value;
					ri_value.in = iNum;
					ri_value.ir = fRatio;

					QMap<int, QVector<ri>>::iterator findedValue 
						= finded2.value().find(iV);

					if(findedValue != finded2.value().end())
					{
						findedValue.value().push_back(ri_value);
					}
					else
					{
						QVector<ri> riVector;
						riVector.push_back(ri_value);
						finded2->insert(iV, riVector);
					}
				}
					
			}
			else
			{
				QMap<int, QVector<ri>> mapValue;
				for(size_t iii = 0; iii < pOut->begin()->second.ratio.interval.size(); iii++)
				{
					QPolygonF plotData3;

					float fRatio = pOut->begin()->second.ratio.ratio.at(iii);
					int iV = pOut->begin()->second.ratio.interval.at(iii);
					int iNum = pOut->begin()->second.ratio.number.at(iii);

					ri ri_value;
					ri_value.in = iNum;
					ri_value.ir = fRatio;
					QVector<ri> riVector;
					riVector.push_back(ri_value);
					mapValue.insert(iV, riVector);
				}
					
				m_Result2.insert(mycd.name, mapValue);
			}
				
			//////////////////////////////////handle reslut////////////////////////////////////////
			m_pControlorInterface->OpenNC(strFilePath2.toStdString());
			DisplayPlot();
		}

		m_pControlorInterface->DeleteNcFile(strFilePath1.toStdString());
	}
#endif
}

void MainWindow::StartReplay()
{/*
	if(m_CurrentReplayFileIndex != 1)
	{
		QMessageBox::StandardButton but = QMessageBox::information(this, QString::fromLocal8Bit("跟踪记录")
			, QString::fromLocal8Bit("上一次动画播放未完成是否继续上一次的播放 ?"), QMessageBox::Yes, QMessageBox::No);

		if(but == QMessageBox::No)
		{
			m_CurrentReplayFileIndex = 1;
		}
	}*/
	m_CurrentReplayFileIndex = 1;
	QVector<cube_data> mycubes;
	//g_GlobleConfig.GetCubes(mycubes);
	QItemSelectionModel* pISM = m_TrackBoxDockList->m_Setup.listView->selectionModel();
	QModelIndexList selectBoxes = pISM->selectedRows();
	
	for(int i = 0; i < selectBoxes.size(); i++)
	{
		QStandardItem* item =  m_pTrackBoxItemModel->item(selectBoxes.at(i).row());
		cube_data cd = g_GlobleConfig.GetCubeFromName(item->text());
		mycubes.push_back(cd);
	}

	//m_MySloveThread.cubes = mycubes;
	m_MySloveThread.setCubes(mycubes);
	//m_MySloveThread.m_iMode = g_GlobleConfig.ModeType();
	m_MySloveThread.setMode(g_GlobleConfig.ModeType());

	m_MySloveThread.start();

	//m_pStartTrackAction->setEnabled(false);
	//m_DirectStartTrackAction->setEnabled(false);
}

void MainWindow::StartTrack()
{
	m_Result1.clear();
	m_Result2.clear();
	/************************************************************************/
	/* 填充文件列表                                                           */
	/************************************************************************/
	FillList();
	FlushList();
	/************************************************************************/
	/*开始跟踪                                                               */
	/************************************************************************/

	if(m_FileList.size() <= 0) return;

	QString strFile = m_FileList.at(0);
	QString strFilePath = m_FileBaseMapToFilePath.value(strFile);
	m_OpenFileName = strFilePath;
	m_pDrawClipLine->SetFileName(strFilePath);
	//m_pControlorInterface->OpenNC(strFilePath.toStdString());
	m_pControlorInterface->LoadNcFile(QStringToStdString(strFilePath));
	m_pControlorInterface->DisplayNcFile(QStringToStdString(strFilePath));

	SetTimeText(strFile);
	//SwitchDrawRectMaipulator();
	/*QMessageBox::information(this, QString::fromLocal8Bit("绘制跟踪框")
		, QString::fromLocal8Bit("请在地图窗口中绘制跟踪框。"));*/
}

void MainWindow::DisplayPlot()
{
	static int iPreValue = -100;
	QMap<QString, QVector<sc>>::iterator i1 = m_Result1.begin();
	for(i1; i1 != m_Result1.end(); i1++)
	{
		QString strName = i1.key();
		QMap<int, QString> dataTimeList;
		
		QVector<sc>* pResult = &(i1.value());
		QPolygonF plotData1, plotData2, plotData3, plotData4, plotData5;;

		for(int ii = 0; ii < pResult->size(); ii++)
		{
			int iX = (ii)*10;
			//QString strK = m_FileList[ii + 1];
			//dataTimeList.insert(iX, strK);
			sc pOut = pResult->at(ii);

			plotData1.push_back(QPointF(iX, pOut.c));
			plotData2.push_back(QPointF(iX, pOut.p));
			plotData3.push_back(QPointF(iX, pOut.h));
			plotData4.push_back(QPointF(iX, pOut.v));
			plotData5.push_back(QPointF(iX, pOut.m));
		}

		//////////////////////////////////绘制折线图////////////////////////////////////////
		QMap<QString, QwtPlotDialogImps*>::iterator fined = m_QwtPlotDialogMap.find(strName);
		QwtPlotDialogImps* pPlotDialog = 0;
		if(fined == m_QwtPlotDialogMap.end())
		{
			pPlotDialog = new QwtPlotDialogImps;
			pPlotDialog->SetTitle(strName);
			/*pPlotDialog->SetScaleDraw(0, &m_aixsX);
			pPlotDialog->SetScaleDraw(1, &m_aixsX);
			pPlotDialog->SetScaleDraw(2, &m_aixsX);*/
			m_QwtPlotDialogMap.insert(strName, pPlotDialog);

			pPlotDialog->InitAll();
		}
		else
		{
			QString strKey = fined.key();
			pPlotDialog = fined.value();
		}

		pPlotDialog->DrawPlot(0, "plotData1", plotData1, 0, QColor(123,23,90)
			, QString::fromLocal8Bit("降水量"));
		pPlotDialog->DrawPlot(2, "plotData2", plotData2, 0, QColor(23,123,0)
			, QString::fromLocal8Bit("液态水含量"));

		pPlotDialog->DrawPlot(3, "plotData3", plotData3, 0, QColor(121, 26, 31)
			, QString::fromLocal8Bit("回波顶高"));
		pPlotDialog->DrawPlot(4, "plotData4", plotData4, 0, QColor(101, 121, 0)
			, QString::fromLocal8Bit("回波体积"));
		pPlotDialog->DrawPlot(5, "plotData5", plotData5, 0, QColor(38, 230, 111)
			, QString::fromLocal8Bit("最大反射率"));

		pPlotDialog->SetScaleDraw(0, &m_aixsX);
		pPlotDialog->SetScaleDraw(2, &m_aixsX);
		pPlotDialog->SetScaleDraw(3, &m_aixsX);
		pPlotDialog->SetScaleDraw(4, &m_aixsX);
		pPlotDialog->SetScaleDraw(5, &m_aixsX);
		///////////////////////////////////绘制折线图///////////////////////////////////////
	}

	QMap<QString, QMap<int, QVector<ri>>>::Iterator i2 = m_Result2.begin();

	for(i2; i2 != m_Result2.end(); i2++)
	{
		QString strName = i2.key();

		QMap<int, QVector<ri>>* pV = &(i2.value());

		QMap<int, QVector<ri>>::iterator i22 = pV->begin();

		QVector<QPolygonF> plotDataVector;
		for(i22; i22 != pV->end(); i22++)
		{
			QPolygonF plotData3;
			QVector<float> otherValue;
			int iKey = i22.key();
			QVector<ri>* pri = &(i22.value());

			QMap<int, QString> dataTimeList;

			for(int iiii = 0; iiii < pri->size(); iiii++)
			{
				plotData3.push_back(QPointF(iiii * 10, pri->at(iiii).in));
				otherValue.push_back(pri->at(iiii).ir);
				//dataTimeList.insert(iiii * 10, m_FileList[iiii + 1]);
			}
			//////////////////////////////////////////////////////////////////////////

			//DistroScaleDraw* dsd = new DistroScaleDraw(Qt::Vertical, dataTimeList);
			QMap<QString, QwtPlotDialogImps*>::Iterator fined = m_QwtPlotDialogMap.find(strName);
			QwtPlotDialogImps* pPlotDialog = 0;
			if(fined == m_QwtPlotDialogMap.end())
			{
				pPlotDialog = new QwtPlotDialogImps;
				m_QwtPlotDialogMap.insert(strName, pPlotDialog);
				/*pPlotDialog->SetScaleDraw(0, &m_aixsX);
				pPlotDialog->SetScaleDraw(1, &m_aixsX);
				pPlotDialog->SetScaleDraw(2, &m_aixsX);*/
				pPlotDialog->InitAll();
			}
			else
			{
				pPlotDialog = fined.value();
			}

			int iIndex = m_pControlorInterface->GetIndexFromValue(iKey);
			QColor colr(0,0,0);
			if(iIndex != 0)
			{
				colr = m_pControlorInterface->GetColor(iIndex - 1);
			}
			

			QString strTitle;

			if(i22 == pV->begin())
			{
				strTitle = "< " + QString("%1").arg(iKey);
				iPreValue = iKey;
			}
			else
			{
				strTitle = QString("%1 - %2").arg(iPreValue).arg(iKey);
				iPreValue = iKey;
			}
			pPlotDialog->DrawPlot(1, strTitle, plotData3, 0
				, colr, strTitle, &otherValue);
			
			pPlotDialog->SetScaleDraw(1, &m_aixsX);

			//////////////////////////////////////////////////////////////////////////
		}
	}
	//////////////////////////////////////添加跟踪狂截图////////////////////////////////////
	m_ClipNcResultMutex.lock();
	QMap<QString, QMap<QString, QImage*>>::iterator be = m_ClipNcImageResult.begin();
	for(be; be != m_ClipNcImageResult.end(); be++)
	{
		QMap<QString, QwtPlotDialogImps*>::Iterator fined = m_QwtPlotDialogMap.find(be.key());
		QwtPlotDialogImps* pPlotDialog = 0;
		if(fined == m_QwtPlotDialogMap.end())
		{
			pPlotDialog = new QwtPlotDialogImps;
			pPlotDialog->InitAll();
		}
		else
		{
			pPlotDialog = fined.value();
		}

		QMap<QString, QImage*>::iterator bebe = be.value().begin();

		for(bebe; bebe != be.value().end(); bebe++)
		{
			pPlotDialog->AddClipImage(bebe.value(), bebe.key());

			delete bebe.value();
		}
	}
	m_ClipNcImageResult.clear();
	/////////////////////////////////////添加剖面图/////////////////////////////////////
	/*QMap<QString, QMap<QString, QImage*>>::iterator */be = m_SectionImageResult.begin();
	for(be; be != m_SectionImageResult.end(); be++)
	{
		QMap<QString, QwtPlotDialogImps*>::Iterator fined = m_QwtPlotDialogMap.find(be.key());
		QwtPlotDialogImps* pPlotDialog = 0;
		if(fined == m_QwtPlotDialogMap.end())
		{
			pPlotDialog = new QwtPlotDialogImps;
			pPlotDialog->InitAll();
		}
		else
		{
			pPlotDialog = fined.value();
		}

		QMap<QString, QImage*>::iterator bebe = be.value().begin();

		for(bebe; bebe != be.value().end(); bebe++)
		{
			pPlotDialog->AddSectionImage(bebe.value(), bebe.key());

			delete bebe.value();
		}
	}
	m_SectionImageResult.clear();
	m_ClipNcResultMutex.unlock();
	//////////////////////////////////////////////////////////////////////////
	QList<QString> keys = m_QwtPlotDialogMap.keys();
	for(int i = 0; i < keys.size(); i++)
	{
		QwtPlotDialogImps* pQwtPlotDialog = m_QwtPlotDialogMap.value(keys[i]);

		bool bV = pQwtPlotDialog->IsVisible();
		if(!bV)
			pQwtPlotDialog->Show(keys[i]);
	}
}

P_TREC_NC_MAP_OUT MainWindow::Test( std::string& file1, std::string& file2, CUBE_DATE& cd )
{
	Sleep(1500);
	static float fzz = 0.01;
	float firstLat(cd.fTargetFstLat), firstLon(cd.fTargetFstLon)
		, lastLat(cd.fTargetLstLat), lastLon(cd.fTargetLstLon);

	static float v1 = 1.10;
	static float v2 = 2.5;

	static float p1 = 0.33;
	//static float p2 = 0;
	//static float p3 = 0;
	//static float p4 = 0;
	static int p2 = 234;

	TREC_NC_MAP_OUT* rr = new TREC_NC_MAP_OUT;
	TREC_OUT ooo;

	ooo.precipitation.content = v1;
	ooo.precipitation.precipitation = v2;

	for(size_t i = 0; i < cd.refs.size(); i++)
	{
		//srand(100);

		ooo.ratio.interval.push_back(cd.refs.at(i));
		ooo.ratio.ratio.push_back(rand() % 50);
		ooo.ratio.number.push_back((rand() + 20) % 100);
	}

	v1 += 0.5;
	v2 += 0.8;
	p1 += 0.3;
	p2 += 13;

	firstLat += fzz;
	firstLon += fzz;
	lastLat += fzz;
	lastLon += fzz;

	ooo.speed.Elat = lastLat;//
	ooo.speed.Elon = lastLon;
	ooo.speed.Nlat = firstLat;
	ooo.speed.NLon = firstLon;

	(*rr)[0] = ooo;
	fzz += 0.05;
	return rr;
}

void MainWindow::DisplayRadarData(const QString& strFile, const QIcon* pIcon)
{
	QFileInfo fileInfo(strFile);
	QString strBaseName = fileInfo.baseName();
	GetTimeString(strBaseName, strBaseName);

	QList<QStandardItem*> pItems = m_pFileListItemModel->findItems(strBaseName);
	if(pItems.size() != 0)
	{
		if(pIcon)
			pItems[0]->setIcon(*pIcon);

		m_FileNameDockList->m_Setup.listView->setCurrentIndex(pItems[0]->index());
		//pItems[0]->setBackground(QBrush(QColor(0, 255, 0)));
		//m_pFileListItemModel->curr
	}
	SetTimeText(strBaseName);
	//QDateTime currDateTime = QDateTime::fromString("20" + strBaseName, "yyyyMMddhhmm");
	//currDateTime.setTimeSpec(Qt::UTC);

	//QDateTime bjtDateTime = currDateTime.addSecs(8 * 3600);//.toTimeSpec(Qt::LocalTime);

	//pHgtTxt->setText(bjtDateTime.toString("yyyy-MM-dd hh:mm:ss (BJT)").toAscii().data());

	m_pControlorInterface->DisplayNcFile(QStringToStdString(strFile));
}

void MainWindow::slot_TrackDone(const QString& doneFile)
{
	//QIcon icon("./icon/foot.ico");
	DisplayRadarData(doneFile);

	QFileInfo fileInfo(doneFile);
	QString strBaseName = fileInfo.baseName();
	GetTimeString(strBaseName, strBaseName);
	QList<QStandardItem*> pItems = m_pFileListItemModel->findItems(strBaseName);
	if(pItems.size() != 0)
	{
		pItems[0]->setBackground(QBrush(QColor(0, 255, 0)));
		//m_pFileListItemModel->curr
	}
	/*for(QSet<ResultRect>::iterator i = m_RectResultSet.begin()
		; i != m_RectResultSet.end(); i++)*/
	m_ResultMutex.lock();
	int iRectCount = m_RectResultSet.size();
	for(int i = 0; i < iRectCount; i++)
	{
		m_pControlorInterface->AddRect(QStringToStdString(m_RectResultSet.at(i).name)
			, (osg::Vec3)m_RectResultSet.at(i).p1
			, (osg::Vec3)m_RectResultSet.at(i).p2
			, (osg::Vec3)m_RectResultSet.at(i).p3
			, (osg::Vec3)m_RectResultSet.at(i).p4
			, (osg::Vec4)m_RectResultSet.at(i).color, QStringToStdString(strBaseName));
	}
	
	
	m_RectResultSet.clear();
	DisplayPlot();

	m_ResultMutex.unlock();

	if(m_bAutoSaveResult == true)
	{
		ChangeProcess(1);
	}
}

void MainWindow::ChangeRectColor(const QString& strBaseName )
{
	QItemSelectionModel* pISM = m_TrackBoxDockList->m_Setup.listView->selectionModel();
	QModelIndexList selectBoxes = pISM->selectedRows();
	for(int i = 0; i < selectBoxes.size(); i++)
	{
		QStandardItem* item =  m_pTrackBoxItemModel->item(selectBoxes.at(i).row());
		cube_data cd = g_GlobleConfig.GetCubeFromName(item->text());
		;
		int iIndex = m_SelectFilesList.indexOf(strBaseName);
		if(iIndex >= 0)
		{
			m_pControlorInterface->SetRectColor(QStringToStdString(cd.name), iIndex, osg::Vec4(0, 0, 0, 1));
		}
	}
	
}

void MainWindow::slot_Replay( const QString& doneFile )
{
	DisplayRadarData(doneFile);
#if 1
	m_ResultMutex.lock();
	int iRectCount = m_RectResultSet.size();
	//qDebug()<<doneFile<<iRectCount;
	for(int i = 0; i < iRectCount; i++)
	{
		QFileInfo fileInfo(doneFile);
		QString strBaseName = fileInfo.baseName();
		GetTimeString(strBaseName, strBaseName);

		int iIndex = m_SelectFilesList.indexOf(strBaseName);
		if(iIndex > 0)
		{

			std::string strName = QStringToStdString(m_RectResultSet.at(i).name);
			m_pControlorInterface->SetRectColor(strName, iIndex, osg::Vec4(0, 0, 0, 1));
		}
	}

	m_RectResultSet.clear();
	m_ResultMutex.unlock();
#endif
}

void MainWindow::slot_ToDeleteOld( const QString& preFile )
{
	m_pControlorInterface->DeleteNcFile(QStringToStdString(preFile));
}

int MainWindow::GetTrackFileID( QPair<QString, QString>& filePair, int iType )
{
	QStringList* pST = 0;
	int* pCurrentIndex = 0;
	switch(iType)
	{
	case 0:
		pST = &m_SelectFilesList;
		pCurrentIndex = &m_CurrentFileIndex;
		break;
	case 1:
		pST = &m_SelectReplayFilesList;
		pCurrentIndex = &m_CurrentReplayFileIndex;
		break;
	default:
		break;
	}

	//m_FileListMutex.lock();
	if(/*m_CurrentFileIndex*/ *pCurrentIndex < /*m_FileList*/pST->size())
	{
		filePair.first = /*m_FileList*/pST->at(*pCurrentIndex - 1);
		filePair.second = /*m_FileList*/pST->at(*pCurrentIndex);
		//m_FileListMutex.unlock();
		(*pCurrentIndex)++;
		return 1;
	}
	else
	{
		*pCurrentIndex = 1;
		//m_FileListMutex.unlock();
		return 0;
	}
}

QString& MainWindow::GetTrackFilePath( const QString& strID )
{
	static QString strVV = "";
	m_FilePathMutex.lock();
	strVV = m_FileBaseMapToFilePath.value(strID);
	m_FilePathMutex.unlock();
	return strVV;
}

int MainWindow::GetRealTrackFileID( QPair<QString, QString>& filePair, int iType )
{
	int* pCurrentIndex = 0;
	QStringList* pLocalSelectListFiles = 0;
	if(iType == 0)
	{
		pLocalSelectListFiles = &m_SelectFilesList;
		pCurrentIndex = &m_CurrentRealFileIndex;
	}
	else if(iType == 1)
	{
		pLocalSelectListFiles = &m_SelectReplayFilesList;
		pCurrentIndex = &m_CurrentReplayFileIndex;
	}
	else
	{
		return -2;
	}

	if(*pCurrentIndex < pLocalSelectListFiles->size())
	{
		filePair.first = pLocalSelectListFiles->at(*pCurrentIndex - 1);
		filePair.second = pLocalSelectListFiles->at(*pCurrentIndex);
		(*pCurrentIndex)++;
		return 0;
	}
	else
	{
		if(pLocalSelectListFiles->size() <= 0) return -1;
		static QString curFileBaseName = pLocalSelectListFiles->at(pLocalSelectListFiles->size() - 1);
		//static QDateTime curTime = QDateTime::fromString("20" + curFileBaseName, "yyyyMMddhhmm");
		static QDateTime curTime = QDateTime::fromString(curFileBaseName, "yyyyMMddhhmmss");
		QString strFilePath = g_GlobleConfig.Path();

		QDir dir(strFilePath);
		QFileInfoList fileList = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::Name | QDir::Reversed);

		for(int i = 0; i < fileList.size(); i++)
		{
			if(fileList.at(i).isDir() || fileList.at(i).suffix() != "nc")
				continue;
			QString strFileName = fileList.at(i).absoluteFilePath();
			QFileInfo fileInfo(strFileName);
			QString strBaseName = fileInfo.baseName();
			GetTimeString(strBaseName, strBaseName);
			//QDateTime newDateTime = QDateTime::fromString("20" + strBaseName, "yyyyMMddhhmm");
			QDateTime newDateTime = QDateTime::fromString(strBaseName, "yyyyMMddhhmmss");
			if(!newDateTime.isValid()) continue;
			if(newDateTime > curTime)
			{
				m_FileList.push_back(strBaseName);

				emit signal_InsertFile(strBaseName);
				QString strFilePathAndName = fileInfo.absoluteFilePath();
				strBaseName = fileInfo.baseName();
				GetTimeString(strBaseName, strBaseName);

				m_FileBaseMapToFilePath.insert(strBaseName
					, strFilePathAndName);

				filePair.first = curFileBaseName;
				filePair.second = strBaseName;

				curTime = newDateTime;
				curFileBaseName = strBaseName;

				(*pCurrentIndex)++;
				return 1;
			}
			return -1;
		}

		return -1;
	}
}

QString& MainWindow::GetRealTrackFilePath( const QString& strID )
{
	static QString temp;
	return temp;
}

void MainWindow::slot_InsertFile( const QString& fileBase )
{
	//FlushFileList();
	m_SelectFilesList.push_back(fileBase);
	QStandardItem* ci = new QStandardItem(fileBase);
	ci->setEditable(false);
	m_pFileListItemModel->appendRow(ci);
}

void MainWindow::FlushCubesList()
{
	m_pTrackBoxItemModel->clear();
	m_pControlorInterface->RemoveAllRects();
	QVector<cube_data> myCubeData;
	//std::vector<CUBE_DATE> cubes;
	g_GlobleConfig.GetCubes(myCubeData);

	for(int i = 0; i < myCubeData.size(); i++)
	{
		cube_data* pCD = &(myCubeData[i]);
		QString strName = pCD->name;
		QStandardItem* ci = new QStandardItem(QIcon("./icon/cube.png"), strName);
		ci->setEditable(false);
		m_pTrackBoxItemModel->appendRow(ci);

		osg::Vec3 p1, p2, p3, p4;
		float fTargetFstLon = pCD->left_top_lon;
		float fTargetFstLat = pCD->right_bottom_lat;
		float fTargetLstLon = pCD->right_bottom_lon;
		float fTargetLstLat = pCD->left_top_lat;

		p1.set(fTargetFstLon, fTargetLstLat, 0);
		p2.set(fTargetLstLon, fTargetLstLat, 0);
		p3.set(fTargetLstLon, fTargetFstLat, 0);
		p4.set(fTargetFstLon, fTargetFstLat, 0);

		m_pControlorInterface->AddRect(QStringToStdString(strName), p1, p2, p3, p4, osg::Vec4(.0, 0.0, 0.0, 1.0));
		m_pControlorInterface->HideRects(QStringToStdString(strName), true);
	}
}

void MainWindow::FlushFileList()
{
	m_pFileListItemModel->clear();
	//m_aixsX.clear();
	for(int i = 0; i < m_FileList.size(); i++)
	{
		QString strBaseName = m_FileList.at(i);
		QStandardItem* ci = new QStandardItem(strBaseName);
		ci->setEditable(false);
		m_pFileListItemModel->appendRow(ci);
		//m_aixsX.insert(i * 10, strBaseName);
	}
	//m_FileNameDockList->m_Setup.listView->setCurrentIndex(QMode)
	FlushPlotAixsMark();
}

void MainWindow::slot_SelectLayer( int index )
{
	if(index > 19)
	{
		m_iCurrentLayer = -1;
		m_pControlorInterface->SeleteLayer(-1);
	}
	else
	{
		m_pControlorInterface->SeleteLayer(index);
		m_iCurrentLayer = index;
	}

	m_pControlorInterface->LoadNcFile(QStringToStdString(m_OpenFileName));
	m_pControlorInterface->DisplayNcFile(QStringToStdString(m_OpenFileName));
}

void MainWindow::closeEvent( QCloseEvent* event )
{
	QList<QString> keys = m_QwtPlotDialogMap.keys();

	for(int i = 0; i < keys.size(); i++)
	{
		QwtPlotDialogImps* pD = m_QwtPlotDialogMap.value(keys[i]);
		pD->CloseAll();
		delete pD;
		//pD->->close();
	}
}

void MainWindow::slot_NameListClicked(const QModelIndex& index)
{
	QItemSelectionModel* pISM = m_FileNameDockList->m_Setup.listView->selectionModel();

	QModelIndexList mil = pISM->selectedRows();

	if(mil.size() >= 0)
	{
		QModelIndex first = mil.at(0);
		//QModelIndex tail = mil.at(mil.size() - 1);

		QStandardItem* pItem1 = m_pFileListItemModel->item(first.row());
		//QStandardItem* pItem2 = m_pFileListItemModel->item(tail.row());

		QStringList selectPairNames;

		selectPairNames.push_back(pItem1->text());
		//selectPairNames.push_back(pItem2->text());
		m_RasterNeedFiles.clear();
		m_RasterNeedFiles.push_back(pItem1->text());

		emit signal_SelectFiles(selectPairNames);
	}
	
	
}

void MainWindow::slot_doubleClicked( const QModelIndex& index )
{
	QStandardItem* pItem = m_pFileListItemModel->item(index.row());
	if(pItem)
	{
		QString strFilePath = m_FileBaseMapToFilePath.value(pItem->text());

		SetTimeText(pItem->text());
		/*QDateTime currDateTime = QDateTime::fromString("20" + pItem->text(), "yyyyMMddhhmm");
		currDateTime.setTimeSpec(Qt::UTC);

		QDateTime bjtDateTime = currDateTime.addSecs(8 * 3600);

		pHgtTxt->setText(bjtDateTime.toString("yyyy-MM-dd hh:mm:ss (BJT)").toAscii().data());*/


		if(strFilePath.size() != 0)
		{
			m_OpenFileName = strFilePath;
			m_pDrawClipLine->SetFileName(strFilePath);
			std::string strTemp = QStringToStdString(strFilePath);
			
			m_pControlorInterface->LoadNcFile(QStringToStdString(strFilePath));
			m_pControlorInterface->DisplayNcFile(QStringToStdString(strFilePath));
			
		}

		ChangeRectColor(pItem->text());
	}
}

void MainWindow::ClearTrackResult()
{
	QList<QString> keys = m_QwtPlotDialogMap.keys();

	for(int i = 0; i < keys.size(); i++)
	{
		QwtPlotDialogImps* pD = m_QwtPlotDialogMap.value(keys[i]);
		pD->CloseAll();
		delete pD;
	}

	m_QwtPlotDialogMap.clear();

	//FlushCubesList();
	m_pControlorInterface->RemoveTrackBox();
	m_Result1.clear();
	m_Result2.clear();
}

void MainWindow::slot_TrackListClicked( const QModelIndex& index )
{
	QItemSelectionModel* pISM = m_TrackBoxDockList->m_Setup.listView->selectionModel();

	QModelIndexList mil = pISM->selectedRows();

	int iI = mil.indexOf(index);
	QStandardItem* pSI = m_pTrackBoxItemModel->item(index.row());
	

	if(iI == -1)
	{
		m_pControlorInterface->HideRects(pSI->text().toStdString(), true);
	}
	else
	{
		m_pControlorInterface->HideRects(pSI->text().toStdString(), false);
		m_pDrawClipLine->SetCurrentRectName(pSI->text());
	}
}

void MainWindow::TrackBoxSelectState()
{

}

void MainWindow::slot_StopTrack()
{
	//m_pStartTrackAction->setEnabled(true);
	//m_DirectStartTrackAction->setEnabled(true);
	EnabledTrack(true);
	EnabledReplay(true);
	m_DirectStartTrackAction->setEnabled(true);
	m_pPlayHistoryTrackAction->setEnabled(true);

	if(m_bAutoSaveResult)
	{
		//save result
		QString strPath = g_GlobleConfig.TrackResultSavePath();
		QList<QString> keys = m_QwtPlotDialogMap.keys();
		for(int i = 0; i < keys.size(); i++)
		{
			QwtPlotDialogImps* pQwtPlotDialog = m_QwtPlotDialogMap.value(keys[i]);
			pQwtPlotDialog->SaveReuslt(strPath + "\\" + keys[i] + "\\");
		}
		ChangeProcess(1);
		m_bAutoSaveResult = true;

		if(m_pProgressDlg)
		{
			m_pProgressDlg->cancel();
			delete m_pProgressDlg;
			m_pProgressDlg = nullptr;
		}
	}
}

void MainWindow::FillSelectList(int iType)
{
	QStringList* pST = 0;
	switch(iType)
	{
	case 0:
		pST = &m_SelectFilesList;
		break;
	case 1:
		pST = &m_SelectReplayFilesList;
		break;
	default:
		break;
	}
	
	pST->clear();
	//QStringList selectList;
	QItemSelectionModel* pSelectFileModel = m_FileNameDockList->m_Setup.listView->selectionModel();
	QModelIndexList selectFileIndexes = pSelectFileModel->selectedRows();
	int iRowCount = 0;
	int iRowStartIndex = -1;

	if(selectFileIndexes.size() <= 0)
	{
		iRowCount = m_pFileListItemModel->rowCount();
		iRowStartIndex = 0;
		//return;
	}
	else if(selectFileIndexes.size() == 1)
	{
		int ic = m_pFileListItemModel->rowCount();
		int ir = selectFileIndexes.at(0).row();
		iRowCount = (ic - 1
			- ir) + 1;

		iRowStartIndex = selectFileIndexes.at(0).row();
	}
	else
	{
		iRowCount = selectFileIndexes.at(selectFileIndexes.size() - 1).row() - selectFileIndexes.at(0).row() + 1;
		iRowStartIndex = selectFileIndexes.at(0).row();
	}

	for(int i = 0; i < iRowCount; i++)
	{
		QStandardItem* item =  m_pFileListItemModel->item(iRowStartIndex + i);
		pST->push_back(item->text());
	}

	FlushPlotAixsMark();
}

void MainWindow::FlushPlotAixsMark()
{
	m_aixsX.clear();
	for(int i = 0; i < m_SelectFilesList/*m_FileList*/.size(); i++)
	{
		QString strBaseName = m_SelectFilesList/*m_FileList*/.at(i);
		m_aixsX.insert(i * 10, strBaseName);
	}
}

void MainWindow::EnabledTrack( bool bd )
{
	m_DirectStartTrackAction->setEnabled(bd);
	m_pStartTrackAction->setEnabled(bd);
	m_pLoadHidtoryAtion->setEnabled(bd);
}

void MainWindow::EnabledReplay( bool br )
{
	m_pPlayHistoryTrackAction->setEnabled(br);
	m_pLoadHidtoryAtion->setEnabled(br);
	m_pTrackBoxSettingAction->setEnabled(br);
	m_pStartTrackAction->setEnabled(br);
	//m_pPrevAction->setEnabled(br);
	//m_pNextAction->setEnabled(br);
}

void MainWindow::DefaultTrackStart()
{
	m_pStartTrackAction->MyAction();
}

#include "CubeSettingsDialogImps.h"

void MainWindow::slot_DeleteBox(bool)
{
	QItemSelectionModel* pISM = m_TrackBoxDockList->m_Setup.listView->selectionModel();
	QModelIndexList selectBoxes = pISM->selectedRows();
	if(selectBoxes.size() <= 0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("跟踪框")
			, QString::fromLocal8Bit("请选择要删除的跟踪框。（可多选）"));
		return;
	}

	for(int i = 0; i < selectBoxes.size(); i++)
	{
		QStandardItem* item =  m_pTrackBoxItemModel->item(selectBoxes.at(i).row());
		g_GlobleConfig.ConfigCubes(2, item->text(), 0);
	}
	
	FlushCubesList();
	g_GlobleConfig.Write();
}

void MainWindow::slot_AddBox(bool)
{
	CubeSettingsDialogImps d;
	QString strName = GetSelectFileName();
	if(strName.size() == 0)
	{
		strName = "NewTrackBox";
	}
	d.SetCubeName(strName);
	d.SetMode(0);

	if(d.exec() == QDialog::Accepted)
	{
		FlushCubesList();
		g_GlobleConfig.Write();
	}
}

void MainWindow::slot_ModifyBox(bool)
{
	CubeSettingsDialogImps d;
	d.SetMode(1);

	QItemSelectionModel* pISM = m_TrackBoxDockList->m_Setup.listView->selectionModel();
	QModelIndexList selectBoxes = pISM->selectedRows();
	if(selectBoxes.size() > 1 || selectBoxes.size() == 0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("跟踪框")
			, QString::fromLocal8Bit("只能选择一个跟踪框去编辑。"));
		return;
	}

	QStandardItem* item =  m_pTrackBoxItemModel->item(selectBoxes.at(0).row());
	cube_data cd = g_GlobleConfig.GetCubeFromName(item->text());

	d.SetValue(cd);

	if(d.exec() == QDialog::Accepted)
	{
		FlushCubesList();
		g_GlobleConfig.Write();
	}
}

void MainWindow::SetUpMenus( QMenuBar* pMB )
{
	QMenu* pMenu = new QMenu(pMB);
	pMenu->setTitle(QString::fromLocal8Bit("实时数据跟踪(&R)"));
	pMenu->addAction(m_pStartTrackAction);
	pMenu->addAction(m_pTrackBoxSettingAction);
	pMenu->addAction(m_pPlayHistoryTrackAction);
	pMB->addMenu(pMenu);

	pMenu = new QMenu(pMB);
	pMenu->setTitle(QString::fromLocal8Bit("历史数据回放(&T)"));
	pMenu->addAction(m_pLoadHidtoryAtion);
	pMenu->addAction(m_pTrackBoxSettingAction);
	pMenu->addAction(m_pPlayHistoryTrackAction);
	pMB->addMenu(pMenu);

	pMenu = new QMenu(pMB);
	pMenu->setTitle(QString::fromLocal8Bit("系统配置(&S)"));
	pMenu->addAction(m_pDataInputSettingAction);
	pMB->addMenu(pMenu);

	pMenu = new QMenu(pMB);
	pMenu->setTitle(QString::fromLocal8Bit("帮助(&H)"));
	pMenu->addAction(m_pHelpAction);
	pMenu->addAction(m_pAboutAction);
	pMB->addMenu(pMenu);
}

QString MainWindow::GetSelectFileName()
{
	QModelIndex mi = m_FileNameDockList->m_Setup.listView->currentIndex();
	QString strName = "TrackCube";
	if(mi.isValid())
	{
		strName = m_pFileListItemModel->item(mi.row())->text();
		return strName;
	}
	else
	{
		return "";
	}
}

void MainWindow::SetTimeText( const QString& text )
{
	QDateTime currDateTime = QDateTime::fromString(text, "yyyyMMddhhmmss");
	currDateTime.setTimeSpec(Qt::UTC);

	QDateTime bjtDateTime = currDateTime.addSecs(8 * 3600);//.toTimeSpec(Qt::LocalTime);

	pHgtTxt->setText(bjtDateTime.toString("yyyy-MM-dd hh:mm:ss (BJT)").toAscii().data());
}

void MainWindow::LoadPaoDian()
{
	goto_gis::GeosOnTheOsg* pGOTO = goto_gis::GeosOnTheOsg::Instantiate();
	goto_gis::Layer* pLayer = new goto_gis::VectorLayer(0, 0, 1);
	QString strFileName("./Config/PaoDian.bjpd");

	pGOTO->OpenDataAsLayer(QStringToStdString(strFileName), pLayer);
	pLayer->CoordTrans(m_pMap->GetCoordinateTransform());
	pLayer->Visible(true);
	pLayer->LayerZ(-10);
	pLayer->SetLayerColor(osg::Vec4(136/255.0, 0, 21/255.0, 1));
	pLayer->CreateLayerData();

	m_pMap->AddLayer("PaoDian", pLayer);
	//osg::Node* pNode = m_pMap->GetRootNode();

	m_pPaoDinaControlerInterface = 
		(PaoDinaControlerInterface*)(pLayer->GetDataProvider()->GetExtendInterface(0));

	m_PaoDianDockWidget->LoadSlot(m_pPaoDinaControlerInterface->GetModel());
	m_PaoDianDockWidget->SetPaoDinaControlerInterface(m_pPaoDinaControlerInterface);

	m_pPaoDinaControlerInterface->HideAll();

}

void MainWindow::SelectLastedFileListItem()
{
	int iRowIndex = m_FileNameDockList->m_Setup.listView->model()->rowCount() - 1;
	if(iRowIndex > 0)
	{
		QModelIndex mi = m_FileNameDockList->m_Setup.listView->model()->index(iRowIndex, 0);

		m_FileNameDockList->m_Setup.listView->setCurrentIndex(mi);
	}
}

FlightPathControler* MainWindow::LoadFlightPath(const QString& fileName)
{
	if(m_FileList.size() <= 0) return 0;
	std::string layerName = QString::fromLocal8Bit("FlightPathLayer").toStdString();
	m_pMap->RemoveLayer(layerName);
	goto_gis::GeosOnTheOsg* pGOTO = goto_gis::GeosOnTheOsg::Instantiate();
	goto_gis::Layer* pLayer = new goto_gis::VectorLayer(0, 0, 1);

	pGOTO->OpenDataAsLayer(fileName.toStdString(), pLayer);
	pLayer->CoordTrans(m_pMap->GetCoordinateTransform());
	pLayer->Visible(true);
	pLayer->SetLineWidth(3.0f);
	pLayer->LayerZ(-20);
	pLayer->SetLayerColor(osg::Vec4(180/255.0, 0, 0, 1));
	pLayer->CreateLayerData();
	
	m_pMap->AddLayer(layerName, pLayer);

	m_FlightPathControler = static_cast<FlightPathControler*>(pLayer->GetDataProvider()->GetExtendInterface(0));
	
	return m_FlightPathControler;
}

void MainWindow::FileListScrollButton()
{
	m_FileNameDockList->m_Setup.listView->scrollToBottom();
	SelectLastedFileListItem();
}

void MainWindow::SetFileListSelectMode( QListView::SelectionMode sm )
{
	m_FileNameDockList->m_Setup.listView->setSelectionMode(sm);
}

void MainWindow::StartRealDisplay()
{

	m_MyDisplayRealFile.start();
}

void MainWindow::StopRealDispaly()
{
	m_MyDisplayRealFile.stopDispaly();
	m_MyDisplayRealFile.wait();
}

void MainWindow::DefaultAction()
{
	//QString strName = GetSelectFileName();

	if(m_FileList.size() < 2) return;

	int iMaxIndex = m_FileList.size() - 1;
	int iFirstIndex = m_FileList.size() - 2;

	m_SelectReplayFilesList.clear();
	m_SelectReplayFilesList.push_back(m_FileList.at(iFirstIndex));
	m_SelectReplayFilesList.push_back(m_FileList.at(iMaxIndex));

	m_CurrentReplayFileIndex = 1;
	StartRealDisplay();
}

void MainWindow::WaitSlaveThread()
{
	m_MySloveThread.wait();
}

void MainWindow::slot_PreviousFile( bool )
{
	QModelIndex mi = m_FileNameDockList->m_Setup.listView->currentIndex();
	if(mi.isValid() )
	{
		int iRowCount = m_pFileListItemModel->rowCount();
		int iPreviousRow = mi.row() - 1;
		if(iPreviousRow >= 0)
		{
			QStandardItem* pItem = m_pFileListItemModel->item(iPreviousRow);
			if(pItem)
			{
				QString FileName = pItem->text();
				QString strFilePath = m_FileBaseMapToFilePath.value(FileName);
				m_pControlorInterface->LoadNcFile(QStringToStdString(strFilePath));
				m_pControlorInterface->DisplayNcFile(QStringToStdString(strFilePath));
				m_FileNameDockList->m_Setup.listView->setCurrentIndex(pItem->index());
				SetTimeText(FileName);
			}
		}
	}
}

void MainWindow::slot_NextFile( bool )
{
	QModelIndex mi = m_FileNameDockList->m_Setup.listView->currentIndex();
	if(mi.isValid() )
	{
		int iRowCount = m_pFileListItemModel->rowCount();
		int iNextRow = mi.row() + 1;
		if(iNextRow < iRowCount)
		{
			QStandardItem* pItem = m_pFileListItemModel->item(iNextRow);
			if(pItem)
			{
				QString FileName = pItem->text();
				QString strFilePath = m_FileBaseMapToFilePath.value(FileName);
				m_pControlorInterface->LoadNcFile(QStringToStdString(strFilePath));
				m_pControlorInterface->DisplayNcFile(QStringToStdString(strFilePath));

				m_FileNameDockList->m_Setup.listView->setCurrentIndex(pItem->index());

				SetTimeText(FileName);
			}
		}
	}
}

void MainWindow::GetSelectTrackBoxList( QVector<cube_data>& boxList )
{
	QItemSelectionModel* pISM = m_TrackBoxDockList->m_Setup.listView->selectionModel();

	QModelIndexList selectBoxes = pISM->selectedRows();
	
	for(int i = 0; i < selectBoxes.size(); i++)
	{
		QStandardItem* item =  m_pTrackBoxItemModel->item(selectBoxes.at(i).row());
		cube_data cd = g_GlobleConfig.GetCubeFromName(item->text());

		if(m_iCurrentLayer >= 0 && m_iCurrentLayer < 20)
			cd.height = QString::number(g_LayerHeight[m_iCurrentLayer]);
		
		boxList.push_back(cd);
	}
}

void MainWindow::AutoStartToTrack()
{
	/*QItemSelectionModel* pISM = m_TrackBoxDockList->m_Setup.listView->selectionModel();

	QModelIndexList selectBoxes = pISM->selectedRows();
	for(int i = 0; i < selectBoxes.size(); i++)
	{
		slot_TrackListClicked(selectBoxes.at(i));
	}*/
	slot_ActionTriggered(m_pLoadHidtoryAtion);
	//QItemSelectionModel* pISM = m_TrackBoxDockList->m_Setup.listView->selectionModel();

	//QModelIndex topLeft = m_TrackBoxDockList->m_Setup.listView->model()->index(0, 0, QModelIndex());
	//QModelIndex bottomRight = m_TrackBoxDockList->m_Setup.listView->model()->index(5, 2, QModelIndex());
	//QItemSelection selection(topLeft, bottomRight);
	//pISM->select(topLeft, QItemSelectionModel::Select);
	//pISM->setCurrentIndex();
	//QModelIndex index = pISM->currentIndex();
	
	
	
	int iSize = m_FileList.size() + 1;
	m_pProgressDlg = new QProgressDialog(QString::fromLocal8Bit("正在自动跟踪...")
		, QString::fromLocal8Bit("停止跟踪")
		, 0, iSize, this);
	m_pProgressDlg->show();
	m_DirectStartTrackAction->MyAction();

	m_bAutoSaveResult = true;
}

void MainWindow::GenerateTrackBox()
{
	PaoDianListDialog pd;
	pd.LoadSlot(m_pPaoDinaControlerInterface->GetModel());
	pd.SetPaoDinaControlerInterface(m_pPaoDinaControlerInterface);
	pd.exec();
}

void MainWindow::CopyTrackBox()
{
	QItemSelectionModel* pISM = m_TrackBoxDockList->m_Setup.listView->selectionModel();

	QModelIndexList selectBoxes = pISM->selectedRows();
	if(selectBoxes.size() <= 0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("跟踪框"), QString::fromLocal8Bit("请选择跟踪框。（可多选）"));
		return;
	}

	for(int i = 0; i < selectBoxes.size(); i++)
	{
		QStandardItem* item =  m_pTrackBoxItemModel->item(selectBoxes.at(i).row());
		cube_data cd = g_GlobleConfig.GetCubeFromName(item->text());

		if(m_iCurrentLayer >= 0 && m_iCurrentLayer < 20)
			cd.height = QString::number(g_LayerHeight[m_iCurrentLayer]);
		
		cd.name = cd.name + "-copy";
		g_GlobleConfig.ConfigCubes(0, cd.name, &cd);
		
		FlushCubesList();
		g_GlobleConfig.Write();
	}
}

void MainWindow::slot_CopyBox( bool )
{
	CopyTrackBox();
}

#if 0
void loadFileFromTime(const std::string& _filePath
	, std::vector<std::string>& fileList)
{
	QString strFilePath = QString::fromStdString(_filePath);
	if(strFilePath.contains("<->"))
	{
		QFileInfo fileInfo(strFilePath);
		QString FileBasename = fileInfo.baseName();
		QString FileName = FileBasename;
		QStringList Times = FileName.split("<->");

		QDateTime startTime = QDateTime::fromString(Times[0], "yyyy-MM-dd hh:mm:ss");
		QDateTime endTime = QDateTime::fromString(Times[1], "yyyy-MM-dd hh:mm:ss");
		QDir dir = fileInfo.absoluteDir();
		//qDebug(startTime.toString("yyyyMMddhhmm").toStdString().c_str());
		//qDebug(endTime.toString("yyyyMMddhhmm").toStdString().c_str());
		//QString strTemp = dir.absolutePath();
		QFileInfoList infoList = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot/*, QDir::Time*/);
		//m_tniv.clear();
		for(int i = 0; i < infoList.size(); i++)
		{
			QString strFilePath = infoList.at(i).fileName();
			QFileInfo info(strFilePath);
			QString strTemp = info.baseName();

			QDateTime currDateTime = QDateTime::fromString("20" + strTemp, "yyyyMMddhhmm");
			//strTemp = currDateTime.toString("yyyyMMddhhmmss");
			//加载雷达反射

			if(startTime <= currDateTime && currDateTime <= endTime)
			{

				QString strFilePathAndName = infoList.at(i).absoluteFilePath();

				fileList.push_back(strFilePathAndName.toStdString());
				//NcFile* pNcFile = new NcFile(strFilePathAndName.toAscii().data());

				//m_FileNameToNcFile[strTemp.toStdString()] = pNcFile;
				//m_FileNames.push_back(strTemp.toStdString());
				////NcFile* pNcFile2 = new NcFile(strFilePathAndName.toAscii().data());
				//if(!pNcFile || pNcFile->is_valid() == FALSE)
				//{
				//	continue;
				//}
				//m_pMakeTexture->loadNcFileToTexture(pNcFile, pTexture, pV3Array);

				////QString ffff("E:\\ProjectData\\Image\\%1.png");
				////osg::Image* pImage = osgDB::readImageFile(ffff.arg(i+1).toStdString());
				//pRG->AddArray(pV3Array);
				//pRG->AddImage(pTexture);

				//m_tniv.push_back(pNcFile);
			}
		}
	}
	else
	{
		//NcFile* pNcFile = new NcFile(_filePath.c_str());
		////m_FileName
		//m_pMakeTexture->loadNcFileToTexture(pNcFile, pTexture, pV3Array);
		//pRG->AddArray(pV3Array);
		//pRG->AddImage(pTexture);

		//pNcFile->close();
		//delete pNcFile;
	}
}
#endif

void MySlaveThread::run()
{
	//QVector<cube_data> mycubes;
	//g_GlobleConfig.GetCubes(mycubes);

	for(/*int i = 0*/; true/*i < m_pMainWindow->m_FileList.size()*/; /*i++*/)
	{
		//QString strFile1 = m_pMainWindow->m_FileList.at(i - 1);
		//QString strFile2 = m_pMainWindow->m_FileList.at(i);
		//QString strFilePath1 = m_pMainWindow->m_FileBaseMapToFilePath.value(strFile1);
		//QString strFilePath2 = m_pMainWindow->m_FileBaseMapToFilePath.value(strFile2);

		QString strFile1;
		QString strFile2;
		QString strFilePath1;
		QString strFilePath2;
		//Sleep(500);

__CHECK__: 
		if(m_pMainWindow->m_TrackState == MainWindow::PAUSE)
		{
			Sleep(800);
			if(m_pMainWindow->m_TrackState == MainWindow::STOP)
			{
				break;
			}
			goto __CHECK__;
		}
		else if(m_pMainWindow->m_TrackState == MainWindow::STOP)
		{
			break;
		}

		if(m_iMode == 0)
		{
			QPair<QString, QString> filePair;
			int iType = 0;
			if(m_pMainWindow->m_TrackState == MainWindow::REPLAY)
			{
				/*iType = 1;
				if(!m_pMainWindow->GetTrackFileID(filePair, iType))
				{
					break;
				}*/
				int iR = m_pMainWindow->GetRealTrackFileID(filePair, 1);
				if(iR != 1 && iR != 0)
				{
					Sleep(20000);
					continue;
				}
			}
			else
			{
				int iR = m_pMainWindow->GetRealTrackFileID(filePair);
				if(iR != 1 && iR != 0)
				{
					Sleep(20000);
					continue;
				}
			}
			
			strFile1 = filePair.first;
			strFile2 = filePair.second;
			strFilePath1 = m_pMainWindow->GetTrackFilePath(strFile1);
			strFilePath2 = m_pMainWindow->GetTrackFilePath(strFile2);
		}
		else
		{
			QPair<QString, QString> filePair;
			int iType = 0;
			if(m_pMainWindow->m_TrackState == MainWindow::REPLAY)
			{
				iType = 1;
			}

			if(!m_pMainWindow->GetTrackFileID(filePair, iType))
			{
				break;
			}

			strFile1 = filePair.first;
			strFile2 = filePair.second;
			strFilePath1 = m_pMainWindow->GetTrackFilePath(strFile1);
			strFilePath2 = m_pMainWindow->GetTrackFilePath(strFile2);
		}

		if(m_pMainWindow->m_TrackState == MainWindow::REPLAY)
		{
			Sleep(800);
			m_pMainWindow->m_pControlorInterface->LoadNcFile(QStringToStdString(strFilePath2));
			
			for(int ii = 0; ii < cubes.size(); ii++)
			{
				CUBE_DATE cd;
				cube_data mycd = cubes.at(ii);
				MainWindow::ResultRect rr;
				rr.name = mycd.name;
				m_pMainWindow->Lock();
				m_pMainWindow->m_RectResultSet.push_back(rr);
				m_pMainWindow->Unlock();
			}

			emit signal_Replay(strFilePath2);
		}
		else
		{
			for(int ii = 0; ii < cubes.size(); ii++)
			{
				CUBE_DATE cd;
				cube_data mycd = cubes.at(ii);

				cd.fTargetFstLon = mycd.left_top_lon;
				cd.fTargetLstLon = mycd.right_bottom_lon;

				cd.fTargetFstLat = mycd.right_bottom_lat;
				cd.fTargetLstLat = mycd.left_top_lat;

				cd.iHeightDown = mycd.height_down.toInt();
				cd.iHeightUp = mycd.height_up.toInt();
				cd.iHeight = mycd.height.toInt();
			
				for(int iii = 0; iii < mycd.displays.size(); iii++)
				{
					if(mycd.displays[iii] == 1)
					{
						cd.refs.push_back(m_pMainWindow->m_pControlorInterface->GetValue(iii));
					}
				}
				////////////////////////////////传递上一次的结果///////////////////////////
				qDebug()<<strFilePath1<<"----------------------------"<<strFilePath2;
				qDebug()<<mycd.left_top_lon;
				qDebug()<<mycd.right_bottom_lat;
				qDebug()<<mycd.right_bottom_lon;
				qDebug()<<mycd.left_top_lat;

				//////////////////////////////////////////////////////////////////////////
				//m_pMainWindow->m_pControlorInterface->OpenNC(strFilePath2.toStdString());
	#if !_DEBUG
				qDebug()<<"--------------------Start----------------------";
			
				P_TREC_NC_MAP_OUT pOut = m_pMainWindow->m_pControlorInterface->runTrec(QStringToStdString(strFilePath1)
					, QStringToStdString(strFilePath2), cd);
			
				qDebug()<<"---------------------end-----------------------";
	#else
				P_TREC_NC_MAP_OUT pOut = m_pMainWindow->Test(QStringToStdString(strFilePath1)
					, QStringToStdString(strFilePath2), cd);
	#endif
				/////////////////////////////////handle reslut/////////////////////////////////////////
				//m_pMainWindow->Lock();

			

				osg::Vec3 p1, p2, p3, p4;
				float fTargetFstLon = pOut->begin()->second.speed.NLon;
				float fTargetFstLat = pOut->begin()->second.speed.Nlat;
				float fTargetLstLon = pOut->begin()->second.speed.Elon;
				float fTargetLstLat = pOut->begin()->second.speed.Elat;

				mycd.left_top_lon = fTargetFstLon;
				mycd.right_bottom_lon = fTargetLstLon;

				mycd.right_bottom_lat = fTargetFstLat;
				mycd.left_top_lat = fTargetLstLat;

				cubes[ii] = mycd;

				qDebug()<<cubes[ii].left_top_lon;
				qDebug()<<cubes[ii].right_bottom_lat;
				qDebug()<<cubes[ii].right_bottom_lon;
				qDebug()<<cubes[ii].left_top_lat;
				qDebug()<<"----------------------------";

				p1.set(fTargetFstLon, fTargetLstLat, 0);
				p2.set(fTargetLstLon, fTargetLstLat, 0);
				p3.set(fTargetLstLon, fTargetFstLat, 0);
				p4.set(fTargetFstLon, fTargetFstLat, 0);

				/*QString strNameID 
					= QString("%1%2%3").arg(mycd.name, "|", strFilePath2);*/

				//qDebug()<<mycd.name<<" : "<<p1.x()<<", "<<p1.y();
				//qDebug()<<mycd.name<<" : "<<p3.x()<<", "<<p3.y();

				MainWindow::ResultRect rr;
				rr.name = mycd.name;
				rr.p1 = p1;
				rr.p2 = p2;
				rr.p3 = p3;
				rr.p4 = p4;
				rr.color = osg::Vec4(0.4, 0.4, 0.4, 0.4);
				m_pMainWindow->Lock();
				m_pMainWindow->m_RectResultSet.push_back(rr);
				m_pMainWindow->Unlock();
				//m_pMainWindow->m_pControlorInterface->AddRect(mycd.name.toStdString(), p1, p2, p3, p4, osg::Vec4(1.0, 0.0, 0.0, 1.0));
				////////////////////////////////////获得跟踪框中的回波强度图像//////////////////////////////////////
				float maxHeight = mycd.heightMax, maxDbz = 0, DbzVolume = mycd.volumeMax;
				int upHeight = getHeightIndex(cd.iHeightUp);
				int downHeight = getHeightIndex(cd.iHeightDown);

				QImage* pImage = m_pMainWindow->m_pControlorInterface->ClipNcFile(QStringToStdString(strFilePath2), osg::Vec3(fTargetFstLon, fTargetLstLat, 0), osg::Vec3(fTargetLstLon, fTargetFstLat, 0), 0,
					upHeight, downHeight,&maxHeight, &DbzVolume, &maxDbz);
				
				//--------------新数据编辑-------------------------
				osg::Vec3 v3WorldP1, v3WorldP2;
				m_pMainWindow->m_pMap->GetCoordinateTransform()->Transform(osg::Vec3(fTargetFstLon, fTargetLstLat, 0), v3WorldP1);
				m_pMainWindow->m_pMap->GetCoordinateTransform()->Transform(osg::Vec3(fTargetLstLon, fTargetFstLat, 0), v3WorldP2);
				//---------------------------------------------------

				//--------------------------自定义剖面-----------------------
				cube_data sectionCD = old_cubes[ii];
				if((abs(sectionCD.point1X - sectionCD.point2X) > 0.0001) || (abs(sectionCD.point1Y - sectionCD.point2Y) > 0.0001))
				//if(false)
				{
					osg::Vec3 v3OldP1(sectionCD.left_top_lon, sectionCD.left_top_lat, 0);
					
					osg::Vec3 v3OldWorldP1;
					m_pMainWindow->m_pMap->GetCoordinateTransform()->Transform(v3OldP1, v3OldWorldP1);
					
					osg::Vec3 v3LeftTop(fTargetFstLon, fTargetLstLat, 0);
					osg::Matrix MTran;
					MTran.setTrans(osg::Vec3(v3WorldP1.x() - v3OldWorldP1.x(), 0, v3WorldP1.z() - v3OldWorldP1.z()));


					osg::Vec3 v3SectionP1(sectionCD.point1X, sectionCD.point1Y, 0);
					osg::Vec3 v3SectionP2(sectionCD.point2X, sectionCD.point2Y, 0);

					osg::Vec3 v3SectionWorldP1, v3SectionWorldP2;
					m_pMainWindow->m_pMap->GetCoordinateTransform()->Transform(v3SectionP1, v3SectionWorldP1);
					m_pMainWindow->m_pMap->GetCoordinateTransform()->Transform(v3SectionP2, v3SectionWorldP2);
					v3WorldP1 = v3SectionWorldP1 * MTran;
					v3WorldP2 = v3SectionWorldP2 * MTran;
				}
				
				//-----------------------------------------------------------

				QImage* pSectionImage = m_pMainWindow->m_pControlorInterface->GetSectionNcFile(QStringToStdString(strFilePath2), v3WorldP1, v3WorldP2);

				m_pMainWindow->m_ClipNcResultMutex.lock();
				QMap<QString, QImage*> clipImage;
				clipImage.insert(strFile2, pImage);
				m_pMainWindow->m_ClipNcImageResult.insert(mycd.name, clipImage);
				//m_pMainWindow->m_ClipNcResultMutex.unlock();
				////////////////////////////////////////////////////////////////////////////////////////////////////////////
				///////////////////////////////////////////////////获得框中的剖面图////////////////////////////////////
				//m_pMainWindow->m_ClipNcResultMutex.lock();
				QMap<QString, QImage*> SectionImage;
				SectionImage.insert(strFile2, pSectionImage);
				m_pMainWindow->m_SectionImageResult.insert(mycd.name, SectionImage);
				m_pMainWindow->m_ClipNcResultMutex.unlock();
				////////////////////////////////////////////////////////////////////////////////////////////////////////////
				QMap<QString, QVector<MainWindow::sc>>::iterator finded1 
					= m_pMainWindow->m_Result1.find(mycd.name);

				MainWindow::sc sc_value;
				sc_value.p = pOut->begin()->second.precipitation.precipitation;
				sc_value.c = pOut->begin()->second.precipitation.content;
				sc_value.h = maxHeight > 0 ? g_LayerHeight[(int)maxHeight] : 0;
				sc_value.m = maxDbz;
				sc_value.v = DbzVolume;
				if(finded1 != m_pMainWindow->m_Result1.end())
				{
					finded1.value().push_back(sc_value);
				}
				else
				{
					QVector<MainWindow::sc> rvector;
					rvector.push_back(sc_value);
					m_pMainWindow->m_Result1.insert(mycd.name, rvector);
				}


				QMap<QString, QMap<int, QVector<MainWindow::ri>>>::Iterator finded2 
					= m_pMainWindow->m_Result2.find(mycd.name);
				//先查跟踪框名，再差对应的反射值。
				if(finded2 != m_pMainWindow->m_Result2.end())
				{
					for(size_t iii = 0; iii < pOut->begin()->second.ratio.interval.size(); iii++)
					{
						QPolygonF plotData3;

						float fRatio = pOut->begin()->second.ratio.ratio.at(iii);
						int iV = pOut->begin()->second.ratio.interval.at(iii);
						int iNum = 0;

						if(pOut->begin()->second.ratio.number.size() > 0)
						{
							iNum = pOut->begin()->second.ratio.number.at(iii);
						}

						MainWindow::ri ri_value;
						ri_value.in = iNum;
						ri_value.ir = fRatio;

						QMap<int, QVector<MainWindow::ri>>::iterator findedValue 
							= finded2.value().find(iV);

						if(findedValue != finded2.value().end())
						{
							findedValue.value().push_back(ri_value);
						}
						else
						{
							QVector<MainWindow::ri> riVector;
							riVector.push_back(ri_value);
							finded2->insert(iV, riVector);
						}
					}

				}
				else
				{
					QMap<int, QVector<MainWindow::ri>> mapValue;
					for(size_t iii = 0; iii < pOut->begin()->second.ratio.interval.size(); iii++)
					{
						QPolygonF plotData3;

						TREC_OUT* pTREC_OUT = &(pOut->begin()->second);
						float fRatio = pTREC_OUT->ratio.ratio.at(iii);
						int iV = pTREC_OUT->ratio.interval.at(iii);
						int iNum = 0;

						if(pTREC_OUT->ratio.number.size() > 0)
						{
							iNum = pTREC_OUT->ratio.number.at(iii);
						}

						MainWindow::ri ri_value;
						ri_value.in = iNum;
						ri_value.ir = fRatio;
						QVector<MainWindow::ri> riVector;
						riVector.push_back(ri_value);
						mapValue.insert(iV, riVector);
					}

					m_pMainWindow->m_Result2.insert(mycd.name, mapValue);

				
				}
				delete pOut;
				//m_pMainWindow->Unlock();
			}
			m_pMainWindow->m_pControlorInterface->LoadNcFile(QStringToStdString(strFilePath2));
			emit signal_TrackDone(strFilePath2);
		}

		//emit signal_ToDeleteOld(strFilePath1);
	}

	emit signal_StopTrack();
}

void MyDisplayRealFile::run()
{
	while(true)
	{
		Sleep(800);
		m_Mutex.lock();
		if(m_Stop)
		{
			m_Mutex.unlock();
			break;
		}
		m_Mutex.unlock();
		QPair<QString, QString> filePair;
		int iR = m_pMainWindow->GetRealTrackFileID(filePair, 1);
		if(iR != 1 && iR != 0)
		{
			//Sleep(20000);
			continue;
		}

		//QString strFile1 = filePair.first;
		QString strFile2 = filePair.second;
		//QString strFilePath1 = m_pMainWindow->GetTrackFilePath(strFile1);
		QString strFilePath2 = m_pMainWindow->GetTrackFilePath(strFile2);
		m_pMainWindow->m_pControlorInterface->LoadNcFile(QStringToStdString(strFilePath2));

		emit signal_Replay(strFilePath2);
	}
	
}

void MyDisplayRealFile::start()
{
	if(g_GlobleConfig.ModeType() != 0) return;

	m_Mutex.lock();
	if(m_Stop == false)
	{
		m_Mutex.unlock();
		return;
	}
	m_Stop = false;
	m_Mutex.unlock();

	QThread::start();
	
}

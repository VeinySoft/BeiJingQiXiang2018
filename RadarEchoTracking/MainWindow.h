#pragma once
#include "MyActionInterface.h"
#include "TrecCallbackImps.h"
#include "ConfigAccesser.h"
#include "ModeSettingDialogImps.h"

class TrecCallback;
class SnapImageDrawCallback;
class TrackBoxDockList;
class FileNameDockList;
class NcFileLayerDockCheckBox;
class PaoDianDockWidget;
class ControlorInterface;
class QwtPlotDialogImps;
class QwtScaleDraw;
class MainWindow;
class DrawClipLine;
class FlightPathControler;
class FlightAndRasterWindow;

class MySlaveThread : public QThread
{
	Q_OBJECT
public:
	virtual void run();
	inline void setMainWindow(MainWindow* pM){m_pMainWindow = pM;}
	inline void setCubes(const QVector<cube_data>& cubes){this->cubes = cubes;old_cubes = cubes;}
	inline void setMode(int iM){m_iMode = iM;}
	inline void setIsTracking(bool bI){m_bToTracking = bI;}
	inline QVector<cube_data>* GetCubesPointer(){return &cubes;}
private:
	MainWindow* m_pMainWindow;
	QVector<cube_data> cubes;
	QVector<cube_data> old_cubes;
	int m_iMode;
	bool m_bToTracking;
signals:
	void signal_TrackDone(const QString& doneFile);
	void signal_ToDeleteOld(const QString& preFile);
	void signal_StopTrack();
	void signal_Replay(const QString& doneFile);
};

class MyDisplayRealFile : public QThread
{
	Q_OBJECT
public:
	inline MyDisplayRealFile() : m_Stop(true){}
	virtual void run();
	inline void stopDispaly(){m_Stop = true;this->wait();}
	void start();
	MainWindow* m_pMainWindow;
signals:
	void signal_Replay(const QString& doneFile);
private:
	bool m_Stop;
	QMutex m_Mutex;
};

class MainWindow :
	public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(void);
	~MainWindow(void);
	void LoadBusinessFeature();
	void TransformData();
	void InitRasterWindow();
	void SetUpActions(QToolBar* pToolBar);
	inline void SwitchDefalultMaipulator(){
		m_keySwitchMaipulator->selectMatrixManipulator(0);
		m_pRestorePointerAction->setChecked(true);
		m_pTrackBoxSettingAction->setChecked(false);
	}

	inline void SwitchDrawRectMaipulator(){
		m_keySwitchMaipulator->selectMatrixManipulator(1);
		m_pRestorePointerAction->setChecked(false);
		m_pTrackBoxSettingAction->setChecked(true);
	}
	void LoadPaoDian();
	FlightPathControler* LoadFlightPath(const QString& fileName);
	inline void SetHandCursor(){
		QPixmap pix("./icon/hand.png");
		m_pViewerQT->setCursor(Qt::WaitCursor);}
	void ScaleToMap();
	//inline P_TREC_NC_MAP_OUT GetTrecOutData(){return &m_TrecOutData;}
	inline TrecCallback* GetTrecCallback(){
		return m_pTrecCallbackImaps;}

	inline MyActionInterface* GetDrawRectAction(){return m_pTrackBoxSettingAction;}
	inline MyActionInterface* GetRestoreHandAction(){return m_pRestorePointerAction;}
	inline bool CheckTrecReuslt(){return m_TrecOutData.size()>0 ? true : false;}
	inline goto_gis::Map* GetMap(){return m_pMap;}
	inline void ClearResult(){m_TrecOutData.clear();}
	inline void ChangeProcess(int iValue){
		m_pProgressDlg != nullptr ? m_pProgressDlg->setValue(m_pProgressDlg->value() + iValue) : m_pProgressDlg = m_pProgressDlg;
	}
	void ExportImage(const QString& fileName);
	void AddFileTime();
	void AddColorTable(bool vertical);
	void StartTrack();
	int StartTrack2();
	void StartReplay();
	void DisplayPlot();
	void ExportRegionData();
	void GetSelectTrackBoxList(QVector<cube_data>& boxList);
	int GetTrackFileID(QPair<QString, QString>& filePair, int iType = 0);
	QString& GetTrackFilePath(const QString& strID);

	int GetRealTrackFileID(QPair<QString, QString>& filePair, int iType = 0);
	QString& GetRealTrackFilePath(const QString& strID);

	P_TREC_NC_MAP_OUT Test(std::string& file1, std::string& file2, CUBE_DATE& cd);

	void FlushCubesList();
	virtual void closeEvent(QCloseEvent* event);

	inline void StopStrack(){m_TrackState = MainWindow::STOP;m_pTrackBoxSettingAction->setEnabled(true);}
	inline void PaushStarck(){m_OldTrackState = m_TrackState;m_TrackState = MainWindow::PAUSE;}
	inline void ResumeStarck(){m_TrackState = m_OldTrackState;}
	inline void PlayStrack(){m_TrackState = MainWindow::PLAY;}
	inline void ReplayStrack(){m_TrackState = MainWindow::REPLAY;}
	inline void Lock(){m_ResultMutex.lock();}
	inline void Unlock(){m_ResultMutex.unlock();}
	inline ControlorInterface* GetControlorInterface(){return m_pControlorInterface;}

	void FillSelectList(int iType = 0);
	void FlushPlotAixsMark();

	void EnabledTrack(bool bd);
	void EnabledReplay(bool br);

	void DefaultTrackStart();
	void SetUpMenus(QMenuBar* pMB);

	QString GetSelectFileName();
	void SetTimeText(const QString& text);
	void CreateRadarDisplayLayer();
	void FlushList();
	void FileListScrollButton();
	void SetFileListSelectMode(QListView::SelectionMode sm);
	void StartRealDisplay();
	void StopRealDispaly();

	void DefaultAction();
	void SelectLastedFileListItem();
	void WaitSlaveThread();
	void AutoStartToTrack();
	void GenerateTrackBox();
	void CopyTrackBox();
signals:
	void signal_InsertFile(const QString& fileBase);
	void signal_SelectFiles(const QStringList& fileNames);
public Q_SLOTS:
	void slot_ActionTriggered( QAction* action );
	void slot_TrecDone(QStringList& fileList, QString& strCubeName, P_TREC_NC_MAP_OUT pOutMap);
	void slot_TrackDone(const QString& doneFile);
	void slot_ToDeleteOld(const QString& preFile);
	void slot_InsertFile(const QString& fileBase);
	void slot_SelectLayer(int index);
	void slot_doubleClicked(const QModelIndex& index);
	void slot_TrackListClicked(const QModelIndex& index);
	void slot_NameListClicked(const QModelIndex& index);
	void slot_StopTrack();
	void slot_DeleteBox(bool);
	void slot_CopyBox(bool);
	void slot_AddBox(bool);
	void slot_ModifyBox(bool);
	void slot_Replay(const QString& doneFile);
	void slot_PreviousFile(bool);
	void slot_NextFile(bool);
private:
	void TrackBoxSelectState();
	void ClearTrackResult();
	void FlushFileList();
	void AddDrawRectManipulator();
	void AddDrawLineManipulator();

	void FillList();
	void DisplayRadarData(const QString& strFile, const QIcon* pIcon = 0);
	void AddScale(const osg::ref_ptr<osg::Geode>& geode);
	void ChangeRectColor(const QString& doneFile );
	void GetTimeString(const QString& str1, QString& strTimeString);

	MySlaveThread m_MySloveThread;
	MyDisplayRealFile m_MyDisplayRealFile;

	ViewerQT* m_pViewerQT;
	goto_gis::Map* m_pMap;
	osgViewer::View* m_pView;
	
	QActionGroup* m_pActionGroup;
	QActionGroup* m_pActionGroup2;

	MyActionInterface* m_pStartTrackAction;
	MyActionInterface* m_DirectStartTrackAction;
	MyActionInterface* m_pPauseTrackAction;
	MyActionInterface* m_pStopTrackAction;

	MyActionInterface* m_pTrackBoxSettingAction;
	MyActionInterface* m_pDataInputSettingAction;
	MyActionInterface* m_pExprotImageAction;
	MyActionInterface* m_pRestorePointerAction;
	MyActionInterface* m_pScaleToMapAction;
	
	MyActionInterface* m_pPrevAction;
	MyActionInterface* m_pNextAction;
	MyActionInterface* m_pPlayHistoryTrackAction;
	QAction* m_pLoadHidtoryAtion;
	QAction* m_pHelpAction;
	QAction* m_pAboutAction;
	QAction* m_pZoomInAction;
	QAction* m_pZoomOutAction;
	QAction* m_pShutdownAction;
	QAction* m_pSaveResultAction;
	QAction* m_pSectionLineAction;
	QAction* m_pStrackBoxSectionAction;
	QAction* m_pDistanctTestAction;
	QAction* m_pShowResultAction;
	QAction* m_pSaveSenceAtion;
	QAction* m_pAutoStartToTrack;
	QAction* m_pAccordingPaoDianToTrackBoxAtion;
	QAction* m_pMoveTrackBoxAction;
	QAction* m_pExportRegionDataAction;
	QAction* m_pOpenFlightPathAction;

	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> m_keySwitchMaipulator;
	osg::ref_ptr<DrawGeometryManipulator> m_pDrawRectMainpulator;
	osg::ref_ptr<DrawGeometryManipulator> m_pDrawLineMainpulator;
	DrawClipLine* m_pDrawClipLine;
	osg::ref_ptr<DefaultManipulator> m_pDefaultMainpulator;
	osg::ref_ptr<osg::Camera> m_pHUDCamera;
	osg::ref_ptr<osg::Group> m_pRootNode;
	osg::ref_ptr<osg::MatrixTransform> modelview;
	TREC_NC_MAP_OUT m_TrecOutData;
	QPointer<TrecCallbackImps> m_pTrecCallbackImaps;
	SnapImageDrawCallback* m_SnapImageDrawCallback;
	PaoDinaControlerInterface* m_pPaoDinaControlerInterface;

	FileNameDockList* m_FileNameDockList;
	TrackBoxDockList* m_TrackBoxDockList;
	NcFileLayerDockCheckBox* m_NcFileLayerDockCheckBox;
	PaoDianDockWidget* m_PaoDianDockWidget;
	ControlorInterface* m_pControlorInterface;
	SetCubesInterface* m_pSetCubesInterface;
	FlightPathControler* m_FlightPathControler;
	QStandardItemModel* m_pFileListItemModel;
	QStandardItemModel* m_pTrackBoxItemModel;
	QMap<QString, QString> m_FileBaseMapToFilePath;
	QStringList m_FileList;
	QStringList m_SelectFilesList;
	QStringList m_SelectReplayFilesList;
	QMap<QString, QwtPlotDialogImps*> m_QwtPlotDialogMap;
	struct ScaleDraw
	{
		int iValue;
		QString strName;
	};

	struct ri
	{
		int in;//数量
		float ir;
	};

	struct sc
	{
		float p;//降水
		float c;
		float h;//回波顶高
		float m;//最大反射率
		float v;//回波体积
	};

	QMap<QString, QVector<sc>> m_Result1;

	QMap<QString, QMap<int, QVector<ri>>> m_Result2;
	osg::ref_ptr<osgText::Text> pHgtTxt;

	friend MySlaveThread;
	friend MyDisplayRealFile;
	QMutex m_FileListMutex;
	QMutex m_FilePathMutex;

	struct ResultRect
	{
		QString name;
		osg::Vec3 p1, p2, p3, p4;
		osg::Vec4 color;
	};

	QVector<ResultRect> m_RectResultSet;

	QMap<int, QString> m_aixsX;
	QMap<QString, QMap<QString, QImage*>> m_ClipNcImageResult;
	QMap<QString, QMap<QString, QImage*>> m_SectionImageResult;

	enum TRACK_STATE {PAUSE = 1, STOP, PLAY, REPLAY};

	TRACK_STATE m_TrackState;
	TRACK_STATE m_OldTrackState;
	QMutex m_ResultMutex;
	QMutex m_ClipNcResultMutex;
	ModeSettingDialogImps m_BoxSettings;
	int m_CurrentFileIndex;
	int m_CurrentReplayFileIndex;
	int m_CurrentRealFileIndex;
	QString m_OpenFileName;
	int m_iCurrentLayer;
	bool m_bAutoSaveResult;
	QProgressDialog* m_pProgressDlg;
	FlightAndRasterWindow* m_pRasterWindow;
};


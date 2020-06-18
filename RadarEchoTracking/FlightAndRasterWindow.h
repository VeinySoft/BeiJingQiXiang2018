#pragma once
#include "QMainWindow"
#include "ui_FlightAndRaster.h"

class FlightPathControler;
class ControlorInterface;
class MainWindow;

class FlightAndRasterWindow :
	public QMainWindow
{
	Q_OBJECT
public:
	typedef QWidget* (*FUNDEF_OpenRasterFile)(int iIndex, const QString& strTitle, const QString& fileName, MemoryData* pGMD);
	typedef void (*FUNDEF_DrawCurve)(int iIndex, const QVector<QPointF>& samplesData);
	typedef QWidget* (*FUNDEF_GetPlotWidget)();
	typedef void (*FUNDEF_ReplotData)(int iIndex);
	typedef void (*FUNDEF_ClearPlot)(int iIndex);
	FlightAndRasterWindow(MainWindow* pM);
	~FlightAndRasterWindow(void);
	void FillList();
	inline void SetMapToFilePath(QMap<QString, QString>* pMap){m_FileBaseMapToFilePath = pMap;}
	inline void FillFilesList(const QStringList& l){m_MatchFileList = l;}
	void UpdateFlightPath(int iState);
	void LinearInterpolation(double* srcBuff, size_t rowsSrc, size_t columnsSrc, double* destBuff, size_t rowsDest, size_t columnsDest);
	void AutoSaveConfig();
	void SaveData(double* pData, size_t rows, size_t cols);

private:
	FUNDEF_OpenRasterFile m_funOpenRasterFile;
	FUNDEF_GetPlotWidget m_funGetPlotWidget;
	FUNDEF_ReplotData m_funReplotData;
	FUNDEF_ClearPlot m_funClearPlot;
	FUNDEF_DrawCurve m_funDrawCurve;

	Ui::FlightAndRaster m_setup;
	FlightPathControler* m_FlightPathControler;
	ControlorInterface* m_pControlorInterface;
	QMap<QString, QString>* m_FileBaseMapToFilePath;
	QStringList m_MatchFileList;
	int m_iDTSelectStartIndex;
	int m_iDTSelectEndIndex;
	bool m_bMatched;
	MainWindow* m_pMainWindow;
	double* m_pCurrentFixRasterData;
	double* m_pCurrentFlyRasterData;
	size_t m_FixDataRows;
	size_t m_FixDataCols;
	size_t m_FlyDataRows;
	size_t m_FlyDataCols;
	//QStringList m_DTSelectList;
public slots:
	void slot_MatchData(bool);
	void slot_MatchFixPointData(bool);
	void slot_DrawRaster(bool);
	void slot_DrawFixPointRaster(bool);
	void slot_ClearMatch(bool);
	void slot_ClearFixPointMatch(bool);
	void slot_updateSelectNcFiles(const QStringList& fileNames);
	void slot_LoadFlightPath(bool);
	void slot_itemSelectionChanged();
	void slot_ExportDrawFlightRaster(bool);
	void slot_exportDrawFixRaster(bool);
	void slot_checkboxUpdate(bool);
	void slot_lineEditUpdate();//editingFinished()
	void slot_lineEdit_2Update();//editingFinished()
	void slot_toolboxUpdate(int);//currentChanged(int index)
};


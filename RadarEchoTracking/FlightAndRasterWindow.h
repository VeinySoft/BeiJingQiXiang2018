#pragma once
#include "QMainWindow"
#include "ui_FlightAndRaster.h"

class FlightPathControler;
class ControlorInterface;

class FlightAndRasterWindow :
	public QMainWindow
{
	Q_OBJECT
public:
	typedef QWidget* (*FUNDEF_OpenRasterFile)(int iIndex, const char* strTitle, const char* fileName, MemoryData* pGMD);
	typedef QWidget* (*FUNDEF_GetPlotWidget)();
	typedef void (*FUNDEF_ReplotData)(int iIndex);
	FlightAndRasterWindow(FlightPathControler* pFPC, ControlorInterface* pCI);
	~FlightAndRasterWindow(void);
	void FillList();
	inline void SetMapToFilePath(QMap<QString, QString>* pMap){m_FileBaseMapToFilePath = pMap;}
	inline void FillFilesList(const QStringList& l){m_FileList = l;}
	void UpdateFlightPath(int iState);
private:
	FUNDEF_OpenRasterFile m_funOpenRasterFile;
	FUNDEF_GetPlotWidget m_funGetPlotWidget;
	FUNDEF_ReplotData m_funReplotData;
	Ui::FlightAndRaster m_setup;
	FlightPathControler* m_FlightPathControler;
	ControlorInterface* m_pControlorInterface;
	QMap<QString, QString>* m_FileBaseMapToFilePath;
	QStringList m_FileList;
	int m_iDTSelectStartIndex;
	int m_iDTSelectEndIndex;
	bool m_bMatched;
	//QStringList m_DTSelectList;
public slots:
	void slot_MatchData(bool);
	void slot_DrawRaster(bool);
	void slot_ClearMatch(bool);
	void slot_updateSelectNcFiles(const QStringList& fileNames);
	
	void slot_itemSelectionChanged();
};


#include "StdAfx.h"
#include "FlightAndRasterWindow.h"
#include <qdatetime.h>

FlightAndRasterWindow::FlightAndRasterWindow(FlightPathControler* pFPC, ControlorInterface* pCI)
	: m_FlightPathControler(pFPC), m_pControlorInterface(pCI), m_bMatched(false), m_funOpenRasterFile(0)
	, m_funGetPlotWidget(0), m_funReplotData(0)
{
	m_setup.setupUi(this);
	this->setWindowIcon(QIcon("./icon/diagram.png"));
	connect(m_setup.pushButton_2, SIGNAL(clicked(bool)), SLOT(slot_MatchData(bool)));
	connect(m_setup.pushButton_3, SIGNAL(clicked(bool)), SLOT(slot_ClearMatch(bool)));
	connect(m_setup.pushButton, SIGNAL(clicked(bool)), SLOT(slot_DrawRaster(bool)));
	connect(m_setup.DateTimeListWidget, SIGNAL(itemSelectionChanged()), SLOT(slot_itemSelectionChanged()));

	QLibrary plotLib("./QwtRasterPolt");

	if(plotLib.load())
	{
		m_funOpenRasterFile = (FUNDEF_OpenRasterFile)plotLib.resolve("OpenRasterFile");
		m_funGetPlotWidget = (FUNDEF_GetPlotWidget)plotLib.resolve("GetPlotWidget");
		m_funReplotData = (FUNDEF_ReplotData)plotLib.resolve("ReplotData");

		QWidget* pPlotWidget = m_funGetPlotWidget();
		m_setup.gridLayout->addWidget(pPlotWidget, 1, 2, 6, 1);
	}
}

FlightAndRasterWindow::~FlightAndRasterWindow(void)
{
}

void FlightAndRasterWindow::FillList()
{
	m_setup.DateTimeListWidget->clear();

	QStringList strDateList = m_FlightPathControler->GetDateList();
	QStringList strTimeList = m_FlightPathControler->GetTimeList();

	for(int i = 0; i < strDateList.size(); i++)
	{
		m_setup.DateTimeListWidget->addItem(strDateList.at(i) + "-" + strTimeList.at(i));
	}
	
	if(strDateList.size() < 2 || strTimeList.size() < 2) return;

	int dtSize = strDateList.size() - 1;
	QString strDTFormat = QString::fromLocal8Bit("d/M/yyyyH:m:s");
	QDateTime startDT = QDateTime::fromString(strDateList.at(0) + strTimeList.at(0), strDTFormat);
	QDateTime endDT = QDateTime::fromString(strDateList.at(dtSize) + strTimeList.at(dtSize), strDTFormat);

	m_setup.dateTimeEdit->setDateTime(startDT);
	m_setup.dateTimeEdit_2->setDateTime(endDT);

	m_iDTSelectStartIndex = 0;
	m_iDTSelectEndIndex = m_setup.DateTimeListWidget->count() - 1;
}

void FlightAndRasterWindow::slot_ClearMatch(bool)
{
	FillList();
	m_bMatched = false;
}

void FlightAndRasterWindow::slot_MatchData(bool)
{
	if(m_bMatched) return;
	QString strDTFormat = QString::fromLocal8Bit("d/M/yyyy-H:m:s");
	int ii = m_iDTSelectStartIndex;
	//QDateTime startDT;
	QDateTime endDT;
	bool bIsSet = true;
	for(int i = 0; i < m_FileList.size(); i++)
	{
		bIsSet = true;
		for(; ii < m_iDTSelectEndIndex + 1; ii++)
		{
			QListWidgetItem* item = m_setup.DateTimeListWidget->item(ii);
			QDateTime currentDT = QDateTime::fromString(item->text(), strDTFormat);
			if(bIsSet)
			{
				//startDT = currentDT;
				endDT = currentDT.addSecs(359);
				bIsSet = false;
			}
			
			if(currentDT <= endDT)
			{
				QString strCurrentText = currentDT.toString(strDTFormat) + "(" + m_FileList[i] + ")";
				item->setText(/*QString("%1").arg(ii) + "-" + */strCurrentText);
				m_bMatched = true;
			}
			else
				break;
		}
	}

}

void FlightAndRasterWindow::slot_DrawRaster(bool)
{
	int iLayerCount = 20;
	MemoryData md;
	md.fXMin = 0;
	md.fXMax = m_iDTSelectEndIndex - m_iDTSelectStartIndex + 1;
	md.fXInterval = 1;
	
	md.fYMin = 0;
	md.fYMax = 19;
	md.fYInterval = 1;

	md.fZMin = -5;
	md.fZMax = 70;
	md.fZInterval = 5;

	int DTCount = m_iDTSelectEndIndex - m_iDTSelectStartIndex + 1;
	md.DataSize = DTCount * iLayerCount;
	md.ColumnCount = DTCount;
	md.pData = new double[md.DataSize];
	memset(md.pData, 100, sizeof(double) * md.DataSize);
	//使用影射将坐标归类到对应的文件中
	QMap<QString, osg::Vec3Array*> dataMap;
	for(int i = m_iDTSelectStartIndex; i < DTCount; i++)
	{
		QListWidgetItem* item = m_setup.DateTimeListWidget->item(i);
		QString strText = item->text();

		int iIndex1 = strText.indexOf("(");
		int iIndex2 = strText.indexOf(")");
		QString strName = strText.mid(iIndex1 + 1, iIndex2 - iIndex1 - 1);

		osg::Vec3 currentVec3 = m_FlightPathControler->GetFlightPath()->at(i);
		if(strName.length() > 0)
		{
			//
			QMap<QString, osg::Vec3Array*>::iterator finded = dataMap.find(strName);
			if(finded != dataMap.end())
			{
				finded.value()->push_back(currentVec3);
			}
			else
			{
				osg::Vec3Array* pVec3Array = new osg::Vec3Array();
				pVec3Array->push_back(currentVec3);
				dataMap.insert(strName, pVec3Array);
			}
		}
		
	}

	//从NC文件中取出一个点20层的数据
	QMap<QString, osg::Vec3Array*>::iterator iterBegin = dataMap.begin();
	
	size_t iSize = 0;
	double* pCurrentPointer = md.pData;
	while(iterBegin != dataMap.end())
	{
		QString strFileFullPathName = m_FileBaseMapToFilePath->value(iterBegin.key(), "");
		
		osg::Vec3Array* pArray = iterBegin.value();
		
		m_pControlorInterface->GetVerticalData(strFileFullPathName.toStdString(), *pArray, osg::Vec3(), pCurrentPointer, &iSize);

		//累加缓冲中的地址，累加迭代器
		pCurrentPointer += pArray->size() * iLayerCount;

		iterBegin++;
	}

	m_funOpenRasterFile(0, "RasterData", "./Config/ColorTable.ini", &md);
	m_funReplotData(0);
	//pWidget->setParent(this);
	//pWidget->show();
	
}

void FlightAndRasterWindow::slot_updateSelectNcFiles(const QStringList& fileNames)
{
	m_FileList.clear();

	QMap<QString, QString>::iterator start = m_FileBaseMapToFilePath->find(fileNames.at(0));
	QMap<QString, QString>::iterator end = m_FileBaseMapToFilePath->find(fileNames.at(1));

	while(1)
	{
		QMap<QString, QString>::iterator current = start++;
		QString strCurrent = current.key();
		if(current == end)
		{
			m_FileList.push_back(end.key());
			break;
		}

		m_FileList.push_back(strCurrent);
	}
}

void FlightAndRasterWindow::slot_itemSelectionChanged()
{
	QList<QListWidgetItem *> itemList = m_setup.DateTimeListWidget->selectedItems();
	
	if(itemList.size() == 1)
	{
		m_iDTSelectStartIndex = m_setup.DateTimeListWidget->row(itemList.at(0));
		m_iDTSelectEndIndex = m_setup.DateTimeListWidget->count() - 1;

		if(m_iDTSelectEndIndex < m_iDTSelectStartIndex)
		{
			int iTemp = m_iDTSelectEndIndex;

			m_iDTSelectEndIndex = m_iDTSelectStartIndex;
			m_iDTSelectStartIndex = iTemp;
		}
	}
	else if(itemList.size() > 1)
	{
		m_iDTSelectStartIndex = m_setup.DateTimeListWidget->row(itemList.at(0));
		m_iDTSelectEndIndex = m_setup.DateTimeListWidget->row(itemList.at(itemList.count() - 1));
		if(m_iDTSelectEndIndex < m_iDTSelectStartIndex)
		{
			int iTemp = m_iDTSelectEndIndex;

			m_iDTSelectEndIndex = m_iDTSelectStartIndex;
			m_iDTSelectStartIndex = iTemp;
		}
	}
	else
	{
		UpdateFlightPath(0);
		return;
	}

	UpdateFlightPath(1);
}

void FlightAndRasterWindow::UpdateFlightPath(int iState)
{
	if(iState)
	{
		m_FlightPathControler->SetFlightPathPartColor(osg::Vec4(1.0, 0.0, 0.0, 1.0), 0, m_FlightPathControler->GetFlightPath()->size());
		m_FlightPathControler->SetFlightPathPartColor(osg::Vec4(0.0, 0.0, 1.0, 1.0), m_iDTSelectStartIndex, m_iDTSelectEndIndex - m_iDTSelectStartIndex + 1);
	}
	else
	{
		m_FlightPathControler->SetFlightPathPartColor(osg::Vec4(1.0, 0.0, 0.0, 1.0), 0, m_FlightPathControler->GetFlightPath()->size());
	}
}
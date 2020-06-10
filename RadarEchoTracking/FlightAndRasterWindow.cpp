#include "StdAfx.h"
#include "FlightAndRasterWindow.h"
#include <qdatetime.h>
#include "MainWindow.h"
#include "../FlightPathProvider/FlightPathControler.h"

FlightAndRasterWindow::FlightAndRasterWindow(MainWindow* pM)
	: m_bMatched(false), m_funOpenRasterFile(0), m_FlightPathControler(0), m_pControlorInterface(0)
	, m_funGetPlotWidget(0), m_funReplotData(0), m_pMainWindow(pM)
{
	m_setup.setupUi(this);
	this->setWindowIcon(QIcon("./icon/diagram.png"));
	connect(m_setup.pushButton_2, SIGNAL(clicked(bool)), SLOT(slot_MatchData(bool)));
	connect(m_setup.pushButton_3, SIGNAL(clicked(bool)), SLOT(slot_ClearMatch(bool)));
	connect(m_setup.pushButton, SIGNAL(clicked(bool)), SLOT(slot_DrawRaster(bool)));
	connect(m_setup.DateTimeListWidget, SIGNAL(itemSelectionChanged()), SLOT(slot_itemSelectionChanged()));
	connect(m_setup.pushButton_4, SIGNAL(clicked(bool)), SLOT(slot_LoadFlightPath(bool)));

	connect(m_setup.pushButton_5, SIGNAL(clicked(bool)), SLOT(slot_MatchFixPointData(bool)));
	connect(m_setup.pushButton_6, SIGNAL(clicked(bool)), SLOT(slot_ClearFixPointMatch(bool)));
	connect(m_setup.pushButton_7, SIGNAL(clicked(bool)), SLOT(slot_DrawFixPointRaster(bool)));

	//connect(m_setup.
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
	QString strDTFormat = QString::fromLocal8Bit("yyyy/MM/ddhh:mm:ss");
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
	QString strDTFormat = QString::fromLocal8Bit("yyyy/MM/dd-HH:mm:ss");
	int ii = m_iDTSelectStartIndex;
	//QDateTime startDT;
	QDateTime endDT;
	bool bIsSet = true;
	for(int i = 0; i < m_MatchFileList.size(); i++)
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
				QString strCurrentText = currentDT.toString(strDTFormat) + "(" + m_MatchFileList[i] + ")";
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
	md.fXInterval = 360;
	
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
	QMap<QString, osg::ref_ptr<osg::Vec3Array>> dataMap;
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
			QMap<QString, osg::ref_ptr<::Vec3Array>>::iterator finded = dataMap.find(strName);
			if(finded != dataMap.end())
			{
				finded.value()->push_back(currentVec3);
			}
			else
			{
				osg::ref_ptr<osg::Vec3Array> pVec3Array = new osg::Vec3Array();
				pVec3Array->push_back(currentVec3);
				dataMap.insert(strName, pVec3Array);
			}
		}
		
	}

	//从NC文件中取出一个点20层的数据
	QMap<QString, osg::ref_ptr<osg::Vec3Array>>::iterator iterBegin = dataMap.begin();
	
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
	m_MatchFileList.clear();

	QMap<QString, QString>::iterator start = m_FileBaseMapToFilePath->find(fileNames.at(0));
	QMap<QString, QString>::iterator end = m_FileBaseMapToFilePath->end();//m_FileBaseMapToFilePath->find(fileNames.at(1));

	while(start != end)
	{
		m_MatchFileList.push_back(start.key());
		start++;
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
	if(!m_FlightPathControler) return;

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

void FlightAndRasterWindow::slot_LoadFlightPath(bool)
{
	QString strFileName = QFileDialog::getOpenFileName(this
			, QString::fromLocal8Bit("打开飞行轨迹"), "", "JPEG (*.csv)");
	if(strFileName.size() > 0)
	{
		m_FlightPathControler = m_pMainWindow->LoadFlightPath(strFileName);
		m_pControlorInterface = m_pMainWindow->GetControlorInterface();
		if(m_FlightPathControler && m_pControlorInterface)
		{
			FillList();
		}
	}
}

void FlightAndRasterWindow::slot_MatchFixPointData(bool)
{
	m_setup.DateTimeListWidget->clear();
	//QString strLonLat = "(" + m_setup.lineEdit->text() + "," + m_setup.lineEdit_2->text() + ")";
	for(int i = 0; i < m_MatchFileList.size(); i++)
	{
		m_setup.DateTimeListWidget->addItem("(" + m_MatchFileList[i] + ")");
	}

	m_iDTSelectStartIndex = 0;
	m_iDTSelectEndIndex = m_setup.DateTimeListWidget->count() - 1;

	m_pControlorInterface = m_pMainWindow->GetControlorInterface();
}

void WriteFile(const QString& strFile, double* data, int rows, int cols)
{
	QFile writeFile(strFile);

	if(writeFile.open(QFile::WriteOnly | QFile::Truncate))
	{
		QTextStream textSteam(&writeFile);
		for (int j = 0; j < cols; j++)
		{
			for (int i = 0; i < rows; i++)
			{
				QString outText = QString("%1     ").arg(*(data + j * rows + i));
				textSteam<<outText;
			
			}
			textSteam<<endl;
		}
		writeFile.close();
	}
}

template <typename DT>
void setValue(DT* data, const DT& tV, size_t size)
{
	for(int i = 0; i < size; i ++)
	{
		*(data + i) = tV;
	}
}

void FlightAndRasterWindow::slot_DrawFixPointRaster(bool)
{
	int DTCount = m_iDTSelectEndIndex - m_iDTSelectStartIndex + 1;
	typedef osg::Vec3Array* ARRAY_POINTER;
	typedef QMap<QString, ARRAY_POINTER> FILE_MAP;
	int iLayerCount = 20;
	MemoryData md;
	md.fXMin = 0;
	md.fXMax = DTCount;
	md.fXInterval = 1;
	
	md.fYMin = 0;
	md.fYMax = 19;
	md.fYInterval = 1;

	md.fZMin = -45;
	md.fZMax = 30;
	md.fZInterval = 5;

	
	md.DataSize = DTCount * iLayerCount;
	md.ColumnCount = DTCount;
	md.pData = new double[md.DataSize];
	memset(md.pData, 100, sizeof(double) * md.DataSize);
	
	FILE_MAP dataMap;
	for(int i = m_iDTSelectStartIndex; i < DTCount; i++)
	{
		QListWidgetItem* item = m_setup.DateTimeListWidget->item(i);
		QString strText = item->text();

		int iIndex1 = strText.indexOf("(");
		int iIndex2 = strText.indexOf(")");
		QString strName = strText.mid(iIndex1 + 1, iIndex2 - iIndex1 - 1);

		double dx, dy;
		dx = m_setup.lineEdit->text().toDouble();
		dy = m_setup.lineEdit_2->text().toDouble();
		osg::Vec3 currentVec3(dx, dy, 0);

		if(strName.length() > 0)
		{
			//
			ARRAY_POINTER pVec3Array = new osg::Vec3Array();
			pVec3Array->push_back(currentVec3);
			dataMap.insert(strName, pVec3Array);
		}
		
	}

	//从NC文件中取出一个点20层的数据
	FILE_MAP::iterator iterBegin = dataMap.begin();
	
	size_t iSize = 0;
	double* pCurrentPointer = md.pData;
	while(iterBegin != dataMap.end())
	{
		QString strFileFullPathName = m_FileBaseMapToFilePath->value(iterBegin.key(), "");
		
		ARRAY_POINTER pArray = iterBegin.value();
		
		m_pControlorInterface->GetVerticalData(strFileFullPathName.toStdString(), *pArray, osg::Vec3(), pCurrentPointer, &iSize);

		//累加缓冲中的地址，累加迭代器
		pCurrentPointer += pArray->size() * iLayerCount;

		iterBegin++;
	}
	
	//线性插值
#if 1
	int iScale = 5;
	double* oldData = md.pData;
	int newiDTCount = DTCount * iScale;
	int newiLayerCount = iLayerCount * iScale;

	md.DataSize = newiDTCount * newiLayerCount;
	md.ColumnCount = newiDTCount;
	md.pData = new double[md.DataSize];
	//memset(md.pData, 0, sizeof(double) * md.DataSize);
	setValue<double>(md.pData, 100.00, md.DataSize);

	//WriteFile("E:\\Project\\out3.txt", md.pData, iLayerCount, DTCount);
	LinearInterpolation(oldData, DTCount, iLayerCount, md.pData, newiDTCount, newiLayerCount);
	//WriteFile("E:\\Project\\out2.txt", md.pData, newiLayerCount, newiDTCount);
	delete[] oldData;
	md.fXMin = 0;
	md.fXMax = newiDTCount - 1;
	md.fXInterval = iScale;
	
	md.fYMin = 0;
	md.fYMax = newiLayerCount - 1;
	md.fYInterval = iScale;

	md.fZMin = -45;
	md.fZMax = 30;
	md.fZInterval = 5;
#endif
	m_funOpenRasterFile(0, "RasterData", "./Config/ColorTable.ini", &md);
	m_funReplotData(0);

	delete[] md.pData;
}

void FlightAndRasterWindow::slot_ClearFixPointMatch(bool)
{
	m_setup.DateTimeListWidget->clear();
}

void FlightAndRasterWindow::LinearInterpolation(double* srcBuff, size_t rowsSrc, size_t columnsSrc, double* destBuff, size_t rowsDest, size_t columnsDest)
{
	double scale_y = (double)rowsSrc / rowsDest;
	double scale_x = (double)columnsSrc / columnsDest;

	///////////////////////p0--------------p1//////////////////////////////////////
	////////////////////////|              |///////////////////////////////////////
	////////////////////////|              |///////////////////////////////////////
	////////////////////////|              |///////////////////////////////////////
	////////////////////////p2-------------p3//////////////////////////////////////
	
	for (size_t iy = 0; iy < rowsSrc - 1; iy++)
	{
		for (size_t ix = 0; ix < columnsSrc - 1; ix++)
		{
			double p0 = *(srcBuff + iy * columnsSrc + ix);
			double p2 = *(srcBuff + (iy + 1) * columnsSrc + ix);
			double p3 = *(srcBuff + (iy + 1) * columnsSrc + ix + 1);
			double p1 = *(srcBuff + iy * columnsSrc + ix + 1);

			double ix0 = (double)ix / scale_x;
			double ix1 = (double)(ix + 1) / scale_x;

			double iy0 = (double)iy / scale_y;
			double iy1 = (double)(iy + 1) / scale_y;
			
			double innerCountX = ix1 - ix0;
			double innerCountY = iy1 - iy0;

			size_t moveXStep = (double)1 / scale_x;
			size_t moveYStep = (double)1 / scale_y;

			*(destBuff + (size_t)iy0 * columnsDest + (size_t)ix0) = p0;
			*(destBuff + (size_t)iy0 * columnsDest + (size_t)ix0 + moveXStep) = p1;
			*(destBuff + ((size_t)iy0 + moveYStep) * columnsDest + (size_t)ix0) = p2;
			*(destBuff + ((size_t)iy0 + moveYStep) * columnsDest + (size_t)ix0 + moveXStep) = p3;
			
			for (size_t ixi = 1; ixi < innerCountX; ixi++)
			{
				//ix1 - (ix0 + ixi)
				//(ix0 + ixi) - ix0
				double newXV0 = p0 * (ix1 - (ix0 + ixi)) / innerCountX + p1 * (ixi) / innerCountX;
				double newXV1 = p2 * (ix1 - (ix0 + ixi)) / innerCountX + p3 * ixi / innerCountX;

				*(destBuff + (size_t)iy0 * columnsDest + (size_t)ix0 + ixi) = newXV0;
				*(destBuff + ((size_t)iy0 + moveYStep) * columnsDest + (size_t)ix0 + ixi) = newXV1;

				for (size_t iyi = 1; iyi < innerCountY; iyi++)
				{
					double newYV0 = newXV0 * (iy1 - (iy0 + iyi)) / innerCountY + newXV1 * iyi / innerCountY;

					*(destBuff + ((size_t)iy0 + iyi) * columnsDest + (size_t)ix0 + ixi) = newYV0;
				}

			}

			for (size_t iyi = 1; iyi < innerCountY; iyi++)
			{
				double newYV0 = p0 * (iy1 - (iy0 + iyi)) / innerCountY + p2 * iyi / innerCountY;
				double newYV1 = p1 * (iy1 - (iy0 + iyi)) / innerCountY + p3 * iyi / innerCountY;

				*(destBuff + ((size_t)iy0 + iyi) * columnsDest + (size_t)ix0) = newYV0;
				*(destBuff + ((size_t)iy0 + iyi) * columnsDest + (size_t)ix0 + moveXStep) = newYV1;
			}
		}
	}
	
}
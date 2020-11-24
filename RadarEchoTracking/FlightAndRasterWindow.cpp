#include "StdAfx.h"
#include "FlightAndRasterWindow.h"
#include <qdatetime.h>
#include <QSettings>
#include "MainWindow.h"
#include "../FlightPathProvider/FlightPathControler.h"

#define MAKE_CSV_LINE(v1, v2, v3, v4) v1 + QString::fromLocal8Bit(",") + v2 + QString::fromLocal8Bit(",") + v3 + QString::fromLocal8Bit(",") + v4 + "\n"

template <typename DT>
void setValue(DT* data, const DT& tV, size_t size)
{
	for(size_t i = 0; i < size; i ++)
	{
		*(data + i) = tV;
	}
}

FlightAndRasterWindow::FlightAndRasterWindow(MainWindow* pM)
	: m_bMatched(false), m_funOpenRasterFile(0), m_FlightPathControler(0), m_pControlorInterface(0)
	, m_funGetPlotWidget(0), m_funReplotData(0), m_pMainWindow(pM), m_iDTSelectStartIndex(-1), m_iDTSelectEndIndex(-1)
	, m_pCurrentFixOldData(0), m_pCurrentFlyOldData(0), m_funExportPlotImage(0)
{
	m_setup.setupUi(this);
	m_setup.label->hide();
	m_setup.label_2->hide();
	m_setup.dateTimeEdit->hide();
	m_setup.dateTimeEdit_2->hide();

	this->setWindowIcon(QIcon("./icon/diagram.png"));
	connect(m_setup.pushButton_2, SIGNAL(clicked(bool)), SLOT(slot_MatchData(bool)));
	connect(m_setup.pushButton_3, SIGNAL(clicked(bool)), SLOT(slot_ClearMatch(bool)));
	connect(m_setup.pushButton, SIGNAL(clicked(bool)), SLOT(slot_DrawRaster(bool)));
	connect(m_setup.DateTimeListWidget, SIGNAL(itemSelectionChanged()), SLOT(slot_itemSelectionChanged()));
	connect(m_setup.pushButton_4, SIGNAL(clicked(bool)), SLOT(slot_LoadFlightPath(bool)));

	connect(m_setup.pushButton_5, SIGNAL(clicked(bool)), SLOT(slot_MatchFixPointData(bool)));
	connect(m_setup.pushButton_6, SIGNAL(clicked(bool)), SLOT(slot_ClearFixPointMatch(bool)));
	connect(m_setup.pushButton_7, SIGNAL(clicked(bool)), SLOT(slot_DrawFixPointRaster(bool)));

	connect(m_setup.pushButton_8, SIGNAL(clicked(bool)), SLOT(slot_exportDrawFixRaster(bool)));
	connect(m_setup.pushButton_9, SIGNAL(clicked(bool)), SLOT(slot_ExportDrawFlightRaster(bool)));
	connect(m_setup.pushButton_10, SIGNAL(clicked(bool)), SLOT(slot_LoadFlightPathAndAutoMatch(bool)));

	connect(m_setup.checkBox, SIGNAL(clicked(bool)), SLOT(slot_checkboxUpdate(bool)));
	connect(m_setup.lineEdit, SIGNAL(editingFinished()), SLOT(slot_lineEditUpdate()));
	connect(m_setup.lineEdit_2, SIGNAL(editingFinished()), SLOT(slot_lineEdit_2Update()));
	connect(m_setup.toolBox, SIGNAL(currentChanged(int)), SLOT(slot_toolboxUpdate(int)));
	connect(m_setup.pushButton_11, SIGNAL(clicked(bool)), SLOT(slot_ExportPlotToImage(bool)));
	//connect(m_setup.
	QLibrary plotLib("./QwtRasterPolt");

	if(plotLib.load())
	{
		m_funOpenRasterFile = (FUNDEF_OpenRasterFile)plotLib.resolve("OpenRasterFile");
		m_funGetPlotWidget = (FUNDEF_GetPlotWidget)plotLib.resolve("GetPlotWidget");
		m_funReplotData = (FUNDEF_ReplotData)plotLib.resolve("ReplotData");
		m_funClearPlot = (FUNDEF_ClearPlot)plotLib.resolve("ClearPlot");
		m_funDrawCurve = (FUNDEF_DrawCurve)plotLib.resolve("DrawCurve");
		m_funExportPlotImage = (FUNDEF_ExportPlotImage)plotLib.resolve("ExportPlotImage");
		QWidget* pPlotWidget = m_funGetPlotWidget();
		m_setup.gridLayout->addWidget(pPlotWidget, 1, 2, 6, 1);
	}

	QSettings configIni("conifg_flight_raster.ini", QSettings::IniFormat);

	configIni.beginGroup("UI_CONIFIG");
	m_setup.checkBox->setChecked(configIni.value("m_setup.checkBox", true).toBool());
	m_setup.toolBox->setCurrentIndex(configIni.value("m_setup.toolBox", 1).toInt());
	m_setup.lineEdit->setText(configIni.value("m_setup.lineEdit", "115.73").toString());
	m_setup.lineEdit_2->setText(configIni.value("m_setup.lineEdit_2", "40.52").toString());
	configIni.endGroup();
}

FlightAndRasterWindow::~FlightAndRasterWindow(void)
{
}

void FlightAndRasterWindow::FillList()
{
	m_setup.DateTimeListWidget->clear();

	if(m_FlightPathControler == NULL)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绘制剖面时发生错误"), QString::fromLocal8Bit("没有加载任何轨迹文件，无法绘制剖面图。"));
		return;
	}

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
	if(m_pCurrentFlyOldData)
	{
		delete[] m_pCurrentFlyOldData;
		m_pCurrentFlyOldData = NULL;
	}
	FillList();
	m_bMatched = false;
}

void FlightAndRasterWindow::slot_MatchData(bool)
{
	if(m_setup.DateTimeListWidget->count() <= 0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("匹配数据发生错误"), QString::fromLocal8Bit("没有加载任何轨迹文件，无法绘制剖面图。"));
		return;
	}
	if(m_bMatched) return;
	QString strDTFormat = QString::fromLocal8Bit("yyyy/MM/dd-H:mm:ss");
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
			if(!currentDT.isValid())
			{
				QMessageBox::warning(this, QString::fromLocal8Bit("匹配数据发生错误"), QString::fromLocal8Bit("轨迹文件的时间格式有错误,正确格式:2020/01/01-1:01:01"));
				return;
			}
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

void GetColorTableValue(const QString& strFile, double* dMax, double* dMix, size_t* uCount)
{
	QSettings configIniWrite(strFile, QSettings::IniFormat);
	int iCount = configIniWrite.value("VALUE_COUNT/count").toInt();
	//int iType = configIniWrite.value("TYPE/section").toInt();
	*uCount = iCount;
	QString strPath = "COLOR_MAP/pair" + QString("%1").arg(0 + 1);
	QString strPair = configIniWrite.value(strPath).toString();
	QStringList strList = strPair.split(':');
	if(strList.size() == 2)//
	{
		*dMix = strList[0].toDouble();
	}

	strPath = "COLOR_MAP/pair" + QString("%1").arg(iCount);
	strPair = configIniWrite.value(strPath).toString();
	strList = strPair.split(':');
	if(strList.size() == 2)
	{
		*dMax = strList[0].toDouble();
	}
}

void FlightAndRasterWindow::LinearInterpolation1D(double* srcBuff, size_t countSrc, double* destBuff, size_t countDest)
{
	double scale_x = (double)countSrc / countDest;

	for (size_t ix = 0; ix < countSrc - 1; ix++)
	{
		double p0 = *(srcBuff + ix);
		double p1 = *(srcBuff + ix + 1);

		double ix0 = (double)ix / scale_x;
		double ix1 = (double)(ix + 1) / scale_x;

	
		double innerCountX = ix1 - ix0;

		size_t moveXStep = (double)1 / scale_x;

		*(destBuff + (size_t)ix0) = p0;
		*(destBuff + (size_t)ix0 + moveXStep) = p1;
		

		for (size_t ixi = 1; ixi < innerCountX; ixi++)
		{
			double newXV0 = p0 * (ix1 - (ix0 + ixi)) / innerCountX + p1 * (ixi) / innerCountX;
			*(destBuff + (size_t)ix0 + ixi) = newXV0;
		}
	}
}

void FlightAndRasterWindow::slot_DrawRaster(bool)
{
	static QWidget* pOldWidet = NULL;
	QString strColorTableFile = "./Config/ColorTable.ini";
	if(m_FlightPathControler == NULL || m_MatchFileList.size() == 0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绘制剖面错误发生"), QString::fromLocal8Bit("没有加载任何轨迹文件，无法绘制剖面图。"));
		return;
	}

	if(m_pCurrentFlyOldData)
	{
		delete[] m_pCurrentFlyOldData;
		m_pCurrentFlyOldData = 0;
	}
	QString strTtile = QString::fromLocal8Bit("飞行轨迹剖面图");
	QString strFirstFile = m_MatchFileList.at(0);

	QListWidgetItem* item = m_setup.DateTimeListWidget->item(m_iDTSelectStartIndex);
	QString strText = item->text();
	int iIndex1 = strText.indexOf("(");
	QString strName = strText.left(iIndex1);

	QString strDTFormat = QString::fromLocal8Bit("yyyy/MM/dd-H:mm:ss");
	QDateTime currentDT = QDateTime::fromString(strName, strDTFormat);
	QString strTest = currentDT.toString(strDTFormat);

	int iLayerCount = 20;
	MemoryData md;
	md.TimeInterval = 1;
	md.StartDateTime = currentDT;//QDateTime::fromString(strFirstFile, "yyyyMMddhhmmss");

	md.fXMin = 0;
	md.fXMax = m_iDTSelectEndIndex - m_iDTSelectStartIndex;
	md.fXInterval = 360;
	
	md.fYMin = 500;
	md.fYMax = 19000;
	md.fYInterval = 1000;
	md.HeightScale = 1;

	size_t uCount;
	GetColorTableValue(strColorTableFile, &md.fZMax, &md.fZMin, &uCount);
	
	md.fZInterval = (md.fZMax - md.fZMin) / (uCount - 1);

	md.strXComment = strFirstFile.toStdString();
	md.strYComment = QString("(KM)").toStdString();

	int DTCount = m_iDTSelectEndIndex - m_iDTSelectStartIndex + 1;
	md.DataSize = DTCount * iLayerCount;
	md.ColumnCount = DTCount;
	md.pData = new double[md.DataSize];

	m_FlyDataCols = md.fXMax + 1;
	m_FlyDataRows = md.DataSize/m_FlyDataCols;

	setValue<double>(md.pData, 100.00, md.DataSize);
	//使用影射将坐标归类到对应的文件中
	QMap<QString, osg::ref_ptr<osg::Vec3Array>> dataMap;
	QVector<QPointF> lineData;
	for(int ii = 0; ii < DTCount; ii++)
	{
		int i = m_iDTSelectStartIndex + ii;
		QListWidgetItem* item = m_setup.DateTimeListWidget->item(i);
		QString strText = item->text();

		int iIndex1 = strText.indexOf("(");
		int iIndex2 = strText.indexOf(")");
		QString strName = strText.mid(iIndex1 + 1, iIndex2 - iIndex1 - 1);

		osg::Vec3 currentVec3 = m_FlightPathControler->GetFlightPath()->at(i);

		lineData.push_back(QPointF(i, currentVec3.z()));
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

	//线性插值
	if(true)
	{
		int iScale = 8;
		//插值飞行轨迹
		size_t newSize = lineData.size() * iScale;
		double* lineDestData = new double[newSize];
		memset(lineDestData, 0, sizeof(double) * newSize);

		double* lineSoucData = new double[lineData.size()];
		memset(lineDestData, 0, sizeof(double) * lineData.size());
		for(int i = 0; i < lineData.size(); i++)
		{
			*(lineSoucData + i) = lineData.at(i).y();
		}

		LinearInterpolation1D(lineSoucData, lineData.size(), lineDestData, newSize);

		delete[] lineSoucData;
		lineData.clear();
		for(size_t i = 0; i < newSize; i++)
		{
			double dV = *(lineDestData + i) * iScale;
			lineData.push_back(QPointF(i, dV));
		}
		delete[] lineDestData;
		//插值raster数据
		m_pCurrentFlyOldData = md.pData;
		int newiDTCount = DTCount * iScale;
		int newiLayerCount = iLayerCount * iScale;

		md.DataSize = newiDTCount * newiLayerCount;
		md.ColumnCount = newiDTCount;
		md.pData = new double[md.DataSize];
		//memset(md.pData, 0, sizeof(double) * md.DataSize);
		setValue<double>(md.pData, 100.00, md.DataSize);

		//WriteFile("E:\\Project\\out3.txt", md.pData, iLayerCount, DTCount);
		LinearInterpolation(m_pCurrentFlyOldData, DTCount, iLayerCount, md.pData, newiDTCount, newiLayerCount);
		//WriteFile("E:\\Project\\out2.txt", md.pData, newiLayerCount, newiDTCount);
		//delete[] oldData;
		md.TimeInterval = (double)1 / iScale;

		md.fXMin = 0;
		md.fXMax = newiDTCount - 1;
		md.fXInterval = iScale * md.fXInterval;
	
		md.fYMin = md.fYMin * iScale;
		md.fYMax = md.fYMax * iScale;
		md.fYInterval = md.fYInterval * iScale;
		md.HeightScale = (double)1 / iScale;

		//md.fZMin = -45;
		//md.fZMax = 30;
		//md.fZInterval = 5;
	}

	//m_pCurrentFlyRasterData = md.pData;
	
	m_funClearPlot(0);
	m_funDrawCurve(0, lineData);
	//if(pOldWidet) delete pOldWidet;
	pOldWidet = m_funOpenRasterFile(0, QString::fromLocal8Bit("飞行轨迹剖面"), strColorTableFile, &md);
	m_funReplotData(0);

	delete[] md.pData;
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
			, QString::fromLocal8Bit("打开飞行轨迹"), "", "csv (*.csv)");
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

void FlightAndRasterWindow::slot_LoadFlightPathAndAutoMatch(bool)
{
	//slot_LoadFlightPath(true);
	QString strDTFormat1 = QString::fromLocal8Bit("yyyy/MM/dd-H:mm:ss");
	QString strDTFormat2 = QString::fromLocal8Bit("yyyyMMddHHmmss");

	if(m_pFileList->size() > 0 && m_setup.DateTimeListWidget->count() > 0)
	{
		QString strFileDateTime = m_pFileList->at(0);
		QString strPathDateTime = m_setup.DateTimeListWidget->item(0)->text();
		
		QDateTime t1 = QDateTime::fromString(strPathDateTime, strDTFormat1);//路径时间
		QDateTime t2 = QDateTime::fromString(strFileDateTime, strDTFormat2);//文件时间

		//t2 = t2.addSecs(3600 * 8);

		int iProperIndex1 = -1;
		int iProperIndex2 = -1;
		int iPathPointCount = m_setup.DateTimeListWidget->count();
		if(t1 > t2)
		{
			for(int i = 0; i < m_pFileList->size(); i++)
			{
				QDateTime currentTime = QDateTime::fromString(m_pFileList->at(i), strDTFormat2);

				qint64 iCurrentDistance = currentTime.secsTo(t1);

				qDebug()<<"Path Time"<<t1<<endl;
				qDebug()<<"File Time"<<currentTime<<endl;
				
				if(iCurrentDistance <= 0)
				{
					if(iCurrentDistance < 0 && i != 0)
					{
						iProperIndex2 = i - 1;
					}
					else
					{
						iProperIndex2 = i;
					}
					
					break;
				}
			}
		}
		else
		{
			for(int i = 0; i < iPathPointCount; i++)
			{
				QString strText = m_setup.DateTimeListWidget->item(i)->text();
				int iIndex1 = strText.indexOf("(");
				QString strName = strText.left(iIndex1);
				QDateTime currentTime = QDateTime::fromString(strName, strDTFormat1);

				qDebug()<<"Path Time"<<t2<<endl;
				qDebug()<<"File Time"<<currentTime<<endl;
				
				qint64 iCurrentDistance = currentTime.secsTo(t2);

				if(iCurrentDistance <= 0)
				{
					if(iCurrentDistance < 0 && i != 0)
					{
						iProperIndex1 = i - 1;
					}
					else
					{
						iProperIndex1 = i;
					}
					break;
				}
			
			}
		}

		if(iProperIndex1 < 0 && iProperIndex2 < 0)
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("自动匹配失败"), QString::fromLocal8Bit("已经匹配到了最后一个轨迹点,由于数据时差太大,请使用手动匹配。"));
			return;
		}

		if(iProperIndex1 >= 0)
		{
			m_iDTSelectStartIndex = iProperIndex1;
		}
		else
		{
			m_MatchFileList.clear();
			for(int i = iProperIndex2; i < m_pFileList->size(); i++)
			{
				m_MatchFileList.push_back(m_pFileList->at(i));
			}
		}
		slot_MatchData(false);
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

void FlightAndRasterWindow::WriteFile(const QString& strFile, double* data, int rows, int cols)
{
	QFile writeFile(strFile);

	if(writeFile.open(QFile::WriteOnly | QFile::Truncate))
	{
		QString tempString;
		QTextStream textSteam(&writeFile);
		for (int j = 0; j < cols; j++)
		{
			QString strText = m_setup.DateTimeListWidget->item(m_iDTSelectStartIndex + j)->text();
			
			QString strDateTime = strText.mid(1, strText.length() - 2);
			QDateTime tempDT = QDateTime::fromString(strDateTime, "yyyyMMddhhmmss");
			strDateTime = tempDT.toString("yyyy/MM/dd-HH:mm:ss");
			QString strLon = m_setup.lineEdit->text();
			QString strLat = m_setup.lineEdit_2->text();

			for (int i = 0; i < rows; i++)
			{
				int iHeight = 500 + (i * 1000);
				
				double iVV = *(data + j * rows + i);
				if((int)iVV - 100 == 0)
				{
					iVV = -31.376;
				}
				QString v0("%1"), v1("%1"), v2("%1"), v3("%1"), v4("%1");
						v0 = strDateTime;
						v1 = v1.arg(strLon);
						v2 = v2.arg(strLat);
						v3 = v3.arg(iHeight);
						v4 = v4.arg(iVV);

				QString temp = v0 + "," + v1 + "," + v2 + "," + v3 + "," + v4 + "\n";
				tempString.push_back(temp);
			}
			
			textSteam<<tempString;
			tempString.clear();
		}
		writeFile.close();
	}
}

void FlightAndRasterWindow::slot_DrawFixPointRaster(bool)
{
	QString strColorTableFile = "./Config/ColorTable.ini";
	if(/*m_pCurrentFixRasterData*/m_pCurrentFixOldData)//删除之前的绘制数据,因为涉及到导出数据所以要保存这个指针
	{
		//delete[] m_pCurrentFixRasterData;
		delete[] m_pCurrentFixOldData;
		m_pCurrentFixOldData = NULL;
	}

	QString strTtile = QString::fromLocal8Bit("定点剖面图");
	if(m_MatchFileList.size() == 0 || m_iDTSelectEndIndex < 0 || m_iDTSelectStartIndex < 0 || m_setup.DateTimeListWidget->count() == 0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绘制剖面错误发生"), QString::fromLocal8Bit("没有任何匹配文件数据，无法绘制剖面图。"));
		return;
	}
	int DTCount = m_iDTSelectEndIndex - m_iDTSelectStartIndex + 1;
	typedef osg::Vec3Array* ARRAY_POINTER;
	typedef QMap<QString, ARRAY_POINTER> FILE_MAP;
	int iLayerCount = 20;
	QString strFirstFile = m_MatchFileList.at(0);
	MemoryData md;
	
	md.StartDateTime = QDateTime::fromString(strFirstFile, "yyyyMMddhhmmss");
	md.TimeInterval = 360;

	md.fXMin = 0;
	md.fXMax = DTCount - 1;
	md.fXInterval = 10;
	
	md.fYMin = 500;
	md.fYMax = 19500;
	md.fYInterval = 1000;
	md.HeightScale = 1;

	size_t uCount;
	GetColorTableValue(strColorTableFile, &md.fZMax, &md.fZMin, &uCount);
	
	md.fZInterval = (md.fZMax - md.fZMin) / (uCount - 1);

	md.strXComment = strFirstFile.toStdString();
	md.strYComment = QString("(KM)").toStdString();

	md.DataSize = DTCount * iLayerCount;
	md.ColumnCount = DTCount;
	md.pData = new double[md.DataSize];
	setValue<double>(md.pData, 100.00, md.DataSize);
	
	m_FixDataCols = md.fXMax + 1;
	m_FixDataRows = md.DataSize/m_FixDataCols;

	FILE_MAP dataMap;
	for(int ii = m_iDTSelectStartIndex; ii < DTCount; ii++)
	{
		int i = m_iDTSelectStartIndex + ii;
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
	if(m_setup.checkBox->isChecked())
	{
		int iScale = 5;
		m_pCurrentFixOldData = md.pData;
		int newiDTCount = DTCount * iScale;
		int newiLayerCount = iLayerCount * iScale;

		md.DataSize = newiDTCount * newiLayerCount;
		md.ColumnCount = newiDTCount;
		md.pData = new double[md.DataSize];
		//memset(md.pData, 0, sizeof(double) * md.DataSize);
		setValue<double>(md.pData, 100.00, md.DataSize);

		//WriteFile("E:\\Project\\out3.txt", md.pData, iLayerCount, DTCount);
		LinearInterpolation(m_pCurrentFixOldData, DTCount, iLayerCount, md.pData, newiDTCount, newiLayerCount);
		//WriteFile("E:\\Project\\out2.txt", md.pData, newiLayerCount, newiDTCount);
		//delete[] oldData;
		md.TimeInterval = 360 / iScale;

		md.fXMin = 0;
		md.fXMax = newiDTCount - 1;
		md.fXInterval = iScale * md.fXInterval;
	
		md.fYMin = md.fYMin * iScale;
		md.fYMax = md.fYMax * iScale;
		md.fYInterval = md.fYInterval * iScale;
		md.HeightScale = (double)1 / iScale;

		//md.fZMin = -45;
		//md.fZMax = 30;
		//md.fZInterval = 5;
	}
	//m_pCurrentFixRasterData = md.pData;
	
	

	m_funClearPlot(0);
	m_funOpenRasterFile(0, strTtile, strColorTableFile, &md);
	m_funReplotData(0);

	delete[] md.pData;
}

void FlightAndRasterWindow::slot_ClearFixPointMatch(bool)
{
	if(m_pCurrentFixOldData)
	{
		delete[] m_pCurrentFixOldData;
		m_pCurrentFixOldData = NULL;
	}
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

			//计算X和Y扩大倍数,用于计算以前对应的值在新坐标中是那个坐标
			size_t moveXStep = (double)1 / scale_x;
			size_t moveYStep = (double)1 / scale_y;

			//在新的坐标系总填充这四个点的坐标
			*(destBuff + (size_t)iy0 * columnsDest + (size_t)ix0) = p0;
			*(destBuff + (size_t)iy0 * columnsDest + (size_t)ix0 + moveXStep) = p1;
			*(destBuff + ((size_t)iy0 + moveYStep) * columnsDest + (size_t)ix0) = p2;
			*(destBuff + ((size_t)iy0 + moveYStep) * columnsDest + (size_t)ix0 + moveXStep) = p3;
			
			//双线性插值算法开始
			for (size_t ixi = 1; ixi < innerCountX; ixi++)
			{
				//ix1 - (ix0 + ixi)
				//(ix0 + ixi) - ix0
				//计算X上面的p0和p1中间一个点的值,有个这个点取决于决定被扩大了几倍
				////////////////////////////////////////////////////////////////////////////////
				/////////////////////p0---newXV0-----p1/////////////////////////////////////
				//////////////////////|              |//////////////////////////////////////
				//////////////////////|              |//////////////////////////////////////
				//////////////////////|              |//////////////////////////////////////
				//////////////////////p2--newXV1-----p3/////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////
				double newXV0 = p0 * (ix1 - (ix0 + ixi)) / innerCountX + p1 * (ixi) / innerCountX;
				double newXV1 = p2 * (ix1 - (ix0 + ixi)) / innerCountX + p3 * ixi / innerCountX;

				*(destBuff + (size_t)iy0 * columnsDest + (size_t)ix0 + ixi) = newXV0;
				*(destBuff + ((size_t)iy0 + moveYStep) * columnsDest + (size_t)ix0 + ixi) = newXV1;

				//从X坐标上计算到的两个点,计算出一个中间点的值
				////////////////////////////////////////////////////////////////////////
				/////////////////////p0---newXV0-----p1/////////////////////////////////////
				//////////////////////|     |        |//////////////////////////////////////
				//////////////////////|    newYV0    |//////////////////////////////////////
				//////////////////////|     |        |//////////////////////////////////////
				//////////////////////p2--newXV1-----p3/////////////////////////////////////
				for (size_t iyi = 1; iyi < innerCountY; iyi++)
				{
					double newYV0 = newXV0 * (iy1 - (iy0 + iyi)) / innerCountY + newXV1 * iyi / innerCountY;

					*(destBuff + ((size_t)iy0 + iyi) * columnsDest + (size_t)ix0 + ixi) = newYV0;
				}

			}

			////////////////////计算Y方向边框上的两个点/////////////////////////////////
			/////////////////////p0---newXV0-----p1/////////////////////////////////////
			//////////////////////|              |//////////////////////////////////////
			///////////////////newYV0         newYV1////////////////////////////////////
			//////////////////////|              |//////////////////////////////////////
			//////////////////////p2--newXV1-----p3/////////////////////////////////////
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

void FlightAndRasterWindow::slot_ExportDrawFlightRaster(bool)
{
	if(!m_pCurrentFlyOldData)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("导出剖面数据"), QString::fromLocal8Bit("没有任何匹配文件数据，无法导出剖面数据。"));

		return;
	}
	QString strFileName = QFileDialog::getSaveFileName(this
			, QString::fromLocal8Bit("导出雷达数据"), "", "csv (*.csv)");

	QFile writeFile(strFileName);

	if(writeFile.open(QFile::WriteOnly | QFile::Truncate))
	{
		QTextStream textSteam(&writeFile);
		QString tempString;
		for(size_t i = m_iDTSelectStartIndex; i < m_FlyDataCols; i++)
		{
			/*if(i >= m_FlyDataCols - 2)
			{
				qDebug()<<"Break Point."<<endl;
			}*/
			//读取轨迹的时间
			QString strText = m_setup.DateTimeListWidget->item(i)->text();
			int iIndex1 = strText.indexOf("(");
			QString strDateTime = strText.left(iIndex1);

			osg::Vec3 currentVec3 = m_FlightPathControler->GetFlightPath()->at(i);
			double* pData = m_pCurrentFlyOldData + i * m_FlyDataRows;
			for(size_t j = 0; j <  m_FlyDataRows; j++)
			{
				int iStartHeight = 500 + (j * 1000);

				double iVV = *(pData + j);
				if((int)iVV - 100 == 0)
				{
					iVV = -31.376;
				}
				QString v0("%1"), v1("%1"), v2("%1"), v3("%1"), v4("%1"), v5("%1");
						v0 = strDateTime;
						v1 = v1.arg(currentVec3.x(), 0, 'f', 2);
						v2 = v2.arg(currentVec3.y(), 0, 'f', 2);
						v3 = v3.arg((int)iStartHeight);
						v4 = v4.arg(iVV);
						v5 = v5.arg(currentVec3.z());

				QString temp = v0 + "," + v1 + "," + v2 + "," + v3 + "," + v5 + "," + v4 + "\n";
				tempString.push_back(temp);
				//textSteam<<currentVec3.x()<<","<<currentVec3.y()<<","<<iStartHeight<<","<<*pData<<endl;
			}
			
		}

		textSteam<<tempString;
		writeFile.close();
	}

	
}

void FlightAndRasterWindow::slot_exportDrawFixRaster(bool)
{
	QString strFileName = QFileDialog::getSaveFileName(this
			, QString::fromLocal8Bit("导出雷达数据"), "", "csv (*.csv)");

	if(strFileName.size() > 0)
	{
		WriteFile(strFileName, m_pCurrentFixOldData, m_FixDataRows, m_FixDataCols);
	}		
}

void FlightAndRasterWindow::AutoSaveConfig()
{
	QSettings configIni("conifg_flight_raster.ini", QSettings::IniFormat);

	configIni.beginGroup("UI_CONIFIG");
	configIni.setValue("m_setup.checkBox", m_setup.checkBox->isChecked());
	configIni.setValue("m_setup.toolBox", m_setup.toolBox->currentIndex());
	configIni.setValue("m_setup.lineEdit", m_setup.lineEdit->text());
	configIni.setValue("m_setup.lineEdit_2", m_setup.lineEdit_2->text());
	configIni.endGroup();

}

void FlightAndRasterWindow::slot_checkboxUpdate(bool)
{
	AutoSaveConfig();
}

void FlightAndRasterWindow::slot_lineEditUpdate()//editingFinished()
{
	AutoSaveConfig();
}

void FlightAndRasterWindow::slot_lineEdit_2Update()//editingFinished()
{
	AutoSaveConfig();
}

void FlightAndRasterWindow::slot_toolboxUpdate(int)//currentChanged(int index)
{
	AutoSaveConfig();
}

void FlightAndRasterWindow::SaveData(double* pData, size_t rows, size_t cols)
{

}

void FlightAndRasterWindow::slot_ExportPlotToImage(bool)
{
	/*QString strFileName = QFileDialog::getSaveFileName(this
			, QString::fromLocal8Bit("导出图像"), "", "jpg (*.jpg)");
	if(strFileName.size() > 0)
	{
		
	}*/
	m_funExportPlotImage(0, "");
}
// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <qwidget.h>
#include "ColorBar.h"
#include "RadarColorMap.h"
#include "RasterPlot.h"
#include "RadarRasterData.h"
#include "ConfigurableColorBar.h"
#include "MemoryData.h"
#include "ParseMemoryRasterData.h"

QVector<RasterPlot*> g_QwtPlotVector;
QWidget* GetPlotWidget()
{
	RasterPlot* plot = new RasterPlot();
	g_QwtPlotVector.push_back(plot);
	return plot;
}

RasterPlot* GetRasterPlot(int iIndex)
{
	RasterPlot* plot = NULL;
	if(g_QwtPlotVector.size() > iIndex && iIndex >= 0)
	{
		plot = g_QwtPlotVector[iIndex];
	}
	else
	{
		plot = new RasterPlot();
		g_QwtPlotVector.push_back(plot);
	}

	return plot;
}

void ReplotData(int iIndex)
{
	RasterPlot* plot = GetRasterPlot(iIndex);
	if(plot)
	{
		plot->replot();
	}
}

QWidget* OpenRasterFile(int iIndex, const char* strTitle, const char* fileName, MemoryData* pGMD)
{
	QString colorTableFile = QString::fromLocal8Bit(fileName);

	if (colorTableFile.size() == 0)
	{
		return 0;
	}

	//QMap<QString, QString> KeyValues = m_uiParameter.value(suffixName);
	//QString strFontName = KeyValues.value("FontName");
	QFont globleFont(QString::fromLocal8Bit("ËÎÌו"));

	RadarColorMap* RCM = new RadarColorMap;
	ConfigurableColorBar ccb;
	ccb.LoadData(colorTableFile.toStdString());
	QString strRightName = "strRightName";
	ParseMemoryRasterData pmrd;
	pmrd.ReadDataFromMemory(pGMD);

	if (ccb.GetBarStyle() == ColorBar::LINEAR)
	{
		RasterPlot* plot = GetRasterPlot(iIndex);
		plot->setFont(globleFont);
		plot->canvas()->setFont(globleFont);
		QwtColorMap* pLinearColorMap = RCM->Make(&ccb);
		QwtColorMap* pLinearColorMap2 = RCM->Make(&ccb);
		RadarRasterData* pRRD = new RadarRasterData();
		pRRD->SetFillValue(RCM->GetMin() + RCM->GetSetp() / 2);
		pRRD->SetMaxValue(RCM->GetMax());

		//ParseRasterFile rrf;
		//rrf.m_strExpression = m_expression.value(suffixName);
		//rrf.ReadDataFromFile(fileName.toStdString());
		//rrf.SetAxisProperty(0, 150, 0, 15, RCM->GetMin(), RCM->GetMax());
		//plot->SetStationName(rrf.m_strStationName);
		plot->SetValueSetp(RCM->GetSetp());
		pRRD->Make(&pmrd);

		int isHideLable = 0;
		if (isHideLable > 0)
		{
			plot->SetHideLable(RCM->GetMin());
		}
		
		std::string strXComment = "strXComment";
		std::string strYComment = "strYComment";
		std::string strZComment = "strZComment";
		//rrf.GetAxisComment(strXComment, strYComment, strZComment);

		QDateTime dt = QDateTime::fromString(QString::fromStdString(strXComment), "yyyyMMddhhmmsszzz");
		QwtText topTitle, leftTitle, rightTitle, bottomTitle;
		topTitle.setText(strTitle); topTitle.setFont(globleFont);
		leftTitle.setText(QString::fromStdString(strYComment)); leftTitle.setFont(globleFont);
		rightTitle.setText(strRightName); rightTitle.setFont(globleFont);
		bottomTitle.setText(QString::fromLocal8Bit("NAQU-") + dt.date().toString("yyyyMMdd")); bottomTitle.setFont(globleFont);
		
		plot->setTitle(strTitle);
		plot->SetStartDateTime(dt);

		plot->setAxisTitle(QwtPlot::xBottom, bottomTitle);
		plot->setAxisTitle(QwtPlot::yLeft, leftTitle);
		plot->setAxisTitle(QwtPlot::yRight, rightTitle);

		plot->SetColorMap(pLinearColorMap);
		plot->SetAxisColorMap(pLinearColorMap2);
		plot->SetRasterData(pRRD);

		plot->DrawPlot();

		return plot;
	}
	else
	{
		RasterPlot* plot = GetRasterPlot(iIndex);

		plot->setFont(globleFont);
		plot->canvas()->setFont(globleFont);

		QwtColorMap* pLinearColorMap = RCM->Make(&ccb);
		QwtColorMap* pLinearColorMap2 = RCM->Make(&ccb);
		RadarRasterData* pRRD = new RadarRasterData();
		pRRD->SetFillValue(RCM->GetMin() - 3);
		pRRD->SetMaxValue(RCM->GetMax());

		//ParseRasterFile rrf;
		//rrf.m_strExpression = m_expression.value(suffixName);
		//rrf.ReadDataFromFile(fileName.toStdString());
		//rrf.SetAxisProperty(0, 150, 0, 15, RCM->GetMin(), RCM->GetMax());
		//plot->SetStationName(rrf.m_strStationName);
		plot->SetValueSetp(RCM->GetSetp());
		pRRD->Make(&pmrd);

		std::string strXComment = "strXComment";
		std::string strYComment = "strYComment";
		std::string strZComment = "strZComment";
		//rrf.GetAxisComment(strXComment, strYComment, strZComment);
		
		QDateTime dt = QDateTime::fromString(QString::fromStdString(strXComment), "yyyyMMddhhmmsszzz");
		QwtText topTitle, leftTitle, rightTitle, bottomTitle;
		topTitle.setText(strTitle); topTitle.setFont(globleFont);
		leftTitle.setText(QString::fromStdString(strYComment)); leftTitle.setFont(globleFont);
		rightTitle.setText(strRightName); rightTitle.setFont(globleFont);
		bottomTitle.setText(QString::fromLocal8Bit("NAQU-") + dt.date().toString("yyyyMMdd")); bottomTitle.setFont(globleFont);

		plot->setTitle(strTitle);
		plot->SetStartDateTime(dt);
		
		plot->setAxisTitle(QwtPlot::xBottom, bottomTitle);
		plot->setAxisTitle(QwtPlot::yLeft, leftTitle);
		plot->setAxisTitle(QwtPlot::yRight, rightTitle);

		plot->SetColorMap(pLinearColorMap);
		plot->SetAxisColorMap(pLinearColorMap2);
		plot->SetRasterData(pRRD);

		plot->DrawPlot();
		plot->enableAxis(QwtPlot::yRight, false);

		
		return plot;
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


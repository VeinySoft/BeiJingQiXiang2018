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

QWidget* OpenRasterFile(int iIndex, const QString& strTitle, const QString& fileName, MemoryData* pGMD)
{
	QString colorTableFile = fileName;

	if (colorTableFile.size() == 0)
	{
		return 0;
	}

	//QMap<QString, QString> KeyValues = m_uiParameter.value(suffixName);
	//QString strFontName = KeyValues.value("FontName");
	QFont globleFont(QString::fromLocal8Bit("ËÎÌå"));

	RadarColorMap* RCM = new RadarColorMap;
	ConfigurableColorBar ccb;
	ccb.LoadData(colorTableFile.toStdString());
	QString strRightName = "strRightName";
	ParseMemoryRasterData pmrd;
	pmrd.ReadDataFromMemory(pGMD);

	if (ccb.GetBarStyle() == ColorBar::LINEAR)
	{
		RasterPlot* plot = GetRasterPlot(iIndex);
		//plot->SetStartDateTime(pGMD->StartDateTime, pGMD->TimeInterval);
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
		
		std::string strXComment = pGMD->strXComment;
		std::string strYComment = pGMD->strYComment;
		std::string strZComment = pGMD->strZComment;
		//rrf.GetAxisComment(strXComment, strYComment, strZComment);

		QDateTime dt = QDateTime::fromString(QString::fromStdString(strXComment), "yyyyMMddhhmmss");
		QwtText topTitle, leftTitle, rightTitle, bottomTitle;

		topTitle.setText(strTitle); topTitle.setFont(globleFont);
		leftTitle.setText(QString::fromStdString(strYComment)); leftTitle.setFont(globleFont);
		rightTitle.setText(strRightName); rightTitle.setFont(globleFont);
		bottomTitle.setText(QString::fromLocal8Bit("") + dt.date().toString("yyyyMMdd")); bottomTitle.setFont(globleFont);
		
		plot->setTitle(strTitle);
		plot->SetStartDateTime(dt, pGMD->TimeInterval);

		plot->setAxisTitle(QwtPlot::xBottom, bottomTitle);
		plot->setAxisTitle(QwtPlot::yLeft, leftTitle);
		plot->setAxisTitle(QwtPlot::yRight, rightTitle);

		plot->SetColorMap(pLinearColorMap);
		plot->SetAxisColorMap(pLinearColorMap2);
		plot->SetRasterData(pRRD);
		plot->SetYScaleDrawInterval(pGMD->HeightScale);
		plot->DrawPlot(pGMD->fXMin, pGMD->fXMax, pGMD->fXInterval, pGMD->fYMin, pGMD->fYMax, pGMD->fYInterval);

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
		
		QDateTime dt = QDateTime::fromString(QString::fromStdString(strXComment), "yyyyMMddhhmmss");
		QwtText topTitle, leftTitle, rightTitle, bottomTitle;
		topTitle.setText(strTitle); topTitle.setFont(globleFont);
		leftTitle.setText(QString::fromStdString(strYComment)); leftTitle.setFont(globleFont);
		rightTitle.setText(strRightName); rightTitle.setFont(globleFont);
		bottomTitle.setText(QString::fromLocal8Bit("ÆÊÃæÍ¼-") + dt.date().toString("yyyyMMdd")); bottomTitle.setFont(globleFont);

		plot->setTitle(strTitle);
		plot->SetStartDateTime(dt, pGMD->TimeInterval);
		
		plot->setAxisTitle(QwtPlot::xBottom, bottomTitle);
		plot->setAxisTitle(QwtPlot::yLeft, leftTitle);
		plot->setAxisTitle(QwtPlot::yRight, rightTitle);

		plot->SetColorMap(pLinearColorMap);
		plot->SetAxisColorMap(pLinearColorMap2);
		plot->SetRasterData(pRRD);

		plot->DrawPlot(pGMD->fXMin, pGMD->fXMax, 180, pGMD->fYMin, pGMD->fYMax, pGMD->fYInterval);
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


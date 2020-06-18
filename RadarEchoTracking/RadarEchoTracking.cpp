// RadarEchoTracking.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ui_MainWindow.h"
#include "MainWindow.h"
#include "ConfigAccesser.h"
#include "QwtPlotDialogImps.h"
#include "DistroScaleDraw.h"
ConfigAccesser g_GlobleConfig;
MainWindow* g_MainWindow = 0;

#define TIMELONG 15 * 24 * 60

class licenceThread : public QThread
{
public:
	void run()
	{
		Sleep(500);
		QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Tracking", QSettings::NativeFormat);

		LARGE_INTEGER m_liPerfFreq={0};
		//获取每秒多少CPU Performance Tick 
		QueryPerformanceFrequency(&m_liPerfFreq); 

		LARGE_INTEGER m_liPerfStart={0};
		QueryPerformanceCounter(&m_liPerfStart);

		LARGE_INTEGER liPerfNow={0};
		// 计算CPU运行到现在的时间
		QueryPerformanceCounter(&liPerfNow);
		int time=( ((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000)/m_liPerfFreq.QuadPart);
		char buffer[100];
		sprintf(buffer,"執行時間 %d millisecond ",time);
		cout<<buffer<<endl;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	QSettings settings("\\HKEY_LOCAL_MACHINE\\SOFTWARE", QSettings::NativeFormat);
	settings.beginGroup("Tracking");
	int time = settings.value("time", 0).toInt();
	if(time == 0)
		settings.setValue("time", TIMELONG);
	settings.endGroup();

	g_GlobleConfig.Read("./Config/RadarTrack.xml");
	
	QApplication app(argc, argv);
	/*QStringList keysList = QStyleFactory::keys();
	for(int i = 0; i < keysList.size(); i++)
	{
		qDebug()<<keysList.at(i)<<endl;
	}*/

	app.setStyle(QStyleFactory::create("Plastique"));
#if 1
	MainWindow mw;
	g_MainWindow = &mw;

	Ui::mainWindow setupMainWindow;
	setupMainWindow.setupUi(&mw);
	
	mw.SetUpActions(setupMainWindow.toolBar);
	mw.SetUpMenus(mw.menuBar());
	mw.LoadBusinessFeature();
	mw.TransformData();
	mw.AddFileTime();
	mw.AddColorTable(true);
	mw.showMaximized();
	
	mw.ScaleToMap();
	mw.LoadPaoDian();
	mw.CreateRadarDisplayLayer();
	mw.FlushList();
	mw.DefaultTrackStart();

	mw.DefaultAction();
#else
	QMap<int, QString> strList;
	QwtPlotDialogImps pi;
	pi.InitPlot();
	QPolygonF p1, p2;

	strList.insert(0, "2014");
	strList.insert(10, "2015");
	strList.insert(20, "2016");
	strList.insert(30, "2017");
	strList.insert(40, "2018");
	strList.insert(50, "2019");
	strList.insert(60, "2020");
	strList.insert(70, "2021");
	strList.insert(80, "2022");
	strList.insert(90, "2023");
	strList.insert(100, "2024");
	strList.insert(110, "2025");
	strList.insert(120, "2026");

	DistroScaleDraw* dsd = new DistroScaleDraw(Qt::Vertical, strList);
	//dsd->setTickLength(QwtScaleDiv::MajorTick, 10);

	p1.push_back(QPointF(0, 0.1));
	p1.push_back(QPointF(10, 0.2));
	p1.push_back(QPointF(20, 24));
	p1.push_back(QPointF(30, 56));
	p1.push_back(QPointF(40, 87));
	p1.push_back(QPointF(50, 91));
	p1.push_back(QPointF(60, 16));
	p1.push_back(QPointF(70, 35));
	p1.push_back(QPointF(80, 25));
	p1.push_back(QPointF(90, 75));
	p1.push_back(QPointF(100, 45));
	p1.push_back(QPointF(110, 62));
	p1.push_back(QPointF(120, 10));

	p2.push_back(QPointF(0, 0.01));
	p2.push_back(QPointF(10, 0.02));
	p2.push_back(QPointF(20, 55));

	//pi.DrawPlot(p1, dsd, QColor(255, 0, 0), "aaaaa");
	//pi.DrawPlot(p2, 0, QColor(0, 255, 0), "bbbbb");
	pi.DrawPlot(0, "aaa", p1, dsd, QColor(255, 0, 0), "aaaaa");

	pi.DrawPlot(1, "bbb", p2, dsd, QColor(0, 255, 0), "bbbbb");
	//pi.AddMark(p1);
	//pi.AddMark(p2);
	pi.Show();

	/*QwtPlotDialogImps pi;
	pi.InitBar();

	QVector<int> values;
	QVector<QColor> colors;
	QVector<QVector<double>> ritos;

	values.push_back(10);colors.push_back(QColor(234,0, 23));
	values.push_back(20);colors.push_back(QColor(0,212, 23));
	values.push_back(30);colors.push_back(QColor(0,0, 100));

	QVector<double> rito1, rito2, rito3;
	rito1.push_back(1.2);
	rito1.push_back(2);
	rito1.push_back(3);

	rito2.push_back(2.3);
	rito2.push_back(1);
	rito2.push_back(2);

	rito3.push_back(4);
	rito3.push_back(5);
	rito3.push_back(6);

	ritos.push_back(rito1);
	ritos.push_back(rito2);
	ritos.push_back(rito3);

	QMap<int, QString> strList;
	strList.insert(0, "2014");
	strList.insert(1, "2015");
	strList.insert(2, "2016");

	DistroScaleDraw* dsd = new DistroScaleDraw(Qt::Vertical, strList);

	pi.DrawBar(values, colors, ritos, 0);

	pi.AddBarMark(ritos);
	pi.Show();*/

#endif
	app.exec();
	return 0;
}


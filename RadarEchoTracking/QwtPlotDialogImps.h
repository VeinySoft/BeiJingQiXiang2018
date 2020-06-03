#pragma once
#include "ui_TabPlot.h"
#include "ui_ClipNcFileImage.h"
#include "DistroScaleDraw.h"

class MainWindow;

class Plot : public QwtPlot
{
public:
	Plot( QWidget *parent = NULL );
	
protected:
	virtual void resizeEvent( QResizeEvent * );

private:
	void populate();
	void updateGradient();
};

class QwtPlotDialogImps : public QObject
{
	Q_OBJECT
public:
	QwtPlotDialogImps(void);
	~QwtPlotDialogImps(void);
	
	void InitPlot();
	void DrawPlot(const QPolygonF& points, QwtScaleDraw* sc, const QColor& color, const QString& strTitle);

	void DrawPlot2(const QPolygonF& points, QwtScaleDraw* sc, const QColor& color, const QString& strTitle);
	void InitPlot2();

	void InitPlot4();
	void DrawPlot4(const QPolygonF& points, QwtScaleDraw* sc, const QColor& color, const QString& strTitle);

	void InitPlot5();
	void DrawPlot5(const QPolygonF& points, QwtScaleDraw* sc, const QColor& color, const QString& strTitle);

	void InitPlot6();
	void DrawPlot6(const QPolygonF& points, QwtScaleDraw* sc, const QColor& color, const QString& strTitle);

	void AddMark(const QPolygonF& points);
	void Show(const QString& strName = QString::fromLocal8Bit("ÔËËã½á¹û"), int ix = 0, int iy = 40, int iWidth = 840, int iHeight = 600);

	void InitBar();
	void DrawBar(QVector<int>& values, QVector<QColor>& colors, QVector<QVector<double>>& rito, QwtScaleDraw* sc = 0);

	void setOrientation( int orientation );
	void AddBarMark(const QVector<QVector<double>>& rito);
	void AddMark2(const QPolygonF& points);

	void InitAll();
	void DrawPlot(int tableid, const QString& name, const QPolygonF& points
		, QwtScaleDraw* sc, const QColor& color, const QString& strTitle, const QVector<float>* pList = 0);

	inline void SetScaleDraw(int i, const QMap<int, QString>* pMM)
	{
		DistroScaleDraw* pSD = new DistroScaleDraw(Qt::Horizontal, *pMM);
		switch(i)
		{
		case 0:
			m_Plot->setAxisScaleDraw(QwtPlot::xBottom, pSD);
			pSD->setMyPlot(m_Plot);
			break;
		case 1:
			m_plot3->setAxisScaleDraw(QwtPlot::xBottom, pSD);
			pSD->setMyPlot(m_plot3);
			break;
		case 2:
			m_plot2->setAxisScaleDraw(QwtPlot::xBottom, pSD);
			pSD->setMyPlot(m_plot2);
			break;
		case 3:
			m_plot4->setAxisScaleDraw(QwtPlot::xBottom, pSD);
			pSD->setMyPlot(m_plot4);
			break;
		case 4:
			m_plot5->setAxisScaleDraw(QwtPlot::xBottom, pSD);
			pSD->setMyPlot(m_plot5);
			break;
		case 5:
			m_plot6->setAxisScaleDraw(QwtPlot::xBottom, pSD);
			pSD->setMyPlot(m_plot6);
			break;
		}
	}
	inline bool IsVisible(){return m_pMainWindow->isVisible();}

	void CloseAll();

	void AddClipImage(QImage* pImage, const QString& text);
	void AddSectionImage(QImage* pImage, const QString& text);

	void SaveReuslt(const QString& strPath);
	QString GetCurveName(const QwtPlotCurve* pPC);
	inline void SetTitle(const QString& strName){m_pMainWindow->setWindowTitle(strName);};
	//inline void Set
public slots:
	void slot_CopyToClipBorad(bool);
	void slot_HideMark(bool);
	void slot_clip_activated(const QModelIndex & index);
	void slot_Section_activated(const QModelIndex & index);
	void slot_clip_currentTextChanged(const QString & currentText);
	void slot_Section_currentTextChanged(const QString & currentText);

private:
	QwtPlotCurve *m_pCurve;
	QMainWindow* m_pMainWindow;
	Plot* m_Plot, *m_plot2, *m_plot3, *m_plot4, *m_plot5, *m_plot6;
	QwtPlotMultiBarChart *d_barChartItem;
	QWidget* plotWidget;
	QwtPlotMarker *m_Pos, *m_Pos2;
	Ui::TabPlot m_Setup;
	Ui::NcFileImageWidget m_Setup2;
	Ui::NcFileImageWidget m_Setup3;
	QVector<QPolygonF> pf;
	QMap<int, QString> dataTimeList;
	QMap<QString, QwtPlotCurve*> m_CurveMap;
	QSet<QString> m_MarkerSet;
	QMap<QString, QVector<float>> m_OtherValue;

	QMap<QString, QImage> m_ClipReslut;
	QMap<QString, QImage> m_SectionReslut;
	QwtPlot* m_pCurrentPlot;
	QWidget* m_ClipImageWidget;
	QWidget* m_SectionImageWidget;

	QGraphicsScene* m_SectionScene;
	QGraphicsScene* m_ClipScene;
	QSet<QwtPlotMarker*> m_MarkerObjectSet;
};


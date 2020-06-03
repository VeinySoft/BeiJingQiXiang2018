#pragma once
#include "qwt_scale_draw.h"

class DistroScaleDraw :
	public QwtScaleDraw
{
public:
	~DistroScaleDraw(void);
public:
	DistroScaleDraw( Qt::Orientation orientation, const QMap<int, QString> &labels);

	virtual QwtText label( double value ) const;

	inline void setMyPlot(QwtPlot* pP){m_pMyPlot = pP;}

private:
	QMap<int, QString> d_labels;
	QwtPlot* m_pMyPlot;
};


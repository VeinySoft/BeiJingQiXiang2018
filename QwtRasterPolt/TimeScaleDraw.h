#pragma once
#include "qwt_scale_draw.h"
class TimeScaleDraw :
	public QwtScaleDraw
{
public:
	TimeScaleDraw(const QDateTime &base, int iInterval = 60);
	virtual QwtText label(double v) const;
	virtual void drawTick( QPainter *pP, double val, double len ) const;
    virtual void drawLabel( QPainter *pP, double val ) const;
private:
	QDateTime baseTime;
	int m_iInterval;
};


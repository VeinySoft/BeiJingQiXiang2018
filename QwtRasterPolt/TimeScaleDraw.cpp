#include "stdafx.h"
#include "TimeScaleDraw.h"

TimeScaleDraw::TimeScaleDraw(const QDateTime & base, int iInterval) :
	baseTime(base), m_iInterval(iInterval)
{
}

QwtText TimeScaleDraw::label(double v) const
{
	QDateTime upTime = baseTime.addSecs((int)v*m_iInterval);
	QString strOutLabel = upTime.toString("hh:mm:ss");
	return strOutLabel;
}

void TimeScaleDraw::drawTick( QPainter *pP, double val, double len ) const
{
	QwtScaleDraw::drawTick(pP, val, len);
}

void TimeScaleDraw::drawLabel( QPainter *pP, double val ) const
{
	QwtScaleDraw::drawLabel(pP, val);
}
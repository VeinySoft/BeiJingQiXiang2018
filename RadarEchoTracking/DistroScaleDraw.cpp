#include "StdAfx.h"
#include "DistroScaleDraw.h"
#include <QDateTime>

DistroScaleDraw::DistroScaleDraw( Qt::Orientation orientation, const QMap<int, QString> &labels):
/*d_labels( labels ), */m_pMyPlot(0)
{
	//QMap<int, QString> data = labels;
	d_labels = labels;
	setTickLength( QwtScaleDiv::MinorTick, 0 );
	setTickLength( QwtScaleDiv::MediumTick, 0 );
	setTickLength( QwtScaleDiv::MajorTick, 2 );

	enableComponent( QwtScaleDraw::Backbone, false );

	if ( orientation == Qt::Vertical )
	{
		setLabelRotation( -60.0 );
	}
	else
	{
		setLabelRotation( -20.0 );
	}

	setLabelAlignment( Qt::AlignLeft | Qt::AlignVCenter );
}

QwtText DistroScaleDraw::label( double value ) const
{
	//const QwtScaleDiv sd = this->scaleDiv();

	QwtText lbl;
	//lbl.setText(QString("%1").arg((int)value));
	const DistroScaleDraw* mypoint = this;
	QMap<int, QString>::ConstIterator find = d_labels.find(int(value));
	if(find != d_labels.end())
	{
		double dw = abs(find.key() - value);
		if(dw < 0.0000001)
		{
			QString strDataTime = find.value();
			QDateTime dt = QDateTime::fromString(strDataTime,
				"yyyyMMddhhmmss");

			QString hm = dt.time().toString("hh:mm");
			QString symd = dt.date().toString(QString::fromLocal8Bit("yyyyÄêMMÔÂddÈÕ"));

			if(m_pMyPlot) m_pMyPlot->setTitle(symd);
			lbl.setText(hm);
		}
	}
	return lbl;
}

DistroScaleDraw::~DistroScaleDraw(void)
{
}

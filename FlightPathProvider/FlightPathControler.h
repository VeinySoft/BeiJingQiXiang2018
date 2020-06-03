#pragma once
#include <qstringlist.h>
#include <osg\Array>

class FlightPathControler
{
public:
	virtual QStringList& GetDateList() = 0;
	virtual QStringList& GetTimeList() = 0;
	virtual osg::ref_ptr<osg::Vec3Array>& GetFlightPath() = 0;
	virtual void SetFlightPathPartColor(const osg::Vec4& color, size_t iStart, size_t iCount) = 0;
};
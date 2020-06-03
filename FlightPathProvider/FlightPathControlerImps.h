#pragma once
#include "flightpathcontroler.h"

class FlightPath;
class FlightPathProvider;

class FlightPathControlerImps :
	public FlightPathControler
{
public:
	FlightPathControlerImps(FlightPathProvider* pFPP);
	~FlightPathControlerImps(void);
	virtual QStringList& GetDateList();
	virtual QStringList& GetTimeList();
	virtual osg::ref_ptr<osg::Vec3Array>& GetFlightPath();
	void SetFlightPathPartColor(const osg::Vec4& color, size_t iStart, size_t iCount);

private:
	FlightPathProvider* m_pFlightPathProvider;
	FlightPath* m_pFlightPath;
};


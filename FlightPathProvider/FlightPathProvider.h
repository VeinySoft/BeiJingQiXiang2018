#pragma once
#include "DataProvider.h"
#include <QtCore>
#include <QtGui>
#include "FlightPath.h"

class FlightPathControler;

class FlightPathProvider  : public goto_gis::DataProvider
{
public:
	FlightPathProvider();
	virtual bool LoadFile(const std::string& filename);
	virtual void AreaAdd(bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p);
	inline QStringList& GetDataList(){return m_strDataList;}
	inline QStringList& GetTimeList(){return m_strTimeList;}
	inline osg::ref_ptr<osg::Vec3Array>& GetLonLatArray(){return m_lonlatArray;}
	inline FlightPath* GetFlightPath(){return m_pFlightPath;}
private:
	FlightPath* m_pFlightPath;
	QStringList m_strDataList;
	QStringList m_strTimeList;
	osg::ref_ptr<osg::Vec3Array> m_lonlatArray;
	FlightPathControler* m_pFlightPathControler;
};
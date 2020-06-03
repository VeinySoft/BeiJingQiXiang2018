// FlightPathProvider.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "FlightPathProvider.h"
#include "FlightPath.h"
#include "FlightPathControlerImps.h"

FlightPathProvider::FlightPathProvider()  : m_lonlatArray(new osg::Vec3Array), m_pFlightPathControler(new FlightPathControlerImps(this))
{
	this->InsertExtendInterface(m_pFlightPathControler);
}
bool FlightPathProvider::LoadFile(const std::string& filename)
{
	QFile file(filename.c_str());
	file.open(QIODevice::ReadOnly);

	QTextStream ts(&file);
	
	while(!ts.atEnd())
	{
		QString strLine = ts.readLine();
		QStringList strDataList = strLine.split(',');
		if(strDataList.length() != 5) return false;
		m_strDataList.push_back(strDataList[0].simplified());
		m_strTimeList.push_back(strDataList[1].simplified());

		QString strX, strY, strZ;
		strX = strDataList[3].simplified();
		strY = strDataList[2].simplified();
		strZ = strDataList[4].simplified();

		double dx(strX.toDouble()),dy(strY.toDouble()),dz(strZ.toInt());
		m_lonlatArray->push_back(osg::Vec3(dx, dy, dz));
	}

	m_pFlightPath = new FlightPath(m_lonlatArray);
	this->ParentLayer()->GetMgoGeometry()->insert(goto_gis::Layer::MgoGeometryPair(0, m_pFlightPath));
	return false;
}

void FlightPathProvider::AreaAdd(bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p)
{
}

#pragma once
#include "gogeometry.h"
//#include <osg/ref>
class FlightPath : public goto_gis::goGeometry
{
public:
	FlightPath(osg::Vec3Array* pVA);
	~FlightPath(void);

	virtual void CreateOsgFromGeos(osg::Node* pNode);
	virtual void SetCoordinateTransform(goto_gis::CoordinateTransform* pCT);
	virtual geom::GeometryTypeId GetGeometryType();
	virtual void TransformGeometry();
	void SetPartColor(const osg::Vec4& color, size_t iStart, size_t iCount);
private:
	osg::ref_ptr<osg::Geometry> m_pFlightLine;
	osg::ref_ptr<osg::Vec3Array> m_pLonLatArray;
	osg::ref_ptr<osg::Vec4Array> m_pColorArray;
	osg::ref_ptr<osg::Geode> m_pGeode;
	goto_gis::CoordinateTransform* m_pCoordinateTransform;
};


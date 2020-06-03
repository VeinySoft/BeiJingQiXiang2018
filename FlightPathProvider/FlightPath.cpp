#include "StdAfx.h"
#include <QtCore>
#include <QtGui>

#include "DataProvider.h"
#include "CoordinateTransform.h"
#include "FlightPath.h"


FlightPath::FlightPath(osg::Vec3Array* pVA) : m_pLonLatArray(pVA), m_pCoordinateTransform(0), m_pGeode(new osg::Geode)
	, m_pColorArray(new osg::Vec4Array), m_pFlightLine(new osg::Geometry)
{

}

FlightPath::~FlightPath(void)
{
}

void FlightPath::CreateOsgFromGeos(osg::Node* pNode)
{
	osg::Group* pGroup = pNode->asGroup();
	
	if(pGroup)
	{
		osg::ref_ptr<osg::LineWidth> lineWid = new osg::LineWidth(3.0f);

		osg::ref_ptr<osg::Vec3Array> vertex = new osg::Vec3Array;
		for(size_t i = 0; i < m_pLonLatArray->size(); i++)
		{
			osg::Vec3 PosTemp;
			osg::Vec3 v3pos(m_pLonLatArray->at(i).x(), m_pLonLatArray->at(i).y(), 0);

			m_pCoordinateTransform->Transform(v3pos, PosTemp);
			vertex->push_back(PosTemp);
		}
		m_pFlightLine->setVertexArray(vertex);

		osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array(1);
		(*normals)[0].set(1.0f,0.0f,0.0f);
		m_pFlightLine->setNormalArray(normals, osg::Array::BIND_OVERALL);

		//osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
		for(size_t i = 0; i < vertex->size(); i ++)
		{
			m_pColorArray->push_back(m_pParentLayer->GetLayerColor());
		}
		m_pFlightLine->setColorArray(m_pColorArray, osg::Array::BIND_PER_VERTEX);

		m_pFlightLine->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, vertex->size()));
		//m_pFlightLine->getOrCreateStateSet()->setAttribute(lineWid, osg::StateAttribute::ON);

		m_pGeode->addDrawable(m_pFlightLine);
		pGroup->addChild(m_pGeode);
	}
	else
	{
		return;
	}


}

void FlightPath::SetCoordinateTransform(goto_gis::CoordinateTransform* pCT)
{
	m_pCoordinateTransform = pCT;
}

geom::GeometryTypeId FlightPath::GetGeometryType()
{
	return geom::GEOS_LINESTRING;
}

void FlightPath::TransformGeometry()
{
}

void FlightPath::SetPartColor(const osg::Vec4& color, size_t iStart, size_t iCount)
{
	for(size_t i = 0; i < iCount; i++)
	{
		(*m_pColorArray)[iStart + i] = color;
	}

	m_pFlightLine->dirtyBound();
	m_pFlightLine->dirtyDisplayList();
	//m_pFlightLine->
}
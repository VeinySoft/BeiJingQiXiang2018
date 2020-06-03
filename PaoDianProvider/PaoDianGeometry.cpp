#include "StdAfx.h"
#include "PaoDianGeometry.h"

PaoDianGeometry::PaoDianGeometry(void)
{
	m_PaoDianSwitch = new osg::Switch();
}


PaoDianGeometry::~PaoDianGeometry(void)
{
}

void PaoDianGeometry::CreateOsgFromGeos( osg::Node* pNode )
{
	osg::Group* pGroup = pNode->asGroup();
	if(pGroup)
	{
		pGroup->addChild(m_PaoDianSwitch);
	}
	else
	{
		return;
	}

	for(int i = 0; i < m_PaoDianData.size(); i++)
	{
		GeomInfo gi = m_PaoDianData.at(i);

		osg::Vec3 v3pos(gi.fLon, gi.fLat, 0), PosTemp;
		osg::ref_ptr<osg::AutoTransform> pAutoTransfrom = new osg::AutoTransform;
		//pAutoTransfrom->setAutoScaleToScreen(true);
		m_pCoordinateTransform->Transform(v3pos, PosTemp);

		PosTemp.set(PosTemp.x(), m_pParentLayer->LayerZ(), PosTemp.z());
		pAutoTransfrom->setPosition(PosTemp);
		
		osg::ref_ptr<osg::MatrixTransform> pMatrixTransform = new osg::MatrixTransform;
		osg::Matrix m4Scale;

		//m4Scale.makeScale(20.0, 1.0, 20.0);
		//pMatrixTransform->setMatrix(m4Scale);
		osg::ref_ptr<osg::Geode> pGeode = new osg::Geode;
		osg::ref_ptr<osg::Geometry> pGeom;
		CreateCircleGeometry(gi.fStart, gi.fEnd, pGeom);
		pGeode->addDrawable(pGeom);
		pMatrixTransform->addChild(pGeode);

		pAutoTransfrom->addChild(pMatrixTransform);

		m_PaoDianSwitch->addChild(pAutoTransfrom);

		m_GeodeMap.insert(gi.iid, pAutoTransfrom.get());
	}
}

void PaoDianGeometry::SetCoordinateTransform( goto_gis::CoordinateTransform* pCT )
{
	m_pCoordinateTransform = pCT;
}

geom::GeometryTypeId PaoDianGeometry::GetGeometryType()
{
	return (geom::GeometryTypeId)-1;
}

void PaoDianGeometry::TransformGeometry()
{
	
}

void PaoDianGeometry::AddPaoDian( GeomInfo& gi )
{
	m_PaoDianData.push_back(gi);
}

void PaoDianGeometry::CreateCircleGeometry( float fS, float fE, osg::ref_ptr<osg::Geometry>& pGeom )
{
	osg::Vec3 rotAxis(0, 1, 0), ZAxis(0, 0, 10000)/*, XAxis(1, 0, 0)*/;
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices->push_back(osg::Vec3(0,0,0));

	osg::Matrix rotStart, rot;

	if(fS > fE)
	{
		float fi = fS;
		while(abs(fi - 360) < 0.00001 || fi - 360 < 0)
		{
			rot.makeRotate((fi) * 3.1415926/180, rotAxis);
			osg::Vec3 newPos;

			newPos = ZAxis * rot;
			vertices->push_back(newPos);
			fi++;
		}

		for(float i = 0; abs(i - fE) < 0.00001 || i - fE < 0; i++)
		{
			rot.makeRotate((i) * 3.1415926/180, rotAxis);
			osg::Vec3 newPos;

			newPos = ZAxis * rot;
			vertices->push_back(newPos);
		}
	}
	else
	{
		for(float i = fS; (i < fE) || abs(i - fE) < 0.00001; i++)
		{
			rot.makeRotate((i) * 3.1415926/180, rotAxis);
			osg::Vec3 newPos;

			newPos = ZAxis * rot;
			vertices->push_back(newPos);
		}
	}

	geom->setVertexArray(vertices);

	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array(1);
	(*normals)[0].set(1.0f,0.0f,0.0f);
	geom->setNormalArray(normals, osg::Array::BIND_OVERALL);

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(m_pParentLayer->GetLayerColor());
	geom->setColorArray(colors, osg::Array::BIND_OVERALL);

	geom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_LOOP, 0, vertices->size()));

	pGeom = geom;
}

void PaoDianGeometry::VisblePaoDian( int i, bool bIsVisable )
{
	QMap<int, osg::AutoTransform*>::iterator ite = m_GeodeMap.find(i);

	if(ite != m_GeodeMap.end())
	{
		m_PaoDianSwitch->setChildValue(ite.value(), bIsVisable);
	}
}

void PaoDianGeometry::VisbleAll( bool bIsVisable )
{
	if(bIsVisable)
		m_PaoDianSwitch->setAllChildrenOn();
	else
		m_PaoDianSwitch->setAllChildrenOff();
}

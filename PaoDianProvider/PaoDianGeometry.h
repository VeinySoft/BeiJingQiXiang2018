#pragma once
#include "gogeometry.h"
#include <QVector>
#include <QString>
#include <QMap>
#include <osg/AutoTransform>
#include <osg/MatrixTransform>
#include <Layer.h>

class PaoDianGeometry : public goto_gis::goGeometry
{
public:
	struct GeomInfo
	{
		float fLon, fLat;
		QString name;
		float fStart, fEnd;
		int iid;
	};

	PaoDianGeometry(void);
	~PaoDianGeometry(void);

	virtual void CreateOsgFromGeos( osg::Node* pNode );

	virtual void SetCoordinateTransform( goto_gis::CoordinateTransform* pCT );

	virtual geom::GeometryTypeId GetGeometryType();

	virtual void TransformGeometry();

	virtual void AddPaoDian(PaoDianGeometry::GeomInfo& gi);

	void CreateCircleGeometry(float fS, float fE, osg::ref_ptr<osg::Geometry>& pGeom);

	void VisblePaoDian(int i, bool bIsVisable);
	void VisbleAll(bool bIsVisable);

	void GetBoundingBox(osg::BoundingBox* pBox){
		if(pBox)
		{
			pBox->expandBy(m_PaoDianSwitch->getBound());
		}
	}

private:
	QVector<GeomInfo> m_PaoDianData;
	goto_gis::CoordinateTransform* m_pCoordinateTransform;
	osg::ref_ptr<osg::Switch> m_PaoDianSwitch;

	QMap<int, osg::AutoTransform*> m_GeodeMap;
};


#pragma once
#include "goGeometry.h"
#include "MyImageSequence.h"

class RadarGeometry : public goto_gis::goGeometry
{
public:
	RadarGeometry(void);
	~RadarGeometry(void);

	virtual void CreateOsgFromGeos( osg::Node* pNode );

	virtual void SetCoordinateTransform( goto_gis::CoordinateTransform* pCT );

	virtual geom::GeometryTypeId GetGeometryType();

	virtual void TransformGeometry();

	virtual void CreateGeometry( geom::Geometry* pGeosGeometry );
	//void AddImage(const osg::ref_ptr<osg::Texture2D>& pImage);
	void AddArray(const osg::ref_ptr<osg::Vec3Array>& v3Array);
	void AddImage(const osg::ref_ptr<osg::Image>& pImage);
	void SetImage(int i, const osg::ref_ptr<osg::Image>& pImage);
	inline osg::ref_ptr<MyImageSequence> GetImageSequence(){return m_pImageSequence;}
	void AddRect(const std::string& name, osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4, osg::Vec4& color, const std::string& text = "");
	void SetRectColor(const std::string& strName, int i, const osg::Vec4& color);
	//inline osg::ref_ptr<osg::Switch> getSwitchNode(){return m_TrackingBoxNodes;}
	void AddRadarImage(const osg::ref_ptr<osg::Vec3Array>& cube, const osg::ref_ptr<osg::Image>& image);
	void AddRadarGeometry(const osg::ref_ptr<osg::Vec3Array>& vs, const osg::ref_ptr<osg::Vec4Array>& cs);
	void HideRects(const std::string& cubeName, bool bHide);
	void RemoveRects(const std::string& cubeName);
	void RemoveAllRect();
	void RemoveTrackRect();
	void MoveRect( const std::string& cubeName, const osg::Matrix& m );
	osg::Geometry* GetRectGeometry(const std::string& cubeName);

private:
	osg::ref_ptr<osg::Group> m_TrackingBoxNodes;
	osg::ref_ptr<osg::Geode> m_MyGeode;
	friend class ControlorImps;
	long m_RadarLayerNum;
	//std::vector<osg::ref_ptr<osg::Texture2D>> m_Images;
	std::vector<osg::ref_ptr<osg::Vec3Array>> m_VectorArrays;
	//osg::Geode* m_pParentGeode;
	std::vector<osg::ref_ptr<osg::Geometry>> m_CurGeometrys;
	osg::ref_ptr<MyImageSequence> m_pImageSequence;
	std::map<std::string, osg::ref_ptr<osg::Switch>> m_TrackBoxes;
	
	struct TowColor
	{
		TowColor(){}
		TowColor(const osg::Vec4 c1, const osg::Vec4& c2){m_c1 = c1; m_c2 = c2;}
		osg::Vec4 m_c1, m_c2;
	};

	std::map<std::string, TowColor> m_oldColorsMap;
	//osg::ref_ptr<osgText::Text> m_FileName1, m_FileName2;
};


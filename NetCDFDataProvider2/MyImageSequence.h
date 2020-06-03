#pragma once
#include <osg/ImageSequence>

class RadarGeometry;

class MyImageSequence :
	public osg::ImageSequence
{
public:
	virtual ~MyImageSequence();
	int getCurrentImage();
	void setMyChildImage(int pos);
	virtual void update( osg::NodeVisitor* nv );

	RadarGeometry* m_pRadarGeometry;
};

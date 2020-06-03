#pragma once
#include <osg/Array>
#include <osg/Texture2D>
#include <QtGui>
#include <CoordinateTransform.h>

class NcFile;

class MakeTextureInterface
{
public:
	virtual  bool loadNcFileToTexture(NcFile *pNcFile
		, osg::ref_ptr<osg::Image>& pImage
		, osg::ref_ptr<osg::Vec3Array>& pV3Array) = 0;
	virtual bool loadNcFileToTexture(NcFile *pNcFile
		, osg::ref_ptr<osg::Image>& pImage
		, osg::ref_ptr<osg::Vec3Array>& pV3Array, int iMode) = 0;
	virtual QImage* loadNcFileToTexture( NcFile *pNcFile , osg::ref_ptr<osg::Vec3Array>& pV3Array) = 0;
	virtual bool loadNcFileToGeometry(NcFile *pNcFile, osg::ref_ptr<osg::Vec3Array>& vs
		, osg::ref_ptr<osg::Vec4Array>& cs, goto_gis::CoordinateTransform* pCT, int iMode) = 0;
};


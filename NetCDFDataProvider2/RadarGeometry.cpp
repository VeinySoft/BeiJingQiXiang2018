#include "RadarGeometry.h"
#include <Layer.h>
#include <osg/ShadeModel>
#include <osgDB/ReadFile>
#include <osg/ShaderAttribute>
RadarGeometry::RadarGeometry(void) : m_pImageSequence(new MyImageSequence)
	, m_TrackingBoxNodes(new osg::Group)
{
	m_pImageSequence->m_pRadarGeometry = this;
	m_TrackingBoxNodes->setName("RadarSwitch");
}

RadarGeometry::~RadarGeometry(void)
{
}

void RadarGeometry::CreateOsgFromGeos( osg::Node* pNode )
{
	osg::Group* pGroup = pNode->asGroup();
	if(pGroup)
	{
		m_MyGeode = new osg::Geode();
		pGroup->addChild(m_MyGeode);
		pGroup->addChild(m_TrackingBoxNodes);
	}
	else
		m_MyGeode = pNode->asGeode();

	osg::StateSet* pStateSet = pGroup->getOrCreateStateSet();
	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	//////////////////////////////////////////////////////////////////////////
#if 0
	osg::ref_ptr<osg::Program> program = new osg::Program;
	osg::ref_ptr<osg::Shader> vertex_shader = osgDB::readShaderFile(osg::Shader::VERTEX, ".\\Config\\test.vert");
	
	if (vertex_shader.valid())
	{
		program->addShader(vertex_shader);
	}
	osg::ref_ptr<osg::Shader> fragment_shader = osgDB::readShaderFile(osg::Shader::FRAGMENT, ".\\Config\\test.frag");
	if (fragment_shader.valid())
	{
		program->addShader(fragment_shader);
	}
	pStateSet->setAttributeAndModes(program, osg::StateAttribute::ON);
//#else
	osg::ShaderAttribute* sa = new osg::ShaderAttribute;
	sa->setType(osg::StateAttribute::Type(10000));

	pStateSet->setAttribute(sa);

	{
		osg::Shader* vertex_shader = new osg::Shader(osg::Shader::VERTEX);
		vertex_shader->addCodeInjection(-1,"varying vec4 color;\n");
		vertex_shader->addCodeInjection(-1,"varying vec4 texcoord;\n");
		vertex_shader->addCodeInjection(0,"color = gl_Color;\n");
		vertex_shader->addCodeInjection(0,"texcoord = gl_MultiTexCoord0;\n");
		vertex_shader->addCodeInjection(0,"gl_Position = ftransform();\n");
		sa->addShader(vertex_shader);
	}

	{
		osg::Shader* fragment_shader = new osg::Shader(osg::Shader::FRAGMENT);
		fragment_shader->addCodeInjection(-1,"varying vec4 color;\n");
		fragment_shader->addCodeInjection(-1,"varying vec4 texcoord;\n");
		fragment_shader->addCodeInjection(-1,"uniform sampler2D baseTexture; \n");
		fragment_shader->addCodeInjection(0,"gl_FragColor = color * texture2DProj( baseTexture, texcoord );\n");

		sa->addShader(fragment_shader);
	}
#endif
	//////////////////////////////////////////////////////////////////////////
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
#if 0
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(4);
	geom->setVertexArray(vertices);

	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array(4);

	(*texcoords)[0].set(0.0f,0.0f);
	(*texcoords)[1].set(1.0f,0.0f);
	(*texcoords)[2].set(1.0f,1.0f);
	(*texcoords)[3].set(0.0f,1.0f);

	geom->setTexCoordArray(0,texcoords);

	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array(1);
	(*normals)[0].set(1.0f,0.0f,0.0f);
	geom->setNormalArray(normals, osg::Array::BIND_OVERALL);

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(1);
	(*colors)[0].set(1.0f,1.0f,1.0f,1.0f);
	geom->setColorArray(colors, osg::Array::BIND_OVERALL);

	geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));

	// set up the texture state.

	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	//texture->allocateMipmap(0);
	texture->setNumMipmapLevels(2);

	texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::NEAREST);
	texture->setFilter(osg::Texture::MAG_FILTER,osg::Texture::NEAREST);
	//texture->setNumMipmapLevels(10000);
	//texture->setWrap(osg::Texture::WRAP_S,osg::Texture::REPEAT);

	//texture->setResizeNonPowerOfTwoHint(true);
	texture->setImage(m_pImageSequence);

	osg::ref_ptr<osg::ShadeModel> shadeModel = new osg::ShadeModel(osg::ShadeModel::FLAT);
	osg::StateSet* stateset = geom->getOrCreateStateSet();
	//stateset->setAttribute(shadeModel, osg::StateAttribute::ON);
	//stateset->setMode(GL_BLEND, osg::StateAttribute::OFF);

	stateset->setTextureAttributeAndModes(0, texture,osg::StateAttribute::ON);
#else
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	geom->setVertexArray(vertices);

	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array(1);
	(*normals)[0].set(1.0f,0.0f,0.0f);

	geom->setNormalArray(normals, osg::Array::BIND_OVERALL);

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

	geom->setColorArray(colors, osg::Array::BIND_PER_VERTEX);

	geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 0));
#endif
	m_MyGeode->addDrawable(geom);
	//////////////////////////////////////////////////////////////////////////
}

void RadarGeometry::SetCoordinateTransform( goto_gis::CoordinateTransform* pCT )
{
	
}

geom::GeometryTypeId RadarGeometry::GetGeometryType()
{
	return (geom::GeometryTypeId)0;
}

void RadarGeometry::TransformGeometry()
{
	
}

void RadarGeometry::CreateGeometry( geom::Geometry* pGeosGeometry )
{
	
}

//void RadarGeometry::AddImage( const osg::ref_ptr<osg::Texture2D>& pImage )
//{
//	m_Images.push_back(pImage);
//}

void RadarGeometry::AddArray( const osg::ref_ptr<osg::Vec3Array>& v3Array )
{
	m_VectorArrays.push_back(v3Array);
}

void RadarGeometry::AddImage( const osg::ref_ptr<osg::Image>& pImage )
{
	m_pImageSequence->addImage(pImage);
}

void RadarGeometry::AddRect(const std::string& name, osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4, osg::Vec4& color, const std::string& text )
{
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	osg::Vec3 p11, p22, p33, p44;

	goto_gis::CoordinateTransform* pCT = GetParentLayer()->CoordTrans();
	pCT->Transform(p1, p11);
	vertices->push_back(p11);

	pCT->Transform(p2, p22);
	vertices->push_back(p22);

	pCT->Transform(p3, p33);
	vertices->push_back(p33);

	pCT->Transform(p4, p44);
	vertices->push_back(p44);

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(color);
	geom->setColorArray(colors, osg::Array::BIND_OVERALL);

	geom->setVertexArray(vertices);
	geom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_LOOP,0,4));
	//m_CurGeometrys.push_back(geom);

	std::map<std::string, osg::ref_ptr<osg::Switch>>::iterator finded = m_TrackBoxes.find(name);
	if(finded != m_TrackBoxes.end())
	{
		osg::ref_ptr<osg::Geode> pGeode = finded->second->getChild(0)->asGeode();
		pGeode->addDrawable(geom);

		int iGeomNum = pGeode->getNumDrawables();
		osg::ref_ptr<osg::Geometry> preGeom = pGeode->getDrawable(iGeomNum - 1)->asGeometry();
		osg::ref_ptr<osg::Geometry> geom = pGeode->getDrawable(0)->asGeometry();

		osg::Vec4Array* pPreV4A = dynamic_cast<osg::Vec4Array*>(preGeom->getColorArray());
		osg::Vec4Array* pV4A = dynamic_cast<osg::Vec4Array*>(geom->getColorArray());

		(*pPreV4A)[0] = (*pV4A)[0];

		preGeom->dirtyDisplayList();

		if(iGeomNum > 2)
		{
			osg::ref_ptr<osg::Geometry> MidleGeom = pGeode->getDrawable(iGeomNum - 2)->asGeometry();
			osg::Vec4Array* pV4AMidle = dynamic_cast<osg::Vec4Array*>(MidleGeom->getColorArray());

			(*pV4AMidle)[0] = color;
			MidleGeom->dirtyDisplayList();

			std::map<std::string, TowColor>::iterator finded = m_oldColorsMap.find(name);

			finded->second.m_c2 = color;
		}
	}
	else
	{
		osg::ref_ptr<osg::Geode> pGeode = new osg::Geode();
		pGeode->addDrawable(geom);
		osg::ref_ptr<osg::Switch> pSwitch = new osg::Switch;

		pSwitch->addChild(pGeode);

		m_TrackBoxes[name] = pSwitch;
		m_TrackingBoxNodes->addChild(pSwitch);

		TowColor tc(color, osg::Vec4());
		m_oldColorsMap[name] = tc;

		//////////////////////////////////////////////////////////////////////////
		
		float fxlen = abs(p11.x() - p33.x());
		float fylen = abs(p11.z() - p33.z());
		/*std::string strText = 
			.arg((double)fylen/1000.0, 2, 'g', 3)
			.toStdString();*/

		osg::ref_ptr<osg::Geode> pTextGeode = new osg::Geode();
		osg::ref_ptr<osgText::Text> pHgtTxt = new osgText::Text();
		pHgtTxt->setFont("Fonts/simhei.ttf");
		pHgtTxt->setText(QString("%1km").arg((double)fxlen/1000.0, 0, 'f', 3).toStdString()); 
		//pHgtTxt->setBackdropColor(osg::Vec4(1, 1, 1, 1));
		//pHgtTxt->setBoundingBoxColor(osg::Vec4(0, 0, 0, 1));
		pHgtTxt->setPosition(p33);
		//pHgtTxt->setAutoRotateToScreen(true);
		pHgtTxt->setCharacterSize(20);
		pHgtTxt->setCharacterSizeMode(osgText::TextBase::SCREEN_COORDS);
		pHgtTxt->setAxisAlignment(osgText::Text::SCREEN);
		//pHgtTxt->setDrawMode(osgText::Text::TEXT);
		pHgtTxt->setColor(osg::Vec4(0.0f, .0f,.0f,1.0f));
		pHgtTxt->setAlignment(osgText::Text::LEFT_TOP);
		pTextGeode->addDrawable(pHgtTxt);

		osg::ref_ptr<osgText::Text> pHgtTxt1 = new osgText::Text();
		pHgtTxt1->setFont("Fonts/simhei.ttf");
		pHgtTxt1->setText(QString("%1km").arg((double)fylen/1000.0, 0, 'f', 3).toStdString()); 
		//pHgtTxt->setBackdropColor(osg::Vec4(1, 1, 1, 1));
		//pHgtTxt->setBoundingBoxColor(osg::Vec4(0, 0, 0, 1));
		pHgtTxt1->setPosition(p11);
		//pHgtTxt->setAutoRotateToScreen(true);
		pHgtTxt1->setCharacterSize(20);
		pHgtTxt1->setCharacterSizeMode(osgText::TextBase::SCREEN_COORDS);
		pHgtTxt1->setAxisAlignment(osgText::Text::SCREEN);
		//pHgtTxt->setDrawMode(osgText::Text::TEXT);
		pHgtTxt1->setColor(osg::Vec4(0.0f, .0f,.0f,1.0f));
		pHgtTxt1->setAlignment(osgText::Text::RIGHT_BOTTOM);
		pTextGeode->addDrawable(pHgtTxt1);

		osg::ref_ptr<osgText::Text> nameText = new osgText::Text();
		nameText->setFont("Fonts/simhei.ttf");
		nameText->setText(name); 
		//pHgtTxt->setBackdropColor(osg::Vec4(1, 1, 1, 1));
		//pHgtTxt->setBoundingBoxColor(osg::Vec4(0, 0, 0, 1));
		nameText->setPosition(p11);
		//pHgtTxt->setAutoRotateToScreen(true);
		nameText->setCharacterSize(20);
		nameText->setCharacterSizeMode(osgText::TextBase::SCREEN_COORDS);
		nameText->setAxisAlignment(osgText::Text::SCREEN);
		//pHgtTxt->setDrawMode(osgText::Text::TEXT);
		nameText->setColor(osg::Vec4(0.0f, .0f,.0f,1.0f));
		nameText->setAlignment(osgText::Text::LEFT_TOP);
		pTextGeode->addDrawable(nameText);

		// = new osgText::Text();
		//TimeText->setFont("Fonts/simhei.ttf");
		//TimeText->setText(name); 
		////pHgtTxt->setBackdropColor(osg::Vec4(1, 1, 1, 1));
		////pHgtTxt->setBoundingBoxColor(osg::Vec4(0, 0, 0, 1));
		//TimeText->setPosition(p11);
		////pHgtTxt->setAutoRotateToScreen(true);
		//TimeText->setCharacterSize(20);
		//TimeText->setCharacterSizeMode(osgText::TextBase::SCREEN_COORDS);
		//TimeText->setAxisAlignment(osgText::Text::SCREEN);
		////pHgtTxt->setDrawMode(osgText::Text::TEXT);
		//TimeText->setColor(osg::Vec4(0.0f, .0f,.0f,1.0f));
		//TimeText->setAlignment(osgText::Text::LEFT_TOP);
		//pTextGeode->addDrawable(TimeText);

		pSwitch->addChild(pTextGeode);
	}

}

void RadarGeometry::SetRectColor(const std::string& strName, int i, const osg::Vec4& color)
{
#if 0
	std::map<std::string, TowColor>::iterator colorFinded = m_oldColorsMap.find(strName);
	osg::Vec4 oldColor(1, 0, 0, 1);

	if(colorFinded != m_oldColorsMap.end())
	{
		if(i == 1)
		{
			oldColor = colorFinded->second.m_c1;
		}
		else
			oldColor = colorFinded->second.m_c2;
	}

	std::map<std::string, osg::ref_ptr<osg::Switch>>::iterator finded = m_TrackBoxes.find(strName);
	if(finded != m_TrackBoxes.end())
	{
		osg::ref_ptr<osg::Geode> pGeode = finded->second->getChild(0)->asGeode();
		if(pGeode->getNumDrawables() > i)
		{
			osg::ref_ptr<osg::Geometry> preGeom = pGeode->getDrawable(i - 1)->asGeometry();
			osg::ref_ptr<osg::Geometry> geom = pGeode->getDrawable(i)->asGeometry();

			osg::Vec4Array* pPreV4A = dynamic_cast<osg::Vec4Array*>(preGeom->getColorArray());
			osg::Vec4Array* pV4A = dynamic_cast<osg::Vec4Array*>(geom->getColorArray());

			(*pPreV4A)[0] = oldColor;
			oldColor = pV4A->at(0);
			if(colorFinded != m_oldColorsMap.end())
			{
				colorFinded->second.m_c2 = oldColor;
			}
			(*pV4A)[0] = color;



			preGeom->dirtyDisplayList();
			geom->dirtyDisplayList();
		}
		else if(pGeode->getNumDrawables() == i)
		{
			osg::ref_ptr<osg::Geometry> preGeom = pGeode->getDrawable(i - 1)->asGeometry();
			osg::Vec4Array* pPreV4A = dynamic_cast<osg::Vec4Array*>(preGeom->getColorArray());
			(*pPreV4A)[0] = colorFinded->second.m_c1;
			//oldColor.set(0,0,0,1);

			preGeom->dirtyDisplayList();
		}
	}
#else
	std::map<std::string, TowColor>::iterator colorFinded = m_oldColorsMap.find(strName);
	if(colorFinded != m_oldColorsMap.end())
	{
		std::map<std::string, osg::ref_ptr<osg::Switch>>::iterator finded = m_TrackBoxes.find(strName);
		if(finded != m_TrackBoxes.end())
		{
			osg::ref_ptr<osg::Geode> pGeode = finded->second->getChild(0)->asGeode();
			int iNum = pGeode->getNumDrawables();
			for(int ii = 1; ii < iNum - 1 ; ii++)
			{
				osg::ref_ptr<osg::Geometry> geom = pGeode->getDrawable(ii)->asGeometry();
				osg::Vec4Array* pV4A = dynamic_cast<osg::Vec4Array*>(geom->getColorArray());
				
				(*pV4A)[0] = colorFinded->second.m_c2;
				if(ii == i)
				{
					(*pV4A)[0] = colorFinded->second.m_c1;
				}

				geom->dirtyDisplayList();
			}
		}
	}
#endif
}

void RadarGeometry::SetImage( int i, const osg::ref_ptr<osg::Image>& pImage )
{

}

void RadarGeometry::AddRadarImage( const osg::ref_ptr<osg::Vec3Array>& pV3Array, const osg::ref_ptr<osg::Image>& image )
{
	goto_gis::CoordinateTransform* pCT = GetParentLayer()->CoordTrans();
	osg::Geometry* pGeometry = m_MyGeode->getDrawable(0)->asGeometry();
	osg::Vec3Array* vertices = dynamic_cast<osg::Vec3Array*>(pGeometry->getVertexArray());
	//stateset->setTextureMode();
	for(int ii = 0; ii < pV3Array->size(); ii++)
	{
		osg::Vec3 dst;
		pCT->Transform(pV3Array->at(ii), dst);

		dst.set(dst.x(), this->GetParentLayer()->LayerZ(), dst.z());
		(*vertices)[ii] = dst;
	}

	pGeometry->dirtyBound();
	pGeometry->dirtyDisplayList();
		
#if 1
#if 1
	m_pImageSequence->setImage(0, image);
#else
	osg::ref_ptr<osg::Image> limage = osgDB::readImageFile(
		"G:\\Document\\333.bmp");
	m_pImageSequence->setImage(0, limage);
#endif
	m_pImageSequence->setMyChildImage(0);
	//osg::ref_ptr<osg::Vec3Array> pV3Array = m_VectorArrays.at(0);

	//m_pParentGeode = m_MyGeode;

	//m_pImageSequence->setTimeMultiplier(0.07);
	//m_pImageSequence->play();
#endif
}

void RadarGeometry::HideRects( const std::string& cubeName, bool bHide )
{
	std::map<std::string, osg::ref_ptr<osg::Switch>>::iterator finded = m_TrackBoxes.find(cubeName);
	if(finded != m_TrackBoxes.end())
	{
		if(bHide)
			finded->second->setAllChildrenOff();
		else
			finded->second->setAllChildrenOn();
	}
}

void RadarGeometry::RemoveRects( const std::string& cubeName )
{
	std::map<std::string, osg::ref_ptr<osg::Switch>>::iterator finded = m_TrackBoxes.find(cubeName);
	if(finded != m_TrackBoxes.end())
	{
		m_TrackingBoxNodes->removeChild(finded->second);
		m_TrackBoxes.erase(finded);
		m_oldColorsMap.erase(finded->first);
	}
}

void RadarGeometry::RemoveAllRect()
{
	m_TrackingBoxNodes->removeChildren(0, m_TrackingBoxNodes->getNumChildren());
	m_TrackBoxes.clear();
	m_oldColorsMap.clear();
}

void RadarGeometry::RemoveTrackRect()
{
	std::map<std::string, osg::ref_ptr<osg::Switch>>::iterator be = m_TrackBoxes.begin();

	while(be != m_TrackBoxes.end())
	{
		osg::Geode* pGeode = be->second->getChild(0)->asGeode();
		pGeode->removeDrawables(1, pGeode->getNumDrawables() - 1);
		be++;
	}
	/*for(; be != m_TrackBoxes.end(); be++)
	{
		osg::Geode* pGeode = be->second->getChild(0)->asGeode();
		pGeode->removeDrawables(1, pGeode->getNumDrawables() - 1);
	}*/
}

void RadarGeometry::AddRadarGeometry( const osg::ref_ptr<osg::Vec3Array>& vs, const osg::ref_ptr<osg::Vec4Array>& cs )
{
	osg::Geometry* pGeometry = m_MyGeode->getDrawable(0)->asGeometry();
	osg::Vec3Array* vertices = dynamic_cast<osg::Vec3Array*>(pGeometry->getVertexArray());
	osg::Vec4Array* colors = dynamic_cast<osg::Vec4Array*>(pGeometry->getColorArray());
	//osg::Vec3Array* normals = dynamic_cast<osg::Vec3Array*>(pGeometry->getVertexArray());

	//(*normals)[0].set(1.0f,1.0f,1.0f,1.0f);
	//pGeometry->setColorArray(colors, osg::Array::BIND_OVERALL);

	*vertices = *vs;
	*colors = *cs;
	
	pGeometry->setPrimitiveSet(0, new osg::DrawArrays(GL_QUADS,0,vertices->size()));
	//pGeometry->dirtyBound();
	//pGeometry->dirtyDisplayList();
	//pGeometry->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,vertices->size()));
}

void RadarGeometry::MoveRect( const std::string& cubeName, const osg::Matrix& m )
{
	osg::ref_ptr<osg::Switch> pSwitch = 0;
	std::map<std::string, osg::ref_ptr<osg::Switch>>::iterator it = m_TrackBoxes.find(cubeName);
	if(it != m_TrackBoxes.end())
	{
		pSwitch = (*it).second;
	}

	if(pSwitch == nullptr)
		return;

	if(pSwitch->getNumChildren() > 0)
	{
		for(int i = 0; i < pSwitch->getNumChildren(); i++)
		{
			osg::Geode* pGeode = pSwitch->getChild(i)->asGeode();
			if(pGeode)
			{
				for(int ii = 0; ii < pGeode->getNumDrawables(); ii++)
				{
					osg::Geometry* pGeometry = pGeode->getDrawable(ii)->asGeometry();
					osgText::TextBase* pText = nullptr;
					if(pGeometry != nullptr)
					{
						osg::Vec3Array* pVertexes = dynamic_cast<osg::Vec3Array*>(pGeometry->getVertexArray());
						if(pVertexes)
						{
							for(int iii = 0; iii < pVertexes->size(); iii++)
							{
								osg::Vec3 oldPoint = pVertexes->at(iii);
								osg::Vec3 newPoint = oldPoint * m;

								(*pVertexes)[iii] = newPoint;
							}

							pGeometry->dirtyDisplayList();
							pGeometry->dirtyBound();
						}
					}
					pText = dynamic_cast<osgText::TextBase*>(pGeode->getDrawable(ii));
					if(pText != nullptr)
					{
						osg::Vec3 oldPoint = pText->getPosition();
						osg::Vec3 newPoint = oldPoint * m;
						pText->setPosition(newPoint);
						pText->dirtyDisplayList();
						pText->dirtyBound();
					}
					
				}
			}
		}
	}
}

osg::Geometry* RadarGeometry::GetRectGeometry( const std::string& cubeName )
{
	osg::Geometry* pGeometry = 0;
	osg::ref_ptr<osg::Switch> pSwitch = 0;
	std::map<std::string, osg::ref_ptr<osg::Switch>>::iterator it = m_TrackBoxes.find(cubeName);
	if(it != m_TrackBoxes.end())
	{
		pSwitch = (*it).second;
	}

	if(pSwitch == nullptr)
		return nullptr;

	if(pSwitch->getNumChildren() > 0)
	{
		osg::Geode* pGeode = pSwitch->getChild(0)->asGeode();
		if(pGeode->getNumDrawables() > 0 && pGeode>0)
		{
			pGeometry = pGeode->getDrawable(0)->asGeometry();
			return pGeometry;
		}
	}

	return pGeometry;
}

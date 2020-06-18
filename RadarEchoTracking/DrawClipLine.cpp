#include "StdAfx.h"
#include "DrawClipLine.h"
#include <Proj4Transform.h>
#include "SectionImageDialog.h"

static SectionImageDialog* s_pSectionImageDialog = nullptr;
#define distance2(x1,x2,y1,y2) sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))
extern ConfigAccesser g_GlobleConfig;
extern std::string QStringToStdString(const QString& inStr);

DrawClipLine::DrawClipLine(void) : m_LineUsage(NORMAL), m_pGetBoxList(0)
{
	//CreateText(m_ValueText);
	CreateText(m_KmText);
	//m_ValueText->setText("");
	m_KmText->setText("");
	//osg::Geode* pGeode = m_DrawNode->asGeode();
	//pGeode->addDrawable(m_ValueText);
	//pGeode->addDrawable(m_KmText);

}

void DrawClipLine::CreateText(osg::ref_ptr<osgText::Text>& tt)
{
	osg::ref_ptr<osgText::Text> pScaleTxt = new osgText::Text();
	pScaleTxt->setFont("Fonts/simhei.ttf");
	pScaleTxt->setText("1km"); 
	pScaleTxt->setBackdropColor(osg::Vec4(1, 1, 1, 1));
	pScaleTxt->setBoundingBoxColor(osg::Vec4(0, 0, 0, 1));
	pScaleTxt->setPosition(osg::Vec3(10, 10, 0));
	pScaleTxt->setAutoRotateToScreen(true);
	pScaleTxt->setCharacterSize(25);
	//pScaleTxt->setBackdropType(osgText::Text::OUTLINE);
	pScaleTxt->setDrawMode(osgText::Text::TEXT);
	pScaleTxt->setCharacterSizeMode(osgText::TextBase::SCREEN_COORDS);
	osg::Vec4 m_markColor(.0, .0,.0,1);
	pScaleTxt->setColor(m_markColor);
	pScaleTxt->setAlignment(osgText::Text::LEFT_BOTTOM);
	tt = pScaleTxt;
}

DrawClipLine::~DrawClipLine(void)
{
}

bool DrawClipLine::handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea )
{
	float fZ = 0;

	if(m_pLayer)
		fZ = m_pLayer->LayerZ();

	osg::Vec3 locPos;
	locPos.set(pos.x(), fZ, pos.z());
	osgGA::GUIEventAdapter::EventType et = ea.getEventType();
	switch(et)
	{
	case osgGA::GUIEventAdapter::MOVE:
		if(m_bStart)
		{
			m_pVertexs->pop_back();
			m_pVertexs->push_back(locPos);

			if(m_pVertexs->size() >=2 && m_LineUsage == DISTANCE)
			{
				float fDis = distance2(m_pVertexs->at(0).x(), m_pVertexs->at(1).x(), m_pVertexs->at(0).z(), m_pVertexs->at(1).z());
				m_KmText->setText(QString("%1Km").arg(fDis/1000).toStdString());
				m_KmText->setPosition(m_pVertexs->at(1));
				//m_KmText->setText(QString("%1Km").arg(fDis/1000).toStdString());
				//m_KmText->setPosition(m_pVertexs->at(1));
			}
		}
		
		break;
	case osgGA::GUIEventAdapter::PUSH:
		{
			if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				if(m_LineUsage != MOVE)
				{
					if(m_bStart)
					{
						//m_pVertexs->push_back(locPos);
						m_pVertexs->push_back(locPos);

						if(m_pVertexs->size() >=2)
						{
							m_bStart = false;
							CreateLineGeometry();
							m_pVertexs->clear();
						}
					}
					else
					{
						m_pVertexs->clear();
						m_pVertexs->push_back(locPos);
						m_pVertexs->push_back(locPos);
						m_bStart = true;
					}
				}
				else
				{
					m_PrePoint = pos;
				}
			}
		}
		break;

	case osgGA::GUIEventAdapter::RELEASE:
		if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			osg::Vec3 p1, p2, p3, p4;
			m_pCI->GetRectVertex(m_strRectName.toStdString(), p1, p2, p3, p4);
			SaveRectChange(m_strRectName, p1, p3);
			//m_pCI->get
		}
		break;
	case osgGA::GUIEventAdapter::DRAG:
		{
			osg::Matrix m;
			double dX = pos.x()-m_PrePoint.x();
			double dZ = pos.z()-m_PrePoint.z();

			if(dX == 0 && dZ == 0)
				break;

			m.makeTranslate(osg::Vec3(dX, 0, dZ));

			m_pCI->MoveRect(m_strRectName.toStdString(), m);

			m_PrePoint = pos;
		}
		break;
	default:
		return false;
	}
	if(m_bStart)
	{
		(*m_Colors)[0] = m_color;
		m_DrawArrays->set(osg::PrimitiveSet::LINES, 0, m_pVertexs->size());
		m_pLine->dirtyDisplayList();
		m_pLine->dirtyBound();
		return true;
	}
	return true;
}

void DrawClipLine::CreateLineGeometry()
{
	if(m_LineUsage == NORMAL)
	{
		if(m_pVertexs->size() > 2 && m_FileName.length() > 0)
		{
			osg::Vec3 p1 = m_pVertexs->at(0);
			osg::Vec3 p2 = m_pVertexs->at(1);

			//goto_gis::Proj4Transform proj4;
			//proj4.setSourceCRS(m_pLayer->CoordTrans()->getDesCRS());
			//proj4.setDestCRS(m_pLayer->CoordTrans()->getSourceCRS());
			//osg::Vec3 p11 = proj4.Transform(p1);
			//osg::Vec3 p22 = proj4.Transform(p2);

			QFileInfo fi(m_FileName);

			QByteArray byteArray = m_FileName.toLocal8Bit();
			char* pData = byteArray.data();
			std::string stdStr = pData;
			QImage* pImage = m_pCI->GetSectionNcFile(stdStr, p1, p2);

			if(s_pSectionImageDialog == nullptr)
			{
				s_pSectionImageDialog = new SectionImageDialog();
				s_pSectionImageDialog->setWindowFlags(s_pSectionImageDialog->windowFlags() | Qt::WindowStaysOnTopHint);
			}

			s_pSectionImageDialog->show();
			s_pSectionImageDialog->setWindowTitle(fi.baseName() + QString::fromLocal8Bit("ÆÊÃæÍ¼"));

			QPixmap pixmap;
			pixmap = QPixmap::fromImage(*pImage);

			s_pSectionImageDialog->UpdateImage(pixmap);
			s_pSectionImageDialog->UpdateRelevantValue(m_pCI, m_FileName, p1, p2);
			//m_KeySwitch->selectMatrixManipulator(0);
			delete pImage;
			//dialog.exec();


		}
	}
	else if(m_LineUsage == DISTANCE)
	{
		m_KmText->setText("");
	}
	else if(m_LineUsage == MOVE)
	{

	}
	else
	{
		if(m_pVertexs->size() > 2)
		{
			osg::Vec3 p1 = m_pVertexs->at(0);
			osg::Vec3 p2 = m_pVertexs->at(1);

			goto_gis::CoordinateTransform*  pCT = this->m_pLayer->CoordTrans();
			goto_gis::Proj4Transform proj4;
			proj4.setSourceCRS(m_pLayer->CoordTrans()->getDesCRS());
			proj4.setDestCRS(m_pLayer->CoordTrans()->getSourceCRS());
			osg::Vec3 p11 = proj4.Transform(p1);
			osg::Vec3 p22 = proj4.Transform(p2);

			QVector<cube_data> boxList;
			m_pGetBoxList->GetSelectTrackBoxList(boxList);
			QLineF clipLine(p11.x(), p11.y(), p22.x(), p22.y());

			if(boxList.size() != 0)
			{
				for(int i = 0; i < boxList.size(); i++)
				{
					QVector<QLineF> lines;
					QLineF leftLine, bottomLine, rightLine, topLine;
					cube_data  cb = boxList.at(i);
					leftLine.setLine(cb.left_top_lon, cb.left_top_lat, cb.right_bottom_lon, cb.left_top_lat);lines.push_back(leftLine);
					bottomLine.setLine(cb.right_bottom_lon, cb.left_top_lat, cb.right_bottom_lon, cb.right_bottom_lat);lines.push_back(bottomLine);
					rightLine.setLine(cb.right_bottom_lon, cb.right_bottom_lat, cb.left_top_lon, cb.right_bottom_lat);lines.push_back(rightLine);
					topLine.setLine(cb.left_top_lon, cb.right_bottom_lat, cb.left_top_lon, cb.left_top_lat);lines.push_back(topLine);
					QVector<QPointF> points;
					for(int j = 0; j < 4; j ++)
					{
						QPointF intersectedPoint;
						QLineF::IntersectType pp = lines.at(j).intersect(clipLine, &intersectedPoint);
						if(pp == QLineF::BoundedIntersection)
						{
							points.push_back(intersectedPoint);
						}
					}

					if(points.size() >= 2)
					{
						cb.point1X = points.at(0).x();
						cb.point1Y = points.at(0).y();
						cb.point2X = points.at(points.size() - 1).x();
						cb.point2Y = points.at(points.size() - 1).y();
						g_GlobleConfig.ConfigCubes(1, cb.name, &cb);
						g_GlobleConfig.Write();
					}
				}

			}
		}

	}
	
	m_DrawArrays->set(osg::PrimitiveSet::LINES, 0, 0);
	m_pLine->dirtyDisplayList();
	m_pLine->dirtyBound();
}

void DrawClipLine::SetLineUsage(DrawClipLine::LINE_USAGE lu)
{
	m_LineUsage = lu;

	if(m_LineUsage == DISTANCE)
	{
		osg::Geode* pGeode = m_DrawNode->asGeode();
		//pGeode->addDrawable(m_ValueText);
		pGeode->addDrawable(m_KmText);
	}
	else
	{
		osg::Geode* pGeode = m_DrawNode->asGeode();
		//pGeode->addDrawable(m_ValueText);
		pGeode->removeDrawable(m_KmText);
	}
}

void DrawClipLine::SaveRectChange(const QString& name, osg::Vec3 p1, osg::Vec3 p2)
{
	cube_data cd;
	cd = g_GlobleConfig.GetCubeFromName(name);

	goto_gis::CoordinateTransform*  pCT = this->m_pLayer->CoordTrans();
	goto_gis::Proj4Transform proj4;
	proj4.setSourceCRS(m_pLayer->CoordTrans()->getDesCRS());
	proj4.setDestCRS(m_pLayer->CoordTrans()->getSourceCRS());
	osg::Vec3 p11 = proj4.Transform(p1);
	osg::Vec3 p22 = proj4.Transform(p2);

	cd.left_top_lon = p11.x();
	cd.left_top_lat = p11.y();
	cd.right_bottom_lon = p22.x();
	cd.right_bottom_lat = p22.y();
	g_GlobleConfig.ConfigCubes(1, name, &cd);
	g_GlobleConfig.Write();
}

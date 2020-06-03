#include "StdAfx.h"
#include "DrawTrackingBoxPost.h"
#include "CubeSettingsDialogImps.h"
#include "Proj4Transform.h"
#include <QMessageBox>
#include "MainWindow.h"

extern MainWindow* g_MainWindow;
extern ConfigAccesser g_GlobleConfig;

DrawTrackingBoxPost::DrawTrackingBoxPost(void)
{
}


DrawTrackingBoxPost::~DrawTrackingBoxPost(void)
{
}

void DrawTrackingBoxPost::FinishDraw( const osg::Vec3Array* pVec3Array )
{
	CubeSettingsDialogImps dsd;
	QString strName = g_MainWindow->GetSelectFileName();
	if(strName.size() == 0)
	{
		strName = "NewTrackBox";
	}
	dsd.SetCubeName(strName);
	dsd.SetMode(0);

	osg::Vec3 v3LeftTop = pVec3Array->at(2);
	osg::Vec3 v3RightBottom = pVec3Array->at(3);
	
	osg::Vec3 v3LeftTopEarth;
	osg::Vec3 v3RightBottomEarth;

	goto_gis::Proj4Transform proj4;
	proj4.setSourceCRS(m_DrawLayer->CoordTrans()->getDesCRS());
	proj4.setDestCRS(m_DrawLayer->CoordTrans()->getSourceCRS());
	v3LeftTopEarth = proj4.Transform(v3LeftTop);
	v3RightBottomEarth = proj4.Transform(v3RightBottom);

	dsd.m_Setup.LeftTopLonlineEdit->setText(QString("%1").arg(v3LeftTopEarth.x()));
	dsd.m_Setup.LeftTopLatlineEdit->setText(QString("%1").arg(v3LeftTopEarth.y()));
	dsd.m_Setup.RightBottonLonlineEdit->setText(QString("%1").arg(v3RightBottomEarth.x()));
	dsd.m_Setup.RightBottonLatlineEdit->setText(QString("%1").arg(v3RightBottomEarth.y()));
	dsd.exec();
	
	osg::Group* pGroup = m_DrawLayer->GetLayerNode()->asGroup();
	unsigned int iCount = pGroup->getNumChildren();
	unsigned int iLastIndex = iCount - 1;

	if(iCount > 1)
	{
		osg::Geode* pGeode = pGroup->getChild(0)->asGeode();

		osg::Geometry* pGeome = pGeode->getDrawable(0)->asGeometry();
		osg::Vec3Array* pVertexes= dynamic_cast<osg::Vec3Array*>(pGeome->getVertexArray());
		if(pVertexes)
		{
			pVertexes->clear();
			/*pVertexes->push_back(osg::Vec3(0,0,0));
			pVertexes->push_back(osg::Vec3(0,0,0));
			pVertexes->push_back(osg::Vec3(0,0,0));
			pVertexes->push_back(osg::Vec3(0,0,0));*/
		}

		DrawArrays* pDA = dynamic_cast<DrawArrays*>(pGeome->getPrimitiveSet(0));
		pDA->setCount(0);
		//pGeome->removePrimitiveSet(0);
		pGeome->dirtyDisplayList();
		pGeome->dirtyBound();
	}
	pGroup->removeChild(iLastIndex);

	g_MainWindow->FlushCubesList();
	g_GlobleConfig.Write();

	g_MainWindow->SwitchDefalultMaipulator();
	/*QMessageBox::StandardButton button = QMessageBox::information(
		0, QString::fromLocal8Bit("»æÖÆ¸ú×Ù¿ò"), QString::fromLocal8Bit("ÊÇ·ñ¼ÌÐø»æÖÆ¸ú×Ù¿ò¡£")
		, QMessageBox::Yes, QMessageBox::No);

	if(button == QMessageBox::Yes)
	{
		return;
	}
	else
	{
		g_MainWindow->StartTrack2();
	}*/

}

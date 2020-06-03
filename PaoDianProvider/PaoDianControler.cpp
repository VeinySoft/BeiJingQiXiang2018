#include "StdAfx.h"
#include "PaoDianControler.h"


PaoDianControler::PaoDianControler(void) : m_pPaoDianGeometry(0)
{
	
}

PaoDianControler::~PaoDianControler(void)
{
}

NodeInfo* PaoDianControler::GetNodeFromID( int id )
{
	return 0;
}

void PaoDianControler::DisplayFromIDs( int id )
{
	m_pPaoDianGeometry->VisblePaoDian(id, true);
}

void PaoDianControler::HideFromIDs( int id )
{
	m_pPaoDianGeometry->VisblePaoDian(id, false);
}

QStandardItemModel* PaoDianControler::GetModel()
{
	return &m_StandardItemModel;
}

void PaoDianControler::HideAll()
{
	m_pPaoDianGeometry->VisbleAll(false);
}

void PaoDianControler::DisplayAll()
{
	m_pPaoDianGeometry->VisbleAll(true);
}

void PaoDianControler::GetPaoDianBoundBox( QVector3D* p1, QVector3D* p2, QVector3D* p3, QVector3D* p4 )
{
	osg::BoundingBox box;
	m_pPaoDianGeometry->GetBoundingBox(&box);

	osg::Vec3 v3p0 = box.corner(0);//×óÏÂ½Ç
	osg::Vec3 v3p1 = box.corner(1);
	//osg::Vec3 v3p2 = box.corner(2);
	//osg::Vec3 v3p3 = box.corner(3);
	osg::Vec3 v3p4 = box.corner(4);
	osg::Vec3 v3p5 = box.corner(5);
	//osg::Vec3 v3p6 = box.corner(6);
	//osg::Vec3 v3p7 = box.corner(7);

	p1->setX(v3p0.x());
	p1->setY(v3p0.y());
	p1->setZ(v3p0.z());

	p2->setX(v3p1.x());
	p2->setY(v3p1.y());
	p2->setZ(v3p1.z());

	p3->setX(v3p5.x());
	p3->setY(v3p5.y());
	p3->setZ(v3p5.z());

	p4->setX(v3p4.x());
	p4->setY(v3p4.y());
	p4->setZ(v3p4.z());
}

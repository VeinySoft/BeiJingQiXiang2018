#pragma once
#include <DrawLine.h>
#include "GetTrackBoxListInterface.h"

class DrawClipLine :
	public DrawLine
{
public:
	enum LINE_USAGE{NORMAL, STACK, DISTANCE, MOVE};
	DrawClipLine(void);
	virtual ~DrawClipLine(void);
	ControlorInterface* m_pCI;
	
	osgGA::KeySwitchMatrixManipulator* m_KeySwitch;
	bool handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea );
	//void SetLineMode(bool bM);
	inline void SetFileName(const QString& fileName){m_FileName = fileName;}
	void SetLineUsage(DrawClipLine::LINE_USAGE lu);
	inline void SetBoxListInterface(GetTrackBoxListInterface* pFBLI){m_pGetBoxList = pFBLI;}
	inline void SetCurrentRectName(const QString& name){m_strRectName = name;}
	void SaveRectChange(const QString& name, osg::Vec3 p1, osg::Vec3 p4);

protected:
	QString m_FileName;
	virtual void CreateLineGeometry();
	void CreateText(osg::ref_ptr<osgText::Text>& tt);
	osg::ref_ptr<osgText::Text> m_ValueText;
	osg::ref_ptr<osgText::Text> m_KmText;
	DrawClipLine::LINE_USAGE m_LineUsage;
	GetTrackBoxListInterface* m_pGetBoxList;
	osg::Vec3 m_PrePoint;
	QString m_strRectName;
};


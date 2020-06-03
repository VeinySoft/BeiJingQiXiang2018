#pragma once

class MainWindow;

class DrawTrackingBoxPost :
	public RectResult
{
public:
	DrawTrackingBoxPost(void);
	~DrawTrackingBoxPost(void);

	virtual void FinishDraw( const osg::Vec3Array* pVec3Array );
	goto_gis::Layer* m_DrawLayer;
};


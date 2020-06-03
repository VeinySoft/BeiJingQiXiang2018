#include "MyImageSequence.h"
#include "RadarGeometry.h"

int MyImageSequence::getCurrentImage()
{
	return osg::ImageSequence::imageIndex(_seekTime);
}

MyImageSequence::~MyImageSequence()
{

}

void MyImageSequence::update( osg::NodeVisitor* nv )
{
	//osg::ImageSequence::update(nv);
	/*static int i = 0;
	if(i == 3)
	{
		i = 0;
	}

	setImageToChild(i);*/
	
	//if(this->getNumImageData() > 0)
	//{
	//	const osg::FrameStamp* fs = nv->getFrameStamp();
	//	double time = (fs->getSimulationTime() - _referenceTime)*_timeMultiplier;
	//	//int iIndex = getCurrentImage();
	//	int iIndex = imageIndex(time);
	//	m_pRadarGeometry->SetRect(iIndex);
	//}
	//i++;

	
}

void MyImageSequence::setMyChildImage( int pos )
{
	setImageToChild(pos);
}

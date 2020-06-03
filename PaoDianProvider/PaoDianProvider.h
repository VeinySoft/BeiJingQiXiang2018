#include "DataProvider.h"
#include "PaoDinaControlerInterface.h"
#include "PaoDianControler.h"
class PaoDianProvider : public goto_gis::DataProvider
{

public:
	PaoDianProvider();
	virtual bool LoadFile( const std::string& filename );

	virtual void AreaAdd( bool tAra,bool tPie,float tMin,float tMax,osg::Vec3Array* p );

private:
	PaoDianControler* m_pPaoDianControler;

};
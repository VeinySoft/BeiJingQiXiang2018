#pragma once

#include "MakeTextureInterface.h"
#include "DataProvider.h"
#include "RadarGeometry.h"

class ControlorInterface;
class SetCubesInterface;

class NetCDFDataProvider : public goto_gis::DataProvider
{
public:
	NetCDFDataProvider();
	~NetCDFDataProvider();
	virtual bool LoadFile(const std::string& filePath);
	virtual void AreaAdd( bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p );
	virtual bool MineData( const std::string& strIns = "" );
	inline void SetCubes(const std::vector<CUBE_DATE> cs){m_Cubes = cs;}
	bool LoadOneFile(const std::string& filePath);
private:
	friend class ControlorImps;
	friend class SetCubesImps;
	
	SetCubesInterface* m_pSetCubesInterface;
	ControlorInterface* m_pControlorInterface;
	RadarGeometry* m_pRadarGeometry;
	std::vector<CUBE_DATE> m_Cubes;
	TREC_NC_IN_VECTOR m_tniv;
	std::vector<std::string> m_FileNames;
};


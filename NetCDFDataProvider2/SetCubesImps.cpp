#include "SetCubesImps.h"
#include "NetCDFDataProvider.h"

SetCubesImps::SetCubesImps(NetCDFDataProvider* pNetcdfPro) 
	: m_pNetCDFDataProvider(pNetcdfPro)
{
}


SetCubesImps::~SetCubesImps(void)
{
}

void SetCubesImps::SetCubes( std::vector<CUBE_DATE>& cubes )
{
	m_pNetCDFDataProvider->m_Cubes = cubes;
}

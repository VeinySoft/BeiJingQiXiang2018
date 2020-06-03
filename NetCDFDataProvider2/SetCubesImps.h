#pragma once
#include "setcubesinterface.h"
class NetCDFDataProvider;
class SetCubesImps :
	public SetCubesInterface
{
public:
	SetCubesImps(NetCDFDataProvider* pNetcdfPro);
	~SetCubesImps(void);

	virtual void SetCubes( std::vector<CUBE_DATE>& cubes );
private:
	NetCDFDataProvider* m_pNetCDFDataProvider;
	
};


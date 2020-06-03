#include "StdAfx.h"
#include "FlightPathControlerImps.h"
#include "FlightPathProvider.h"

FlightPathControlerImps::FlightPathControlerImps(FlightPathProvider* pFPP) : m_pFlightPathProvider(pFPP)
{
}

FlightPathControlerImps::~FlightPathControlerImps(void)
{
}

QStringList& FlightPathControlerImps::GetDateList()
{
	return m_pFlightPathProvider->GetDataList();
}

QStringList& FlightPathControlerImps::GetTimeList()
{
	return m_pFlightPathProvider->GetTimeList();
}

osg::ref_ptr<osg::Vec3Array>& FlightPathControlerImps::GetFlightPath()
{
	return m_pFlightPathProvider->GetLonLatArray();
}

void FlightPathControlerImps::SetFlightPathPartColor(const osg::Vec4& color, size_t iStart, size_t iCount)
{
	m_pFlightPathProvider->GetFlightPath()->SetPartColor(color, iStart, iCount);
}
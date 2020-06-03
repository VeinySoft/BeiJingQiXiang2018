#include "StdAfx.h"
#include "ParseMemoryRasterData.h"
#include "MemoryData.h"

ParseMemoryRasterData::ParseMemoryRasterData(void)
{
	ParseData::m_pData = 0;
	ParseData::m_OtherData = 0;
}

ParseMemoryRasterData::~ParseMemoryRasterData(void)
{
}

bool ParseMemoryRasterData::ReadDataFromFile(const std::string& strFielName)
{
	return false;
}

bool ParseMemoryRasterData::ReadDataFromMemory(const void* pData)
{
	MemoryData* pMD = (MemoryData*)pData;
	
	m_pData = pMD->pData;
	m_DataSize = pMD->DataSize;
	m_ColumnCount = pMD->ColumnCount;
	m_fXMin = pMD->fXMin;
	m_fXMax = pMD->fXMax;
	m_fYMin = pMD->fYMin;
	m_fYMax = pMD->fYMax;
	m_fZMin = pMD->fZMin;
	m_fZMax = pMD->fZMax;
	m_fXInterval = pMD->fXInterval;
	m_fYInterval = pMD->fYInterval;
	m_fZInterval = pMD->fZInterval;

	m_strXComment = pMD->strXComment;
	m_strYComment = pMD->strYComment;
	m_strZComment = pMD->strZComment;

	m_strTtile = pMD->strTtile;

	return false;
}
ParseData::DataType ParseMemoryRasterData::GetDataType()
{
	return RASTER;
}
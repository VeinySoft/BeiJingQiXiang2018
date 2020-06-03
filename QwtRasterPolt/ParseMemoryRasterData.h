#pragma once
#include "ParseData.h"

class ParseMemoryRasterData : public ParseData
{
public:
	ParseMemoryRasterData(void);
	~ParseMemoryRasterData(void);
	virtual bool ReadDataFromFile(const std::string& strFielName);
	virtual bool ReadDataFromMemory(const void* pData);
	virtual ParseData::DataType GetDataType();
private:
	QStringList m_strDate;
	QStringList m_strTime;
};


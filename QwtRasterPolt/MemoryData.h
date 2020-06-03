#pragma once
#include <qstringlist.h>
#include <qvector.h>

struct MemoryData
{
	double* pData;
	size_t DataSize;
	int ColumnCount;
	double fXMin;
	double fXMax;
	double fYMin;
	double fYMax;
	double fZMin;
	double fZMax;
	double fXInterval;
	double fYInterval;
	double fZInterval;

	std::string strXComment;
	std::string strYComment;
	std::string strZComment;

	std::string strTtile;

	//AdditionalData* OtherData;
};


#pragma once
#include <map>
#include <vector>

class NcFile;

typedef struct __CubeData___
{
	float fTargetFstLon;	
	float fTargetLstLon;
	float fTargetFstLat;
	float fTargetLstLat;
	int iHeight;
	int iHeightUp;
	int iHeightDown;
	std::vector<float> refs;
} CUBE_DATE, *P_CUBE_DATA;

//速度
typedef struct Speed
{
	float u;
	float v;
	float Nlat;
	float NLon;
	float Elat;
	float Elon;

} SPEED, *P_SPEED;

//回波比例
typedef struct __ratio__
{
	std::vector<float> interval;
	std::vector<float> ratio;
	std::vector<int> number;
} RATIO, *P_RATIO;

//降水量和水含量
typedef struct __precipitation__
{
	float precipitation;
	float content;
} PRECIPITATION, *P_PRECIPITATION;

//
typedef struct __Algorithm_Out__
{
	NcFile* file;
	SPEED speed;
	RATIO ratio;
	PRECIPITATION precipitation;

} TREC_OUT, *P_TREC_OUT;

typedef std::map<NcFile*, TREC_OUT> TREC_NC_MAP_OUT, *P_TREC_NC_MAP_OUT;//NcFile指针和输出结果一一对应
typedef std::vector<NcFile*> TREC_NC_IN_VECTOR, *P_TREC_NC_IN_VECTOR;

//void TrecInput(const P_TREC_NC_IN_VECTOR pFileVector, const P_CUBE_DATA pCube, P_TREC_NC_MAP_OUT pOutMap);

typedef bool (* _TrecAlgorithm) (const P_TREC_NC_IN_VECTOR pFileVector, const P_CUBE_DATA pCube, P_TREC_NC_MAP_OUT pOutMap);
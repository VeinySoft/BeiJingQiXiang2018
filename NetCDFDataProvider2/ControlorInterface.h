#pragma once
#include "DataObtainInterface.h"
#include <osg/Vec3>
#include <osg/Vec4>
#include <QString>
#include <QStringList>
#include <QColor>
#include <QIMage>

class TrecCallback
{
public:
	virtual void TrecDone(QStringList& fileList, QString& strCubeName, P_TREC_NC_MAP_OUT pOutMap) = 0;
};

class ControlorInterface
{
public:
	virtual void startTrack(int i, const std::vector<std::string>& fileList) = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual void prev() = 0;
	virtual void next() = 0;
	virtual void runTrec(TrecCallback* pTC, std::vector<std::string>& files, const std::map<std::string, CUBE_DATE>& cubes) = 0;
	virtual void clearTrec() = 0;
	virtual void LoadNcFile(std::string& name) = 0;
	virtual void DeleteNcFile(std::string& name) = 0;
	virtual void FileToName(std::string& name, void** id) = 0;
	virtual void AddRect(const std::string& name, osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4, osg::Vec4& color, const std::string& strName = "") = 0;
	virtual void GetFileNames(std::vector<std::string>& names) = 0;
	virtual void DispalyFile(const std::string& fileName) = 0;
	virtual void OpenNC(const std::string& fileName) = 0;
	virtual P_TREC_NC_MAP_OUT runTrec(std::string& files1, std::string& file2, CUBE_DATE& cd) = 0;
	virtual int GetValue(int iI) = 0;
	virtual int GetIndexFromValue(int iV) = 0;
	virtual QColor GetColor(int iI) = 0;
	virtual void SeleteLayer(int iL) = 0;
	virtual void HideRects(const std::string& cubeName, bool bHide) = 0;
	virtual void RemoveRect(const std::string& cubeName) = 0;
	virtual void MoveRect(const std::string& name, osg::Matrix m) = 0;
	virtual void RemoveAllRects() = 0;
	virtual void RemoveTrackBox() = 0;
	virtual void DisplayNcFile(const std::string& strName) = 0;
	virtual void SetRectColor(const std::string& strName, int i, const osg::Vec4& color) = 0;
	virtual QImage* ClipNcFile(const std::string& strName, osg::Vec3 p1, osg::Vec3 p2, int iMode, int upIndex = 0, int downIndex = 0, float* fMaxHeight = 0, float* fVolume = 0, float* fMaxDBZ = 0) = 0;
	virtual QImage* GetSectionNcFile( const std::string& strName, osg::Vec3 p1, osg::Vec3 p2 ) = 0;
	virtual int GetRectVertex(const std::string& strName, osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4) = 0;
	//virtual void SectionFile(std::string& name);
};


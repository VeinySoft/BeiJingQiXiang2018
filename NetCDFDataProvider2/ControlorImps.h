#pragma once
#include "controlorinterface.h"
#include "MyImageSequence.h"

#include <QtCore/QPointer>
#include <QtCore/QThreadPool>
#include <QtCore/QThread>
#include <QtCore/QVector>
#include <QtCore/QMutex>

#include "InvokeTrecThread.h"

class NetCDFDataProvider;
class NetCDFDataProvider;
class RadarGeometry;
class ControlorImps;
class MakeTextureInterface;

class WaitThread : public QThread
{
public:
	virtual void run()
	{
		for(int i = 0; i < m_Runables.size(); i++)
		{
			InvokeTrecThread* pITT = m_Runables.at(i);
			pITT->SetCubeName(&m_strCubeName);
			pITT->SetFileList(&m_strFileList);
			m_pPool->start(pITT);
		}
		
		m_pPool->waitForDone();
		if(m_pTrecCllback)
			m_pTrecCllback->TrecDone(m_strFileList, m_strCubeName, m_pOut);
		m_pPool->releaseThread();
		//delete m_pOut;
	}
	
	QVector<InvokeTrecThread*> m_Runables;
	P_TREC_NC_MAP_OUT m_pOut;
	TrecCallback* m_pTrecCllback;
	QThreadPool* m_pPool;
	QString m_strCubeName;
	QStringList m_strFileList;
};

class ControlorImps :
	public ControlorInterface
{
public:
	ControlorImps(NetCDFDataProvider* pNetCDFDataProvider);
	~ControlorImps(void);
	virtual void OpenNC(const std::string& fileName);
	virtual P_TREC_NC_MAP_OUT runTrec(std::string& files1, std::string& file2, CUBE_DATE& cd);

	virtual void start();

	virtual void stop();

	virtual void pause();

	virtual void resume();

	virtual void runTrec(TrecCallback* pTC, std::vector<std::string>& files, const std::map<std::string, CUBE_DATE>& cubes);

	virtual void DeleteNcFile(std::string& name);

	virtual void LoadNcFile(std::string& name);

	virtual void FileToName( std::string& name, void** id );

	virtual void clearTrec();

	virtual void AddRect( const std::string& name, osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4, osg::Vec4& color, const std::string& strName = "" );
	//inline void SetRadarGeometry(RadarGeometry* pRadarGeometry){m_pRadarGeometry = pRadarGeometry;}

	virtual void prev();

	virtual void next();

	virtual void GetFileNames( std::vector<std::string>& names );

	inline void setTrackingMode(int iTM){m_iTrackingMode = iTM;}
	inline void setHistoryFile(const std::vector<std::string>& fileList){m_HistoryFile = fileList;}

	virtual void startTrack( int i, const std::vector<std::string>& fileList );

	virtual void DispalyFile( const std::string& fileName );

	int GetValue(int iI);
	int GetIndexFromValue(int iV);
	QColor GetColor(int iI);

	inline int GetLayer(){return m_Layer;}
	inline void SeleteLayer(int iL){m_Layer = iL;}

	virtual void HideRects( const std::string& cubeName, bool bHide );

	virtual void RemoveRect( const std::string& cubeName );

	virtual void MoveRect(const std::string& name, osg::Matrix m);

	virtual void RemoveAllRects();

	virtual void RemoveTrackBox();

	virtual void DisplayNcFile(const std::string& strName);
	virtual void SetRectColor(const std::string& strName, int i, const osg::Vec4& color);

	virtual QImage* ClipNcFile(const std::string& strName, osg::Vec3 p1, osg::Vec3 p2, int iMode, int upIndex = 0, int downIndex = 0, float* fMaxHeight = 0, float* fVolume = 0, float* fMaxDBZ = 0);
	virtual QImage*  GetSectionNcFile( const std::string& strName, osg::Vec3 p1, osg::Vec3 p2 );
	virtual int GetRectVertex(const std::string& strName, osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4);

	virtual int ExportPartNcFile(const QString& strName, osg::Vec3 p1, osg::Vec3 p2, int iMode, int upIndex, int downIndex, const QString& strExortFile);
	virtual int ExportSctionFile(const QString& strName, osg::Vec3 p1, osg::Vec3 p2, int iMode, const QString& strExortFile);

	virtual int GetVerticalData(const std::string& strName, osg::Vec3Array& vec3Array, osg::Vec3 p2, double* pData, size_t* pSize);
private:
	NetCDFDataProvider* m_pNetCDFDataProvider;
	//RadarGeometry* m_pRadarGeometry;
	QPointer<WaitThread> m_pWaitThread;
	QPointer<QThreadPool> m_pTrecAlgorithmPool;
	P_TREC_NC_MAP_OUT m_pResult;
	double m_time;
	std::vector<std::string> m_HistoryFile;
	int m_iTrackingMode;
	std::map<std::string, NcFile*> m_FileNameToNcFile;
	MakeTextureInterface* m_pMakeTexture;
	QMutex m_Mutex;
	QMutex m_CacheMutex;
	int m_Layer;

	struct RadarImage
	{
		osg::ref_ptr<osg::Image> pTexture;
		osg::ref_ptr<osg::Vec3Array> pV3Array;
	};

	struct RadarGeometry
	{
		osg::ref_ptr<osg::Vec3Array> pVertexs;
		osg::ref_ptr<osg::Vec4Array> pColors;
	};
	std::map<std::string, RadarImage> m_CacheOpenFile;
	std::map<std::string, RadarGeometry> m_CacheOpenFile2;
};


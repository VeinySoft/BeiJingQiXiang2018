#include "DataObtainInterface.h"
#include "NetCDFDataProvider.h"
#include "../NetCDF/cxx/netcdfcpp.h"
#include "RadarGeometry.h"
#include "InvokeTrecThread.h"
#include "MakeTextureImps.h"
#include "RadarGeometry.h"
#include "ControlorImps.h"

extern _TrecAlgorithm	g_pTrecAlgorithm;
extern QColor gColorTable[];
extern short gValue[];

ControlorImps::ControlorImps(NetCDFDataProvider* pNetCDFDataProvider) 
	: m_pNetCDFDataProvider(pNetCDFDataProvider)
	, m_pWaitThread(new WaitThread)
	, m_pTrecAlgorithmPool(new QThreadPool())
	, m_time(0)
	, m_Layer(-1)
{
	m_pMakeTexture = new MakeTextureImps(0);
}


ControlorImps::~ControlorImps(void)
{
}

void ControlorImps::start()
{
	m_pNetCDFDataProvider->m_pRadarGeometry->m_pImageSequence->play();
}

void ControlorImps::stop()
{
	m_pNetCDFDataProvider->m_pRadarGeometry->m_pImageSequence->pause();
}

void ControlorImps::pause()
{
	m_pNetCDFDataProvider->m_pRadarGeometry->m_pImageSequence->pause();
}

void ControlorImps::resume()
{
	m_pNetCDFDataProvider->m_pRadarGeometry->m_pImageSequence->play();
}
#include <QDebug>
void ControlorImps::runTrec( TrecCallback* pTC, std::vector<std::string>& files, const std::map<std::string, CUBE_DATE>& cubes )
{
#if 0
	//Ó¦ÓÃËã·¨
	/*if(m_pNetCDFDataProvider->m_Cubes.size() > 0 && m_pNetCDFDataProvider->m_tniv.size() > 1)
	{
		TREC_NC_MAP_OUT Result;
		CUBE_DATE cubes = m_pNetCDFDataProvider->m_Cubes[0];

		g_pTrecAlgorithm(&(m_pNetCDFDataProvider->m_tniv), &cubes, &Result);

		for(int i = 0; i < m_pNetCDFDataProvider->m_tniv.size(); i++)
		{
			NcFile* pNcFile = m_pNetCDFDataProvider->m_tniv.at(i);
			pNcFile->close();
		}
	}*/
	osg::ref_ptr<osg::Image> pTexture;
	osg::ref_ptr<osg::Vec3Array> pV3Array;

	QStringList fileslist;
	for(int i = 0; i < files.size(); i++)
	{
		NcFile* pNcFile = new NcFile(files[i].c_str());

		fileslist.push_back(QString::fromStdString(files[i]));
	}

	m_pMakeTexture->loadNcFileToTexture(pNcFile, pTexture, pV3Array);

	m_pResult = new TREC_NC_MAP_OUT;

	std::map<std::string, CUBE_DATE>::const_iterator be = cubes.begin();
	for(be; be != cubes.end(); be++)
	{
		std::string cubeName = be->first;
		CUBE_DATE cd = be->second;
		osg::Vec3 p1, p2, p3, p4;
		p1.set(cd.fTargetFstLon, cd.fTargetLstLat, 0);
		p2.set(cd.fTargetLstLon, cd.fTargetLstLat, 0);
		p3.set(cd.fTargetLstLon, cd.fTargetFstLat, 0);
		p4.set(cd.fTargetFstLon, cd.fTargetFstLat, 0);

		m_pRadarGeometry->AddRect(p1, p2, p3, p4, osg::Vec4(1.0, 0,0 ,1));
		InvokeTrecThread* pITT = new InvokeTrecThread(
			, &cd, m_pResult);
		pITT->SetFileList(fileslist);
		pITT->SetCubeName(QString::fromStdString(cubeName));
		pITT->setAutoDelete(true);

		m_pWaitThread->m_Runables.push_back(pITT);
	}

	m_pWaitThread->m_pOut = m_pResult;
	m_pWaitThread->m_pTrecCllback = pTC;
	m_pWaitThread->m_pPool = m_pTrecAlgorithmPool;
	m_pWaitThread->start();
#endif
}

P_TREC_NC_MAP_OUT ControlorImps::runTrec( std::string& file1, std::string& file2, CUBE_DATE& cd )
{
	P_TREC_NC_MAP_OUT pOutMap = new TREC_NC_MAP_OUT;
#if 0
	m_Mutex.lock();
	std::map<std::string, NcFile*>::iterator finded1 = m_FileNameToNcFile.find(file1);
	std::map<std::string, NcFile*>::iterator finded2 = m_FileNameToNcFile.find(file2);

	if(finded1 == m_FileNameToNcFile.end() || finded2 == m_FileNameToNcFile.end()) 
	{
		m_Mutex.unlock();
		return 0;
	}
	m_Mutex.unlock();
#endif
	m_Mutex.lock();
	NcFile* pNcFile1 = new NcFile(file1.c_str());
	NcFile* pNcFile2 = new NcFile(file2.c_str());
	TREC_NC_IN_VECTOR tniv;
	tniv.push_back(pNcFile1);
	tniv.push_back(pNcFile2);

	//qDebug()<<file1.c_str();
	//qDebug()<<file2.c_str();

	g_pTrecAlgorithm(&tniv, &cd, pOutMap);


	pNcFile1->close();
	pNcFile2->close();

	delete pNcFile1;
	delete pNcFile2;
	m_Mutex.unlock();
	return pOutMap;
}

void ControlorImps::OpenNC( const std::string& fileName )
{
	/*NcFile* pNcFile = new NcFile(fileName.c_str());
	m_Mutex.lock();
	m_FileNameToNcFile[fileName] = pNcFile;
	m_Mutex.unlock();*/
}


void ControlorImps::LoadNcFile( std::string& fileName )
{
#if 0
	std::map<std::string, NcFile*>::iterator finded2 = m_FileNameToNcFile.find(fileName);

	if(finded2 == m_FileNameToNcFile.end())
	{
		OpenNC(fileName);
	}
#endif
	int iRepeat = 3;

REPEAT_POINT:
	NcFile* pNcFile = new NcFile(fileName.c_str());
	osg::ref_ptr<osg::Image> pTexture;
	osg::ref_ptr<osg::Vec3Array> pV3Array;

#if 0
	RadarImage radarImage;
	
	if(m_Layer == -1)
	{
		//QImage* pImage = m_pMakeTexture->loadNcFileToTexture(pNcFile, pV3Array);
		//pImage->save("./temp.bmp");
		//pTexture = osgDB::readImageFile("./temp.bmp");
		m_pMakeTexture->loadNcFileToTexture(pNcFile, pTexture, pV3Array);
	}
	else
	{
		m_pMakeTexture->loadNcFileToTexture(pNcFile, pTexture, pV3Array, m_Layer);
	}
	
	radarImage.pV3Array = pV3Array;
	radarImage.pTexture = pTexture;

	pNcFile->close();
	delete pNcFile;

	m_CacheMutex.lock();
	m_CacheOpenFile[fileName] = radarImage;
	m_CacheMutex.unlock();
#else
	RadarGeometry rg;

	osg::ref_ptr<osg::Vec3Array> vs;
	osg::ref_ptr<osg::Vec4Array> cs;

	if(pNcFile->is_valid())
	{
		iRepeat = 3;
		m_pMakeTexture->loadNcFileToGeometry(pNcFile, vs, cs, m_pNetCDFDataProvider->m_pParentLayer->CoordTrans(), m_Layer);
	}
	else
	{
		pNcFile->close();
		delete pNcFile;

		if(iRepeat != 0)
		{
			iRepeat--;
			goto REPEAT_POINT;
		}
		else
			return;
	}

	rg.pVertexs = vs;
	rg.pColors = cs;

	m_CacheMutex.lock();
	m_CacheOpenFile2[fileName] = rg;
	qDebug()<<"LoadNcFile"<<fileName.c_str()<<endl;
	m_CacheMutex.unlock();
	pNcFile->close();
	delete pNcFile;
#endif
}

void ControlorImps::DeleteNcFile(std::string& name)
{
	/*m_Mutex.lock();
	std::map<std::string, NcFile*>::iterator finded1 = m_FileNameToNcFile.find(name);
	std::map<std::string, NcFile*>::iterator endIter = m_FileNameToNcFile.end();

	if(finded1 == endIter)
	{
		finded1->second->close();
		delete finded1->second;

		m_FileNameToNcFile.erase(finded1);
	}
	m_Mutex.unlock();*/
}

void ControlorImps::FileToName(std::string& name, void** id)
{
	/*std::map<std::string, NcFile*>::iterator reslut 
		= m_pNetCDFDataProvider->m_FileNameToNcFile.find(name);

	if (reslut != m_pNetCDFDataProvider->m_FileNameToNcFile.end())
	{
		*id = reslut->second;
	}*/
}

void ControlorImps::clearTrec()
{
	//DeleteNcFile();

}

void ControlorImps::AddRect( const std::string& name, osg::Vec3& p1
	, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4, osg::Vec4& color, const std::string& strName )
{
	m_pNetCDFDataProvider->m_pRadarGeometry->AddRect(name, p1, p2, p3, p4, color, strName);
}

void ControlorImps::prev()
{
	m_pNetCDFDataProvider->m_pRadarGeometry->m_pImageSequence->seek(m_time);
	m_time -= 0.01;
}

void ControlorImps::next()
{
	m_pNetCDFDataProvider->m_pRadarGeometry->m_pImageSequence->seek(m_time);
	m_time += 0.05;
}

void ControlorImps::GetFileNames( std::vector<std::string>& names )
{
	names = m_pNetCDFDataProvider->m_FileNames;
}

void ControlorImps::startTrack( int i, const std::vector<std::string>& fileList )
{
	
}

void ControlorImps::DispalyFile( const std::string& fileName )
{
	
}

int ControlorImps::GetValue( int iI )
{
	if(iI >= 0 && iI < 16)
	{
		return gValue[iI];
	}
	return -999;
}

int ControlorImps::GetIndexFromValue( int iV )
{
	for(int i = 0; i < 16; i++)
	{
		if(gValue[i] == iV)
			return i;
	}

	return -1;
}

QColor ControlorImps::GetColor( int iI )
{
	if(iI >= 0 && iI < 16) return gColorTable[iI];
	return QColor(0,0,0);
}

void ControlorImps::HideRects( const std::string& cubeName, bool bHide )
{
	m_pNetCDFDataProvider->m_pRadarGeometry->HideRects(cubeName, bHide);
}

void ControlorImps::RemoveRect( const std::string& cubeName )
{
	m_pNetCDFDataProvider->m_pRadarGeometry->RemoveRects(cubeName);
}

void ControlorImps::RemoveAllRects()
{
	m_pNetCDFDataProvider->m_pRadarGeometry->RemoveAllRect();
}

void ControlorImps::RemoveTrackBox()
{
	m_pNetCDFDataProvider->m_pRadarGeometry->RemoveTrackRect();
}

void ControlorImps::DisplayNcFile( const std::string& strName )
{
	m_CacheMutex.lock();
#if 0
	std::map<std::string, RadarImage>::iterator finded 
		= m_CacheOpenFile.find(strName);

	if(finded != m_CacheOpenFile.end())
	{

		m_pNetCDFDataProvider->m_pRadarGeometry->AddRadarImage(
			finded->second.pV3Array, finded->second.pTexture);

		m_CacheOpenFile.erase(strName);
	}
#else
	std::map<std::string, RadarGeometry>::iterator finded 
		= m_CacheOpenFile2.find(strName);

	if(finded != m_CacheOpenFile2.end())
	{
		qDebug()<<"DisplayNcFile"<<strName.c_str()<<endl;
		m_pNetCDFDataProvider->m_pRadarGeometry->AddRadarGeometry(
			finded->second.pVertexs, finded->second.pColors);

		//finded->second.pVertexs.release();
		//finded->second.pColors.release();
		m_CacheOpenFile2.erase(strName);
	}
#endif
	m_CacheMutex.unlock();
}

void ControlorImps::SetRectColor( const std::string& strName, int i, const osg::Vec4& color )
{
	m_pNetCDFDataProvider->m_pRadarGeometry->SetRectColor(strName, i, color);
}

QImage* ControlorImps::ClipNcFile( const std::string& strName, osg::Vec3 p1, osg::Vec3 p2, int iMode, int upIndex, int downIndex, float* fMaxHeight, float* fVolume, float* fMaxDBZ )
{
	NcFile ncFile(strName.c_str());
	MakeTextureImps mti(m_pNetCDFDataProvider->m_pParentLayer->CoordTrans());
	QImage* pImage = mti.GetPartNcFile(&ncFile, p1, p2, m_Layer, upIndex, downIndex, fMaxHeight, fVolume, fMaxDBZ);
	ncFile.close();
	return pImage;
}

QImage* ControlorImps::GetSectionNcFile( const std::string& strName, osg::Vec3 p1, osg::Vec3 p2 )
{
	NcFile ncFile(strName.c_str());
	MakeTextureImps mti(m_pNetCDFDataProvider->m_pParentLayer->CoordTrans());
	QImage* pImage = mti.GetSectionNcFile(&ncFile, p1, p2);
	ncFile.close();
	return pImage;
}

void ControlorImps::MoveRect(const std::string& name, osg::Matrix m )
{
	m_pNetCDFDataProvider->m_pRadarGeometry->MoveRect(name, m);

}

int ControlorImps::GetRectVertex( const std::string& strName, osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4 )
{
	osg::Geometry* pGeometry = m_pNetCDFDataProvider->m_pRadarGeometry->GetRectGeometry(strName);

	if(pGeometry != nullptr)
	{
		osg::Vec3Array* pVertexes = dynamic_cast<osg::Vec3Array*>(pGeometry->getVertexArray());
		if(pVertexes->size() == 4)
		{
			p1 = pVertexes->at(0);
			p2 = pVertexes->at(1);
			p3 = pVertexes->at(2);
			p4 = pVertexes->at(3);
		}
	}
	else
	{
		return 0;
	}

	return 1;
}

int ControlorImps::ExportPartNcFile(const QString& strName, osg::Vec3 p1, osg::Vec3 p2, int iMode, int upIndex, int downIndex, const QString& strExortFile)
{
	NcFile ncFile(strName.toStdString().c_str());
	MakeTextureImps mti(m_pNetCDFDataProvider->m_pParentLayer->CoordTrans());
	int iR = mti.ExportPartNcFile(&ncFile, p1, p2, m_Layer, upIndex, downIndex, strExortFile);
	ncFile.close();
	return iR;
}

int ControlorImps::ExportSctionFile(const QString& strName, osg::Vec3 p1, osg::Vec3 p2, int iMode, const QString& strExortFile)
{
	NcFile ncFile(strName.toStdString().c_str());
	MakeTextureImps mti(m_pNetCDFDataProvider->m_pParentLayer->CoordTrans());
	int iR = mti.ExportSectionFile(&ncFile, p1, p2, strExortFile);
	ncFile.close();

	return iR;
}

int ControlorImps::GetVerticalData(const std::string& strName, osg::Vec3Array& vec3Array, osg::Vec3 p2, double* pData, size_t* pSize)
{
	NcFile ncFile(strName.c_str());
	MakeTextureImps mti(m_pNetCDFDataProvider->m_pParentLayer->CoordTrans());
	int iR = mti.GetVerticalData(&ncFile, vec3Array, p2, pData, pSize);
	ncFile.close();

	return iR;
}

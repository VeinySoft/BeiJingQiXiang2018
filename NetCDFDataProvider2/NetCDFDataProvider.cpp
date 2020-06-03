#include "SetCubesImps.h"
#include "NetCDFDataProvider.h"
#include "MakeTextureImps.h"
#include "RadarGeometry.h"
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QLibrary>
#include <QtCore/QDateTime>
#include <QtCore/QtGlobal>
#include "MyImageSequence.h"
#include "DataObtainInterface.h"
#include "ControlorImps.h"
#include "../NetCDF/cxx/netcdfcpp.h"

_TrecAlgorithm	g_pTrecAlgorithm = 0;

goto_gis::DataProvider* gNCProvider = 0;
extern "C"
{
	__declspec(dllexport) goto_gis::DataProvider* DataProviderPluginMain()
	{
		if(gNCProvider == 0)
		{
			gNCProvider = new NetCDFDataProvider;
			return gNCProvider;
		}
		else
			return gNCProvider;
	}
}

extern "C"
{
	bool __declspec(dllexport) MineData(std::string& stFileName)
	{
		QString strFileName;
		strFileName = QString::fromStdString(stFileName);
		QFileInfo fileInfo;
		fileInfo.setFile(strFileName);

		QString strSuffix("nc");
		QString strSuffix1 = fileInfo.suffix();
		if(strSuffix1 == strSuffix)
			return true;
		else
			return false;
	}
}
//#include <Windows.h>
NetCDFDataProvider::NetCDFDataProvider()
{
	SetCubesImps* pci = new SetCubesImps(this);
	ControlorImps* pContrlor = new ControlorImps(this);

	m_pSetCubesInterface = pci;
	m_pControlorInterface = pContrlor;
	this->InsertExtendInterface(m_pSetCubesInterface);
	this->InsertExtendInterface(pContrlor);

	//m_pMakeTexture = new MakeTextureImps;

	QLibrary lib(".\\TrecAlgorithm.dll");

	if (lib.load())
	{
		g_pTrecAlgorithm = (_TrecAlgorithm)lib.resolve("TrecAlgorithm");
	}
	//FreeLibrary(hDll);
}

NetCDFDataProvider::~NetCDFDataProvider()
{
	gNCProvider = nullptr;
}

#if 0
bool NetCDFDataProvider::LoadFile(const std::string& _filePath)
{
	QString filePath = QString::fromStdString(_filePath);

	if(filePath.isEmpty())
	{ 
		return false;
	}

	std::string stdPath = filePath.toStdString();
	const char * path = stdPath.data();

	NcFile *pNcFile = new NcFile(path);
	if(!pNcFile || pNcFile->is_valid() == FALSE)
	{
		//qDebug() << " NOT valid";
		return false;
	}

	RadarGeometry* pRG = new RadarGeometry;
	m_pNcFile = pNcFile;
	//获得维度
	const int iDims = pNcFile->num_dims();
	for(int i = 0; iDims > i; i++)
	{
		NcDim* pNcDim = pNcFile->get_dim(i);
		std::string strDimName = pNcDim->name();
		if(strDimName == "longtitude")
		{
			m_x = pNcDim->size();
		}
		else if(strDimName == "latitude")
		{
			m_y = pNcDim->size();
		}
		else if(strDimName == "layer")
		{
			m_z = pNcDim->size();
		}
		else
		{
			return false;
		}
	}
	//获得属性
	const int iAtts = pNcFile->num_atts();

	float fFirstLon = 0;
	float fFirstLat = 0;
	float fLastLon = 0;
	float fLastLat = 0;

	for(int i = 0; iAtts > i; i++)
	{
		NcAtt* pNcAtt = pNcFile->get_att(i);
		std::string strAttName = pNcAtt->name();
		if(strAttName.find("FirstLon") != std::string::npos)
		{
			fFirstLon = pNcAtt->as_float(0);
		}
		else if(strAttName.find("FirstLat") != std::string::npos)
		{
			fFirstLat = pNcAtt->as_float(0);
		}
		else if(strAttName.find("LastLon") != std::string::npos)
		{
			fLastLon = pNcAtt->as_float(0);
		}
		else if(strAttName.find("LastLat") != std::string::npos)
		{
			fLastLat = pNcAtt->as_float(0);
		}
	}

	const int iVars = pNcFile->num_vars();
	for(int i = 0; i < iVars; ++i)
	{
		NcVar* pVar =  pNcFile->get_var(i);
		if(!pVar)
		{
			return false;
		}
		QString name = pVar->name();
		
		if(name == QString::fromLocal8Bit("reflectivity"))
		{
			//获得维度
			//const int iDims = pVar->num_dims();

			for(int ii = 0; m_z > ii; ii++)
			{
				osg::ref_ptr<osg::Vec3Array> pVec3Array = new osg::Vec3Array;
				pVec3Array->push_back(osg::Vec3(fFirstLon, fFirstLat, 0));
				pVec3Array->push_back(osg::Vec3(fLastLon, fFirstLat, 0));
				pVec3Array->push_back(osg::Vec3(fLastLon, fLastLat, 0));
				pVec3Array->push_back(osg::Vec3(fFirstLon, fLastLat, 0));
				pRG->AddArray(pVec3Array);

				osg::ref_ptr<osg::Image> osgImage = new osg::Image;
				osg::ref_ptr<osg::Texture2D> pTexture = new osg::Texture2D;
				osgImage->allocateImage(m_x, m_y, 1, GL_RGBA, GL_FLOAT);
				osgImage->setOrigin(osg::Image::TOP_LEFT);

				unsigned int uiSize = osgImage->getImageSizeInBytes();
				
				/*QImage newPixmap(m_x, m_y, QImage::Format_ARGB32);
				newPixmap.fill(QColor(0, 0, 0, 0));*/

				//NcDim* pNcDim = pNcFile->get_dim(ii);
				//std::string strDimName = pNcDim->name();

				long lCur[3] = {0};
				lCur[0] = ii;

				for(int iii = 0; iii < m_y; iii++)
				{
					lCur[1] = iii;
					lCur[2] = 0;
					pVar->set_cur(lCur);

					long laCount[3] = {0};
					laCount[0] = 1;
					laCount[1] = 1;
					laCount[2] = m_x;
					short* pData = new short[m_x];
					pVar->get(pData, laCount);

					for(int j = 0; j < m_x; j++)
					{
						short shN = *(pData + j);
						short realValue = -33 + 0.5 * shN;
						if(shN == 0 || shN == 1 || shN == 2 || shN == 3)
							continue;
						
						if(realValue < gValue[0])
							continue;

						for(int k = 1; k < 15; k++)
						{
							if(realValue < gValue[k] && realValue > gValue[k-1])
							{
								QColor cc = gColorTable[k];
								int iR = cc.red();
								int iG = cc.green();
								int iB = cc.blue();
								int iA = cc.alpha();

								//QColor ccc(iR, iG, iB);
								float* pRGBA = (float*)(osgImage->data(j, iii));
								*pRGBA		=iR/255.0;
								*(pRGBA + 1)=iG/255.0;
								*(pRGBA + 2)=iB/255.0;
								*(pRGBA + 3)=iA/255.0;

								/*osg::Vec4 curColor = osgImage->getColor(j, iii);
								iR = 255.0*curColor.r();
								iG = 255.0*curColor.g();
								iB = 255.0*curColor.b();
								iA = 255.0*curColor.a();*/
								//printf("aaaa");
								//newPixmap.setPixel(j, iii, cc.rgb()/*cc.rgba()*/);
							}
						}
					}
				}
				//QString strFileName = QString("%1").arg(ii);
				//strFileName = "D:/" + strFileName;
				//strFileName += ".bmp";
				//osgDB::writeImageFile(*osgImage, strFileName.toStdString().c_str());
				//newPixmap.save("D:/" + strFileName + ".png");

				pTexture->setImage(osgImage);
				
				pRG->AddImage(pTexture);
			}
		}
	}

	this->ParentLayer()->GetMgoGeometry()->insert(goto_gis::Layer::MgoGeometryPair(0, pRG));
	//获得数据
	
	return true;
}
#else
bool NetCDFDataProvider::LoadOneFile(const std::string& filePath)
{
	return false;
}

bool NetCDFDataProvider::LoadFile(const std::string& _filePath)
{
#if 1
	//m_FileNameToNcFile.clear();
	m_FileNames.clear();
	m_pRadarGeometry = new RadarGeometry;

	ControlorImps* pContrlor = dynamic_cast<ControlorImps*>(m_pControlorInterface);
	//pContrlor->SetRadarGeometry(m_pRadarGeometry);

	/*osg::ref_ptr<osg::Image> pTexture;
	osg::ref_ptr<osg::Vec3Array> pV3Array

	for(int i = 0; i > 6; i++)
	{

		pRG->AddImage();
	}*/
	//pRG->AddImage(pTexture);
	//ControlorImps* pContrlor = dynamic_cast<ControlorImps*>(m_pControlorInterface);
	
	m_pRadarGeometry->SetParentLayer(ParentLayer());
	this->ParentLayer()->GetMgoGeometry()->insert(goto_gis::Layer::MgoGeometryPair(0, m_pRadarGeometry));
#else
	P_TREC_NC_MAP_OUT OutMap = new TREC_NC_MAP_OUT;
	//1.参数设置;
	P_TREC_NC_IN_VECTOR pFileVector=new TREC_NC_IN_VECTOR;
	P_CUBE_DATA pCube=new CUBE_DATE;
	/*P_TREC_NC_MAP_OUT pOutMap=new TREC_NC_MAP_OUT;*/


	float fRefs[4]={20,30,40,50};
	pCube->fTargetFstLon=112;
	pCube->fTargetLstLon=121;
	pCube->fTargetFstLat=36;
	pCube->fTargetLstLat=44;
	pCube->iHeight=4000;
	pCube->iHeightUp=5000;
	pCube->iHeightDown=3000;
	pCube->refs.assign(fRefs,fRefs+4);

	//2.查找打开;
	//std::string strDataPath="E:\\Trec算法\\测试数据\\";
	std::vector<std::string> vct_strDataName;
	vct_strDataName.push_back("E:\\ProjectData\\BejingRadar\\1506292136.nc");
	vct_strDataName.push_back("E:\\ProjectData\\BejingRadar\\1506292142.nc");

	for (int i=0;i<vct_strDataName.size();i++)
	{
		NcFile *pNcFile = new NcFile(vct_strDataName[i].c_str());

		if(!pNcFile || pNcFile->is_valid() == FALSE)
		{
			pNcFile->close();
			//delete pNcFile;
			continue;
		}

		pFileVector->push_back(pNcFile);
	}

	//3.算法;
	//TrecAlgorithm(pFileVector,pCube,&OutMap);

	typedef bool (* _TrecAlgorithm) (const P_TREC_NC_IN_VECTOR pFileVector, const P_CUBE_DATA pCube, P_TREC_NC_MAP_OUT pOutMap);
	_TrecAlgorithm	TrecAlgorithm=NULL;
	
	QLibrary lib(".\\TrecAlgorithm.dll");

	if (lib.load())
	{
		TrecAlgorithm = (_TrecAlgorithm)lib.resolve("TrecAlgorithm");

		if ( TrecAlgorithm==NULL )
		{
			//AfxMessageBox( _T("加载DLL函数失败!") );
			return 0;
		}
	}
	else
	{
		//AfxMessageBox( _T("打开DLL文件失败!") );
		return 0;
	}

	TrecAlgorithm(pFileVector,pCube,OutMap);
	qDebug("-------------satrt----------");
	delete OutMap;
	qDebug("-------------end----------");
	//FreeLibrary(hDll);

#endif
	return true;
}
#endif


void NetCDFDataProvider::AreaAdd( bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p )
{
	
}

bool NetCDFDataProvider::MineData( const std::string& strIns /*= "" */ )
{
	return true;
}
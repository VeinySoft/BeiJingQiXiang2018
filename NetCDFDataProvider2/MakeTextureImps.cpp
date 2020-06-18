#include "MakeTextureImps.h"
#include "Proj4Transform.h"
#include <osgDB/WriteFile>
//QColor gColorTable[] = 
//{
//	QColor(0, 160, 192),    QColor(166, 202, 240), QColor(128, 128, 192), QColor(32, 32, 192),
//	QColor(192, 220, 192),  QColor(0, 224, 0),     QColor(32, 160, 0),    QColor(224, 244, 128),
//	QColor(192, 192, 0),    QColor(128, 128, 0),   QColor(224, 160, 192), QColor(224, 96, 64),
//	QColor(220, 0, 64),     QColor(224, 128, 192), QColor(160, 32, 192),  QColor(255, 255, 255)
//};
#define compute_distance(x1,x2,y1,y2) sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))
#define CSV_LINE(stream, v1, v2, v3, v4) stream<<v1<<QString::fromLocal8Bit(",")<<v2<<QString::fromLocal8Bit(",")<<v3<<QString::fromLocal8Bit(",")<<v4<<endl
#define MAKE_CSV_LINE(v1, v2, v3, v4) v1 + QString::fromLocal8Bit(",") + v2 + QString::fromLocal8Bit(",") + v3 + QString::fromLocal8Bit(",") + v4 + "\n"

#define compute_distance(x1,x2,y1,y2) sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))

QColor gColorTable[] = 
{
	QColor(0, 172, 164), QColor(192, 192, 254), QColor(122, 144, 234),
	QColor(30, 38, 208),  QColor(166, 252, 168),     QColor(0, 234, 0),    QColor(16, 146, 26),
	QColor(255, 244, 100),  QColor(200, 200,   2), QColor(140, 140, 0), QColor(254, 172, 172),
	QColor(254, 100, 92 ),  QColor(235,   2,  48), QColor(212, 142, 254),  QColor(170, 36, 250), QColor(255, 255, 255)
};

short gValue[] = 
{
	-5,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70
};

MakeTextureImps::MakeTextureImps(goto_gis::CoordinateTransform* pCT)
{
	m_pCoordinateTransform = pCT;
}

MakeTextureImps::~MakeTextureImps(void)
{
}

#include <QDebug>
bool MakeTextureImps::loadNcFileToTexture( NcFile *pNcFile 
	, osg::ref_ptr<osg::Image>& pTextrue2D 
	, osg::ref_ptr<osg::Vec3Array>& pV3Array)
{
	long m_x, m_y, m_z;
	/*QString lfilePath = QString::fromStdString(filePath);

	if(lfilePath.isEmpty())
	{
		return false;
	}

	std::string stdPath = lfilePath.toStdString();
	const char * path = stdPath.data();

	NcFile *pNcFile = new NcFile(path);*/
	/*if(!pNcFile || pNcFile->is_valid() == FALSE)
	{
		return false;
	}*/

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
			//pNcFile->close();
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
			//pNcFile->close();
			return false;
		}
		QString name = pVar->name();
		
		if(name == QString::fromLocal8Bit("reflectivity"))
		{
			//获得维度
			//const int iDims = pVar->num_dims();
			osg::ref_ptr<osg::Vec3Array> pVec3Array = new osg::Vec3Array;
			pVec3Array->push_back(osg::Vec3(fFirstLon, fFirstLat, 0));
			pVec3Array->push_back(osg::Vec3(fLastLon, fFirstLat, 0));
			pVec3Array->push_back(osg::Vec3(fLastLon, fLastLat, 0));
			pVec3Array->push_back(osg::Vec3(fFirstLon, fLastLat, 0));
			pV3Array = pVec3Array;

			osg::ref_ptr<osg::Image> osgImage = new osg::Image;
			osgImage->allocateImage(m_x, m_y, 1, GL_RGBA, GL_FLOAT);
			//memset(osgImage->data(), 255, osgImage->getImageSizeInBytes());

			osgImage->setOrigin(osg::Image::TOP_LEFT);

			unsigned int uiSize = osgImage->getImageSizeInBytes();

			ncbyte* pData = new ncbyte[m_x];
			ncbyte* MaxValues = new ncbyte[m_x * m_y];
			memset(MaxValues, -100, sizeof(ncbyte) * m_x * m_y);

			for(int ii = 0; m_z > ii; ii++)
			{
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

					memset(pData, 0, sizeof(ncbyte) * m_x);
					pVar->get(pData, laCount);

					for(int j = 0; j < m_x; j++)
					{
						ncbyte shN = *(pData + j);

						if(shN == 0 || shN == 1 || shN == 2 || shN == 3)
							continue;

						ncbyte realValue = 0xee;
						
						if(shN < 0)
						{
							realValue = -33 + 0.5 * (256 + shN);
						}
						else
						{
							realValue = -33 + 0.5 * shN;
						}
						if(realValue < gValue[0])
							continue;

						ncbyte isMax = *(MaxValues + m_x * iii + j);
						if(isMax < realValue || isMax == 0xee)
						{
							*(MaxValues + m_x * iii + j) = realValue;
						}
					}
				}
			}

			for(int c = 0; c < m_y; c++)
			{
				for(int r = 0; r < m_x; r++)
				{
					ncbyte isMax = *(MaxValues + m_x * c + r);

					if(isMax == -100) 
						continue;

					//if(isMax > 65)
					//{
					//	QColor cc = gColorTable[14];
					//	int iR = cc.red();
					//	int iG = cc.green();
					//	int iB = cc.blue();
					//	int iA = cc.alpha();

					//	//QColor ccc(iR, iG, iB);
					//	/*float* pRGBA = (float*)(osgImage->data(r, c));
					//	*pRGBA		=iR/255.0;
					//	*(pRGBA + 1)=iG/255.0;
					//	*(pRGBA + 2)=iB/255.0;
					//	*(pRGBA + 3)=0.5f;*/

					//	unsigned int* pRGBA = (unsigned int*)(osgImage->data(r, c));
					//	*pRGBA = 0x0000FF00;
					//	//*pRGBA = cc.rgba();
					//}

					for(int k = 1; k < 16; k++)
					{
						int iSV = gValue[k];
						int iEV = gValue[k-1];

						if((isMax < iSV && isMax >= iEV))
						{
							QColor cc = gColorTable[k-1];
							int iR = cc.red();
							int iG = cc.green();
							int iB = cc.blue();
							int iA = cc.alpha();

							//QColor ccc(iR, iG, iB);
							float* pRGBA = (float*)(osgImage->data(r, c));
							*pRGBA		=iR/255.0;
							*(pRGBA + 1)=iG/255.0;
							*(pRGBA + 2)=iB/255.0;
							*(pRGBA + 3)=1.0f;

							//unsigned int* pRGBA = (unsigned int*)(osgImage->data(r, c));
							////*pRGBA = 0x0000FF00;
							//unsigned char* byteColor = (unsigned char*)pRGBA;
							//*byteColor		=(unsigned char)255;
							//*(byteColor + 1)=(unsigned char)0;
							//*(byteColor + 2)=(unsigned char)0;
							//*(byteColor + 3)=0;
						}
					}
				}
			}
			//osgDB::writeImageFile(*osgImage, "D:\\aaaa.bmp");
			pTextrue2D = osgImage;
			delete[] MaxValues;
			delete[] pData;
		}
	}

	//NcBool ncR = pNcFile->close();
	return true;
}

QImage* MakeTextureImps::loadNcFileToTexture( NcFile *pNcFile
	, osg::ref_ptr<osg::Vec3Array>& pV3Array)
{
	QImage* pImage = 0;
	long m_x, m_y, m_z;
	/*QString lfilePath = QString::fromStdString(filePath);

	if(lfilePath.isEmpty())
	{
		return false;
	}

	std::string stdPath = lfilePath.toStdString();
	const char * path = stdPath.data();

	NcFile *pNcFile = new NcFile(path);*/
	/*if(!pNcFile || pNcFile->is_valid() == FALSE)
	{
		return false;
	}*/

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
			//pNcFile->close();
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
			//pNcFile->close();
			return false;
		}
		QString name = pVar->name();
		
		if(name == QString::fromLocal8Bit("reflectivity"))
		{
			//获得维度
			//const int iDims = pVar->num_dims();
			osg::ref_ptr<osg::Vec3Array> pVec3Array = new osg::Vec3Array;
			pVec3Array->push_back(osg::Vec3(fFirstLon, fFirstLat, 0));
			pVec3Array->push_back(osg::Vec3(fLastLon, fFirstLat, 0));
			pVec3Array->push_back(osg::Vec3(fLastLon, fLastLat, 0));
			pVec3Array->push_back(osg::Vec3(fFirstLon, fLastLat, 0));
			pV3Array = pVec3Array;

			pImage = new QImage(m_x, m_y, QImage::Format_RGB888);
			pImage->fill(QColor(255, 255, 255));
			//osg::ref_ptr<osg::Image> osgImage = new osg::Image;
			//osgImage->allocateImage(m_x, m_y, 1, GL_RGBA, GL_FLOAT);
			//memset(osgImage->data(), 255, osgImage->getImageSizeInBytes());

			//osgImage->setOrigin(osg::Image::TOP_LEFT);

			//unsigned int uiSize = osgImage->getImageSizeInBytes();

			ncbyte* pData = new ncbyte[m_x];
			ncbyte* MaxValues = new ncbyte[m_x * m_y];
			memset(MaxValues, -100, sizeof(ncbyte) * m_x * m_y);

			for(int ii = 0; m_z > ii; ii++)
			{
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

					memset(pData, 0, sizeof(ncbyte) * m_x);
					pVar->get(pData, laCount);

					for(int j = 0; j < m_x; j++)
					{
						ncbyte shN = *(pData + j);

						if(shN == 0 || shN == 1 || shN == 2 || shN == 3)
							continue;

						ncbyte realValue = 0xee;
						
						if(shN < 0)
						{
							realValue = -33 + 0.5 * (256 + shN);
						}
						else
						{
							realValue = -33 + 0.5 * shN;
						}
						if(realValue < gValue[0])
							continue;

						ncbyte isMax = *(MaxValues + m_x * iii + j);
						if(isMax < realValue || isMax == 0xee)
						{
							*(MaxValues + m_x * iii + j) = realValue;
						}
					}
				}
			}

			for(int c = 0; c < m_y; c++)
			{
				for(int r = 0; r < m_x; r++)
				{
					ncbyte isMax = *(MaxValues + m_x * c + r);

					if(isMax == -100) 
						continue;

					//if(isMax > 65)
					//{
					//	QColor cc = gColorTable[14];
					//	int iR = cc.red();
					//	int iG = cc.green();
					//	int iB = cc.blue();
					//	int iA = cc.alpha();

					//	//QColor ccc(iR, iG, iB);
					//	/*float* pRGBA = (float*)(osgImage->data(r, c));
					//	*pRGBA		=iR/255.0;
					//	*(pRGBA + 1)=iG/255.0;
					//	*(pRGBA + 2)=iB/255.0;
					//	*(pRGBA + 3)=0.5f;*/

					//	unsigned int* pRGBA = (unsigned int*)(osgImage->data(r, c));
					//	*pRGBA = 0x0000FF00;
					//	//*pRGBA = cc.rgba();
					//}

					for(int k = 1; k < 16; k++)
					{
						int iSV = gValue[k];
						int iEV = gValue[k-1];

						if((isMax < iSV && isMax >= iEV))
						{
							QColor cc = gColorTable[k-1];
							int iR = cc.red();
							int iG = cc.green();
							int iB = cc.blue();
							int iA = cc.alpha();

							pImage->setPixel(r, c, cc.toRgb().rgb());
						}
					}
				}
			}
			//osgDB::writeImageFile(*osgImage, "D:\\aaaa.bmp");
			
			delete[] MaxValues;
			delete[] pData;
		}
	}

	//NcBool ncR = pNcFile->close();
	return pImage;
}

bool MakeTextureImps::loadNcFileToTexture( NcFile *pNcFile 
	, osg::ref_ptr<osg::Image>& pTextrue2D 
	, osg::ref_ptr<osg::Vec3Array>& pV3Array, int iMode )
{
	long m_x, m_y, m_z;

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
			//pNcFile->close();
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
			//pNcFile->close();
			return false;
		}
		QString name = pVar->name();

		if(name == QString::fromLocal8Bit("reflectivity"))
		{
			//获得维度
			//const int iDims = pVar->num_dims();
			osg::ref_ptr<osg::Vec3Array> pVec3Array = new osg::Vec3Array;
			pVec3Array->push_back(osg::Vec3(fFirstLon, fFirstLat, 0));
			pVec3Array->push_back(osg::Vec3(fLastLon, fFirstLat, 0));
			pVec3Array->push_back(osg::Vec3(fLastLon, fLastLat, 0));
			pVec3Array->push_back(osg::Vec3(fFirstLon, fLastLat, 0));
			pV3Array = pVec3Array;

			osg::ref_ptr<osg::Image> osgImage = new osg::Image;
			osgImage->allocateImage(m_x, m_y, 1, GL_RGBA, GL_FLOAT);
			osgImage->setOrigin(osg::Image::TOP_LEFT);

			unsigned int uiSize = osgImage->getImageSizeInBytes();

			ncbyte* pData = new ncbyte[m_x];
			ncbyte* MaxValues = new ncbyte[m_x * m_y];
			memset(MaxValues, invalid_value, sizeof(ncbyte) * m_x * m_y);

			for(int ii = 0; m_z > ii; ii++)
			{
				long lCur[3] = {0};

				if(iMode > -1 && iMode < m_z)
				{
					if(ii != iMode)
						continue;
				}

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
					
					memset(pData, 0, sizeof(ncbyte) * m_x);
					pVar->get(pData, laCount);

					for(int j = 0; j < m_x; j++)
					{
						ncbyte shN = *(pData + j);

						if(shN == 0 || shN == 1 || shN == 2 || shN == 3)
							continue;

						ncbyte realValue = 0xee;
						
						if(shN < 0)
						{
							realValue = -33 + 0.5 * (256 + shN);
						}
						else
						{
							realValue = -33 + 0.5 * shN;
						}
						if(realValue < gValue[0])
							continue;

						for(int k = 1; k < 16; k++)
						{
							int iSV = gValue[k];
							int iEV = gValue[k-1];
							if(/*isMax <= gValue[0] || */(realValue < iSV && realValue >= iEV))
							{
								QColor cc = gColorTable[k-1];
								int iR = cc.red();
								int iG = cc.green();
								int iB = cc.blue();
								int iA = cc.alpha();

								//QColor ccc(iR, iG, iB);
								float* pRGBA = (float*)(osgImage->data(j, iii));
								*pRGBA		=iR/255.0;
								*(pRGBA + 1)=iG/255.0;
								*(pRGBA + 2)=iB/255.0;
								*(pRGBA + 3)=1.0f;
							}
						}
					}
				}
			}

			pTextrue2D = osgImage;

			delete[] pData;
		}
	}

	//NcBool ncR = pNcFile->close();
	return true;
}

QImage* MakeTextureImps::GetPartNcFile( NcFile *pNcFile, osg::Vec3 p1, osg::Vec3 p2, int iMode, int upIndex, int downIndex, float* fMaxHeight, float* fVolume, float* fMaxDBZ )
{
	float fDbzMax = 0;
	float fVolumeMax = 0;
	if(fMaxHeight != nullptr)
	{
		fDbzMax = *fMaxHeight;
		*fMaxHeight = 0;
	}
	if(fVolume != nullptr)
	{
		fVolumeMax = *fVolume;
		*fVolume = 0;
	}

	osg::Vec3 v3LocalP1, v3LocalP2;

	m_pCoordinateTransform->Transform(p1, v3LocalP1);
	m_pCoordinateTransform->Transform(p2, v3LocalP2);
	p1.x() < p2.x() ? (v3LocalP1.set(v3LocalP1.x()/1000, v3LocalP1.y()/1000, v3LocalP1.z()/1000), v3LocalP2.set(v3LocalP2.x()/1000, v3LocalP2.y()/1000, v3LocalP2.z()/1000)) 
		: (v3LocalP1.set(v3LocalP2.x()/1000, v3LocalP2.y()/1000, v3LocalP2.z()/1000), v3LocalP2.set(v3LocalP1.x()/1000, v3LocalP1.y()/1000, v3LocalP1.z()/1000));

	long m_x, m_y, m_z;
	int iLeftX(400), iLeftY(400);
	//获得维度
	const int iDims = pNcFile->num_dims();
	for(int i = 0; iDims > i; i++)
	{
		NcDim* pNcDim = pNcFile->get_dim(i);
		std::string strDimName = pNcDim->name();
		if(strDimName == "time")
		{

		}
		else if(strDimName == "height")
		{
			m_z = pNcDim->size();
		}
		else if(strDimName == "Y")
		{
			m_y = pNcDim->size();
		}
		else if(strDimName == "X")
		{
			m_x = pNcDim->size();
		}
		else
		{
			//pNcFile->close();
			return false;
		}
	}
	//获得属性
	const int iAtts = pNcFile->num_atts();

	float fOriginLon = 0;
	float fOriginLat = 0;

	for(int i = 0; iAtts > i; i++)
	{
		NcAtt* pNcAtt = pNcFile->get_att(i);
		std::string strAttName = pNcAtt->name();
		if(strAttName.find("OriginX") != std::string::npos)
		{
			fOriginLon = pNcAtt->as_float(0);
		}
		else if(strAttName.find("OriginY") != std::string::npos)
		{
			fOriginLat = pNcAtt->as_float(0);
		}
	}

	osg::Vec3 OriginLonLat(fOriginLon, fOriginLat, 0);

	osg::Vec3 OriginXY;

	m_pCoordinateTransform->Transform(OriginLonLat, OriginXY);

	float* pfHeight = nullptr;
	float* pfY = nullptr;
	float* pfX = nullptr;

	float fheightFV = 0;
	float fYFV = 0;
	float fXFV = 0;
	float fValueFV = 0;
	float fMisingValue = 0;

	osg::Matrix moveOrigin;
	moveOrigin.makeTranslate(osg::Vec3(-OriginXY.x()/1000, 0, -OriginXY.z()/1000));

	osg::Vec3 v3ResultP1 = v3LocalP1 * moveOrigin;
	osg::Vec3 v3ResultP2 = v3LocalP2 * moveOrigin;

	int p1x = iLeftX + v3ResultP1.x();
	int p1y = iLeftY + v3ResultP1.z();

	int p2x = iLeftX + v3ResultP2.x();
	int p2y = iLeftY + v3ResultP2.z();

	int m_xLen(abs(p1x - p2x) + 1), m_yLen(abs(p1y - p2y) + 1);

	QImage* pImage = new QImage(m_xLen, m_yLen, QImage::Format_RGB32);
	pImage->fill(QColor(255, 255, 255));

	const int iVars = pNcFile->num_vars();
	for(int i = 0; i < iVars; ++i)
	{
		NcVar* pVar =  pNcFile->get_var(i);
		if(!pVar)
		{
			//pNcFile->close();
			return false;
		}
		QString name = pVar->name();
		if(name == QString::fromLocal8Bit("time"))
		{

		}
		else if(name == QString::fromLocal8Bit("height"))
		{
			long lCur = 0;
			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fheightFV = pNcVals[0].as_float(0);

			long lVarNum = pVar->num_vals();
			pfHeight = new float[lVarNum];
			memset(pfHeight, 0, sizeof(pfHeight) * lVarNum);

			pVar->set_cur(lCur);
			pVar->get(pfHeight, lVarNum);


		}
		else if(name == QString::fromLocal8Bit("Y"))
		{
			long lCur = 0;

			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fYFV = pNcVals[0].as_float(0);

			long lVarNum = pVar->num_vals();
			pfY = new float[lVarNum];
			memset(pfY, 0, sizeof(pfY) * lVarNum);

			pVar->set_cur(lCur);
			pVar->get(pfY, lVarNum);

			for(int iyn = 0; iyn < m_y; iyn++)
			{
				pfY[iyn] = pfY[iyn] * 1000;
			}
		}
		else if(name == QString::fromLocal8Bit("X"))
		{
			long lCur = 0;

			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fXFV = pNcVals[0].as_float(0);

			long lVarNum = pVar->num_vals();
			pfX = new float[lVarNum];
			memset(pfX, 0, sizeof(pfX) * lVarNum);

			pVar->set_cur(lCur);
			pVar->get(pfX, lVarNum);

			for(int ixn = 0; ixn < m_x; ixn++)
			{
				pfX[ixn] = pfX[ixn] * 1000;
			}
		}
		else if(name == QString::fromLocal8Bit("DBZ"))
		{
			//获得维度
			//const int iDims = pVar->num_dims();

			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fValueFV = pNcVals[0].as_float(0);

			NcAtt* pAtt2 = pVar->get_att("missing_value");
			NcValues* pNcVals2 = pAtt2->values();
			fMisingValue = pNcVals2[0].as_float(0);

			osg::ref_ptr<osg::Vec3Array> pVec3Array = new osg::Vec3Array;
			osg::ref_ptr<osg::Vec4Array> pColorArray = new osg::Vec4Array;

			float* pData = new float[m_xLen];
			float* MaxValues = new float[m_xLen * m_yLen];
			int maxHeight = -1;
			float maxDBZ = -5;
			//memset(MaxValues, 0xff, sizeof(float) * m_x * m_y);
			for(int iFillIndex = 0; iFillIndex < m_xLen * m_yLen; iFillIndex++)
			{
				MaxValues[iFillIndex] = -100.0f;
			}
			float fInvalid = -100.0f;

			for(int ii = 0; m_z > ii; ii++)
			{
				long lCur[4] = {0};
				lCur[0] = 0;
				if(iMode > -1 && iMode < m_z)
				{
					if(ii != iMode)
						continue;
				}

				lCur[1] = ii;

				for(int iii = 0; iii < m_yLen; iii++)
				{
					lCur[2] = p1y - iii;
					lCur[3] = p1x;
					pVar->set_cur(lCur);

					long laCount[4] = {0};
					laCount[0] = 1;
					laCount[1] = 1;
					laCount[2] = 1;
					laCount[3] = m_xLen;

					memset(pData, 0, sizeof(float) * m_xLen);
					pVar->get(pData, laCount);

					for(int j = 0; j < m_xLen; j++)
					{
						float realValue = *(pData + j);

						if(abs(realValue - fMisingValue) < 0.00001)
							continue;

						float isMax = *(MaxValues + m_xLen * iii + j);
						if(isMax < realValue || (isMax - (fInvalid)) < 0.00001)
						{
							*(MaxValues + m_xLen * iii + j) = realValue;
						}

						if(fMaxHeight != nullptr)
						{
							if(/*ii >= downIndex && ii <= upIndex*/true)
							{
								if(realValue - fDbzMax > 0.0001)
								{
									if(ii > maxHeight)
										maxHeight = ii;
								}
							}
							
						}

						if(fVolume != nullptr)
						{
							if(ii >= downIndex && ii <= upIndex)
							{
								if(realValue - fVolumeMax >= 0.0001)
								{
									float fx = *(pfX + p1x + j)/1000;
									float fy = *(pfY + p1y + iii)/1000;

									float len = sqrt((0-fx)*(0-fx) + (0-fy) * (0-fy));
									*fVolume += 3.14 * (( 3.14/360 * len) * ( 3.14/360 * len)) * 0.3;
								}

								if(maxDBZ - realValue < 0.0001)
								{
									maxDBZ = realValue;
								}
							}
						}
					}
				}
			}

			for(int c = 0; c < m_yLen; c++)
			{
				for(int r = 0; r < m_xLen; r++)
				{
					ncbyte isMax = *(MaxValues + m_xLen * c + r);

					if(abs(isMax - fInvalid) < 0.00001) 
						continue;

					for(int k = 1; k < 16; k++)
					{
						int iSV = gValue[k];
						int iEV = gValue[k-1];

						if((isMax >= iEV && isMax < iSV))
						{
							QColor cc = gColorTable[k-1];
							int iR = cc.red();
							int iG = cc.green();
							int iB = cc.blue();
							int iA = cc.alpha();

							pImage->setPixel(r, c, cc.rgba());
						}
					}
				}
			}
			delete[] MaxValues;
			delete[] pData;
			delete[] pfX;
			delete[] pfY;
			delete[] pfHeight;

			if(fMaxHeight != nullptr && fMaxDBZ != nullptr )
			{
				*fMaxHeight = maxHeight;
				*fMaxDBZ = maxDBZ;
			}
			
		}
	}

	QImage* pNewImage = 0;
	float fScale = pImage->width() / (float)((pImage->height()));
	int newHeight = 300.0 / fScale;

	pNewImage = new QImage(300, newHeight, QImage::Format_RGB32);

	QPainter painter;
	painter.begin(pNewImage);
	painter.drawImage(QRect(0, 0, 300, newHeight), *pImage);
	painter.end();

	delete pImage;
	return pNewImage;
}

QImage*  MakeTextureImps::GetSectionNcFile( NcFile *pNcFile, osg::Vec3 p1, osg::Vec3 p2 )
{
	osg::Vec3 v3LocalP1, v3LocalP2;
	p1.x() < p2.x() ? (v3LocalP1.set(p1.x()/1000, p1.y()/1000, p1.z()/1000), v3LocalP2.set(p2.x()/1000, p2.y()/1000, p2.z()/1000)) 
		: (v3LocalP1.set(p2.x()/1000, p2.y()/1000, p2.z()/1000), v3LocalP2.set(p1.x()/1000, p1.y()/1000, p1.z()/1000));

	long m_x, m_y, m_z;
	int lonFistIndex(0), latFistIndex(0), lonLastIndex(0), latLastIndex(0);
	//获得维度
	const int iDims = pNcFile->num_dims();
	for(int i = 0; iDims > i; i++)
	{
		NcDim* pNcDim = pNcFile->get_dim(i);
		std::string strDimName = pNcDim->name();
		if(strDimName == "time")
		{

		}
		else if(strDimName == "height")
		{
			m_z = pNcDim->size();
		}
		else if(strDimName == "Y")
		{
			m_y = pNcDim->size();
		}
		else if(strDimName == "X")
		{
			m_x = pNcDim->size();
		}
		else
		{
			//pNcFile->close();
			return false;
		}
	}
	//获得属性
	const int iAtts = pNcFile->num_atts();

	float fOriginLon = 0;
	float fOriginLat = 0;

	for(int i = 0; iAtts > i; i++)
	{
		NcAtt* pNcAtt = pNcFile->get_att(i);
		std::string strAttName = pNcAtt->name();
		if(strAttName.find("OriginX") != std::string::npos)
		{
			fOriginLon = pNcAtt->as_float(0);
		}
		else if(strAttName.find("OriginY") != std::string::npos)
		{
			fOriginLat = pNcAtt->as_float(0);
		}
	}

	float* pfHeight = nullptr;
	float* pfY = nullptr;
	float* pfX = nullptr;

	float fheightFV = 0;
	float fYFV = 0;
	float fXFV = 0;
	float fValueFV = 0;
	float fMisingValue = 0;

	osg::Vec3 lonLat(fOriginLon, fOriginLat, 0), v3Projection;
	m_pCoordinateTransform->Transform(lonLat, v3Projection);
	osg::Matrix moveOrigin;
	moveOrigin.makeTranslate(osg::Vec3(-v3Projection.x()/1000, 0, -v3Projection.z()/1000));
	int m_xLen(abs(v3LocalP2.x() - v3LocalP1.x()) + 1)/*, m_yLen(latLastIndex - latFistIndex + 1)*/;
	
	int iLeftX, iLeftY;

	float fInvalid = -100.0f;
	float* tempBuffer = new float[m_xLen * m_z];
	float* fZBuffer = new float[m_xLen * m_z];
	memset(fZBuffer, 0, sizeof(float) * m_xLen * m_z);

	for(int iFillIndex = 0; iFillIndex < m_xLen * m_z; iFillIndex++)
	{
		tempBuffer[iFillIndex] = fInvalid;
	}

	const int iVars = pNcFile->num_vars();
	for(int i = 0; i < iVars; ++i)
	{
		NcVar* pVar =  pNcFile->get_var(i);
		if(!pVar)
		{
			//pNcFile->close();
			return false;
		}
		QString name = pVar->name();
		if(name == QString::fromLocal8Bit("time"))
		{

		}
		else if(name == QString::fromLocal8Bit("height"))
		{
			long lCur = 0;
			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fheightFV = pNcVals[0].as_float(0);

			long lVarNum = pVar->num_vals();
			pfHeight = new float[lVarNum];
			memset(pfHeight, 0, sizeof(pfHeight) * lVarNum);

			pVar->set_cur(lCur);
			pVar->get(pfHeight, lVarNum);
		}
		else if(name == QString::fromLocal8Bit("Y"))
		{
			long lCur = 0;

			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fYFV = pNcVals[0].as_float(0);

			long lVarNum = pVar->num_vals();
			pfY = new float[lVarNum];
			memset(pfY, 0, sizeof(pfY) * lVarNum);

			pVar->set_cur(lCur);
			pVar->get(pfY, lVarNum);

			iLeftY = pfY[0];

			delete[] pfY;
		}
		else if(name == QString::fromLocal8Bit("X"))
		{
			long lCur = 0;

			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fXFV = pNcVals[0].as_float(0);

			long lVarNum = pVar->num_vals();
			pfX = new float[lVarNum];
			memset(pfX, 0, sizeof(pfX) * lVarNum);

			pVar->set_cur(lCur);
			pVar->get(pfX, lVarNum);
			
			iLeftX = pfX[0];

			delete[] pfX;
		}
		else if(name == QString::fromLocal8Bit("DBZ"))
		{
			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fValueFV = pNcVals[0].as_float(0);

			NcAtt* pAtt2 = pVar->get_att("missing_value");
			NcValues* pNcVals2 = pAtt2->values();
			fMisingValue = pNcVals2[0].as_float(0);

			for(int ii = 0; m_z > ii; ii++)
			{
				long lCur[4] = {0};
				lCur[0] = 0;
				lCur[1] = ii;

				long laCount[4] = {0};
				laCount[0] = 1;
				laCount[1] = 1;
				laCount[2] = 1;
				laCount[3] = 1;

				for(size_t i = 0; i < m_xLen; i++)
				{
					//x坐标增量为1，通过直线方程求出y坐标的增量
					float x = v3LocalP1.x() + i;
					float y = ((x-v3LocalP1.x())/(v3LocalP2.x() - v3LocalP1.x()) + v3LocalP1.z()
						/(v3LocalP2.z() - v3LocalP1.z())) * (v3LocalP2.z() - v3LocalP1.z());
					//求出坐标距离原点(0,0)的距离，这个原点是回波图象的左下角的点
					osg::Vec3 v3Result = osg::Vec3(x, 0, y) * moveOrigin;
					int iYindex = -iLeftY + v3Result.z();

					int iXindex = -iLeftX + v3Result.x();

					if(iYindex < 0 || iYindex > m_y) continue;
					if(iXindex < 0 || iXindex > m_x) continue;

					lCur[2] = iYindex;
					lCur[3] = iXindex;
					pVar->set_cur(lCur);
					
					float pValue = fMisingValue;
					pVar->get(&pValue, laCount);
					
					
					if(pValue < gValue[0] || abs(pValue - fMisingValue) < 0.0001)
						continue;
					
					tempBuffer[ii * m_xLen + i] = pValue;
					fZBuffer[ii * m_xLen + i] = ii;
				}
			}
		}
	}
	//ncbyte (*temp_buffer)[20] = 0;
	QImage* pImage = new QImage(m_xLen, 190*2, QImage::Format_RGB32);
	pImage->fill(QColor(255, 255, 255));

	//int iOne(5), iTow(5), iThree(12);
	int iIntervalCount = 0;
	QMap<int, QString> markMap;
	int iImage_Y_Interval = 190*2 / m_z;

	for(int ii = 0; ii < m_z; ii++)
	{
		double markValue = pfHeight[0] + ii;
		markMap.insert(190*2 - (ii * iImage_Y_Interval), QString("%1").arg(markValue, 0, 'f', 1));
	}

	for(int i = 0; i < m_xLen; i++)
	{
		int iImageY = 0;
		for(int ii = 0; ii < m_z - 1; ii++)
		{
			float va1 = tempBuffer[ii * m_xLen + i];
			float va2 = tempBuffer[(ii + 1) * m_xLen + i];
			float fZ = fZBuffer[ii * m_xLen + i];

			if((va1 - fInvalid) < 0.000001 || (va2 - fInvalid) < 0.0000001)
				continue;

			if((va1 - fMisingValue) < 0.000001 || (va2 - fMisingValue) < 0.0000001)
				continue;

			for(int iii = 0; iii < iImage_Y_Interval; iii++)
			{
				float newVA = va1 * ((iImage_Y_Interval - iii)/(float)iImage_Y_Interval) 
					+ va2 * (iii/(float)iImage_Y_Interval);

				for(int k = 1; k < 16; k++)
				{
					float iSV = gValue[k];
					float iEV = gValue[k-1];
					if(newVA - iSV < 0 && (newVA - iEV > 0 || abs(newVA - iEV) < 0.001))
					{
						QColor cc = gColorTable[k-1];
						pImage->setPixel(i, 190 * 2 - iImageY, cc.rgba());
						
					}
				}

				iImageY++;
			}

		}
	}
	//////////////////////////////////////////////////////////////////////////
	float distance = 0.0f;

	osg::Vec3 p11(p1), p22(p2);
	
	//m_pCoordinateTransform->Transform(p1, p11);
	//m_pCoordinateTransform->Transform(p2, p22);
	distance = compute_distance(p11.x(), p22.x(), p11.z(), p22.z());
	
	QImage* pNewImage = new QImage(600 + 50 + 75, pImage->height() + 30, QImage::Format_RGB32);
	pNewImage->fill(QColor(255, 255, 255));

	QPainter pain;
	pain.begin(pNewImage);
	QMap<int, QString>::iterator be = markMap.begin();
	pain.setPen(QColor(0,0,0));
	QFont font;
	//font.setFamily();
	font.setPointSize(8);
	
	pain.setFont(font);

	//////////////////////////////////////////////////////////////////////////
	pain.drawImage(QRect(55, 15, 600, pImage->height()), *pImage);
	//////////////////////////////////////////////////////////////////////////
	//pain.drawLine(QPoint(55, 15), QPoint(40, pImage->height() + 5));
	pain.drawLine(QPoint(600 + 55, 15), QPoint(600 + 55, pImage->height() + 15));//最右竖线
	pain.drawLine(QPoint(55, 15), QPoint(600 + 55 - 1, 15));//最上横线
	while(be != markMap.end())
	{
		int iY = be.key();
		QString strMark = be.value();

		pain.drawLine(QPoint(55, iY + 15), QPoint(600 + 55 - 1, iY + 15));
		
		pain.drawText(15, iY + 15 + 4, strMark+"km");
		be++;
	}
	
	int iInterval = 600/10;
	float fDisInterval = distance/10;

	for(int i = 0; i < 10; i++)
	{
		pain.drawLine(QPoint(55 + i * iInterval, 15), QPoint(55 + i * iInterval, pImage->height() + 15));

		QString strText;
		if(i != 0)
		{
			strText = QString("%1km").arg(i * fDisInterval / 1000.0f, 0, 'f', 2);
			pain.drawText(55 + i * iInterval - 10, pImage->height() + 28, strText);
		}

		if(i == 9)
		{
			strText = QString("%1km").arg(distance / 1000.0f, 0, 'f', 2);
			pain.drawText(600 + 40, pImage->height() + 28, strText);
		}
	}

	pain.drawImage(600 + 60, 15, QImage("./icon/color_Table2.png"));
	pain.end();

	delete[] tempBuffer;
	delete pImage;
	return pNewImage;
}

bool MakeTextureImps::loadNcFileToGeometry( NcFile *pNcFile, osg::ref_ptr<osg::Vec3Array>& vs
	, osg::ref_ptr<osg::Vec4Array>& cs, goto_gis::CoordinateTransform* pCT, int iMode)
{
	long m_x, m_y, m_z;

	//获得维度
	const int iDims = pNcFile->num_dims();
	for(int i = 0; iDims > i; i++)
	{
		NcDim* pNcDim = pNcFile->get_dim(i);
		std::string strDimName = pNcDim->name();
		if(strDimName == "time")
		{
			
		}
		else if(strDimName == "height")
		{
			m_z = pNcDim->size();
		}
		else if(strDimName == "Y")
		{
			m_y = pNcDim->size();
		}
		else if(strDimName == "X")
		{
			m_x = pNcDim->size();
		}
		else
		{
			//pNcFile->close();
			return false;
		}
	}
	//获得属性
	const int iAtts = pNcFile->num_atts();

	float fOriginLon = 0;
	float fOriginLat = 0;

	for(int i = 0; iAtts > i; i++)
	{
		NcAtt* pNcAtt = pNcFile->get_att(i);
		std::string strAttName = pNcAtt->name();
		if(strAttName.find("OriginX") != std::string::npos)
		{
			fOriginLon = pNcAtt->as_float(0);
		}
		else if(strAttName.find("OriginY") != std::string::npos)
		{
			fOriginLat = pNcAtt->as_float(0);
		}
	}
	
	osg::Vec3 OriginLonLat(fOriginLon, fOriginLat, 0);
	
	osg::Vec3 OriginXY;

	pCT->Transform(OriginLonLat, OriginXY);
	
	float* pfHeight = nullptr;
	float* pfY = nullptr;
	float* pfX = nullptr;

	float fheightFV = 0;
	float fYFV = 0;
	float fXFV = 0;
	float fValueFV = 0;
	float fMisingValue = 0;

	const int iVars = pNcFile->num_vars();
	for(int i = 0; i < iVars; ++i)
	{
		NcVar* pVar =  pNcFile->get_var(i);
		if(!pVar)
		{
			//pNcFile->close();
			return false;
		}
		QString name = pVar->name();
		if(name == QString::fromLocal8Bit("time"))
		{

		}
		else if(name == QString::fromLocal8Bit("height"))
		{
			long lCur = 0;
			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fheightFV = pNcVals[0].as_float(0);

			long lVarNum = pVar->num_vals();
			pfHeight = new float[lVarNum];
			memset(pfHeight, 0, sizeof(pfHeight) * lVarNum);

			pVar->set_cur(lCur);
			pVar->get(pfHeight, lVarNum);


		}
		else if(name == QString::fromLocal8Bit("Y"))
		{
			long lCur = 0;

			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fYFV = pNcVals[0].as_float(0);

			long lVarNum = pVar->num_vals();
			pfY = new float[lVarNum];
			memset(pfY, 0, sizeof(float) * lVarNum);

			pVar->set_cur(lCur);
			pVar->get(pfY, lVarNum);

			for(int iyn = 0; iyn < m_y; iyn++)
			{
				pfY[iyn] = pfY[iyn] * 1000;
			}
		}
		else if(name == QString::fromLocal8Bit("X"))
		{
			long lCur = 0;

			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fXFV = pNcVals[0].as_float(0);

			long lVarNum = pVar->num_vals();
			pfX = new float[lVarNum];
			memset(pfX, 0, sizeof(float) * lVarNum);

			pVar->set_cur(lCur);
			pVar->get(pfX, lVarNum);

			for(int ixn = 0; ixn < m_x; ixn++)
			{
				pfX[ixn] = pfX[ixn] * 1000;
			}
		}
		else if(name == QString::fromLocal8Bit("DBZ"))
		{
			//获得维度
			//const int iDims = pVar->num_dims();

			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fValueFV = pNcVals[0].as_float(0);

			NcAtt* pAtt2 = pVar->get_att("missing_value");
			NcValues* pNcVals2 = pAtt2->values();
			fMisingValue = pNcVals2[0].as_float(0);
			
			osg::ref_ptr<osg::Vec3Array> pVec3Array = new osg::Vec3Array;
			osg::ref_ptr<osg::Vec4Array> pColorArray = new osg::Vec4Array;

			float* pData = new float[m_x];
			float* MaxValues = new float[m_x * m_y];
			//memset(MaxValues, 0xff, sizeof(float) * m_x * m_y);
			for(int iFillIndex = 0; iFillIndex < m_x * m_y; iFillIndex++)
			{
				MaxValues[iFillIndex] = -100.0f;
			}
			float fInvalid = -100.0f;

			for(int ii = 0; m_z > ii; ii++)
			{
				long lCur[4] = {0};
				lCur[0] = 0;
				if(iMode > -1 && iMode < m_z)
				{
					if(ii != iMode)
						continue;
				}

				lCur[1] = ii;

				for(int iii = 0; iii < m_y; iii++)
				{
					lCur[2] = iii;
					lCur[3] = 0;
					pVar->set_cur(lCur);

					long laCount[4] = {0};
					laCount[0] = 1;
					laCount[1] = 1;
					laCount[2] = 1;
					laCount[3] = m_x;

					memset(pData, 0, sizeof(float) * m_x);
					pVar->get(pData, laCount);

					for(int j = 0; j < m_x; j++)
					{
						float realValue = *(pData + j);
						
						if(abs(realValue - fMisingValue) < 0.00001)
							continue;

						float isMax = *(MaxValues + m_x * iii + j);
						if(isMax < realValue || (isMax - (fInvalid)) < 0.00001)
						{
							*(MaxValues + m_x * iii + j) = realValue;
						}
					}
				}
			}
#if 0
			QFile debugOutFile("D:/debug.txt");
			debugOutFile.open(QFile::WriteOnly | QIODevice::Truncate);
			QTextStream outText(&debugOutFile);
#endif
			for(int c = 0; c < m_y - 1; c++)
			{
				for(int r = 0; r < m_x - 1; r++)
				{
					ncbyte isMax = *(MaxValues + m_x * c + r);

					if(abs(isMax - fInvalid) < 0.00001) 
						continue;

					for(int k = 1; k < 16; k++)
					{
						int iSV = gValue[k];
						int iEV = gValue[k-1];

						if((isMax >= iEV && isMax < iSV))
						{
							QColor cc = gColorTable[k-1];
							int iR = cc.red();
							int iG = cc.green();
							int iB = cc.blue();
							int iA = cc.alpha();

							osg::Vec4 v4c(iR/255.0, iG/255.0, iB/255.0, 1.0);
							osg::Vec3 p1, p2, p3, p4;
							p1.set(OriginXY.x() + *(pfX + r), 10, OriginXY.z() + *(pfY + c));
							p2.set(OriginXY.x() + *(pfX + r + 1), 10, p1.z());
							p3.set(p2.x(), 10, OriginXY.z() + *(pfY + (c + 1)));
							p4.set(p1.x(), 10, p3.z());
#if 0
							if(abs(*(pfX + r)) - 0 < 0.00001 && r != 400)
							{
								outText<<QString("----------------------start(%1,%2)------------------------").arg(r).arg(c)<<endl;
								outText<<QString("p1:%1,%2,%3").arg(QString::number(p1.x(),'f',7)).arg(QString::number(p1.y(),'f',7)).arg(QString::number(p1.z(),'f',7))<<endl;
								outText<<QString("p2:%1,%2,%3").arg(QString::number(p2.x(),'f',7)).arg(QString::number(p2.y(),'f',7)).arg(QString::number(p2.z(),'f',7))<<endl;
								outText<<QString("p3:%1,%2,%3").arg(QString::number(p3.x(),'f',7)).arg(QString::number(p3.y(),'f',7)).arg(QString::number(p3.z(),'f',7))<<endl;
								outText<<QString("p4:%1,%2,%3").arg(QString::number(p4.x(),'f',7)).arg(QString::number(p4.y(),'f',7)).arg(QString::number(p4.z(),'f',7))<<endl;
								outText<<QString("----------------------end(%1,%2)------------------------").arg(r).arg(c)<<endl;
							}
#endif
							pVec3Array->push_back(p1);
							pVec3Array->push_back(p2);
							pVec3Array->push_back(p3);
							pVec3Array->push_back(p4);

							pColorArray->push_back(v4c);
							pColorArray->push_back(v4c);
							pColorArray->push_back(v4c);
							pColorArray->push_back(v4c);
						}
					}
				}
			}

			delete[] MaxValues;
			delete[] pData;
			delete[] pfX;
			delete[] pfY;
			delete[] pfHeight;
			vs = pVec3Array;
			cs = pColorArray;
		}
	}

	//NcBool ncR = pNcFile->close();
	return true;
}

int MakeTextureImps::ExportPartNcFile(NcFile *pNcFile, osg::Vec3 p1, osg::Vec3 p2, int iMode, const QString& strExortFile)
{
	long m_x, m_y, m_z;
	int lonFistIndex(0), latFistIndex(0), lonLastIndex(0), latLastIndex(0);
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
			//pNcFile->close();
			return 0;
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

	/*lonFistIndex = (p1.x() - fFirstLon) * 100;
	lonLastIndex = (p2.x() - fFirstLon) * 100;

	latFistIndex = (p2.y() - fFirstLat) * 100;
	latLastIndex = (p1.y() - fFirstLat) * 100;*/

	lonFistIndex = (p1.x() - fFirstLon)/0.01;
	lonLastIndex = (p2.x() - fFirstLon)/0.01;

	latFistIndex = (p2.y() - fFirstLat)/0.01;
	latLastIndex = (p1.y() - fFirstLat)/0.01;

	int m_xLen(lonLastIndex - lonFistIndex + 1), m_yLen(latLastIndex - latFistIndex + 1);

	//QImage* pImage = new QImage(m_xLen, m_yLen, QImage::Format_RGB32);
	//pImage->fill(QColor(255, 255, 255));
	QFile csvFile(strExortFile);
	if(!csvFile.open(QIODevice::Truncate|QIODevice::WriteOnly|QIODevice::Text)) return 0;
	QTextStream ts(&csvFile);
	/*ts<<QString::fromLocal8Bit("lon")<<QString::fromLocal8Bit(",")
		<<QString::fromLocal8Bit("lat")<<QString::fromLocal8Bit(",")
		<<QString::fromLocal8Bit("la")<<QString::fromLocal8Bit(",")
		<<QString::fromLocal8Bit("value")<<endl;*/
	CSV_LINE(ts, QString::fromLocal8Bit("lon"), QString::fromLocal8Bit("lat"), QString::fromLocal8Bit("layer"), QString::fromLocal8Bit("value"));
	QString* bufferString = new QString();
	const int iVars = pNcFile->num_vars();
	for(int i = 0; i < iVars; ++i)
	{
		NcVar* pVar =  pNcFile->get_var(i);
		if(!pVar)
		{
			return 0;
		}
		QString name = pVar->name();

		if(name == QString::fromLocal8Bit("reflectivity"))
		{
			ncbyte* pData = new ncbyte[m_xLen];
			
			for(int ii = 0; m_z > ii; ii++)
			{
				long lCur[3] = {0};

				if(iMode > -1 && iMode < m_z)
				{
					if(ii != iMode)
						continue;
				}

				lCur[0] = ii;
				
				bufferString->clear();
				for(int iii = 0; iii < m_yLen; iii++)
				{
					lCur[1] = latFistIndex + iii;
					lCur[2] = lonFistIndex;
					pVar->set_cur(lCur);

					long laCount[3] = {0};
					laCount[0] = 1;
					laCount[1] = 1;
					laCount[2] = m_xLen;

					memset(pData, 0, sizeof(ncbyte) * m_xLen);
					pVar->get(pData, laCount);

					for(int j = 0; j < m_xLen; j++)
					{
						ncbyte shN = *(pData + j);

						float fLon = p1.x() + 0.01 * j;
						float fLat = p1.y() - 0.01 * iii;
						int fLayer = 500 + 500 * ii;
						ncbyte fValue = shN;

						QString v1("%1"), v2("%1"), v3("%1"), v4("%1");
						v1 = v1.arg((double)fLon, 0, 'f', 2);
						v2 = v2.arg((double)fLat, 0, 'f', 2);
						v3 = v3.arg((int)fLayer);
						v4 = v4.arg((int)fValue);

						QString temp = MAKE_CSV_LINE(v1, v2, v3, v4);
						bufferString->push_back(temp);
						//CSV_LINE(ts, v1, v2, v3, v4);
					}
				}

				ts<<*bufferString;
			}

			delete[] pData;
		}
	}

	delete bufferString;
	csvFile.close();
	return 1;
}

int MakeTextureImps::ExportSectionFile(NcFile *pNcFile, osg::Vec3 p1, osg::Vec3 p2, const QString& strExortFile)
{
	QFile csvFile(strExortFile);
	if(!csvFile.open(QIODevice::Truncate|QIODevice::WriteOnly|QIODevice::Text)) return 0;
	QTextStream ts(&csvFile);
	CSV_LINE(ts, QString::fromLocal8Bit("lon"), QString::fromLocal8Bit("lat"), QString::fromLocal8Bit("layer"), QString::fromLocal8Bit("value"));
	QString* bufferString = new QString();

	osg::Vec3 v3LocalP1, v3LocalP2;
	p1.x() < p2.x() ? (v3LocalP1.set(p1.x()/1000, p1.y()/1000, p1.z()/1000), v3LocalP2.set(p2.x()/1000, p2.y()/1000, p2.z()/1000)) 
		: (v3LocalP1.set(p2.x()/1000, p2.y()/1000, p2.z()/1000), v3LocalP2.set(p1.x()/1000, p1.y()/1000, p1.z()/1000));

	long m_x, m_y, m_z;
	int lonFistIndex(0), latFistIndex(0), lonLastIndex(0), latLastIndex(0);
	//获得维度
	const int iDims = pNcFile->num_dims();
	for(int i = 0; iDims > i; i++)
	{
		NcDim* pNcDim = pNcFile->get_dim(i);
		std::string strDimName = pNcDim->name();
		if(strDimName == "time")
		{

		}
		else if(strDimName == "height")
		{
			m_z = pNcDim->size();
		}
		else if(strDimName == "Y")
		{
			m_y = pNcDim->size();
		}
		else if(strDimName == "X")
		{
			m_x = pNcDim->size();
		}
		else
		{
			//pNcFile->close();
			return false;
		}
	}
	//获得属性
	const int iAtts = pNcFile->num_atts();

	float fOriginLon = 0;
	float fOriginLat = 0;

	for(int i = 0; iAtts > i; i++)
	{
		NcAtt* pNcAtt = pNcFile->get_att(i);
		std::string strAttName = pNcAtt->name();
		if(strAttName.find("OriginX") != std::string::npos)
		{
			fOriginLon = pNcAtt->as_float(0);
		}
		else if(strAttName.find("OriginY") != std::string::npos)
		{
			fOriginLat = pNcAtt->as_float(0);
		}
	}

	float* pfHeight = nullptr;
	float* pfY = nullptr;
	float* pfX = nullptr;

	float fheightFV = 0;
	float fYFV = 0;
	float fXFV = 0;
	float fValueFV = 0;
	float fMisingValue = 0;

	osg::Vec3 lonLat(fOriginLon, fOriginLat, 0), v3Projection;
	m_pCoordinateTransform->Transform(lonLat, v3Projection);
	
	osg::Matrix moveOrigin;
	moveOrigin.makeTranslate(osg::Vec3(-v3Projection.x()/1000, 0, -v3Projection.z()/1000));
	int m_xLen(abs(v3LocalP2.x() - v3LocalP1.x()) + 1)/*, m_yLen(latLastIndex - latFistIndex + 1)*/;
	int iLeftX, iLeftY;
	const int iVars = pNcFile->num_vars();
	for(int i = 0; i < iVars; ++i)
	{
		NcVar* pVar =  pNcFile->get_var(i);
		if(!pVar)
		{
			//pNcFile->close();
			return false;
		}
		QString name = pVar->name();
		if(name == QString::fromLocal8Bit("time"))
		{

		}
		else if(name == QString::fromLocal8Bit("height"))
		{
			long lCur = 0;
			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fheightFV = pNcVals[0].as_float(0);

			long lVarNum = pVar->num_vals();
			pfHeight = new float[lVarNum];
			memset(pfHeight, 0, sizeof(pfHeight) * lVarNum);

			pVar->set_cur(lCur);
			pVar->get(pfHeight, lVarNum);
		}
		else if(name == QString::fromLocal8Bit("Y"))
		{
			long lCur = 0;

			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fYFV = pNcVals[0].as_float(0);

			long lVarNum = pVar->num_vals();
			pfY = new float[lVarNum];
			memset(pfY, 0, sizeof(pfY) * lVarNum);

			pVar->set_cur(lCur);
			pVar->get(pfY, lVarNum);

			iLeftY = pfY[0];

			delete[] pfY;
		}
		else if(name == QString::fromLocal8Bit("X"))
		{
			long lCur = 0;

			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fXFV = pNcVals[0].as_float(0);

			long lVarNum = pVar->num_vals();
			pfX = new float[lVarNum];
			memset(pfX, 0, sizeof(pfX) * lVarNum);

			pVar->set_cur(lCur);
			pVar->get(pfX, lVarNum);
			
			iLeftX = pfX[0];

			delete[] pfX;
		}
		else if(name == QString::fromLocal8Bit("DBZ"))
		{
			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fValueFV = pNcVals[0].as_float(0);

			NcAtt* pAtt2 = pVar->get_att("missing_value");
			NcValues* pNcVals2 = pAtt2->values();
			fMisingValue = pNcVals2[0].as_float(0);
			
			for(int ii = 0; m_z > ii; ii++)
			{
				long lCur[4] = {0};
				lCur[0] = 0;
				lCur[1] = ii;

				long laCount[4] = {0};
				laCount[0] = 1;
				laCount[1] = 1;
				laCount[2] = 1;
				laCount[3] = 1;

				int fLayer = 500 + 1000 * ii;
				for(size_t i = 0; i < m_xLen; i++)
				{
					//x坐标增量为1，通过直线方程求出y坐标的增量
					float x = v3LocalP1.x() + i;
					float y = ((x-v3LocalP1.x())/(v3LocalP2.x() - v3LocalP1.x()) + v3LocalP1.z()
						/(v3LocalP2.z() - v3LocalP1.z())) * (v3LocalP2.z() - v3LocalP1.z());
					//求出坐标距离原点(0,0)的距离，这个原点是回波图象的左下角的点
					osg::Vec3 v3Result = osg::Vec3(x, 0, y) * moveOrigin;
					int iYindex = -iLeftY + v3Result.z();

					int iXindex = -iLeftX + v3Result.x();

					if(iYindex < 0 || iYindex > m_y) continue;
					if(iXindex < 0 || iXindex > m_x) continue;

					lCur[2] = iYindex;
					lCur[3] = iXindex;
					pVar->set_cur(lCur);
					
					float pValue = fMisingValue;
					pVar->get(&pValue, laCount);
					
					goto_gis::Proj4Transform proj4;
					proj4.setSourceCRS(m_pCoordinateTransform->getDesCRS());
					proj4.setDestCRS(m_pCoordinateTransform->getSourceCRS());
					osg::Vec3 latLonPoint = proj4.Transform(osg::Vec3(x * 1000, 0, y * 1000));

					QString v1("%1"), v2("%1"), v3("%1"), v4("%1");
						v1 = v1.arg(latLonPoint.x(), 0, 'f', 2);
						v2 = v2.arg(latLonPoint.y(), 0, 'f', 2);
						v3 = v3.arg((int)fLayer);
						v4 = v4.arg((int)pValue);

						QString temp = MAKE_CSV_LINE(v1, v2, v3, v4);
						bufferString->push_back(temp);
				}

				ts<<*bufferString;
				bufferString->clear();
			}
		}
	}
	delete bufferString;
	csvFile.close();
	return 1;
}

int MakeTextureImps::GetVerticalData(NcFile *pNcFile, osg::Vec3Array& vec3Array, osg::Vec3 p2, double* pData, size_t* pSize )
{
	long m_x, m_y, m_z;
	int lonFistIndex(0), latFistIndex(0), lonLastIndex(0), latLastIndex(0);
	//获得维度
	const int iDims = pNcFile->num_dims();
	for(int i = 0; iDims > i; i++)
	{
		NcDim* pNcDim = pNcFile->get_dim(i);
		std::string strDimName = pNcDim->name();
		if(strDimName == "time")
		{

		}
		else if(strDimName == "height")
		{
			m_z = pNcDim->size();
		}
		else if(strDimName == "Y")
		{
			m_y = pNcDim->size();
		}
		else if(strDimName == "X")
		{
			m_x = pNcDim->size();
		}
		else
		{
			//pNcFile->close();
			return false;
		}
	}
	//获得属性
	const int iAtts = pNcFile->num_atts();

	float fOriginLon = 0;
	float fOriginLat = 0;

	for(int i = 0; iAtts > i; i++)
	{
		NcAtt* pNcAtt = pNcFile->get_att(i);
		std::string strAttName = pNcAtt->name();
		if(strAttName.find("OriginX") != std::string::npos)
		{
			fOriginLon = pNcAtt->as_float(0);
		}
		else if(strAttName.find("OriginY") != std::string::npos)
		{
			fOriginLat = pNcAtt->as_float(0);
		}
	}

	float* pfHeight = nullptr;
	float* pfY = nullptr;
	float* pfX = nullptr;

	float fheightFV = 0;
	float fYFV = 0;
	float fXFV = 0;
	float fValueFV = 0;
	float fMisingValue = 0;

	osg::Vec3 lonLat(fOriginLon, fOriginLat, 0), v3Projection;
	m_pCoordinateTransform->Transform(lonLat, v3Projection);
	osg::Matrix moveOrigin;
	moveOrigin.makeTranslate(osg::Vec3(-v3Projection.x()/1000, 0, -v3Projection.z()/1000));
	
	int iLeftX, iLeftY;

	float fInvalid = -100.0f;

	const int iVars = pNcFile->num_vars();
	for(int i = 0; i < iVars; ++i)
	{
		NcVar* pVar =  pNcFile->get_var(i);
		if(!pVar)
		{
			//pNcFile->close();
			return false;
		}
		QString name = pVar->name();
		if(name == QString::fromLocal8Bit("time"))
		{

		}
		else if(name == QString::fromLocal8Bit("height"))
		{
			long lCur = 0;
			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fheightFV = pNcVals[0].as_float(0);

			long lVarNum = pVar->num_vals();
			pfHeight = new float[lVarNum];
			memset(pfHeight, 0, sizeof(pfHeight) * lVarNum);

			pVar->set_cur(lCur);
			pVar->get(pfHeight, lVarNum);
		}
		else if(name == QString::fromLocal8Bit("Y"))
		{
			long lCur = 0;

			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fYFV = pNcVals[0].as_float(0);

			long lVarNum = pVar->num_vals();
			pfY = new float[lVarNum];
			memset(pfY, 0, sizeof(pfY) * lVarNum);

			pVar->set_cur(lCur);
			pVar->get(pfY, lVarNum);

			iLeftY = pfY[0];

			delete[] pfY;
		}
		else if(name == QString::fromLocal8Bit("X"))
		{
			long lCur = 0;

			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fXFV = pNcVals[0].as_float(0);

			long lVarNum = pVar->num_vals();
			pfX = new float[lVarNum];
			memset(pfX, 0, sizeof(pfX) * lVarNum);

			pVar->set_cur(lCur);
			pVar->get(pfX, lVarNum);
			
			iLeftX = pfX[0];

			delete[] pfX;
		}
		else if(name == QString::fromLocal8Bit("DBZ"))
		{
			NcAtt* pAtt = pVar->get_att("_FillValue");
			NcValues* pNcVals = pAtt->values();
			fValueFV = pNcVals[0].as_float(0);

			NcAtt* pAtt2 = pVar->get_att("missing_value");
			NcValues* pNcVals2 = pAtt2->values();
			fMisingValue = pNcVals2[0].as_float(0);

			size_t arraySize = vec3Array.size();
			for(size_t i = 0; i < arraySize; i++)
			{
				osg::Vec3 currentLatLon = vec3Array[i];
				osg::Vec3 currentLocalPos;
				m_pCoordinateTransform->Transform(currentLatLon, currentLocalPos);
				
				for(int ii = 0; m_z > ii; ii++)
				{
					long lCur[4] = {0};
					lCur[0] = 0;
					lCur[1] = ii;

					long laCount[4] = {0};
					laCount[0] = 1;
					laCount[1] = 1;
					laCount[2] = 1;
					laCount[3] = 1;

#if 1
					//x坐标增量为1，通过直线方程求出y坐标的增量
					float x = currentLocalPos.x() / 1000;
					float y = currentLocalPos.z() / 1000;
					//求出坐标距离原点(0,0)的距离，这个原点是回波图象的左下角的点
					osg::Vec3 v3Result = osg::Vec3(x, 0, y) * moveOrigin;
					int iYindex = -iLeftY + v3Result.z();

					int iXindex = -iLeftX + v3Result.x();
#else
					double latPropKM = (double)1/111.31955;
					double lonPropKM = (double)1/111.31955;

					double distanceLon = lonLat.x() - currentLatLon.x();
					double distanceLat = lonLat.y() - currentLatLon.y();

					int iYindex = distanceLon/lonPropKM - iLeftY;
					int iXindex = distanceLat/latPropKM - iLeftX;
#endif
					if(iYindex < 0 || iYindex > m_y)
					{
						pData[ii] = 100;
						continue;
					}
					if(iXindex < 0 || iXindex > m_x)
					{
						pData[ii] = 100;
						continue;
					}

					lCur[2] = iYindex;
					lCur[3] = iXindex;
					pVar->set_cur(lCur);
					
					float pValue = fMisingValue;
					pVar->get(&pValue, laCount);
					
					
					if(pValue < gValue[0] || abs(pValue - fMisingValue) < 0.0001)
					{
						pData[ii] = 100;
						continue;
					}
					
					pData[ii]  = pValue;
				}

				pData += m_z;
			}
			
		}
	}
	
	
	return 1;
}

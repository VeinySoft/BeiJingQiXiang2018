// PaoDianProvider.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "PaoDianProvider.h"
#include <QFile>
#include <QTextStream>
#include "PaoDianGeometry.h"

PaoDianProvider::PaoDianProvider()
{
	m_pPaoDianControler = new PaoDianControler;
	m_ExtendInterface[0] = m_pPaoDianControler;
}

bool PaoDianProvider::LoadFile( const std::string& filename )
{
	QFile file(filename.c_str());
	file.open(QIODevice::ReadOnly);
	PaoDianGeometry* pPaoDianGeom = new PaoDianGeometry;

	QTextStream inputText(&file);
	int iID = 0;
	m_pPaoDianControler->m_StandardItemModel.setHorizontalHeaderLabels(
		QStringList()<<QString::fromLocal8Bit("Ãû³Æ")<<QString::fromLocal8Bit("ÀàÐÍ"));
	
	while(!inputText.atEnd())
	{
		QString strLine = inputText.readLine();

		QStringList valueList = strLine.split('\t');

		if(valueList[0].size() == 0)
		//if(valueList.size() < 4)
		{
			continue;
		}
		else
		{
			if(valueList[0].contains("#"))
			{
				//NodeInfo ni0;
				int ilevel = iID++;
				QString strname = valueList[1];

				//NodeInfo ni1;
				int ilevel1 = iID++;
				QString strname1 = valueList[2];

				QList<QStandardItem*> itemList = m_pPaoDianControler->m_StandardItemModel.findItems(strname);

				QStandardItem* newChildItem = 0;
				if(itemList.size() == 0)
				{
					QStandardItem* newItem = new QStandardItem(strname);
					newItem->setCheckable(true);
					newItem->setTristate(true);
					newItem->setData(ilevel);

					newChildItem = new QStandardItem(strname1);
					newChildItem->setCheckable(true);
					newChildItem->setTristate(true);
					newChildItem->setData(ilevel1);

					newItem->appendRow(newChildItem);

					m_pPaoDianControler->m_StandardItemModel.appendRow(newItem);
				}
				else if(itemList.size() == 1)
				{
					newChildItem = new QStandardItem(strname1);
					newChildItem->setCheckable(true);
					newChildItem->setTristate(true);
					newChildItem->setData(ilevel1);
					itemList.at(0)->appendRow(newChildItem);
				}

				int iCount = valueList[3].toInt();
				while(iCount)
				{
					if(!inputText.atEnd())
					{
						QString strLine1 = inputText.readLine();
						QStringList valueList1 = strLine1.split('\t');
						QStringList ReslutList;
						for(int i = 0; i < valueList1.size(); i++)
						{
							if(valueList1[i].size() != 0)
							{
								ReslutList.push_back(valueList1[i]);
							}
							if(valueList1[i].size() == 0 && i > 11)
							{
								ReslutList.push_back(valueList1[i]);
							}
						}

						if(ReslutList.size() != 16)
						{
							iCount++;
							continue;
						}
						//ReslutList[0];
						int iMoveType = ReslutList[1].toInt();
						QString strName = ReslutList[2];
						//ReslutList[3];
						//ReslutList[4];
						int iHour = ReslutList[5].toInt();
						int iMinute = ReslutList[6].toInt();
						int iSecond = ReslutList[7].toInt();

						float flon = (float)iHour + iMinute/60.0 + iSecond/3600.0;
						
						//ReslutList[8];
						iHour = ReslutList[9].toInt();
						iMinute = ReslutList[10].toInt();
						iSecond = ReslutList[11].toInt();

						float flat = (float)iHour + iMinute/60.0 + iSecond/3600.0;
						QString strCannon_type = ReslutList[12] + " " + ReslutList[13];
						
						float fstart_engle = ReslutList[14].toFloat();
						float fend_engle = ReslutList[15].toFloat();

						int ilevel3 = iID++;
						PaoDianGeometry::GeomInfo info;
						info.fLon = flon;
						info.fLat = flat;
						info.fStart = fstart_engle;
						info.fEnd = fend_engle;
						info.iid = ilevel3;

						QStandardItem* newItem = new QStandardItem(strName);
						newItem->setCheckable(true);
						//newItem->setTristate(true);
						newItem->setData(ilevel3);

						QStandardItem* newTypeItme = new QStandardItem(strCannon_type);
						//newItem->setChild(0, 1, newTypeItme);
						int iMaxCount = newChildItem->rowCount();
						newChildItem->setChild(iMaxCount, 0, newItem);
						newChildItem->setChild(iMaxCount, 1, newTypeItme);
						//newChildItem->appendColumn(newItem);
						pPaoDianGeom->AddPaoDian(info);
					}
					iCount--;
				}
			}
			else
			{

			}
		}
	}
	this->ParentLayer()->GetMgoGeometry()->insert(goto_gis::Layer::MgoGeometryPair(0, pPaoDianGeom));
	m_pPaoDianControler->m_pPaoDianGeometry = pPaoDianGeom;

	return true;
}

void PaoDianProvider::AreaAdd( bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p )
{
	
}




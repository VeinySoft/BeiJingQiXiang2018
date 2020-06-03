#include "StdAfx.h"
#include "ConfigAccesser.h"

//QColor gColorTable1[] = 
//{
//	QColor(0, 172, 164), QColor(192, 192, 154), QColor(122, 144, 234),
//	QColor(30, 38, 108),  QColor(166, 252, 168),     QColor(0, 234, 0),    QColor(16, 146, 26),
//	QColor(255, 244, 100),  QColor(200, 200,   2), QColor(140, 140, 0), QColor(254, 172, 172),
//	QColor(254, 100, 92 ),  QColor(235,   2,  48), QColor(212, 142, 254),  QColor(170, 36, 250), QColor(0, 0, 0)
//};
//
//short gValue1[] = 
//{
//	-5,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70
//};


ConfigAccesser::ConfigAccesser(void)
{
}


ConfigAccesser::~ConfigAccesser(void)
{
}

void ConfigAccesser::Read( const QString& fileName )
{
	QFile file(fileName);

	if(!file.open(QFile::ReadOnly))
		return;
	
	m_FileName = fileName;

	QDomDocument XmlReader;
	XmlReader.setContent(&file);

	//读取模式
	QDomNodeList nodeList = XmlReader.elementsByTagName("mode");
	if(nodeList.size() > 0)
	{
		QDomNode node  = nodeList.at(0);
		QDomElement e = node.toElement();
		m_Data.mode_type = e.attribute("type").toInt();
	}
	//读取历史设置
	nodeList = XmlReader.elementsByTagName("datatime");
	if(nodeList.size() > 0)
	{
		QDomNode node  = nodeList.at(0);
		QDomElement e = node.toElement();
		m_Data.start_time = e.attribute("start");
		m_Data.end_time = e.attribute("end");
	}
	//读取文件夹设置
	nodeList = XmlReader.elementsByTagName("file_path");
	if(nodeList.size() > 0)
	{
		QDomNode node  = nodeList.at(0);
		QDomElement e = node.toElement();
		m_Data.path = e.attribute("value");
	}
	//读取文件夹设置2
	nodeList = XmlReader.elementsByTagName("file_path2");
	if(nodeList.size() > 0)
	{
		QDomNode node  = nodeList.at(0);
		QDomElement e = node.toElement();
		m_Data.path2 = e.attribute("value");
	}

	//读取文件夹设置2
	nodeList = XmlReader.elementsByTagName("file_path3");
	if(nodeList.size() > 0)
	{
		QDomNode node  = nodeList.at(0);
		QDomElement e = node.toElement();
		m_Data.path3 = e.attribute("value");
	}

	//读取跟踪框
	nodeList = XmlReader.elementsByTagName("track_cube");
	if(nodeList.size() > 0)
	{
		QDomNode node  = nodeList.at(0);
		QDomNodeList child = node.childNodes();

		for(int i = 0; i < child.size(); i++)
		{
			QDomNode childNode = child.at(i);

			QDomElement ce = childNode.toElement();
			cube_data cd;
			cd.name = ce.attribute("name");
			cd.height = ce.attribute("height");
			cd.used = ce.attribute("used").toInt();
			cd.left_top_lon = ce.attribute("left_top_lon").toFloat();
			cd.left_top_lat = ce.attribute("left_top_lat").toFloat();
			cd.right_bottom_lon = ce.attribute("right_bottom_lon").toFloat();
			cd.right_bottom_lat = ce.attribute("right_bottom_lat").toFloat();
			cd.height_up =   ce.attribute("height_up");
			cd.height_down = ce.attribute("height_down");
			
			cd.point1X = ce.attribute("point1X").toFloat();
			cd.point1Y = ce.attribute("point1Y").toFloat();
			cd.point2X = ce.attribute("point2X").toFloat();
			cd.point2Y = ce.attribute("point2Y").toFloat();
			cd.volumeMax = ce.attribute("volumeMax").toInt();
			cd.heightMax = ce.attribute("heightMax").toInt();

			cd.listNum = ce.attribute("listNum").toInt();
			QDomNodeList displayNodes = childNode.childNodes();
			//读取显示设置
			for(int i = 0; i < displayNodes.size(); i++)
			{
				QDomNode childNode = displayNodes.at(i);
				QString strName = childNode.nodeName();
				if(strName == QString::fromLocal8Bit("value"))
				{
					QDomElement de = childNode.toElement();
					int iV = de.attribute("enable").toInt();
					cd.displays.push_back(iV);
				}
				if(strName == QString::fromLocal8Bit("item"))
				{
					QDomElement de = childNode.toElement();
					QString strV = de.attribute("value");
					cd.valueList.push_back(strV);
				}
			}
			m_Data.cubes.push_back(cd);
		}
	}
	//读取导出设置
	nodeList = XmlReader.elementsByTagName("exprot");
	if(nodeList.size() > 0)
	{
		QDomNode node  = nodeList.at(0);
		
		QDomElement e = node.toElement();
		m_Data.export_config.type = e.attribute("type");
		m_Data.export_config.width = e.attribute("width").toInt();
		m_Data.export_config.height = e.attribute("height").toInt();

	}
	
	file.close();
}

void ConfigAccesser::Write()
{
	if(m_FileName.size() == 0) return;

	QDomDocument XmlWriter;

	QDomElement allConfigElement = XmlWriter.createElement(QObject::tr("AllConfig"));
	//读取模式
	QDomElement newElement = XmlWriter.createElement("mode");
	newElement.setAttribute("type", m_Data.mode_type);
	allConfigElement.appendChild(newElement);

	//读取历史设置
	newElement = XmlWriter.createElement("datatime");
	newElement.setAttribute("start", m_Data.start_time);
	newElement.setAttribute("end", m_Data.end_time);
	allConfigElement.appendChild(newElement);

	//读取文件夹设置
	newElement = XmlWriter.createElement("file_path");
	newElement.setAttribute("value", m_Data.path);
	allConfigElement.appendChild(newElement);

	//读取文件夹设置2
	newElement = XmlWriter.createElement("file_path2");
	newElement.setAttribute("value", m_Data.path2);
	allConfigElement.appendChild(newElement);

	//读取文件夹设置3
	newElement = XmlWriter.createElement("file_path3");
	newElement.setAttribute("value", m_Data.path3);
	allConfigElement.appendChild(newElement);

	//读取跟踪框
	newElement = XmlWriter.createElement("track_cube");
	
	for(int i = 0; i < m_Data.cubes.size(); i++)
	{
		QDomElement ce = XmlWriter.createElement("cube");
		cube_data cd = m_Data.cubes.at(i);
		ce.setAttribute("name", cd.name);
		ce.setAttribute("height", cd.height);
		ce.setAttribute("used", cd.used);
		ce.setAttribute("left_top_lon", cd.left_top_lon);
		ce.setAttribute("left_top_lat", cd.left_top_lat);
		ce.setAttribute("right_bottom_lon", cd.right_bottom_lon);
		ce.setAttribute("right_bottom_lat", cd.right_bottom_lat);
		ce.setAttribute("height_up", cd.height_up);
		ce.setAttribute("height_down", cd.height_down);

		ce.setAttribute("point1X", cd.point1X);
		ce.setAttribute("point1Y", cd.point1Y);
		ce.setAttribute("point2X", cd.point2X);
		ce.setAttribute("point2Y", cd.point2Y);
		ce.setAttribute("listNum", cd.listNum);
		ce.setAttribute("volumeMax", cd.volumeMax);
		ce.setAttribute("heightMax", cd.heightMax);
		
		//QDomElement displayNode = XmlWriter.createElement("display");
		//读取显示设置
		for(int i = 0; i < cd.displays.size(); i++)
		{
			int iS = cd.displays.at(i);
			QString name = "value";

			if(name.size() == 0) break;

			QDomElement child = XmlWriter.createElement(name);

			child.setAttribute("enable", iS);
			ce.appendChild(child);
		}

		for(int i = 0; i < cd.valueList.size(); i++)
		{

			QDomElement child = XmlWriter.createElement("item");

			child.setAttribute("value", cd.valueList.at(i));
			ce.appendChild(child);
		}
		//ce.appendChild(displayNode);

		newElement.appendChild(ce);
	}
	allConfigElement.appendChild(newElement);

	//读取导出设置
	newElement = XmlWriter.createElement("exprot");
	newElement.setAttribute("type", m_Data.export_config.type);
	newElement.setAttribute("width", m_Data.export_config.width);
	newElement.setAttribute("height", m_Data.export_config.height);
	allConfigElement.appendChild(newElement);


	XmlWriter.appendChild(allConfigElement);

	QFile file(m_FileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
		return ;
	QTextStream out(&file);
	out.setCodec("UTF-8");
	
	XmlWriter.save(out,4, QDomNode::EncodingFromTextStream);
	file.close();
}

void ConfigAccesser::ConfigCubes( int iCommand, QString strName, cube_data* pData )
{
	switch(iCommand)
	{
	case 0://add
		m_Data.cubes.push_back(*pData);
		break;
	case 1://modify
		for(int i = 0; i < m_Data.cubes.size(); i++)
		{
			if(m_Data.cubes.at(i).name == pData->name)
			{
				m_Data.cubes[i] = *pData;
			}
		}
		break;
	case 2://delete
		for(int i = 0; i < m_Data.cubes.size(); i++)
		{
			if(m_Data.cubes.at(i).name == strName)
			{
				m_Data.cubes.remove(i);
			}
		}
		break;
	default:
		break;
	}
}

cube_data ConfigAccesser::GetCubeFromName( QString strName )
{
	cube_data cd;
	for(int i = 0; i < m_Data.cubes.size(); i++)
	{
		if(m_Data.cubes.at(i).name == strName)
		{
			return m_Data.cubes.at(i);
		}
	}
	return cd;
}

void ConfigAccesser::GetCubes( QVector<cube_data>& cubes )
{
	cubes = m_Data.cubes;
}
//
//int ConfigAccesser::GetValue( int iI )
//{
//	if(iI >= 0 && iI < 16)
//	{
//		return gValue1[iI];
//	}
//	return -999;
//}
//
//int ConfigAccesser::GetIndexFromValue( int iV )
//{
//	for(int i = 0; i < 16; i++)
//	{
//		if(gValue1[i] == iV)
//			return i;
//	}
//
//	return -1;
//}
//
//QColor ConfigAccesser::GetColor( int iI )
//{
//	if(iI >= 0 && iI < 16) return gColorTable1[iI];
//	return QColor(0,0,0);
//}

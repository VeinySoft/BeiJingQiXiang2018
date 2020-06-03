#pragma once
struct cube_data
{
	QString name;
	QString height;
	bool used;
	float left_top_lon;
	float left_top_lat;
	float right_bottom_lon;
	float right_bottom_lat;
	QString height_up;
	QString height_down;
	QVector<int> displays;//list0
	QStringList valueList;//list1
	int listNum;//Ê¹ÓÃÄÄ¸ölist
	float point1X, point1Y;
	float point2X, point2Y;
	int volumeMax;
	int heightMax;
};

struct exprot_data
{
	QString type;
	int width;
	int height;
};

struct config_data
{
	int mode_type;
	QString path;
	QString path2;
	QString path3;
	QString start_time;
	QString end_time;
	QVector<cube_data> cubes;
	exprot_data export_config;
};

class ConfigAccesser
{
public:
	ConfigAccesser(void);
	~ConfigAccesser(void);

	void Read(const QString& fileName);
	void Write();
	/*inline void Get(config_data& pED){pED = m_Data;}
	inline void Set(config_data& pED){m_Data = pED;}*/
	void ConfigCubes(int iCommand, QString strName, cube_data* pData);
	cube_data GetCubeFromName(QString strName);
	void GetCubes(QVector<cube_data>& cubes);

	inline void ModeType(int imt){m_Data.mode_type = imt;}
	inline int ModeType(){return m_Data.mode_type;}
	inline void Path(const QString& strPath){m_Data.path = strPath;}
	inline QString Path(){return m_Data.path;}
	inline void HistoryPath(const QString& strPath){m_Data.path2 = strPath;}
	inline QString HistoryPath(){return m_Data.path2;}
	inline void TrackResultSavePath(const QString& strPath){m_Data.path3 = strPath;}
	inline QString TrackResultSavePath(){return m_Data.path3;}
	inline void StartTime(const QString& strST){m_Data.start_time = strST;}
	inline QString StartTime(){return m_Data.start_time;}
	inline void EndTime(const QString& strET){m_Data.end_time = strET;}
	inline QString EndTime(){return m_Data.end_time;}
	/*inline void Displays(int iIndex, int iState){iIndex < 3 ? m_Data.displays[iIndex] = iState : 0;}
	inline int Displays(int iIndex){
	return  iIndex < m_Data.displays.size() ?
	m_Data.displays.at(iIndex) : -1;
	}*/
	//inline QVector<int>* Displays(){return &(m_Data.displays);}
	inline void ExportConfig(const exprot_data& ed)
	{
		m_Data.export_config = ed;
	}
	inline exprot_data ExprotConfig(){return m_Data.export_config;}
private:
	config_data m_Data;
	QString m_FileName;
};


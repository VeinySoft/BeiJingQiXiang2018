#pragma once
class ParseData
{
public:
	struct AdditionalData
	{
		char* buffer;
		size_t size;
	};

	enum DataType{RASTER, CURVE};
	ParseData();
	virtual ~ParseData();

	virtual bool ReadDataFromFile(const std::string& strFielName) = 0;
	virtual bool ReadDataFromMemory(const void* pData) = 0;
	virtual ParseData::DataType GetDataType() = 0;
	inline double* GetData() { return m_pData; }
	inline size_t GetDataSize() { return m_DataSize; }
	inline int GetDataColumnCount() { return m_ColumnCount; }
	inline void SetAxisProperty(double fXMin, double fXMax, double fYMin, double fYMax, double fZMin, double fZMax) {
		m_fXMin = fXMin;
		m_fXMax = fXMax;
		m_fYMin = fYMin;
		m_fYMax = fYMax;
		m_fZMin = fZMin;
		m_fZMax = fZMax;
	}
	inline void GetAxisProperty(double* fXMin, double* fXMax, double* fYMin, double* fYMax, double* fZMin, double* fZMax) {
		*fXMin = m_fXMin;
		*fXMax = m_fXMax;
		*fYMin = m_fYMin;
		*fYMax = m_fYMax;
		*fZMin = m_fZMin;
		*fZMax = m_fZMax;
	}

	inline void SetAxisComment(const std::string& strXComment, const std::string& strYComment, const std::string& strZComment) {
		m_strXComment = strXComment;
		m_strYComment = strYComment;
		m_strZComment = strZComment;
	}

	inline void GetAxisComment(std::string& strXComment, std::string& strYComment, std::string& strZComment)
	{
		strXComment = m_strXComment;
		strYComment = m_strYComment;
		strZComment = m_strZComment;
	}

	inline void SetTitle(const std::string& strTitle) { m_strTtile = strTitle; }

	inline void GetTitle(std::string& strTitle) { strTitle = m_strTtile; }

	inline AdditionalData* GetAdditionalData() { return m_OtherData; }

	inline void SetAxisInterval(double fXInterval, double fYInterval, double fZInterval)
	{
		m_fXInterval = fXInterval;
		m_fYInterval = fYInterval;
		m_fZInterval = fZInterval;
	}

	inline void GetAxisInterval(double* fXInterval, double* fYInterval, double* fZInterval)
	{
		*fXInterval = m_fXInterval;
		*fYInterval = m_fYInterval;
		*fZInterval = m_fZInterval;
	}
protected:
	double* m_pData;
	size_t m_DataSize;
	int m_ColumnCount;
	double m_fXMin;
	double m_fXMax;
	double m_fYMin;
	double m_fYMax;
	double m_fZMin;
	double m_fZMax;
	double m_fXInterval;
	double m_fYInterval;
	double m_fZInterval;

	std::string m_strXComment;
	std::string m_strYComment;
	std::string m_strZComment;

	std::string m_strTtile;

	AdditionalData* m_OtherData;
};


#pragma once
#include <QtCore/QRunnable>
#include <QtCore/QStringList>
#include "DataObtainInterface.h"

class InvokeTrecThread :
	public QRunnable
{
public:
	InvokeTrecThread(const P_TREC_NC_IN_VECTOR pFileVector, const P_CUBE_DATA pCube, P_TREC_NC_MAP_OUT pOutMap);
	~InvokeTrecThread(void);

	virtual void run();
	inline void SetCubeName(const QString& strCubesName){
		m_strCubeName = strCubesName;}
	inline void SetFileList(const QStringList& fileList){
		m_FileVector = fileList;}

	inline void SetCubeName(QString* pCN){m_pCubeName = pCN;}
	inline void SetFileList(QStringList* pFL){m_pFileList = pFL;}
	inline bool GetSetpWait(){return m_bSetpWait;}

private:
	P_TREC_NC_IN_VECTOR m_pFileVector;
	CUBE_DATE m_pCube;
	P_TREC_NC_MAP_OUT m_pOutMap;
	QString m_strCubeName;
	QStringList m_FileVector;
	QString* m_pCubeName;
	QStringList* m_pFileList;
	bool m_bSetpWait;
};


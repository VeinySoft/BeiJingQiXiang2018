#include "InvokeTrecThread.h"
#include "MyImageSequence.h"
#include <QtCore/QString>
extern _TrecAlgorithm	g_pTrecAlgorithm;
//extern "C" bool TrecAlgorithm (const P_TREC_NC_IN_VECTOR pFileVector, const P_CUBE_DATA pCube, P_TREC_NC_MAP_OUT pOutMap);
InvokeTrecThread::InvokeTrecThread(const P_TREC_NC_IN_VECTOR pFileVector, const P_CUBE_DATA pCube, P_TREC_NC_MAP_OUT pOutMap)
{
	m_pFileVector = pFileVector;
	m_pCube = *pCube;
	m_pOutMap = pOutMap;
	m_bSetpWait = true;
}

InvokeTrecThread::~InvokeTrecThread(void)
{
}

void InvokeTrecThread::run()
{
	//Ó¦ÓÃËã·¨
	if(m_pFileVector->size() > 1)
	{
		g_pTrecAlgorithm(m_pFileVector, &m_pCube, m_pOutMap);

		*m_pCubeName = m_strCubeName;
		*m_pFileList = m_FileVector;
	}
}


#include "MakeTextureThread.h"
#include "MakeTextureImps.h"

MakeTextureThread::MakeTextureThread(void) : m_Stop(false)
	, m_pMaker(new MakeTextureImps(0))
{
}


MakeTextureThread::~MakeTextureThread(void)
{
}

void MakeTextureThread::run()
{
	while(!m_Stop)
	{
		for(int i = 0; i < m_FileList.size(); i++)
		{
			m_Mutex.lock();
			std::string strFileName = m_FileList.at(i).toStdString();
			m_Mutex.unlock();

			//m_pMaker->loadNcFileToTexture(strFileName, )
		}
	}
}


void MakeTextureThread::SetTextureQueue( std::vector<osg::ref_ptr<osg::Texture2D>>* pTextures 
	, std::vector<osg::ref_ptr<osg::Vec3Array>>* pVertexes )
{

}

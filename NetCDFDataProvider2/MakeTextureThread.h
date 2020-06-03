#pragma once
#include <QtCore/QMutex>
#include <QtCore/QRunnable>
#include <QtCore/QStringList>
#include <osg/Texture2D>

class MakeTextureInterface;

class MakeTextureThread :
	public QRunnable
{
public:
	virtual void run();
	void SetMaker(MakeTextureInterface* pMTI){m_pMaker = pMTI;}
	MakeTextureInterface* GetMaker(){return m_pMaker;}

	void SetTextureQueue(std::vector<osg::ref_ptr<osg::Texture2D>>* pTextures
		, std::vector<osg::ref_ptr<osg::Vec3Array>>* pVertexes);
	MakeTextureThread(void);
	~MakeTextureThread(void);
	inline void PutFile(const QString strFileName){
		m_Mutex.lock();
		m_FileList.push_back(strFileName);
		m_Mutex.unlock();
	}

private:
	QMutex m_Mutex;
	bool m_Stop;
	MakeTextureInterface* m_pMaker;
	QStringList m_FileList;
	std::vector<osg::Texture2D*>* m_pQueue;
};


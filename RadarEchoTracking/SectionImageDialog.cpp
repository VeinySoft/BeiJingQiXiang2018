#include "StdAfx.h"
#include "SectionImageDialog.h"


SectionImageDialog::SectionImageDialog()
{
	m_pPixmapItem = 0;
	m_Step.setupUi(this);

	QGraphicsScene* pScene = new QGraphicsScene;
	m_Step.graphicsView->setScene(pScene);

	connect(m_Step.pushButton, SIGNAL(clicked(bool)), SLOT(slot_SaveImage(bool)));
	connect(m_Step.pushButton_2, SIGNAL(clicked(bool)), SLOT(slot_SaveCSV(bool)));
}


SectionImageDialog::~SectionImageDialog(void)
{
}

void SectionImageDialog::slot_SaveImage( bool )
{
	if(m_pPixmapItem)
	{
		QPixmap ipx =  m_pPixmapItem->pixmap();
		QString strFileName = QFileDialog::getSaveFileName(this
			, QString::fromLocal8Bit("导出图片"), "", "JPEG (*.jpg *.jpeg)");
		if(strFileName.size() > 0)
			ipx.save(strFileName);
	}
}

void SectionImageDialog::UpdateImage( const QPixmap& pixmap )
{
	QGraphicsScene* pScene = m_Step.graphicsView->scene();
	pScene->clear();
	m_pPixmapItem = new QGraphicsPixmapItem(pixmap, 0, pScene);
	pScene->addItem(m_pPixmapItem);
}

void SectionImageDialog::slot_SaveCSV(bool)
{
	QPixmap ipx =  m_pPixmapItem->pixmap();
		QString strFileName = QFileDialog::getSaveFileName(this
			, QString::fromLocal8Bit("导出CSV文件"), "", "CSV (*.csv)");
		if(strFileName.size() > 0)
			m_pCI->ExportSctionFile(m_strNCFile, m_v1, m_v2, 0, strFileName);
}
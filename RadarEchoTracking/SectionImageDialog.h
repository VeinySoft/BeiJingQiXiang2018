#pragma once
#include "ui_SectionDialog.h"
#include <osg/Vec3>

class ControlorInterface;

class SectionImageDialog :
	public QDialog
{
	Q_OBJECT
public:
	SectionImageDialog();
	~SectionImageDialog(void);
	Ui::SectionDialog m_Step;
	QGraphicsPixmapItem* m_pPixmapItem;
	void UpdateImage(const QPixmap& pixmap);
	inline void UpdateRelevantValue(ControlorInterface* pCI, const QString& strFile, const osg::Vec3& p1, const osg::Vec3& p2){m_pCI = pCI;m_strNCFile = strFile;m_v1 = p1;m_v2 = p2;}
private:
	QString m_strNCFile;
	ControlorInterface* m_pCI;
	osg::Vec3 m_v1, m_v2;
private slots:
	void slot_SaveImage(bool);
	void slot_SaveCSV(bool);
};


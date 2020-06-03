#pragma once
#include "ui_SectionDialog.h"

class SectionImageDialog :
	public QDialog
{
	Q_OBJECT
public:
	SectionImageDialog(void);
	~SectionImageDialog(void);
	Ui::SectionDialog m_Step;
	QGraphicsPixmapItem* m_pPixmapItem;
	void UpdateImage(const QPixmap& pixmap);
private slots:
	void slot_SaveImage(bool);
};


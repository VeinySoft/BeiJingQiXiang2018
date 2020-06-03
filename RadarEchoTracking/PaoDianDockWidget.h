#pragma once
#include "ui_PaoDianDockWidget.h"
#include <QStandardItemModel>

class PaoDianDockWidget :
	public QDockWidget
{
	Q_OBJECT
public:
	PaoDianDockWidget(void);
	~PaoDianDockWidget(void);
	Ui::PaoDianDockWidget m_Setup;
	void LoadSlot(QStandardItemModel* pSIM);
	void treeItem_checkAllChild(QStandardItem * item, bool check);
	void treeItem_checkAllChild_recursion(QStandardItem * item,bool check);
	inline void SetPaoDinaControlerInterface(PaoDinaControlerInterface* pPDCI){
		m_pPaoDinaControlerInterface = pPDCI;}

private slots :
	void treeItemChanged(QStandardItem * item);
	
private:
	PaoDinaControlerInterface* m_pPaoDinaControlerInterface;
	void treeItem_CheckChildChanged(QStandardItem * item);
	Qt::CheckState checkSibling(QStandardItem * item);
};


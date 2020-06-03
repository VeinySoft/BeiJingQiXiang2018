#pragma once
#include "ui_PaoDianListDialog.h"

class PaoDinaControlerInterface;

class PaoDianListDialog :
	public QDialog
{
	Q_OBJECT
public:
	PaoDianListDialog(void);
	~PaoDianListDialog(void);
	void LoadSlot(QStandardItemModel* pSIM);
	void treeItem_checkAllChild(QStandardItem * item, bool check);
	void treeItem_checkAllChild_recursion(QStandardItem * item,bool check);
	inline void SetPaoDinaControlerInterface(PaoDinaControlerInterface* pPDCI){
		m_pPaoDinaControlerInterface = pPDCI;}

	Ui_Dialog m_Setup;
private slots :
	void treeItemChanged(QStandardItem * item);
	void slot_accepted();
private:
	void treeItem_CheckChildChanged(QStandardItem * item);
	Qt::CheckState checkSibling(QStandardItem * item);
	PaoDinaControlerInterface* m_pPaoDinaControlerInterface;
};


#include "StdAfx.h"
#include "PaoDianDockWidget.h"


PaoDianDockWidget::PaoDianDockWidget(void)
{
	m_Setup.setupUi(this);
}


PaoDianDockWidget::~PaoDianDockWidget(void)
{
}

void PaoDianDockWidget::treeItemChanged( QStandardItem * item )
{
	if ( item == nullptr )
		return ;
	if ( item->isCheckable ())
	{
		//�����Ŀ�Ǵ��ڸ�ѡ��ģ���ô�ͽ�������Ĳ���
		Qt::CheckState state = item->checkState (); //��ȡ��ǰ��ѡ��״̬
		int iD = item->data().toInt();
		
		if ( item->isTristate())
		{
			//�����Ŀ����̬�ģ�˵�����Զ���Ŀ¼����ȫѡ��ȫ��ѡ������
			if ( state != Qt::PartiallyChecked )
			{
				//��ǰ��ѡ��״̬����Ҫ��������Ŀ����ȫѡ
				treeItem_checkAllChild ( item, state==Qt::Checked ? true : false );
			}
		}
		else
		{
			//˵������̬�ģ���̬��Ը�������̬��Ӱ��
			//�ж��ֵܽڵ�����
			treeItem_CheckChildChanged ( item );

			if(item->checkState() == Qt::Checked)
			{
				m_pPaoDinaControlerInterface->DisplayFromIDs(iD);
			}
			else if(item->checkState() == Qt::Unchecked)
			{
				m_pPaoDinaControlerInterface->HideFromIDs(iD);
			}
		}
	}
}

void PaoDianDockWidget::LoadSlot( QStandardItemModel* pSIM )
{
	connect(pSIM ,SIGNAL(itemChanged(QStandardItem*)), SLOT(treeItemChanged(QStandardItem *)));

	m_Setup.treeView->setModel(pSIM);

	m_Setup.treeView->setColumnWidth(0, 200);
	m_Setup.treeView->setColumnWidth(1, 50);
	m_Setup.treeView->expandAll();
}

///
/// \brief �ݹ��������е�����ĿΪȫѡ��ȫ��ѡ״̬
/// \param item ��ǰ��Ŀ
/// \param check trueʱΪȫѡ��falseʱȫ��ѡ
///
void PaoDianDockWidget::treeItem_checkAllChild(QStandardItem * item, bool check)
{
	if(item == nullptr)
		return;
	int rowCount = item->rowCount();
	for(int i=0;i<rowCount;++i)
	{
		QStandardItem* childItems = item->child(i);
		treeItem_checkAllChild_recursion(childItems,check);
	}
	if(item->isCheckable())
		item->setCheckState(check ? Qt::Checked : Qt::Unchecked);
}

void PaoDianDockWidget::treeItem_checkAllChild_recursion(QStandardItem * item,bool check)
{
	if(item == nullptr)
		return;
	int rowCount = item->rowCount();
	for(int i=0;i<rowCount;++i)
	{
		QStandardItem* childItems = item->child(i);
		treeItem_checkAllChild_recursion(childItems,check);
	}
	if(item->isCheckable())
		item->setCheckState(check ? Qt::Checked : Qt::Unchecked);
}

///
/// \brief �����ӽڵ�ĸı䣬���ĸ��ڵ��ѡ�����
/// \param item
///
void PaoDianDockWidget::treeItem_CheckChildChanged(QStandardItem * item)
{
	if(nullptr == item)
		return;
	Qt::CheckState siblingState = checkSibling(item);
	QStandardItem * parentItem = item->parent();
	
	if(nullptr == parentItem)
		return;
	if(Qt::PartiallyChecked == siblingState)
	{
		if(parentItem->isCheckable() && parentItem->isTristate())
			parentItem->setCheckState(Qt::PartiallyChecked);
	}
	else if(Qt::Checked == siblingState)
	{
		if(parentItem->isCheckable())
			parentItem->setCheckState(Qt::Checked);
	}
	else
	{
		if(parentItem->isCheckable())
			parentItem->setCheckState(Qt::Unchecked);
	}
	treeItem_CheckChildChanged(parentItem);
}

///
/// \brief �����ֵܽڵ������������ѡ�з���Qt::Checked������ѡ��Qt::Unchecked,����ȫѡ�з���Qt::PartiallyChecked
/// \param item
/// \return �����ѡ�з���Qt::Checked������ѡ��Qt::Unchecked,����ȫѡ�з���Qt::PartiallyChecked
///
Qt::CheckState PaoDianDockWidget::checkSibling(QStandardItem * item)
{
	//��ͨ�����ڵ��ȡ�ֵܽڵ�
	QStandardItem * parent = item->parent();
	if(nullptr == parent)
		return item->checkState();
	int brotherCount = parent->rowCount();
	int checkedCount(0),unCheckedCount(0);
	Qt::CheckState state;
	for(int i=0;i<brotherCount;++i)
	{
		QStandardItem* siblingItem = parent->child(i);
		state = siblingItem->checkState();
		if(Qt::PartiallyChecked == state)
			return Qt::PartiallyChecked;
		else if(Qt::Unchecked == state)
			++unCheckedCount;
		else
			++checkedCount;
		if(checkedCount>0 && unCheckedCount>0)
			return Qt::PartiallyChecked;
	}
	if(unCheckedCount>0)
		return Qt::Unchecked;
	return Qt::Checked;
}
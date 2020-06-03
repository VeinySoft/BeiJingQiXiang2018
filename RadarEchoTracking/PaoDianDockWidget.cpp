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
		//如果条目是存在复选框的，那么就进行下面的操作
		Qt::CheckState state = item->checkState (); //获取当前的选择状态
		int iD = item->data().toInt();
		
		if ( item->isTristate())
		{
			//如果条目是三态的，说明可以对子目录进行全选和全不选的设置
			if ( state != Qt::PartiallyChecked )
			{
				//当前是选中状态，需要对其子项目进行全选
				treeItem_checkAllChild ( item, state==Qt::Checked ? true : false );
			}
		}
		else
		{
			//说明是两态的，两态会对父级的三态有影响
			//判断兄弟节点的情况
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
/// \brief 递归设置所有的子项目为全选或全不选状态
/// \param item 当前项目
/// \param check true时为全选，false时全不选
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
/// \brief 根据子节点的改变，更改父节点的选择情况
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
/// \brief 测量兄弟节点的情况，如果都选中返回Qt::Checked，都不选中Qt::Unchecked,不完全选中返回Qt::PartiallyChecked
/// \param item
/// \return 如果都选中返回Qt::Checked，都不选中Qt::Unchecked,不完全选中返回Qt::PartiallyChecked
///
Qt::CheckState PaoDianDockWidget::checkSibling(QStandardItem * item)
{
	//先通过父节点获取兄弟节点
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
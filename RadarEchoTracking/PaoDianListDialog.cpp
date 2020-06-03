#include "StdAfx.h"
#include "PaoDianListDialog.h"
#include "CubeSettingsDialogImps.h"
#include "MainWindow.h"
#include <Proj4Transform.h>

extern MainWindow* g_MainWindow;
extern ConfigAccesser g_GlobleConfig;

PaoDianListDialog::PaoDianListDialog(void)
{
	m_Setup.setupUi(this);
	connect(this, SIGNAL(accepted()), SLOT(slot_accepted()));
}


PaoDianListDialog::~PaoDianListDialog(void)
{
}

void PaoDianListDialog::treeItemChanged( QStandardItem * item )
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

void PaoDianListDialog::LoadSlot( QStandardItemModel* pSIM )
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
void PaoDianListDialog::treeItem_checkAllChild(QStandardItem * item, bool check)
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

void PaoDianListDialog::treeItem_checkAllChild_recursion(QStandardItem * item,bool check)
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
void PaoDianListDialog::treeItem_CheckChildChanged(QStandardItem * item)
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
Qt::CheckState PaoDianListDialog::checkSibling(QStandardItem * item)
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

void PaoDianListDialog::slot_accepted()
{
	QVector3D p1, p2, p3, p4;
	m_pPaoDinaControlerInterface->GetPaoDianBoundBox(&p1, &p2, &p3, &p4);

	CubeSettingsDialogImps dsd;
	QString strName = g_MainWindow->GetSelectFileName();
	if(strName.size() == 0)
	{
		strName = "NewTrackBox";
	}
	dsd.SetCubeName(strName);
	dsd.SetMode(0);

	osg::Vec3 v3LeftTop(p4.x(), p4.y(), p4.z());
	osg::Vec3 v3RightBottom(p2.x(), p2.y(), p2.z());

	osg::Vec3 v3LeftTopEarth;
	osg::Vec3 v3RightBottomEarth;

	goto_gis::CoordinateTransform* pCT = g_MainWindow->GetMap()->GetCoordinateTransform();
	goto_gis::Proj4Transform proj4;
	proj4.setSourceCRS(pCT->getDesCRS());
	proj4.setDestCRS(pCT->getSourceCRS());
	v3LeftTopEarth = proj4.Transform(v3LeftTop);
	v3RightBottomEarth = proj4.Transform(v3RightBottom);

	dsd.m_Setup.LeftTopLonlineEdit->setText(QString("%1").arg(v3LeftTopEarth.x()));
	dsd.m_Setup.LeftTopLatlineEdit->setText(QString("%1").arg(v3LeftTopEarth.y()));
	dsd.m_Setup.RightBottonLonlineEdit->setText(QString("%1").arg(v3RightBottomEarth.x()));
	dsd.m_Setup.RightBottonLatlineEdit->setText(QString("%1").arg(v3RightBottomEarth.y()));
	dsd.exec();

	g_MainWindow->FlushCubesList();
	g_GlobleConfig.Write();
}
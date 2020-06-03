#include "StdAfx.h"
#include "CubeSettingsDialogImps.h"

extern ConfigAccesser g_GlobleConfig;

CubeSettingsDialogImps::CubeSettingsDialogImps(void)
	: m_iMode(-1), m_CheckGroup(new QButtonGroup)
{
	m_Setup.setupUi(this);
	connect(m_Setup.pushButton, SIGNAL(clicked(bool)), SLOT(slot_AddValueButton(bool)));
	connect(m_Setup.pushButton_2, SIGNAL(clicked(bool)), SLOT(slot_DelValueButton(bool)));
	m_Setup.buttonBox->button(QDialogButtonBox::Ok)->setText(QString::fromLocal8Bit("确定"));
	m_Setup.buttonBox->button(QDialogButtonBox::Cancel)->setText(QString::fromLocal8Bit("取消"));
	QStringList items;
	items.push_back("500");
	items.push_back("1500");
	items.push_back("2500");
	items.push_back("3500");
	items.push_back("4500");
	items.push_back("5500");
	items.push_back("6500");
	items.push_back("7500");
	items.push_back("8500");
	items.push_back("9500");
	items.push_back("10500");
	items.push_back("11500");
	items.push_back("12500");
	items.push_back("13500");
	items.push_back("14500");
	items.push_back("15500");
	items.push_back("16500");
	items.push_back("17500");
	items.push_back("18500");
	items.push_back("19500");

	m_Setup.HeightcomboBox->addItems(items);
	m_Setup.HeightUpComboBox->addItems(items);
	m_Setup.HeightDownComboBox->addItems(items);

	m_CheckGroup->setExclusive(false);
	m_CheckGroup->addButton(m_Setup.checkBox_1, 0);
	m_CheckGroup->addButton(m_Setup.checkBox_2, 1);
	m_CheckGroup->addButton(m_Setup.checkBox_3, 2);
	m_CheckGroup->addButton(m_Setup.checkBox_4, 3);
	m_CheckGroup->addButton(m_Setup.checkBox_5, 4);
	m_CheckGroup->addButton(m_Setup.checkBox_6, 5);
	m_CheckGroup->addButton(m_Setup.checkBox_7, 6);
	m_CheckGroup->addButton(m_Setup.checkBox_8, 7);
	m_CheckGroup->addButton(m_Setup.checkBox_9, 8);
	m_CheckGroup->addButton(m_Setup.checkBox_10, 9);
	m_CheckGroup->addButton(m_Setup.checkBox_11, 10);
	m_CheckGroup->addButton(m_Setup.checkBox_12, 11);
	m_CheckGroup->addButton(m_Setup.checkBox_13, 12);
	m_CheckGroup->addButton(m_Setup.checkBox_14, 13);
	m_CheckGroup->addButton(m_Setup.checkBox_15, 14);
	m_CheckGroup->addButton(m_Setup.checkBox_16, 15);

	items.clear();
	items.append("-5");
	items.append("0");
	items.append("5");
	items.append("10");
	items.append("15");
	items.append("20");
	items.append("25");
	items.append("30");
	items.append("35");
	items.append("40");
	items.append("45");
	items.append("50");
	items.append("55");
	items.append("60");
	items.append("65");
	items.append("75");

	m_Setup.comboBox->setEditable(true);
	m_Setup.comboBox->addItems(items);

	m_Setup.comboBox_2->setEditable(true);
	m_Setup.comboBox_2->addItems(items);
}


CubeSettingsDialogImps::~CubeSettingsDialogImps(void)
{
}

void CubeSettingsDialogImps::accept()
{
	cube_data checkCD = g_GlobleConfig.GetCubeFromName(m_Setup.NamelineEdit->text());
	if(m_Setup.NamelineEdit->text().size() <= 0)
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("错误!")
			, QString::fromLocal8Bit("名称不能为空。"));
		return;
	}

	if (checkCD.name.size() > 0)
	{
		QMessageBox::StandardButton iResult = QMessageBox::question(this, QString::fromLocal8Bit("抉择!")
			, QString::fromLocal8Bit("名称为\"") + checkCD.name 
			+  QString::fromLocal8Bit("\"的跟踪框已经存在，是否要修改成当前参数？"));

		if(iResult == QMessageBox::Ok)
		{
			m_iMode = 1;
		}
		
	}

	if(m_iMode != -1)
	{
		cube_data cd;
		cd.height = m_Setup.HeightDownComboBox->currentText();
		cd.height_down = m_Setup.HeightDownComboBox->currentText();
		cd.height_up = m_Setup.HeightUpComboBox->currentText();
		cd.left_top_lat = m_Setup.LeftTopLatlineEdit->text().toFloat();
		cd.left_top_lon = m_Setup.LeftTopLonlineEdit->text().toFloat();
		cd.name = m_Setup.NamelineEdit->text();
		cd.right_bottom_lat = m_Setup.RightBottonLatlineEdit->text().toFloat();
		cd.right_bottom_lon = m_Setup.RightBottonLonlineEdit->text().toFloat();
		cd.used = 1;

		cd.volumeMax = m_Setup.comboBox_2->currentText().toInt();
		cd.heightMax = m_Setup.comboBox->currentText().toInt();
		int iCurTabNum = m_Setup.tabWidget->currentIndex();
		cd.listNum = iCurTabNum;
		for(int i = 0; i < 16; i++)
		{
			QCheckBox* pCheck = dynamic_cast<QCheckBox*>(m_CheckGroup->button(i));

			if(pCheck)
			{
				bool cs = pCheck->isChecked();
				
				if(cs)
				{
					cd.displays.push_back(1);
				}
				else
				{
					cd.displays.push_back(0);
				}
			}
			
		}
		
		for(int i = 0; i < m_Setup.listWidget->count(); i++)
		{
			QListWidgetItem* pItem = m_Setup.listWidget->item(i);
			QString itemText = pItem->text();
			QStringList vs = itemText.split("-");
			if(vs.size() == 2)
			{
				cd.valueList.push_back(vs[0]);
				cd.valueList.push_back(vs[1]);
			}
		}
		g_GlobleConfig.ConfigCubes(m_iMode, m_Setup.NamelineEdit->text(), &cd);
	}

	QDialog::accept();
}

void CubeSettingsDialogImps::slot_buttonClicked( int index )
{
	
}

void CubeSettingsDialogImps::SetValue( const cube_data& cd )
{
	m_Setup.NamelineEdit->setText(cd.name);
	m_Setup.LeftTopLonlineEdit->setText(QString("%1").arg(cd.left_top_lon));
	m_Setup.LeftTopLatlineEdit->setText(QString("%1").arg(cd.left_top_lat));
	m_Setup.RightBottonLonlineEdit->setText(QString("%1").arg(cd.right_bottom_lon));
	m_Setup.RightBottonLatlineEdit->setText(QString("%1").arg(cd.right_bottom_lat));
	int iIndex = m_Setup.HeightcomboBox->findText(cd.height);
	m_Setup.HeightcomboBox->setCurrentIndex(iIndex);

	iIndex = m_Setup.HeightDownComboBox->findText(cd.height_down);
	m_Setup.HeightDownComboBox->setCurrentIndex(iIndex);

	iIndex = m_Setup.HeightUpComboBox->findText(cd.height_up);
	m_Setup.HeightUpComboBox->setCurrentIndex(iIndex);

	m_Setup.comboBox->setEditText(QString("%1").arg(cd.volumeMax));
	m_Setup.comboBox_2->setEditText(QString("%1").arg(cd.heightMax));

	for(int i = 0; i < 16; i++)
	{
		QCheckBox* pCheck = dynamic_cast<QCheckBox*>(m_CheckGroup->button(i));

		int iValue = cd.displays.at(i);

		if(iValue == 0)
		{
			pCheck->setCheckState(Qt::Unchecked);
		}
		else if(iValue > 0)
		{
			pCheck->setCheckState(Qt::Checked);
		}
		
	}

	for(int i = 0; i < cd.valueList.size(); i+=2)
	{
		m_Setup.listWidget->addItem(cd.valueList.at(i) + "-" + cd.valueList.at(i + 1));
	}

	m_Setup.tabWidget->setCurrentIndex(cd.listNum);
}

void CubeSettingsDialogImps::SetCubeName( const QString& name )
{
	static int i = 0;
	static char extname[] = 
	{
		'a','b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
	};
	m_Setup.NamelineEdit->setText(name + "-" + extname[i]);
	if(i == 25)
	{
		i = 0;
	}
	else
	{
		i++;
	}
}

void CubeSettingsDialogImps::slot_AddValueButton( bool )
{
	int fV1 = m_Setup.lineEdit->text().toInt();
	int fV2 = m_Setup.lineEdit_2->text().toInt();
	if(fV1 < -5)
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("输入错误"), QString::fromLocal8Bit("当左边的值小于-5时，没有意义。"));
		return;
	}
	if(fV2 > 70)
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("输入错误"), QString::fromLocal8Bit("当右边的值大于70时，没有意义。"));
		return;
	}
	if (fV1 > fV2)
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("输入错误"), QString::fromLocal8Bit("当左边的值大于右边的值时，没有意义。"));
		return;
	}

	m_Setup.listWidget->addItem(QString("%1-%2").arg(fV1).arg(fV2));
}

void CubeSettingsDialogImps::slot_DelValueButton( bool )
{
	QList<QListWidgetItem*> sl = m_Setup.listWidget->selectedItems();

	for(int i = 0; i < sl.size(); i++)
	{
		QListWidgetItem* pItem = sl.at(i);
		m_Setup.listWidget->removeItemWidget(pItem);
		delete pItem;
	}
}

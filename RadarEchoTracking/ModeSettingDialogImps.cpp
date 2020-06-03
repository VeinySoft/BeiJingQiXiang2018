#include "StdAfx.h"
#include "ModeSettingDialogImps.h"
#include "CubeSettingsDialogImps.h"

extern ConfigAccesser g_GlobleConfig;

ModeSettingDialogImps::ModeSettingDialogImps(void) 
	: m_pModel(new QStandardItemModel)
	, m_RaidoGroup(new QButtonGroup)
	, m_CheckGroup(new QButtonGroup)
{
	m_Setup.setupUi(this);

	m_Setup.buttonBox->button(QDialogButtonBox::Ok)->setText(QString::fromLocal8Bit("确定"));
	m_Setup.buttonBox->button(QDialogButtonBox::Cancel)->setText(QString::fromLocal8Bit("取消"));
	m_Setup.groupBox_3->setVisible(false);

	m_Setup.ApplaypushButton->setVisible(false);
	//m_Setup.listView->setViewMode(QListView::ViewMode::IconMode);
	m_Setup.PathLineEdit->setText(g_GlobleConfig.Path());
	m_Setup.PathLineEdit_2->setText(g_GlobleConfig.HistoryPath());
	m_Setup.PathLineEdit_3->setText(g_GlobleConfig.TrackResultSavePath());
	QDateTime dt = QDateTime::fromString(g_GlobleConfig.StartTime(), "yyyy-MM-dd hh:mm:ss");
	m_Setup.dateTimeEdit->setDateTime(dt);
	//m_Setup.dateTimeEdit_2 = QDateTime::fromString(ca.StartTime());
	
	m_Setup.listView->setModel(m_pModel);
	m_Setup.dateTimeEdit_2->setDateTime(QDateTime::fromString(g_GlobleConfig.EndTime(), "yyyy-MM-dd hh:mm:ss"));
	m_RaidoGroup->addButton(m_Setup.radioButton, 0);
	m_RaidoGroup->addButton(m_Setup.radioButton_2, 1);

	RefillList();

	if(g_GlobleConfig.ModeType() == 0)
	{
		m_Setup.radioButton->setChecked(true);
	}
	else
		m_Setup.radioButton_2->setChecked(true);

	//ReadRitio();

	connect(m_Setup.pushButton, SIGNAL(clicked(bool)), SLOT(slot_AddCube(bool)));
	connect(m_Setup.pushButton_2, SIGNAL(clicked(bool)), SLOT(slot_ModifyCube(bool)));
	connect(m_Setup.pushButton_3, SIGNAL(clicked(bool)), SLOT(slot_DeleteCube(bool)));
	connect(m_Setup.SetPathPushButton, SIGNAL(clicked(bool)), SLOT(slot_setPathButton(bool)));
	connect(m_Setup.SetPathPushButton_2, SIGNAL(clicked(bool)), SLOT(slot_setPathButton2(bool)));
	connect(m_Setup.SetPathPushButton_3, SIGNAL(clicked(bool)), SLOT(slot_setPathButton3(bool)));
	connect(m_Setup.ApplaypushButton, SIGNAL(clicked(bool)), SLOT(slot_Apply(bool)));
	connect(m_Setup.StartFilePushButton, SIGNAL(clicked(bool)), SLOT(slot_FirstTime(bool)));
	connect(m_Setup.EndFilePushButton, SIGNAL(clicked(bool)), SLOT(slot_LastTime(bool)));
	connect(m_Setup.comboBox, SIGNAL(activated(int)), SLOT(slot_activated(int)));

	connect(m_CheckGroup, SIGNAL(buttonClicked(int)), SLOT(slot_buttonClicked(int)));
	//connect(m_Setup.pushButton_4, SIGNAL(clicked(bool)), SLOT(slot_addToList(bool)));
	//connect(m_Setup.pushButton_5, SIGNAL(clicked(bool)), SLOT(slot_RemoveFromList(bool)));
}


ModeSettingDialogImps::~ModeSettingDialogImps(void)
{
}

void ModeSettingDialogImps::slot_AddCube(bool)
{
	CubeSettingsDialogImps d;
	d.SetMode(0);

	if(d.exec() == QDialog::Accepted)
	{
		RefillList();
	}
}

void ModeSettingDialogImps::slot_ModifyCube(bool)
{
	CubeSettingsDialogImps d;
	d.SetMode(1);

	cube_data cd = g_GlobleConfig.GetCubeFromName(m_Setup.comboBox->currentText());

	d.SetValue(cd);
	//cd.height = d.m_Setup.HeightcomboBox->setCurrentText().toInt();
	//cd.used = 1;
	d.exec();
}

void ModeSettingDialogImps::slot_DeleteCube(bool)
{
	g_GlobleConfig.ConfigCubes(2, m_Setup.comboBox->currentText(), 0);
	RefillList();
}

void ModeSettingDialogImps::slot_Apply(bool)
{

}

void ModeSettingDialogImps::slot_addToList( bool )
{

}

void ModeSettingDialogImps::slot_RemoveFromList( bool )
{

}

void ModeSettingDialogImps::accept()
{
	/*QMessageBox::StandardButton r = (QMessageBox::StandardButton)QMessageBox::question(this
		, QString::fromLocal8Bit("保存配置")
		, QString::fromLocal8Bit("确定要保存新的配置信息吗？")
		, QMessageBox::Yes
		, QMessageBox::No);

	if(r == QMessageBox::No) 
		return;*/

	g_GlobleConfig.Path(m_Setup.PathLineEdit->text());
	g_GlobleConfig.HistoryPath(m_Setup.PathLineEdit_2->text());
	g_GlobleConfig.TrackResultSavePath(m_Setup.PathLineEdit_3->text());
	QString strStratTime = m_Setup.dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
	g_GlobleConfig.StartTime(strStratTime);
	
	g_GlobleConfig.EndTime(m_Setup.dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss"));

	//g_GlobleConfig.Displays()
	g_GlobleConfig.ModeType(m_RaidoGroup->checkedId());
	//writeRitio();

	g_GlobleConfig.Write();

	QDialog::accept();
}

void ModeSettingDialogImps::RefillList()
{
	m_Setup.comboBox->clear();
	m_pModel->removeRows(0, m_pModel->rowCount());

	QVector<cube_data> cubes;

	g_GlobleConfig.GetCubes(cubes);

	for(int i = 0; i < cubes.size(); i++)
	{
		m_Setup.comboBox->addItem(cubes.at(i).name);

		if(cubes.at(i).used == 1)
		{
			QStandardItem* ci = new QStandardItem(QIcon("./icon/cube.png")
				, cubes.at(i).name 
				+ "  "
				+ QString("%1").arg(cubes.at(i).left_top_lon)
				+ ", "
				+ QString("%1").arg(cubes.at(i).left_top_lat)
				+ "<->"
				+ QString("%1").arg(cubes.at(i).right_bottom_lon)
				+ ", "
				+ QString("%1").arg(cubes.at(i).right_bottom_lat)
				+ " ["
				+ cubes.at(i).height_down
				+ " < "
				+ cubes.at(i).height
				+ " < "
				+ cubes.at(i).height_up
				+ "]");
			m_pModel->appendRow(ci);
		}
	}
}

void ModeSettingDialogImps::ReadRitio(const cube_data& cd)
{
	m_Setup.checkBox_1->setChecked(cd.displays[0]);
	m_Setup.checkBox_2->setChecked(cd.displays[1]);
	m_Setup.checkBox_3->setChecked(cd.displays[2]);
	m_Setup.checkBox_4->setChecked(cd.displays[3]);
	m_Setup.checkBox_5->setChecked(cd.displays[4]);
	m_Setup.checkBox_6->setChecked(cd.displays[5]);
	m_Setup.checkBox_7->setChecked(cd.displays[6]);
	m_Setup.checkBox_8->setChecked(cd.displays[7]);
	m_Setup.checkBox_9->setChecked(cd.displays[8]);
	m_Setup.checkBox_10->setChecked(cd.displays[9]);
	m_Setup.checkBox_11->setChecked(cd.displays[10]);
	m_Setup.checkBox_12->setChecked(cd.displays[11]);
	m_Setup.checkBox_13->setChecked(cd.displays[12]);
	m_Setup.checkBox_14->setChecked(cd.displays[13]);
	m_Setup.checkBox_15->setChecked(cd.displays[14]);
	m_Setup.checkBox_16->setChecked(cd.displays[15]);
}

void ModeSettingDialogImps::writeRitio(cube_data& cd)
{
	cd.displays[0] = (m_Setup.checkBox_1->checkState());
	cd.displays[1] = (m_Setup.checkBox_2->checkState());
	cd.displays[2] = (m_Setup.checkBox_3->checkState());
	cd.displays[3] = (m_Setup.checkBox_4->checkState());
	cd.displays[4] = (m_Setup.checkBox_5->checkState());
	cd.displays[5] = (m_Setup.checkBox_6->checkState());
	cd.displays[6] = (m_Setup.checkBox_7->checkState());
	cd.displays[7] = (m_Setup.checkBox_8->checkState());
	cd.displays[8] = (m_Setup.checkBox_9->checkState());
	cd.displays[9] = (m_Setup.checkBox_10->checkState());
	cd.displays[10] = (m_Setup.checkBox_11->checkState());
	cd.displays[11] = (m_Setup.checkBox_12->checkState());
	cd.displays[12] = (m_Setup.checkBox_13->checkState());
	cd.displays[13] = (m_Setup.checkBox_14->checkState());
	cd.displays[14] = (m_Setup.checkBox_15->checkState());
	cd.displays[15] = (m_Setup.checkBox_16->checkState());
}

void ModeSettingDialogImps::GetTimeString(const QString& str1, QString& strTimeString)
{
	QString strBaseName = str1.left(15);
	strTimeString = strBaseName.remove("_");
}

void ModeSettingDialogImps::slot_FirstTime( bool )
{
	QString strPath = QFileDialog::getOpenFileName(this);
	if(strPath.size() != 0)
	{
		QFileInfo info(strPath);
		QString strTemp = info.baseName();
		GetTimeString(strTemp, strTemp);
		QDateTime currDateTime = QDateTime::fromString(/*"20" + */strTemp, "yyyyMMddhhmmss");

		m_Setup.dateTimeEdit->setDateTime(currDateTime);
	}
}

void ModeSettingDialogImps::slot_LastTime( bool )
{
	QString strPath = QFileDialog::getOpenFileName(this);
	if(strPath.size() != 0)
	{
		QFileInfo info(strPath);
		QString strTemp = info.baseName();
		GetTimeString(strTemp, strTemp);
		QDateTime currDateTime = QDateTime::fromString(/*"20" + */strTemp, "yyyyMMddhhmmss");

		m_Setup.dateTimeEdit_2->setDateTime(currDateTime);
	}
}

void ModeSettingDialogImps::slot_activated( int index )
{
	QString strText = m_Setup.comboBox->currentText();
	cube_data cd = g_GlobleConfig.GetCubeFromName(strText);

	ReadRitio(cd);
}

void ModeSettingDialogImps::slot_buttonClicked( int index )
{
	QCheckBox* pCheck = dynamic_cast<QCheckBox*>(m_CheckGroup->button(index));

	if(pCheck)
	{
		Qt::CheckState cs = pCheck->checkState();
		QString strName = m_Setup.comboBox->currentText();

		if(cs == Qt::Checked)
		{
			//g_GlobleConfig.
		}
		else if(cs == Qt::Unchecked)
		{

		}
		else
		{

		}
	}
}

void ModeSettingDialogImps::slot_setPathButton( bool )
{
	QString strPath = QFileDialog::getExistingDirectory(this, "Radar File Path", m_Setup.PathLineEdit->text());
	if(strPath.size() != 0)
	{
		m_Setup.PathLineEdit->setText(strPath);
	}
}

void ModeSettingDialogImps::slot_setPathButton2( bool )
{
	QString strPath = QFileDialog::getExistingDirectory(this, "Radar File Path", m_Setup.PathLineEdit->text());
	if(strPath.size() != 0)
	{
		m_Setup.PathLineEdit_2->setText(strPath);
		QStringList fileFilter;
		fileFilter.append("*.nc");
		QDir dir(strPath);
		QFileInfoList fileList = dir.entryInfoList(fileFilter, QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::Time);

		if(fileList.size() >= 2)
		{
			QString strFileName1 = fileList.at(0).baseName();
			QString strFileName2 = fileList.at(fileList.size() - 1).baseName();

			GetTimeString(strFileName1, strFileName1);
			GetTimeString(strFileName2, strFileName2);

			QDateTime StartDateTime = QDateTime::fromString(/*"20" + */strFileName2, "yyyyMMddhhmmss");
			QDateTime EndDateTime = QDateTime::fromString(/*"20" + */strFileName1, "yyyyMMddhhmmss");

			m_Setup.dateTimeEdit->setDateTime(StartDateTime);
			m_Setup.dateTimeEdit_2->setDateTime(EndDateTime);
		}
	}
}

void ModeSettingDialogImps::slot_setPathButton3( bool )
{
	QString strPath = QFileDialog::getExistingDirectory(this, "Radar File Path", m_Setup.PathLineEdit_3->text());
	if(strPath.size() != 0)
	{
		m_Setup.PathLineEdit_3->setText(strPath);
	}
}

#include "StdAfx.h"
#include "NcFileLayerDockCheckBox.h"


NcFileLayerDockCheckBox::NcFileLayerDockCheckBox(void) : m_pButtonGroup(new QButtonGroup(this))
{
	m_Setup.setupUi(this);

	m_pButtonGroup->addButton(m_Setup.checkBox, 0);
	m_pButtonGroup->addButton(m_Setup.checkBox_2, 1);
	m_pButtonGroup->addButton(m_Setup.checkBox_3, 2);
	m_pButtonGroup->addButton(m_Setup.checkBox_4, 3);
	m_pButtonGroup->addButton(m_Setup.checkBox_5, 4);
	m_pButtonGroup->addButton(m_Setup.checkBox_6, 5);
	m_pButtonGroup->addButton(m_Setup.checkBox_7, 6);
	m_pButtonGroup->addButton(m_Setup.checkBox_8, 7);
	m_pButtonGroup->addButton(m_Setup.checkBox_9, 8);
	m_pButtonGroup->addButton(m_Setup.checkBox_10,9);
	m_pButtonGroup->addButton(m_Setup.checkBox_11, 10);
	m_pButtonGroup->addButton(m_Setup.checkBox_12, 11);
	m_pButtonGroup->addButton(m_Setup.checkBox_13, 12);
	m_pButtonGroup->addButton(m_Setup.checkBox_14, 13);
	m_pButtonGroup->addButton(m_Setup.checkBox_15, 14);
	m_pButtonGroup->addButton(m_Setup.checkBox_16, 15);
	m_pButtonGroup->addButton(m_Setup.checkBox_17, 16);
	m_pButtonGroup->addButton(m_Setup.checkBox_18, 17);
	m_pButtonGroup->addButton(m_Setup.checkBox_19, 18);
	m_pButtonGroup->addButton(m_Setup.checkBox_20, 19);
	m_pButtonGroup->addButton(m_Setup.checkBox_21, 20);

	this->setWindowTitle(QString::fromLocal8Bit("·Ö²ãÏÔÊ¾"));
	m_Setup.checkBox_21->setCheckState(Qt::Checked);
}


NcFileLayerDockCheckBox::~NcFileLayerDockCheckBox(void)
{
}

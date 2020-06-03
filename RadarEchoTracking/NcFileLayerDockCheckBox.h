#pragma once
#include "ui_LayerDaockWidget.h"
class NcFileLayerDockCheckBox :
	public QDockWidget
{
public:
	NcFileLayerDockCheckBox(void);
	~NcFileLayerDockCheckBox(void);
	Ui::LayerDockWidget m_Setup;

	inline QButtonGroup* GetCheckBoxGroup(){return m_pButtonGroup;}

private:
	QButtonGroup* m_pButtonGroup;
};


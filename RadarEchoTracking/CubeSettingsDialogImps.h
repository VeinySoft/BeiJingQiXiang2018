#pragma once
#include "ui_CubeSettings.h"
#include "ConfigAccesser.h"

class CubeSettingsDialogImps :
	public QDialog
{
	Q_OBJECT
public:
	CubeSettingsDialogImps(void);
	~CubeSettingsDialogImps(void);
	Ui::CubeSettingsDialog m_Setup;
	virtual void accept();
	inline void SetMode(int iMode){m_iMode = iMode;}
	void SetValue(const cube_data& cd);
	void SetCubeName(const QString& name);
public slots:
	void slot_buttonClicked(int index);
	void slot_AddValueButton(bool);
	void slot_DelValueButton(bool);
private:
	int m_iMode;
	QPointer<QButtonGroup> m_CheckGroup;
};


#pragma once
#include "ui_ModeSettings.h"
#include "ConfigAccesser.h"

class ModeSettingDialogImps :
	public QDialog
{
	Q_OBJECT
public:
	ModeSettingDialogImps(void);
	~ModeSettingDialogImps(void);
	Ui::ModeSettingDialog m_Setup;
public slots:
	void slot_AddCube(bool);
	void slot_ModifyCube(bool);
	void slot_DeleteCube(bool);
	void slot_Apply(bool);
	void slot_addToList(bool);
	void slot_setPathButton(bool);
	void slot_setPathButton2(bool);
	void slot_setPathButton3(bool);
	void slot_RemoveFromList(bool);
	virtual void accept();
	void slot_FirstTime(bool);
	void slot_LastTime(bool);
	void slot_activated(int index);
	void slot_buttonClicked(int index);
private:
	void RefillList();
	void ReadRitio(const cube_data& cd);
	void writeRitio(cube_data& cd);
	void GetTimeString(const QString& str1, QString& strTimeString);
	QPointer<QStandardItemModel> m_pModel;
	QPointer<QButtonGroup> m_RaidoGroup;
	QPointer<QButtonGroup> m_CheckGroup;
};


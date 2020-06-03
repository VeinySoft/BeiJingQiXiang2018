#pragma once
#include <QString>
#include <QVector>
#include <QStandardItemModel>

struct NodeInfo
{
	int level;
	int id;
	char move_type;
	float lon;
	float lat;
	QString cannon_type;
	float start_engle;
	float end_engle;
	QString name;
};

class PaoDinaControlerInterface
{
public:
	virtual NodeInfo* GetNodeFromID(int id) = 0;
	virtual void DisplayFromIDs(int id) = 0;
	virtual void HideFromIDs(int id) = 0;
	virtual QStandardItemModel* GetModel() = 0;
	virtual void HideAll() = 0;
	virtual void DisplayAll() = 0;
	virtual void GetPaoDianBoundBox(QVector3D* p1, QVector3D* p2, QVector3D* p3, QVector3D* p4) = 0;
};


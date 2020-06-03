#pragma once
#include "paodinacontrolerinterface.h"
#include <QMap>
#include <QVector3D>
#include "PaoDianGeometry.h"

class PaoDianControler :
	public PaoDinaControlerInterface
{
public:
	PaoDianControler(void);
	~PaoDianControler(void);

	virtual NodeInfo* GetNodeFromID( int id );

	virtual void DisplayFromIDs( int id );

	virtual void HideFromIDs( int id );

	virtual QStandardItemModel* GetModel();
	virtual void HideAll();
	virtual void DisplayAll();
	virtual void GetPaoDianBoundBox(QVector3D* p1, QVector3D* p2, QVector3D* p3, QVector3D* p4);
	QStandardItemModel m_StandardItemModel;
	QMap<int, NodeInfo> m_AllNode;
	PaoDianGeometry* m_pPaoDianGeometry;

};


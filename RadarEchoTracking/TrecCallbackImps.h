#pragma once
#include "..\netcdfdataprovider2\controlorinterface.h"
class TrecCallbackImps : public QObject,
	public TrecCallback
{
	Q_OBJECT
public:
	TrecCallbackImps();
	~TrecCallbackImps(void);

	virtual void TrecDone( QStringList& fileList, QString& strCubeName, P_TREC_NC_MAP_OUT pOutMap );

signals:
	void signal_TrecDone( QStringList& fileList, QString& strCubeName, P_TREC_NC_MAP_OUT pOutMap );
};


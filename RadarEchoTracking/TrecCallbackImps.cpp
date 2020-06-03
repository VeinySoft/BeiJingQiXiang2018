#include "StdAfx.h"
#include "TrecCallbackImps.h"

TrecCallbackImps::TrecCallbackImps()
{
}


TrecCallbackImps::~TrecCallbackImps(void)
{
}

void TrecCallbackImps::TrecDone( QStringList& fileList, QString& strCubeName, P_TREC_NC_MAP_OUT pOutMap )
{
	emit signal_TrecDone(fileList, strCubeName, pOutMap);
}
#pragma once
#include "ConfigAccesser.h"

class GetTrackBoxListInterface
{
public:
	virtual void GetSelectTrackBoxList(QVector<cube_data>& boxList) = 0;
};

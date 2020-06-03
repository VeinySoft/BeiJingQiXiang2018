#pragma once
#include "myactioninterface.h"
class TrackBoxSettingAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(TrackBoxSettingAction)
	virtual void MyAction();

	virtual void InitMyAction();

};


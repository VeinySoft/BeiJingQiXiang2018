#pragma once
#include "myactioninterface.h"
class StartTrackingAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(StartTrackingAction)

	virtual void MyAction();

	virtual void InitMyAction();
};


#pragma once
#include "myactioninterface.h"
class StopTrackingAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(StopTrackingAction)

	virtual void MyAction();

	virtual void InitMyAction();

};


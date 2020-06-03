#pragma once
#include "myactioninterface.h"
class RestorePointerAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(RestorePointerAction)

	virtual void MyAction();

	virtual void InitMyAction();

};


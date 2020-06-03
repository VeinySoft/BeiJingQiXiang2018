#pragma once
#include "myactioninterface.h"
class NextFrameAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(NextFrameAction)

	virtual void MyAction();

	virtual void InitMyAction();

};


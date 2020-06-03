#pragma once
#include "myactioninterface.h"
class ScaleToMapAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(ScaleToMapAction)

	virtual void MyAction();

	virtual void InitMyAction();

};


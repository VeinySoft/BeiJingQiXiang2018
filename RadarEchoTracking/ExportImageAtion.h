#pragma once
#include "myactioninterface.h"
class ExportImageAtion :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(ExportImageAtion)
	virtual void MyAction();

	virtual void InitMyAction();

};


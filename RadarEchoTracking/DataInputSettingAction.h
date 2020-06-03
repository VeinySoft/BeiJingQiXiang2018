#pragma once
#include "MyActionInterface.h"

class DataInputSettingAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(DataInputSettingAction)

	virtual void MyAction();

	virtual void InitMyAction();

};


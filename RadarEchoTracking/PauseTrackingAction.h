#pragma once
#include "myactioninterface.h"
class PauseTrackingAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(PauseTrackingAction)

	virtual void MyAction();

	virtual void InitMyAction();
private:
	bool m_bPlay;
};


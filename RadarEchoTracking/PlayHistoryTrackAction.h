#pragma once
#include "myactioninterface.h"
class PlayHistoryTrackAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(PlayHistoryTrackAction)

	virtual void MyAction();

	virtual void InitMyAction();

};


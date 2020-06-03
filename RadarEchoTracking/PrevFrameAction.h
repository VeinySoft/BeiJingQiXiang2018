#pragma once
#include "myactioninterface.h"
class PrevFrameAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(PrevFrameAction)

	virtual void MyAction();

	virtual void InitMyAction();
private:
	bool m_bPlay;
};


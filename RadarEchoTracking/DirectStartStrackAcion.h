#pragma once

class DirectStartStrackAcion :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(DirectStartStrackAcion)

	virtual void MyAction();
	
	virtual void InitMyAction();
	
};


#pragma 
#ifndef __MOVE_TO_RANGE_ACTION__
#define __MOVE_TO_RANGE_ACTION__

#include "ActionNode.h"

class MoveToRangeAction : public ActionNode
{
public:
	MoveToRangeAction();
	virtual ~MoveToRangeAction();

	virtual void Action() override; // Specialization

};
#endif /* defined (__MOVE_TO_RANGE_ACTION__) */

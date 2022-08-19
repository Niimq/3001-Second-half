#pragma once
#ifndef __RANGED_COMBAT_CONDITION__
#define __RANGED_COMBAT_CONDITION__

#include "ConditionNode.h"

class RangedCombatCondition : public ConditionNode
{
public:
	RangedCombatCondition(bool Within_Combat_Range = false);
	virtual ~RangedCombatCondition();

	// Getters and Setters
	void SetIsWithinCombatRange(bool state);

	virtual bool Condition() override;
	
private:
	bool m_isWithinCombatRange;
};

#endif /* Defined __RANGED_COMBAT_CONDITION__ */


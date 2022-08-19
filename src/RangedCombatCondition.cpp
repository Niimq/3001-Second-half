#include "RangedCombatCondition.h"

RangedCombatCondition::RangedCombatCondition(bool Within_Combat_Range)
{
	m_name = "Ranged Combat Condition";
	SetIsWithinCombatRange(Within_Combat_Range);
}

RangedCombatCondition::~RangedCombatCondition()
= default;

void RangedCombatCondition::SetIsWithinCombatRange(bool state)
{
	m_isWithinCombatRange = state;
}

bool RangedCombatCondition::Condition()
{
	return m_isWithinCombatRange;
}

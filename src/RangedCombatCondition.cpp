#include "CloseCombatCondition.h"

CloseCombatCondition::CloseCombatCondition(bool Within_Combat_Range)
{
	m_name = "Close Combat Condition";
	SetIsWithinCombatRange(Within_Combat_Range);
}

CloseCombatCondition::~CloseCombatCondition()
= default;

void CloseCombatCondition::SetIsWithinCombatRange(bool state)
{
	m_isWithinCombatRange = state;
}

bool CloseCombatCondition::Condition()
{
	return m_isWithinCombatRange;
}

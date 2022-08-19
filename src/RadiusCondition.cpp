#include "RadiusCondition.h"

RadiusCondition::RadiusCondition(bool within_radius)
{
	m_name = "Radius Condition";
	SetIsWithingRadius(within_radius);
}

RadiusCondition::~RadiusCondition()
= default;

void RadiusCondition::SetIsWithingRadius(bool state)
{
	m_isWithinRadius = state;
}

bool RadiusCondition::Condition()
{
	return m_isWithinRadius;
}

#include "RangedCombatEnemy.h"
#include "TextureManager.h"
#include "Util.h"
#include "Game.h"
#include "MoveToRangeAction.h"
#include "EventManager.h"

RangedCombatEnemy::RangedCombatEnemy() : m_maxSpeed(20.0f),
	                                   m_turnRate(5.0f), m_accelerationRate(2.0f)
{
	TextureManager::Instance().Load("../Assets/textures/reliant_small.png","Ranged_Enemy");

	const auto size = TextureManager::Instance().GetTextureSize("Ranged_Enemy");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));

	GetRigidBody()->velocity = glm::vec2(0, 0);
	GetRigidBody()->acceleration = glm::vec2(0, 0);
	GetRigidBody()->isColliding = false;

	SetCurrentHeading(0.0f); // current facing angle
	SetCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right
	SetLOSDistance(400.0f);
	SetWhiskerAngle(45.0f);

	SetLOSColour(glm::vec4(1, 0, 0, 1)); // default LOS colour is Red
	
	SetType(GameObjectType::AGENT);

	// New for Lab 7
	SetActionState(ActionState::NO_ACTION);
	BuildPatrolPath();

	// New for Lab 7 part 2
	m_tree = new DecisionTree(this); // Create a new Tree
	m_buildTree();
	m_tree->Display(); // optional
}

RangedCombatEnemy::~RangedCombatEnemy()
= default;

void RangedCombatEnemy::Draw()
{
	// draw the target
	TextureManager::Instance().Draw("Close_Enemy", GetTransform()->position, GetCurrentHeading(), 255, true);

	// draw LOS

	if (EventManager::Instance().IsIMGUIActive())
	{
		Util::DrawLine(GetTransform()->position + GetCurrentDirection() * 0.5f * static_cast<float>(GetWidth()),
			GetMiddleLOSEndPoint(), GetLOSColour());
	}
}

void RangedCombatEnemy::Update()
{
	// Determine which action to perform
	m_tree->MakeDecision();
}

void RangedCombatEnemy::Clean()
{

}

float RangedCombatEnemy::GetMaxSpeed() const
{
	return m_maxSpeed;
}

float RangedCombatEnemy::GetTurnRate() const
{
	return m_turnRate;
}

glm::vec2 RangedCombatEnemy::GetDesiredVelocity() const
{
	return m_desiredVelocity;
}

float RangedCombatEnemy::GetAccelerationRate() const
{
	return m_accelerationRate;
}

void RangedCombatEnemy::SetMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

void RangedCombatEnemy::SetTurnRate(const float angle)
{
	m_turnRate = angle;
}

void RangedCombatEnemy::SetDesiredVelocity(const glm::vec2 target_position)
{
	m_desiredVelocity = Util::Normalize(target_position - GetTransform()->position) * m_maxSpeed;
}

void RangedCombatEnemy::SetAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

void RangedCombatEnemy::Seek()
{
	// Find Next WayPoint if within 10px of the current waypoint
	if (Util::Distance(m_patrolPath[m_wayPoint], GetTransform()->position) < 10)
	{
		// Check to see if you are at the last point in the path
		if (++m_wayPoint == m_patrolPath.size())
		{
			// if so...reset
			m_wayPoint = 0;
		}
		SetTargetPosition(m_patrolPath[m_wayPoint]);
	}

	SetDesiredVelocity(GetTargetPosition());

	const glm::vec2 steering_direction = GetDesiredVelocity() - GetCurrentDirection();

	LookWhereIAmGoing(steering_direction);

	GetRigidBody()->acceleration = GetCurrentDirection() * GetAccelerationRate();
}

void RangedCombatEnemy::LookWhereIAmGoing(const glm::vec2 target_direction)
{
	const auto target_rotation = Util::SignedAngle(GetCurrentDirection(), target_direction);
	if (GetCollisionWhiskers()[0])
	{
		SetCurrentHeading(GetCurrentHeading() + GetTurnRate());
	}
	else if (GetCollisionWhiskers()[2])
	{
		SetCurrentHeading(GetCurrentHeading() - GetTurnRate());
	}
	else if (abs(target_rotation) > m_turnRate)
	{
		SetCurrentHeading(GetCurrentHeading() + GetTurnRate() * Util::Sign(target_rotation));
	}
	UpdateWhiskers(GetWhiskerAngle()); // New for Lab 3.
}

void RangedCombatEnemy::BuildPatrolPath()
{
	m_patrolPath.push_back(glm::vec2(760, 40)); // Top Right Corner node
	m_patrolPath.push_back(glm::vec2(760, 560)); // Bottom Right Corner node
	m_patrolPath.push_back(glm::vec2(40, 560)); // Bottom left Corner node
	m_patrolPath.push_back(glm::vec2(40, 40)); // Top left Corner node
	m_wayPoint = 0;

	SetTargetPosition(m_patrolPath[m_wayPoint]); // Top Right to Start
}

void RangedCombatEnemy::Move()
{
	Seek(); // Get our target for this frame.

	// Kinematic equation: final_position = position + velocity * time + 0.5*acceleration * time*time
	const auto delta_time = Game::Instance().GetDeltaTime();

	// compute the position term
	const glm::vec2 initial_position = GetTransform()->position;

	// compute the velocity term
	const glm::vec2 velocity_term = GetRigidBody()->velocity * delta_time;

	// compute the acceleration term
	const glm::vec2 acceleration_term = GetRigidBody()->acceleration * 0.5f;// *dt;


	// compute the new position
	glm::vec2 final_position = initial_position + velocity_term + acceleration_term;

	GetTransform()->position = final_position;

	// add our acceleration to velocity
	GetRigidBody()->velocity += GetRigidBody()->acceleration;

	// clamp our velocity at max speed
	GetRigidBody()->velocity = Util::Clamp(GetRigidBody()->velocity, GetMaxSpeed());
}

void RangedCombatEnemy::CheckBounds(){}


void RangedCombatEnemy::Reset()
{
	GetTransform()->position = m_startPos;
}

void RangedCombatEnemy::Patrol()
{
	if (GetActionState() != ActionState::PATROL)
	{
		// Initialize
		SetActionState(ActionState::PATROL);
	}
	Move();
}

void RangedCombatEnemy::MoveToPlayer()
{
	if (GetActionState() != ActionState::MOVE_TO_PLAYER)
	{
		// Initialize
		SetActionState(ActionState::MOVE_TO_PLAYER);
	}
}

DecisionTree* RangedCombatEnemy::GetTree() const
{
	return m_tree;
}

void RangedCombatEnemy::m_buildTree()
{//	// Conditions

	// Create and add the root node
	m_tree->SetLOSNode(new LOSCondition());
	m_tree->GetTree().push_back(m_tree->GetLOSNode());

	m_tree->SetRadiusNode(new RadiusCondition());
	m_tree->AddNode(m_tree->GetLOSNode(), m_tree->GetRadiusNode(), TreeNodeType::LEFT_TREE_NODE);
	m_tree->GetTree().push_back(m_tree->GetRadiusNode());

	m_tree->SetRangedCombatNode(new RangedCombatCondition());
	m_tree->AddNode(m_tree->GetLOSNode(), m_tree->GetRangedCombatNode(), TreeNodeType::RIGHT_TREE_NODE);
	m_tree->GetTree().push_back(m_tree->GetRangedCombatNode());

	// Actions

	// Left Sub-Tree
	TreeNode* patrolNode = m_tree->AddNode(m_tree->GetRadiusNode(), new PatrolAction(), TreeNodeType::LEFT_TREE_NODE);
	dynamic_cast<ActionNode*>(patrolNode)->SetAgent(this);
	m_tree->GetTree().push_back(patrolNode);

	TreeNode* MoveToLOSNode = m_tree->AddNode(m_tree->GetRadiusNode(), new MoveToLOSAction(), TreeNodeType::RIGHT_TREE_NODE);
	dynamic_cast<ActionNode*>(MoveToLOSNode)->SetAgent(this);
	m_tree->GetTree().push_back(MoveToLOSNode);

	// Right Sub-Tree
	TreeNode* MoveToPlayerNode = m_tree->AddNode(m_tree->GetRangedCombatNode(), new MoveToRangeAction(), TreeNodeType::LEFT_TREE_NODE);
	dynamic_cast<ActionNode*>(MoveToPlayerNode)->SetAgent(this);
	m_tree->GetTree().push_back(MoveToPlayerNode);

	TreeNode* attackNode = m_tree->AddNode(m_tree->GetRangedCombatNode(), new AttackAction(), TreeNodeType::RIGHT_TREE_NODE);
	dynamic_cast<ActionNode*>(attackNode)->SetAgent(this);
	m_tree->GetTree().push_back(attackNode);
}
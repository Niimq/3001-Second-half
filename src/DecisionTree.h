#pragma once
#ifndef __DECISION_TREE__
#define __DECISION_TREE__

#include "Agent.h"
#include "LOSCondition.h"
#include "CloseCombatCondition.h"
#include "RadiusCondition.h"
#include "TreeNode.h"
#include "TreeNodeType.h"
#include "ActionNode.h"
#include "PatrolAction.h"
#include "MoveToLOSAction.h"
#include "MoveToPlayerAction.h"
#include "AttackAction.h"

#include <vector>
#include <iostream>

class DecisionTree
{
public:
	// Constructor(s)
	DecisionTree(); // default constructor
	DecisionTree(Agent* agent);

	// Destructor
	~DecisionTree();

	// Getters and Setters (Accessors and Mutators)
	[[nodiscard]] Agent* GetAgent() const;
	void SetAgent(Agent* agent);

	[[nodiscard]] LOSCondition* GetLOSNode() const;
	[[nodiscard]] RadiusCondition* GetRadiusNode() const;
	[[nodiscard]] CloseCombatCondition* GetCloseCombatNode() const;
	// RangedCombatCondition* GetRangedCombatNode() const;
	std::vector<TreeNode*>& GetTree();

	// New for Lab 7 part 2
	void SetLOSNode(LOSCondition* node);
	void SetRadiusNode(RadiusCondition* node);
	void SetCloseCombatNode(CloseCombatCondition* node);
	// void SetRangerCombatNode (RangedCombatCondition* node);

	// Public Utility / Convenience Methods
	TreeNode* AddNode(TreeNode* parent, TreeNode* child_node, TreeNodeType type);
	void Display();
	void Update();
	void Clean();

	void MakeDecision() const; // in-order traversal

private:
	// Private Data Members (Fields)
	Agent* m_agent;
	LOSCondition* m_LOSNode;
	RadiusCondition* m_RadiusNode;
	CloseCombatCondition* m_CloseCombatNode;
	// new fr Lab 7 part 2
	// RangedCombatCondition* m_RangedCombatNode;

	// TreeNode List
	std::vector<TreeNode*> m_treeNodeList;

	// Private Methods
	void m_buildTree();
};

#endif /* defined (__DECISION_TREE__) */

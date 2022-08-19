#include "DecisionTree.h"

DecisionTree::DecisionTree()
{
	//m_buildTree();
}

DecisionTree::DecisionTree(Agent* agent)
{
	m_agent = agent;
	//m_buildTree();
}

DecisionTree::~DecisionTree()
= default;

Agent* DecisionTree::GetAgent() const
{
	return m_agent;
}

void DecisionTree::SetAgent(Agent* agent)
{
	m_agent = agent;
}

LOSCondition* DecisionTree::GetLOSNode() const
{
	return m_LOSNode;
}

RadiusCondition* DecisionTree::GetRadiusNode() const
{
	return m_RadiusNode;
}

CloseCombatCondition* DecisionTree::GetCloseCombatNode() const
{
	return m_CloseCombatNode;
}

std::vector<TreeNode*>& DecisionTree::GetTree()
{
	return m_treeNodeList;
}

void DecisionTree::SetLOSNode(LOSCondition* node)
{
	m_LOSNode;
}

void DecisionTree::SetRadiusNode(RadiusCondition* node)
{
	m_RadiusNode = node;
}

void DecisionTree::SetCloseCombatNode(CloseCombatCondition* node)
{
	m_CloseCombatNode = node;
}

TreeNode* DecisionTree::AddNode(TreeNode* parent, TreeNode* child_node, TreeNodeType type)
{
	switch (type)
	{
	case TreeNodeType::LEFT_TREE_NODE:
		parent->m_pLeft = child_node;
		break;
	case TreeNodeType::RIGHT_TREE_NODE:
		parent->m_pRight = child_node;
		break;
	}
	child_node->m_pParent = parent;
	return child_node;
}

void DecisionTree::Display()
{
	std::cout << std::endl;
	std::cout << "Decision Tree Nodes: " << std::endl;
	std::cout << "-----------------------------" << std::endl;
	for (auto node : m_treeNodeList)
	{
		std::cout << node->m_name << std::endl;
	}
	std::cout << "-----------------------------" << std::endl;
}

void DecisionTree::Update()
{
	// Do something in the play scene
}

void DecisionTree::Clean()
{
	for (auto node : m_treeNodeList)
	{
		delete node;
		node = nullptr;
	}
	m_treeNodeList.clear();
	m_treeNodeList.shrink_to_fit();

	m_LOSNode = nullptr;
	m_RadiusNode = nullptr;
	m_CloseCombatNode = nullptr;
}

// Traverse the Tree "in-order"
void DecisionTree::MakeDecision() const
{
	TreeNode* current_node = m_treeNodeList[0]; // root node;

	// Traverse Tree down to through each Decision
	while (!current_node->m_isLeaf)
	{
		current_node = static_cast <ConditionNode*>(current_node)->Condition() ?
			(current_node->m_pRight) : (current_node->m_pLeft);
	}
	// Take Action
	dynamic_cast<ActionNode*>(current_node)->Action();
}

//void DecisionTree::m_buildTree()
//{
//	// Conditions
//
//	// Create and add the root node
//	m_LOSNode = new LOSCondition();
//	m_treeNodeList.push_back(m_LOSNode);
//
//	m_RadiusNode = new RadiusCondition();
//	AddNode(m_LOSNode, m_RadiusNode, TreeNodeType::LEFT_TREE_NODE);
//	m_treeNodeList.push_back(m_RadiusNode);
//
//	m_CloseCombatNode = new CloseCombatCondition();
//	AddNode(m_LOSNode, m_CloseCombatNode, TreeNodeType::RIGHT_TREE_NODE);
//	m_treeNodeList.push_back(m_CloseCombatNode);
//
//	// Actions
//
//	// Left Sub-Tree
//	TreeNode* patrolNode = AddNode(m_RadiusNode, new PatrolAction(), TreeNodeType::LEFT_TREE_NODE);
//	dynamic_cast<ActionNode*>(patrolNode)->SetAgent(m_agent);
//	m_treeNodeList.push_back(patrolNode);
//
//	TreeNode* MoveToLOSNode = AddNode(m_RadiusNode, new MoveToLOSAction(), TreeNodeType::RIGHT_TREE_NODE);
//	dynamic_cast<ActionNode*>(MoveToLOSNode)->SetAgent(m_agent);
//	m_treeNodeList.push_back(MoveToLOSNode);
//
//	// Right Sub-Tree
//	TreeNode* MoveToPlayerNode = AddNode(m_CloseCombatNode, new MoveToPlayerAction(), TreeNodeType::LEFT_TREE_NODE);
//	dynamic_cast<ActionNode*>(MoveToPlayerNode)->SetAgent(m_agent);
//	m_treeNodeList.push_back(MoveToPlayerNode);
//
//	TreeNode* attackNode = AddNode(m_CloseCombatNode, new AttackAction(), TreeNodeType::RIGHT_TREE_NODE);
//	dynamic_cast<ActionNode*>(attackNode)->SetAgent(m_agent);
//	m_treeNodeList.push_back(attackNode);
//}

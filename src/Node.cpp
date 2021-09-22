#include "Node.h"

Node::Node(int x, int y, bool obstacle, bool start, bool target):
	m_x(x),
	m_y(y),
	m_isObstacle(obstacle),
	m_isStart(start),
	m_isTarget(target)
{

}

bool Node::IsObstacle() const
{
	return m_isObstacle;
}

bool Node::IsStart() const
{
	return m_isStart;
}

bool Node::IsTarget() const
{
	return m_isTarget;
}

int Node::GetGCost() const
{
	return m_gCost;
}

int Node::GetHCost() const
{
	return m_hCost;
}

int Node::GetFCost() const
{
	return m_gCost + m_hCost;
}

int Node::GetX() const
{
	return m_x;
}

int Node::GetY() const
{
	return m_y;
}

Node* Node::GetParent() const
{
	return m_parent;
}

void Node::SetCost(int g, int h)
{
	m_gCost = g;
	m_hCost = h;
}

void Node::SetParent(Node* parent)
{
	m_parent = parent;
}

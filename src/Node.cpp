#include "Node.h"
#include <cmath>

Node::Node(bool obstacle, int x, int y, bool isStart, bool isTarget) :
	m_isObstacle(obstacle),
	m_isStart(isStart),
	m_isTarget(isTarget),
	m_x(x),
	m_y(y)
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

void Node::SetCost(int g, int h)
{
	m_gCost = g;
	m_hCost = h;
}

void Node::SetParent(Node* n)
{
	m_parent = n;
}

const Node* Node::GetParent() const
{
	return m_parent;
}

int Node::DistanceToNode(const Node& other) const
{
	return abs(m_x - other.m_x) + abs(m_y - other.m_y);
}
#include "Field.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

Field::Field()
{
	Reset();
}

Field::~Field()
{
	for (int i = 0; i < m_nodes.size(); ++i)
	{
		const std::vector<Node*>& row = m_nodes.at(i);
		for (const Node* node : row)
		{
			delete node;
		}
	}
	m_startNode = nullptr;
	m_targetNode = nullptr;
}

void Field::Reset()
{	
	for (int i = 0; i < m_nodes.size(); ++i)
	{
		std::vector<Node*>& row = m_nodes.at(i);
		for (const Node* node : row)
		{
			delete node;
		}
		row.clear();
	}
	m_nodes.clear();
	m_openNodes.clear();
	m_closedNodes.clear();
	m_shortestPathNodes.clear();
	
	m_startNode = nullptr;
	m_targetNode = nullptr;
	m_completed = false;
	m_totalSteps = 1;
	
	std::ifstream file(INPUT_FILE);
	std::string line;
	bool firstLineLoaded = false;
	int row = 0;
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			if (!firstLineLoaded)
			{
				firstLineLoaded = true;
				std::stringstream ss(line);
				ss >> m_rows;
				ss >> m_columns;
			}
			else
			{
				std::stringstream ss(line);
				char nodeVal;
				std::vector<Node*> nodeRow;
				for (int i = 0; i < m_columns; ++i)
				{
					bool obstacle = false;
					bool isStart = false;
					bool isTarget = false;
					int x = row;
					int y = i;
					ss >> nodeVal;

					switch (nodeVal)
					{
					case '0':
						obstacle = false;
						break;

					case '1':
						obstacle = true;
						break;

					case 'S':
						obstacle = false;
						isStart = true;
						break;

					case 'T':
						obstacle = false;
						isTarget = true;
						break;

					default:
						throw std::string("Unknown character while parsing input file");
						break;
					}

					Node* n = new Node(obstacle, x, y, isStart, isTarget);
					nodeRow.push_back(n);

					if (nodeVal == 'S')
					{
						m_startNode = n;
					}

					if (nodeVal == 'T')
					{
						m_targetNode = n;
					}
				}
				m_nodes[row] = nodeRow;
				++row;
			}
		}
		std::cout << "Loaded " << m_rows << " rows and " << m_columns << " columns" << std::endl;
	}
	else
	{
		throw std::string("Unable to open input file");
	}

	m_openNodes.push_back(m_startNode);
}

void Field::Print()
{
	for (int i = 0; i < m_nodes.size(); ++i)
	{
		const std::vector<Node*>& row = m_nodes.at(i);
		for (const Node* node : row)
		{
			bool obstacle = node->IsObstacle();

			char val = 'X';

			if (node == m_startNode)
			{
				val = 'S';
			}
			else if (node == m_targetNode)
			{
				val = 'T';
			}
			else
			{
				val = (obstacle) ? '1' : '0';
			}

			std::cout << val << " ";
		}
		std::cout << std::endl;
	}
}

const FieldType& Field::GetField() const
{
	return m_nodes;
}

bool Field::IsCompleted() const
{
	return m_completed;
}

int Field::GetSteps() const
{
	return m_totalSteps;
}

int Field::GetRows() const
{
	return m_rows;
}

int Field::GetColumns() const
{
	return m_columns;
}

const std::vector<Node*>& Field::GetOpenNodes()
{
	return m_openNodes;
}

const std::vector<Node*>& Field::GetClosedNodes()
{
	return m_closedNodes;
}

const std::vector<const Node*>& Field::GetPathNodes()
{
	return m_shortestPathNodes;
}

void Field::Step()
{
	if (m_completed || m_openNodes.size() < 1)
	{
		return;
	}

	Node* current = m_openNodes.at(0);

	for (Node* node : m_openNodes)
	{
		if (*node < *current)
		{
			current = node;
		}
	}

	m_openNodes.erase(std::remove(m_openNodes.begin(), m_openNodes.end(), current), m_openNodes.end());
	m_closedNodes.push_back(current);

	if (current == m_targetNode)
	{
		m_completed = true;
		OnCompleted();
		return;
	}

	std::vector<Node*> neighbourNodes;

	const int currentX = current->GetX();
	const int currentY = current->GetY();

	// right neighbour
	if (currentY < m_columns - 1)
	{
		neighbourNodes.push_back(m_nodes[currentX].at(currentY + 1));
	}

	// left neighbour
	if (currentY > 0)
	{
		neighbourNodes.push_back(m_nodes[currentX].at(currentY - 1));
	}

	// top neighbour
	if (currentX > 1)
	{
		neighbourNodes.push_back(m_nodes[currentX - 1].at(currentY));
	}

	// bottom neighbour
	if (currentX < m_rows - 1)
	{
		neighbourNodes.push_back(m_nodes[currentX + 1].at(currentY));
	}

	for (Node* node : neighbourNodes)
	{
		if (node->IsObstacle() || std::count(m_closedNodes.begin(), m_closedNodes.end(), node))
		{
			continue;
		}

		bool neighbourInOpen = std::count(m_openNodes.begin(), m_openNodes.end(), node);
		int newCost = current->GetGCost() + current->DistanceToNode(*node);
		if (newCost < node->GetGCost() || !neighbourInOpen)
		{
			node->SetCost(newCost, node->DistanceToNode(*m_targetNode));
			node->SetParent(current);
			if (!neighbourInOpen)
			{
				m_openNodes.push_back(node);
			}
		}
	}
	++m_totalSteps;
}

void Field::OnCompleted()
{
	Node const* current = m_targetNode;
	while (current != m_startNode)
	{
		current = current->GetParent();
		m_shortestPathNodes.push_back(current);
	}
}

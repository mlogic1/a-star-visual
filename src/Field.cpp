#include "Field.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

Field::Field()
{
	ParseInputFile();
}

Field::~Field()
{
	ClearData();
}

void Field::Step()
{
	if (m_isCompleted || m_openSet.size() < 1)
	{
		return;
	}

	// algorithm start
	Node* currentNode = m_openSet.at(0);

	for (Node* n : m_openSet)	// lowest cost node in open set
	{
		if (*n < *currentNode)
		{
			currentNode = n;
		}
	}

	m_openSet.erase(std::remove(m_openSet.begin(), m_openSet.end(), currentNode), m_openSet.end());
	m_closedSet.push_back(currentNode);

	if (currentNode == m_targetNode)
	{
		OnCompleted();
		return;
	}

	// neighbours
	std::vector<Node*> neighbourNodes = GetNeighbourNodes(currentNode);

	for (Node* neighbour : neighbourNodes)
	{
		if (neighbour->IsObstacle() || std::count(m_closedSet.begin(), m_closedSet.end(), neighbour))
		{
			continue;
		}

		bool neighbourInOpen = std::count(m_openSet.begin(), m_openSet.end(), neighbour);
		int newCost = currentNode->GetGCost() + GetDistanceBetweenNodes(currentNode, neighbour);
		if (newCost < neighbour->GetGCost() || !neighbourInOpen)
		{
			neighbour->SetCost(newCost, GetDistanceBetweenNodes(neighbour, m_targetNode));
			neighbour->SetParent(currentNode);
			if (!neighbourInOpen)
			{
				m_openSet.push_back(neighbour);
			}
		}
	}
}

void Field::Reset()
{
	ClearData();
	ParseInputFile();
}

bool Field::IsCompleted() const
{
	return m_isCompleted;
}

int Field::GetRows() const
{
	return m_rows;
}

int Field::GetColumns() const
{
	return m_columns;
}

const std::vector<Node*>& Field::GetShortestPathNodes() const
{
	return m_shortestPathNodes;
}

const std::vector<Node*>& Field::GetOpenSet() const
{
	return m_openSet;
}

const std::vector<Node*>& Field::GetClosedSet() const
{
	return m_closedSet;
}

const FieldType& Field::GetField() const
{
	return m_nodes;
}

void Field::ClearData()
{
	for (int i = 0 ; i < m_nodes.size() ; ++i)
	{
		std::vector<Node*>& row = m_nodes.at(i);

		for (Node* n : row)
		{
			delete n;
		}
		row.clear();
	}

	m_nodes.clear();
	m_openSet.clear();
	m_closedSet.clear();
	m_shortestPathNodes.clear();

	m_rows = 0;
	m_columns = 0;
	m_startNode = nullptr;
	m_targetNode = nullptr;
	m_isCompleted = false;
}

void Field::ParseInputFile()
{
	std::ifstream inFile(INPUT_FILE);
	std::string line;

	bool firstLineLoaded = false;
	int row = 0;

	if (!inFile.is_open())
	{
		throw std::string("Unable to read input file.");
	}

	while (std::getline(inFile, line))
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
			std::vector<Node*> nodeRow;
			std::stringstream ss(line);

			char nodeVal;
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
					case '0':	// walkable space
						break;

					case '1':	// obstacle
						obstacle = true;
						break;
					
					case 'S':	// start node
						isStart = true;
						break;

					case 'T':	// target node
						isTarget = true;
						break;

					default:
						throw std::string("Unknown character found while parsing input file: ") + std::to_string(nodeVal);

				}

				Node* n = new Node(x, y, obstacle, isStart, isTarget);

				if (nodeVal == 'S')
				{
					if (m_startNode != nullptr)
					{
						throw std::string("Duplicate starting node found. Check input file");
					}
					m_startNode = n;
				}
					

				if (nodeVal == 'T')
				{
					if (m_targetNode != nullptr)
					{
						throw std::string("Duplicate starting node found. Check input file");
					}
					m_targetNode = n;
				}
					

				nodeRow.push_back(n);
			}
			m_nodes[row] = nodeRow;
			++row;
		}
	}
	m_openSet.push_back(m_startNode);
	std::cout << "Loaded " << m_rows << " rows and " << m_columns << " columns" << std::endl;
}

void Field::OnCompleted()
{
	m_isCompleted = true;
	Node* current = m_targetNode;
	while (current != m_startNode)
	{
		current = current->GetParent();
		m_shortestPathNodes.push_back(current);
	}
}

std::vector<Node*> Field::GetNeighbourNodes(Node* currentNode)
{
	std::vector<Node*> neighbours;

	const int currentX = currentNode->GetX();
	const int currentY = currentNode->GetY();

	// top
	if (currentX > 0)
	{
		neighbours.push_back(m_nodes[currentX - 1].at(currentY));
	}

	// left
	if (currentY > 0)
	{
		neighbours.push_back(m_nodes[currentX].at(currentY - 1));
	}

	// right
	if (currentY < m_columns - 1)
	{
		neighbours.push_back(m_nodes[currentX].at(currentY + 1));
	}

	// bottom
	if (currentX < m_rows - 1)
	{
		neighbours.push_back(m_nodes[currentX + 1].at(currentY));
	}

	return neighbours;
}

int Field::GetDistanceBetweenNodes(const Node* n1, const Node* n2) const
{
	return std::abs(n1->GetX() - n2->GetX()) + std::abs(n1->GetY() - n2->GetY());
}

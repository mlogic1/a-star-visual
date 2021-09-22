#ifndef FIELD_H
#define FIELD_H

#include "Node.h"
#include <map>
#include <vector>

using FieldType = std::map<int, std::vector<Node*>>;

class Field
{
public:
	Field();
	~Field();
	
	void Step();
	void Reset();

	bool IsCompleted() const;
	int GetRows() const;
	int GetColumns() const;
	const std::vector<Node*>& GetShortestPathNodes() const;
	const std::vector<Node*>& GetOpenSet() const;
	const std::vector<Node*>& GetClosedSet() const;
	const FieldType& GetField() const;

private:
	void ClearData();
	void ParseInputFile();
	void OnCompleted();

	std::vector<Node*> GetNeighbourNodes(Node* currentNode);
	int GetDistanceBetweenNodes(const Node* n1, const Node* n2) const;

private:
	static constexpr const char* INPUT_FILE = "../input.txt";

	int m_rows;
	int m_columns;
	FieldType m_nodes;

	Node* m_startNode = nullptr;
	Node* m_targetNode = nullptr;

	std::vector<Node*> m_openSet;
	std::vector<Node*> m_closedSet;

	bool m_isCompleted = false;

	std::vector<Node*> m_shortestPathNodes;
};

#endif
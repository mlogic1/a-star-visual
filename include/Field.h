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
	void Reset();

	void Print();	// test only

	const FieldType& GetField() const;
	bool IsCompleted() const;
	int GetSteps() const;
	int GetRows() const;
	int GetColumns() const;

	const std::vector<Node*>& GetOpenNodes();
	const std::vector<Node*>& GetClosedNodes();
	const std::vector<const Node*>& GetPathNodes();

	void Step();

private:
	void OnCompleted();

private:
	static constexpr const char* INPUT_FILE = "input.txt";

	int m_rows;
	int m_columns;

	FieldType m_nodes;

	Node* m_startNode = nullptr;
	Node* m_targetNode = nullptr;
	std::vector<Node*> m_openNodes;
	std::vector<Node*> m_closedNodes;
	std::vector<const Node*> m_shortestPathNodes;
	bool m_completed = false;

	int m_totalSteps;
};

#endif
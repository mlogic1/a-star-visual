#ifndef NODE_H
#define NODE_H

class Node
{
public:
	Node(bool obstacle, int x, int y, bool isStart, bool isTarget);

	bool IsObstacle() const;
	bool IsStart() const;
	bool IsTarget() const;

	int GetGCost() const;
	int GetHCost() const;
	int GetFCost() const;
	int GetX() const;
	int GetY() const;

	void SetCost(int g, int h);
	void SetParent(Node* n);
	const Node* GetParent() const;

	int DistanceToNode(const Node& other) const;

	inline bool operator<(const Node& other) const
	{
		return GetFCost() <= other.GetFCost();
	}

private:
	int m_gCost = 0;
	int m_hCost = 0;
	Node* m_parent = nullptr;

	bool m_isObstacle;
	bool m_isStart;
	bool m_isTarget;
	int m_x;
	int m_y;
};

#endif
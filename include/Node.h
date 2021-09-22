#ifndef NODE_H
#define NODE_H

class Node
{
public:
	Node(int x, int y, bool obstacle, bool start, bool target);
	~Node() = default;

	bool IsObstacle() const;
	bool IsStart() const;
	bool IsTarget() const;

	int GetGCost() const;
	int GetHCost() const;
	int GetFCost() const;
	int GetX() const;
	int GetY() const;
	Node* GetParent() const;

	void SetCost(int g, int h);
	void SetParent(Node* parent);

	inline bool operator<(const Node& other) const
	{
		const int fCost = GetFCost();
		const int otherFCost = other.GetFCost();
		return fCost < other.GetFCost() || fCost == otherFCost && m_gCost < other.GetGCost();
	}
	
private:
	bool m_isObstacle;
	bool m_isStart;
	bool m_isTarget;

	int m_x;
	int m_y;

	int m_gCost = 0;
	int m_hCost = 0;
	Node* m_parent = nullptr;
};

#endif
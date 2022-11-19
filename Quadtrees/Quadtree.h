#pragma once
#include "Main.h"

class Puck;

class QuadTree
{
private:
	sf::Vector2f center;
	QuadTree* ChildNodes[4];
	Puck* ContainedPucks;
	//float halfwidthX;
	//float halfwidthY;

public:
	QuadTree(const sf::Vector2f& _center, float& _halfwidthX, float& _halfwidthY, int& _stopDepth);
	~QuadTree();
	static void Insert(QuadTree* Node, Puck* puck);
	//static sf::RectangleShape QuadTree::Visualize(QuadTree *&Node);
	static void TestAllCollisions(QuadTree*& Node);
	static void Clear(QuadTree* Node);
};

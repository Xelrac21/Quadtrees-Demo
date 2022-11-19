#include "Quadtree.h"

QuadTree::QuadTree(const sf::Vector2f& _center, float& _halfwidthX, float& _halfwidthY, int& _stopDepth)
{
	center = _center;
	float halfwidthX = _halfwidthX * 0.5f;
	float halfwidthY = _halfwidthY * 0.5f;
	ContainedPucks = nullptr;

	if (_stopDepth > 0)
	{
		sf::Vector2f _offset;
		for (unsigned int i = 0; i < 4; ++i)
		{
			_offset.x = ((i & 1) ? halfwidthX : -halfwidthX);
			_offset.y = ((i & 2) ? halfwidthY : -halfwidthY);
			int depth = _stopDepth - 1;
			ChildNodes[i] = new QuadTree(_center + _offset, halfwidthX, halfwidthY, depth);
			ChildNodes[i] = new QuadTree(_center + _offset, halfwidthX, halfwidthY, depth);
		}
	}
	else
	{
		return;
	}
}

QuadTree::~QuadTree()
{
	for (int i = 0; i < 4; ++i)
	{
		if (ChildNodes[i]) delete ChildNodes[i];
	}
}

void QuadTree::Insert(QuadTree* Node, Puck* puck)
{
	int index = 0;
	bool straddle = 0;

	float deltaX = puck->GetPosition().x - Node->center.x;
	float deltaY = puck->GetPosition().y - Node->center.y;
	float puckRadius = puck->GetRadius();

	for (int i = 0; i < 2; ++i)
	{
		float _delta;
		if (i == 0)
		{
			_delta = deltaX;
		}
		else
		{
			_delta = deltaY;
		}

		if (std::abs(_delta) <= puckRadius)
		{
			straddle = 1;
			break;
		}

		if (_delta > 0.0f) index |= (1 << i);
	}

	if (!straddle && Node->ChildNodes[index])
	{
		Insert(Node->ChildNodes[index], puck);
	}
	else
	{
		//quads.push_back(QuadTree::Visualize(Node));
		puck->AddNextPuck(Node->ContainedPucks);
		Node->ContainedPucks = puck;
	}
}

/*static sf::RectangleShape QuadTree::Visualize(QuadTree *&Node)
{
	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(Node->halfwidthX*4.0f, Node->halfwidthY*4.0f));
	rect.setOrigin(Node->halfwidthX*2.0f, Node->halfwidthY*2.0f);
	rect.setPosition(Node->center);
	rect.setOutlineColor(sf::Color::Yellow);
	rect.setOutlineThickness(-3);
	rect.setFillColor(sf::Color(0, 0, 0, 0));
	return rect;
}*/

void QuadTree::TestAllCollisions(QuadTree*& Node)
{
	const int MAX_DEPTH = 40;
	static QuadTree* ancestorStack[MAX_DEPTH];
	static int depth = 0;

	ancestorStack[depth++] = Node;
	for (unsigned int n = 0; n < depth; n++)
	{
		Puck* pA, * pB;
		for (pA = ancestorStack[n]->ContainedPucks; pA; pA = pA->GetNextPuck())
		{
			for (pB = Node->ContainedPucks; pB; pB = pB->GetNextPuck())
			{
				if (pA == pB) break;
				Puck::Collision(*pA, *pB);
			}
		}
	}

	for (unsigned int i = 0; i < 4; ++i)
	{
		if (Node->ChildNodes[i])
		{
			TestAllCollisions(Node->ChildNodes[i]);
		}
	}

	depth--;
}

void QuadTree::Clear(QuadTree* Node)
{
	Node->ContainedPucks = nullptr;
	for (int i = 0; i < 4; ++i)
	{
		if ((Node->ChildNodes[i]))
		{
			Clear(Node->ChildNodes[i]);
		}
	}
}
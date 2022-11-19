#include "Main.h"

void moveToScreen(const sf::Vector2f& position)
{
	if (objectPool.size() != 0)
	{
		objectsOnScreen.push_back(objectPool[objectPool.size() - 1]);
		objectsOnScreen[objectsOnScreen.size() - 1]->SetPosition(sf::Vector2f(position));
		objectPool.pop_back();
	}
}

void normalize(sf::Vector2f& vector)
{
	float denominator = sqrt(vector.x * vector.x + vector.y * vector.y);
	if (denominator)
	{
		vector.x = vector.x / denominator;
		vector.y = vector.y / denominator;
	}
}

void setKeyboardMovement(sf::Vector2f& movementDirection)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		movementDirection.x--;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		movementDirection.x++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		movementDirection.y--;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		movementDirection.y++;
	}
	normalize(movementDirection);
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(width, height), "Quadtrees");
	//window.setFramerateLimit(framerate);
	window.setKeyRepeatEnabled(true);

	sf::Clock clock;
	srand(rand() ^ time(0));

	unsigned int numPucks = 0;
	std::cin >> numPucks;

	objectPool.reserve(numPucks);
	for (unsigned int i = 0; i < numPucks; ++i)
	{
		objectPool.emplace_back(new Puck(rand() % 5 + 1.f, sf::Color::Red, sf::Vector2f(-5000, -5000), sf::Vector2f(rand() % 11 - 5, rand() % 11 - 5), 0.0f, 0.2f));
		int spawnX = width + 1;
		int spawnY = height + 1;
		moveToScreen(sf::Vector2f(rand() % spawnX, rand() % spawnY));
	}

	Puck* playerCircle = new Puck(20, sf::Color::Green, sf::Vector2f(width / 2.0f, height / 2.0f), sf::Vector2f(0, 0), 20.0f, 0.2f, true);
	objectsOnScreen.push_back(playerCircle);

	sf::Vector2f center(width * 0.5f, height * 0.5f);
	float newWidth = width * 0.5f;
	float newHeight = height * 0.5f;
	int depth = 8;
	QuadTree* quadtree = new QuadTree(center, newWidth, newHeight, depth);

	while (window.isOpen())
	{
		sf::Event event;
		float dt = clock.restart().asSeconds();
		float fps = 1.0f / (dt);
		std::cout << fps << std::endl;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		/*if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			int spawnX = width + 1;
			int spawnY = height + 1;
			moveToScreen(sf::Vector2f(rand()%(spawnX), rand()%(spawnY)));
		}*/

		for (unsigned int i = 0; i < objectsOnScreen.size(); ++i)
		{
			objectsOnScreen[i]->Move(dt);
		}

		for (unsigned int i = 0; i < objectsOnScreen.size(); ++i)
		{
			QuadTree::Insert(quadtree, objectsOnScreen[i]);
		}

		/*for (unsigned int j = 0; j < objectsOnScreen.size(); ++j)
		{
		if (i != j)
		{
		Puck::Collision(*objectsOnScreen[i], *objectsOnScreen[j]);
		}
		}*/

		QuadTree::TestAllCollisions(quadtree);
		QuadTree::Clear(quadtree);

		window.clear();
		/*for (unsigned int i = 0; i < quads.size(); ++i)
		{
			window.draw(quads[i]);
		}*/
		for (unsigned int i = 0; i < objectsOnScreen.size(); ++i)
		{
			objectsOnScreen[i]->Draw(window);
		}
		window.display();
		//quads.clear();
	}

	delete quadtree;
	for (unsigned int i = 0; i < objectPool.size(); ++i)
	{
		delete objectPool[i];
	}
	objectPool.clear();

	for (unsigned int i = 0; i < objectsOnScreen.size(); ++i)
	{
		delete objectsOnScreen[i];
	}
	objectsOnScreen.clear();
}
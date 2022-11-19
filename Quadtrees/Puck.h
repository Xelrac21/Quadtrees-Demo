#pragma once
#include "Main.h"

class Puck
{
private:
	sf::Vector2f m_Velocity = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f m_Acceleration = sf::Vector2f(0.0f, 0.0f);
	float m_Speed;
	float m_Elasticity;
	double m_Mass = 5;
	Puck* m_NextPuck = nullptr;
	sf::CircleShape m_Puck;
	bool m_isPlayer = false;

public:
	Puck(float radius, const sf::Color& color, const sf::Vector2f& initialPosition, const sf::Vector2f& velocity, float speed, float elasticity, bool isPlayer = false);
	void Draw(sf::RenderWindow& window);
	void Move(float dt);
	static void Collision(Puck& puck1, Puck& puck2);
	void AddNextPuck(Puck*& NodeList);
	Puck* GetNextPuck();
	const sf::Vector2f& GetPosition() const;
	void SetPosition(const sf::Vector2f& position);
	float GetRadius() const;
};
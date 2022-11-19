#include "Puck.h"

Puck::Puck(float radius, const sf::Color& color, const sf::Vector2f& initialPosition, const sf::Vector2f& velocity, float speed, float elasticity, bool isPlayer)
	: m_Velocity(velocity)
	, m_Speed(speed)
	, m_Elasticity(elasticity)
	, m_isPlayer(isPlayer)
	, m_NextPuck(nullptr)
{
	m_Puck.setOrigin(radius * 0.5f, radius * 0.5f);
	m_Puck.setRadius(radius);
	m_Puck.setFillColor(color);
	m_Puck.setPosition(initialPosition);
}

void Puck::Draw(sf::RenderWindow& window)
{
	window.draw(m_Puck);
}

void Puck::Move(float dt)
{
	/*if (m_Puck.getPosition().x - m_Puck.getGlobalBounds().width * 0.5f <= 0)
	{
		m_Puck.setPosition(m_Puck.getGlobalBounds().width * 0.5f + 1.0f, m_Puck.getPosition().y);
		m_Velocity.x = -m_Elasticity * m_Velocity.x * 4.f;
	}
	if (m_Puck.getPosition().x + m_Puck.getGlobalBounds().width * 0.5f >= width)
	{
		m_Puck.setPosition(width - m_Puck.getGlobalBounds().width * 0.5f - 1.0f, m_Puck.getPosition().y);
		m_Velocity.x = -m_Elasticity * m_Velocity.x * 4.f;
	}
	if (m_Puck.getPosition().y + m_Puck.getGlobalBounds().height * 0.5f >= height)
	{
		m_Puck.setPosition(m_Puck.getPosition().x, height - m_Puck.getGlobalBounds().height * 0.5f - 1.0f);
		m_Velocity.y = -m_Elasticity * m_Velocity.y * 4.f;
	}
	if (m_Puck.getPosition().y - m_Puck.getGlobalBounds().height * 0.5f <= 0)
	{
		m_Puck.setPosition(m_Puck.getPosition().x, m_Puck.getGlobalBounds().height * 0.5f + 1.0f);
		m_Velocity.y = -m_Elasticity * m_Velocity.y * 4.f;
	}*/

	float xPosition = GetPosition().x;
	float yPosition = GetPosition().y;
	float radius = GetRadius();

	if (xPosition - radius <= 0)
	{
		m_Puck.setPosition(radius + 0.1f, yPosition);
		m_Velocity.x = -m_Elasticity * m_Velocity.x;
	}
	if (xPosition + radius >= width)
	{
		m_Puck.setPosition(width - radius - 0.1f, yPosition);
		m_Velocity.x = -m_Elasticity * m_Velocity.x;
	}
	if (yPosition + radius >= height)
	{
		m_Puck.setPosition(xPosition, height - radius - 0.1f);
		m_Velocity.y = -m_Elasticity * m_Velocity.y;
	}
	if (yPosition - radius <= 0)
	{
		m_Puck.setPosition(xPosition, radius + 0.1f);
		m_Velocity.y = -m_Elasticity * m_Velocity.y;
	}

	if (m_isPlayer)
	{
		setKeyboardMovement(m_Acceleration);
	}
	else
	{
		normalize(m_Acceleration);
	}

	m_Velocity.x += m_Speed * m_Acceleration.x * dt;
	m_Velocity.y += m_Speed * m_Acceleration.y * dt;
	m_Acceleration.x = 0.f;
	m_Acceleration.y = 0.f;
	m_Puck.move(m_Velocity.x, m_Velocity.y);
}

void Puck::Collision(Puck& puck1, Puck& puck2)
{
	double distanceX = puck1.GetPosition().x - puck2.GetPosition().x;
	double distanceY = puck1.GetPosition().y - puck2.GetPosition().y;
	double distanceMagnitude = sqrt(distanceX * distanceX + distanceY * distanceY);
	double radiusLength = puck1.GetRadius() + puck2.GetRadius();

	if (distanceMagnitude <= radiusLength)
	{
		sf::Vector2f collisionNormal(distanceX / distanceMagnitude, distanceY / distanceMagnitude);
		puck1.m_Puck.move(collisionNormal.x, collisionNormal.y);
		puck2.m_Puck.move(-collisionNormal.x, -collisionNormal.y);

		sf::Vector2f relativeVelocities(puck1.m_Velocity.x - puck2.m_Velocity.x, puck1.m_Velocity.y - puck2.m_Velocity.y);
		double RdotC = relativeVelocities.x * collisionNormal.x + relativeVelocities.y * collisionNormal.y;
		double rMagnitude = sqrt(relativeVelocities.x * relativeVelocities.x + relativeVelocities.y * relativeVelocities.y);
		if (RdotC / rMagnitude < 0.f)
		{
			double CdotC = collisionNormal.x * collisionNormal.x + collisionNormal.y * collisionNormal.y;
			float impulse = -((1.f + puck1.m_Elasticity) * RdotC) / ((CdotC) * (1.f / puck1.m_Mass + 1.f / puck2.m_Mass));
			puck1.m_Velocity.x += (impulse / puck1.m_Mass) * collisionNormal.x;
			puck1.m_Velocity.y += (impulse / puck1.m_Mass) * collisionNormal.y;
			puck2.m_Velocity.x -= (impulse / puck2.m_Mass) * collisionNormal.x;
			puck2.m_Velocity.y -= (impulse / puck2.m_Mass) * collisionNormal.y;
		}
	}
}

void Puck::AddNextPuck(Puck*& NodeList)
{
	m_NextPuck = NodeList;
}

Puck* Puck::GetNextPuck()
{
	return m_NextPuck;
}

const sf::Vector2f& Puck::GetPosition() const
{
	return m_Puck.getPosition();
}

void Puck::SetPosition(const sf::Vector2f& position)
{
	m_Puck.setPosition(position);
}

float Puck::GetRadius() const
{
	return m_Puck.getRadius();
}
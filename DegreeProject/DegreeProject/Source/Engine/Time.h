#pragma once
#include <SFML/Graphics.hpp>

class Time
{
private:
	static float m_DeltaTime;
	static sf::Clock m_DeltaClock;

public:
	static float DeltaTime()
	{
		return m_DeltaTime;
	}

	static void UpdateTime()
	{
		m_DeltaTime = m_DeltaClock.restart().asSeconds();
	}
};
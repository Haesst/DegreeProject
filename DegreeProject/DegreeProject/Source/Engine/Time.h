#pragma once
#include <SFML/Graphics.hpp>

struct GameDate;

class Time
{
private:
	static float m_DeltaTime;
	static sf::Clock m_DeltaClock;
	static bool m_GamePaused;

public:
	static GameDate m_GameDate;

public:
	static float DeltaTime()
	{
		return m_DeltaTime;
	}

	static void UpdateTime();
	static void PauseGame();
	static void UnpauseGame();
	static bool GamePaused() { return m_GamePaused; };
};
#pragma once
#include <SFML/Graphics.hpp>

struct GameDate;

class Time
{
public:
	static float DeltaTime() { return m_DeltaTime;	};

	static void UpdateTime();
	static void PauseGame();
	static void UnpauseGame();
	static void DecreaseGameSpeed();
	static void IncreaseGameSpeed();
	static void SetGameSpeed(int speedLevel);
	static bool GamePaused() { return m_GamePaused; };

private:
	static float m_DeltaTime;
	static sf::Clock m_DeltaClock;
	static bool m_GamePaused;
	static float m_CurrentSpeedMultiplier;
	static float m_SpeedModifer;
	static int m_MinSpeedLevel;
	static int m_MaxSpeedLevel;

public:
	static GameDate m_GameDate;
	static int m_CurrentSpeedLevel;
};
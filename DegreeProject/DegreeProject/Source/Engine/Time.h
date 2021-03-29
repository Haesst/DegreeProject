#pragma once
#include <SFML/Graphics.hpp>

struct GameDate;

class Time
{
public:
	static float deltaTime() { return m_DeltaTime;	};

	static void updateTime();
	static void pauseGame();
	static void unpauseGame();
	static void decreaseGameSpeed();
	static void increaseGameSpeed();
	static void setGameSpeed(int speedLevel);
	static bool gamePaused() { return m_GamePaused; };

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
#include "Time.h"
#include "Game/GameDate.h"

float Time::m_DeltaTime = 0.0f;
float Time::m_CurrentSpeedMultiplier = 1.0f;
float Time::m_SpeedModifer = 0.3f;
int Time::m_MinSpeedLevel = 1;
int Time::m_MaxSpeedLevel = 5;
int Time::m_CurrentSpeedLevel = 3;
bool Time::m_GamePaused = false;
sf::Clock Time::m_DeltaClock;
GameDate Time::m_GameDate;

void Time::UpdateTime()
{
	m_DeltaTime = m_GamePaused ? 0 : m_DeltaClock.restart().asSeconds() * m_CurrentSpeedMultiplier;
	m_GameDate.Update();
}

void Time::PauseGame()
{
	m_GamePaused = true;
}

void Time::UnpauseGame()
{
	m_DeltaClock.restart().asSeconds();
	m_GamePaused = false;
}

void Time::DecreaseGameSpeed()
{
	if (m_CurrentSpeedLevel <= m_MinSpeedLevel)
	{
		return;
	}

	--m_CurrentSpeedLevel;
	m_CurrentSpeedMultiplier -= m_SpeedModifer;
}

void Time::IncreaseGameSpeed()
{
	if (m_CurrentSpeedLevel >= m_MaxSpeedLevel)
	{
		return;
	}

	++m_CurrentSpeedLevel;
	m_CurrentSpeedMultiplier += m_SpeedModifer;
}

void Time::SetGameSpeed(int speedLevel)
{
	if (speedLevel < m_MinSpeedLevel)
	{
		speedLevel = m_MinSpeedLevel;
	}

	if (speedLevel > m_MaxSpeedLevel)
	{
		speedLevel = m_MaxSpeedLevel;
	}

	int differenceToCurrentSpeed = speedLevel - m_CurrentSpeedLevel;

	float modifier = m_SpeedModifer * differenceToCurrentSpeed;

	m_CurrentSpeedLevel = speedLevel;
	m_CurrentSpeedMultiplier += modifier;
}

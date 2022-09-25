#include "Time.h"
#include "Game/GameDate.h"
#include "Engine/Window.h"
#include "Game/UI/UIManager.h"
#include "Game/UI/PauseWindow.h"

float Time::m_DeltaTime = 0.0f;
float Time::m_CurrentSpeedMultiplier = 1.0f;
float Time::m_SpeedModifer = 0.3f;
int Time::m_MinSpeedLevel = 1;
int Time::m_MaxSpeedLevel = 5;
unsigned long Time::m_Ticks = 0;
int Time::m_CurrentSpeedLevel = 3;
bool Time::m_GamePaused = false;
sf::Clock Time::m_DeltaClock;
GameDate Time::m_GameDate;

void Time::updateTime()
{
	m_DeltaTime = m_GamePaused || !Window::getWindow()->isOpen() ? 0 : m_DeltaClock.restart().asSeconds() * m_CurrentSpeedMultiplier;
	m_GameDate.update();

	if (m_GamePaused == false)
	{
		m_Ticks++;
	}
}

void Time::pauseGame()
{
	m_GamePaused = true;
	UIManager::get().m_PauseWindow->openWindow();
}

void Time::unpauseGame()
{
	m_DeltaClock.restart().asSeconds();
	m_GamePaused = false;
	UIManager::get().m_PauseWindow->closeWindow();
}

void Time::decreaseGameSpeed()
{
	if (m_CurrentSpeedLevel <= m_MinSpeedLevel)
	{
		return;
	}

	--m_CurrentSpeedLevel;
	m_CurrentSpeedMultiplier -= m_SpeedModifer;
}

void Time::increaseGameSpeed()
{
	if (m_CurrentSpeedLevel >= m_MaxSpeedLevel)
	{
		return;
	}

	++m_CurrentSpeedLevel;
	m_CurrentSpeedMultiplier += m_SpeedModifer;
}

void Time::setGameSpeed(int speedLevel)
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

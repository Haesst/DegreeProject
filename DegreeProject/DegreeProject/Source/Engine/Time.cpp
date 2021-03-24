#include "Time.h"
#include "Game/GameDate.h"

float Time::m_DeltaTime = 0.0f;
sf::Clock Time::m_DeltaClock;
GameDate Time::m_GameDate;
bool Time::m_GamePaused = false;

void Time::UpdateTime()
{
	m_DeltaTime = m_GamePaused ? 0 : m_DeltaClock.restart().asSeconds();
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

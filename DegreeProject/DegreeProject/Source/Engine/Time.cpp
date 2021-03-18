#include "Time.h"
#include "Game/GameDate.h"

float Time::m_DeltaTime = 0.0f;
sf::Clock Time::m_DeltaClock;
GameDate Time::m_GameDate;

void Time::UpdateTime()
{
	m_DeltaTime = m_DeltaClock.restart().asSeconds();
	m_GameDate.Update();
}

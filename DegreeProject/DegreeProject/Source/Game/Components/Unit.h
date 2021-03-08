#pragma once
#include "ECS/Component.h"
#include <ECS\Components\CharacterComponent.h>
#include <string>

struct UnitComponent : public Component
{
	sf::Sprite m_UnitSprite;
	int m_RepresentedForce = 0;
	sf::Vector2f m_Position = sf::Vector2f(0.0f, 0.0f);

	UnitComponent()
	{

	}

	UnitComponent(int representedForce, sf::Vector2f position)
	{
		m_RepresentedForce = representedForce;
		m_Position = position;
		m_UnitSprite.setPosition(m_Position);
	}
};

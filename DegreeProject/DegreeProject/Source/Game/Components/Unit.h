#pragma once
#include "ECS/Component.h"
#include "Game/Components/CharacterComponent.h"
#include <string>

struct UnitComponent : public Component
{
	sf::Sprite m_UnitSprite;
	int m_RepresentedForce = 0;
	Vector2DInt m_Position;
	Vector2DInt m_CurrentMapPosition;
	bool m_Raised = false;
	std::list<Vector2DInt> m_CurrentPath = std::list<Vector2DInt>();

	UnitComponent()
	{

	}

	UnitComponent(int representedForce, Vector2DInt position)
	{
		m_RepresentedForce = representedForce;
		m_Position = position;
		m_UnitSprite.setPosition(sf::Vector2f((float)position.x, (float)position.y));
	}
};

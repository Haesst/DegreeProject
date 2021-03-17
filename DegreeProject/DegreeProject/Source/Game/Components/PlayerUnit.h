#pragma once

#include <SFML/Graphics.hpp>

#include "ECS/Component.h"
#include "ECS/EntityManager.h"

#include "Engine/Vector2D.h"

struct PlayerUnit : public Component
{
	sf::RectangleShape m_Shape;
	sf::Color m_FillColor = sf::Color::Transparent;
	sf::Color m_OutlineColor = sf::Color::White;
	float m_Size = 32.0f;
	float m_OutlineThickness = 0.0f;
	bool m_Selected = false;
	bool m_Moving = false;
	Vector2DInt m_CurrentMapPosition;
	Vector2DInt m_RecentlyConquered;
	unsigned int m_Owner = 18;
	Vector2D m_Direction = Vector2D(0.0f, 0.0f);
	Vector2D m_Target = Vector2D(1920.0f, 1080.0f);
	float m_Speed = 150.0f;

	PlayerUnit(){}

	PlayerUnit(unsigned int owner)
	{
		m_Owner = owner;
	}
};
#pragma once

#include <SFML/Graphics.hpp>

#include "../../ECS/Component.h"
#include "../../ECS/EntityManager.h"

#include "../../Engine/Vector2D.h"

struct Player : public Component
{
	sf::RectangleShape m_Shape;
	sf::Color m_Color = sf::Color::Black;
	float m_Size = 10.0f;

	Vector2D m_Direction = Vector2D(1.0f, 1.0f);
	float m_Speed = 150.0f;
};
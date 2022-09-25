#pragma once

#include <Engine/Vector2D.h>

class Entity
{
public:

	float m_SizeX;
	float m_SizeY;
	float m_PositionX;
	float m_PositionY;
	bool m_Visible;
	Entity(Vector2D position, Vector2D size);
	virtual void start();
	virtual void update();
	virtual void render();
};

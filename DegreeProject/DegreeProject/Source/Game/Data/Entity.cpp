#include <Game/Data/Entity.h>

Entity::Entity(Vector2D position, Vector2D size)
{
	m_Visible = false;
	m_PositionX = position.x;
	m_PositionY = position.y;
	m_SizeX = size.x;
	m_SizeY = size.y;
}

void Entity::start()
{
}

void Entity::update()
{
}

void Entity::render()
{
}

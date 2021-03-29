#pragma once

#include <SFML/Graphics.hpp>

#include "ECS/Component.h"
#include "ECS/EntityManager.h"
#include "ECS/Components/Transform.h"
#include "Engine/AssetHandler.h"

struct UISpriteRenderer : public Component
{
	Transform* m_Transform = nullptr;

	sf::Sprite m_Sprite;
	sf::Texture m_Texture;

	int m_Width = 0;
	int m_Height = 0;

	UISpriteRenderer(){}

	UISpriteRenderer(const char* path, int width, int height, AssetHandler* assetHandler)
		: m_Width(width), m_Height(height)
	{
		m_Transform = &EntityManager::get().getComponent<Transform>(m_EntityID);
		m_Texture = assetHandler->loadImageFromFile(path);
		m_Sprite.setTexture(m_Texture);
		m_Sprite.setPosition(m_Transform->m_Position.x, m_Transform->m_Position.y);
		updateSize();
	}

	void updateSize()
	{
		sf::FloatRect localSize = m_Sprite.getLocalBounds();

		m_Sprite.setScale(
			m_Width / localSize.width,
			m_Height / localSize.height
		);
	}
};
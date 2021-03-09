#pragma once

#include <SFML/Graphics.hpp>

#include "ECS/Component.h"
#include "ECS/EntityManager.h"
#include "ECS/Components/Transform.h"
#include "Engine/AssetHandler.h"

struct UISpriteRenderer : public Component
{
	Transform* m_Transform;

	sf::Sprite m_Sprite;
	sf::Texture m_Texture;

	int m_Width;
	int m_Height;

	UISpriteRenderer()
	{}

	UISpriteRenderer(const char* path, int width, int height, AssetHandler* assetHandler)
		: m_Width(width), m_Height(height)
	{
		m_Transform = &EntityManager::Get().GetComponent<Transform>(m_EntityID);
		m_Texture = assetHandler->LoadImageFromFile(path);
		m_Sprite.setTexture(m_Texture);
		m_Sprite.setPosition(m_Transform->m_Position.x, m_Transform->m_Position.y);
		UpdateSize();
	}

	void UpdateSize()
	{
		sf::FloatRect localSize = m_Sprite.getLocalBounds();

		m_Sprite.setScale(
			m_Width / localSize.width,
			m_Height / localSize.height
		);
	}
};
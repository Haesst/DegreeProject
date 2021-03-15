#pragma once
#include "ECS/System.h"
#include <Game\Components\Unit.h>
#include <Engine\AssetHandler.h>

struct UnitSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	UnitComponent* m_UnitComponents = nullptr;

	UnitSystem()
	{
		AddComponentSignature<UnitComponent>();
		m_EntityManager = &EntityManager::Get();
		m_UnitComponents = m_EntityManager->GetComponentArray<UnitComponent>();

		AssetHandler assetLoader;
		sf::Texture tex = assetLoader.LoadImageFromFile("Assets/Graphics/Unit.png");
		SetSprite(tex);
	}

	void SetSprite(sf::Texture& tex)
	{
		//UnitComponent* units = m_EntityManager->GetComponentArray<UnitComponent>();
		//
		//for (auto unit : units)
		//{
		//	unit->m_UnitSprite.setTexture(tex);
		//}
	}

	virtual void Update() override 
	{

	}

	virtual void Render() override
	{
		for (auto entity : m_Entities)
		{
			if (m_UnitComponents[entity].m_Raised)
			{
				Window::GetWindow()->draw(m_UnitComponents[entity].m_UnitSprite);
			}
		}
	}
};

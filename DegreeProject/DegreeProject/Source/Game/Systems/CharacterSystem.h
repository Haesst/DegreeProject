#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"

#include "Engine/Time.h"
#include "Engine/Window.h"

#include "ECS/Components/Transform.h"
#include "Game/Components/CharacterComponent.h"

struct CharacterSystem : System
{
	EntityManager* m_EntityManager = nullptr;

	// Constructor, Runs when the system is initialized
	// Do any kind of init here but remember to register
	// the systems component signature. IE: add every component
	// that the system needs or should look for
	CharacterSystem()
	{
		AddComponentSignature<CharacterComponent>();
		m_EntityManager = &EntityManager::Get();
	}

	virtual void Start() override
	{
		CharacterComponent* characters = m_EntityManager->GetComponentArray<CharacterComponent>();

		for (auto entity : m_Entities)
		{
			for (unsigned int ownedID : characters[entity].m_OwnedRegionIDs)
			{
				MapInfo::SetOwnerName(characters[entity].m_KingdomName, ownedID);
			}
		}
	}

	// Update gets called every frame and loops through every entity that has the signature that
	// the system has registered and do the necessary update
	virtual void Update() override
	{
		//Transform* transforms = m_EntityManager->GetComponentArray<Transform>();
		CharacterComponent* characters = m_EntityManager->GetComponentArray<CharacterComponent>();

		for (auto entity : m_Entities)
		{
			if (characters[entity].m_UpdateOwnership == true)
			{
				characters[entity].m_UpdateOwnership = false;
				for (unsigned int ownedID : characters[entity].m_OwnedRegionIDs)
				{
					MapInfo::SetOwnerName(characters[entity].m_KingdomName, ownedID);
				}
			}

			// characters[entity].m_CurrentGold += TAX_INCOME;
			// characters[entity].m_CurrentArmySize += ARMY_RECRUITED;
			// characters[entity].m_OwnedRegionIDs.push_back(REGIONS_CONQUERED);
		}
	}

	virtual void Render() override
	{
		//CharacterComponent* characters = m_EntityManager->GetComponentArray<CharacterComponent>();

		//for (auto entity : m_Entities)
		//{
		//	
		//}
	}

	void ConquerRegion(int regionId, int conqueringEntity)
	{
		CharacterComponent* characters = m_EntityManager->GetComponentArray<CharacterComponent>();
		characters[conqueringEntity].m_OwnedRegionIDs.push_back(regionId);
	}

	void loseRegion(int regionId, int losingEntity)
	{
		CharacterComponent* characters = m_EntityManager->GetComponentArray<CharacterComponent>();
		auto& regions = characters[losingEntity].m_OwnedRegionIDs;

		for (unsigned int i = 0; i < regions.size(); i++)
		{
			if (regions[i] == (unsigned int)regionId)
			{
				regions.erase(regions.begin() + i);
				return;
			}
		} 
	}
};
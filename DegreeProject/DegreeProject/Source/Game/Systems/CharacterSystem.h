#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Time.h"
#include "Engine/Window.h"
#include "ECS/Components/Transform.h"
#include "Game/Components/CharacterComponent.h"
#include "Game/Components/Unit.h"
#include "Game/Components/SpriteRenderer.h"
#include "Game/Systems/UITextSystem.h"

struct CharacterSystem : System
{
	EntityManager* m_EntityManager = nullptr;
	CharacterComponent* m_Characters = nullptr;
	UnitComponent* m_Units = nullptr;
	SpriteRenderer* m_Renderers = nullptr;

	// Constructor, Runs when the system is initialized
	// Do any kind of init here but remember to register
	// the systems component signature. IE: add every component
	// that the system needs or should look for

	CharacterSystem()
	{
		AddComponentSignature<CharacterComponent>();
		m_EntityManager = &EntityManager::Get();
		m_Characters = m_EntityManager->GetComponentArray<CharacterComponent>();
		m_Units = m_EntityManager->GetComponentArray<UnitComponent>();
		m_Renderers = m_EntityManager->GetComponentArray<SpriteRenderer>();
	}

	virtual void Start() override
	{
		for (auto entity : m_Entities)
		{
			for (unsigned int ownedID : m_Characters[entity].m_OwnedRegionIDs)
			{
				Map::GetRegionById(ownedID).m_OwnerID = entity;
			}

			m_Characters[entity].Start();
		}
	}

	bool GetPlayerControlled(EntityID ent)
	{
		EntityManager* entityManager = &EntityManager::Get();
		CharacterComponent& character = entityManager->GetComponent<CharacterComponent>(ent);

		return character.m_IsPlayerControlled;
	}

	// Update gets called every frame and loops through every entity that has the signature that
	// the system has registered and do the necessary update
	virtual void Update() override
	{
		//Transform* transforms = m_EntityManager->GetComponentArray<Transform>();
		//CharacterComponent* characters = m_EntityManager->GetComponentArray<CharacterComponent>();

		for (auto entity : m_Entities)
		{
			if (m_Characters[entity].m_AtWar && !m_Units[m_Characters[entity].m_UnitEntity].m_Raised)
			{
				if (!GetPlayerControlled(entity))
				{
					int regionIndex = m_Characters[entity].m_OwnedRegionIDs[0];
					Vector2DInt capitalPos = Map::GetRegionCapitalLocation(regionIndex);
					RaiseUnit(entity, false, m_Units[m_Characters[entity].m_UnitEntity], m_Renderers[m_Characters[entity].m_UnitEntity], capitalPos);
				}
			}
		}
	}

	virtual void Render() override
	{
	}

	void RaiseUnit(EntityID ownerID, bool playerControlled, UnitComponent& unit, SpriteRenderer& renderer, const Vector2DInt& mapPosition)
	{
		CharacterComponent& character = m_EntityManager->GetComponent<CharacterComponent>(ownerID);

		Transform& unitTransform = m_EntityManager->GetComponent<Transform>(unit.GetID());
		unit.m_Owner = ownerID;
		unit.m_PlayerControlled = playerControlled;
		Vector2D screenPosition = Map::ConvertToScreen(mapPosition);
		unit.m_HighlightShape.setPosition(screenPosition.x, screenPosition.y);
		unitTransform.m_Position = screenPosition;
		renderer.m_Sprite.setPosition(screenPosition.x, screenPosition.y);
		Map::m_MapUnitData[mapPosition].AddUnique(unit.GetID());
		unit.m_LastPosition = screenPosition;
		unit.m_Raised = true;
		renderer.m_ShouldRender = true;
		renderer.m_Sprite.setColor(character.m_RegionColor);

		character.m_RaisedArmySize = character.m_MaxArmySize; //Todo: Army recharge
	}

	void ConquerRegion(unsigned int regionID, unsigned int conqueringEntity)
	{
		CharacterComponent& character = m_EntityManager->GetComponent<CharacterComponent>(conqueringEntity);
		character.m_OwnedRegionIDs.push_back(regionID);
		Map::SetRegionColor(regionID, character.m_RegionColor);
		Map::GetRegionById(regionID).m_OwnerID = conqueringEntity;
#pragma warning(push)
#pragma warning(disable: 26815)
		UITextSystem* textUISystem = (UITextSystem*)m_EntityManager->GetSystem<UITextSystem>().get();
#pragma warning(pop)
		textUISystem->ConquerRegion(regionID, conqueringEntity);
	}

	void LoseRegion(unsigned int regionID, unsigned int losingEntity)
	{
		CharacterComponent& character = m_EntityManager->GetComponent<CharacterComponent>(losingEntity);
		unsigned int regionIndex = 0;
		for (unsigned int OwnedRegionID : character.m_OwnedRegionIDs)
		{
			if (OwnedRegionID == regionID)
			{
				character.m_OwnedRegionIDs.erase(character.m_OwnedRegionIDs.begin() + regionIndex);
				break;
			}
			regionIndex++;
		}
#pragma warning(push)
#pragma warning(disable: 26815)
		UITextSystem* textUISystem = (UITextSystem*)m_EntityManager->GetSystem<UITextSystem>().get();
#pragma warning(pop)
		textUISystem->LoseRegion(regionIndex, losingEntity);
	}

	EntityID GetPlayerID()
	{
		CharacterComponent* characters = m_EntityManager->GetComponentArray<CharacterComponent>();

		for (auto entity : m_Entities)
		{
			if (characters[entity].m_IsPlayerControlled)
			{
				return characters[entity].GetID();
			}
		}
		return 0;
	}
};
#pragma once

#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "Engine/Time.h"
#include "Engine/Window.h"
#include "ECS/Components/Transform.h"
#include "Game/Components/CharacterComponent.h"
#include "Game/Components/Unit.h"
#include "Game/Components/SpriteRenderer.h"
#include "Game/Systems/UI/UITextSystem.h"
#include "Game/WarManager.h"

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
		addComponentSignature<CharacterComponent>();
		m_EntityManager = &EntityManager::get();
		m_Characters = m_EntityManager->getComponentArray<CharacterComponent>();
		m_Units = m_EntityManager->getComponentArray<UnitComponent>();
		m_Renderers = m_EntityManager->getComponentArray<SpriteRenderer>();

		WarManager* warManager = &WarManager::get(); 
	}

	virtual void start() override
	{
		for (auto entity : m_Entities)
		{
			for (unsigned int ownedID : m_Characters[entity].m_OwnedRegionIDs)
			{
				Map::get().getRegionById(ownedID).m_OwnerID = entity;
			}

			m_Characters[entity].start();
		}
	}

	bool getPlayerControlled(EntityID ent)
	{
		EntityManager* entityManager = &EntityManager::get();
		CharacterComponent& character = entityManager->getComponent<CharacterComponent>(ent);

		return character.m_IsPlayerControlled;
	}

	// Update gets called every frame and loops through every entity that has the signature that
	// the system has registered and do the necessary update
	virtual void update() override
	{
		//Transform* transforms = m_EntityManager->GetComponentArray<Transform>();
		//CharacterComponent* characters = m_EntityManager->GetComponentArray<CharacterComponent>();

		for (auto entity : m_Entities)
		{
			if (m_Characters[entity].m_AtWar && !m_Units[m_Characters[entity].m_UnitEntity].m_Raised)
			{
				if (m_Characters[entity].m_RecentlyAtWar)
				{
					if (!getPlayerControlled(entity))
					{
						int regionIndex = m_Characters[entity].m_OwnedRegionIDs[0];
						Vector2DInt capitalPos = Map::get().getRegionCapitalLocation(regionIndex);
						raiseUnit(entity, false, m_Units[m_Characters[entity].m_UnitEntity], m_Renderers[m_Characters[entity].m_UnitEntity], capitalPos);
						m_Characters[entity].m_RecentlyAtWar = false;
					}
				}
			}
		}
	}

	virtual void render() override
	{
	}

	void declareWar(EntityID character, EntityID target, int warGoalRegion)
	{
		auto* warManager = &WarManager::get();

		for (auto& war : m_Characters[character].m_CurrentWars)
		{
			//if (warManager->getWar(war)->m_Defender->getID() == target)
			//{
			//	LOG_INFO("Tried to declare war against current opponent");
			//	return;
			//}
		}

		EntityManager* entityManager = &EntityManager::get();
		CharacterComponent* characters = entityManager->getComponentArray<CharacterComponent>();
		WarmindComponent* warminds = nullptr;//entityManager->getComponentArray<WarmindComponent>();

		LOG_INFO("{0} Declared war with {1} for {2}", m_Characters[character].m_Name, characters[target].m_Name, Map::get().getRegionById(warGoalRegion).m_RegionName);
		//int war = warManager->createWar(characters[character], characters[target], warGoalRegion);
		//m_Characters[character].m_CurrentWars.push_back(war);
		//m_Characters[character].m_AtWar = true;
		//
		//characters[target].m_RecentlyAtWar = true;
		//characters[target].m_CurrentWars.push_back(war);
		//characters[target].m_AtWar = true;
		//
		//if (!m_Characters[character].m_IsPlayerControlled)
		//{
		//	//warminds[character].m_Active = true;
		//	m_Characters[character].m_RecentlyAtWar = true;
		//}
		//
		//if (!characters[target].m_IsPlayerControlled)
		//{
		//	//warminds[target].m_Opponent = character;
		//	//warminds[target].m_Active = true;
		//}
	}

	void makePeace(CharacterComponent& attacker, CharacterComponent& defender, int warToEnd)
	{
		unsigned int warIndex = 0;

		WarManager* warManager = &WarManager::get();

		for (auto& handle : attacker.m_CurrentWars)
		{
			if (handle == warToEnd)
			{
				if (warManager->getWar(handle) != nullptr)
				{
					warManager->endWar(warToEnd);
				}

				attacker.m_CurrentWars.erase(attacker.m_CurrentWars.begin() + warIndex);
				break;
			}
			warIndex++;
		}

		warIndex = 0;

		for (auto& handle : defender.m_CurrentWars)
		{
			if (handle == warToEnd)
			{
				defender.m_CurrentWars.erase(defender.m_CurrentWars.begin() + warIndex);
				break;
			}
			warIndex++;
		}

		LOG_INFO("{0} Made peace with {1}", attacker.m_Name, defender.m_Name);
	}

	void raiseUnit(EntityID ownerID, bool playerControlled, UnitComponent& unit, SpriteRenderer& renderer, const Vector2DInt& mapPosition)
	{
		CharacterComponent& character = m_EntityManager->getComponent<CharacterComponent>(ownerID);

		if (unit.m_Raised)
		{
			return;
		}

		Transform& unitTransform = m_EntityManager->getComponent<Transform>(unit.getID());
		unit.m_Owner = ownerID;
		unit.m_PlayerControlled = playerControlled;
		Vector2D screenPosition = Map::convertToScreen(mapPosition);
		unit.m_HighlightShape.setPosition(screenPosition.x, screenPosition.y);
		unitTransform.m_Position = screenPosition;
		renderer.m_Sprite.setPosition(screenPosition.x, screenPosition.y);
		
		for (auto& squareData : Map::get().m_MapSquareData)
		{
			if (squareData.m_Position == mapPosition)
			{
				squareData.addUnique(unit.getID());
				break;
			}
		}

		unit.m_LastPosition = screenPosition;
		unit.m_Raised = true;
		renderer.m_ShouldRender = true;
		renderer.m_Sprite.setColor(character.m_RegionColor);

		character.m_RaisedArmySize = character.m_MaxArmySize; //Todo: Army recharge
	}

	void dismissUnit(EntityID ownerID)
	{
		CharacterComponent& character = m_EntityManager->getComponent<CharacterComponent>(ownerID);
		UnitComponent& unit = m_EntityManager->getComponent<UnitComponent>(character.m_UnitEntity);
		if (!unit.m_Raised)
		{
			return;
		}
		unit.m_Moving = false;
		unit.m_CurrentPath.clear();
		unit.m_RepresentedForce = 0;
		unit.m_Raised = false;
		SpriteRenderer& renderer = m_EntityManager->getComponent<SpriteRenderer>(character.m_UnitEntity);
		renderer.m_ShouldRender = false;
		character.m_RaisedArmySize = 0;
		Transform& unitTransform = m_EntityManager->getComponent<Transform>(character.m_UnitEntity);
		Vector2DInt unitMapPosition = Map::convertToMap(unitTransform.m_Position);
		
		for (auto& squareData : Map::get().m_MapSquareData)
		{
			if (squareData.m_Position == unitMapPosition)
			{
				squareData.remove(unit.getID());
				break;
			}
		}
	}

	void conquerRegion(unsigned int regionID, unsigned int conqueringEntity)
	{
		CharacterComponent& character = m_EntityManager->getComponent<CharacterComponent>(conqueringEntity);
		character.m_OwnedRegionIDs.push_back(regionID);
		Map::get().setRegionColor(regionID, character.m_RegionColor);
		Map::get().getRegionById(regionID).m_OwnerID = conqueringEntity;
#pragma warning(push)
#pragma warning(disable: 26815)
		UITextSystem* textUISystem = (UITextSystem*)m_EntityManager->getSystem<UITextSystem>().get();
#pragma warning(pop)
		textUISystem->conquerRegion(regionID, conqueringEntity);
	}

	void loseRegion(unsigned int regionID, unsigned int losingEntity)
	{
		CharacterComponent& character = m_EntityManager->getComponent<CharacterComponent>(losingEntity);
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
		UITextSystem* textUISystem = (UITextSystem*)m_EntityManager->getSystem<UITextSystem>().get();
#pragma warning(pop)
		textUISystem->loseRegion(regionIndex, losingEntity);
	}

	EntityID getPlayerID()
	{
		CharacterComponent* characters = m_EntityManager->getComponentArray<CharacterComponent>();

		for (auto entity : m_Entities)
		{
			if (characters[entity].m_IsPlayerControlled)
			{
				return characters[entity].getID();
			}
		}
		return 0;
	}
};
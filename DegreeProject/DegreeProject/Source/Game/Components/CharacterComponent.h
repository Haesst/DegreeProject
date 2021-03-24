#pragma once
#include <vector>
#include "Game/GameDate.h"
#include "Game/Map/Map.h"
#include "Engine/Log.h"
#include "ECS/Component.h"
#include "Game/Components/Unit.h"
#include "Game/Components/SpriteRenderer.h"
#include "Game/Components/Warmind.h"
#include "Game/War.h"

enum class Title
{
	Emperor,
	King,
	Duke,
	Count,
	Baron
};

#pragma warning(push)
#pragma warning(disable: 26812)
struct CharacterComponent : public Component
{
	sf::Sprite m_CharacterPortrait;
	Title m_CharacterTitle = Title::Baron;
	const char* m_KingdomName = "";
	const char* m_Name = "";
	std::vector<unsigned int> m_OwnedRegionIDs;
	int m_CurrentGold = 0;
	unsigned int m_RaisedArmySize = 0;
	unsigned int m_CurrentMaxArmySize = 0;
	unsigned int m_MaxArmySize = 0;
	bool m_IsPlayerControlled = false;
	sf::Color m_RegionColor = sf::Color::Black;
	bool m_AtWar = false;
	int m_PersonalityIndex = 0;
	int m_Income = 0;
	War* m_CurrentWar = nullptr;

	EntityID m_UnitEntity = INT_MAX;

	CharacterComponent() {};

	CharacterComponent(Title title, const char* kingdomName, const char* characterName, std::vector<unsigned int> regionIDs, int gold, int armySize, bool isPlayerControlled, sf::Color regionColor, int personalityIndex)
	{
		m_CharacterTitle = title;
		m_KingdomName = kingdomName;
		m_Name = characterName;
		m_OwnedRegionIDs = regionIDs;
		m_CurrentGold = gold;
		m_MaxArmySize = armySize;
		m_CurrentMaxArmySize = m_MaxArmySize;
		m_IsPlayerControlled = isPlayerControlled;
		m_RegionColor = regionColor;
		m_PersonalityIndex = personalityIndex;

		InitUnit();
	}

	void Start()
	{
		Time::m_GameDate.SubscribeToMonthChange(std::bind(&CharacterComponent::OnMonthChange, this, std::placeholders::_1));
	}

	void MakePeace()
	{
		m_AtWar = false;

		if (!m_IsPlayerControlled)
		{
			EntityManager* entityManager = &EntityManager::Get();
			WarmindComponent& warmind = entityManager->GetComponent<WarmindComponent>(m_EntityID);
			UnitComponent& unit = entityManager->GetComponent<UnitComponent>(m_UnitEntity);

			warmind.m_Active = false;
			warmind.m_AtWar = false;
			m_RaisedArmySize = 0;
			unit.m_Raised = false;
		}

		delete m_CurrentWar;
		m_CurrentWar = nullptr;
	}

	void DeclareWar(EntityID target, int warGoalRegion)
	{
		EntityManager* entityManager = &EntityManager::Get();
		CharacterComponent* characters = entityManager->GetComponentArray<CharacterComponent>();
		WarmindComponent* warminds = entityManager->GetComponentArray<WarmindComponent>();

		LOG_INFO("{0} Declared war with {1} for {2}", m_Name, characters[target].m_Name, Map::GetRegionById(warGoalRegion).m_RegionName);

		War* war = new War(characters[m_EntityID], characters[target], warGoalRegion);
		m_CurrentWar = war;

		if (!m_IsPlayerControlled)
		{
			m_AtWar = true;
			warminds[m_EntityID].m_Active = true;
		}

		if (!characters[target].m_IsPlayerControlled)
		{
			characters[target].m_CurrentWar = war;
			characters[target].m_AtWar = true;
			warminds[target].m_Opponent = m_EntityID;
			warminds[target].m_Active = true;
		}
	}

	void OnMonthChange(Date)
	{
		int incomingGold = 0;

		for (auto& regionId : m_OwnedRegionIDs)
		{
			MapRegion region = Map::GetRegionById(regionId);

			incomingGold += region.m_RegionTax;
		}

		incomingGold -= (int)(m_RaisedArmySize * 0.1f);
		m_Income = incomingGold;
		m_CurrentGold += incomingGold;
	}

	void InitUnit()
	{
		EntityManager* entityManager = &EntityManager::Get();
		CharacterComponent& character = entityManager->GetComponent<CharacterComponent>(m_EntityID);

		m_UnitEntity = entityManager->AddNewEntity();
		entityManager->AddComponent<UnitComponent>(m_UnitEntity, character.m_MaxArmySize, m_EntityID);

		AssetHandler handler;
		entityManager->AddComponent<SpriteRenderer>(m_UnitEntity, "Assets/Graphics/soldier unit.png", 32, 32, &handler);
		SpriteRenderer& renderer = entityManager->GetComponent<SpriteRenderer>(m_UnitEntity);
		renderer.m_ShouldRender = false;
	}
};
#pragma warning(pop)

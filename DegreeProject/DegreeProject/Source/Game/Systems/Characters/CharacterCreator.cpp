#include "CharacterCreator.h"

#include "Game/Data/CharacterConstants.h"
#include "Game/Systems/Characters/CharacterNamePool.h"

#include "Game/Systems/Characters/CharacterPool.h"
#include "Game/Systems/UnitManager.h"
#include "Game/Map/Map.h"
#include "Game/Map/MapRegion.h"
#include "Game/AI/AIManager.h"

CharacterID CharacterCreator::createCharacterWithRandomBirthday(CharacterPool& characterPool, const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, size_t minAge, size_t maxAge)
{
	ASSERT(minAge <= maxAge, "Minimum age can't be less than maximum age");

	Character& character = characterPool.Rent();

	character.m_Birthday = Time::m_GameDate.getRandomDate(false, minAge, maxAge);

	return internalCreateCharacter(character, characterName, title, gender, ownedRegions, realmName, army, gold, color, playerControlled);
}

CharacterID CharacterCreator::createCharacter(CharacterPool& characterPool, const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, Date birthday)
{
	Character& character = characterPool.Rent();

	character.m_Birthday = birthday;

	return internalCreateCharacter(character, characterName, title, gender, ownedRegions, realmName, army, gold, color, playerControlled);
}

CharacterID CharacterCreator::createNewChild(CharacterPool& characterPool, CharacterID motherID)
{
	CharacterManager& characterManager = CharacterManager::get();

	bool male = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) <= 0.5f;
	Gender gender = male ? Gender::Male : Gender::Female;
	char* name = male ? CharacterNamePool::getMaleName() : CharacterNamePool::getFemaleName();
	std::vector<unsigned int> regions = std::vector<unsigned int>();
	CharacterID childID = createCharacter(characterPool, name, Title::Unlanded, gender, regions, "NONAME", 0, 0, sf::Color::Black, false, Time::m_GameDate.m_Date);
	Character& child = characterManager.getCharacter(childID);
	Character& mother = characterManager.getCharacter(motherID);
	Character& father = characterManager.getCharacter(mother.m_LastChildFather);
	child.m_Mother = motherID;
	child.m_Father = father.m_CharacterID;
	mother.m_Children.push_back(childID);
	father.m_Children.push_back(childID);

	for (Trait& trait : mother.m_Traits)
	{
		if (trait.m_Inheritable)
		{
			bool inherited = characterManager.chancePerPercent(CharacterConstants::m_InheritTraitChance);

			if (inherited && !characterManager.hasTrait(childID, trait))
			{
				characterManager.addTrait(childID, trait);
			}
		}
	}

	for (Trait& trait : father.m_Traits)
	{
		if (trait.m_Inheritable)
		{
			bool inherited = characterManager.chancePerPercent(CharacterConstants::m_InheritTraitChance);

			if (inherited && !characterManager.hasTrait(childID, trait))
			{
				characterManager.addTrait(childID, trait);
			}
		}
	}

	return childID;
}

CharacterID CharacterCreator::internalCreateCharacter(Character& character, const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled)
{
	CharacterManager& characterManager = CharacterManager::get();

	CharacterID id = characterManager.getNewCharacterID();

	character.m_CharacterID = id;
	character.m_CharacterTitle = title;

	character.m_OwnedRegionIDs = ownedRegions;
	character.m_KingdomName = realmName;

	character.m_CurrentMaxArmySize = army;

	character.m_Fertility = (rand() % (CharacterConstants::m_UpperBaseFertility - CharacterConstants::m_LowerBaseFertility) + CharacterConstants::m_LowerBaseFertility) * CharacterConstants::m_OneOverOneHundred;
	character.m_Gender = gender;

	character.m_CurrentGold = gold;

	character.m_IsPlayerControlled = playerControlled;

	character.m_RegionColor = color;

	character.m_Name = characterName;
	character.m_UnitEntity = UnitManager::get().addUnit(id, army);

	for (auto& regionid : ownedRegions)
	{
		MapRegion& region = Map::get().getRegionById(regionid);
		character.m_MaxArmySize += region.m_ManPower;

		for (auto& buildingSlot : region.m_BuildingSlots)
		{
			if (buildingSlot.m_BuildingId == INVALID_BUILDING_ID)
			{
				continue;
			}

			character.m_MaxArmySize += GameData::m_Buildings[buildingSlot.m_BuildingId].m_ArmyModifier;
		}
	}

	characterManager.addNewCharacter(character);

	for (Trait trait : characterManager.getAllTraits())
	{
		if (!trait.m_CanSpawnWith)
		{
			continue;
		}

		bool chance = characterManager.chancePerPercent(CharacterConstants::m_NewTraitChance);

		if (chance && !characterManager.hasTrait(id, trait))
		{
			characterManager.addTrait(id, trait);
		}
	}

	if (playerControlled)
	{
		characterManager.setPlayerCharacter(character);
	}

	else
	{
		AIManager::get().initAI(id);
	}

	return id;
}

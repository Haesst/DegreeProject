#include "CharacterManager.h"

#include "Engine/Log.h"
#include "Game/Systems/UnitManager.h"
#include "Game/Systems/Characters/CharacterNamePool.h"

#include "Game/GameDate.h"
#include "Game/Map/Map.h"
#include "Game/AI/AIManager.h"
#include "Game/UI/UIManager.h"
#include "Game/Game.h"
#include "Game/Player.h"
#include <random>
#include <math.h>

#include <fstream>
#include <json.hpp>
using json = nlohmann::json;
#include <mutex>
#include "Game/WarManager.h"

CharacterManager* CharacterManager::m_Instance = nullptr;
CharacterID CharacterManager::m_CharacterIDs = 1;

CharacterManager* CharacterManager::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new CharacterManager();
	}

	return m_Instance;
}

CharacterID CharacterManager::createCharacterWithRandomBirthday(const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, size_t minAge, size_t maxAge)
{
	ASSERT(minAge <= maxAge, "Minimum age can't be less than maximum age");

	Character& character = m_CharacterPool.Rent();

	character.m_Birthday = Time::m_GameDate.getRandomDate(false, minAge, maxAge);

	return internalCreateCharacter(character, characterName, title, gender, ownedRegions, realmName, army, gold, color, playerControlled);
}

void CharacterManager::loadTraits(const char* path)
{
	std::ifstream file(path);
	json j;
	file >> j;

	m_TraitMtx.lock();
	m_Traits.clear();

	for (auto& element : j)
	{
		std::string traitName = element["debugName"].get<std::string>();

		int generalOppinion = element["generalOpinion"].get<int>();
		int attraction = element["attraction"].get<int>();
		int fertility = element["fertility"].get<int>();

		Trait trait(traitName);
		trait.generalOppinion = generalOppinion;
		trait.attraction = attraction;
		trait.fertility = fertility;

		m_Traits.push_back(trait);
	}

	m_TraitMtx.unlock();
}

CharacterID CharacterManager::createNewChild(CharacterID motherID)
{
	bool male = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) <= 0.5f;
	Gender gender = male ? Gender::Male : Gender::Female;
	char* name = male ? CharacterNamePool::getMaleName() : CharacterNamePool::getFemaleName();
	std::vector<unsigned int> regions = std::vector<unsigned int>();
	CharacterID childID = createCharacter(name, Title::Unlanded, gender, regions, "NONAME", 0, 0, sf::Color::Black, false, Time::m_GameDate.m_Date);
	Character& child = getCharacter(childID);
	Character& mother = getCharacter(motherID);
	Character& father = getCharacter(mother.m_LastChildFather);
	child.m_Mother = motherID;
	child.m_Father = father.m_CharacterID;
	mother.m_Children.push_back(childID);
	father.m_Children.push_back(childID);
	return childID;
}

void CharacterManager::createUnlandedCharacters(size_t amount)
{
	for (size_t i = 0; i < amount; ++i)
	{
		bool male = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) <= 0.5f;
		Gender gender = male ? Gender::Male : Gender::Female;
		char* name = male ? CharacterNamePool::getMaleName() : CharacterNamePool::getFemaleName();
		std::vector<unsigned int> regions = std::vector<unsigned int>();

		createCharacterWithRandomBirthday(name, Title::Unlanded, gender, regions, "NONAME", 0, 0, sf::Color::White, false, 1, 72);
	}
}

CharacterID CharacterManager::createCharacter(const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, Date birthday)
{
	Character& character = m_CharacterPool.Rent();

	character.m_Birthday = birthday;

	return internalCreateCharacter(character, characterName, title, gender, ownedRegions, realmName, army, gold, color, playerControlled);
}

void CharacterManager::addTrait(CharacterID ID, Trait trait)
{
	for (auto& t : getCharacter(ID).m_Traits)
	{
		if (t.m_TraitName == trait.m_TraitName)
		{
			return;
		}
	}

	getCharacter(ID).m_Traits.push_back(trait);
}

void CharacterManager::removeTrait(CharacterID ID, Trait trait)
{
	unsigned int index = 0;
	for (auto& t : getCharacter(ID).m_Traits)
	{
		if (t.m_TraitName == trait.m_TraitName)
		{
			getCharacter(ID).m_Traits.erase(getCharacter(ID).m_Traits.begin() + index);
			break;
		}

		index++;
	}
}

bool CharacterManager::hasTrait(CharacterID ID, Trait trait)
{
	for (auto& t : getCharacter(ID).m_Traits)
	{
		if (t.m_TraitName == trait.m_TraitName)
		{
			return true;
		}
	}

	return false;
}

int CharacterManager::getCharacterOpinion(CharacterID characterID, CharacterID otherID)
{
	Character& character = getCharacter(characterID);
	Character& other = getCharacter(otherID);

	bool useAttraction = character.m_Gender != other.m_Gender; // Todo: Allow other attraction types than just heterosexual

	int opinion = 0;

	for (auto& trait : other.m_Traits)
	{
		if (trait.m_TraitName == "INVALID")
		{
			continue;
		}

		opinion += trait.generalOppinion;

		if (useAttraction)
		{
			opinion += trait.attraction;
		}
	}

	if (character.m_Spouse == otherID)
	{
		opinion += m_SpouseOpinion;
	}

	if (character.m_Father == otherID || character.m_Mother == otherID)
	{
		opinion += m_ParentOpinion;
	}

	for (auto& child : character.m_Children)
	{
		if (getCharacter(child).m_CharacterID == otherID)
		{
			opinion += m_ChildOpinion;
			break;
		}
	}

	if (character.m_Father == otherID || character.m_Mother == otherID)
	{
		opinion += m_ParentOpinion;
	}

	return opinion;
}

Trait CharacterManager::getTrait(const char* traitName)
{
	for (auto trait : m_Traits)
	{
		if (trait.m_TraitName == traitName)
		{
			return trait;
		}
	}

	return Trait("INVALID");
}

bool CharacterManager::isAlliedWith(CharacterID character, CharacterID other)
{
	for (auto ID : getCharacter(character).m_Allies)
	{
		if (ID == other)
		{
			return true;
		}
	}

	return false;
}

void CharacterManager::onAllianceCreated(CharacterID character, CharacterID other)
{
	getCharacter(character).m_Allies.push_back(character);
	getCharacter(other).m_Allies.push_back(other);
}

void CharacterManager::sendAllianceOffer(CharacterID sender, CharacterID reciever)
{
	if (!getCharacter(reciever).m_IsPlayerControlled)
	{
		if (AIManager::get().handleAllianceRequest(sender, reciever))
		{
			onAllianceCreated(sender, reciever);
			if (getCharacter(sender).m_IsPlayerControlled)
			{
				UIManager::get()->createUIEventElement(reciever, sender, UIType::AllianceAccepted);
			}
		}
		else if (getCharacter(sender).m_IsPlayerControlled)
		{
			UIManager::get()->createUIEventElement(reciever, sender, UIType::AllianceDeclined);
		}
	}
	else
	{
		UIManager::get()->createUIEventElement(sender, reciever, UIType::AllianceRequest);
	}
}

void CharacterManager::onWarEnded(CharacterID sender, CharacterID reciever)
{
	WarManager::get().endWar(WarManager::get().getWarAgainst(sender, reciever)->getHandle(), sender);
}

void CharacterManager::sendPeaceOffer(CharacterID sender, CharacterID reciever, PeaceType type)
{
	if (!getCharacter(reciever).m_IsPlayerControlled)
	{
		if (AIManager::get().handlePeaceRequest(sender, reciever, type))
		{
			onWarEnded(sender, reciever);
			if (getCharacter(sender).m_IsPlayerControlled)
			{
				UIManager::get()->createUIEventElement(reciever, sender, UIType::PeaceAccepted);
			}
		}
		else if (getCharacter(sender).m_IsPlayerControlled)
		{
			UIManager::get()->createUIEventElement(reciever, sender, UIType::PeaceDeclined);
		}
	}
	else
	{
		UIManager::get()->createUIEventElement(sender, reciever, UIType::PeaceRequest);
	}
}

void CharacterManager::start()
{
	Time::m_GameDate.subscribeToMonthChange(std::bind(&CharacterManager::onMonthChange, this, std::placeholders::_1));

	for (auto& character : m_Characters)
	{
		for (int id : character.m_OwnedRegionIDs)
		{
			Map::get().setRegionColor(id, character.m_RegionColor);
			Map::get().getRegionById(id).m_OwnerID = character.m_CharacterID;
		}
	}

	createUnlandedCharacters(m_UnlandedCharactersAtStart);
}

void CharacterManager::update()
{
	m_Player->update();
	
	for (auto& character : m_Characters)
	{
		if (character.m_Dead)
		{
			continue;
		}

		if (m_LastDayUpdate < Time::m_GameDate.m_Date)
		{
			dailyUpdates(character);
		}
	}

	m_LastDayUpdate = Time::m_GameDate.m_Date;
}

void CharacterManager::dailyUpdates(Character& character)
{
	tryForPregnancy(character);

	if (character.m_Gender == Gender::Female)
	{
		progressPregnancy(character);
	}
}

void CharacterManager::tryForPregnancy(Character& character)
{
	if (character.m_Dead || character.m_Gender == Gender::Female)
	{
		return;
	}

	if (character.m_Spouse == INVALID_CHARACTER_ID)
	{
		return; // Later add support for lovers and remove this check
	}

	Character& spouse = getCharacter(character.m_Spouse);

	if (spouse.m_Gender == character.m_Gender || hasTrait(spouse.m_CharacterID, getTrait("Pregnant")))
	{
		return;
	}

	unsigned int spouseAge = Time::m_GameDate.getAge(spouse.m_Birthday);
	float spouseFertility = (((spouseAge - m_FertilityBarrenAge) * (m_FertilityBarrenAge - spouseAge) * (spouseAge - m_FertilityBarrenSmoother)) * m_OneOverFertilityCurveSteep) * spouse.m_Fertility;

	float traitFertility = 0.0f;

	for (Trait& trait : character.m_Traits)
	{
		traitFertility += (float)trait.fertility;
	}

	for (Trait& trait : spouse.m_Traits)
	{
		traitFertility += (float)trait.fertility;
	}

	traitFertility *= 0.1f;
	traitFertility += 1.0f;

	float fertility = (character.m_Fertility * spouseFertility) * traitFertility;

	bool success = chancePerPercent(fertility * 0.1f); // Todo: This should absolutely not have a magic number.

	if (success)
	{
		addTrait(character.m_Spouse, getTrait("Pregnant"));
		spouse.m_PregnancyDay = Time::m_GameDate.m_Date;
		spouse.m_LastChildFather = character.m_CharacterID;
		if (character.m_Spouse == getPlayerCharacterID() || character.m_CharacterID == getPlayerCharacterID())
		{
			UIManager::get()->createUIEventElement(character.m_Spouse, character.m_CharacterID, UIType::Pregnant);
		}
	}
}

void CharacterManager::progressPregnancy(Character& character)
{
	if (!hasTrait(character.m_CharacterID, getTrait("Pregnant")))
	{
		return;
	}

	unsigned int pregnancyDays = Time::m_GameDate.getDaysBetweenDates(character.m_PregnancyDay, Time::m_GameDate.m_Date);

	if (pregnancyDays < m_PregnancyDays - m_MaxPrematureBirth)
	{
		return;
	}

	bool chance = chancePerPercent(m_BirthChance);

	if (chance || pregnancyDays > m_PregnancyDays + m_MaxLateBirth) // Todo: Make this more random
	{
		createNewChild(character.m_CharacterID);
		removeTrait(character.m_CharacterID, getTrait("Pregnant"));
		if (character.m_Spouse == getPlayerCharacterID() || character.m_CharacterID == getPlayerCharacterID())
		{
			UIManager::get()->createUIEventElement(character.m_CharacterID, character.m_Spouse, UIType::ChildBirth);
		}
	}
}

void CharacterManager::render()
{
	m_Player->render();
}

CharacterManager::CharacterManager()
	: m_CharacterPool(CharacterPool(m_PoolInitSize, m_PoolGrowSize))
{
	loadTraits("Assets\\Data\\Traits.json");
}

CharacterManager::~CharacterManager()
{
	delete m_Player;
}

Character& CharacterManager::getPlayerCharacter()
{
	ASSERT(m_PlayerCharacterID != INVALID_CHARACTER_ID, "Player not set");
	return getCharacter(m_PlayerCharacterID);
}

CharacterID CharacterManager::getPlayerCharacterID()
{
	ASSERT(m_PlayerCharacterID != INVALID_CHARACTER_ID, "Player not set");
	return m_PlayerCharacterID;
}

#pragma warning(push)
#pragma warning(disable: 4239 4172)
Character& CharacterManager::getCharacter(CharacterID id)
{
	ASSERT(id != INVALID_CHARACTER_ID, "Invalid character id requested");

	for (auto& character : m_Characters)
	{
		if (character.m_CharacterID == id)
		{
			return character;
		}
	}

	ASSERT(false, "Character id not found");
	return Character();
}
#pragma warning(pop)

void CharacterManager::onMonthChange(Date)
{
	for (auto& character : m_Characters)
	{
		float incomingGold = 0;

		for (unsigned int id : character.m_OwnedRegionIDs)
		{
			if (Map::get().getRegionById(id).m_OccupiedBy != INVALID_CHARACTER_ID)
			{
				continue;
			}

			MapRegion& region = Map::get().getRegionById(id);

			for (auto& slot : region.m_BuildingSlots)
			{
				if (slot.m_BuildingId == -1)
				{
					continue;
				}

				Building& building = GameData::m_Buildings[slot.m_BuildingId];
				
				incomingGold += building.m_IncomeModifier;
			}

			incomingGold += region.m_RegionTax;
		}

		incomingGold -= (character.m_RaisedArmySize * 0.1f); // Todo: Declare army cost somewhere
		character.m_Income = incomingGold; // Todo: Change to prediction for upcoming month instead of showing last month.
		character.m_CurrentGold += incomingGold;

		if (!character.m_Dead)
		{	
			unsigned int age = Time::m_GameDate.getAge(character.m_Birthday);
			if (age > character.m_DeadlyAge)
			{
				float dieChance = ((float)((age - character.m_DeadlyAge) * (age - character.m_DeadlyAge)) / (character.m_AgeMax * character.m_AgeMax)) * m_MortalityRate;
				bool die = chancePerPercent(dieChance);
				if (die)
				{
					killCharacter(character.m_CharacterID);
				}
			}
		}
	}
}

void CharacterManager::killCharacter(CharacterID characterID)
{
	if (characterID == INVALID_CHARACTER_ID)
	{
		return;
	}

	for (auto& region : getCharacter(characterID).m_OwnedRegionIDs)
	{
		WarManager::get().invalidateWarsForRegion(region);
	}

	Character& character = getCharacter(characterID);
	character.m_Dead = true;

	if (character.m_CharacterTitle != Title::Unlanded && character.m_OwnedRegionIDs.size() > 0)
	{
		UIManager::get()->createUIEventElement(characterID, characterID, UIType::Death);
		handleInheritance(character);
	}

	character.m_RegionColor = sf::Color::Black;
	character.m_KingdomName = "";
	character.m_CurrentGold = 0;
	character.m_MaxArmySize = 0;
	character.m_RaisedArmySize = 0;
	character.m_OwnedRegionIDs.clear();
	character.m_CharacterTitle = Title::Unlanded;
	if (character.m_Spouse != INVALID_CHARACTER_ID)
	{
		getCharacter(character.m_Spouse).m_Spouse = INVALID_CHARACTER_ID;
		character.m_Spouse = INVALID_CHARACTER_ID;
	}
}

void CharacterManager::updateTitle(Character& character)
{
	if (character.m_OwnedRegionIDs.size() == 0)
	{
		character.m_CharacterTitle = Title::Unlanded;
		character.m_KingdomName = "Unlanded ";
	}
	if (character.m_OwnedRegionIDs.size() == 1)
	{
		character.m_CharacterTitle = Title::Baron;
		character.m_KingdomName = "Barony of " + Map::get().getRegionById(character.m_OwnedRegionIDs.front()).m_RegionName;
	}
	else if (character.m_OwnedRegionIDs.size() == 2 || character.m_OwnedRegionIDs.size() == 3)
	{
		character.m_CharacterTitle = Title::Count;
		character.m_KingdomName = "County of " + Map::get().getRegionById(character.m_OwnedRegionIDs.front()).m_RegionName;
	}
	else if (character.m_OwnedRegionIDs.size() == 4)
	{
		character.m_CharacterTitle = Title::Duke;
		character.m_KingdomName = "Duchy of " + Map::get().getRegionById(character.m_OwnedRegionIDs.front()).m_RegionName;
	}
	else if (character.m_OwnedRegionIDs.size() <= 6)
	{
		character.m_CharacterTitle = Title::King;
		character.m_KingdomName = "Kingdom of " + Map::get().getRegionById(character.m_OwnedRegionIDs.front()).m_RegionName;
	}
	else if (character.m_OwnedRegionIDs.size() >= 9)
	{
		character.m_CharacterTitle = Title::Emperor;
		character.m_KingdomName = "Empire of " + Map::get().getRegionById(character.m_OwnedRegionIDs.front()).m_RegionName;
	}
}

void CharacterManager::handleInheritance(Character& character)
{
	if (character.m_Children.size() == 1)
	{
		Character& child = getCharacter(character.m_Children.front());
		child.m_CurrentGold += character.m_CurrentGold;
		child.m_UnitEntity = character.m_UnitEntity;
		child.m_RegionColor = character.m_RegionColor;
		if (!child.m_IsPlayerControlled)
		{
			child.m_IsPlayerControlled = character.m_IsPlayerControlled;
		}
		if (child.m_IsPlayerControlled)
		{
			m_PlayerCharacterID = child.m_CharacterID;
			m_PlayerCharacter = &child;
		}
		for (unsigned int ownedRegionID : character.m_OwnedRegionIDs)
		{
			child.m_OwnedRegionIDs.push_back(ownedRegionID);
			Map::get().setRegionColor(ownedRegionID, child.m_RegionColor);
		}
		updateTitle(child);
		UIManager::get()->createUITextElement(Game::m_UIFont, child.m_CharacterID, child.m_KingdomName, child.m_OwnedRegionIDs);
		for (unsigned int ownedRegionID : character.m_OwnedRegionIDs)
		{
			UIManager::get()->AdjustOwnership(child.m_CharacterID, character.m_CharacterID, ownedRegionID);
		}
	}
	else if (character.m_Children.size() > 1)
	{
		unsigned int children = character.m_Children.size();
		unsigned int regions = character.m_OwnedRegionIDs.size();
		float giveawayGold = character.m_CurrentGold / children;
		float giveawayRegions = (float)regions / children;
		unsigned int numberOfLoops = 1;
		if (giveawayRegions > 1)
		{
			numberOfLoops = (int)ceilf(giveawayRegions);
		}
		for (unsigned int index = 0; index < numberOfLoops; index++)
		{
			for (unsigned int ownedRegionID : character.m_OwnedRegionIDs)
			{
				for (CharacterID childID : character.m_Children)
				{
					Character& child = getCharacter(childID);
					if (!child.m_Dead && !child.m_Inherited)
					{
						child.m_Inherited = true;
						if (child.m_RegionColor == sf::Color::Black)
						{
							child.m_RegionColor = sf::Color((sf::Uint8)std::rand(), (sf::Uint8)std::rand(), (sf::Uint8)std::rand());
						}
						addRegion(childID, ownedRegionID);
						Map::get().setRegionColor(ownedRegionID, child.m_RegionColor);
						break;
					}
				}
			}
			for (CharacterID childID : character.m_Children)
			{
				getCharacter(childID).m_Inherited = false;
			}
			if (numberOfLoops - index != 1 && numberOfLoops > 1)
			{
				for (unsigned int i = 0; i < children; i++)
				{
					character.m_OwnedRegionIDs[i] = character.m_OwnedRegionIDs[character.m_OwnedRegionIDs.size() - 1];
					character.m_OwnedRegionIDs.pop_back();
				}
			}
		}
		for (CharacterID childID : character.m_Children)
		{
			Character& child = getCharacter(childID);
			if (child.m_OwnedRegionIDs.size() > 0)
			{
				if (children > regions)
				{
					for (unsigned int i = 0; i < child.m_OwnedRegionIDs.size(); i++)
					{
						child.m_CurrentGold += giveawayGold;
					}
				}
				else
				{
					child.m_CurrentGold += giveawayGold;
				}
				updateTitle(child);
				UIManager::get()->createUITextElement(Game::m_UIFont, childID, child.m_KingdomName, child.m_OwnedRegionIDs);
				for (unsigned int childOwnedRegionID : child.m_OwnedRegionIDs)
				{
					UIManager::get()->AdjustOwnership(childID, character.m_CharacterID, childOwnedRegionID);
				}
			}
		}
		Character& child = CharacterManager::get()->getCharacter(character.m_Children.front());
		if (!child.m_IsPlayerControlled)
		{
			child.m_IsPlayerControlled = character.m_IsPlayerControlled;
		}
		if (child.m_IsPlayerControlled)
		{
			m_PlayerCharacterID = child.m_CharacterID;
			m_PlayerCharacter = &child;
		}
	}
	else
	{
		float giveawayGold = character.m_CurrentGold / character.m_OwnedRegionIDs.size();
		for (unsigned int ownedRegionID : character.m_OwnedRegionIDs)
		{
			for (Character& otherCharacter : m_Characters)
			{
				if (otherCharacter.m_CharacterTitle == Title::Unlanded && !otherCharacter.m_Dead && otherCharacter.m_Father == INVALID_CHARACTER_ID && otherCharacter.m_Mother == INVALID_CHARACTER_ID)
				{
					addRegion(otherCharacter.m_CharacterID, ownedRegionID);
					otherCharacter.m_CurrentGold += giveawayGold;
					otherCharacter.m_RegionColor = sf::Color((sf::Uint8)std::rand(), (sf::Uint8)std::rand(), (sf::Uint8)std::rand());
					updateTitle(otherCharacter);
					Map::get().setRegionColor(ownedRegionID, otherCharacter.m_RegionColor);
					UIManager::get()->createUITextElement(Game::m_UIFont, otherCharacter.m_CharacterID, otherCharacter.m_KingdomName, otherCharacter.m_OwnedRegionIDs);
					UIManager::get()->AdjustOwnership(otherCharacter.m_CharacterID, character.m_CharacterID, ownedRegionID);
					break;
				}
			}
		}
	}
	character.m_CurrentWars.clear();
	character.m_IsPlayerControlled = false;
	//if (character.m_UnitEntity != INVALID_UNIT_ID)
	//{
	//	UnitManager::get().dismissUnit(character.m_UnitEntity);
	//	character.m_UnitEntity = INVALID_UNIT_ID;
	//}
}

bool CharacterManager::chancePerPercent(float weight)
{
	float f = rand() * 1.0f / RAND_MAX;
	//float vv = weight / 10.0f;
	return f < weight;
}


void CharacterManager::constructBuilding(const CharacterID characterId, const int buildingId, const int regionId, const int buildingSlot)
{
	Character& character = getCharacter(characterId);
	Building building = GameData::m_Buildings[buildingId];
	MapRegion& region = Map::get().getRegionById(regionId);

	if (building.m_Cost > character.m_CurrentGold)
	{
		return;
	}

	if (region.m_OwnerID != characterId)
	{
		return;
	}

	if (region.m_BuildingSlots[buildingSlot].m_BuildingId != -1)
	{
		return;
	}

	Map::get().startConstructionOfBuilding(buildingId, buildingSlot, regionId);
	character.m_CurrentGold -= building.m_Cost;
}

void CharacterManager::addRegion(const CharacterID characterId, const unsigned int regionId)
{
	Character& character = getCharacter(characterId);
	character.m_OwnedRegionIDs.push_back(regionId);

	MapRegion region = Map::get().getRegionById(regionId);
	character.m_MaxArmySize += region.m_ManPower;

	for (auto& buildingSlot : region.m_BuildingSlots)
	{
		if (buildingSlot.m_BuildingId == -1)
		{
			continue;
		}

		character.m_MaxArmySize += GameData::m_Buildings[buildingSlot.m_BuildingId].m_ArmyModifier;
	}
}

void CharacterManager::removeRegion(const CharacterID characterId, const unsigned int regionId)
{
	Character& character = getCharacter(characterId);

	for (size_t i = 0; i < character.m_OwnedRegionIDs.size(); ++i)
	{
		if (character.m_OwnedRegionIDs[i] == regionId)
		{
			character.m_OwnedRegionIDs.erase(character.m_OwnedRegionIDs.begin() + i);
			MapRegion region = Map::get().getRegionById(regionId);
			character.m_MaxArmySize -= region.m_ManPower;

			for (auto& buildingSlot : region.m_BuildingSlots)
			{
				if (buildingSlot.m_BuildingId == -1)
				{
					continue;
				}

				character.m_MaxArmySize -= GameData::m_Buildings[buildingSlot.m_BuildingId].m_ArmyModifier;
			}
			break;
		}
	}
}

void CharacterManager::onMarriage(CharacterID sender, CharacterID reciever)
{
	getCharacter(sender).m_Spouse = reciever;
	getCharacter(reciever).m_Spouse = sender;
}

void CharacterManager::marry(CharacterID character, CharacterID spouse)
{
	if (getCharacter(spouse).m_Spouse != INVALID_CHARACTER_ID || getCharacter(character).m_Spouse != INVALID_CHARACTER_ID)
	{
		return;
	}

	if (Time::m_GameDate.getAge(getCharacter(character).m_Birthday) < m_AgeOfConsent || Time::m_GameDate.getAge(getCharacter(spouse).m_Birthday) < m_AgeOfConsent)
	{
		return;
	}

	if (!getCharacter(spouse).m_IsPlayerControlled)
	{
		if (AIManager::get().handleRecieveMarriageRequest(spouse, character))
		{
			onMarriage(character, spouse);
			if (getCharacter(character).m_IsPlayerControlled)
			{
				UIManager::get()->createUIEventElement(spouse, character, UIType::MarriageAccepted);
			}
		}
		else if(getCharacter(character).m_IsPlayerControlled)
		{
			UIManager::get()->createUIEventElement(spouse, character, UIType::MarriageDeclined);
		}
	}
	else
	{
		UIManager::get()->createUIEventElement(character, spouse, UIType::MarriageRequest);
	}
}

CharacterID CharacterManager::internalCreateCharacter(Character& character, const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled)
{
	CharacterID id = m_CharacterIDs++;

	character.m_CharacterID = id;
	character.m_CharacterTitle = title;

	character.m_OwnedRegionIDs = ownedRegions;
	character.m_KingdomName = realmName;

	//character.m_MaxArmySize = army;
	character.m_CurrentMaxArmySize = army;

	//Generate random number between 0 - 1, will be used as percent
	character.m_Fertility = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
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
			if (buildingSlot.m_BuildingId == -1)
			{
				continue;
			}

			character.m_MaxArmySize += GameData::m_Buildings[buildingSlot.m_BuildingId].m_ArmyModifier;
		}
	}

	m_Characters.push_back(character);

	if (playerControlled)
	{
		ASSERT(m_PlayerCharacter == nullptr, "No multiplayer game yet, only one player controlled character allowed.");

		m_PlayerCharacter = &getCharacter(id);
		m_PlayerCharacterID = id;

		m_Player = new Player(id);
	}

	else
	{
		AIManager::get().initAI(id);
	}

	return id;
}
#include "CharacterManager.h"

#include "Engine/Log.h"
#include "Game/Systems/UnitManager.h"
#include "Game/Systems/Characters/CharacterNamePool.h"

#include "Game/GameDate.h"
#include "Game/Map/Map.h"
#include "Game/AI/AIManager.h"
#include "Game/UI/UIManager.h"
#include "Game/UI/DateBar.h"
#include "Game/UI/StatBar.h"
#include "Game/Game.h"
#include "Game/Player.h"
#include "Game/Data/Trait.h"
#include "Game/Data/CharacterConstants.h"
#include <random>
#include <math.h>

#include <fstream>
#include <json.hpp>
using json = nlohmann::json;
#include <mutex>
#include "Game/WarManager.h"

CharacterManager* CharacterManager::m_Instance = nullptr;
CharacterID CharacterManager::m_CharacterIDs = 1;

CharacterManager& CharacterManager::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new CharacterManager();
	}

	return *m_Instance;
}

CharacterID CharacterManager::getNewCharacterID()
{
	return m_CharacterIDs++;
}

void CharacterManager::setPlayerCharacter(Character& character)
{
	ASSERT(m_PlayerCharacter == nullptr, "No multiplayer game yet, only one player controlled character allowed.");

	m_PlayerCharacter = &character;
	m_PlayerCharacterID = character.m_CharacterID;

	m_Player = new Player(character.m_CharacterID);
}

void CharacterManager::addNewCharacter(Character& character)
{
	m_Characters.push_back(character);
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

CharacterID CharacterManager::createCharacterWithRandomBirthday(const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, size_t minAge, size_t maxAge)
{
	return m_CharacterCreator.createCharacterWithRandomBirthday(m_CharacterPool, characterName, title, gender, ownedRegions, realmName, army, gold, color, playerControlled, minAge, maxAge);
}

CharacterID CharacterManager::createCharacter(const char* characterName, Title title, Gender gender, std::vector<unsigned int>& ownedRegions, const char* realmName, int army, float gold, sf::Color color, bool playerControlled, Date birthday)
{
	return m_CharacterCreator.createCharacter(m_CharacterPool, characterName, title, gender, ownedRegions, realmName, army, gold, color, playerControlled, birthday);
}

CharacterID CharacterManager::createNewChild(CharacterID motherID)
{
	return m_CharacterCreator.createNewChild(m_CharacterPool, motherID);
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
		bool inheritable = element["inheritable"].get<bool>();
		bool canSpawnWith = element["canSpawnWith"].get<bool>();

		Trait trait(traitName);
		trait.m_GeneralOpinion = generalOppinion;
		trait.m_Attraction = attraction;
		trait.m_Fertility = fertility;
		trait.m_Inheritable = inheritable;
		trait.m_CanSpawnWith = canSpawnWith;

		m_Traits.push_back(trait);
	}

	m_TraitMtx.unlock();
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

	if (isAlliedWith(character.m_CharacterID, other.m_CharacterID))
	{
		opinion += 40;
	}

	for (auto& trait : other.m_Traits)
	{
		if (trait.m_TraitName == "INVALID")
		{
			continue;
		}

		opinion += trait.m_GeneralOpinion;

		if (useAttraction)
		{
			opinion += trait.m_Attraction;
		}
	}

	if (character.m_Spouse == otherID)
	{
		opinion += CharacterConstants::m_SpouseOpinion;
	}

	if (character.m_Father == otherID || character.m_Mother == otherID)
	{
		opinion += CharacterConstants::m_ParentOpinion;
	}

	for (auto& child : character.m_Children)
	{
		if (getCharacter(child).m_CharacterID == otherID)
		{
			opinion += CharacterConstants::m_ChildOpinion;
			break;
		}
	}

	if (character.m_Father == otherID || character.m_Mother == otherID)
	{
		opinion += CharacterConstants::m_ParentOpinion;
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
	for (auto ID : WarManager::get().getAlliances(character))
	{
		if (ID == other)
		{
			return true;
		}
	}

	return false;
}

void CharacterManager::callAllies(CharacterID character, int war)
{
	War* currentWar = WarManager::get().getWar(war);
	AIManager* aiManager = &AIManager::get();

	for (auto ID : WarManager::get().getAlliances(character))
	{
		if (!getCharacter(ID).m_IsPlayerControlled)
		{
			if (aiManager->handleWarCallRequest(character, ID, war))
			{
				if (character == currentWar->getAttacker())
				{
					aiManager->getWarmindOfCharacter(ID).m_Active = true;
					currentWar->m_Attackers.push_back(ID);
				}
	
				else if (character == currentWar->getDefender())
				{
					aiManager->getWarmindOfCharacter(ID).m_Active = true;
					currentWar->m_Defenders.push_back(ID);
				}

				if (character == m_PlayerCharacterID)
				{
					UIManager::get().createUIEventElement(ID, character, UIType::CallToArmsAccepted);
				}
			}
			else if (character == m_PlayerCharacterID)
			{
				UIManager::get().createUIEventElement(ID, character, UIType::CallToArmsRejected);
			}
		}
	
		else
		{
			UIManager::get().createUIEventElement(character, ID, UIType::CallToArmsRequest);
		}
	}
}

void CharacterManager::sendAllianceOffer(CharacterID sender, CharacterID reciever)
{
	if (getCharacter(sender).m_Dead || getCharacter(reciever).m_Dead)
	{
		if (getCharacter(sender).m_IsPlayerControlled)
		{
			UIManager::get().createUIEventElement(reciever, sender, UIType::AllianceDeclined);
		}
		return;
	}

	if (!getCharacter(reciever).m_IsPlayerControlled)
	{
		if (AIManager::get().handleAllianceRequest(sender, reciever))
		{
			WarManager::get().createAlliance(sender, reciever);
			if (getCharacter(sender).m_IsPlayerControlled)
			{
				UIManager::get().createUIEventElement(reciever, sender, UIType::AllianceAccepted);
			}
		}
		else if (getCharacter(sender).m_IsPlayerControlled)
		{
			UIManager::get().createUIEventElement(reciever, sender, UIType::AllianceDeclined);
		}
	}
	else
	{
		UIManager::get().createUIEventElement(sender, reciever, UIType::AllianceRequest);
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
			switch (type)
			{
			case PeaceType::Enforce_Demands:
				onWarEnded(sender, reciever);
				break;

			case PeaceType::White_Peace:
				WarManager::get().endWar(WarManager::get().getWarAgainst(sender, reciever)->getHandle(), INVALID_CHARACTER_ID);
				break;

			case PeaceType::Surrender:
				onWarEnded(reciever, sender);
				break;
			default:
				break;
			}

			if (getCharacter(sender).m_IsPlayerControlled)
			{
				UIManager::get().createUIEventElement(reciever, sender, UIType::PeaceAccepted);
			}
		}
		else if (getCharacter(sender).m_IsPlayerControlled)
		{
			UIManager::get().createUIEventElement(reciever, sender, UIType::PeaceDeclined);
		}
	}
	else
	{
		UIManager::get().createUIEventElement(sender, reciever, UIType::PeaceRequest);
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

	createUnlandedCharacters(CharacterConstants::m_UnlandedCharactersAtStart);
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
	float spouseFertility = (((spouseAge - CharacterConstants::m_FertilityBarrenAge) * (CharacterConstants::m_FertilityBarrenAge - spouseAge) * (spouseAge - CharacterConstants::m_FertilityBarrenSmoother)) * CharacterConstants::m_OneOverFertilityCurveSteep) * spouse.m_Fertility;

	float traitFertility = 0.0f;

	for (Trait& trait : character.m_Traits)
	{
		traitFertility += (float)trait.m_Fertility;
	}

	for (Trait& trait : spouse.m_Traits)
	{
		traitFertility += (float)trait.m_Fertility;
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
			UIManager::get().createUIEventElement(character.m_Spouse, character.m_CharacterID, UIType::Pregnant);
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

	if (pregnancyDays < CharacterConstants::m_PregnancyDays - CharacterConstants::m_MaxPrematureBirth)
	{
		return;
	}

	bool chance = chancePerPercent(CharacterConstants::m_BirthChance);

	if (chance || pregnancyDays > CharacterConstants::m_PregnancyDays + CharacterConstants::m_MaxLateBirth) // Todo: Make this more random
	{
		createNewChild(character.m_CharacterID);
		removeTrait(character.m_CharacterID, getTrait("Pregnant"));
		if (character.m_Spouse == getPlayerCharacterID() || character.m_CharacterID == getPlayerCharacterID())
		{
			UIManager::get().createUIEventElement(character.m_CharacterID, character.m_Spouse, UIType::ChildBirth);
		}
	}
}

void CharacterManager::render()
{
	m_Player->render();
}

CharacterManager::CharacterManager()
	: m_CharacterPool(CharacterPool(CharacterConstants::m_PoolInitSize, CharacterConstants::m_PoolGrowSize))
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
				if (slot.m_BuildingId == INVALID_BUILDING_ID)
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
	}
}

void CharacterManager::killCharacter(CharacterID characterID)
{
	Character& character = getCharacter(characterID);
	WarManager* warManager = &WarManager::get();

	for (auto& region : getCharacter(characterID).m_OwnedRegionIDs)
	{
		WarManager::get().invalidateWarsForRegion(region);
	}

	for (auto& war : warManager->getWarHandlesOfCharacter(characterID))
	{
		War* currentWar = warManager->getWar(war);

		if (currentWar->isAllyOf(characterID, currentWar->getAttacker()))
		{
			warManager->removeAllyFromWar(characterID, currentWar->getHandle());
		}

		else if (currentWar->isAllyOf(characterID, currentWar->getDefender()))
		{
			warManager->removeAllyFromWar(characterID, war);
		}
	}

	if (!character.m_IsPlayerControlled)
	{
		AIManager::get().deactivateAI(characterID);
	}

	if (characterID == INVALID_CHARACTER_ID)
	{
		return;
	}

	UnitManager::get().dismissUnit(character.m_UnitEntity);

	character.m_Dead = true;

	if (character.m_CharacterTitle != Title::Unlanded && character.m_OwnedRegionIDs.size() > 0)
	{
		UIManager::get().createUIEventElement(characterID, characterID, UIType::Death);
		handleInheritance(character);
	}

	character.m_RegionColor = sf::Color::Black;
	character.m_KingdomName = "";
	character.m_CurrentGold = 0;
	character.m_MaxArmySize = 0;
	character.m_RaisedArmySize = 0;
	character.m_OwnedRegionIDs.clear();
	character.m_CharacterTitle = Title::Unlanded;

	for (const CharacterID& ally : WarManager::get().getAlliances(characterID))
	{
		//breakAlliance(characterID, ally);
		WarManager::get().breakAlliance(characterID, ally);
	}

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
		if (character.m_CharacterTitle < child.m_CharacterTitle)
		{
			child.m_RegionColor = character.m_RegionColor;
		}
		if (!child.m_IsPlayerControlled)
		{
			child.m_IsPlayerControlled = character.m_IsPlayerControlled;
		}

		if (child.m_IsPlayerControlled)
		{
			m_PlayerCharacterID = child.m_CharacterID;
			m_PlayerCharacter = &child;

			AIManager::get().deactivateAI(child.m_CharacterID);
			UIManager::get().m_DateBar->updateOwnerColor(child.m_RegionColor);
			UIManager::get().m_StatBar->updateOwnerColor(child.m_RegionColor);
		}
		if (child.m_OwnedRegionIDs.empty())
		{
			for (unsigned int ownedRegionID : character.m_OwnedRegionIDs)
			{
				addRegion(child.m_CharacterID, ownedRegionID);
				Map::get().setRegionColor(ownedRegionID, child.m_RegionColor);
				Map::get().getRegionById(ownedRegionID).m_OwnerID = child.m_CharacterID;
			}

			updateTitle(child);
			UIManager::get().createUITextElement(Game::m_UIFont, child.m_CharacterID, child.m_KingdomName, child.m_OwnedRegionIDs);
		}
		else
		{
			for (unsigned int ownedRegionID : child.m_OwnedRegionIDs)
			{
				Map::get().setRegionColor(ownedRegionID, child.m_RegionColor);
			}
			for (unsigned int ownedRegionID : character.m_OwnedRegionIDs)
			{
				addRegion(child.m_CharacterID, ownedRegionID);
				Map::get().setRegionColor(ownedRegionID, child.m_RegionColor);
				Map::get().getRegionById(ownedRegionID).m_OwnerID = child.m_CharacterID;
			}
			updateTitle(child);
			UIManager::get().SetRealmNameOnText(child.m_CharacterID, child.m_KingdomName);
			for (unsigned int ownedRegionID : character.m_OwnedRegionIDs)
			{
				UIManager::get().AdjustOwnership(child.m_CharacterID, character.m_CharacterID, ownedRegionID);
			}
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
						Map::get().getRegionById(ownedRegionID).m_OwnerID = child.m_CharacterID;
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
				if (child.m_CharacterTitle < Title::Unlanded)
				{
					updateTitle(child);
					UIManager::get().SetRealmNameOnText(child.m_CharacterID, child.m_KingdomName);
					for (unsigned int childOwnedRegionID : child.m_OwnedRegionIDs)
					{
						UIManager::get().AdjustOwnership(childID, character.m_CharacterID, childOwnedRegionID);
					}
				}
				else
				{
					updateTitle(child);
					UIManager::get().createUITextElement(Game::m_UIFont, childID, child.m_KingdomName, child.m_OwnedRegionIDs);
				}
			}
		}
		Character& child = CharacterManager::get().getCharacter(character.m_Children.front());
		if (!child.m_IsPlayerControlled)
		{
			child.m_IsPlayerControlled = character.m_IsPlayerControlled;
		}
		if (child.m_IsPlayerControlled)
		{
			m_PlayerCharacterID = child.m_CharacterID;
			m_PlayerCharacter = &child;
			AIManager::get().deactivateAI(child.m_CharacterID);
			UIManager::get().m_DateBar->updateOwnerColor(child.m_RegionColor);
			UIManager::get().m_StatBar->updateOwnerColor(child.m_RegionColor);
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
					UIManager::get().createUITextElement(Game::m_UIFont, otherCharacter.m_CharacterID, otherCharacter.m_KingdomName, otherCharacter.m_OwnedRegionIDs);
					UIManager::get().AdjustOwnership(otherCharacter.m_CharacterID, character.m_CharacterID, ownedRegionID);
					break;
				}
			}
		}
	}

	character.m_IsPlayerControlled = false;
	UIManager::get().SetRealmTextAsConquered(character.m_CharacterID);

	if (character.m_UnitEntity != INVALID_UNIT_ID)
	{
		UnitManager::get().dismissUnit(character.m_UnitEntity);
		character.m_UnitEntity = INVALID_UNIT_ID;
	}
}

bool CharacterManager::chancePerPercent(float weight)
{
	float f = rand() * 1.0f / RAND_MAX;
	//float vv = weight / 10.0f;
	return f < weight;
}

bool CharacterManager::ownsRegion(CharacterID ID, int regionIndex)
{
	if (regionIndex == INVALID_REGION_ID)
	{
		return false;
	}

	for (auto regionID : getCharacter(ID).m_OwnedRegionIDs)
	{
		if(regionID == (unsigned int)regionIndex)
		{
			return true;
		}
	}

	return false;
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

	if (region.m_BuildingSlots[buildingSlot].m_BuildingId != INVALID_BUILDING_ID)
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
		if (buildingSlot.m_BuildingId == INVALID_BUILDING_ID)
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
				if (buildingSlot.m_BuildingId == INVALID_BUILDING_ID)
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
	if (getCharacter(spouse).m_Dead || getCharacter(character).m_Dead
	 || getCharacter(spouse).m_Spouse != INVALID_CHARACTER_ID || getCharacter(character).m_Spouse != INVALID_CHARACTER_ID
	 || getCharacter(character).m_Gender == getCharacter(spouse).m_Gender
	 || Time::m_GameDate.getAge(getCharacter(character).m_Birthday) < CharacterConstants::m_AgeOfConsent || Time::m_GameDate.getAge(getCharacter(spouse).m_Birthday) < CharacterConstants::m_AgeOfConsent)
	{
		if (getCharacter(character).m_IsPlayerControlled)
		{
			UIManager::get().createUIEventElement(spouse, character, UIType::MarriageDeclined);
		}
		return;
	}

	if (!getCharacter(spouse).m_IsPlayerControlled)
	{
		if (AIManager::get().handleRecieveMarriageRequest(spouse, character))
		{
			onMarriage(character, spouse);
			WarManager::get().createAlliance(character, spouse);
			if (getCharacter(character).m_IsPlayerControlled)
			{
				UIManager::get().createUIEventElement(spouse, character, UIType::MarriageAccepted);
			}
		}
		else if(getCharacter(character).m_IsPlayerControlled)
		{
			UIManager::get().createUIEventElement(spouse, character, UIType::MarriageDeclined);
		}
	}
	else
	{
		UIManager::get().createUIEventElement(character, spouse, UIType::MarriageRequest);
	}
}
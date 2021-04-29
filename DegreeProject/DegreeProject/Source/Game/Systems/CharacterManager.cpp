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
#include <sstream>
#include <json.hpp>
using json = nlohmann::json;
#include <mutex>
#include "Game/DiplomacyManager.h"

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
		m_CharacterCreator.createRandomUnlandedCharacter(m_CharacterPool, CharacterConstants::m_MinAgeNewUnlandedChar, CharacterConstants::m_MaxAgeNewUnlandedChar);
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
		opinion += CharacterConstants::m_AllyOpinion;
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
	for (auto ID : DiplomacyManager::get().getAlliances(character))
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
	DiplomacyManager& warManager = DiplomacyManager::get();
	AIManager* aiManager = &AIManager::get();

	for (auto ID : DiplomacyManager::get().getAlliances(character))
	{
		if (!getCharacter(ID).m_IsPlayerControlled)
		{
			if (aiManager->handleWarCallRequest(character, ID, war))
			{
				if (character == warManager.getAttacker(war))
				{
					for (auto& defender : warManager.getWar(war)->m_Defenders)
					{
						if (warManager.isAllyOf(war, ID, defender))
						{
							warManager.breakAlliance(ID, defender);
						}
					}

					aiManager->getWarmindOfCharacter(ID).m_Active = true;
					warManager.addAttacker(war, ID);
				}

				else if (character == warManager.getDefender(war))
				{
					for (auto& attacker : warManager.getWar(war)->m_Attackers)
					{
						if (warManager.isAllyOf(war, ID, attacker))
						{
							warManager.breakAlliance(ID, attacker);
						}
					}

					aiManager->getWarmindOfCharacter(ID).m_Active = true;
					warManager.addDefender(war, ID);
				}
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
			DiplomacyManager::get().createAlliance(sender, reciever);
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
	DiplomacyManager::get().endWar(DiplomacyManager::get().getWarAgainst(sender, reciever)->getHandle(), sender);
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
				DiplomacyManager::get().endWar(DiplomacyManager::get().getWarAgainst(sender, reciever)->getHandle(), INVALID_CHARACTER_ID);
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
		Unit& characterUnit = UnitManager::get().getUnitOfCharacter(character.m_CharacterID);
		
		if (characterUnit.m_Raised)
		{
			incomingGold -= (characterUnit.m_RepresentedForce * 0.1f); // Todo: Declare army cost somewhere
		}

		character.m_Income = incomingGold; // Todo: Change to prediction for upcoming month instead of showing last month.
		character.m_CurrentGold += incomingGold;
	}
}

void CharacterManager::killCharacter(CharacterID characterID)
{
	Character& character = getCharacter(characterID);
	DiplomacyManager& warManager = DiplomacyManager::get();

	for (auto& region : getCharacter(characterID).m_OwnedRegionIDs)
	{
		DiplomacyManager::get().invalidateWarsForRegion(region);
	}

	for (auto& war : warManager.getWarHandlesOfCharacter(characterID))
	{
		War* currentWar = warManager.getWar(war);

		if (warManager.isAttacker(war, characterID))
		{
			warManager.invalidateWar(war);
		}

		if (warManager.isAllyOf(war, characterID, warManager.getAttacker(war)))
		{
			warManager.removeAllyFromWar(characterID, currentWar->getHandle());
		}

		else if (warManager.isAllyOf(war, characterID, warManager.getDefender(war)))
		{
			warManager.removeAllyFromWar(characterID, war);
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
	character.m_DeathDate = Time::m_GameDate.m_Date;
	character.m_DeathDate.m_Month += 1;
	character.m_TitleAtDeath = character.m_CharacterTitle;
	character.m_ColorAtDeath = character.m_RegionColor;
	character.m_AgeAtDeath = Time::m_GameDate.getAge(character.m_Birthday);

	if (character.m_CharacterTitle != Title::Unlanded && character.m_OwnedRegionIDs.size() > 0)
	{
		UIManager::get().createUIEventElement(characterID, characterID, UIType::Death);
		handleInheritance(character);
		UIManager::get().SetRealmTextAsConquered(character.m_CharacterID);
	}

	character.m_RegionColor = sf::Color::Black;
	character.m_KingdomName = "";
	character.m_CurrentGold = 0;
	character.m_MaxArmySize = 0;
	character.m_OwnedRegionIDs.clear();
	character.m_CharacterTitle = Title::Unlanded;
	character.m_IsPlayerControlled = false;

	for (const CharacterID& ally : DiplomacyManager::get().getAlliances(characterID))
	{
		//breakAlliance(characterID, ally);
		DiplomacyManager::get().breakAlliance(characterID, ally);
	}

	if (character.m_Spouse != INVALID_CHARACTER_ID)
	{
		getCharacter(character.m_Spouse).m_Spouse = INVALID_CHARACTER_ID;
		character.m_Spouse = INVALID_CHARACTER_ID;
	}
}

std::vector<CharacterID> CharacterManager::getAliveChildren(CharacterID characterID)
{
	std::vector<CharacterID> returnVector;
	for (const CharacterID& child : getCharacter(characterID).m_Children)
	{
		if (!getCharacter(child).m_Dead)
		{
			returnVector.push_back(child);
		}
	}

	return returnVector;
}

void CharacterManager::updateTitleAndUIText(Character& character)
{
	bool createNewUI = character.m_CharacterTitle >= Title::Unlanded;
	if (character.m_OwnedRegionIDs.size() == 0)
	{
		character.m_CharacterTitle = Title::Unlanded;
		character.m_KingdomName = "Unlanded ";
		return;
	}

	std::stringstream stream;
	if (character.m_OwnedRegionIDs.size() == 1)
	{
		character.m_CharacterTitle = Title::Baron;
		stream << "Barony of ";
	}
	else if (character.m_OwnedRegionIDs.size() == 2 || character.m_OwnedRegionIDs.size() == 3)
	{
		character.m_CharacterTitle = Title::Count;
		stream << "County of ";
	}
	else if (character.m_OwnedRegionIDs.size() == 4)
	{
		character.m_CharacterTitle = Title::Duke;
		stream << "Duchy of ";
	}
	else if (character.m_OwnedRegionIDs.size() <= 6)
	{
		character.m_CharacterTitle = Title::King;
		stream << "Kingdom of ";
	}
	else if (character.m_OwnedRegionIDs.size() >= 9)
	{
		character.m_CharacterTitle = Title::Emperor;
		stream << "Empire of ";
	}
	stream << Map::get().getRegionById(character.m_OwnedRegionIDs.front()).m_RegionName;
	character.m_KingdomName = stream.str();

	if (createNewUI)
	{
		UIManager::get().createUITextElement(Game::m_UIFont, character.m_CharacterID, character.m_KingdomName, character.m_OwnedRegionIDs);
	}
	else
	{
		UIManager::get().SetRealmNameOnText(character.m_CharacterID, character.m_KingdomName);
	}
}

void CharacterManager::handleInheritance(Character& character)
{
	std::vector<CharacterID> aliveChildren = getAliveChildren(character.m_CharacterID);

	if (aliveChildren.size() <= 0)
	{
		for (unsigned int ownedRegionID : character.m_OwnedRegionIDs)
		{
			Character& newCharacter = getCharacter(m_CharacterCreator.createRandomUnlandedCharacter(m_CharacterPool, CharacterConstants::m_MinAgeNewUnlandedChar, CharacterConstants::m_MaxAgeNewUnlandedChar));
			newCharacter.m_RegionColor = sf::Color((sf::Uint8)std::rand(), (sf::Uint8)std::rand(), (sf::Uint8)std::rand());
			addRegion(newCharacter.m_CharacterID, ownedRegionID);
			updateTitleAndUIText(newCharacter);
			UIManager::get().AdjustOwnership(newCharacter.m_CharacterID, character.m_CharacterID, ownedRegionID);
		}
		return;
	}

	if (character.m_OwnedRegionIDs.size() > 0)
	{
		// Todo: Get alive children instead of children
		size_t regionsByChild = character.m_OwnedRegionIDs.size() / aliveChildren.size();
		size_t giveOneMoreToChildren = character.m_OwnedRegionIDs.size() % aliveChildren.size();
		float goldByChild = character.m_CurrentGold / aliveChildren.size();

		if (aliveChildren.size() > character.m_OwnedRegionIDs.size())
		{
			regionsByChild = 1;
			giveOneMoreToChildren = 0;
		}

		size_t currentChild = 0;
		size_t currentChildInheritedRegions = 0;
		Character* currentChildCharacter = &getCharacter(aliveChildren[currentChild]);
		currentChildCharacter->m_CurrentGold += goldByChild;
		bool childGotOneMore = false;
		if (currentChildCharacter->m_RegionColor == sf::Color::Black)
		{
			currentChildCharacter->m_RegionColor = sf::Color((sf::Uint8)std::rand(), (sf::Uint8)std::rand(), (sf::Uint8)std::rand());
		} // Todo: Create character color on birth

		if (!currentChildCharacter->m_IsPlayerControlled)
		{
			currentChildCharacter->m_IsPlayerControlled = character.m_IsPlayerControlled;
		}
		if (currentChildCharacter->m_IsPlayerControlled && m_PlayerCharacterID != currentChildCharacter->m_CharacterID)
		{
			m_PlayerCharacterID = currentChildCharacter->m_CharacterID;
			m_PlayerCharacter = currentChildCharacter;
			AIManager::get().deactivateAI(currentChildCharacter->m_CharacterID);
			UIManager::get().m_DateBar->updateOwnerColor(currentChildCharacter->m_RegionColor);
			UIManager::get().m_StatBar->updateOwnerColor(currentChildCharacter->m_RegionColor);
		}

		for (size_t i = 0; i < character.m_OwnedRegionIDs.size(); ++i)
		{
			addRegion(currentChildCharacter->m_CharacterID, character.m_OwnedRegionIDs[i]);
			currentChildInheritedRegions++;

			if (currentChildInheritedRegions >= regionsByChild && i < character.m_OwnedRegionIDs.size() - 1)
			{
				if (giveOneMoreToChildren <= currentChild || childGotOneMore)
				{
					updateTitleAndUIText(*currentChildCharacter);
					UIManager::get().AdjustOwnerships(currentChildCharacter->m_CharacterID, character.m_CharacterID, currentChildCharacter->m_OwnedRegionIDs);

					++currentChild;
					currentChildCharacter = &getCharacter(aliveChildren[currentChild]);
					currentChildInheritedRegions = 0;
					currentChildCharacter->m_CurrentGold += goldByChild;
					childGotOneMore = false;

					if (currentChildCharacter->m_RegionColor == sf::Color::Black)
					{
						currentChildCharacter->m_RegionColor = sf::Color((sf::Uint8)std::rand(), (sf::Uint8)std::rand(), (sf::Uint8)std::rand());
					} // Todo: Create character color on birth
				}
				else
				{
					childGotOneMore = true;
				}
			}
		}
		
		updateTitleAndUIText(*currentChildCharacter);
		UIManager::get().AdjustOwnerships(currentChildCharacter->m_CharacterID, character.m_CharacterID, currentChildCharacter->m_OwnedRegionIDs);
	}

	if (character.m_UnitEntity != INVALID_UNIT_ID)
	{
		UnitManager::get().dismissUnit(character.m_UnitEntity);
		character.m_UnitEntity = INVALID_UNIT_ID;
	}
}

bool CharacterManager::chancePerPercent(float weight)
{
	float f = rand() * 1.0f / RAND_MAX;
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

	if (region.m_OwnerID != characterId)
	{
		return;
	}

	if (region.m_BuildingSlots[buildingSlot].m_BuildingId != INVALID_BUILDING_ID)
	{
		return;
	}

	if (building.m_Cost > character.m_CurrentGold)
	{
		if (character.m_IsPlayerControlled)
		{
			UIManager::get().createUIEventElement(m_PlayerCharacterID, m_PlayerCharacterID, UIType::CannotAffordMessage, (float)building.m_Cost);
		}
		return;
	}

	if (character.m_IsPlayerControlled)
	{
		UIManager::get().createUIEventElement(m_PlayerCharacterID, m_PlayerCharacterID, UIType::BuildingMessage, (float)building.m_Cost, building.m_Name, region.m_RegionName);
	}

	Map::get().startConstructionOfBuilding(buildingId, buildingSlot, regionId);
	character.m_CurrentGold -= building.m_Cost;
}

void CharacterManager::addRegion(const CharacterID characterId, const unsigned int regionId)
{
	Character& character = getCharacter(characterId);
	character.m_OwnedRegionIDs.push_back(regionId);

	MapRegion& region = Map::get().getRegionById(regionId);
	Map::get().setRegionColor(regionId, character.m_RegionColor);
	character.m_MaxArmySize += region.m_ManPower;
	region.m_OwnerID = characterId;

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
			updateTitleAndUIText(character);

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


	if (character.m_OwnedRegionIDs.empty())
	{
		DiplomacyManager* diplomacyManager = &DiplomacyManager::get();
		auto& truces = diplomacyManager->getTruces(character.m_CharacterID);
		diplomacyManager->endTruces(truces);

		for (auto& ally : diplomacyManager->getAlliances(character.m_CharacterID))
		{
			diplomacyManager->breakAlliance(character.m_CharacterID, ally);
		}
	}
}

void CharacterManager::onMarriage(CharacterID sender, CharacterID reciever)
{
	getCharacter(sender).m_Spouse = reciever;
	getCharacter(sender).m_TotalSpouses.push_back(reciever);
	getCharacter(reciever).m_Spouse = sender;
	getCharacter(reciever).m_TotalSpouses.push_back(sender);
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
			DiplomacyManager::get().createAlliance(character, spouse);
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
#pragma once
#include <Game\Data\Character.h>

enum class Action
{
	War,
	Upgrade_Settlement,
	Marriage,
	Seek_Alliance,
	NONE
};

struct Personality
{
	Personality() {};
	std::string m_PersonalityName;
	float m_DeclareWarModifier = 0.0f;

	Personality(std::string personalityName, float declareWarModifier) : m_PersonalityName(personalityName), m_DeclareWarModifier(declareWarModifier) {}
};

struct AIData
{
	AIData() {};

	Action m_CurrentAction = Action::NONE;
	CharacterID m_OwnerID = INVALID_CHARACTER_ID;
	Action m_LastAction = Action::NONE;
	Date m_LastMarriageOfferDate;
	Personality m_Personality;

	std::vector<std::pair<float, Action>> m_Evaluations;

	int m_TickPrio = -1;
	int m_SettlementToUpgrade = -1;
	CharacterID m_PotentialSpouseID = INVALID_CHARACTER_ID;
	CharacterID m_PotentialAllyID = INVALID_CHARACTER_ID;
};
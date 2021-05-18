#include "CharacterPool.h"

#include "Game/Systems/CharacterManager.h"

Character& CharacterPool::Rent()
{
	if (m_Pool.size() <= 0)
	{
		Init(m_GrowSize);
	}

	Character& c = m_Pool.top();
	m_Pool.pop();

	return c;
}

void CharacterPool::Push(Character& character)
{
	character.m_KingdomName = "";
	character.m_Name = "";
	character.m_CharacterID = INVALID_CHARACTER_ID;

	m_Pool.push(character);
}

void CharacterPool::Init(size_t amount)
{
	for (size_t i = 0; i < amount; ++i)
	{
		Character newChar;

		newChar.m_CharacterID = INVALID_CHARACTER_ID;
		newChar.m_CharacterTitle = Title::Baron;

		newChar.m_OwnedRegionIDs = std::vector<unsigned int>();
		newChar.m_KingdomName = "";

		newChar.m_MaxArmySize = 0;
		newChar.m_CurrentMaxArmySize = 0;

		newChar.m_CurrentGold = 0;

		newChar.m_IsPlayerControlled = false;

		newChar.m_RegionColor = sf::Color::White;

		newChar.m_Name = "";

		m_Pool.push(newChar);
	}
}

CharacterPool::CharacterPool(size_t initSize, size_t growBy)
	: m_GrowSize(growBy)
{
	Init(initSize);
}

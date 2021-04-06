#pragma once

#include <stack>

#include "Game/Data/Character.h"

class CharacterPool
{
public:
	Character& Rent();
	void Push(Character& character);
	void Init(size_t amount);

	CharacterPool(size_t initSize, size_t growBy);

private:
	std::stack<Character> m_Pool;
	size_t m_GrowSize = 1;
};
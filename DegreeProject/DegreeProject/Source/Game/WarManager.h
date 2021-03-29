#pragma once
#include <vector>
#include <Game\War.h>
#include "Game/Components/CharacterComponent.h"

class WarManager
{
public:
	WarManager() {};

	//Returns handle to war
	int createWar(CharacterComponent& attacker, CharacterComponent& defender, int warGoalRegion);
	void endWar(int warHandle);
	War* getWar(int handle);
	War* getWarAgainst(CharacterComponent& character, CharacterComponent& enemy);

	inline static WarManager& get()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = new WarManager();
		}

		return *m_Instance;
	}

private:
	int m_Warhandle = 0;
	std::vector<std::pair<int, War>> m_Wars = std::vector<std::pair<int, War>>();
	static WarManager* m_Instance;
};

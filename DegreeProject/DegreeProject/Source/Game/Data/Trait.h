#pragma once

#include <string>

struct Trait
{
	std::string m_TraitName;
	int m_Attraction;
	int m_Fertility;
	int m_GeneralOpinion;
	bool m_Inheritable;
	bool m_CanSpawnWith;

	Trait(std::string traitName)
	{
		m_TraitName = traitName;
	}
};
#pragma once

namespace CharacterConstants
{
	static const size_t m_PoolInitSize = 20000;
	static const size_t m_PoolGrowSize = 10000;
	static const size_t m_UnlandedCharactersAtStart = 200;

	static const unsigned int m_MinAgeNewUnlandedChar = 1;
	static const unsigned int m_MaxAgeNewUnlandedChar = 72;

	static const unsigned int m_LowerBaseFertility = 2;
	static const unsigned int m_UpperBaseFertility = 15;
	static const float m_OneOverOneHundred = 1.0f / 100.0f;
	static const unsigned int m_AgeOfConsent = 16;
	static const unsigned int m_FertilityBarrenAge = 60;
	static const unsigned int m_FertilityBarrenSmoother = 59;
	static const unsigned int m_FertilityCurveSteepness = 75000;
	static const unsigned int m_PregnancyDays = 280;
	static const unsigned int m_MaxPrematureBirth = 14;
	static const unsigned int m_MaxLateBirth = 14;
	static const float m_BirthChance = 0.13f;
	static const float m_OneOverFertilityCurveSteep = 1.0f / m_FertilityCurveSteepness;

	static const int m_SpouseOpinion = 50;
	static const int m_ParentOpinion = 80;
	static const int m_ChildOpinion = 80;
	static const int m_AllyOpinion = 40;

	static const float m_InheritTraitChance = 0.4f;
	static const float m_NewTraitChance = 0.05f;
}
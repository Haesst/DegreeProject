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

	static const char* m_MalePortraitPath = "Assets/Graphics/Portraits/Male/(";
	static const char* m_FemalePortraitPath = "Assets/Graphics/Portraits/Female/(";
	static const char* m_YoungMalePortraitPath = "Assets/Graphics/Portraits/Male/Young(";
	static const char* m_YoungFemalePortraitPath = "Assets/Graphics/Portraits/Female/Young(";
	static const char* m_ChildMalePortraitPath = "Assets/Graphics/Portraits/Male/Child(";
	static const char* m_ChildFemalePortraitPath = "Assets/Graphics/Portraits/Female/Child(";
	static const char* m_PortraitPathEnding = ").png";
	static const unsigned int m_NumberOfFemalePortraits = 121;
	static const unsigned int m_NumberOfMalePortraits = 151;
	static const unsigned int m_NumberOfYoungFemalePortraits = 121;
	static const unsigned int m_NumberOfYoungMalePortraits = 151;
	static const unsigned int m_NumberOfChildFemalePortraits = 121;
	static const unsigned int m_NumberOfChildMalePortraits = 151;
}
#pragma once

#include <vector>;

struct CharacterNamePool
{
public:
	static char* getMaleName();
	static char* getFemaleName();
private:
	const static std::vector<char*> m_MaleNames;
	const static std::vector<char*> m_FemaleNames;
};
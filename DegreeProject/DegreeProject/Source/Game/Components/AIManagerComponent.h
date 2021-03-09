#pragma once

#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

#include "ECS/Component.h"
#include "Engine/FileWatcher.h"
#include "Game/HotReloader.h"

struct Personality
{
	const char* m_PersonalityName;
	float m_DeclareWarModifier = 0.0f;

	Personality(const char* personalityName, float declareWarModifier)
	{
		m_PersonalityName = personalityName;
		m_DeclareWarModifier = declareWarModifier;
	}
};

struct AIManagerComponent : public Component
{
	std::vector<Personality> m_Personalities;

	AIManagerComponent()
	{
		LoadPersonalities("Assets/Data/AI/AIPersonalities.json");
		HotReloader::Get()->SubscribeToFileChange("Assets/Data/AI/AIPersonalities.json", std::bind(&AIManagerComponent::OnFileChange, this, std::placeholders::_1, std::placeholders::_2));
	}

	void LoadPersonalities(const char* path)
	{
		std::ifstream file(path);
		json j;
		file >> j;

		m_Personalities.clear();

		for (auto& element : j)
		{
			const char* personalityName = element["debugName"].get<std::string>().c_str();
			float declareWarModifier = element["declareWarMod"].get<float>();

			m_Personalities.push_back({ personalityName, declareWarModifier });
		}
	}

	void OnFileChange(std::string path, FileStatus status)
	{
		if (status != FileStatus::Modified)
		{
			return;
		}

		LoadPersonalities(path.c_str());
	}
};

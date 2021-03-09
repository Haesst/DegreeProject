#pragma once

#include <fstream>
#include <json.hpp>
using json = nlohmann::json;
#include <mutex>

#include "Engine/FileWatcher.h"
#include "Game/HotReloader.h"
#include "Engine/Log.h"

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

struct AIManager
{
	std::vector<Personality> m_Personalities;
	mutable std::mutex m_PersonalityMtx;

	AIManager()
	{
		LoadPersonalities("Assets\\Data\\AI\\AIPersonalities.json");
		HotReloader::Get()->SubscribeToFileChange("Assets\\Data\\AI\\AIPersonalities.json", std::bind(&AIManager::OnFileChange, this, std::placeholders::_1, std::placeholders::_2));
	}

	void LoadPersonalities(const char* path)
	{
		std::ifstream file(path);
		json j;
		file >> j;

		std::lock_guard<std::mutex> lock{ m_PersonalityMtx };
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

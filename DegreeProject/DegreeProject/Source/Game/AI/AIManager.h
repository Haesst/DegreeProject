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
		loadPersonalities("Assets\\Data\\AI\\AIPersonalities.json");
		HotReloader::get()->subscribeToFileChange("Assets\\Data\\AI\\AIPersonalities.json", std::bind(&AIManager::onFileChange, this, std::placeholders::_1, std::placeholders::_2));
	}

	void loadPersonalities(const char* path)
	{
		std::ifstream file(path);
		json j;
		file >> j;

		m_PersonalityMtx.lock();
		m_Personalities.clear();

		for (auto& element : j)
		{
			std::string personalityName = element["debugName"].get<std::string>();
			float declareWarModifier = element["declareWarMod"].get<float>();

			m_Personalities.push_back({ personalityName.c_str(), declareWarModifier });
		}
		m_PersonalityMtx.unlock();
	}

	void onFileChange(std::string path, FileStatus status)
	{
		if (status != FileStatus::Modified)
		{
			return;
		}

		loadPersonalities(path.c_str());
	}
};

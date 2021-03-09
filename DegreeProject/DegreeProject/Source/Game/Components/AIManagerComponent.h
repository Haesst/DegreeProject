//#pragma once
//
//#include <fstream>
//#include <json.hpp>
//using json = nlohmann::json;
//
//#include "ECS/Component.h"
//#include "Engine/FileWatcher.h"
//#include "Game/HotReloader.h"
//#include <Engine/Log.h>
//#include <mutex>
//
////extern std::mutex mtx;
//
//struct Personality
//{
//	const char* m_PersonalityName;
//	float m_DeclareWarModifier = 0.0f;
//
//	Personality(const char* personalityName, float declareWarModifier)
//	{
//		m_PersonalityName = personalityName;
//		m_DeclareWarModifier = declareWarModifier;
//	}
//};
//
//struct AIManagerComponent : public Component
//{
//	std::vector<Personality> m_Personalities;
//	mutable std::mutex m_PersonalityMtx;
//
//	AIManagerComponent()
//	{
//		LoadPersonalities("Assets\\Data\\AI\\AIPersonalities.json");
//		HotReloader::Get()->SubscribeToFileChange("Assets\\Data\\AI\\AIPersonalities.json", std::bind(&AIManagerComponent::OnFileChange, this, std::placeholders::_1, std::placeholders::_2));
//	}
//
//	AIManagerComponent& operator=(const AIManagerComponent& other)
//	{
//		m_Personalities = other.m_Personalities;
//		return *this;
//	}
//
//	void LoadPersonalities(const char* path)
//	{
//		if (!EntityManager::Get().IsEntityActive(m_EntityID))
//		{
//			return;
//		}
//
//		std::ifstream file(path);
//		json j;
//		file >> j;
//
//		LOG_INFO(path);
//
//		std::lock_guard<std::mutex> lock{ m_PersonalityMtx };
//		m_Personalities.clear();
//
//		for (auto& element : j)
//		{
//			const char* personalityName = element["debugName"].get<std::string>().c_str();
//			float declareWarModifier = element["declareWarMod"].get<float>();
//
//			m_Personalities.push_back({ personalityName, declareWarModifier });
//		}
//	}
//
//	void OnFileChange(std::string path, FileStatus status)
//	{
//		if (status != FileStatus::Modified)
//		{
//			return;
//		}
//
//		LoadPersonalities(path.c_str());
//	}
//};

#include "SystemManager.h"

SystemManager* SystemManager::m_Instance = nullptr;

SystemManager* SystemManager::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new SystemManager();
	}

	return m_Instance;
}

void SystemManager::start()
{
	for (auto& system : m_RegisteredSystems)
	{
		system.second->start();
	}
}

void SystemManager::update()
{
	for (auto& system : m_RegisteredSystems)
	{
		system.second->update();
	}
}

void SystemManager::render()
{
	for (auto& system : m_RegisteredSystems)
	{
		system.second->render();
	}
}
//
//template<typename T>
//void SystemManager::registerSystem()
//{
//	const GameSystemID type = getSystemType<T>();
//	assert(m_RegisteredSystems.count(type) == (size_t)0 && "System already registered");
//	auto system = new T();
//
//	m_RegisteredSystems[type] = std::move(system);
//}

#pragma once
#include <map>
#include <memory>

using GameSystemID = size_t;

class GameSystem
{
public:
	virtual void start() {};
	virtual void update() {};
	virtual void render() {};
};

class SystemManager
{
public:
	void start();
	void update();
	void render();

	static SystemManager* get();

	template<typename T>
	void registerSystem()
	{
		const GameSystemID type = getSystemType<T>();
		assert(m_RegisteredSystems.count(type) == (size_t)0 && "System already registered");
		auto system = std::make_shared<T>();

		m_RegisteredSystems[type] = std::move(system);
	}

	inline static const GameSystemID getRuntimeGameSystemID()
	{
		static GameSystemID typeID = 0u;
		return typeID++;
	}

	// Attach typeid to system class and return it
	template<typename T>
	inline static const GameSystemID getSystemType() noexcept
	{
		ASSERT((std::is_base_of<GameSystem, T>::value && !std::is_same<GameSystem, T>::value), "INVALID TEMPLATE TYPE");
		static const SystemTypeID typeID = getRuntimeGameSystemID();

		return typeID;
	}

	template<typename T>
	GameSystem& getSystem() const
	{
		const GameSystemID systemType = getSystemType<T>();
		return m_RegisteredSystems.at(systemType).second;
	}

private:
	static SystemManager* m_Instance;
	std::map<GameSystemID, std::shared_ptr<GameSystem>> m_RegisteredSystems;
};
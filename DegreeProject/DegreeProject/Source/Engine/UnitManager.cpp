#include "UnitManager.h"

#include "Game/Data/Unit.h"
#include "Engine/Log.h"

UnitManager* UnitManager::m_Instance = nullptr;

void UnitManager::start()
{
}

void UnitManager::update()
{
	// Move unit
}

void UnitManager::render()
{
	// Render unit
}

UnitManager& UnitManager::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new UnitManager();
	}

	return *m_Instance;
}

void UnitManager::addUnit(CharacterID owner)
{
	Unit newUnit;
	newUnit.m_Owner = owner;

	m_Units.push_back(newUnit);
}

Unit UnitManager::getUnitOfCharacter(CharacterID character)
{
	for (auto& unit : m_Units)
	{
		if (unit.m_Owner == character)
		{
			return unit;
		}
	}

	ASSERT(false, "Given character doesn't have any units.");
	return Unit();
}

void UnitManager::giveUnitPath(std::vector<Vector2DInt> path)
{
}
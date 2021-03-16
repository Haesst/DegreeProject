#pragma once
#include "ECS/Component.h"
#include "Game/Components/CharacterComponent.h"
#include <string>
#include <Engine\AssetHandler.h>

struct UnitComponent : public Component
{
	int m_RepresentedForce = 0;
	Vector2DInt m_CurrentMapPosition;
	bool m_Raised = false;
	std::list<Vector2DInt> m_CurrentPath = std::list<Vector2DInt>();

	UnitComponent()
	{

	}

	UnitComponent(int representedForce, Vector2DInt position)
	{
		m_CurrentMapPosition = position;
		m_RepresentedForce = representedForce;
	}
};

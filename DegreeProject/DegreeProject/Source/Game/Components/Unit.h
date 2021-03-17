#pragma once
#include "ECS/Component.h"
#include "Game/Components/CharacterComponent.h"
#include <string>
#include <Engine\AssetHandler.h>

struct UnitComponent : public Component
{
	int m_RepresentedForce = 0;
	bool m_Raised = false;
	std::list<Vector2DInt> m_CurrentPath = std::list<Vector2DInt>();

	UnitComponent()
	{

	}

	UnitComponent(int representedForce)
	{
		m_RepresentedForce = representedForce;
	}
};

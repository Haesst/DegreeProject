#pragma once

#include "Engine/Vector2D.h"
#include "Game/Data/UIType.h"

using UIWindowID = size_t;

#pragma warning(push)
#pragma warning(disable: 26812)
struct UIWindow
{
	UIWindowID m_UIWindowID = 0;
	Vector2D m_Position = Vector2D();
	Vector2D m_Size = Vector2D();
	UIType m_Type = UIType::CharacterWindow;
};
#pragma warning(pop)
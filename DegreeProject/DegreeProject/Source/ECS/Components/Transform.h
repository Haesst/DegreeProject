#pragma once

#include "../Component.h"
#include "../../Engine/Vector2D.h"

struct Transform : public Component
{
	Vector2D m_Position = Vector2D().zero();
	Vector2D m_Scale = Vector2D().one();
	float m_Rotation = 0.0f;

	Transform() = default;

	Transform(float x, float y)
		: m_Position(Vector2D(x, y)), m_Scale(Vector2D().one())
	{}

	Transform(float x, float y, float scaleX, float scaleY)
		: m_Position(Vector2D(x, y)), m_Scale(Vector2D(scaleX, scaleY))
	{}

	Transform(float x, float y, float scaleX, float scaleY, float rotation)
		: m_Position(Vector2D(x, y)), m_Scale(Vector2D(scaleX, scaleY)), m_Rotation(rotation)
	{}

	void translate(Vector2D v)
	{
		m_Position += v;
	}

	virtual ~Transform() = default;
};
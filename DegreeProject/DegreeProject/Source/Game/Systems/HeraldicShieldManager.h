#pragma once

#include <map>
#include <SFML/Graphics.hpp>

#include "Engine/Vector2D.h"
#include "Game/Data/HeraldicShield.h"

class HeraldicShieldManager
{
public:
	static void initialize();
	static HeraldicShield generateRandomShield();

	static void renderShield(const HeraldicShield& shield, const Vector2D& position);

private:
	static void loadBaseShield();
	static void loadAllPatterns();
	static sf::Color generateRandomColor();

private:
	static std::map<size_t, sf::Sprite> m_PatternSprites;
	static std::map<size_t, sf::Texture> m_PatternTextures;
	static sf::Sprite m_ShieldBaseSprite;
	static sf::Texture m_ShieldBaseTexture;
	static sf::Shader m_ShieldShader;
	static sf::RenderStates m_ShieldRenderStates;
};
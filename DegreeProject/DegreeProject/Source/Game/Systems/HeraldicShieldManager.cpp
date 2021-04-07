#include "HeraldicShieldManager.h"

#include "Engine/Window.h"
#include "Engine/AssetHandler.h"
#include "Game/Map/Map.h"

std::map<size_t, sf::Sprite> HeraldicShieldManager::m_PatternSprites;
std::map<size_t, sf::Texture> HeraldicShieldManager::m_PatternTextures;
sf::Sprite HeraldicShieldManager::m_ShieldBaseSprite;
sf::Texture HeraldicShieldManager::m_ShieldBaseTexture;

sf::Shader HeraldicShieldManager::m_ShieldShader;
sf::RenderStates HeraldicShieldManager::m_ShieldRenderStates;

void HeraldicShieldManager::initialize()
{
	loadBaseShield();
	loadAllPatterns();

	m_ShieldShader.loadFromFile("Assets\\Shaders\\ShieldShader.frag", sf::Shader::Fragment);
	m_ShieldRenderStates.shader = &m_ShieldShader;
}

void HeraldicShieldManager::loadBaseShield()
{
	m_ShieldBaseTexture = AssetHandler::get().getTextureAtPath("Assets\\Graphics\\HeraldicShields\\HeraldicShieldsBase.png");
	m_ShieldBaseSprite.setTexture(m_ShieldBaseTexture, true);
}

void HeraldicShieldManager::loadAllPatterns()
{

}

sf::Color HeraldicShieldManager::generateRandomColor()
{
	float r = (rand() / (RAND_MAX / 250.0f));
	float g = (rand() / (RAND_MAX / 250.0f));
	float b = (rand() / (RAND_MAX / 250.0f));
	
	return sf::Color(r, g, b);
}

HeraldicShield HeraldicShieldManager::generateRandomShield()
{
	HeraldicShield shield;
	shield.m_BaseColor = generateRandomColor();
	shield.m_PatternColor = generateRandomColor();
	shield.m_PatternId = 0;
	return shield;
}

void HeraldicShieldManager::renderShield(const HeraldicShield& shield, const Vector2D& position)
{
	m_ShieldBaseSprite.setPosition({ position.x, position.y });
	m_ShieldShader.setUniform("u_Color", sf::Glsl::Vec4(shield.m_BaseColor));
	Window::getWindow()->draw(m_ShieldBaseSprite, &m_ShieldShader);

	if (shield.m_PatternId != 0)
	{
		m_ShieldShader.setUniform("u_Color", sf::Glsl::Vec4(shield.m_PatternColor));
		Window::getWindow()->draw(m_PatternSprites[shield.m_PatternId], &m_ShieldShader);
	}
}
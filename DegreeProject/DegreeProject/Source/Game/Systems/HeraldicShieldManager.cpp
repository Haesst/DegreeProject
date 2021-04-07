#include "HeraldicShieldManager.h"
#include <filesystem>

#include "Engine/Window.h"
#include "Engine/Log.h"
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

	static size_t textureId = textureId++;

	for (auto& file : std::filesystem::recursive_directory_iterator("Assets\\Graphics\\HeraldicShields\\Patterns"))
	{
		m_PatternTextures[textureId] = AssetHandler::get().getTextureAtPath(file.path().string().c_str());
		m_PatternSprites[textureId].setTexture(m_PatternTextures[textureId], true);
	}
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
	int r = (rand() / (RAND_MAX / 255));
	int g = (rand() / (RAND_MAX / 255));
	int b = (rand() / (RAND_MAX / 255));
	
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

	m_ShieldShader.setUniform("u_Color", sf::Glsl::Vec4(shield.m_PatternColor));
	m_PatternSprites[shield.m_PatternId].setPosition({ position.x, position.y });
	Window::getWindow()->draw(m_PatternSprites[shield.m_PatternId], &m_ShieldShader);
}
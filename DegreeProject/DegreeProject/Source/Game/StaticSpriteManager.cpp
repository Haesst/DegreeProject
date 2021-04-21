#include "StaticSpriteManager.h"

StaticSpriteManager* StaticSpriteManager::m_Instance = nullptr;

StaticSpriteManager::~StaticSpriteManager()
{
	for (StaticSprite* staticSprite : m_StaticSprites)
	{
		delete staticSprite;
	}
	delete m_Instance;
}

StaticSpriteManager& StaticSpriteManager::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new StaticSpriteManager();
	}
	return *m_Instance;
}

void StaticSpriteManager::start()
{
	for (StaticSprite* staticSprite : m_StaticSprites)
	{
		staticSprite->start();
	}
}

void StaticSpriteManager::update()
{
	for (StaticSprite* staticSprite : m_StaticSprites)
	{
		staticSprite->update();
	}
}

void StaticSpriteManager::render()
{
	for (StaticSprite* staticSprite : m_StaticSprites)
	{
		staticSprite->render();
	}
}

void StaticSpriteManager::createStaticSprite(const char* path, int width, int height, Vector2D position)
{
	m_StaticSprites.push_back(new StaticSprite(path, width, height, position));
}

#pragma once

#include <SFML/Graphics.hpp>
#include "Engine/Vector2D.h"
#include "Game/StaticSprite.h"
#include "Game/Data/Types.h"

class StaticSpriteManager
{
public:
	~StaticSpriteManager();
	static StaticSpriteManager* get();
	void start();
	void update();
	void render();
	void createStaticSprite(const char* path, int width, int height, Vector2D position);
private:
	static StaticSpriteManager* m_Instance;
	std::vector<StaticSprite*> m_StaticSprites;
};
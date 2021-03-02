#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Engine/Vector2D.h"

class Window;
class AssetHandler;
class MapDrawer;

class Game
{
private:
	char* m_GameTitle = "GRAND STRATEGY GAME 2.5!";
	Vector2DInt m_Resolution = { 1920, 1080 };
private:
	Window* m_Window;
	//Window* m_Window;
	AssetHandler* m_AssetHandler;
	MapDrawer* m_MapDrawer;
	std::vector<sf::RectangleShape> m_Map;
	sf::SoundBuffer m_SoundBuffer;
	sf::Sound m_Sound;

public:
	~Game();
	void Init();
	void Run();

private:
	void InitWindow();
	void InitAssets();
	void InitSound();
	void InitMap();

	void InitSystems();
	void AddEntitys();
};
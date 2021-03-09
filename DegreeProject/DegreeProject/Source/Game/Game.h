#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Engine/Vector2D.h"
#include <mutex>

class Window;
class AssetHandler;
class MapDrawer;
class HotReloader;
class AIManager;

enum class FileStatus;

class Game
{
private:
	char* m_GameTitle = "GRAND STRATEGY GAME 2.5!";
	Vector2DInt m_Resolution = { 1920, 1080 };
private:
	Window* m_Window = nullptr;
	AssetHandler* m_AssetHandler = nullptr;
	MapDrawer* m_MapDrawer = nullptr;
	HotReloader* m_HotReloader = nullptr;
	AIManager* m_AIManager = nullptr;
	std::vector<sf::RectangleShape> m_Map;
	sf::SoundBuffer m_SoundBuffer;
	sf::Sound m_Sound;

public:
	~Game();
	void Init();
	void Run();

private:
	void InitWindow();
	void InitHotReloading();
	void InitAssets();
	void InitSound();
	void InitMap();

	void InitSystems();
	void AddEntitys();
	void InitAI();
};
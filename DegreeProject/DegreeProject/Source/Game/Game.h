#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Engine/Vector2D.h"
#include "Game/AI/AIManager.h"
#include <mutex>
#include <Game/GameDate.h>
#include "Game/Map/Map.h"

class Window;
class AssetHandler;
class HotReloader;

enum class FileStatus;

class Game
{
private:
	char* m_GameTitle = "GRAND STRATEGY GAME 2.5!";
	Vector2DInt m_Resolution = { 1920, 1080 };
	unsigned int m_NumberOfRegions = 17;
	Map m_Map;
private:
	Window* m_Window = nullptr;
	AssetHandler* m_AssetHandler = nullptr;
	HotReloader* m_HotReloader = nullptr;
	AIManager* m_AIManager = nullptr;
	sf::SoundBuffer m_SoundBuffer;
	sf::Sound m_Sound;
	GameDate m_GameDate;

public:
	~Game();
	void Init();
	void Run();

private:
	void InitWindow();
	void InitHotReloading();
	void InitAssets();
	void InitSound();

	void InitSystems();
	void AddEntitys();
	void InitAI();
};
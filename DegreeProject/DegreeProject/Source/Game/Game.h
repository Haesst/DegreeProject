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

enum class FileStatus;

class Game
{
private:
	char* m_GameTitle = "GRAND STRATEGY GAME 2.5!";
	Vector2DInt m_Resolution = { 1920, 1080 };
private:
	Window* m_Window;
	AssetHandler* m_AssetHandler;
	MapDrawer* m_MapDrawer;
	HotReloader* m_HotReloader;
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
	void Test(FileStatus status);
};
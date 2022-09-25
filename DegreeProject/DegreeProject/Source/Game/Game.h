#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Engine/Vector2D.h>
#include <mutex>
#include <Game/GameDate.h>
#include <Game/Map/Map.h>

class Window;
class AssetHandler;
class HotReloader;

enum class FileStatus;
enum class Title;

const static unsigned int BUILDING_SLOTS = 4;
const static unsigned int NUMBER_OF_REGIONS = 28;

class Game
{
private:
	char* m_GameTitle = "GRAND STRATEGY GAME 2.5!";
	Vector2DInt m_Resolution = { 1920, 1080 };

private:
	static Window* m_Window;
	AssetHandler* m_AssetHandler = nullptr;
	HotReloader* m_HotReloader = nullptr;
	sf::SoundBuffer m_SoundBuffer;
	CharacterID m_PlayerRandomCharacterID = INVALID_CHARACTER_ID;
	static sf::View m_GameView;
	sf::Vector2f m_GameViewCenter;

public:
	static sf::Font m_UIFont;

public:
	~Game();
	void init();
	void run();
	void static setGameViewCenter(sf::Vector2f viewCenter);

private:

	void initWindow();
	void initHotReloading();
	void initAssets();
	void initSound();

	void addEntitys();
	void fillEmptyRegionWithOwners();
	CharacterID addRandomEntityOwningRegion(std::vector<size_t> regions, bool playerControlled = false, unsigned int minAge = 17, unsigned int maxAge = 62);
	void createUI();
	void showSplashScreen(sf::Vector2f resolution, sf::View& uiView);
};

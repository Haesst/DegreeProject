#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Engine/Vector2D.h"
#include <mutex>
#include <Game/GameDate.h>
#include "Game/Map/Map.h"

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
	Window* m_Window = nullptr;
	AssetHandler* m_AssetHandler = nullptr;
	HotReloader* m_HotReloader = nullptr;
	sf::SoundBuffer m_SoundBuffer;
	sf::SoundBuffer m_BattleSoundBuffer;
	sf::Vector2f m_ViewCenter;
	const float m_Volume = 5.0f;
	CharacterID m_PlayerRandomCharacterID = INVALID_CHARACTER_ID;
public:
	static sf::Sound m_Sound;
	static sf::Sound m_BattleSound;
	static sf::Font m_UIFont;

public:
	~Game();
	void init();
	void run();

private:
	void initWindow();
	void initHotReloading();
	void initAssets();
	void initSound();

	void addEntitys();
	void fillEmptyRegionWithOwners();
	CharacterID addRandomEntityOwningRegion(std::vector<size_t> regions, bool playerControlled = false, unsigned int minAge = 17, unsigned int maxAge = 62);
};
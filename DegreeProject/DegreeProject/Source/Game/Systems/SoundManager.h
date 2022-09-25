#pragma once

#include <SFML/Audio.hpp>

class AssetHandler;

class SoundManager
{
public:
	SoundManager();
	~SoundManager();
	static SoundManager& get();
	void init(AssetHandler* assethandler);
	void update();
	void playRandomMusic();
	void pauseCurrentMusic();
	void resumeCurrentMusic();
	void stopCurrentMusic();
	void playBattleMusic();
	void stopBattleMusic();
	void playDefeatMusic();
	void playVictoryMusic();

private:

	static SoundManager* m_Instance;
	std::vector<sf::Music*> m_Music;
	sf::Music m_BattleMusic;
	sf::Music m_VictoryMusic;
	sf::Music m_DefeatMusic;
	sf::Music* m_CurrentMusic = nullptr;
	const float m_Volume = 5.0f;

	void CheckIfPlayNextSong();
};

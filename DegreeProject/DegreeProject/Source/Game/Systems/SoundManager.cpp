#include <Game/Systems/SoundManager.h>
#include <Engine/AssetHandler.h>
#include <filesystem>
#include <sstream>

SoundManager* SoundManager::m_Instance = nullptr;

SoundManager::SoundManager() { }

SoundManager::~SoundManager()
{
	for (sf::Music* music : m_Music)
	{
		delete music;
	}
	delete m_Instance;
}

SoundManager& SoundManager::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new SoundManager();
	}
	return *m_Instance;
}

void SoundManager::init(AssetHandler* assethandler)
{
	unsigned int i = 0;
	std::stringstream stream;
	const char* musicPath = "Assets/Audio/Music";
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(musicPath))
	{
		m_Music.push_back(new sf::Music());
		stream << entry.path().string();
		if (m_Music.size() > 0)
		{
			if (assethandler->openMusicFile(stream.str().c_str(), *m_Music[i]))
			{
				m_Music[i]->setVolume(m_Volume);
			}
		}
		stream.str(std::string());
		stream.clear();
		i++;
	}
	if (assethandler->openMusicFile("Assets/Audio/Battle/Battle.wav", m_BattleMusic))
	{
		m_BattleMusic.setLoop(true);
		m_BattleMusic.setVolume(m_Volume);
	}
	if (assethandler->openMusicFile("Assets/Audio/Ending/Win/Victory.wav", m_VictoryMusic))
	{
		m_VictoryMusic.setLoop(true);
		m_VictoryMusic.setVolume(m_Volume);
	}
	if (assethandler->openMusicFile("Assets/Audio/Ending/Loss/Defeat.wav", m_DefeatMusic))
	{
		m_DefeatMusic.setLoop(true);
		m_DefeatMusic.setVolume(m_Volume);
	}
	playRandomMusic();
}

void SoundManager::update()
{
	CheckIfPlayNextSong();
}

void SoundManager::playRandomMusic()
{
	stopCurrentMusic();
	if (m_Music.size() <= 0)
	{
		return;
	}
	unsigned int randomIndex = std::rand() % 8;
	m_Music[randomIndex]->play();
	m_CurrentMusic = m_Music[randomIndex];
}

void SoundManager::pauseCurrentMusic()
{
	if (m_CurrentMusic != nullptr)
	{
		m_CurrentMusic->pause();
	}
}

void SoundManager::resumeCurrentMusic()
{
	if (m_VictoryMusic.getStatus() == sf::SoundSource::Playing)
	{
		return;
	}
	if (m_DefeatMusic.getStatus() == sf::SoundSource::Playing)
	{
		return;
	}
	if (m_CurrentMusic != nullptr)
	{
		m_CurrentMusic->play();
	}
}

void SoundManager::stopCurrentMusic()
{
	if (m_CurrentMusic != nullptr)
	{
		m_CurrentMusic->stop();
	}
}

void SoundManager::playBattleMusic()
{
	if (m_BattleMusic.getStatus() != sf::SoundSource::Playing)
	{
		m_BattleMusic.play();
	}
}

void SoundManager::stopBattleMusic()
{
	m_BattleMusic.stop();
}

void SoundManager::playDefeatMusic()
{
	if (m_DefeatMusic.getStatus() != sf::SoundSource::Playing)
	{
		m_DefeatMusic.play();
	}
}

void SoundManager::playVictoryMusic()
{
	if (m_VictoryMusic.getStatus() != sf::SoundSource::Playing)
	{
		m_VictoryMusic.play();
	}
}

void SoundManager::CheckIfPlayNextSong()
{
	if (m_CurrentMusic != nullptr)
	{
		if (m_CurrentMusic->getStatus() == sf::SoundSource::Playing)
		{
			return;
		}
	}
	if (m_BattleMusic.getStatus() == sf::SoundSource::Playing)
	{
		return;
	}
	if (m_VictoryMusic.getStatus() == sf::SoundSource::Playing)
	{
		return;
	}
	if (m_DefeatMusic.getStatus() == sf::SoundSource::Playing)
	{
		return;
	}
	playRandomMusic();
}

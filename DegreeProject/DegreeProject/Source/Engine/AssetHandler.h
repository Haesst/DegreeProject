#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class AssetHandler
{
public:

	inline const static char* TestSprite = "Assets/Graphics/Test.jpg";
	inline const static char* TileSet = "Assets/Graphics/TileSet.png";

public:

	inline static AssetHandler& get()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = new AssetHandler();
		}

		return *m_Instance;
	}

	const sf::Texture loadImageFromFile(const char* FilePath);

	sf::Font loadFontFromFile(const char* FilePath);

	sf::Sound loadSoundFile(const char* FilePath, sf::SoundBuffer& Buffer);

	bool openMusicFile(const char* FilePath, sf::Music& music);

	const sf::Texture getTextureAtPath(const char* FilePath);

private:
	static AssetHandler* m_Instance;
	std::vector<std::pair<const char*, sf::Texture>> m_LoadedTextures;
};

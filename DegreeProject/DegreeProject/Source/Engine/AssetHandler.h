#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class AssetHandler
{
public:

	inline const static char* TestSprite = "Assets/Graphics/Test.jpg";
	inline const static char* TileSet = "Assets/Graphics/TileSet.png";

public:

	const sf::Texture loadImageFromFile(const char* FilePath);

	sf::Font loadFontFromFile(const char* FilePath);

	sf::Sound loadAudioFile(const char* FilePath, sf::SoundBuffer& Buffer);

	const sf::Texture getTextureAtPath(const char* FilePath);

private:
	std::vector<std::pair<const char*, sf::Texture>> m_LoadedTextures;
};

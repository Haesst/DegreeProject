#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class AssetHandler
{
public:
	sf::Sprite LoadImageFromFile(const char* FilePath, sf::Texture& Texture);

	sf::Text LoadFontFromFileToText(const char* FilePath);

	sf::Sound LoadAudioFile(const char* FilePath, sf::SoundBuffer& Buffer);

private:
};

#pragma once
#include <SFML/Graphics.hpp>

class AssetHandler
{
public:
	sf::Sprite LoadImageFromFile(const char* FilePath, sf::Texture& Texture);

	sf::Text LoadFontFromFileToText(const char* FilePath);

private:
};

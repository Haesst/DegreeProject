#pragma once
#include <SFML/Graphics.hpp>

class AssetHandler
{
public:
	sf::Sprite LoadImageFromFileToSprite(const char* FilePath, sf::Texture& Texture);

	sf::Text LoadFontFromFileToText(const char* FilePath);

private:
};

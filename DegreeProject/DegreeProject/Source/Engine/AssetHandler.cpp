#include "AssetHandler.h"

sf::Sprite AssetHandler::LoadImageFromFileToSprite(const char* FilePath, sf::Texture& Texture)
{
	sf::Image Img;
	sf::Sprite sprite;

	if (Img.loadFromFile(FilePath))
	{
		Texture.loadFromImage(Img);

		sprite.setTexture(Texture);
		
		return sprite;
	}

	return sprite;
}

sf::Text AssetHandler::LoadFontFromFileToText(const char* FilePath)
{
	sf::Text Text;
	sf::Font Font;

	if (Font.loadFromFile(FilePath))
	{
		Text.setFont(Font);
		return Text;
	}

	return Text;
}
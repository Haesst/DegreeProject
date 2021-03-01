#include "AssetHandler.h"

sf::Sprite AssetHandler::LoadImageFromFile(const char* FilePath, sf::Texture& Texture)
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

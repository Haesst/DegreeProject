#include "AssetHandler.h"

const sf::Sprite AssetHandler::LoadImageFromFile(const char* FilePath, sf::Texture& Texture)
{
	sf::Image Img;
	sf::Sprite Sprite;

	for(const auto& Pair : LoadedImages)
	{
		if (0 == strcmp(Pair.first, FilePath))
		{
			Texture.loadFromImage(Pair.second);
			Sprite.setTexture(Texture);
			return Sprite;
		}
	}

	if(Img.loadFromFile(FilePath))
	{
		Texture.loadFromImage(Img);
		Sprite.setTexture(Texture);

		auto LoadedData = std::make_pair(FilePath, Img);

		LoadedImages.push_back(LoadedData);
		
		return Sprite;
	}

	return Sprite;
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

sf::Sound AssetHandler::LoadAudioFile(const char* FilePath, sf::SoundBuffer& Buffer)
{
	if (Buffer.loadFromFile(FilePath))
	{
		sf::Sound SoundFile;
		SoundFile.setBuffer(Buffer);
		return SoundFile;
	}
	
	return sf::Sound();
}
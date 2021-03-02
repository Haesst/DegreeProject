#include "AssetHandler.h"

const sf::Texture AssetHandler::LoadImageFromFile(const char* FilePath)
{

	for(const auto& Pair : LoadedTextures)
	{
		if (0 == strcmp(Pair.first, FilePath))
		{
			return GetTextureAtPath(FilePath);
		}
	}

	sf::Image Img;
	sf::Texture Texture;

	if(Img.loadFromFile(FilePath))
	{
		Texture.loadFromImage(Img);

		const auto LoadedData = std::make_pair(FilePath, Texture);
		LoadedTextures.push_back(LoadedData);
		
		return Texture;
	}

	return Texture;
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

const sf::Texture AssetHandler::GetTextureAtPath(const char* FilePath)
{
	for (const auto& Pair : LoadedTextures)
	{
		if (0 == strcmp(Pair.first, FilePath))
		{
			return Pair.second;
		}
	}

	return sf::Texture();
}

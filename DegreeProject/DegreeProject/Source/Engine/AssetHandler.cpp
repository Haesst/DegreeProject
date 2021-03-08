#include "AssetHandler.h"

const sf::Texture AssetHandler::LoadImageFromFile(const char* FilePath)
{
	for(const auto& Pair : m_LoadedTextures)
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
		m_LoadedTextures.push_back(LoadedData);
		
		return Texture;
	}

	return Texture;
}

sf::Font AssetHandler::LoadFontFromFileToText(const char* FilePath)
{
	sf::Text Text;
	sf::Font Font;

	if (Font.loadFromFile(FilePath))
	{
		Text.setFont(Font);
		return Font;
		//return Text;
	}

	return Font;
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
	for (const auto& Pair : m_LoadedTextures)
	{
		if (0 == strcmp(Pair.first, FilePath))
		{
			return Pair.second;
		}
	}

	sf::Texture TextureToLoad = LoadImageFromFile(FilePath);

	auto LoadedData = std::make_pair(FilePath, TextureToLoad);
	m_LoadedTextures.push_back(LoadedData);

	return TextureToLoad;
}

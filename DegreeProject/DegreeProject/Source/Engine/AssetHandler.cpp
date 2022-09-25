#include "AssetHandler.h"

AssetHandler* AssetHandler::m_Instance = nullptr;

const sf::Texture AssetHandler::loadImageFromFile(const char* FilePath)
{
	for(const auto& Pair : m_LoadedTextures)
	{
		if (0 == strcmp(Pair.first, FilePath))
		{
			return getTextureAtPath(FilePath);
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

sf::Font AssetHandler::loadFontFromFile(const char* FilePath)
{
	sf::Font Font;

	if (Font.loadFromFile(FilePath))
	{
		return Font;
	}

	return Font;
}

sf::Sound AssetHandler::loadSoundFile(const char* FilePath, sf::SoundBuffer& Buffer)
{
	if (Buffer.loadFromFile(FilePath))
	{
		sf::Sound SoundFile;
		SoundFile.setBuffer(Buffer);
		return SoundFile;
	}
	
	return sf::Sound();
}

bool AssetHandler::openMusicFile(const char* FilePath, sf::Music& music)
{
	if (music.openFromFile(FilePath))
	{
		return true;
	}

	return false;
}

const sf::Texture AssetHandler::getTextureAtPath(const char* FilePath)
{
	for (const auto& Pair : m_LoadedTextures)
	{
		if (0 == strcmp(Pair.first, FilePath))
		{
			return Pair.second;
		}
	}

	sf::Texture TextureToLoad = loadImageFromFile(FilePath);

	auto LoadedData = std::make_pair(FilePath, TextureToLoad);
	m_LoadedTextures.push_back(LoadedData);

	return TextureToLoad;
}

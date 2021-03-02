#pragma once
#include <SFML/Graphics.hpp>
#include "AssetHandler.h"

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	bool Load(AssetHandler& AssetLoader, const std::string& TileSet, sf::Vector2u TileSize, const int* tiles, unsigned int width, unsigned int height);
	sf::Texture m_TileSetTexture;

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Texture m_TileSet;
	sf::VertexArray m_Verticies;
};

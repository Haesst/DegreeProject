#include "TileMap.h"

bool TileMap::Load(AssetHandler& AssetLoader, const std::string& TileSet, sf::Vector2u TileSize, const int* tiles, unsigned int width, unsigned int height)
{
	m_TileSet = AssetLoader.GetTextureAtPath(AssetLoader.TileSet); 

	//Resize the vertex array to fit the level size
	m_Verticies.setPrimitiveType(sf::Quads);
	m_Verticies.resize(width * height * 4);

	//Populate the vertex array with one quad per tile

	for (unsigned int x = 0; x < width; x++)
	{
		for (unsigned int y = 0; y < height; y++)
		{
			int TileNumber = tiles[x + y * width];

			//Find its position in the tileset texture
			int Tu = TileNumber % (m_TileSet.getSize().x / TileSize.x);
			int Tv = TileNumber / (m_TileSet.getSize().x / TileSize.x);

			//Get a pointer to the current tiles quad
			sf::Vertex* Quad = &m_Verticies[(x + y * width) * 4];

			//define its 4 corners
			Quad[0].position = sf::Vector2f(x * TileSize.x, y * TileSize.y);
			Quad[1].position = sf::Vector2f((x + 1) * TileSize.x, y * TileSize.y);
			Quad[2].position = sf::Vector2f((x + 1) * TileSize.x, (y + 1) * TileSize.y);
			Quad[3].position = sf::Vector2f(x * TileSize.x, (y + 1) * TileSize.y);

			//Define its 4 texture coordinates
			Quad[0].texCoords = sf::Vector2f(Tu * TileSize.x, Tv * TileSize.y);
			Quad[1].texCoords = sf::Vector2f((Tu + 1) * TileSize.x, Tv * TileSize.y);
			Quad[2].texCoords = sf::Vector2f((Tu + 1) * TileSize.x, (Tv + 1) * TileSize.y);
			Quad[3].texCoords = sf::Vector2f(Tu * TileSize.x, (Tv + 1) * TileSize.y);
		}
	}

	return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Apply transform
	states.transform *= getTransform();

	//Apply tileset Texture
	states.texture = &m_TileSet;

	//Draw the vertex array
	target.draw(m_Verticies, states);
}

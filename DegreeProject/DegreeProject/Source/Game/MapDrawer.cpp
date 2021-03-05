#include "MapDrawer.h"
#include <fstream>

MapDrawer::MapDrawer(){}

MapDrawer::~MapDrawer(){}

std::vector<sf::RectangleShape> MapDrawer::GetMap(const char* path, const char walkable, const sf::Vector2<int> resolution, const float size)
{
	std::string tempString;
	std::ifstream inData;
	std::vector<std::pair<char, std::pair<int, int> > > mapData;
	inData.open(path);
	if (inData.is_open())
	{
		int y = 0;
		while (!inData.eof())
		{
			getline(inData, tempString);
			int x = 0;
			for each (char character in tempString)
			{
				std::pair<int, int> Position;
				Position.first = x;
				Position.second = y;
				mapData.push_back(std::make_pair(character, Position));
				x++;
			}
			y++;
		}
	}
	inData.close();
	return CreateMap(mapData, walkable, resolution, size);
}

std::vector<sf::RectangleShape> MapDrawer::CreateMap(const std::vector<std::pair<char, std::pair<int, int> > > mapData, const char walkable,const sf::Vector2<int> resolution, const float size)
{
	std::vector<sf::RectangleShape> map;
	for (size_t i = 0; i < mapData.size(); ++i)
	{
		if (mapData[i].first == walkable)
		{
			sf::RectangleShape rectangle;
			rectangle.setSize(sf::Vector2(size, size));
			rectangle.setPosition(resolution.x * 0.1f + mapData[i].second.first * rectangle.getSize().x * 0.5f, mapData[i].second.second * rectangle.getSize().y);
			rectangle.setFillColor(sf::Color::Green);
			map.push_back(rectangle);
		}
	}

	m_Tile.Load(m_AssetLoader, m_AssetLoader.TileSet, sf::Vector2u(32, 32), level, 100, 40);
	return map;
}

void MapDrawer::DrawMap(const std::vector<sf::RectangleShape>* mapData, sf::RenderWindow& window)
{
	window.draw(m_Tile);

	//window.draw(Tile);
	//for each (sf::RectangleShape rectangle in *mapData)
	//{
	//	window.draw(rectangle);
	//}
}
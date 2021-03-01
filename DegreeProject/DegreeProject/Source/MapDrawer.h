#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class MapDrawer
{
public:
	MapDrawer();
	~MapDrawer();
	std::vector<sf::RectangleShape> GetMap(const char* path, const char walkable, const sf::Vector2<int> resolution, const float size);
	void DrawMap(const std::vector<sf::RectangleShape>* mapData, sf::RenderWindow& window);
private:
	std::vector<sf::RectangleShape> CreateMap(const std::vector<std::pair<char, std::pair<int, int> > > mapData, const char walkable, const sf::Vector2<int> resolution, const float size);
};
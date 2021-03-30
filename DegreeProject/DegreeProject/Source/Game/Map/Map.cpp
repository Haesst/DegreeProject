#pragma warning(push)
#pragma warning(disable: 26812)

#include "Map.h"
#include "Game/HotReloader.h"
#include "Engine/FileWatcher.h"
#include <Game/Components/CharacterComponent.h>

const int   Map::m_XOffset = -300;
const int   Map::m_YOffset = -100;
const float Map::m_TileSize = 32;
const float Map::m_HalfTileSize = m_TileSize * 0.5f;

Map* Map::m_Instance = nullptr;

void Map::init()
{
	clearRegions();
	loadAllRegions();
	clearRegionMapTiles();
	loadMap();
	loadShadersAndCreateRenderStates();
	createVertexArrays();
	setupHotReloading();
}

void Map::setLandTexture(sf::Texture tex)
{
	m_Data.m_LandTexture = tex;
}

void Map::setupHotReloading()
{
	HotReloader::get()->subscribeToFileChange("Assets\\Data\\Regions.json", std::bind(&Map::regionsChanged, this, std::placeholders::_1, std::placeholders::_2));
	HotReloader::get()->subscribeToFileChange("Assets\\Map\\RegionMap.txt", std::bind(&Map::regionsChanged, this, std::placeholders::_1, std::placeholders::_2));

	HotReloader::get()->subscribeToFileChange("Assets\\Shaders\\LandShader.frag", std::bind(&Map::shadersChanged, this, std::placeholders::_1, std::placeholders::_2));
	HotReloader::get()->subscribeToFileChange("Assets\\Shaders\\LandShader.vert", std::bind(&Map::shadersChanged, this, std::placeholders::_1, std::placeholders::_2));
}

Map& Map::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new Map();
	}

	return *m_Instance;
}

void Map::regionsChanged(std::string path, FileStatus fileStatus)
{
	if (fileStatus != FileStatus::Modified)
	{
		return;
	}

	LOG_INFO("Regions changed");

	m_RegionMutex.lock();
	updateRegions();
	m_RegionMutex.unlock();
}

void Map::shadersChanged(std::string path, FileStatus fileStatus)
{
	if (fileStatus != FileStatus::Modified)
	{
		return;
	}

	m_ShaderMutex.lock();
	loadShadersAndCreateRenderStates();
	m_ShaderMutex.unlock();
}

std::ifstream Map::loadFile(const char* path)
{
	std::ifstream file(path);

	return file;
}

void Map::loadAllRegions()
{
	std::ifstream file = loadFile(m_Data.m_RegionPath);

	json j;
	file >> j;

	ASSERT(m_Data.m_Regions.size() <= 0, "Regions not cleared");

	for (auto& element : j)
	{
		MapRegion region;

		region.m_HighlightColor = sf::Color::White;//GetColor(element["color"].get<std::string>());
		std::string mapCharString = element["mapchar"].get<std::string>();
		region.m_RegionId = element["id"].get<unsigned int>();
		region.m_RegionTax = element["tax"].get<unsigned int>();
		region.m_RegionName = element["name"].get<std::string>();
		region.m_RegionCapital.x = element["capitalx"].get<unsigned int>();
		region.m_RegionCapital.y = element["capitaly"].get<unsigned int>();

		if (mapCharString.size() > 0)
		{
			region.m_MapChar = mapCharString[0];
		}

		m_Data.m_Regions.push_back(region);
	}
}

void Map::loadMap()
{
	ASSERT(m_Data.m_Regions[0].m_MapSquares.size() <= 0, "Mapsquare is empty");

	std::string tempString;
	std::ifstream inData = loadFile(m_Data.m_MapPath);

	int maxWidth = 0;
	int maxHeight = 0;

	if (inData.is_open())
	{
		int y = 0;
		while (!inData.eof())
		{
			getline(inData, tempString);

			if (tempString.empty())
			{
				break;
			}

			int x = 0;
			for (char character : tempString)
			{
				int regionPosition = getRegionPositionFromMapCharacter(character);

				if (regionPosition >= 0)
				{
					m_Data.m_Regions[regionPosition].m_MapSquares.push_back({ x, y });
					Vector2DInt loc = { x,y };
					SquareData data = { m_Data.m_Regions[regionPosition].m_RegionId };
					data.m_Position = { x,y };

					m_MapSquareData.push_back(data);
				}

				x++;
			}

			y++;

			if (x > maxWidth)
			{
				maxWidth = x;
			}

			if (y > maxHeight)
			{
				maxHeight = y;
			}
		}
	}
	width = maxWidth;
	height = maxHeight;

	inData.close();
}

void Map::updateRegions()
{
	std::ifstream file = loadFile(m_Data.m_RegionPath);

	json j;
	file >> j;

	for (auto& element : j)
	{
		int arrayLoc = getRegionPositionFromRegionId(element["id"]);

		if (arrayLoc >= 0)
		{
			std::string mapCharString = element["mapchar"].get<std::string>();

			if (mapCharString.size() > 0)
			{
				m_Data.m_Regions[arrayLoc].m_MapChar = mapCharString[0];
			}
			m_Data.m_Regions[arrayLoc].m_RegionTax = element["tax"].get<unsigned int>();
			m_Data.m_Regions[arrayLoc].m_RegionName = element["name"].get<std::string>();
			m_Data.m_Regions[arrayLoc].m_RegionCapital.x = element["capitalx"].get<unsigned int>();
			m_Data.m_Regions[arrayLoc].m_RegionCapital.y = element["capitaly"].get<unsigned int>();
		}
	}
}

void Map::render()
{
	for (auto& region : m_Data.m_Regions)
	{
		m_Data.m_Shader.setUniform("u_Color", sf::Glsl::Vec4(region.m_HighlightColor));
		m_Data.m_Shader.setUniform("u_Texture", m_Data.m_LandTexture);
		m_Data.m_Shader.setUniform("u_Highlighted", region.m_Highlighted);
		Window::getWindow()->draw(region.m_VertexArray, m_Data.m_RenderStates);
	}
}

void Map::setRegionColor(int regionId, sf::Color color)
{
	getRegionById(regionId).m_HighlightColor = color;
}

void Map::clearRegions()
{
	m_Data.m_Regions.clear();
}

bool Map::mapSquareDataContainsKey(const Vector2DInt& key)
{
	for (auto& p : m_MapSquareData)
	{
		if (p.m_Position == key)
		{
			return true;
		}
	}

	return false;
}

void Map::clearRegionMapTiles()
{
	for (auto& region : m_Data.m_Regions)
	{
		region.m_MapSquares.clear();
	}
}

void Map::createVertexArrays()
{
	for (auto& region : m_Data.m_Regions)
	{
		ASSERT(region.m_VertexArray.getVertexCount() <= 0, "Vertex array is not cleared");
		region.m_VertexArray.setPrimitiveType(sf::Triangles);

		for (auto& square : region.m_MapSquares)
		{
			sf::Vertex v1;
			sf::Vertex v2;
			sf::Vertex v3;
			sf::Vertex v4;

			v1.position = { (square.x * m_TileSize) -m_HalfTileSize + m_XOffset, (square.y * m_TileSize) - m_HalfTileSize + m_YOffset }; // Top left
			v2.position = { (square.x * m_TileSize) +m_HalfTileSize + m_XOffset, (square.y * m_TileSize) - m_HalfTileSize + m_YOffset }; // Top right
			v3.position = { (square.x * m_TileSize) +m_HalfTileSize + m_XOffset, (square.y * m_TileSize) + m_HalfTileSize + m_YOffset }; // Bottom right
			v4.position = { (square.x * m_TileSize) -m_HalfTileSize + m_XOffset, (square.y * m_TileSize) + m_HalfTileSize + m_YOffset }; // Bottom Left

			v1.texCoords = { (square.x * m_TileSize), (square.y * m_TileSize) };
			v2.texCoords = { (square.x * m_TileSize) + m_TileSize, (square.y * m_TileSize) };
			v3.texCoords = { (square.x * m_TileSize) + m_TileSize, (square.y * m_TileSize) + m_TileSize };
			v4.texCoords = { (square.x * m_TileSize), (square.y * m_TileSize) + m_TileSize };

			region.m_VertexArray.append(v1);
			region.m_VertexArray.append(v2);
			region.m_VertexArray.append(v3);
			region.m_VertexArray.append(v1);
			region.m_VertexArray.append(v3);
			region.m_VertexArray.append(v4);
		}
	}
}

void Map::loadShadersAndCreateRenderStates()
{
	m_Data.m_Shader.loadFromFile(m_Data.m_VertexShaderPath, m_Data.m_FragmentShaderPath);

	m_Data.m_RenderStates.shader = &m_Data.m_Shader;
	m_Data.m_RenderStates.texture = &m_Data.m_LandTexture;
}

void Map::startConstructionOfBuilding(int buildingId, int buildSlot, int regionId)
{
	// not working -> int maxBuildings = (sizeof(RegionBuilding) / sizeof(*GetRegionById(regionId).m_BuildingSlots));
	ASSERT(buildSlot >= 0 /*&& buildSlot < maxBuildings */, "Invalid buildslot");
	int characterGold = EntityManager::get().getComponent<CharacterComponent>(getRegionById(regionId).m_OwnerID).m_CurrentGold;
	int buildingCost = GameData::m_Buildings[buildingId].m_Cost;
	ASSERT(characterGold >= buildingCost, "Not enough money to build");

	RegionBuilding& building = getRegionById(regionId).m_BuildingSlots[buildSlot];

	if (building.m_BuildingId < 0)
	{
		building.startBuild(buildingId);
	}
}

int Map::getRegionPositionFromMapCharacter(const char& c)
{
	for (size_t i = 0; i < m_Data.m_Regions.size(); ++i)
	{
		if (m_Data.m_Regions[i].m_MapChar == c)
		{
			return i;
		}
	}

	return -1;
}

int Map::getRegionPositionFromRegionId(const unsigned int id)
{
	for (size_t i = 0; i < m_Data.m_Regions.size(); ++i)
	{
		if (m_Data.m_Regions[i].m_RegionId == id)
		{
			return (int)i;
		}
	}

	return -1;
}

MapRegion& Map::getRegionById(unsigned int regionId)
{
	for (auto& region : m_Data.m_Regions)
	{
		if (region.m_RegionId == regionId)
		{
			return region;
		}
	}

	ASSERT(true, "No region with this id exists");
	return m_Data.m_Regions[0]; // To remove warning, unreachable code.
}

Vector2DInt Map::convertToMap(Vector2D position)
{
	return Vector2DInt((int)(position.x - m_XOffset + m_HalfTileSize) / m_TileSize, (int)(position.y - m_YOffset + m_HalfTileSize) / m_TileSize);
}

Vector2D Map::convertToScreen(Vector2DInt position)
{
	return Vector2D((float)position.x * m_TileSize + m_XOffset - m_HalfTileSize, (float)position.y * m_TileSize + m_YOffset - m_HalfTileSize);
}

Vector2DInt Map::getRegionCapitalLocation(unsigned int regionId)
{
	return getRegionById(regionId).m_RegionCapital;
}

std::vector<Vector2DInt> Map::getRegionCapitals()
{
	std::vector<Vector2DInt> capitalLocations;

	for (const MapRegion& region : m_Data.m_Regions)
	{
		capitalLocations.push_back(region.m_RegionCapital);
	}

	return capitalLocations;
}

std::vector<int> Map::getRegionIDs()
{
	std::vector<int> regionIds;

	for (const MapRegion& region : m_Data.m_Regions)
	{
		regionIds.push_back(region.m_RegionId);
	}

	return regionIds;
}
Vector2DInt Map::getMapSize()
{
	return Vector2DInt(width, height);
}
#pragma warning(pop)
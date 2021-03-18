#pragma warning(push)
#pragma warning(disable: 26812)

#include "Map.h"
#include "Game/HotReloader.h"
#include "Engine/FileWatcher.h"

const char* Map::m_RegionPath = "Assets/Data/Regions.json";
const char* Map::m_MapPath = "Assets/Map/RegionMap.txt";
const char* Map::m_FragmentShaderPath = "Assets/Shaders/LandShader.frag";
const char* Map::m_VertexShaderPath = "Assets/Shaders/LandShader.vert";
MapData Map::m_Data;
std::mutex Map::m_RegionMutex;
std::map<Vector2DInt, SquareData> Map::m_MapUnitData;

#pragma region Init
void Map::Init()
{
	ClearRegions();
	LoadAllRegions();
	ClearRegionMapTiles();
	LoadMap();
	LoadShadersAndCreateRenderStates();
	CreateVertexArrays();
	SetupHotReloading();
}
#pragma endregion

void Map::SetLandTexture(sf::Texture tex)
{
	m_Data.m_LandTexture = tex;
}

#pragma region HotReloading Setup

void Map::SetupHotReloading()
{
	HotReloader::Get()->SubscribeToFileChange("Assets\\Data\\Regions.json", std::bind(&Map::RegionsChanged, std::placeholders::_1, std::placeholders::_2));
	HotReloader::Get()->SubscribeToFileChange("Assets\\Map\\RegionMap.txt", std::bind(&Map::RegionsChanged, std::placeholders::_1, std::placeholders::_2));

	HotReloader::Get()->SubscribeToFileChange("Assets\\Shaders\\LandShader.frag", std::bind(&Map::ShadersChanged, std::placeholders::_1, std::placeholders::_2));
	HotReloader::Get()->SubscribeToFileChange("Assets\\Shaders\\LandShader.vert", std::bind(&Map::ShadersChanged, std::placeholders::_1, std::placeholders::_2));
}

void Map::RegionsChanged(std::string path, FileStatus fileStatus)
{
	if (fileStatus != FileStatus::Modified)
	{
		return;
	}

	LOG_INFO("Regions changed");

	m_RegionMutex.lock();
	UpdateRegions();
	m_RegionMutex.unlock();
}

void Map::MapChanged(std::string, FileStatus)
{}

void Map::ShadersChanged(std::string path, FileStatus fileStatus)
{
	if (fileStatus != FileStatus::Modified)
	{
		return;
	}
}

#pragma endregion

#pragma region Region & Map loading

std::ifstream Map::LoadFile(const char* path)
{
	std::ifstream file(path);

	return file;
}

void Map::LoadAllRegions()
{
	std::ifstream file = LoadFile(m_RegionPath);

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

void Map::LoadMap()
{
	ASSERT(m_Data.m_Regions[0].m_MapSquares.size() <= 0);

	std::string tempString;
	std::ifstream inData = LoadFile(m_MapPath);

	if (inData.is_open())
	{
		int y = 0;
		while (!inData.eof())
		{
			getline(inData, tempString);
			int x = 0;
			for (char character : tempString)
			{
				int regionPosition = GetRegionPositionFromMapCharacter(character);

				if (regionPosition >= 0)
				{
					m_Data.m_Regions[regionPosition].m_MapSquares.push_back({ x, y });
					m_MapUnitData.insert(std::make_pair(Vector2DInt(x, y), SquareData()));
				}

				x++;
			}
			y++;
		}
	}
	inData.close();
}

void Map::UpdateRegions()
{
	std::ifstream file = LoadFile(m_RegionPath);

	json j;
	file >> j;

	for (auto& element : j)
	{
		int arrayLoc = GetRegionPositionFromRegionId(element["id"]);

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

#pragma endregion

void Map::Render()
{
	for (auto& region : m_Data.m_Regions)
	{
		m_Data.m_Shader.setUniform("u_Color", sf::Glsl::Vec4(region.m_HighlightColor));
		m_Data.m_Shader.setUniform("u_Texture", m_Data.m_LandTexture);
		Window::GetWindow()->draw(region.m_VertexArray, m_Data.m_RenderStates);
	}
}

void Map::SetRegionColor(int regionId, sf::Color color)
{
	GetRegionById(regionId).m_HighlightColor = color;
}

void Map::ClearRegions()
{
	m_Data.m_Regions.clear();
}

void Map::ClearRegionMapTiles()
{
	for (auto& region : m_Data.m_Regions)
	{
		region.m_MapSquares.clear();
	}
}

void Map::CreateVertexArrays()
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

			v1.position = { (square.x * m_Data.m_TileSize) - m_Data.m_HalfTileSize + m_Data.m_XOffset, (square.y * m_Data.m_TileSize) - m_Data.m_HalfTileSize + m_Data.m_YOffset }; // Top left
			v2.position = { (square.x * m_Data.m_TileSize) + m_Data.m_HalfTileSize + m_Data.m_XOffset, (square.y * m_Data.m_TileSize) - m_Data.m_HalfTileSize + m_Data.m_YOffset }; // Top right
			v3.position = { (square.x * m_Data.m_TileSize) + m_Data.m_HalfTileSize + m_Data.m_XOffset, (square.y * m_Data.m_TileSize) + m_Data.m_HalfTileSize + m_Data.m_YOffset }; // Bottom right
			v4.position = { (square.x * m_Data.m_TileSize) - m_Data.m_HalfTileSize + m_Data.m_XOffset, (square.y * m_Data.m_TileSize) + m_Data.m_HalfTileSize + m_Data.m_YOffset }; // Bottom Left

			v1.texCoords = { (square.x * m_Data.m_TileSize), (square.y * m_Data.m_TileSize) };
			v2.texCoords = { (square.x * m_Data.m_TileSize) + m_Data.m_TileSize, (square.y * m_Data.m_TileSize) };
			v3.texCoords = { (square.x * m_Data.m_TileSize) + m_Data.m_TileSize, (square.y * m_Data.m_TileSize) + m_Data.m_TileSize };
			v4.texCoords = { (square.x * m_Data.m_TileSize), (square.y * m_Data.m_TileSize) + m_Data.m_TileSize };

			region.m_VertexArray.append(v1);
			region.m_VertexArray.append(v2);
			region.m_VertexArray.append(v3);
			region.m_VertexArray.append(v1);
			region.m_VertexArray.append(v3);
			region.m_VertexArray.append(v4);
		}
	}
}

void Map::LoadShadersAndCreateRenderStates()
{
	m_Data.m_Shader.loadFromFile(m_VertexShaderPath, m_FragmentShaderPath);

	m_Data.m_RenderStates.shader = &m_Data.m_Shader;
	m_Data.m_RenderStates.texture = &m_Data.m_LandTexture;
}

int Map::GetRegionPositionFromMapCharacter(const char& c)
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

int Map::GetRegionPositionFromRegionId(const unsigned int id)
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

MapRegion& Map::GetRegionById(unsigned int regionId)
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

Vector2DInt Map::ConvertToMap(Vector2D position)
{
	return Vector2DInt((int)(position.x - m_Data.m_XOffset + 16) / 32, (int)(position.y - m_Data.m_YOffset + 16) / 32);
}

Vector2D Map::ConvertToScreen(Vector2DInt position)
{
	return Vector2D((float)position.x * 32 + m_Data.m_XOffset - 16, (float)position.y * 32 + m_Data.m_YOffset - 16);
}

Vector2DInt Map::GetRegionCapitalLocation(unsigned int regionId)
{
	int regionArrayLocation = GetRegionPositionFromRegionId(regionId);

	ASSERT(regionArrayLocation >= 0, "Region with id: {0} doesn't exist", regionId);

	return m_Data.m_Regions[regionArrayLocation].m_RegionCapital;
}

std::vector<Vector2DInt> Map::GetRegionCapitals()
{
	std::vector<Vector2DInt> capitalLocations;

	for (const MapRegion& region : m_Data.m_Regions)
	{
		capitalLocations.push_back(region.m_RegionCapital);
	}

	return capitalLocations;
}

std::vector<int> Map::GetRegionIDs()
{
	std::vector<int> regionIds;

	for (const MapRegion& region : m_Data.m_Regions)
	{
		regionIds.push_back(region.m_RegionId);
	}

	return regionIds;
}
#pragma warning(pop)
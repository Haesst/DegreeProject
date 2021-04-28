#include "MiniMap.h"

#include "Engine/Window.h"
#include "Engine/Log.h"

#include "Game/Map/Map.h"

void MiniMap::start()
{
	float actualAspectRatio = 1920.0f / 1080.0f;
	float desiredAspectRatio = 1920.0f / 1920.0f;

	float height = m_Width * desiredAspectRatio;
	sf::Vector2u screenSize = Window::getWindow()->getSize();
	sf::Vector2f miniMapPosition = { (float)screenSize.x - m_Width - m_RightOffset - m_BorderSize, (float)screenSize.y - height - m_BottomOffset - m_BorderSize };
	m_MiniMapBorder.setPosition({ (float)screenSize.x - m_Width - m_RightOffset - m_BorderSize, (float)screenSize.y - height - m_BottomOffset - m_BorderSize + m_BorderHeightOffset });

	sf::Vector2f floatResolution = sf::Vector2f((float)1920, (float)1080);

	float viewWidth = m_Width / floatResolution.x;
	float viewBorderWidth = m_BorderSize / floatResolution.x;
	float viewRightOffset = m_RightOffset / floatResolution.x;

	float viewHeight = height / floatResolution.y;
	float viewBottomOffset = m_BottomOffset / floatResolution.y;

	m_MiniMapView.setViewport(sf::FloatRect(1.0f - viewWidth - viewBorderWidth - viewRightOffset, 1.0f - viewBorderWidth - viewBottomOffset - viewHeight, viewWidth, viewHeight));
	m_MiniMapView.setCenter({ floatResolution.x * 4.5f, floatResolution.y * 1.25f });
	m_MiniMapView.zoom(8.0f);

	m_MiniMapBorder.setFillColor(m_Fillcolor);
	m_MiniMapBorder.setOutlineColor(m_BorderColor);
	m_MiniMapBorder.setSize({ m_Width, height + m_ViewHeightOffset });
	m_MiniMapBorder.setOutlineThickness(m_BorderSize);
}

void MiniMap::update()
{
}

void MiniMap::render()
{
	sf::RenderWindow* window = Window::getWindow();
	window->draw(m_MiniMapBorder);

	m_ViewHolder = window->getView();
	window->setView(m_MiniMapView);
	Map::get().drawMiniMap();
	window->setView(m_ViewHolder);
}

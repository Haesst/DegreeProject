#include "MiniMap.h"

#include "Engine/Window.h"
#include "Engine/Log.h"

#include "Game/Map/Map.h"

void MiniMap::start()
{
	float ar = m_SpriteSize.y / m_SpriteSize.x;

	float desiredAspectRatio = m_Height / m_Width;
	float arDifference = desiredAspectRatio / ar;

	sf::RenderWindow* window = Window::getWindow();

	float borderLeftPos = window->getSize().x - m_Width - m_BorderSize - m_RightOffset;
	float borderRightPos = borderLeftPos + m_Width;

	float borderTopPos = window->getSize().y - m_Height - m_BorderSize - m_BottomOffset;
	float borderBottomPos = borderTopPos + m_Height;

	float viewLeftPos = (borderLeftPos + m_BorderSize + m_ViewLeftOffset) / window->getSize().x;
	float viewRightPos = (borderRightPos - m_BorderSize + m_WidthOffset) / window->getSize().x;
	float viewTopPos = (borderTopPos + m_BorderSize + -(m_Height * arDifference) + m_ViewTopOffset) / window->getSize().y;
	float viewBottomPos = (borderBottomPos - m_BorderSize + (m_Height * arDifference) + m_ViewHeightOffset) / window->getSize().y;
	
	float viewWidth = viewRightPos - viewLeftPos;
	float viewHeight = viewBottomPos - viewTopPos;

	m_MiniMapBorder.setPosition(borderLeftPos, borderTopPos);
	m_MiniMapBorder.setSize({ m_Width, m_Height });
	m_MiniMapBorder.setOutlineColor(m_BorderColor);
	m_MiniMapBorder.setOutlineThickness(m_BorderSize);

	m_MiniMapView.setViewport(sf::FloatRect(viewLeftPos, viewTopPos, viewWidth, viewHeight));
	m_MiniMapView.setCenter({ m_StartPosition.x, m_StartPosition.y });
	m_MiniMapView.zoom(m_ZoomLevel);
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

void MiniMap::setUIView(sf::View uiView)
{
	m_UIView = uiView;
}

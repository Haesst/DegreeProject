#include "MiniMap.h"

#include "Engine/Window.h"
#include "Engine/Log.h"
#include "Engine/InputHandler.h"

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

	float windowX = window->getSize().x;
	float windowY = window->getSize().y;

	float viewLeftPos = (borderLeftPos + m_ViewLeftOffset) / windowX;
	float viewRightPos = (borderRightPos + m_ViewLeftOffset + m_ViewWidthOffset) / windowX;

	float viewTopPos = (borderTopPos + m_ViewTopOffset) / windowY;
	float viewBottomPos = (borderBottomPos + m_ViewTopOffset + m_ViewHeightOffset) / windowY;
	
	float viewWidth = viewRightPos - viewLeftPos;
	float viewHeight = viewBottomPos - viewTopPos;

	m_MiniMapBorder.setPosition(borderLeftPos, borderTopPos);
	m_MiniMapBorder.setSize({ m_Width, m_Height });
	m_MiniMapBorder.setOutlineColor(m_BorderColor);
	m_MiniMapBorder.setOutlineThickness(m_BorderSize);

	m_MiniMapView.setViewport(sf::FloatRect(viewLeftPos, viewTopPos, viewWidth, viewHeight));
	m_MiniMapView.setCenter({ m_StartPosition.x, m_StartPosition.y });
	m_MiniMapView.zoom(m_ZoomLevel);

	InputHandler::setMiniMapView(m_MiniMapView);

	float rectX = window->getSize().x;
	float rectY = window->getSize().y;

	m_GameViewRectangle.setFillColor(sf::Color::Transparent);
	m_GameViewRectangle.setOutlineColor(sf::Color::White);
	m_GameViewRectangle.setOutlineThickness(40.0f);
	m_GameViewRectangle.setSize({ rectX, rectY });
}

void MiniMap::update()
{
	updateGameViewRectangle();

	if (InputHandler::getLeftMouseReleased())
	{
		Vector2D mousePos = InputHandler::getMiniMapMousePosition();

		bool mouseInsideMiniMap = mousePos.x > 0.0f && mousePos.x < m_SpriteSize.x && mousePos.y > 0.0f && mousePos.y < (m_SpriteSize.y * 1.4f);
		
		if (mouseInsideMiniMap)
		{
			ASSERT(m_GameView != nullptr, "Gameview not set");
			m_GameView->setCenter({mousePos.x, mousePos.y});
			Window::getWindow()->setView(*m_GameView);
			InputHandler::setLeftMouseReleased(false);
		}
	}
}

void MiniMap::render()
{
	sf::RenderWindow* window = Window::getWindow();
	window->draw(m_MiniMapBorder);

	m_ViewHolder = window->getView();
	window->setView(m_MiniMapView);
	Map::get().drawMiniMap();
	window->draw(m_GameViewRectangle);
	window->setView(m_ViewHolder);
}

void MiniMap::setUIView(sf::View& uiView)
{
	m_UIView = uiView;
}

void MiniMap::setGameView(sf::View* gameView)
{
	m_GameView = gameView;
}

void MiniMap::updateGameViewRectangle()
{
	sf::RenderWindow* window = Window::getWindow();

	m_GameViewCenter = window->getView().getCenter();
	sf::Vector2u gameViewSize = window->getSize();
	sf::Vector2f resolution = { 1920, 1080 };
	resolution *= InputHandler::m_TotalZoom;

	m_GameViewRectangle.setSize({(float)resolution.x, (float)resolution.y});
	
	float startX = m_GameViewCenter.x - (resolution.x / 2);
	float startY = m_GameViewCenter.y - (resolution.y / 2);
	m_GameViewRectangle.setPosition({ startX, startY });
}

void MiniMap::setPlayerColor(sf::Color& color)
{
	m_BorderColor = color;
	m_MiniMapBorder.setOutlineColor(color);
}

MiniMap::MiniMap()
	: m_ViewHolder(sf::View()), m_UIView(sf::View()), m_GameView(nullptr)
{}

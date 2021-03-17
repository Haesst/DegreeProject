#include "InputHandler.h"
#include <SFML/Graphics.hpp>
#include "Window.h"
#include <Engine/Log.h>
#include "Game/Map/Map.h"

bool m_LeftMouseClicked = false;
bool m_RightMouseClicked = false;
bool m_LeftMouseReleased = false;
bool m_RightMouseReleased = false;
bool m_MouseScrollWheelChanged = false;
bool m_PlayerUnitSelected = false;
bool m_MouseMoved = false;
int m_MouseScrollDirection = 0;
const float MAX_ZOOM = 1000.0f;
const float MIN_ZOOM = 100.0f;
const float MOVE_SPEED = 25.0f;
const float ZOOM_SPEED = 0.1f;
Vector2D m_MousePosition = Vector2D(0.0f, 0.0f);
Vector2DInt m_MouseMapPosition = Vector2DInt(0, 0);

void InputHandler::HandleInputEvents()
{
	m_RightMouseReleased = false;
	m_LeftMouseReleased = false;
	m_MouseMoved = false;
	sf::RenderWindow* window = Window::GetWindow();
	sf::View view = window->getView();
	sf::Event event;
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
			{
				window->close();
				break;
			}
			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
				{
					if (view.getCenter().y > sf::VideoMode::getDesktopMode().height * 0.1f)
					{
						view.move(0.0f, -MOVE_SPEED);
						window->setView(view);
					}
				}
				if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left)
				{
					if (view.getCenter().x > sf::VideoMode::getDesktopMode().width * 0.1f)
					{
						view.move(-MOVE_SPEED, 0.0f);
						window->setView(view);
					}
				}
				if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
				{
					if (view.getCenter().y < sf::VideoMode::getDesktopMode().height * 0.9f)
					{
						view.move(0.0f, MOVE_SPEED);
						window->setView(view);
					}
				}
				if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right)
				{
					if (view.getCenter().x < sf::VideoMode::getDesktopMode().width * 0.9f)
					{
						view.move(MOVE_SPEED, 0.0f);
						window->setView(view);
					}
				}
				if (event.key.code == sf::Keyboard::Escape)
				{
					window->close();
				}
				if (event.key.code == sf::Keyboard::Space)
				{
					//std::cout << "SpacePressed" << std::endl;
					//std::cout << "PauseGame" << std::endl;
				}
				break;
			}
			case sf::Event::Resized:
			{
				break;
			}
			case sf::Event::GainedFocus:
			{
				//std::cout << "ResumeGame" << std::endl;
				break;
			}
			case sf::Event::LostFocus:
			{
				//std::cout << "PauseGame" << std::endl;
				break;
			}
			case sf::Event::TextEntered:
			{
				if (event.text.unicode < 128)
				{
				}
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				if (event.key.code == sf::Mouse::Left)
				{
					m_LeftMouseClicked = true;
					m_MousePosition = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
					m_MouseMapPosition = Map::ConvertToMap(m_MousePosition);
				}
				if (event.key.code == sf::Mouse::Right)
				{
					m_RightMouseClicked = true;
					m_MousePosition = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
					m_MouseMapPosition = Map::ConvertToMap(m_MousePosition);
				}
				if (event.key.code == sf::Mouse::Middle)
				{
				}
				if (event.key.code == sf::Mouse::XButton1)
				{
				}
				if (event.key.code == sf::Mouse::XButton2)
				{
				}
				break;
			}
			case sf::Event::MouseButtonReleased:
			{
				if (event.key.code == sf::Mouse::Left)
				{
					m_LeftMouseClicked = false;
					m_LeftMouseReleased = true;
					m_MousePosition = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
					m_MouseMapPosition = Map::ConvertToMap(m_MousePosition);
				}
				if (event.key.code == sf::Mouse::Right)
				{
					m_RightMouseClicked = false;
					m_RightMouseReleased = true;
					m_MousePosition = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
					m_MouseMapPosition = Map::ConvertToMap(m_MousePosition);
				}
				if (event.key.code == sf::Mouse::Middle)
				{
				}
				if (event.key.code == sf::Mouse::XButton1)
				{
				}
				if (event.key.code == sf::Mouse::XButton2)
				{
				}
				break;
			}
			case sf::Event::MouseWheelScrolled:
			{
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
				{
					m_MouseScrollWheelChanged = true;
					m_MouseScrollDirection = (int)event.mouseWheelScroll.delta;
					if ((m_MouseScrollDirection == 1 && (view.getSize().x > MIN_ZOOM || view.getSize().y > MIN_ZOOM)) || (m_MouseScrollDirection == -1 && (view.getSize().x < MAX_ZOOM || view.getSize().y < MAX_ZOOM)))
					{
						view.zoom(1.0f - m_MouseScrollDirection * ZOOM_SPEED);
						window->setView(view);
					}
				}
				break;
			}
			case sf::Event::MouseMoved:
			{
				m_MouseMoved = true;
				m_MouseMapPosition = Vector2DInt(event.mouseMove.x, event.mouseMove.y);
				m_MousePosition = window->mapPixelToCoords(sf::Vector2i(m_MouseMapPosition.x, m_MouseMapPosition.y));
				m_MouseMapPosition = Map::ConvertToMap(m_MousePosition);
				break;
			}
			case sf::Event::MouseEntered:
			{
				//std::cout << "ResumeGame" << std::endl;
				break;
			}
			case sf::Event::MouseLeft:
			{
				//std::cout << "PauseGame" << std::endl;
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

bool InputHandler::GetLeftMouseClicked()
{
	return m_LeftMouseClicked;
}

bool InputHandler::GetRightMouseClicked()
{
	return m_RightMouseClicked;
}

bool InputHandler::GetLeftMouseReleased()
{
	return m_LeftMouseReleased;
}

bool InputHandler::GetRightMouseReleased()
{
	return m_RightMouseReleased;
}

bool InputHandler::GetMouseScrollWheelChanged()
{
	return m_MouseScrollWheelChanged;
}

Vector2D InputHandler::GetMousePosition()
{
	return m_MousePosition;
}

Vector2DInt InputHandler::GetMouseMapPosition()
{
	return m_MouseMapPosition;
}

int InputHandler::GetMouseScrollDirection()
{
	return m_MouseScrollDirection;
}

bool InputHandler::GetPlayerUnitSelected()
{
	return m_PlayerUnitSelected;
}

bool InputHandler::GetMouseMoved()
{
	return m_MouseMoved;
}

void InputHandler::SetPlayerUnitSelected(bool selected)
{
	m_PlayerUnitSelected = selected;
}

float InputHandler::GetZoomSpeed()
{
	return ZOOM_SPEED;
}

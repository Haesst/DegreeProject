#include "InputHandler.h"
#include <SFML/Graphics.hpp>
#include "Window.h"
#include <Engine/Log.h>
#include "Game/Map/Map.h"

float m_MouseScrollDirection = 0.0f;
Vector2DInt m_ViewMoveDirection = Vector2DInt();
const float MAX_ZOOM = 1000.0f;
const float MIN_ZOOM = 100.0f;
const float MOVE_SPEED = 25.0f;
const float ZOOM_SPEED = 0.1f;
Vector2D m_MousePosition = Vector2D(0.0f, 0.0f);
Vector2DInt m_MouseMapPosition = Vector2DInt(0, 0);
bool inputs[Inputs::PlayerUnitSelected + 1];

void InputHandler::HandleInputEvents()
{
	inputs[RightMouseReleased] = false;
	inputs[LeftMouseReleased] = false;
	inputs[MouseMoved] = false;
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
				switch (event.key.code)
				{
					case sf::Keyboard::W:
					{
						m_ViewMoveDirection = Vector2DInt(0, -1);
						if (AllowedToMoveView(view))
						{
							MoveView(window, view);
						}
						break;
					}
					case sf::Keyboard::A:
					{
						m_ViewMoveDirection = Vector2DInt(-1, 0);
						if (AllowedToMoveView(view))
						{
							MoveView(window, view);
						}
						break;
					}
					case sf::Keyboard::S:
					{
						m_ViewMoveDirection = Vector2DInt(0, 1);
						if (AllowedToMoveView(view))
						{
							MoveView(window, view);
						}
						break;
					}
					case sf::Keyboard::D:
					{
						m_ViewMoveDirection = Vector2DInt(1, 0);
						if (AllowedToMoveView(view))
						{
							MoveView(window, view);
						}
						break;
					}
					case sf::Keyboard::Up:
					{
						m_ViewMoveDirection = Vector2DInt(0, -1);
						if (AllowedToMoveView(view))
						{
							MoveView(window, view);
						}
						break;
					}
					case sf::Keyboard::Left:
					{
						m_ViewMoveDirection = Vector2DInt(-1, 0);
						if (AllowedToMoveView(view))
						{
							MoveView(window, view);
						}
						break;
					}
					case sf::Keyboard::Down:
					{
						m_ViewMoveDirection = Vector2DInt(0, 1);
						if (AllowedToMoveView(view))
						{
							MoveView(window, view);
						}
						break;
					}
					case sf::Keyboard::Right:
					{
						m_ViewMoveDirection = Vector2DInt(1, 0);
						if (AllowedToMoveView(view))
						{
							MoveView(window, view);
						}
						break;
					}
					case sf::Keyboard::Escape:
					{
						window->close();
					}
					case sf::Keyboard::Space:
					{
						//std::cout << "SpacePressed" << std::endl;
						//std::cout << "PauseGame" << std::endl;
					}
					default:
					{
						break;
					}
				}
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
				//if (event.text.unicode < 128)
				//{
				//}
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				switch (event.key.code)
				{
					case sf::Mouse::Left:
					{
						inputs[LeftMouseClicked] = true;
						SetMousePosition(event.mouseButton.x, event.mouseButton.y, window);
						break;
					}
					case sf::Mouse::Right:
					{
						inputs[RightMouseClicked] = true;
						SetMousePosition(event.mouseButton.x, event.mouseButton.y, window);
						break;
					}
					case sf::Mouse::Middle:
					{
						break;
					}
					case sf::Mouse::XButton1:
					{
						break;
					}
					case sf::Mouse::XButton2:
					{
						break;
					}
					default:
					{
						break;
					}
				}
			}
			case sf::Event::MouseButtonReleased:
			{
				switch (event.key.code)
				{
					case sf::Mouse::Left:
					{
						inputs[LeftMouseClicked] = false;
						inputs[LeftMouseReleased] = true;
						SetMousePosition(event.mouseButton.x, event.mouseButton.y, window);
						break;
					}
					case sf::Mouse::Right:
					{
						inputs[RightMouseClicked] = false;
						inputs[RightMouseReleased] = true;
						SetMousePosition(event.mouseButton.x, event.mouseButton.y, window);
						break;
					}
					case sf::Mouse::Middle:
					{
						break;
					}
					case sf::Mouse::XButton1:
					{
						break;
					}
					case sf::Mouse::XButton2:
					{
						break;
					}
					default:
					{
						break;
					}
				}
			}
			case sf::Event::MouseWheelScrolled:
			{
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
				{
					m_MouseScrollDirection = event.mouseWheelScroll.delta;
					if (AllowedToZoomView(view))
					{
						ZoomView(window, view);
					}
				}
				break;
			}
			case sf::Event::MouseMoved:
			{
				inputs[MouseMoved] = true;
				SetMousePosition(event.mouseMove.x, event.mouseMove.y, window);
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

void InputHandler::ZoomView(sf::RenderWindow* window, sf::View& view)
{
	view.zoom(1.0f - m_MouseScrollDirection * ZOOM_SPEED);
	window->setView(view);
}

bool InputHandler::AllowedToZoomView(sf::View& view)
{
	return (m_MouseScrollDirection >= 0.0f && (view.getSize().x > MIN_ZOOM || view.getSize().y > MIN_ZOOM)) 
		|| (m_MouseScrollDirection <= 0.0f && (view.getSize().x < MAX_ZOOM || view.getSize().y < MAX_ZOOM));
}

bool InputHandler::AllowedToMoveView(sf::View& view)
{
	return (m_ViewMoveDirection.y == -1 && view.getCenter().y > sf::VideoMode::getDesktopMode().height * 0.1f)
		|| (m_ViewMoveDirection.y == 1 && view.getCenter().y < sf::VideoMode::getDesktopMode().height * 0.9f)
		|| (m_ViewMoveDirection.x == 1 && view.getCenter().x < sf::VideoMode::getDesktopMode().width * 0.9f)
		|| (m_ViewMoveDirection.x == -1 && view.getCenter().x > sf::VideoMode::getDesktopMode().width * 0.1f);
}

void InputHandler::SetMousePosition(int xPosition, int yPosition, sf::RenderWindow* window)
{
	m_MouseMapPosition = Vector2DInt(xPosition, yPosition);
	m_MousePosition = window->mapPixelToCoords(sf::Vector2i(m_MouseMapPosition.x, m_MouseMapPosition.y));
	m_MouseMapPosition = Map::ConvertToMap(m_MousePosition);
}

void InputHandler::MoveView(sf::RenderWindow* window, sf::View& view)
{
	view.move(m_ViewMoveDirection.x * MOVE_SPEED, m_ViewMoveDirection.y * MOVE_SPEED);
	window->setView(view);
}

bool InputHandler::GetLeftMouseClicked()
{
	return inputs[LeftMouseClicked];
}

bool InputHandler::GetRightMouseClicked()
{
	return inputs[RightMouseReleased];
}

bool InputHandler::GetLeftMouseReleased()
{
	return inputs[LeftMouseReleased];
}

bool InputHandler::GetRightMouseReleased()
{
	return inputs[RightMouseReleased];
}

Vector2D InputHandler::GetMousePosition()
{
	return m_MousePosition;
}

Vector2DInt InputHandler::GetMouseMapPosition()
{
	return m_MouseMapPosition;
}

float InputHandler::GetMouseScrollDirection()
{
	return m_MouseScrollDirection;
}

bool InputHandler::GetPlayerUnitSelected()
{
	return inputs[PlayerUnitSelected];
}

bool InputHandler::GetMouseMoved()
{
	return inputs[MouseMoved];
}

void InputHandler::SetPlayerUnitSelected(bool selected)
{
	inputs[PlayerUnitSelected] = selected;
}

float InputHandler::GetZoomSpeed()
{
	return ZOOM_SPEED;
}

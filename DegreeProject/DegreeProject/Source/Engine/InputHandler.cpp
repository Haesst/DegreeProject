#include "InputHandler.h"
#include <SFML/Graphics.hpp>
#include "Window.h"
#include <Engine/Log.h>
#include "Game/Map/Map.h"

static float m_MouseScrollDirection = 0.0f;
static Vector2DInt m_ViewMoveDirection = Vector2DInt();
static const float MAX_ZOOM = 1000.0f;
static const float MIN_ZOOM = 100.0f;
static const float MOVE_SPEED = 25.0f;
static const float ZOOM_SPEED = 0.1f;
static Vector2D m_MousePosition = Vector2D(0.0f, 0.0f);
static Vector2DInt m_MouseMapPosition = Vector2DInt(0, 0);
static bool inputs[Inputs::PlayerUnitSelected + 1];

void InputHandler::HandleInputEvents()
{
	inputs[LeftMouseClicked] = false;
	inputs[RightMouseClicked] = false;
	inputs[LeftMouseReleased] = false;
	inputs[RightMouseReleased] = false;
	inputs[MiddleMouseClicked] = false;
	inputs[MouseMoved] = false;
	inputs[MouseScrolled] = false;
	inputs[EscapePressed] = false;
	inputs[CharacterWindowOpen] = false;
	inputs[RegionWindowOpen] = false;
	inputs[PlayerUnitSelected] = false;
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
					case sf::Keyboard::Equal:
					case sf::Keyboard::Add:
					{
						Time::IncreaseGameSpeed();
						break;
					}
					case sf::Keyboard::Hyphen:
					case sf::Keyboard::Subtract:
					{
						Time::DecreaseGameSpeed();
						break;
					}
					case sf::Keyboard::Escape:
					{
						inputs[EscapePressed] = true;
						break;
					}
					case sf::Keyboard::Space:
					{
						Time::GamePaused() ? Time::UnpauseGame() : Time::PauseGame();
						break;
					}
					case sf::Keyboard::Delete:
					{
						window->close();
						break;
					}
					case sf::Keyboard::Numpad1:
					case sf::Keyboard::Num1:
					{
						Time::SetGameSpeed(1);
						break;
					}
					case sf::Keyboard::Numpad2:
					case sf::Keyboard::Num2:
					{
						Time::SetGameSpeed(2);
						break;
					}
					case sf::Keyboard::Numpad3:
					case sf::Keyboard::Num3:
					{
						Time::SetGameSpeed(3);
						break;
					}
					case sf::Keyboard::Numpad4:
					case sf::Keyboard::Num4:
					{
						Time::SetGameSpeed(4);
						break;
					}
					case sf::Keyboard::Numpad5:
					case sf::Keyboard::Num5:
					{
						Time::SetGameSpeed(5);
						break;
					}
					default:
					{
						break;
					}
				}
				break;
			}
			case sf::Event::Resized:
			{
				break;
			}
			case sf::Event::GainedFocus:
			{
				Time::UnpauseGame();
				break;
			}
			case sf::Event::LostFocus:
			{
				Time::PauseGame();
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
						inputs[MiddleMouseClicked] = true;
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
				break;
			}
			case sf::Event::MouseButtonReleased:
			{
				switch (event.key.code)
				{
					case sf::Mouse::Left:
					{
						inputs[LeftMouseReleased] = true;
						SetMousePosition(event.mouseButton.x, event.mouseButton.y, window);
						break;
					}
					case sf::Mouse::Right:
					{
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
				break;
			}
			case sf::Event::MouseWheelScrolled:
			{
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
				{
					m_MouseScrollDirection = event.mouseWheelScroll.delta;
					if (AllowedToZoomView(view))
					{
						inputs[MouseScrolled] = true;
						ZoomView(window, view);
					}
				}
				break;
			}
			case sf::Event::MouseMoved:
			{
				inputs[MouseMoved] = true;
				Vector2D previousMousePosition = GetMousePosition();
				SetMousePosition(event.mouseMove.x, event.mouseMove.y, window);
				if (inputs[MiddleMouseClicked] && AllowedToMoveView(view))
				{
					Vector2D direction = (previousMousePosition - GetMousePosition()).Normalized();
					m_ViewMoveDirection = Vector2DInt((int)direction.x, (int)direction.y);
					MoveView(window, view);
				}
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
	return inputs[RightMouseClicked];
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

bool InputHandler::GetMouseScrolled()
{
	return inputs[MouseScrolled];
}

bool InputHandler::GetPlayerUnitSelected()
{
	return inputs[PlayerUnitSelected];
}

bool InputHandler::GetCharacterWindowOpen()
{
	return inputs[CharacterWindowOpen];
}

bool InputHandler::GetRegionWindowOpen()
{
	return inputs[RegionWindowOpen];
}

bool InputHandler::GetMouseMoved()
{
	return inputs[MouseMoved];
}

bool InputHandler::GetEscapePressed()
{
	return inputs[EscapePressed];
}

void InputHandler::SetPlayerUnitSelected(bool selected)
{
	inputs[PlayerUnitSelected] = selected;
}

void InputHandler::SetCharacterWindowOpen(bool open)
{
	inputs[CharacterWindowOpen] = open;
}

void InputHandler::SetRegionWindowOpen(bool open)
{
	inputs[RegionWindowOpen] = open;
}

float InputHandler::GetZoomSpeed()
{
	return ZOOM_SPEED;
}

#include "InputHandler.h"
#include <SFML/Graphics.hpp>
#include "Window.h"
#include <Engine/Log.h>
#include "Game/Map/Map.h"

static float mouseScrollDirection = 0.0f;
static Vector2DInt viewMoveDirection = Vector2DInt();
static const float MAX_ZOOM = 5000.0f;
static const float MIN_ZOOM = 100.0f;
static const float MOVE_SPEED = 25.0f;
static const float ZOOM_SPEED = 0.1f;
static Vector2D mousePosition = Vector2D(0.0f, 0.0f);
static Vector2D mousePositionUI = Vector2D(0.0f, 0.0f);
static Vector2DInt mouseMapPosition = Vector2DInt(0, 0);
bool InputHandler::m_Inputs[Inputs::PlayerUnitSelected + 1];

sf::View& InputHandler::m_UIView = sf::View();

float InputHandler::maxCenterXPosition = 26010.0f;
float InputHandler::minCenterXPosition = -140.0f;
float InputHandler::maxCenterYPosition = 10015.0f;
float InputHandler::minCenterYPosition = -85.0f;

void InputHandler::handleInputEvents()
{
	m_Inputs[LeftMouseClicked] = false;
	m_Inputs[RightMouseClicked] = false;
	m_Inputs[LeftMouseReleased] = false;
	m_Inputs[RightMouseReleased] = false;
	m_Inputs[MiddleMouseReleased] = false;
	m_Inputs[MouseMoved] = false;
	m_Inputs[MouseScrolled] = false;
	m_Inputs[EscapePressed] = false;
	m_Inputs[PlayerUnitSelected] = false;
	sf::RenderWindow& window = *Window::getWindow();
	sf::View view = window.getView();
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
		{
			window.close();
			break;
		}
		case sf::Event::KeyPressed:
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Up:
			case sf::Keyboard::W:
			{
				viewMoveDirection = Vector2DInt(0, -1);
				moveView(window, view);
				break;
			}
			case sf::Keyboard::Left:
			case sf::Keyboard::A:
			{
				viewMoveDirection = Vector2DInt(-1, 0);
				
				moveView(window, view);
				break;
			}
			case sf::Keyboard::Down:
			case sf::Keyboard::S:
			{
				viewMoveDirection = Vector2DInt(0, 1);
				moveView(window, view);
				break;
			}
			case sf::Keyboard::Right:
			case sf::Keyboard::D:
			{
				viewMoveDirection = Vector2DInt(1, 0);
				moveView(window, view);
				break;
			}
			case sf::Keyboard::Equal:
			case sf::Keyboard::Add:
			{
				Time::increaseGameSpeed();
				break;
			}
			case sf::Keyboard::Hyphen:
			case sf::Keyboard::Subtract:
			{
				Time::decreaseGameSpeed();
				break;
			}
			case sf::Keyboard::Escape:
			{
				m_Inputs[EscapePressed] = true;
				break;
			}
			case sf::Keyboard::Space:
			{
				Time::gamePaused() ? Time::unpauseGame() : Time::pauseGame();
				break;
			}
			case sf::Keyboard::Delete:
			{
				window.close();
				break;
			}
			case sf::Keyboard::Numpad1:
			case sf::Keyboard::Num1:
			{
				Time::setGameSpeed(1);
				break;
			}
			case sf::Keyboard::Numpad2:
			case sf::Keyboard::Num2:
			{
				Time::setGameSpeed(2);
				break;
			}
			case sf::Keyboard::Numpad3:
			case sf::Keyboard::Num3:
			{
				Time::setGameSpeed(3);
				break;
			}
			case sf::Keyboard::Numpad4:
			case sf::Keyboard::Num4:
			{
				Time::setGameSpeed(4);
				break;
			}
			case sf::Keyboard::Numpad5:
			case sf::Keyboard::Num5:
			{
				Time::setGameSpeed(5);
				break;
			}
			case sf::Keyboard::U:
			{
				CharacterManager::get().killCharacter(CharacterManager::get().getPlayerCharacterID());
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
			Time::unpauseGame();
			break;
		}
		case sf::Event::LostFocus:
		{
			Time::pauseGame();
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
				m_Inputs[LeftMouseClicked] = true;
				setMousePosition(event.mouseButton.x, event.mouseButton.y, window);
				break;
			}
			case sf::Mouse::Right:
			{
				m_Inputs[RightMouseClicked] = true;
				setMousePosition(event.mouseButton.x, event.mouseButton.y, window);
				break;
			}
			case sf::Mouse::Middle:
			{
				m_Inputs[MiddleMouseClicked] = true;
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
				m_Inputs[LeftMouseReleased] = true;
				setMousePosition(event.mouseButton.x, event.mouseButton.y, window);
				break;
			}
			case sf::Mouse::Right:
			{
				m_Inputs[RightMouseReleased] = true;
				setMousePosition(event.mouseButton.x, event.mouseButton.y, window);
				break;
			}
			case sf::Mouse::Middle:
			{
				m_Inputs[MiddleMouseReleased] = true;
				m_Inputs[MiddleMouseClicked] = false;
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
				mouseScrollDirection = event.mouseWheelScroll.delta;
				if (allowedToZoomView(view))
				{
					m_Inputs[MouseScrolled] = true;
					zoomView(window, view);
				}
			}
			break;
		}
		case sf::Event::MouseMoved:
		{
			m_Inputs[MouseMoved] = true;
			Vector2D previousMousePosition = getMousePosition();
			setMousePosition(event.mouseMove.x, event.mouseMove.y, window);
			if (m_Inputs[MiddleMouseClicked])
			{
				Vector2D distance = (getMousePosition() - previousMousePosition);
				float length = distance.getLength();
				if (length > 1.0f)
				{
					Vector2D direction = distance.normalized();
					view.move(-direction.x, -direction.y);
					window.setView(view);
				}
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

void InputHandler::setUIView(sf::View& uiView)
{
	m_UIView = uiView;
}

void InputHandler::zoomView(sf::RenderWindow& window, sf::View& view)
{
	view.zoom(1.0f - mouseScrollDirection * ZOOM_SPEED);
	window.setView(view);
}

bool InputHandler::allowedToZoomView(const sf::View& view)
{
	return (mouseScrollDirection >= 0.0f && (view.getSize().x > MIN_ZOOM || view.getSize().y > MIN_ZOOM))
		|| (mouseScrollDirection <= 0.0f && (view.getSize().x < MAX_ZOOM || view.getSize().y < MAX_ZOOM));
}

void InputHandler::setMousePosition(int xPosition, int yPosition, const sf::RenderWindow& window)
{
	mouseMapPosition = Vector2DInt(xPosition, yPosition);
	mousePosition = window.mapPixelToCoords(sf::Vector2i(mouseMapPosition.x, mouseMapPosition.y));
	mousePositionUI = window.mapPixelToCoords(sf::Vector2i(mouseMapPosition.x, mouseMapPosition.y), m_UIView);
	mouseMapPosition = Map::convertToMap(mousePosition);
}

void InputHandler::moveView(sf::RenderWindow& window, sf::View& view)
{
	float xMovement = viewMoveDirection.x * MOVE_SPEED;
	float yMovement = viewMoveDirection.y * MOVE_SPEED;

	float newX = view.getCenter().x + xMovement;
	float newY = view.getCenter().y + yMovement;

	if (newX >= maxCenterXPosition || newX <= minCenterXPosition)
	{
		xMovement = 0;
	}

	if (newY >= maxCenterYPosition || newY <= minCenterYPosition)
	{
		yMovement = 0;
	}

	view.move(xMovement, yMovement);

	window.setView(view);
}

bool InputHandler::getLeftMouseClicked()
{
	return m_Inputs[LeftMouseClicked];
}

bool InputHandler::getRightMouseClicked()
{
	return m_Inputs[RightMouseClicked];
}

bool InputHandler::getLeftMouseReleased()
{
	return m_Inputs[LeftMouseReleased];
}

bool InputHandler::getRightMouseReleased()
{
	return m_Inputs[RightMouseReleased];
}

Vector2D InputHandler::getMousePosition()
{
	return mousePosition;
}

Vector2D InputHandler::getUIMousePosition()
{
	return mousePositionUI;
}

Vector2DInt InputHandler::getMouseMapPosition()
{
	return mouseMapPosition;
}

float InputHandler::getMouseScrollDirection()
{
	return mouseScrollDirection;
}

bool InputHandler::getMouseScrolled()
{
	return m_Inputs[MouseScrolled];
}

bool InputHandler::getPlayerUnitSelected()
{
	return m_Inputs[PlayerUnitSelected];
}

bool InputHandler::getCharacterWindowOpen()
{
	return m_Inputs[CharacterWindowOpen];
}

bool InputHandler::getRegionWindowOpen()
{
	return m_Inputs[RegionWindowOpen];
}

bool InputHandler::getWarWindowOpen()
{
	return m_Inputs[WarWindowOpen];
}

bool InputHandler::getMouseMoved()
{
	return m_Inputs[MouseMoved];
}

bool InputHandler::getEscapePressed()
{
	return m_Inputs[EscapePressed];
}

void InputHandler::setPlayerUnitSelected(bool selected)
{
	m_Inputs[PlayerUnitSelected] = selected;
}

void InputHandler::setCharacterWindowOpen(bool open)
{
	m_Inputs[CharacterWindowOpen] = open;
}

void InputHandler::setRegionWindowOpen(bool open)
{
	m_Inputs[RegionWindowOpen] = open;
}

void InputHandler::setWarWindowOpen(bool open)
{
	m_Inputs[WarWindowOpen] = open;
}

float InputHandler::getZoomSpeed()
{
	return ZOOM_SPEED;
}

void InputHandler::setLeftMouseClicked(bool clicked)
{
	m_Inputs[LeftMouseClicked] = clicked;
}

void InputHandler::setRightMouseClicked(bool clicked)
{
	m_Inputs[RightMouseClicked] = clicked;
}

void InputHandler::setLeftMouseReleased(bool released)
{
	m_Inputs[LeftMouseReleased] = released;
}

void InputHandler::setRightMouseReleased(bool released)
{
	m_Inputs[RightMouseReleased] = released;
}
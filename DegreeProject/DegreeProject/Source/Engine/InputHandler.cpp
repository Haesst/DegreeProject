#include "InputHandler.h"
#include <SFML/Graphics.hpp>
#include "Window.h"
#include <Engine/Log.h>
#include "Game/Map/Map.h"

static float mouseScrollDirection = 0.0f;
static Vector2DInt viewMoveDirection = Vector2DInt();
static const float MAX_ZOOM = 1500.0f;
static const float MIN_ZOOM = 100.0f;
static const float MOVE_SPEED = 25.0f;
static const float ZOOM_SPEED = 0.1f;
static Vector2D mousePosition = Vector2D(0.0f, 0.0f);
static Vector2DInt mouseMapPosition = Vector2DInt(0, 0);
static bool inputs[Inputs::PlayerUnitSelected + 1];

void InputHandler::handleInputEvents()
{
	inputs[LeftMouseClicked] = false;
	inputs[RightMouseClicked] = false;
	inputs[LeftMouseReleased] = false;
	inputs[RightMouseReleased] = false;
	inputs[MiddleMouseReleased] = false;
	inputs[MouseMoved] = false;
	inputs[MouseScrolled] = false;
	inputs[EscapePressed] = false;
	inputs[PlayerUnitSelected] = false;
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
				if (allowedToMoveView(view))
				{
					moveView(window, view);
				}
				break;
			}
			case sf::Keyboard::Left:
			case sf::Keyboard::A:
			{
				viewMoveDirection = Vector2DInt(-1, 0);
				if (allowedToMoveView(view))
				{
					moveView(window, view);
				}
				break;
			}
			case sf::Keyboard::Down:
			case sf::Keyboard::S:
			{
				viewMoveDirection = Vector2DInt(0, 1);
				if (allowedToMoveView(view))
				{
					moveView(window, view);
				}
				break;
			}
			case sf::Keyboard::Right:
			case sf::Keyboard::D:
			{
				viewMoveDirection = Vector2DInt(1, 0);
				if (allowedToMoveView(view))
				{
					moveView(window, view);
				}
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
				inputs[EscapePressed] = true;
				if (!inputs[RegionWindowOpen] && !inputs[CharacterWindowOpen] && !inputs[WarWindowOpen])
				{
					window.close();
				}
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
				inputs[LeftMouseClicked] = true;
				setMousePosition(event.mouseButton.x, event.mouseButton.y, window);
				break;
			}
			case sf::Mouse::Right:
			{
				inputs[RightMouseClicked] = true;
				setMousePosition(event.mouseButton.x, event.mouseButton.y, window);
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
				setMousePosition(event.mouseButton.x, event.mouseButton.y, window);
				break;
			}
			case sf::Mouse::Right:
			{
				inputs[RightMouseReleased] = true;
				setMousePosition(event.mouseButton.x, event.mouseButton.y, window);
				break;
			}
			case sf::Mouse::Middle:
			{
				inputs[MiddleMouseReleased] = true;
				inputs[MiddleMouseClicked] = false;
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
					inputs[MouseScrolled] = true;
					zoomView(window, view);
				}
			}
			break;
		}
		case sf::Event::MouseMoved:
		{
			inputs[MouseMoved] = true;
			Vector2D previousMousePosition = getMousePosition();
			setMousePosition(event.mouseMove.x, event.mouseMove.y, window);
			if (inputs[MiddleMouseClicked])
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

bool InputHandler::allowedToMoveView(const sf::View& view)
{
	return (viewMoveDirection.y == -1 && view.getCenter().y > sf::VideoMode::getDesktopMode().height * 0.1f)
		|| (viewMoveDirection.y == 1 && view.getCenter().y < sf::VideoMode::getDesktopMode().height * 0.9f)
		|| (viewMoveDirection.x == 1 && view.getCenter().x < sf::VideoMode::getDesktopMode().width * 0.9f)
		|| (viewMoveDirection.x == -1 && view.getCenter().x > sf::VideoMode::getDesktopMode().width * 0.1f);
}

void InputHandler::setMousePosition(int xPosition, int yPosition, const sf::RenderWindow& window)
{
	mouseMapPosition = Vector2DInt(xPosition, yPosition);
	mousePosition = window.mapPixelToCoords(sf::Vector2i(mouseMapPosition.x, mouseMapPosition.y));
	mouseMapPosition = Map::convertToMap(mousePosition);
}

void InputHandler::moveView(sf::RenderWindow& window, sf::View& view)
{
	view.move(viewMoveDirection.x * MOVE_SPEED, viewMoveDirection.y * MOVE_SPEED);
	window.setView(view);
}

bool InputHandler::getLeftMouseClicked()
{
	return inputs[LeftMouseClicked];
}

bool InputHandler::getRightMouseClicked()
{
	return inputs[RightMouseClicked];
}

bool InputHandler::getLeftMouseReleased()
{
	return inputs[LeftMouseReleased];
}

bool InputHandler::getRightMouseReleased()
{
	return inputs[RightMouseReleased];
}

Vector2D InputHandler::getMousePosition()
{
	return mousePosition;
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
	return inputs[MouseScrolled];
}

bool InputHandler::getPlayerUnitSelected()
{
	return inputs[PlayerUnitSelected];
}

bool InputHandler::getCharacterWindowOpen()
{
	return inputs[CharacterWindowOpen];
}

bool InputHandler::getRegionWindowOpen()
{
	return inputs[RegionWindowOpen];
}

bool InputHandler::getWarWindowOpen()
{
	return inputs[WarWindowOpen];
}

bool InputHandler::getMouseMoved()
{
	return inputs[MouseMoved];
}

bool InputHandler::getEscapePressed()
{
	return inputs[EscapePressed];
}

void InputHandler::setPlayerUnitSelected(bool selected)
{
	inputs[PlayerUnitSelected] = selected;
}

void InputHandler::setCharacterWindowOpen(bool open)
{
	inputs[CharacterWindowOpen] = open;
}

void InputHandler::setRegionWindowOpen(bool open)
{
	inputs[RegionWindowOpen] = open;
}

void InputHandler::setWarWindowOpen(bool open)
{
	inputs[WarWindowOpen] = open;
}

float InputHandler::getZoomSpeed()
{
	return ZOOM_SPEED;
}

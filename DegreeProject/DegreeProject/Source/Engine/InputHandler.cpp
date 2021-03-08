#include "InputHandler.h"
#include <iostream>
#include "Window.h"
#include "Time.h"

bool InputHandler::m_LeftMouseClicked = false;
bool InputHandler::m_RightMouseClicked = false;
bool InputHandler::m_LeftMouseReleased = false;
bool InputHandler::m_RightMouseReleased = false;
bool InputHandler::m_MouseScrollWheelChanged = false;
bool InputHandler::m_PlayerSelected = false;
int InputHandler::m_MouseScrollDirection = 0;
const float InputHandler::MAX_ZOOM = 1000.0f;
const float InputHandler::MIN_ZOOM = 100.0f;
const float InputHandler::MOVE_SPEED = 2500.0f;
const float InputHandler::ZOOM_SPEED = 7.5f;
Vector2D InputHandler::m_MousePosition = Vector2D(0.0f, 0.0f);

void InputHandler::HandleInputEvents()
{
	m_RightMouseReleased = false;
	m_LeftMouseReleased = false;
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
						view.move(0.0f, -MOVE_SPEED * Time::DeltaTime());
						window->setView(view);
					}
				}
				if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left)
				{
					if (view.getCenter().x > sf::VideoMode::getDesktopMode().width * 0.1f)
					{
						view.move(-MOVE_SPEED * Time::DeltaTime(), 0.0f);
						window->setView(view);
					}
				}
				if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
				{
					if (view.getCenter().y < sf::VideoMode::getDesktopMode().height * 0.9f)
					{
						view.move(0.0f, MOVE_SPEED * Time::DeltaTime());
						window->setView(view);
					}
				}
				if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right)
				{
					if (view.getCenter().x < sf::VideoMode::getDesktopMode().width * 0.9f)
					{
						view.move(MOVE_SPEED * Time::DeltaTime(), 0.0f);
						window->setView(view);
					}
				}
				if (event.key.code == sf::Keyboard::Escape)
				{
					window->close();
				}
				if (event.key.code == sf::Keyboard::Space)
				{
					std::cout << "SpacePressed" << std::endl;
					std::cout << "PauseGame" << std::endl;
				}
				break;
			}
			case sf::Event::Resized:
			{
				break;
			}
			case sf::Event::GainedFocus:
			{
				std::cout << "ResumeGame" << std::endl;
				break;
			}
			case sf::Event::LostFocus:
			{
				std::cout << "PauseGame" << std::endl;
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
					sf::Vector2 worldPosition = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
					m_MousePosition.x = worldPosition.x;
					m_MousePosition.y = worldPosition.y;
				}
				if (event.key.code == sf::Mouse::Right)
				{
					m_RightMouseClicked = true;
					sf::Vector2 worldPosition = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
					m_MousePosition.x = worldPosition.x;
					m_MousePosition.y = worldPosition.y;
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
				}
				if (event.key.code == sf::Mouse::Right)
				{
					m_RightMouseClicked = false;
					m_RightMouseReleased = true;
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
						view.zoom(1.0f - m_MouseScrollDirection * ZOOM_SPEED * Time::DeltaTime());
						window->setView(view);
					}
				}
				break;
			}
			case sf::Event::MouseMoved:
			{
				break;
			}
			case sf::Event::MouseEntered:
			{
				std::cout << "ResumeGame" << std::endl;
				break;
			}
			case sf::Event::MouseLeft:
			{
				std::cout << "PauseGame" << std::endl;
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

int InputHandler::GetMouseScrollDirection()
{
	return m_MouseScrollDirection;
}

bool InputHandler::GetPlayerSelected()
{
	return m_PlayerSelected;
}

void InputHandler::SetPlayerSelected(bool selected)
{
	m_PlayerSelected = selected;
}
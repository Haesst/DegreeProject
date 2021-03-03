#include "InputHandler.h"
#include <iostream>
#include "Window.h"
#include "Time.h"

InputHandler::InputHandler(){}

InputHandler::~InputHandler(){}

bool InputHandler::m_LeftMouseClicked = false;
bool InputHandler::m_MouseScrollWheelChanged = false;
int InputHandler::m_MouseScrollDirection = 0;
const float InputHandler::MAX_ZOOM = 1000.0f;
const float InputHandler::MIN_ZOOM = 100.0f;
const float InputHandler::MOVE_SPEED = 10000.0f;
const float InputHandler::ZOOM_SPEED = 100.0f;
Vector2D InputHandler::m_MousePosition = Vector2D(0.0f, 0.0f);

void InputHandler::Initialize(){}

void InputHandler::HandleInputEvents()
{
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
				if (event.key.code == sf::Keyboard::W)
				{
					view.move(0.0f, -MOVE_SPEED * Time::DeltaTime());
					window->setView(view);
					printf("x: %f ", view.getCenter().x);
					printf("y: %f\n", view.getCenter().y);
				}
				if (event.key.code == sf::Keyboard::A)
				{
					view.move(-MOVE_SPEED * Time::DeltaTime(), 0.0f);
					window->setView(view);
					printf("x: %f ", view.getCenter().x);
					printf("y: %f\n", view.getCenter().y);
				}
				if (event.key.code == sf::Keyboard::S)
				{
					view.move(0.0f, MOVE_SPEED * Time::DeltaTime());
					window->setView(view);
					printf("x: %f ", view.getCenter().x);
					printf("y: %f\n", view.getCenter().y);
				}
				if (event.key.code == sf::Keyboard::D)
				{
					view.move(MOVE_SPEED * Time::DeltaTime(), 0.0f);
					window->setView(view);
					printf("x: %f ", view.getCenter().x);
					printf("y: %f\n", view.getCenter().y);
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
				sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
				window->setView(sf::View(visibleArea));
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
				//std::cout << "TextEntered" << std::endl;
				if (event.text.unicode < 128)
				{
					//std::cout << "ASCII: " << static_cast<char>(event.text.unicode) << std::endl;
				}
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				if (event.key.code == sf::Mouse::Left)
				{
					m_LeftMouseClicked = true;
					m_MousePosition.x = event.mouseButton.x;
					m_MousePosition.y = event.mouseButton.y;
				}
				if (event.key.code == sf::Mouse::Right)
				{
					m_MousePosition.x = event.mouseButton.x;
					m_MousePosition.y = event.mouseButton.y;
				}
				if (event.key.code == sf::Mouse::Middle)
				{
					//std::cout << "MiddleMousePressed" << std::endl;
				}
				if (event.key.code == sf::Mouse::XButton1)
				{
					//std::cout << "XButton1MousePressed" << std::endl;
				}
				if (event.key.code == sf::Mouse::XButton2)
				{
					//std::cout << "XButton2MousePressed" << std::endl;
				}
				break;
			}
			case sf::Event::MouseButtonReleased:
			{
				if (event.key.code == sf::Mouse::Left)
				{
					m_LeftMouseClicked = false;
					//std::cout << "LeftMouseReleased" << std::endl;
					//std::cout << "MouseX: " << event.mouseButton.x << std::endl;
					//std::cout << "MouseY: " << event.mouseButton.y << std::endl;
				}
				if (event.key.code == sf::Mouse::Right)
				{
					//std::cout << "RightMouseReleased" << std::endl;
					//std::cout << "MouseX: " << event.mouseButton.x << std::endl;
					//std::cout << "MouseY: " << event.mouseButton.y << std::endl;
				}
				if (event.key.code == sf::Mouse::Middle)
				{
					//std::cout << "MiddleMouseReleased" << std::endl;
				}
				if (event.key.code == sf::Mouse::XButton1)
				{
					//std::cout << "XButton1MouseReleased" << std::endl;
				}
				if (event.key.code == sf::Mouse::XButton2)
				{
					//std::cout << "XButton2MouseReleased" << std::endl;
				}
				break;
			}
			case sf::Event::MouseWheelScrolled:
			{
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
				{
					m_MouseScrollWheelChanged = true;
					m_MouseScrollDirection = event.mouseWheelScroll.delta;
					if (m_MouseScrollDirection == 1 && (view.getSize().x > MIN_ZOOM || view.getSize().y > MIN_ZOOM))
					{
						view.zoom(0.9f);
					}
					else if (m_MouseScrollDirection == -1 && (view.getSize().x < MAX_ZOOM || view.getSize().y < MAX_ZOOM))
					{
						view.zoom(1.1f);
					}
					window->setView(view);
					printf("x: %f ", view.getSize().x);
					printf("y: %f\n", view.getSize().y);
				}
				break;
			}
			case sf::Event::MouseMoved:
			{
				//std::cout << "NewMouseX: " << event.mouseMove.x << std::endl;
				//std::cout << "NewMouseY: " << event.mouseMove.y << std::endl;
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
				break;
		}
	}
}

bool InputHandler::GetLeftMouseClicked()
{
	return m_LeftMouseClicked;
}

bool InputHandler::GetMouseScrollWheelChanged()
{
	return m_MouseScrollWheelChanged;
}

void InputHandler::SetMouseScrollWheelChanged(bool changed)
{
	m_MouseScrollWheelChanged = changed;
}

Vector2D InputHandler::GetMousePosition()
{
	return m_MousePosition;
}

int InputHandler::GetMouseScrollDirection()
{
	return m_MouseScrollDirection;
}

#include "InputHandler.h"
#include <iostream>
#include "Window.h"

InputHandler::InputHandler(){}

InputHandler::~InputHandler(){}

bool InputHandler::leftMouseClicked = false;
bool InputHandler::mouseScrollWheelChanged = false;
int InputHandler::mouseScrollDirection = 0;
sf::Vector2<float> InputHandler::mousePosition = sf::Vector2(0.0f, 0.0f);

void InputHandler::Initialize()
{
	leftMouseClicked = false;
	mouseScrollWheelChanged = false;
	mouseScrollDirection = 0;
	mousePosition = sf::Vector2(0.0f, 0.0f);
}

void InputHandler::HandleInputEvents()
{
	sf::RenderWindow* window = Window::GetWindow();
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
					std::cout << "WPressed" << std::endl;
				}
				if (event.key.code == sf::Keyboard::A)
				{
					std::cout << "APressed" << std::endl;
				}
				if (event.key.code == sf::Keyboard::S)
				{
					std::cout << "SPressed" << std::endl;
				}
				if (event.key.code == sf::Keyboard::D)
				{
					std::cout << "DPressed" << std::endl;
				}
				if (event.key.code == sf::Keyboard::Escape)
				{
					window->close();
				}
				if (event.key.code == sf::Keyboard::Space)
				{
					std::cout << "SpacePressed" << std::endl;
				}
				break;
			}
			case sf::Event::Resized:
			{
				//window->setSize(sf::Vector2u(event.size.width, event.size.height));
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
					leftMouseClicked = true;
					mousePosition.x = event.mouseButton.x;
					mousePosition.y = event.mouseButton.y;
				}
				if (event.key.code == sf::Mouse::Right)
				{
					mousePosition.x = event.mouseButton.x;
					mousePosition.y = event.mouseButton.y;
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
					leftMouseClicked = false;
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
					mouseScrollWheelChanged = true;
					mouseScrollDirection = event.mouseWheelScroll.delta;
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
	return leftMouseClicked;
}

bool InputHandler::GetMouseScrollWheelChanged()
{
	return mouseScrollWheelChanged;
}

void InputHandler::SetMouseScrollWheelChanged(bool changed)
{
	mouseScrollWheelChanged = changed;
}

sf::Vector2<float> InputHandler::GetMousePosition()
{
	return mousePosition;
}

int InputHandler::GetMouseScrollDirection()
{
	return mouseScrollDirection;
}

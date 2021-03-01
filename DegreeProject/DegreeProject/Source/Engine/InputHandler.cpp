#include "InputHandler.h"
#include <iostream>

InputHandler::InputHandler(){}

InputHandler::~InputHandler(){}

void InputHandler::HandleInputEvents(sf::RenderWindow& window)
{
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
					window.close();
				}
				if (event.key.code == sf::Keyboard::Space)
				{
					std::cout << "SpacePressed" << std::endl;
				}
				break;
			}
			case sf::Event::Resized:
			{
				std::cout << "WindowResized" << std::endl;
				std::cout << "NewWidth: " << event.size.width << std::endl;
				std::cout << "NewHeight: " << event.size.height << std::endl;
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
				std::cout << "TextEntered" << std::endl;
				if (event.text.unicode < 128)
				{
					std::cout << "ASCII: " << static_cast<char>(event.text.unicode) << std::endl;
				}
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				if (event.key.code == sf::Mouse::Left)
				{
					std::cout << "LeftMousePressed" << std::endl;
					std::cout << "MouseX: " << event.mouseButton.x << std::endl;
					std::cout << "MouseY: " << event.mouseButton.y << std::endl;
				}
				if (event.key.code == sf::Mouse::Right)
				{
					std::cout << "RightMousePressed" << std::endl;
					std::cout << "MouseX: " << event.mouseButton.x << std::endl;
					std::cout << "MouseY: " << event.mouseButton.y << std::endl;
				}
				if (event.key.code == sf::Mouse::Middle)
				{
					std::cout << "MiddleMousePressed" << std::endl;
				}
				if (event.key.code == sf::Mouse::XButton1)
				{
					std::cout << "XButton1MousePressed" << std::endl;
				}
				if (event.key.code == sf::Mouse::XButton2)
				{
					std::cout << "XButton2MousePressed" << std::endl;
				}
				break;
			}
			case sf::Event::MouseButtonReleased:
			{
				if (event.key.code == sf::Mouse::Left)
				{
					std::cout << "LeftMouseReleased" << std::endl;
					std::cout << "MouseX: " << event.mouseButton.x << std::endl;
					std::cout << "MouseY: " << event.mouseButton.y << std::endl;
				}
				if (event.key.code == sf::Mouse::Right)
				{
					std::cout << "RightMouseReleased" << std::endl;
					std::cout << "MouseX: " << event.mouseButton.x << std::endl;
					std::cout << "MouseY: " << event.mouseButton.y << std::endl;
				}
				if (event.key.code == sf::Mouse::Middle)
				{
					std::cout << "MiddleMouseReleased" << std::endl;
				}
				if (event.key.code == sf::Mouse::XButton1)
				{
					std::cout << "XButton1MouseReleased" << std::endl;
				}
				if (event.key.code == sf::Mouse::XButton2)
				{
					std::cout << "XButton2MouseReleased" << std::endl;
				}
				break;
			}
			case sf::Event::MouseWheelScrolled:
			{
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
				{
					std::cout << "MouseVerticalWheelChanged" << std::endl;
					std::cout << "WheelMovement: " << event.mouseWheelScroll.delta << std::endl;
					std::cout << "MouseX: " << event.mouseWheelScroll.x << std::endl;
					std::cout << "MouseY: " << event.mouseWheelScroll.y << std::endl;
				}
				break;
			}
			case sf::Event::MouseMoved:
			{
				std::cout << "NewMouseX: " << event.mouseMove.x << std::endl;
				std::cout << "NewMouseY: " << event.mouseMove.y << std::endl;
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
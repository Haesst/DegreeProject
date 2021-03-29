#pragma once
#include <SFML/Graphics.hpp>

class Window
{
private:
	static sf::RenderWindow* m_Window;

public:
	static void init (sf::VideoMode videoMode, char* title, int styleMode)
	{
		m_Window = new sf::RenderWindow(videoMode, title, styleMode);
	}

	static sf::RenderWindow* getWindow()
	{
		return m_Window;
	}

	static void cleanup()
	{
	}

	~Window()
	{
		if (m_Window != nullptr)
		{
			delete m_Window;
		}
	}
};
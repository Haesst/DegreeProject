#pragma once
#include <SFML/Graphics.hpp>

class Window
{
private:
	static sf::RenderWindow* m_Window;

public:
	static void Init (sf::VideoMode videoMode, char* title, int styleMode)
	{
		m_Window = new sf::RenderWindow(videoMode, title, styleMode);
	}

	static sf::RenderWindow* GetWindow()
	{
		return m_Window;
	}

	static void Cleanup()
	{
		delete m_Window;
	}

	~Window()
	{
		Cleanup();
	}
};
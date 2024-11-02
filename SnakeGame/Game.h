#pragma once
#include "World.h"
#include "Snake.h"
#include "Window.h"
#include	"Textbox.h"
class Game
{

private:
	World m_world;
	Snake m_snake;
	Window m_window;
	sf::Time m_elapsed;
	Textbox m_textbox;


public:
	Game();
	void Update();
	void Render(sf::RenderWindow& l_window);
	~Game();
	void HandleInput();
	void Render();
	Window* GetWindow();
};


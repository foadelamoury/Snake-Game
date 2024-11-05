#include "Game.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Window.hpp"
#include <string>
#include <ctime> // Updated for modern C++ header style
#include "MainMenu.h"

Game::Game() : m_context(std:: make_shared<Context>())
{

	m_context->m_window->create(sf::VideoMode(1808, 608), "Professor Snake",sf::Style::Close);
	m_context->m_stateManager->Add(std::make_unique<MainMenu>(m_context));
}
Game::~Game() 
{
}

void Game::Run()
{
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	sf::Clock clock;
	sf::Time timeSinceLastFrame = sf::Time::Zero;

	while (m_context->m_window->isOpen())
	{


		timeSinceLastFrame += clock.restart();
		while (timeSinceLastFrame > TIME_PER_FRAME)
		{

			timeSinceLastFrame -= TIME_PER_FRAME;

			m_context->m_stateManager->ProcessStateChange();
			m_context->m_stateManager->GetCurrent()->ProcessInput();
			m_context->m_stateManager->GetCurrent()->Update(TIME_PER_FRAME);
			m_context->m_stateManager->GetCurrent()->Draw();






		}
	}


}

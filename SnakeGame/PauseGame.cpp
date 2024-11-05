#include "PauseGame.h"
#include <SFML/Window/Event.hpp>

PauseGame::PauseGame(std::shared_ptr<Context>& context)
{
}

PauseGame::~PauseGame()
{
}

void PauseGame::Init()
{
#pragma region Game Title
	m_context->m_assetManager->AddFont(MAIN_FONT, "Assets/Fonts/PixelifySans.ttf");
	m_pauseTitle.setFont(m_context->m_assetManager->GetFont(MAIN_FONT));
	m_pauseTitle.setString("Paused");
	m_pauseTitle.setCharacterSize(100);
	m_pauseTitle.setOrigin(m_pauseTitle.getLocalBounds().width / 2,
	m_pauseTitle.getLocalBounds().height / 2);
	m_pauseTitle.setPosition(m_context->m_window->getSize().x / 2,
		m_context->m_window->getSize().y / 2
		- 150.f
	);
#pragma endregion

}

void PauseGame::ProcessInput()
{
	sf::Event event;
	while (m_context->m_window->pollEvent(event))
	{

		if (event.type == sf::Event::Closed)
			m_context->m_window->close();
		else if (event.type == sf::Event::KeyPressed)
		{

			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
			{
				m_context->m_stateManager->PopCurrent();
				break;
			}
			
			default:
			{ 
				break; 
			}
			}


		}

	}
}

void PauseGame::Update(sf::Time deltaTime)
{
}

void PauseGame::Draw()
{
	m_context->m_window->clear(sf::Color::Black);
	m_context->m_window->draw(m_pauseTitle);

	m_context->m_window->display();
}

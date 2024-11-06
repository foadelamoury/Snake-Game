#include "Gameover.h"

#include "Gameplay.h"

#include <SFML/Window/Event.hpp>


Gameover::Gameover(std::shared_ptr<Context>& context)
	: m_context(context), m_retryButtonState(Selected),
	m_exitButtonState(None)
{


}

Gameover::~Gameover()
{
}

void Gameover::Init()
{

#pragma region Game Over Title
	m_context->m_assetManager->AddFont(MAIN_FONT, "Assets/Fonts/PixelifySans.ttf");
	m_gameoverTitle.setFont(m_context->m_assetManager->GetFont(MAIN_FONT));
	m_gameoverTitle.setString("Game Over!");
	m_gameoverTitle.setCharacterSize(100);
	m_gameoverTitle.setOrigin(m_gameoverTitle.getLocalBounds().width / 2,
	m_gameoverTitle.getLocalBounds().height / 2);
	m_gameoverTitle.setPosition(m_context->m_window->getSize().x / 2,
		m_context->m_window->getSize().y / 2
		- 150.f
	);
#pragma endregion

#pragma region Retry Button
	m_retryButton.setFont(m_context->m_assetManager->GetFont(MAIN_FONT));
	m_retryButton.setCharacterSize(50);
	m_retryButton.setFillColor(sf::Color::Magenta);
	m_retryButton.setString("Retry");
	m_retryButton.setOrigin(m_retryButton.getLocalBounds().width / 2,
	m_retryButton.getLocalBounds().height / 2);
	m_retryButton.setPosition(m_context->m_window->getSize().x / 2,
	m_context->m_window->getSize().y / 2
		- 25.f
	);

#pragma endregion

#pragma region Exit Button


	m_exitButton.setFont(m_context->m_assetManager->GetFont(MAIN_FONT));
	m_exitButton.setString("Exit");
	m_exitButton.setFillColor(sf::Color::Red);
	m_exitButton.setCharacterSize(50);

	m_exitButton.setOrigin(m_exitButton.getLocalBounds().width / 2,
	m_exitButton.getLocalBounds().height / 2);
	m_exitButton.setPosition(m_context->m_window->getSize().x / 2,
		m_context->m_window->getSize().y / 2
		+ 25.f
	);
#pragma endregion
}

void Gameover::ProcessInput()
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
			case sf::Keyboard::Up:
			{
				if (m_retryButtonState != Selected)
				{
					m_retryButtonState = Selected;
					m_exitButtonState = None;
				}
				break;
			}
			case sf::Keyboard::Down:
			{
				if (m_exitButtonState != Selected)
				{
					m_retryButtonState = None;
					m_exitButtonState = Selected;
				}
				break;
			}
			case sf::Keyboard::Return:
			{

				if (m_retryButtonState == Selected)
				{
					m_retryButtonState = Pressed;
				}
				else
				{

					m_exitButtonState = Pressed;
				}
				break;
			}
			default:
			{ break; }
			}


		}

	}
}

void Gameover::Update(const sf::Time& deltaTime)
{

	if (m_retryButtonState == Selected)
	{
		m_retryButton.setFillColor(sf::Color::Yellow);
		m_exitButton.setFillColor(sf::Color::Red);
	}

	else
	{
		m_exitButton.setFillColor(sf::Color::Yellow);

		m_retryButton.setFillColor(sf::Color::Magenta);
	}


	if (m_retryButtonState == Pressed)
	{
		m_context->m_stateManager->Add(std::make_unique<Gameplay>(m_context), true);
	}
	else if (m_exitButtonState == Pressed)
	{
		m_context->m_window->close();
	}
}

void Gameover::Draw()
{
	m_context->m_window->clear(sf::Color::Black);
	m_context->m_window->draw(m_gameoverTitle);
	m_context->m_window->draw(m_retryButton);
	m_context->m_window->draw(m_exitButton);

	m_context->m_window->display();
}

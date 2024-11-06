#include "MainMenu.h"
#include "Gameplay.h"
#include <SFML/Window/Event.hpp>

MainMenu::MainMenu(std::shared_ptr<Context>& context)
: m_context(context), m_playButtonState(Selected),
m_exitButtonState(None)
{
}

MainMenu::~MainMenu()
{
}

void MainMenu::Init()

{

#pragma region Game Title
	m_context->m_assetManager->AddFont(MAIN_FONT, "Assets/Fonts/PixelifySans.ttf");
	m_gameTitle.setFont(m_context->m_assetManager->GetFont(MAIN_FONT));
	m_gameTitle.setString("Professor Snake");
	m_gameTitle.setCharacterSize(100);
	m_gameTitle.setOrigin(m_gameTitle.getLocalBounds().width / 2,
		m_gameTitle.getLocalBounds().height / 2);
	m_gameTitle.setPosition(m_context->m_window->getSize().x / 2,
		m_context->m_window->getSize().y / 2 
		- 150.f
	);
#pragma endregion

#pragma region Play Button
	m_playButton.setFont(m_context->m_assetManager->GetFont(MAIN_FONT));
	m_playButton.setCharacterSize(50);
	m_playButton.setFillColor(sf::Color::Magenta);
	m_playButton.setString("Play");
	m_playButton.setOrigin(m_playButton.getLocalBounds().width / 2,
		m_playButton.getLocalBounds().height / 2);
	m_playButton.setPosition(m_context->m_window->getSize().x / 2,
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

void MainMenu::ProcessInput()
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
				if (m_playButtonState != Selected)
				{
					m_playButtonState = Selected;
					m_exitButtonState = None;
				}
				break;
			}
			case sf::Keyboard::Down:
			{
				if (m_exitButtonState != Selected)
				{
					m_playButtonState = None;
					m_exitButtonState = Selected;
				}
				break;
			}
			case sf::Keyboard::Return:
			{

				if (m_playButtonState == Selected)
				{
					m_playButtonState = Pressed;
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

void MainMenu::Update(const sf::Time& deltaTime)
{
	if (m_playButtonState == Selected) 
	{
		m_playButton.setFillColor(sf::Color::Yellow);
		m_exitButton.setFillColor(sf::Color::Red);
	}

	else
	{
		m_exitButton.setFillColor(sf::Color::Yellow);

		m_playButton.setFillColor(sf::Color::Magenta);
	}


	if (m_playButtonState == Pressed)
	{
		m_context->m_stateManager->Add(std::make_unique<Gameplay>(m_context), true);
	}
	else if (m_exitButtonState == Pressed)
	{
		m_context->m_window->close();
	}


}

void MainMenu::Draw()
{
	m_context->m_window->clear(sf::Color::Black);
	m_context->m_window->draw(m_gameTitle);
	m_context->m_window->draw(m_playButton);
	m_context->m_window->draw(m_exitButton);

	m_context->m_window->display();
}

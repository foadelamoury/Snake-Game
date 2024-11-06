#include "Gameplay.h"

#include "Gameover.h"
#include "PauseGame.h"
#include "Snake.h"


#include <SFML/Window/Event.hpp>

#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <chrono>
#include <thread>


using namespace std::this_thread; 
using namespace std::chrono;
#pragma region Clamp Definition

/// <summary>
/// Because std::Clamp is not working
/// </summary>
template <typename T>
T clamp(T value, T min, T max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

#pragma endregion

Gameplay::Gameplay(std::shared_ptr<Context>& context)
	: m_context(context) ,
	m_snakeDirection({ (m_snake.GetSpeed()),0.f}),
	m_elapsedTime(sf::Time::Zero),
	m_score(0),
	m_isPaused(false)
{
	srand(time(nullptr));
}

Gameplay::~Gameplay()
{
}

void Gameplay::Init()
{
#pragma region Adding Texture files to the program
	m_context->m_assetManager->AddTexture(GRASS, "C:/Game Development/C++/SnakeGame/Assets/Textures/grass.png", true);
	m_context->m_assetManager->AddTexture(FOOD, "C:/Game Development/C++/SnakeGame/Assets/Textures/food.png");
	m_context->m_assetManager->AddTexture(WALL, "C:/Game Development/C++/SnakeGame/Assets/Textures/wall.png", true);
	m_context->m_assetManager->AddTexture(SNAKE, "C:/Game Development/C++/SnakeGame/Assets/Textures/snake.png");
	m_context->m_assetManager->AddTexture(SNAKE_DAMAGED, "C:/Game Development/C++/SnakeGame/Assets/Textures/snakeDamaged.png");
	m_context->m_assetManager->AddTexture(SNAKE_CONFUSED, "C:/Game Development/C++/SnakeGame/Assets/Textures/snakeConfused.png");

#pragma endregion

#pragma region Grass Texture and Position

	m_grass.setTexture(m_context->m_assetManager->GetTexture(GRASS));
	m_grass.setTextureRect(m_context->m_window->getViewport(m_context->m_window->getDefaultView()));
#pragma endregion


#pragma region Walls Texture and Position

	m_walls[0].setTextureRect({ 0, 0, (int)m_context->m_window->getSize().x, 16 });
	m_walls[1].setTextureRect({ 0, 0, (int)m_context->m_window->getSize().x, 16 });
	m_walls[1].setPosition(0, m_context->m_window->getSize().y - 16);

	m_walls[2].setTextureRect({ 0, 0, 16, (int)m_context->m_window->getSize().y });
	m_walls[3].setTextureRect({ 0, 0, 16, (int)m_context->m_window->getSize().y });
	m_walls[3].setPosition(m_context->m_window->getSize().x - 16, 0);
#pragma endregion

#pragma region Food Texture and Position

	m_food.setTexture(m_context->m_assetManager->GetTexture(FOOD));
	m_food.setOrigin(m_food.getLocalBounds().width / 2, m_scoreText.getLocalBounds().height / 2);
	m_food.setPosition(m_context->m_window->getSize().x / 14, m_context->m_window->getSize().y / 14);
	//m_food.setColor(sf::Color::Black);
#pragma endregion


#pragma region Snake Texture and Position

	m_snake.Init(m_context->m_assetManager->GetTexture(SNAKE));
	
#pragma endregion

#pragma region Score Text

	m_scoreText.setFont(m_context->m_assetManager->GetFont(MAIN_FONT));
	m_scoreText.setString("Score: " + std::to_string(m_score));
	m_scoreText.setFillColor(sf::Color::White);
	m_scoreText.setCharacterSize(50);
	m_scoreText.setOrigin(m_scoreText.getLocalBounds().width / 2, m_scoreText.getLocalBounds().height / 2);
	m_scoreText.setPosition(m_context->m_window->getSize().x / 14, m_context->m_window->getSize().y / 14);
#pragma endregion


	for (auto& wall : m_walls)
	{

		wall.setTexture(m_context->m_assetManager->GetTexture(WALL));
	}


}

void Gameplay::ProcessInput()
{
	sf::Event event;
	while (m_context->m_window->pollEvent(event))
	{

		if (event.type == sf::Event::Closed)
			m_context->m_window->close();
	#pragma region Keyboard Input for moing snake
			else if (event.type == sf::Event::KeyPressed)
			{
				sf::Vector2f newDirection = m_snakeDirection;
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
					newDirection = { 0.f, -(m_snake.GetSpeed()) };
					break;
				case sf::Keyboard::Down:
					newDirection = { 0.f, (m_snake.GetSpeed()) };
					break;
				case sf::Keyboard::Left:
					newDirection = { -(m_snake.GetSpeed()), 0.f };
					break;
				case sf::Keyboard::Right:
					newDirection = { (m_snake.GetSpeed()), 0.f };
					break;
				case sf::Keyboard::Escape:
				m_context->m_stateManager->Add(std::make_unique<PauseGame>(m_context));
					break;
				}
				if(std::abs(m_snakeDirection.x) != std::abs(newDirection.x) || std::abs(m_snakeDirection.y) != std::abs(newDirection.y))
					m_snakeDirection = newDirection;
			}
	#pragma endregion

	}

}

void Gameplay::Update(const sf::Time& deltaTime)
{
	if (!m_isPaused)
	{
		m_elapsedTime += deltaTime;
		if (m_elapsedTime.asSeconds() > 0.1)
		{
			for (int i= 0 ; i<m_walls.size() ; i++)
			{
				
				if (m_snake.IsOn(m_walls[i]))
				{
					#pragma region Snake hits wall and changes color
					snakeColorClock.restart();
					ChangeSnakeDirection(m_snakeDirection, i);
					

					m_snake.ChangeTexture(m_context->m_assetManager->GetTexture(SNAKE_DAMAGED));

					snakeColorChanged = true;
					#pragma endregion

					//m_context->m_stateManager->Add(std::make_unique<Gameover>(m_context), true);
					break;
				}
			}

			if (m_snake.IsOn(m_food))
			{
				m_snake.Grow(m_snakeDirection);

				int x = clamp<int>(rand() % (int)m_context->m_window->getSize().x, 16, (int)m_context->m_window->getSize().x - 16);
				int y = clamp<int>(rand() % (int)m_context->m_window->getSize().y, 16, (int)m_context->m_window->getSize().y - 16);
				m_food.setPosition(x, y);

				m_score++;
				m_scoreText.setString("Score: " + std::to_string(m_score));
				#pragma region Score changing color

				m_scoreText.setFillColor(sf::Color::Green);
				scoreColorChanged = true;
				scoreColorClock.restart();
				#pragma endregion

			}
			else
			{
				m_snake.Move(m_snakeDirection);
			}

			if (scoreColorChanged && scoreColorClock.getElapsedTime().asSeconds() > 0.15)
			{
				m_scoreText.setFillColor(sf::Color::White);
				scoreColorChanged = false;
			}
			if (snakeColorChanged && snakeColorClock.getElapsedTime().asSeconds() > 0.5)
			{
				m_snake.ChangeTexture(m_context->m_assetManager->GetTexture(SNAKE));
				snakeColorChanged = false;
			}

			m_elapsedTime = sf::Time::Zero;
		}
	}
}


void Gameplay::Draw()
{
	m_context->m_window->clear();
	//m_context->m_window->draw(m_grass);
	for (auto& wall : m_walls)
	{
		m_context->m_window->draw(wall);

	}
	m_context->m_window->draw(m_food);
	
	m_context->m_window->draw(m_snake);
	
	
	m_context->m_window->draw(m_scoreText);



	m_context->m_window->display();



}

void Gameplay::Pause()
{
	m_isPaused = true;
}

void Gameplay::Start()
{
	m_isPaused = false;
}



#pragma region Snake Change Direction

void Gameplay::ChangeSnakeDirection(sf::Vector2f& direction, int i)
{
	if (i == 1 || i == 3)
		direction = { -(m_snake.GetSpeed()), -(m_snake.GetSpeed()) };
	else
		direction = { (m_snake.GetSpeed()), (m_snake.GetSpeed()) };
	m_snake.Move(direction);
}
#pragma endregion

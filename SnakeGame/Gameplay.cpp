#include "Gameplay.h"

#include "Gameover.h"

#include <SFML/Window/Event.hpp>

#include <stdlib.h>
#include <time.h>
#include <algorithm>



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
	: m_context(context) , m_snakeDirection({16.f,0.f}), m_elapsedTime(sf::Time::Zero)
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
	m_food.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2);
	//m_food.setColor(sf::Color::Black);
#pragma endregion


#pragma region Snake Texture and Position

	m_snake.Init(m_context->m_assetManager->GetTexture(SNAKE));
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
					newDirection = { 0.f, -16.f };
					break;
				case sf::Keyboard::Down:
					newDirection = { 0.f, 16.f };
					break;
				case sf::Keyboard::Left:
					newDirection = { -16.f, 0.f };
					break;
				case sf::Keyboard::Right:
					newDirection = { 16.f, 0.f };
					break;
				case sf::Keyboard::Escape:
					//pause
					break;
				}
				if(std::abs(m_snakeDirection.x) != std::abs(newDirection.x) || std::abs(m_snakeDirection.y) != std::abs(newDirection.y))
					m_snakeDirection = newDirection;
			}
	#pragma endregion

	}

}

void Gameplay::Update(sf::Time deltaTime)
{
	
	m_elapsedTime += deltaTime;
	if (m_elapsedTime.asSeconds() > 0.1)
	{
		bool isOnWall = false;
		for (auto& wall : m_walls)
		{
			if(m_snake.IsOn(wall))
			{

				m_context->m_stateManager->Add(std::make_unique<Gameover>(m_context),true);
				isOnWall = true;
				break;
			}
		}
		if (m_snake.IsOn(m_food)) 
		{
			m_snake.Grow(m_snakeDirection);

			int x=0, y=0;

			

			x= clamp<int>(rand() % (int)m_context->m_window->getSize().x, 16, (int)m_context->m_window->getSize().x - 16);

			y= clamp<int>(rand() % (int)m_context->m_window->getSize().y, 16, (int)m_context->m_window->getSize().y - 16);


			m_food.setPosition(x, y);
		}
		else
		{
		m_snake.Move(m_snakeDirection);
				
		}
		m_elapsedTime = sf::Time::Zero;
	}



}

void Gameplay::Draw()
{
	m_context->m_window->clear();
	m_context->m_window->draw(m_grass);
	for (auto& wall : m_walls)
	{
		m_context->m_window->draw(wall);

	}
	m_context->m_window->draw(m_food);
	
	m_context->m_window->draw(m_snake);


	m_context->m_window->display();



}

void Gameplay::Pause()
{
}

void Gameplay::Start()
{
}

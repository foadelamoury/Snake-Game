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
#include <fstream>
#include <vector>
#include <iostream>



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
	m_snakeDirection({ (m_snake.GetDirection()),0.f}),
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

#pragma region Map Textures

	m_context->m_assetManager->AddTexture(WALL, "C:/Game Development/C++/SnakeGame/Assets/Textures/wall.png", true);
	m_context->m_assetManager->AddTexture(STONE, "C:/Game Development/C++/SnakeGame/Assets/Textures/stone.png", true);
#pragma endregion

	
#pragma region Snake different Status effects
	m_context->m_assetManager->AddTexture(SNAKE, "C:/Game Development/C++/SnakeGame/Assets/Textures/snake.png");
	m_context->m_assetManager->AddTexture(SNAKE_DAMAGED, "C:/Game Development/C++/SnakeGame/Assets/Textures/snakeDamaged.png");
	m_context->m_assetManager->AddTexture(SNAKE_CONFUSED, "C:/Game Development/C++/SnakeGame/Assets/Textures/snakeConfused.png");
#pragma endregion



	m_context->m_assetManager->AddTexture(FOOD, "C:/Game Development/C++/SnakeGame/Assets/Textures/food.png");
#pragma region Apples with Effects
	m_context->m_assetManager->AddTexture(GOLDEN_APPLE, "C:/Game Development/C++/SnakeGame/Assets/Textures/goldenApple.png");
	m_context->m_assetManager->AddTexture(POISONED_APPLE, "C:/Game Development/C++/SnakeGame/Assets/Textures/poisonedApple.png");
#pragma endregion



#pragma region Moving Enemies
	m_context->m_assetManager->AddTexture(ENEMY_GOBLIN, "C:/Game Development/C++/SnakeGame/Assets/Textures/enemy1.png");
	m_context->m_assetManager->AddTexture(ENEMY_DOG, "C:/Game Development/C++/SnakeGame/Assets/Textures/enemy2.png");


#pragma endregion
	


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
	m_food.setOrigin(m_food.getLocalBounds().width / 2, m_food.getLocalBounds().height / 2);
	//m_food.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 4);
	m_food.setPosition(m_context->m_window->getSize().x / 12, m_context->m_window->getSize().y -180);

#pragma endregion

#pragma region Golden Food Texture and Position

	m_golden_food.setTexture(m_context->m_assetManager->GetTexture(GOLDEN_APPLE));
	m_golden_food.setOrigin(m_golden_food.getLocalBounds().width / 2, m_golden_food.getLocalBounds().height / 2);
	//m_golden_food.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 4);
	m_golden_food.setPosition(m_context->m_window->getSize().x / 12, m_context->m_window->getSize().y -200);

#pragma endregion

#pragma region Poisoned Food Texture and Position
	
	m_poisoned_food.setTexture(m_context->m_assetManager->GetTexture(POISONED_APPLE));
	m_poisoned_food.setOrigin(m_poisoned_food.getLocalBounds().width / 2, m_poisoned_food.getLocalBounds().height / 2);
	m_poisoned_food.setPosition(m_context->m_window->getSize().x / 4, m_context->m_window->getSize().y / 2);
	//m_food.setColor(sf::Color::Black);
#pragma endregion



#pragma region Snake Texture and Position and Health

	m_snake.Init(m_context->m_assetManager->GetTexture(SNAKE));
	
	m_snake.SetHealth(100);

#pragma endregion



#pragma region Enemy Goblin
	m_goblin.setTexture(m_context->m_assetManager->GetTexture(ENEMY_GOBLIN));
	m_goblin.setOrigin(m_goblin.getLocalBounds().width / 2, m_goblin.getLocalBounds().height / 2);
	m_goblin.setPosition(m_context->m_window->getSize().x - 500, m_context->m_window->getSize().y - 400);

	m_goblinDirection = { 0.f, -16.f };

#pragma endregion

#pragma region Enemy Dog
	m_dog.setTexture(m_context->m_assetManager->GetTexture(ENEMY_DOG));
	m_dog.setOrigin(m_dog.getLocalBounds().width / 2, m_dog.getLocalBounds().height / 2);
	m_dog.setPosition(m_context->m_window->getSize().x - 700, m_context->m_window->getSize().y - 230);
	m_dogDirection = { -16.f, 0.f };
	

#pragma endregion



#pragma region Health Text
	m_healthText.setFont(m_context->m_assetManager->GetFont(MAIN_FONT));
	m_healthText.setString("Health: " + std::to_string(m_snake.GetHealth()+1));
	m_healthText.setFillColor(sf::Color::White);
	m_healthText.setCharacterSize(50);
	m_healthText.setOrigin(m_healthText.getLocalBounds().width / 2, m_healthText.getLocalBounds().height / 2);
	m_healthText.setPosition(m_context->m_window->getSize().x -160, m_context->m_window->getSize().y - 600);

#pragma endregion

#pragma region Score Text

	m_scoreText.setFont(m_context->m_assetManager->GetFont(MAIN_FONT));
	m_scoreText.setString("Score: " + std::to_string(m_score));
	m_scoreText.setFillColor(sf::Color::White);
	m_scoreText.setCharacterSize(50);
	m_scoreText.setOrigin(m_scoreText.getLocalBounds().width / 2, m_scoreText.getLocalBounds().height / 2);
	m_scoreText.setPosition(m_context->m_window->getSize().x / 18, m_context->m_window->getSize().y / 60);
#pragma endregion
	

	for (auto& wall : m_walls)
	{

		wall.setTexture(m_context->m_assetManager->GetTexture(WALL));
	}
	LoadWallsFromTextFile("C:/Game Development/C++/SnakeGame/Assets/Maps/walls.txt");


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
					if(snakePoisonedColorChanged) // this is where the snake gets poisoned
						newDirection = { 0.f, (m_snake.GetDirection())  };
					else
					newDirection = { 0.f, -(m_snake.GetDirection()) };
					break;
				case sf::Keyboard::Down:
					if (snakePoisonedColorChanged)  // this is where the snake gets poisoned
					newDirection = { 0.f, -(m_snake.GetDirection()) };
					else
					newDirection = { 0.f, (m_snake.GetDirection()) };
					break;
				case sf::Keyboard::Left:
					if (snakePoisonedColorChanged)  // this is where the snake gets poisoned
					newDirection = { (m_snake.GetDirection()), 0.f };

					else
					newDirection = { -(m_snake.GetDirection()), 0.f };
					break;
				case sf::Keyboard::Right:
					if (snakePoisonedColorChanged)  // this is where the snake gets poisoned
						newDirection = { -(m_snake.GetDirection()), 0.f };
					else
					newDirection = { (m_snake.GetDirection()), 0.f };
					break;
				case sf::Keyboard::Escape:
				m_context->m_stateManager->Add(std::make_unique<PauseGame>(m_context));
					break;
				}
				if(std::abs(m_snakeDirection.x) != std::abs(newDirection.x) || std::abs(m_snakeDirection.y) != std::abs(newDirection.y))
					m_snakeDirection = newDirection /* m_snake.GetSpeed()*/
					;
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
			MoveEnemy(m_goblinDirection, m_goblin);

			#pragma region dog chasing snake

			sf::Vector2f dogPosition = m_dog.getPosition();
			sf::Vector2f snakeHeadPosition = m_snake.GetSnakeHead().getPosition();
			sf::Vector2f direction = snakeHeadPosition - dogPosition;

			float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
			if (magnitude != 0) {
				direction = direction / magnitude;
			}

			float dogSpeed = 600.f * deltaTime.asSeconds();
			sf::Vector2f newDogPosition = dogPosition + direction * dogSpeed;

			m_dog.setPosition(newDogPosition);
			if (IsCollidingWithWall(m_dog))
			{
				m_dog.setPosition(dogPosition);
				MoveEnemy(m_dogDirection, m_dog);
				m_dogDirection = ChangeEnemyDirection(m_dog, m_dogDirection);


			}
#pragma endregion
			
			m_goblinDirection = ChangeEnemyDirection(m_goblin, m_goblinDirection);

			for (int i= 0 ; i<m_v_walls.size() ; i++) // it changed from m_walls
			{
				
				if (m_snake.IsOn(m_v_walls[i]))
				{
					#pragma region Snake hits wall and changes color
					snakeColorClock.restart();
					
					ChangeSnakeDirection(m_snakeDirection, i);
					
	
					m_snake.ChangeTexture(m_context->m_assetManager->GetTexture(SNAKE_DAMAGED));

					snakeColorChanged = true;
					#pragma endregion
					m_snake.SetHealth(m_snake.GetHealth() - 1);
					m_healthText.setString("Health: " + std::to_string(m_snake.GetHealth() + 1));

					if(m_snake.GetHealth() < 0)
					m_context->m_stateManager->Add(std::make_unique<Gameover>(m_context), true);
					break;
				}
			}
			if (m_snake.IsOn(m_goblin))
			{
				#pragma region Snake hits goblin and changes color
				snakeColorClock.restart();

				ChangeSnakeDirection(m_snakeDirection, m_goblin);


				m_snake.ChangeTexture(m_context->m_assetManager->GetTexture(SNAKE_DAMAGED));

				snakeColorChanged = true;
				#pragma endregion
				m_snake.SetHealth(m_snake.GetHealth() - 1);
				m_healthText.setString("Health: " + std::to_string(m_snake.GetHealth() + 1));

				if (m_snake.GetHealth() < 0)
					m_context->m_stateManager->Add(std::make_unique<Gameover>(m_context), true);
			}

			if (m_snake.IsOn(m_dog))
			{
				#pragma region Snake hits goblin and changes color
				snakeColorClock.restart();

				ChangeSnakeDirection(m_snakeDirection, m_dog);


				m_snake.ChangeTexture(m_context->m_assetManager->GetTexture(SNAKE_DAMAGED));

				snakeColorChanged = true;
				#pragma endregion
				m_snake.SetHealth(m_snake.GetHealth() - 1);
				m_healthText.setString("Health: " + std::to_string(m_snake.GetHealth() + 1));

				if (m_snake.GetHealth() < 0)
					m_context->m_stateManager->Add(std::make_unique<Gameover>(m_context), true);
			}
			if (m_snake.IsOn(m_food))
			{
				m_snake.Grow(m_snakeDirection);
				m_snake.SetHealth(m_snake.GetHealth() + 1);
				m_healthText.setString("Health: " + std::to_string(m_snake.GetHealth() + 1));
				for (int i = 0; i < m_v_walls.size(); i++) // it changed from m_walls
				{
					ChangeAppleSpawn(m_food);
				}

				m_score++;
				m_scoreText.setString("Score: " + std::to_string(m_score));
				#pragma region Score changing color

				m_scoreText.setFillColor(sf::Color::Green);
				scoreColorChanged = true;
				scoreColorClock.restart();
				#pragma endregion

			}

			

			
			if (m_snake.IsOn(m_golden_food))
			{
				for (int i = 0; i < 5; i++)
					m_snake.Grow(m_snakeDirection);


				for (int i = 0; i < m_v_walls.size(); i++) // it changed from m_walls
				{
					ChangeAppleSpawn(m_golden_food);
				}

				m_score+=5;
				m_scoreText.setString("Score: " + std::to_string(m_score));
				#pragma region Score changing color

					m_scoreText.setFillColor(sf::Color::Yellow);
					scoreColorChanged = true;
					scoreColorClock.restart();
				#pragma endregion

			}

			if (m_snake.IsOn(m_poisoned_food))
			{
				

				for (int i = 0; i < m_v_walls.size(); i++) // it changed from m_walls
				{
					ChangeAppleSpawn(m_poisoned_food);
				}
				#pragma region Snake hits wall and changes color
				snakePoisonedColorClock.restart();


				m_snake.ChangeTexture(m_context->m_assetManager->GetTexture(SNAKE_CONFUSED));

				snakePoisonedColorChanged = true;

				#pragma endregion
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
			if (snakeColorChanged && snakeColorClock.getElapsedTime().asSeconds() > 0.13)
			{
				if(snakePoisonedColorChanged)
					m_snake.ChangeTexture(m_context->m_assetManager->GetTexture(SNAKE_CONFUSED));
				else
					m_snake.ChangeTexture(m_context->m_assetManager->GetTexture(SNAKE));
				snakeColorChanged = false;
			}
			if (snakePoisonedColorChanged && snakePoisonedColorClock.getElapsedTime().asSeconds() > 3.8)
			{
				m_snake.ChangeTexture(m_context->m_assetManager->GetTexture(SNAKE));
				snakePoisonedColorChanged = false;
			}
			m_elapsedTime = sf::Time::Zero;
		}
	}
}


void Gameplay::Draw()
{
	m_context->m_window->clear();
	//m_context->m_window->draw(m_grass);


	/*for (auto& wall : m_walls)
	{
		m_context->m_window->draw(wall);

	}*/

	for (auto& wall : m_v_walls)
	{
		m_context->m_window->draw(wall);

	}
	m_context->m_window->draw(m_food);
	m_context->m_window->draw(m_poisoned_food);
	m_context->m_window->draw(m_golden_food);

	
	m_context->m_window->draw(m_snake);
	
	
	m_context->m_window->draw(m_scoreText);
	m_context->m_window->draw(m_healthText);

	m_context->m_window->draw(m_goblin);
	m_context->m_window->draw(m_dog);





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

void Gameplay::LoadWallsFromTextFile(const std::string& filename)
{
	std::ifstream inputFile(filename);
	if (!inputFile.is_open()) {
		std::cerr << "Error: Could not open " << filename << std::endl;
		return;
	}

	std::vector<std::string> map;
	std::string line;

	while (std::getline(inputFile, line)) {
		map.push_back(line);
	}
	inputFile.close();

	int rows = map.size();
	int cols = rows > 0 ? map[0].size() : 0; 
	if (cols == 0) return;

	float cellWidth = m_context->m_window->getSize().x / cols;
	float cellHeight = m_context->m_window->getSize().y / rows;

	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < map[row].size(); ++col) {
			if (map[row][col] == '*') {
				sf::Sprite wall;
				wall.setTexture(m_context->m_assetManager->GetTexture(WALL));
				wall.setPosition(col * cellWidth, row * cellHeight);
				wall.setTextureRect({ 0, 0, static_cast<int>(cellWidth), static_cast<int>(cellHeight) });
				m_v_walls.push_back(wall);
			}
			else if (map[row][col] == '&') {
				sf::Sprite stone;
				stone.setTexture(m_context->m_assetManager->GetTexture(STONE));
				stone.setPosition(col * cellWidth, row * cellHeight);
				stone.setTextureRect({ 0, 0, static_cast<int>(cellWidth), static_cast<int>(cellHeight) });
				m_v_walls.push_back(stone);
			}
		}
	}
}



#pragma region Snake Change Direction

void Gameplay::ChangeSnakeDirection(sf::Vector2f& direction, int i)
{
	sf::FloatRect wallBounds = m_v_walls[i].getGlobalBounds();
	sf::FloatRect snakeBounds = m_snake.GetSnakeHead().getGlobalBounds();


	if (snakeBounds.top < wallBounds.top + wallBounds.height && snakeBounds.top + snakeBounds.height > wallBounds.top)
	{
		direction.x = -direction.x;
	}
	if (snakeBounds.left < wallBounds.left + wallBounds.width && snakeBounds.left + snakeBounds.width > wallBounds.left)
	{
		direction.y = -direction.y;
	}
	m_snake.Move(direction);
}

void Gameplay::ChangeSnakeDirection(sf::Vector2f& direction, sf::Sprite enemy)
{
	sf::FloatRect enemyBounds = enemy.getGlobalBounds();
	sf::FloatRect snakeBounds = m_snake.GetSnakeHead().getGlobalBounds();


	if (snakeBounds.top < enemyBounds.top + enemyBounds.height && snakeBounds.top + snakeBounds.height > enemyBounds.top)
	{
		direction.x = -direction.x;
	}
	if (snakeBounds.left < enemyBounds.left + enemyBounds.width && snakeBounds.left + snakeBounds.width > enemyBounds.left)
	{
		direction.y = -direction.y;
	}
	m_snake.Move(direction);
}
//void Gameplay::ChangeEnemyDirection(sf::Vector2f& direction, int i)

#pragma endregion

#pragma region Change Apple Spawn

void Gameplay::ChangeAppleSpawn(sf::Sprite & food)
{
	bool isColliding;
	do {
		int x = clamp<int>(rand() % (int)m_context->m_window->getSize().x, 16, (int)m_context->m_window->getSize().x - 16);
		int y = clamp<int>(rand() % (int)m_context->m_window->getSize().y, 16, (int)m_context->m_window->getSize().y - 16);
		food.setPosition(x, y);

		isColliding = false;
		for (const auto& wall : m_v_walls) 
		{
			if (food.getGlobalBounds().intersects(wall.getGlobalBounds())) {
				isColliding = true;
				break;
			}
		}

	} while (isColliding);  
}


#pragma endregion

#pragma region Enemy Movement


void Gameplay::MoveEnemy(sf::Vector2f& direction , sf::Sprite & enemy)
{
	enemy.setPosition(enemy.getPosition() + direction);
	
}
#pragma endregion

#pragma region Change Enemy Direction

sf::Vector2f Gameplay::ChangeEnemyDirection(sf::Sprite& enemy, sf::Vector2f  enemyDirection)
{

	sf::FloatRect enemyBounds = enemy.getGlobalBounds();
	sf::Vector2f newDirection = enemyDirection;

	for (const auto& wall : m_v_walls)
	{
		sf::FloatRect wallBounds = wall.getGlobalBounds();

		if (enemyBounds.intersects(wallBounds))
		{
			if (enemyDirection.x != 0) {
				newDirection.x = -newDirection.x;

			}
			if (enemyDirection.y != 0) newDirection.y = -newDirection.y;
			break;
		}
	}
	return newDirection;

}
#pragma endregion


bool Gameplay::IsCollidingWithWall(sf::Sprite& enemy) {
	for (const auto& wall : m_v_walls) {
		if (enemy.getGlobalBounds().intersects(wall.getGlobalBounds())) {
			return true;
		}
	}
	return false;
}



//• A “structured programming” version(50m)



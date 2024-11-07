#pragma once
#include <memory>
#include <array>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


#include "Game.h"
#include "State.h"
#include "Snake.h"


class Gameplay : public Engine::State
{
private:
	std::shared_ptr<Context> m_context;
	sf::Sprite m_grass;

	sf::Sprite m_food;
	sf::Sprite m_golden_food;
	sf::Sprite m_poisoned_food;

	std::array<sf::Sprite, 4> m_walls;

	std::vector<sf::Sprite> m_v_walls;


	sf::Sprite m_goblin;
	sf::Sprite m_dog;

	sf::Vector2f m_goblinDirection;
	sf::Vector2f m_dogDirection;

	bool isWallHitDog;
	bool isWallHitGoblin;



	

	Snake m_snake;
	bool snakeColorChanged = false;
	sf::Clock snakeColorClock;

	bool snakePoisonedColorChanged = false;
	sf::Clock snakePoisonedColorClock;

	sf::Text m_healthText;


	sf::Text m_scoreText;
	int m_score;
	sf::Clock scoreColorClock;
	bool scoreColorChanged = false;

	sf::Vector2f m_snakeDirection;

	

	sf::Time m_elapsedTime;
	bool m_isPaused;


	
public:
	Gameplay(std::shared_ptr<Context>& context);
	~Gameplay();
	void Init() override;
	void ProcessInput() override;
	void Update(const sf::Time& deltaTime) override;
	void Draw() override;
	void Pause() override;
	void Start() override;


#pragma region Walls from file


	void LoadWallsFromGridFile(const std::string& filename);

#pragma endregion



#pragma region Snake Change Direction

	void ChangeSnakeDirection(sf::Vector2f & direction, int i);

	void ChangeSnakeDirection(sf::Vector2f& direction, sf::Sprite enemy);

#pragma endregion


#pragma region Change Apple Spawn

	void ChangeAppleSpawn(sf::Sprite& food);

#pragma endregion

#pragma region Enemy Movement

	void MoveEnemy(sf::Vector2f& direction, sf::Sprite& enemy);

	sf::Vector2f ChangeEnemyDirection(sf::Sprite& enemy, sf::Vector2f  enemyDirection);

#pragma endregion

};


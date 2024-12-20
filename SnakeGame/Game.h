#pragma once
#include <memory>

#include "SFML/Graphics/RenderWindow.hpp"

#include "StateManager.h"
#include "AssetManager.h"


enum AssetID
{
	MAIN_FONT = 0,
	GRASS,
	FOOD,
	WALL,
	SNAKE,
	SNAKE_DAMAGED,
	SNAKE_CONFUSED,
	STONE,
	POISONED_APPLE,
	GOLDEN_APPLE, 
	ENEMY_GOBLIN,
	ENEMY_DOG
};

struct Context
{
	std::unique_ptr<Engine::StateManager> m_stateManager;
	std::unique_ptr<Engine::AssetManager> m_assetManager;
	std::unique_ptr<sf::RenderWindow> m_window;
	
	Context()
	{
		m_stateManager = std::make_unique<Engine::StateManager>();
		m_assetManager = std::make_unique<Engine::AssetManager>();
		m_window = std::make_unique<sf::RenderWindow>();
	}
};
class Game
{

private:
	/// <summary>
	/// Context struct will be common for all states
	/// </summary>
	
	std::shared_ptr<Context> m_context; 
	const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 60.f);  // 60 fps


public:
	Game();

	~Game();

	void Run();
	
};


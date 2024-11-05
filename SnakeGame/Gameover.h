#pragma once
#include <memory>

#include <SFML/Graphics/Text.hpp>

#include "State.h"

#include "Game.h"

enum ButtonState
{
	None,
	Pressed,
	Selected,

};
class Gameover : public Engine::State
{

private:
	std::shared_ptr<Context> m_context;
	sf::Text m_gameoverTitle;

	sf::Text m_retryButton;
	sf::Text m_exitButton;

	ButtonState m_retryButtonState;
	ButtonState m_exitButtonState;


public:
	Gameover(std::shared_ptr<Context>& context);
	~Gameover();

	void Init() override;
	void ProcessInput() override;
	void Update(sf::Time deltaTime) override;
	void Draw() override;



};


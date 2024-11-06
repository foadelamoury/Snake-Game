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
class MainMenu : public Engine::State
{

	private:
		std::shared_ptr<Context> m_context;
		sf:: Text m_gameTitle;

		sf::Text m_playButton;
		sf::Text m_exitButton;

		ButtonState m_playButtonState;
		ButtonState m_exitButtonState;


public:
	MainMenu(std::shared_ptr<Context>& context);
	~MainMenu();

	void Init() override;
	void ProcessInput() override;
	void Update(const sf::Time& deltaTime) override;
	void Draw() override;



};


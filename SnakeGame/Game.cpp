#include "Game.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "World.h"
#include "Snake.h"
#include "SnakeDependencies.h"
#include <string>
#include <ctime> // Updated for modern C++ header style

Game::Game()
    : m_window("Professor Snake", sf::Vector2u(800, 600)),
    m_snake(m_world.GetBlockSize()),
    m_world(sf::Vector2u(800, 600))
{
    m_textbox.Setup(5, 14, 350, sf::Vector2f(225, 0));
    m_textbox.Add("Seeded random number generator with: " + std::to_string(std::time(nullptr)));
}

void Game::HandleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_snake.GetDirection() != Direction::Down) {
        m_snake.SetDirection(Direction::Up);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_snake.GetDirection() != Direction::Up) {
        m_snake.SetDirection(Direction::Down);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && m_snake.GetDirection() != Direction::Right) {
        m_snake.SetDirection(Direction::Left);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && m_snake.GetDirection() != Direction::Left) {
        m_snake.SetDirection(Direction::Right);
    }
}

void Game::Update() {
    HandleInput(); // Ensure input is handled during each update

    float timestep = 1.0f / m_snake.GetSpeed();
    if (m_elapsed.asSeconds() >= timestep) {
        m_snake.Tick();
        m_world.Update(m_snake);
        // m_elapsed -= timestep; // Uncomment and ensure proper resetting if needed
        if (m_snake.HasLost()) {
            m_snake.Reset();
        }
    }
}

void Game::Render() {
    m_window.BeginDraw();
    m_world.Render(m_window.GetRenderWindow());
    m_snake.Render(m_window.GetRenderWindow());
    m_textbox.Render(m_window.GetRenderWindow());
    m_window.EndDraw();
}

Game::~Game() {}

#include "Window.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <iostream>
#include <string>

// Constructor with default parameters
Window::Window() {
    Setup("Professor Snake", sf::Vector2u(640, 480));
}

// Constructor with custom parameters
Window::Window(const std::string& l_title, const sf::Vector2u& l_size) {
    Setup(l_title, l_size);
}

// Destructor to ensure window is properly closed
Window::~Window() {
    Destroy();
}

// Private setup method
void Window::Setup(const std::string& l_title, const sf::Vector2u& l_size) {
    m_windowTitle = l_title;
    m_windowSize = l_size;
    m_isFullscreen = false;
    m_isDone = false;
    Create();
}

// Method to create the window
void Window::Create() {
    auto style = (m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
    m_window.create({ m_windowSize.x, m_windowSize.y, 32 }, m_windowTitle, style);
}

// Corrected method to return the render window directly
sf::RenderWindow& Window::GetRenderWindow() {
    return m_window;
}

// Method to destroy/close the window
void Window::Destroy() {
    if (m_window.isOpen()) {
        m_window.close();
    }
}

// Event handling and window updates
void Window::Update() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_isDone = true;
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5) {
            ToggleFullscreen();
        }
    }
}

// Toggle between fullscreen and windowed mode
void Window::ToggleFullscreen() {
    m_isFullscreen = !m_isFullscreen;
    Destroy();
    Create();
}

// Clear the window before drawing
void Window::BeginDraw() {
    m_window.clear(sf::Color::Black);
}

// Display the drawn content
void Window::EndDraw() {
    m_window.display();
}

// Check if the window is marked as done/closed
bool Window::IsDone() {
    return m_isDone;
}

// Check if the window is in fullscreen mode
bool Window::IsFullscreen() {
    return m_isFullscreen;
}

// Get the size of the window
sf::Vector2u Window::GetWindowSize() {
    return m_windowSize;
}

// Draw a drawable object to the window
void Window::Draw(sf::Drawable& l_drawable) {
    m_window.draw(l_drawable);
}

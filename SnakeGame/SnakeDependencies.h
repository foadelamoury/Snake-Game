#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include <iostream>
#include "SFML/System.hpp"

struct SnakeSegment {
	SnakeSegment(int x, int y) : position(x, y) {}
	sf::Vector2i position;
};
using SnakeContainer = std::vector<SnakeSegment>;




enum class Direction { None, Up, Down, Left, Right };


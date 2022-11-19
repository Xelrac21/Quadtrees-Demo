#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Puck.h"
#include "Quadtree.h"

#define framerate 60.0f
#define TIMESTEP 1.0f/framerate

class Puck;

void moveToScreen(sf::Vector2f& position);
void normalize(sf::Vector2f& vector);
void setKeyboardMovement(sf::Vector2f& movementDirection);

static float width = 2000;
static float height = 1000;
static float globalFriction = -10;
static std::vector<Puck*> objectPool;
static std::vector<Puck*> objectsOnScreen;
//static std::vector<sf::RectangleShape> quads;

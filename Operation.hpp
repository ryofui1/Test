#pragma once
#include <SFML/Graphics.hpp>
#include "main.hpp"

extern void Operation(sf::Time deltaTime);
extern void ResolveEnemyCollision(Enemy& e1, Enemy& e2);
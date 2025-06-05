#include <SFML/Graphics.hpp>
#include <iostream>
#include "Operation.hpp"
#include "main.hpp"

void Operation(sf::Time deltaTime) {
    player.move(deltaTime.asSeconds()*10);
    for (auto& enemy : enemyList) {
        enemy.move(deltaTime.asSeconds()*10);
    }
    player.checkCollisionWithGround(groundY);
}
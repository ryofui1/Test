#include <SFML/Graphics.hpp>
#include <iostream>
#include "Draw.hpp"
#include "main.hpp"

void Draw(sf::Time deltaTime) {
    window.clear(sf::Color::Cyan);
    window.draw(ground);
    player.draw(deltaTime.asSeconds()*10);
    window.display();
}
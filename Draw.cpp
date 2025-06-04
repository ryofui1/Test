#include <SFML/Graphics.hpp>
#include <iostream>
#include "Draw.hpp"
#include "main.hpp"

void Draw(sf::Time deltaTime) {
    window.clear(sf::Color::Cyan);
    window.draw(ground);
    window.draw(player.shape);
    window.display();
}
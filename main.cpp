#include "main.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.hpp"

int main() {
    defValues();
    std::cout << "HelloWorld!!!!!!!!!!!!" << std::endl;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Mario-like Action Game");

    Player player;
    float groundY = 550.f;

    
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        window.clear(sf::Color::Cyan);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.move(deltaTime.asSeconds() * 10);
        player.checkCollisionWithGround(groundY);

        sf::RectangleShape ground(sf::Vector2f(800.f, 50.f));
        ground.setFillColor(sf::Color::Green);
        ground.setPosition(0.f, groundY);

        window.draw(ground);
        window.draw(player.shape);
        window.display();
    }

    return 0;
}

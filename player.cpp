#include "Player.hpp"
#include <iostream>

Player::Player() {
    shape.setSize(sf::Vector2f(50.f, 50.f));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(100.f, 300.f);
    jumpForce = -0.4;
    onGround = false;
    velocityY = 0.f;
}
void Player::move(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        shape.move(-speed * deltaTime, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        shape.move(speed * deltaTime, 0.f);
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && onGround) {
        velocityY = jumpForce;
        onGround = false;
        std::cout << "jump!!" << std::endl;
    }

    velocityY += 0.2f * deltaTime;
    shape.move(0.f, velocityY);
}

void Player::checkCollisionWithGround(float groundY) {
    if (shape.getPosition().y + shape.getSize().y > groundY) {
        shape.setPosition(shape.getPosition().x, groundY - shape.getSize().y);
        velocityY = 0.f;
        onGround = true;
    }
}

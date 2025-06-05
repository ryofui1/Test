#include "Player.hpp"
#include <iostream>

Player::Player() {
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color::Black);
    shape.setPosition(100.f, 300.f);
    velocityY = 0.f;
    velocityX = 0.f;
}
void Player::move(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        shape.move(-speedX * deltaTime, 0.f);
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        shape.move(speedX * deltaTime, 0.f);
    }
    
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))) {
        shape.move(0.f, -speedY * deltaTime);
    }
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))) {
        shape.move(0.f, speedY * deltaTime);
    }

    if (velocityY < -velocityDamping) {
        velocityY += velocityDamping * deltaTime;
    } else if (velocityY > velocityDamping) {
        velocityY -= velocityDamping * deltaTime;
    } else{
        velocityY = 0.f;
    }
   
    if (velocityX < -velocityDamping) {
        velocityX += velocityDamping * deltaTime;
    } else if (velocityX > velocityDamping) {
        velocityX -= velocityDamping * deltaTime;
    } else{
        velocityX = 0.f;
    }
    
    shape.move(velocityX, velocityY);
    std::cout << shape.getPosition().x << ", " << shape.getPosition().y << std::endl;
}

void Player::checkCollisionWithGround(float groundY) {
   
}

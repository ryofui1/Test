#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    sf::RectangleShape shape;
    float speed = 1.f;
    float jumpForce;
    bool onGround;
    float velocityY;

    Player();
    void move(float deltaTime);
    void checkCollisionWithGround(float groundY);
};

#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
public:
    sf::RectangleShape shape;
    // 敵のタイプを数で指定
    int EnemyType;

    // 敵のAIのタイプを数で指定 1: 追跡, 2: 傍観, -1: 不動
    int AItype;

    float sizeX ;
    float sizeY ;
    float speed ; 

    Enemy(int enemyType);
    void move(float deltaTime);
    void draw(float deltaTime);
};

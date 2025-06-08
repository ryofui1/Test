#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.hpp"

class Enemy : public Collider {
public:
    sf::RectangleShape shape;
    // 敵のタイプを数で指定
    int EnemyType;

    // 敵のAIのタイプを数で指定 1: 追跡, 2: 傍観, -1: 不動
    int AItype;

    float sizeX ;
    float sizeY ;
    float speed ; 

    float velocityX; // X方向の速度
    float velocityY; // Y方向の速度
    float velocityDamping = 2.f; // 速度減衰の値

    Enemy(int enemyType);
    void move(float deltaTime);
    void draw(float deltaTime);
    void velocitySet(float vx, float vy); // 速度を設定する
    void velocityAdd(float vx, float vy); // 速度を加える
    void velocityAccessible(float vx, float vy); // 速度を近づける    
    sf::FloatRect getBounds() const override { return shape.getGlobalBounds(); }
    void move(const sf::Vector2f& offset) override { shape.move(offset); }
private:
    sf::Vector2f velocity; // 速度を保持するための変数
    bool isSpacePressed;
};

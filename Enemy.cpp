#include "Enemy.hpp"
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "main.hpp"
#include <iostream>
#include <cmath>

Enemy::Enemy(int enemyType) {
    EnemyType = enemyType;
    if (EnemyType == 0) {
        sizeX = 50.f;
        sizeY = 50.f;
        speed = 30.f;
        AItype = 1;
    } else if (EnemyType == 1) {
        sizeX = 100.f;
        sizeY = 100.f;
        speed = 50.f;
        AItype = 2;
    } else {
        sizeX = 0.f;
        sizeY = 0.f;
        speed = 0.f;
        AItype = -1;
    }
    shape.setSize(sf::Vector2f(sizeX, sizeY));
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(100.f, 100.f);
}
void Enemy::move(float deltaTime) {
    if (AItype == 1) {
        // 追跡AI
        sf::Vector2f playerPosition = player.shape.getPosition(); // プレイヤーの位置を取得
        sf::Vector2f direction = playerPosition - shape.getPosition();
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            direction /= length;    // 正規化
            shape.move(direction * speed * deltaTime); // 速度100で移動
        }
    } else if (AItype == 2) {
        // 傍観AI
    } else if (AItype == -1) {
        // 不動AI
    }
}
void Enemy::draw(float deltaTime) {
    window.draw(shape);
}
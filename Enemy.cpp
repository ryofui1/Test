#include "Enemy.hpp"
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "main.hpp"
#include <iostream>
#include <cmath>
#include <random>

// Enemy.cpp
// std::random_device rd;  // ハードウェア乱数生成器
// std::mt19937 mt(rd());   // メルセンヌ・ツイスタ法の生成器
// // 0から99までの一様分布の乱数を生成
// std::uniform_int_distribution<int> dist(10, 35);

Enemy::Enemy(int enemyType) {
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(0.f, 0.f);
    knockbackResistance = 0.1f; // ノックバック耐性の初期値
    speed = 20.f;
    EnemyType = enemyType;
    if (EnemyType == 0) {
        sizeX = 50.f;
        sizeY = 50.f;
        // speed = dist(mt);
        AItype = 1;
        knockbackResistance = 0.1f;
    } else if (EnemyType == 1) {
        sizeX = 100.f;
        sizeY = 100.f;
        speed = 50.f;
        AItype = 2;
        shape.setPosition(100.f,100.f);
    } else {
        sizeX = 0.f;
        sizeY = 0.f;
        speed = 0.f;
        AItype = -1;
    }

    shape.setSize(sf::Vector2f(sizeX, sizeY));
    velocityX = 0.f; // X方向の初期速度
    velocityY = 0.f; // Y方向の初期速度
    
    isSpacePressed = false;
}

void Enemy::velocityAccessible(float vx, float vy) {
    // X方向
    velocityX += (vx - velocityX) * 0.1;
    // Y方向
    velocityY += (vy - velocityY) * 0.1;
}
void Enemy::velocitySet(float vx, float vy) {
    // X方向
    velocityX = vx;
    // Y方向
    velocityY = vy;
}
void Enemy::velocityAdd(float vx, float vy) {
    // X方向
    velocityX += vx;
    // Y方向
    velocityY += vy;
}

void Enemy::move(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isSpacePressed) {
        velocityAdd(-velocityX*10, -velocityY*10); // スペースキーで反転
        isSpacePressed = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        isSpacePressed = false;
    }

    if (velocityY < -velocityDamping) {
        velocityY += velocityDamping ;
    } else if (velocityY > velocityDamping) {
        velocityY -= velocityDamping ;
    } else{
        velocityY = 0.f;
    }
    if (velocityX < -velocityDamping) {
        velocityX += velocityDamping;
    } else if (velocityX > velocityDamping) {
        velocityX -= velocityDamping;
    } else{
        velocityX = 0.f;
    }
    if (AItype == 1) {
        // 追跡AI
        sf::Vector2f playerPosition = player.shape.getPosition(); // プレイヤーの位置を取得
        sf::Vector2f direction = playerPosition - shape.getPosition();
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            direction /= length;    // 正規化
            sf::Vector2f newVelocity = direction * speed;
            float newSpeed = std::sqrt(newVelocity.x * newVelocity.x + newVelocity.y * newVelocity.y);
            float currentSpeed = std::sqrt(velocityX * velocityX + velocityY * velocityY);

            // 新しい速度ベクトルの大きさが現状より大きい場合のみ上書き
            if (newSpeed > currentSpeed) {
                velocitySet(newVelocity.x, newVelocity.y);
            }
            shape.move(velocityX*deltaTime,velocityY*deltaTime); // 速度100で移動
        }
    } else if (AItype == 2) {
        // 傍観AI
    } else if (AItype == -1) {
        // 不動AI
    }
    //std::cout << "Enemy Position: " << shape.getPosition().x << ", " << shape.getPosition().y << std::endl;
    //std::cout << "Enemy Velocity: " << velocityX << ", " << velocityY << std::endl;
}
void Enemy::draw(float deltaTime) {
    window.draw(shape);

    // 速度ベクトルの大きさ
    float speedNorm = std::sqrt(velocityX * velocityX + velocityY * velocityY);
    if (speedNorm > 1e-4f) { // 速度がほぼ0でなければ描画
        // 矢印の長さ
        float arrowLength = 40.f;
        sf::Vector2f start = shape.getPosition() + sf::Vector2f(sizeX / 2, sizeY / 2);
        sf::Vector2f dir = sf::Vector2f(velocityX, velocityY) / speedNorm;
        sf::Vector2f end = start + dir * arrowLength;

        // 矢印本体（線分）
        sf::Vertex line[] = {
            sf::Vertex(start, sf::Color::Yellow),
            sf::Vertex(end, sf::Color::Yellow)
        };
        window.draw(line, 2, sf::Lines);

        // 矢印の先端（三角形）
        float headSize = 10.f;
        sf::Vector2f left = end + sf::Vector2f(-dir.y, dir.x) * (headSize / 2.f) - dir * headSize;
        sf::Vector2f right = end + sf::Vector2f(dir.y, -dir.x) * (headSize / 2.f) - dir * headSize;
        sf::ConvexShape arrowHead;
        arrowHead.setPointCount(3);
        arrowHead.setPoint(0, end);
        arrowHead.setPoint(1, left);
        arrowHead.setPoint(2, right);
        arrowHead.setFillColor(sf::Color::Yellow);
        window.draw(arrowHead);
    }
}
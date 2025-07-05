#include "Object.hpp"
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "main.hpp"
#include "Debug.hpp"
#include <iostream>
#include <cmath>
// Object.cpp
// std::random_device rd;  // ハードウェア乱数生成器
// std::mt19937 mt(rd());   // メルセンヌ・ツイスタ法の生成器
// // 0から99までの一様分布の乱数を生成
// std::uniform_int_distribution<int> dist(10, 35);

Object::Object(int objectType, int movetype, float positionX, float postionY, float Angle, float Damage ,float KnockbackPower) {
    ColliderType = 2;
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(positionX, postionY);
    knockbackResistance = 0.f; // ノックバック耐性の初期値
    speed = 0.f;
    lifeTime = -100.f;

    angle = Angle;

    hp = 1;
    damage = Damage; // ダメージ量の初期値
    knockbackPower = KnockbackPower; // ノックバック力の初期値

    MoveType = movetype; // 移動タイプの初期値
    velocityDamping = 0.f; // 速度減衰の初期
    ObjectType = objectType;
    if (ObjectType == 0) {
        sizeX = 50.f;
        sizeY = 100.f;
        speed = 1000.f;
        lifeTime = 0.5f;
    } else if (ObjectType == 1) {
        sizeX = 50.f;
        sizeY = 50.f;
        speed = 700.f;
        lifeTime = 1.0f;
    } else {
        sizeX = 0.f;
        sizeY = 0.f;
        speed = 0.f;
    }

    shape.setSize(sf::Vector2f(sizeX, sizeY));
    velocityX = speed * cos(angle * 3.1415f / 180.f); // X方向の初期速度
    velocityY = speed * sin(angle * 3.1415f / 180.f); // Y方向の初期速度
}

void Object::velocityAccessible(float vx, float vy) {
    // X方向
    velocityX += (vx - velocityX) * 0.1;
    // Y方向
    velocityY += (vy - velocityY) * 0.1;
}
void Object::velocitySet(float vx, float vy) {
    // X方向
    velocityX = vx;
    // Y方向
    velocityY = vy;
}
void Object::velocityAdd(float vx, float vy) {
    // X方向
    velocityX += vx;
    // Y方向
    velocityY += vy;
}
int Object::oneDamage(){
    return(damage); 
}
float Object::oneKnockbackPower() {
    return knockbackPower;
}
void Object::move(float deltaTime) {
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

    if (MoveType == 0) {
        shape.move(velocityX * deltaTime, velocityY * deltaTime);
        lifeTime -= deltaTime;
        

    } else if (MoveType == 1) {
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
    }
}
void Object::draw(float deltaTime) {
    window.draw(shape);

    // // 速度ベクトルの大きさ
    // float speedNorm = std::sqrt(velocityX * velocityX + velocityY * velocityY);
    // if (speedNorm > 1e-4f) { // 速度がほぼ0でなければ描画
    //     // 矢印の長さ
    //     float arrowLength = 40.f;
    //     sf::Vector2f start = shape.getPosition() + sf::Vector2f(sizeX / 2, sizeY / 2);
    //     sf::Vector2f dir = sf::Vector2f(velocityX, velocityY) / speedNorm;
    //     sf::Vector2f end = start + dir * arrowLength;

    //     // 矢印本体（線分）
    //     sf::Vertex line[] = {
    //         sf::Vertex(start, sf::Color::Yellow),
    //         sf::Vertex(end, sf::Color::Yellow)
    //     };
    //     window.draw(line, 2, sf::Lines);

    //     // 矢印の先端（三角形）
    //     float headSize = 10.f;
    //     sf::Vector2f left = end + sf::Vector2f(-dir.y, dir.x) * (headSize / 2.f) - dir * headSize;
    //     sf::Vector2f right = end + sf::Vector2f(dir.y, -dir.x) * (headSize / 2.f) - dir * headSize;
    //     sf::ConvexShape arrowHead;
    //     arrowHead.setPointCount(3);
    //     arrowHead.setPoint(0, end);
    //     arrowHead.setPoint(1, left);
    //     arrowHead.setPoint(2, right);
    //     arrowHead.setFillColor(sf::Color::Yellow);
    //     window.draw(arrowHead);
    // }
}

void Object::contact(Collider& other){
    debugPrint("contact damage:" + std::to_string(damage));
    if (ObjectType == 0 && other.ColliderType == 1) {
        hp -= 1;
    } else if (ObjectType == 1 && other.ColliderType == 0) {
        hp -= 1;
    }
}

bool Object::isDestroy() {
    if (hp <= 0 || (lifeTime != -100.f && lifeTime <= 0.f)) {
        return(true);
    } else {
        return(false);
    }
}
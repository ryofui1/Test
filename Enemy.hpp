#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.hpp"
#include "Object.hpp"
#include <vector>


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

    float maxHp;
    float hp;
    bool isDisappear();
    std::vector<std::vector<int>> timeTable; // 行動管理用 時間+内容
    std::vector<bool> timeTableIsComplete; // 行動管理用 完了
    float activeTime; //行動管理用の時間
    int phase; //行動の順序

    float velocityX; // X方向の速度
    float velocityY; // Y方向の速度
    float velocityDamping = 400.f; // 速度減衰の値

    Enemy(int enemyType);
    void move(float deltaTime);
    void contactPlayerAttack(Object enemyAttack);
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

#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.hpp"
#include "Object.hpp"
#include <iostream>
#include <string>

const std::string PlayerPhoto = "Test/photo/test/iconnew.png";

class Player : public Collider {
public:
    //定数はここで宣言+定義,変数(計算に使う変わる数など)はここで宣言のみ 
    //hppの変更はexeの再生成に寄与しない
    //なので、cppで一度何か変更してからF5すること

    // Player バトル要素関係
    float maxHp;
    float hp;
    
    //プレイヤーの画像
    sf::RectangleShape shape; // 当たり判定用
    sf::Texture texture;      // 画像データ
    sf::Sprite sprite;        // 画像表示用

    //プレイヤーのサイズ
    float width = 50.f;
    float height = 50.f;
    
    //プレイヤーの移動速度
    float speedX = 500.f;
    float speedY = 450.0f;

    //プレイヤーの方向
    int playerDirection;
    int playerBeforeDirection;

    //プレイヤーが受けている力
    float velocityY;
    float velocityX;

    //プレイヤーの減速率
    float velocityDamping = 10000.0f;

    bool isSpacePressed;
    bool canTurn;

    Player();
    void move(float deltaTime);
    void draw(float deltaTime);
    void contactEnemyAttack(Object enemyAttack);
    void velocitySet(float vx, float vy); // 速度を設定する
    void velocityAdd(float vx, float vy); // 速度を加える
    void velocityAccessible(float vx, float vy); // 速度を近づける 
    sf::FloatRect getBounds() const override { return shape.getGlobalBounds(); }
    void move(const sf::Vector2f& offset) override { shape.move(offset); }
};

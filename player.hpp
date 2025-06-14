#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.hpp"

class Player : public Collider {
public:
    //定数はここで宣言+定義,変数(計算に使う変わる数など)はここで宣言のみ 
    //hppの変更はexeの再生成に寄与しない
    //なので、cppで一度何か変更してからF5すること

    
    //プレイヤーの画像
    sf::RectangleShape shape; // 当たり判定用
    sf::Texture texture;      // 画像データ
    sf::Sprite sprite;        // 画像表示用

    //プレイヤーのサイズ
    float width = 50.f;
    float height = 50.f;
    
    //プレイヤーの移動速度
    float speedX = 50.f;
    float speedY = 45.f;
    
    //プレイヤーが受けている力
    float velocityY;
    float velocityX;

    //プレイヤーの減速率
    float velocityDamping = 0.2f;

    Player();
    void move(float deltaTime);
    void draw(float deltaTime);
    sf::FloatRect getBounds() const override { return shape.getGlobalBounds(); }
    void move(const sf::Vector2f& offset) override { shape.move(offset); }
};

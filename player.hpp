#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    sf::RectangleShape shape;
    //定数はここで宣言+定義,変数(計算に使う変わる数など)はここで宣言のみ 
    //hppの変更はexeの再生成に寄与しない
    //なので、cppで一度何か変更してからF5すること

    
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
    void checkCollisionWithGround(float groundY);
};

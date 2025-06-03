#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    sf::RectangleShape shape;
    //定数はここで宣言+定義,変数(計算に使う変わる数など)はここで宣言のみ 
    //hppの変更はexeの再生成に寄与しない
    //なので、cppで一度何か変更してからF5すること
    //分かったby田中
    float speed = 50.f;
    float jumpForce = -0.4;
    bool onGround;
    float velocityY;

    Player();
    void move(float deltaTime);
    void checkCollisionWithGround(float groundY);
};

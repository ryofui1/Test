#include "Player.hpp"
#include "main.hpp"
#include "Debug.hpp"
#include "Object.hpp"
#include <iostream>
#include <cmath>
// Player.cpp
Player::Player() {
    ColliderType = 0;
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color::Black);
    shape.setPosition(100.f, 300.f);

    if (!texture.loadFromFile("Test/photo/test/iconnew.png")) {
        std::cout << "Error_FUZAKERUNA" << std::endl;
    } else {
        std::cout << "OK" << std::endl;
    }
    
    sprite.setTexture(texture);
    sprite.setPosition(shape.getPosition());
    velocityY = 0.f;
    velocityX = 0.f;
    playerDirection = 1;
    playerBeforeDirection = 1;
    knockbackResistance = 1.0f; // ノックバック耐性の初期値

    maxHp = 100;
    hp = maxHp;

    isSpacePressed = false;
    canTurn = true;
}

void Player::contactEnemyAttack(Object enemyAttack) {
    hp -= enemyAttack.damage;
    velocityAdd(
        enemyAttack.knockbackPower * cos(enemyAttack.angle * 3.1415f / 180.f),
        enemyAttack.knockbackPower * sin(enemyAttack.angle * 3.1415f / 180.f)
    );
}


void Player::move(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::V) && !isSpacePressed) {
        ObjectList_PlayerAttack.push_back(Object(
            0,0,shape.getPosition().x + 50.f*playerDirection,
            shape.getPosition().y,
            90*playerDirection-90,
            20.f,300.f));
        isSpacePressed = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
        isSpacePressed = false;
    }
    debugPrint("deltaTime:" + std::to_string(deltaTime));
    debugPrint("ObjectList:" + std::to_string(ObjectList_PlayerAttack.size()));
    // Bキーのトグル処理
    static bool prevBPressed = false;
    bool nowBPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::B);
    if (nowBPressed && !prevBPressed) {
        isDebug = !isDebug;
        // debugPrint("isDebug");
    }
    prevBPressed = nowBPressed;

    // WASDキーの押下状態
    bool nowAPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool nowDPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    bool nowWPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    bool nowSPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

    // 移動処理
    float currentSpeed = std::sqrt(velocityX * velocityX + velocityY * velocityY ) *0.8 ;
    if (nowAPressed && currentSpeed <= speedX) {velocitySet(-speedX, velocityY); if (canTurn) playerDirection=-1;}
    if (nowDPressed && currentSpeed <= speedX) {velocitySet(speedX, velocityY); if (canTurn) playerDirection=1;}
    if (nowWPressed && currentSpeed <= speedY) {velocitySet(velocityX, -speedY);}
    if (nowSPressed && currentSpeed <= speedY) {velocitySet(velocityX, speedY);}

    debugPrint("Player Position: " + std::to_string(shape.getPosition().x) + ", " + std::to_string(shape.getPosition().y));
    debugPrint("Player velocities:" + std::to_string(velocityX) + "," + std::to_string(velocityY));

    // velocityの減衰処理（必要なら）
    if (velocityY < -velocityDamping * deltaTime) {
        velocityY += velocityDamping * deltaTime;
    } else if (velocityY > velocityDamping * deltaTime) {
        velocityY -= velocityDamping * deltaTime;
    } else{
        velocityY = 0.f;
    }

    if (velocityX < -velocityDamping * deltaTime) {
        velocityX += velocityDamping * deltaTime;
    } else if (velocityX > velocityDamping * deltaTime) {
        velocityX -= velocityDamping * deltaTime;
    } else{
        velocityX = 0.f;
    }
    
    shape.move(velocityX * deltaTime, velocityY * deltaTime);

    //std::cout << shape.getPosition().x << ", " << shape.getPosition().y << std::endl;
}

void Player::draw(float deltaTime) {
    if (playerDirection == 1) {
        sprite.setScale(-shape.getSize().x / texture.getSize().x, shape.getSize().y / texture.getSize().y);
        sprite.setPosition(sprite.getGlobalBounds().width + shape.getPosition().x, shape.getPosition().y);
        playerBeforeDirection *= -1;
    } else {
        sprite.setScale(shape.getSize().x / texture.getSize().x, shape.getSize().y / texture.getSize().y);
        sprite.setPosition(shape.getPosition());
    }
    
    window.draw(sprite);

    // --- HPバー描画 ---
    float barWidth = shape.getSize().x;
    float barHeight = 10.f;
    float hpRatio = std::max(0.f, std::min(1.f, hp / static_cast<float>(maxHp)));
    sf::Vector2f barPos = shape.getPosition() + sf::Vector2f(0, -barHeight - 6.f);

    sf::RectangleShape backBar(sf::Vector2f(barWidth, barHeight));
    backBar.setPosition(barPos);
    backBar.setFillColor(sf::Color(80, 80, 80)); // グレー

    sf::RectangleShape hpBar(sf::Vector2f(barWidth * hpRatio, barHeight));
    hpBar.setPosition(barPos);
    hpBar.setFillColor(sf::Color::Green);

    window.draw(backBar);
    window.draw(hpBar);

    if (isDebug) {
        // 枠線のみ黒、塗りつぶしは透明
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(2.f); // 枠線の太さ
        shape.setOutlineColor(sf::Color::Black);
        window.draw(shape);

        // 元の塗りつぶし色に戻す（次回以降の描画のため）
        shape.setFillColor(sf::Color::Black);
        shape.setOutlineThickness(0.f);
    }
}

void Player::velocityAccessible(float vx, float vy) {
    // X方向
    velocityX += (vx - velocityX) * 0.1;
    // Y方向
    velocityY += (vy - velocityY) * 0.1;
}
void Player::velocitySet(float vx, float vy) {
    // X方向
    velocityX = vx;
    // Y方向
    velocityY = vy;
}
void Player::velocityAdd(float vx, float vy) {
    // X方向
    velocityX += vx;
    // Y方向
    velocityY += vy;
}
#include "Player.hpp"
#include "main.hpp"
#include <iostream>

// Player.cpp
Player::Player() {
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
    knockbackResistance = 1.0f; // ノックバック耐性の初期値
}

void Player::move(float deltaTime) {
    // Bキーのトグル処理
    static bool prevBPressed = false;
    bool nowBPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::B);
    if (nowBPressed && !prevBPressed) {
        isDebug = !isDebug;
        std::cout << "isDebug: " << isDebug << std::endl;
    }
    prevBPressed = nowBPressed;

    // WASDキーの押下状態
    static bool prevAPressed = false;
    static bool prevDPressed = false;
    static bool prevWPressed = false;
    static bool prevSPressed = false;

    bool nowAPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool nowDPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    bool nowWPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    bool nowSPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

    // 移動処理
    if (nowAPressed) shape.move(-speedX * deltaTime, 0.f);
    if (nowDPressed) shape.move(speedX * deltaTime, 0.f);
    if (nowWPressed) shape.move(0.f, -speedY * deltaTime);
    if (nowSPressed) shape.move(0.f, speedY * deltaTime);

    // isDebug時、押した瞬間だけ座標表示
    if (isDebug) {
        if (nowAPressed && !prevAPressed) {
            std::cout << shape.getPosition().x << ", " << shape.getPosition().y << std::endl;
        }
        if (nowDPressed && !prevDPressed) {
            std::cout << shape.getPosition().x << ", " << shape.getPosition().y << std::endl;
        }
        if (nowWPressed && !prevWPressed) {
            std::cout << shape.getPosition().x << ", " << shape.getPosition().y << std::endl;
        }
        if (nowSPressed && !prevSPressed) {
            std::cout << shape.getPosition().x << ", " << shape.getPosition().y << std::endl;
        }
    }

    prevAPressed = nowAPressed;
    prevDPressed = nowDPressed;
    prevWPressed = nowWPressed;
    prevSPressed = nowSPressed;

    // velocityの減衰処理（必要なら）
    if (velocityY < -velocityDamping) {
        velocityY += velocityDamping * deltaTime;
    } else if (velocityY > velocityDamping) {
        velocityY -= velocityDamping * deltaTime;
    } else{
        velocityY = 0.f;
    }

    if (velocityX < -velocityDamping) {
        velocityX += velocityDamping * deltaTime;
    } else if (velocityX > velocityDamping) {
        velocityX -= velocityDamping * deltaTime;
    } else{
        velocityX = 0.f;
    }
    
    shape.move(velocityX, velocityY);

    //std::cout << shape.getPosition().x << ", " << shape.getPosition().y << std::endl;
}

void Player::draw(float deltaTime) {
    sprite.setPosition(shape.getPosition());
    sprite.setScale(shape.getSize().x / texture.getSize().x, shape.getSize().y / texture.getSize().y);
    
    window.draw(sprite);

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

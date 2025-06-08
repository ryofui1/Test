#pragma once
#include <SFML/Graphics.hpp>

class Collider {
public:
    float knockbackResistance = 1.0f; // ノックバック耐性（大きいほど動きにくい）

    virtual sf::FloatRect getBounds() const = 0;
    virtual void move(const sf::Vector2f& offset) = 0;
    virtual ~Collider() = default;
};
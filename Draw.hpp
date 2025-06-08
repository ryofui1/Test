#pragma once
#include <SFML/Graphics.hpp>
#include "main.hpp"

extern void Draw(sf::Time deltaTime);
extern std::vector<std::pair<sf::Vector2f, sf::Vector2f>> collisionLines; // 当たり判定計算したペアを線で描画するための変数 Draw.hpp

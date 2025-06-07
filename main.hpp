#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.hpp"
#include "Enemy.hpp"
#include <list>
#include <vector>
// 初期化内容(変数宣言,代入のみ) ↓

// window関連
extern sf::RenderWindow window;
constexpr float ScreenWidth = 800.f;
constexpr float ScreenHeight = 600.f;

// Player関連
extern Player player;
constexpr float groundY = 550.f;

//Enemy関連
extern std::list<Enemy> enemyList;

//地面
extern sf::RectangleShape ground;

// // その他
// clock
extern sf::Clock myClock;

extern void defValue();

extern std::vector<std::pair<sf::Vector2f, sf::Vector2f>> collisionLines;
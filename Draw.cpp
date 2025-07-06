#include <SFML/Graphics.hpp>
#include <iostream>
#include "Draw.hpp"
#include "main.hpp"

// 格子状タイルを描画
void DrawGridTiles() {
    for (int y = 0; y < TileRows; ++y) {
        for (int x = 0; x < TileCols; ++x) {
            sf::RectangleShape tile(sf::Vector2f(TileSize, TileSize));
            tile.setFillColor(sf::Color(100, 200, 100)); // 緑色
            tile.setOutlineThickness(1.f);
            tile.setOutlineColor(sf::Color::Black);
            tile.setPosition(x * TileSize, y * TileSize);
            window.draw(tile);
        }
    }
}

// 壁タイルを描画（左右端）
void DrawSideWalls() {
    for (int y = 0; y < TileRows; ++y) {
        // 左
        sf::RectangleShape wallL(sf::Vector2f(TileSize, TileSize));
        wallL.setFillColor(sf::Color(150, 150, 150));
        wallL.setOutlineThickness(1.f);
        wallL.setOutlineColor(sf::Color::Black);
        wallL.setPosition(0, y * TileSize);
        window.draw(wallL);
        // 右
        sf::RectangleShape wallR(sf::Vector2f(TileSize, TileSize));
        wallR.setFillColor(sf::Color(150, 150, 150));
        wallR.setOutlineThickness(1.f);
        wallR.setOutlineColor(sf::Color::Black);
        wallR.setPosition((TileCols - 1) * TileSize, y * TileSize);
        window.draw(wallR);
    }
}

void Draw(sf::Time deltaTime) {
    window.clear(sf::Color::Cyan);

    // 格子状タイル
    DrawGridTiles();
    // 壁
    DrawSideWalls();

    window.draw(ground);
    player.draw(deltaTime.asSeconds() * 10);

    for (auto& enemy : enemyList) {
        enemy.draw(deltaTime.asSeconds() * 10);
    }

    for (const auto& line : collisionLines) {
        sf::Vertex vertices[2] = {
            sf::Vertex(line.first, sf::Color::Magenta),
            sf::Vertex(line.second, sf::Color::Magenta)
        };
        window.draw(vertices, 2, sf::Lines);
    }

    window.display();
}
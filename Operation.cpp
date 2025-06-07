#include <SFML/Graphics.hpp>
#include <iostream>
#include "Operation.hpp"
#include "main.hpp"
#include <cmath>
#include <vector>
#include <algorithm>

// 空間分割用パラメータ
const int gridCols = 8; // 横分割数
const int gridRows = 6; // 縦分割数
const float gridWidth = ScreenWidth;   // ゲーム画面の幅
const float gridHeight = ScreenHeight; // ゲーム画面の高さ
const float cellWidth = gridWidth / gridCols;
const float cellHeight = gridHeight / gridRows;

std::vector<std::pair<sf::Vector2f, sf::Vector2f>> collisionLines;

void Operation(sf::Time deltaTime) {
    player.move(deltaTime.asSeconds()*10);
    for (auto& enemy : enemyList) {
        enemy.move(deltaTime.asSeconds()*10);
    }

    // 空間分割グリッドの作成
    std::vector<std::vector<std::vector<Enemy*>>> grid(gridCols, std::vector<std::vector<Enemy*>>(gridRows));
    for (auto& enemy : enemyList) {
        sf::Vector2f pos = enemy.shape.getPosition();
        int col = std::clamp(static_cast<int>(pos.x / cellWidth), 0, gridCols - 1);
        int row = std::clamp(static_cast<int>(pos.y / cellHeight), 0, gridRows - 1);
        grid[col][row].push_back(&enemy);
    }
    
    collisionLines.clear();
    // 各セル内と隣接セルのエネミー同士で判定
    for (int col = 0; col < gridCols; ++col) {
        for (int row = 0; row < gridRows; ++row) {
            // 対象セルと周囲8セル
            for (int dcol = -1; dcol <= 1; ++dcol) {
                for (int drow = -1; drow <= 1; ++drow) {
                    int ncol = col + dcol;
                    int nrow = row + drow;
                    if (ncol < 0 || ncol >= gridCols || nrow < 0 || nrow >= gridRows) continue;
                    auto& cell1 = grid[col][row];
                    auto& cell2 = grid[ncol][nrow];
                    for (size_t i = 0; i < cell1.size(); ++i) {
                        for (size_t j = 0; j < cell2.size(); ++j) {
                            // 同じセル内はi<jのみ、異なるセルなら全組み合わせ
                            if (cell1[i] == cell2[j]) continue;
                            if (&cell1 == &cell2 && j <= i) continue;
                            ResolveEnemyCollision(*cell1[i], *cell2[j]);
                            sf::Vector2f c1 = cell1[i]->shape.getPosition() + sf::Vector2f(cell1[i]->sizeX/2, cell1[i]->sizeY/2);
                            sf::Vector2f c2 = cell2[j]->shape.getPosition() + sf::Vector2f(cell2[j]->sizeX/2, cell2[j]->sizeY/2);
                            collisionLines.emplace_back(c1, c2);
                        }
                    }
                }
            }
        }
    }
}

void ResolveEnemyCollision(Enemy& e1, Enemy& e2) {
    sf::FloatRect rect1 = e1.shape.getGlobalBounds();
    sf::FloatRect rect2 = e2.shape.getGlobalBounds();
    if (rect1.intersects(rect2)) {
        sf::Vector2f center1 = e1.shape.getPosition() + sf::Vector2f(e1.sizeX/2, e1.sizeY/2);
        sf::Vector2f center2 = e2.shape.getPosition() + sf::Vector2f(e2.sizeX/2, e2.sizeY/2);
        sf::Vector2f dir = center1 - center2;
        float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
        if (len != 0) dir /= len; // 正規化

        float overlapX = std::min(rect1.left + rect1.width, rect2.left + rect2.width) - std::max(rect1.left, rect2.left);
        float overlapY = std::min(rect1.top + rect1.height, rect2.top + rect2.height) - std::max(rect1.top, rect2.top);
        float overlap = std::min(overlapX, overlapY);

        sf::Vector2f correction = dir * (overlap / 2.0f);
        e1.shape.move(correction);
        e2.shape.move(-correction);
    }
}
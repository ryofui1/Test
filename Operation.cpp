#include <SFML/Graphics.hpp>
#include <iostream>
#include "Operation.hpp"
#include "main.hpp"
#include <cmath>
#include <vector>
#include <algorithm>
#include "Collider.hpp"
#include "Enemy.hpp"
#include "player.hpp"
#include "Debug.hpp"

// 空間分割用パラメータ
const int gridCols = 8;
const int gridRows = 6;
const float gridWidth = ScreenWidth;
const float gridHeight = ScreenHeight;
const float cellWidth = gridWidth / gridCols;
const float cellHeight = gridHeight / gridRows;

std::vector<std::pair<sf::Vector2f, sf::Vector2f>> collisionLines;

// 汎用コライダー同士の当たり判定・位置補正
void ResolveCollision(Collider& a, Collider& b) {
    sf::FloatRect rect1 = a.getBounds();
    sf::FloatRect rect2 = b.getBounds();
    if (rect1.intersects(rect2)) {
        sf::Vector2f center1(rect1.left + rect1.width / 2, rect1.top + rect1.height / 2);
        sf::Vector2f center2(rect2.left + rect2.width / 2, rect2.top + rect2.height / 2);
        sf::Vector2f dir = center1 - center2;
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len != 0) dir /= len;

        float overlapX = std::min(rect1.left + rect1.width, rect2.left + rect2.width) - std::max(rect1.left, rect2.left);
        float overlapY = std::min(rect1.top + rect1.height, rect2.top + rect2.height) - std::max(rect1.top, rect2.top);
        float overlap = std::min(overlapX, overlapY);

        // ノックバック耐性の比率で補正量を分配
        float r1 = a.knockbackResistance;
        float r2 = b.knockbackResistance;
        float total = r1 + r2;
        if (total <= 0.0001f) total = 1.0f; // 0除算防止

        sf::Vector2f correctionA = dir * (overlap * (r2 / total));
        sf::Vector2f correctionB = -dir * (overlap * (r1 / total));
        a.move(correctionA);
        b.move(correctionB);
    }
}

void Operation(sf::Time deltaTime) {
    debugPrint("Player.move");
    player.move(deltaTime.asSeconds());
    debugPrint("Enemy.move");
    for (auto& enemy : enemyList) {
        enemy.move(deltaTime.asSeconds());
    }
    debugPrint("Object.move");
    for (auto& object : ObjectList_PlayerAttack){
        object.move(deltaTime.asSeconds());
    }
    for (auto& object : ObjectList_EnemyAttack){
        object.move(deltaTime.asSeconds());
    }
    for (auto& object : ObjectList_Other){
        object.move(deltaTime.asSeconds());
    }

    // 空間分割グリッドの作成
    std::vector<std::vector<std::vector<Enemy*>>> grid(gridCols, std::vector<std::vector<Enemy*>>(gridRows));
    for (auto& enemy : enemyList) {
        sf::Vector2f pos = enemy.shape.getPosition();
        int col = std::clamp(static_cast<int>(pos.x / cellWidth), 0, gridCols - 1);
        int row = std::clamp(static_cast<int>(pos.y / cellHeight), 0, gridRows - 1);
        grid[col][row].push_back(&enemy);
    }
    // プレイヤーのグリッド座標を計算
    sf::Vector2f playerPos = player.shape.getPosition();
    int playerCol = std::clamp(static_cast<int>(playerPos.x / cellWidth), 0, gridCols - 1);
    int playerRow = std::clamp(static_cast<int>(playerPos.y / cellHeight), 0, gridRows - 1);
    
    // PlayerAttack, EnemyAttack, Other それぞれをグリッドに分割
    std::vector<std::vector<std::vector<Object*>>> playerAttackGrid(gridCols, std::vector<std::vector<Object*>>(gridRows));
    for (auto& object : ObjectList_PlayerAttack) {
        sf::Vector2f pos = object.shape.getPosition();
        int col = std::clamp(static_cast<int>(pos.x / cellWidth), 0, gridCols - 1);
        int row = std::clamp(static_cast<int>(pos.y / cellHeight), 0, gridRows - 1);
        playerAttackGrid[col][row].push_back(&object);
    }

    std::vector<std::vector<std::vector<Object*>>> enemyAttackGrid(gridCols, std::vector<std::vector<Object*>>(gridRows));
    for (auto& object : ObjectList_EnemyAttack) {
        sf::Vector2f pos = object.shape.getPosition();
        int col = std::clamp(static_cast<int>(pos.x / cellWidth), 0, gridCols - 1);
        int row = std::clamp(static_cast<int>(pos.y / cellHeight), 0, gridRows - 1);
        enemyAttackGrid[col][row].push_back(&object);
    }

    std::vector<std::vector<std::vector<Object*>>> otherObjectGrid(gridCols, std::vector<std::vector<Object*>>(gridRows));
    for (auto& object : ObjectList_Other) {
        sf::Vector2f pos = object.shape.getPosition();
        int col = std::clamp(static_cast<int>(pos.x / cellWidth), 0, gridCols - 1);
        int row = std::clamp(static_cast<int>(pos.y / cellHeight), 0, gridRows - 1);
        otherObjectGrid[col][row].push_back(&object);
    }

    collisionLines.clear();

    // エネミー同士の当たり判定
    for (int col = 0; col < gridCols; ++col) {
        for (int row = 0; row < gridRows; ++row) {
            for (int dcol = -1; dcol <= 1; ++dcol) {
                for (int drow = -1; drow <= 1; ++drow) {
                    int ncol = col + dcol;
                    int nrow = row + drow;
                    if (ncol < 0 || ncol >= gridCols || nrow < 0 || nrow >= gridRows) continue;
                    auto& cell1 = grid[col][row];
                    auto& cell2 = grid[ncol][nrow];
                    for (size_t i = 0; i < cell1.size(); ++i) {
                        for (size_t j = 0; j < cell2.size(); ++j) {
                            if (cell1[i] == cell2[j]) continue;
                            if (&cell1 == &cell2 && j <= i) continue;
                            sf::FloatRect rect1 = cell1[i]->getBounds();
                            sf::FloatRect rect2 = cell2[j]->getBounds();
                            if (rect1.intersects(rect2)) {
                                sf::Vector2f c1(rect1.left + rect1.width / 2, rect1.top + rect1.height / 2);
                                sf::Vector2f c2(rect2.left + rect2.width / 2, rect2.top + rect2.height / 2);
                                collisionLines.emplace_back(c1, c2);
                                ResolveCollision(*cell1[i], *cell2[j]);
                            }
                        }
                    }
                }
            }
        }
    }

    // プレイヤーと各エネミーの当たり判定
    // 近傍セルのみ探索
    for (int dcol = -1; dcol <= 1; ++dcol) {
        for (int drow = -1; drow <= 1; ++drow) {
            int ncol = playerCol + dcol;
            int nrow = playerRow + drow;
            if (ncol < 0 || ncol >= gridCols || nrow < 0 || nrow >= gridRows) continue;

            // エネミーとの当たり判定
            for (Enemy* enemyPtr : grid[ncol][nrow]) {
                sf::FloatRect rect1 = player.getBounds();
                sf::FloatRect rect2 = enemyPtr->getBounds();
                if (rect1.intersects(rect2)) {
                    sf::Vector2f c1(rect1.left + rect1.width / 2, rect1.top + rect1.height / 2);
                    sf::Vector2f c2(rect2.left + rect2.width / 2, rect2.top + rect2.height / 2);
                    collisionLines.emplace_back(c1, c2);
                    ResolveCollision(player, *enemyPtr);
                }
            }
        }
    }

    // プレイヤー攻撃（PlayerAttack）とエネミーの当たり判定（空間分割グリッド利用、近傍セルのみ）
    for (auto& object : ObjectList_PlayerAttack) {
        sf::Vector2f pos = object.shape.getPosition();
        int col = std::clamp(static_cast<int>(pos.x / cellWidth), 0, gridCols - 1);
        int row = std::clamp(static_cast<int>(pos.y / cellHeight), 0, gridRows - 1);

        for (int dcol = -1; dcol <= 1; ++dcol) {
            for (int drow = -1; drow <= 1; ++drow) {
                int ncol = col + dcol;
                int nrow = row + drow;
                if (ncol < 0 || ncol >= gridCols || nrow < 0 || nrow >= gridRows) continue;
                for (Enemy* enemyPtr : grid[ncol][nrow]) {
                    sf::FloatRect rect1 = object.getBounds();
                    sf::FloatRect rect2 = enemyPtr->getBounds();
                    if (rect1.intersects(rect2)) {
                        sf::Vector2f c1(rect1.left + rect1.width / 2, rect1.top + rect1.height / 2);
                        sf::Vector2f c2(rect2.left + rect2.width / 2, rect2.top + rect2.height / 2);
                        collisionLines.emplace_back(c1, c2);
                        object.contact(*enemyPtr);
                        enemyPtr->contactPlayerAttack(object);
                    }
                }
            }
        }
    }
    // エネミー攻撃（EnemyAttack）とプレイヤーの当たり判定（空間分割グリッド利用、近傍セルのみ）
    for (auto& object : ObjectList_EnemyAttack) {
        sf::Vector2f pos = object.shape.getPosition();
        int col = std::clamp(static_cast<int>(pos.x / cellWidth), 0, gridCols - 1);
        int row = std::clamp(static_cast<int>(pos.y / cellHeight), 0, gridRows - 1);

        // プレイヤーの近傍セルのみ探索
        for (int dcol = -1; dcol <= 1; ++dcol) {
            for (int drow = -1; drow <= 1; ++drow) {
                int ncol = col + dcol;
                int nrow = row + drow;
                if (ncol < 0 || ncol >= gridCols || nrow < 0 || nrow >= gridRows) continue;

                // プレイヤーがこのセルにいる場合のみ判定
                if (ncol == playerCol && nrow == playerRow) {
                    sf::FloatRect rect1 = object.getBounds();
                    sf::FloatRect rect2 = player.getBounds();
                    if (rect1.intersects(rect2)) {
                        sf::Vector2f c1(rect1.left + rect1.width / 2, rect1.top + rect1.height / 2);
                        sf::Vector2f c2(rect2.left + rect2.width / 2, rect2.top + rect2.height / 2);
                        collisionLines.emplace_back(c1, c2);
                        object.contact(player);
                        player.contactEnemyAttack(object);
                    }
                }
            }
        }
    }



    for (auto it = enemyList.begin(); it != enemyList.end(); ) {
        if (it->isDisappear()) {
            it = enemyList.erase(it); // eraseの戻り値で次の要素を受け取る
        } else {
            ++it;
        }
    }

    // PlayerAttack
    for (auto it = ObjectList_PlayerAttack.begin(); it != ObjectList_PlayerAttack.end(); ) {
        if (it->isDestroy()) {
            it = ObjectList_PlayerAttack.erase(it);
        } else {
            ++it;
        }
    }

    // EnemyAttack
    for (auto it = ObjectList_EnemyAttack.begin(); it != ObjectList_EnemyAttack.end(); ) {
        if (it->isDestroy()) {
            it = ObjectList_EnemyAttack.erase(it);
        } else {
            ++it;
        }
    }

    // Other
    for (auto it = ObjectList_Other.begin(); it != ObjectList_Other.end(); ) {
        if (it->isDestroy()) {
            it = ObjectList_Other.erase(it);
        } else {
            ++it;
        }
    }


    // デバッグメッセージの表示
    printDebugMessagesEverySecond(deltaTime.asSeconds());
}
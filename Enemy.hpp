#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Collider.hpp"
#include "Object.hpp"
#include <vector>
#include <functional>    // std::function
#include <memory>        // std::unique_ptr

enum class EnemyState {
    Idle,
    Attack0,
    Attack1,
    Escape,
    Walk,
    Stan
};
enum class EnemyAI {
    Tracking, // 近づく
    Leaving, // 距離をとる
    Immovable, // 不動
};
enum class EnemyTypes{
    TrackandArcher,
    MidleTree
};

class IAttackStrategy {
public:
    virtual ~IAttackStrategy() = default;

    virtual void attack(
        const sf::Vector2f& position,
        const sf::Vector2f& direction
    ) = 0;
};

struct EnemyConfig {
    EnemyAI aiType;
    std::function<std::shared_ptr<IAttackStrategy>()> attackStrategyFactory;
    float speed;
    float attackCooldown;
    float maxChopGauge;
    bool hasChopGauge;
    float sizeX;
    float sizeY;
    int maxhp;
    float knockbackResistance;
};


extern std::unordered_map<EnemyTypes, EnemyConfig> enemyTypeTable;

class Enemy : public Collider {
public:
    sf::RectangleShape shape;
    // 敵のタイプを数で指定
    EnemyTypes EnemyType;

    // 敵のAIのタイプを指定
    EnemyAI aiType;

    float sizeX ;
    float sizeY ;
    float speed ; 

    float maxHp;
    float hp;
    bool isDisappear();
    float activeTime; //行動管理用の時間
    EnemyState State; //行動の内容
    


    float velocityX; // X方向の速度
    float velocityY; // Y方向の速度
    float velocityDamping = 400.f; // 速度減衰の値

    Enemy(EnemyTypes enemyType);
    void move(float deltaTime);
    void contactPlayerAttack(Object enemyAttack);
    void applyChopDamage(float amount, bool isFromRight);
    void draw(float deltaTime);
    void velocitySet(float vx, float vy); // 速度を設定する
    void velocityAdd(float vx, float vy); // 速度を加える
    void velocityAccessible(float vx, float vy); // 速度を近づける    
    sf::FloatRect getBounds() const override { return shape.getGlobalBounds(); }
    void move(const sf::Vector2f& offset) override { shape.move(offset); }
    EnemyConfig config;

    virtual ~Enemy();

    bool hasChopGauge;
    float maxChopGauge;

    // 右攻撃から減った量（時計回りに減るゲージ）
    float chopGaugeRight;

    // 左攻撃から減った量（反時計回りに減るゲージ）
    float chopGaugeLeft;

    float stunDuration;
    float stunTimeRemaining ;

private:
    sf::Vector2f velocity; // 速度を保持するための変数
    bool isSpacePressed;
    std::shared_ptr<IAttackStrategy> attackStrategy;
};

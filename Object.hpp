#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.hpp"

class Object : public Collider {
public:
    sf::RectangleShape shape;
    
    int  ObjectType; // Objectのタイプ(0:PlayerAttack, 1:EnemyAttack, 2:InteractItem)
    int MoveType; // 移動タイプ(0:等加速度直線運動 1:追跡)

    float sizeX ;
    float sizeY ;
    float speed ;

    float angle; // 角度(度数法使用 | 0°~360°)

    float velocityX; // X方向の速度
    float velocityY; // Y方向の速度
    float velocityDamping = 0.f; // 速度減衰の値

    Object(int objectType, int movetype,float positionX=0 ,float postionY=0, float Angle = 90.f,float Damge = -1 ,float KnockbackPower = 0);
    void move(float deltaTime);
    void draw(float deltaTime);
    void velocitySet(float vx, float vy); // 速度を設定する
    void velocityAdd(float vx, float vy); // 速度を加える
    void velocityAccessible(float vx, float vy); // 速度を近づける
    sf::FloatRect getBounds() const override { return shape.getGlobalBounds(); }
    void move(const sf::Vector2f& offset) override { shape.move(offset); }

    int hp; // Object自体のHP
    int damage; // ダメージ量
    float lifeTime; // Objectの生存可能時間
    float knockbackPower; // ノックバック力
    void contact(Collider& other);
    float oneKnockbackPower(); 
    int oneDamage();
    bool isDestroy();
private:
    sf::Vector2f velocity; // 速度を保持するための変数
};
#include "Enemy.hpp"
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "main.hpp"
#include "Debug.hpp"
#include "Object.hpp"
#include <iostream>
#include <cmath>
#include <random>

class ArrowAttack : public IAttackStrategy {
public:
    void attack(
        const sf::Vector2f& position,
        const sf::Vector2f& direction
    ) override {
        float angle = std::atan2(direction.y, direction.x);
        angle = angle * 180.0f / 3.14159265f;
        if (angle < 0) angle += 360.0f;
        
        ObjectList_EnemyAttack.push_back(Object(
            1, 0, position.x, position.y, angle, 10.f, 1000.f
        ));
    }
};
class NoAttack : public IAttackStrategy {
public:
    void attack(
        const sf::Vector2f& position,
        const sf::Vector2f& direction
    ) override {}
};


std::unordered_map<EnemyTypes, EnemyConfig> enemyTypeTable = {
    {
        EnemyTypes::TrackandArcher,
        EnemyConfig{
            EnemyAI::Tracking,
            []() -> std::shared_ptr<IAttackStrategy> {
                return std::make_shared<ArrowAttack>();
            },
            200.f,/*speed*/   1.5f,/*Stan時間*/
            100.f,/*maxChopGage*/   true,/*hasChopGauge*/
            50.f,/*sizeX*/   50.f,/*sizeY*/
            10,/*maxhp*/   0.1f/*knockbackResistance*/
        }
    },
    {
        EnemyTypes::MidleTree,
        EnemyConfig{
            EnemyAI::Immovable,
            []() -> std::shared_ptr<IAttackStrategy> {
                return std::make_shared<NoAttack>();},
            0.f,/*speed*/   10.f,/*Stan時間*/
            50.f,/*maxChopGage*/   true,/*hasChopGauge*/
            200.f,/*sizeX*/   400.f,/*sizeY*/
            300,/*maxhp*/   1000.f/*knockbackResistance*/
        }
    },
};


Enemy::~Enemy() = default;


Enemy::Enemy(EnemyTypes enemyType) {
    ColliderType = 1;
    shape.setFillColor(sf::Color::Red);
    hp = 1000;


    EnemyType = enemyType;
    config = enemyTypeTable[EnemyType];
    
    maxHp = config.maxhp;
    hp = maxHp;
    // print("hp:" + std::to_string(hp));
    hasChopGauge = config.hasChopGauge;
    maxChopGauge = config.maxChopGauge;
    chopGaugeRight = 0.f;
    chopGaugeLeft = 0.f;
    stunDuration = 3.f;
    stunTimeRemaining = 0.f;


    shape.setPosition(0.f, 0.f);
    knockbackResistance = 0.1f; // ノックバック耐性の初期値
    speed = 200.f;

    activeTime = 100;
    State = EnemyState::Idle;
    aiType = config.aiType;

    if (config.attackStrategyFactory) {
    attackStrategy = config.attackStrategyFactory();
    } else {
        std::cerr << "EnemyTypeに対応するattackStrategyFactoryが未定義です" << std::endl;
    }
    speed = config.speed;

    sizeX = config.sizeX;
    sizeY = config.sizeY;
    knockbackResistance = config.knockbackResistance;

    shape.setSize(sf::Vector2f(sizeX, sizeY));
    velocityX = 0.f; // X方向の初期速度
    velocityY = 0.f; // Y方向の初期速度
    
    isSpacePressed = false;
}

void Enemy::velocityAccessible(float vx, float vy) {
    // X方向
    velocityX += (vx - velocityX) * 0.1;
    // Y方向
    velocityY += (vy - velocityY) * 0.1;
}
void Enemy::velocitySet(float vx, float vy) {
    // X方向
    velocityX = vx;
    // Y方向
    velocityY = vy;
}
void Enemy::velocityAdd(float vx, float vy) {
    // X方向
    velocityX += vx;
    // Y方向
    velocityY += vy;
}

void Enemy::move(float deltaTime) {
    activeTime += deltaTime;
    debugPrint("Attack activeTime:" + std::to_string(activeTime));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isSpacePressed) {
        velocityAdd(-velocityX*10, -velocityY*10); // スペースキーで反転
        // hp -= 10;
        isSpacePressed = true;
        debugPrint("pushSpace");
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        isSpacePressed = false;
    }

    if (velocityY < -velocityDamping * deltaTime) {
        velocityY += velocityDamping * deltaTime ;
    } else if (velocityY > velocityDamping * deltaTime) {
        velocityY -= velocityDamping * deltaTime ;
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
    if (aiType == EnemyAI::Tracking) {
        // 追跡AI

        //Playerの方向directionを取得
        sf::Vector2f playerPosition = player.shape.getPosition(); // プレイヤーの位置を取得
        sf::Vector2f direction = playerPosition - shape.getPosition();
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length;   // 正規化

        switch (State) {
            // ---- Idle 待機状態 ---- // 
            case EnemyState::Idle:{
                if (activeTime > 2) {
                    if (length < 300) {
                        State = EnemyState::Attack0;
                    } else {
                        State = EnemyState::Walk;
                    }
                }
            break;}
            // ---- Attack0 攻撃状態 ---- //
            case EnemyState::Attack0:{
                if (attackStrategy) {
                    attackStrategy->attack(shape.getPosition(), direction);
                } else {
                    std::cerr << "attackStrategy is nullptr!" << std::endl;
                }
                State = EnemyState::Idle;
                activeTime = 0;

            break;}
            // ---- Walk 歩行状態 ---- //
            case EnemyState::Walk:{
                sf::Vector2f newVelocity = direction * speed;
                float newSpeed = std::sqrt(newVelocity.x * newVelocity.x + newVelocity.y * newVelocity.y);
                float currentSpeed = std::sqrt(velocityX * velocityX + velocityY * velocityY);

                // 新しい速度ベクトルの大きさが現状より大きい場合のみ上書き
                if (newSpeed > currentSpeed) {
                    velocitySet(newVelocity.x, newVelocity.y);
                }

                // 近くなったら待機状態へ
                if (length < 200) {
                    State = EnemyState::Idle;
                    activeTime = 0;
                }
            break;}
            // ---- Stan スタン状態 ----- //
            case EnemyState::Stan:{
                if (activeTime > stunDuration) { 
                    State = EnemyState::Idle;
                }
                break;}
            default:{
                debugPrint("Any Enemy has FAILED State");
            }
        }
        shape.move(velocityX*deltaTime,velocityY*deltaTime);
    } else if (aiType == EnemyAI::Leaving) {
        // 傍観AI
    } else if (aiType == EnemyAI::Immovable) {
        // 不動AI
    }
}
void Enemy::draw(float deltaTime) {
    window.draw(shape);

    // --- HPバー描画 ---
    float barWidth = sizeX;
    float barHeight = 8.f;
    float hpRatio = std::max(0.f, std::min(1.f, hp / static_cast<float>(maxHp)));
    sf::Vector2f barPos = shape.getPosition() + sf::Vector2f(0, -barHeight - 4.f);

    sf::RectangleShape backBar(sf::Vector2f(barWidth, barHeight));
    backBar.setPosition(barPos);
    backBar.setFillColor(sf::Color(80, 80, 80)); // グレー

    sf::RectangleShape hpBar(sf::Vector2f(barWidth * hpRatio, barHeight));
    hpBar.setPosition(barPos);
    hpBar.setFillColor(sf::Color::Red);

    window.draw(backBar);
    window.draw(hpBar);

    // 頭上の位置（HPバーのさらに上）
    float verticalOffset = -20.f;
    sf::Vector2f center = shape.getPosition() + sf::Vector2f(sizeX / 2, verticalOffset);

    // ----- HP ゲージの描画 ----- //
    float fillRatio;
    if (hp > 0) {fillRatio = std::clamp(hp / static_cast<float>(maxHp), 0.f, 1.f);}
    else {fillRatio = 0.f;}
    
    
    static sf::Texture heartTex;
    static sf::Texture heartBehind;
    static bool texturesLoaded = [](){
        return heartTex.loadFromFile("test/photo/test/Heart.png") &&
            heartBehind.loadFromFile("test/photo/test/HeartBehind.png");
    }();

    if (texturesLoaded) {
        sf::Vector2u texSize = heartTex.getSize();

        // --- RenderTextureを再利用 ---
        static sf::RenderTexture rt;
        static bool rtInitialized = false;
        if (!rtInitialized) {
            rt.create(texSize.x, texSize.y);
            rtInitialized = true;
        }

        rt.clear(sf::Color::Transparent);

        // HP量に応じた赤い塗りつぶし
        sf::RectangleShape fillRect(sf::Vector2f(texSize.x, texSize.y * fillRatio));
        fillRect.setFillColor(sf::Color::Red);
        fillRect.setPosition(0.f, texSize.y * (1.f - fillRatio));
        rt.draw(fillRect);

        // 前景ハートでマスク（アルファ適用）
        sf::Sprite heart(heartTex);
        rt.draw(heart, sf::BlendMultiply);

        rt.display();

        // 前景ハート（塗り込み済み）
        sf::Sprite filledHeart(rt.getTexture());
        filledHeart.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
        filledHeart.setPosition(center);
        filledHeart.setScale(0.5f, 0.5f);

        // 背景ハート（常に表示されるベース）
        sf::Sprite heartBg(heartBehind);
        heartBg.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
        heartBg.setPosition(center);
        heartBg.setScale(0.5f, 0.5f);

        // --- 描画順（背景 → 前景） ---
        window.draw(heartBg);       // 背景を先に描画
        window.draw(filledHeart);   // 前景（塗りつぶしハート）を上に描画
    }
    // ----- HP ゲージの描画終了 ----- //

    // --- ChopGauge（環状）描画 ---
    if (hasChopGauge && maxChopGauge > 0.0f) {
        const int segments = 100;
        float radiusOuter = 18.f;
        float radiusInner = 12.f;

        auto drawRingSegment = [&](float startAngleDeg, float sweepAngleDeg, sf::Color color) {
            sf::VertexArray ring(sf::TriangleStrip);
            for (int i = 0; i <= segments; ++i) {
                float t = static_cast<float>(i) / segments;
                float angleDeg = startAngleDeg + sweepAngleDeg * t;
                float angleRad = angleDeg * 3.14159265f / 180.f;

                sf::Vector2f outer = center + sf::Vector2f(std::cos(angleRad), std::sin(angleRad)) * radiusOuter;
                sf::Vector2f inner = center + sf::Vector2f(std::cos(angleRad), std::sin(angleRad)) * radiusInner;

                ring.append(sf::Vertex(outer, color));
                ring.append(sf::Vertex(inner, color));
            }
            window.draw(ring);
        };

        const float baseAngle = 270.f;

        if (State == EnemyState::Stan) {
            // スタン中：全黒で塗り潰し
            drawRingSegment(baseAngle, 360.f, sf::Color::Black);
        } else {
            // ベース（赤）
            drawRingSegment(baseAngle, 360.f, sf::Color(180, 0, 0));
            // 右攻撃：時計回りに暗緑
            float ratioRight = std::min(1.f, chopGaugeRight / maxChopGauge);
            drawRingSegment(baseAngle, 360.f * ratioRight, sf::Color(0, 150, 0));
            // 左攻撃：反時計回りに暗青
            float ratioLeft = std::min(1.f, chopGaugeLeft / maxChopGauge);
            drawRingSegment(baseAngle, -360.f * ratioLeft, sf::Color(0, 80, 180));
        }

        // 外周境界線（白）
        sf::VertexArray border(sf::LineStrip);
        for (int i = 0; i <= segments; ++i) {
            float angleDeg = baseAngle + 360.f * (float(i) / segments);
            float angleRad = angleDeg * 3.14159265f / 180.f;
            sf::Vector2f point = center + sf::Vector2f(std::cos(angleRad), std::sin(angleRad)) * radiusOuter;
            border.append(sf::Vertex(point, sf::Color::White));
        }
        window.draw(border);
    }




    // --- 速度ベクトル（矢印）描画 ---
    float speedNorm = std::sqrt(velocityX * velocityX + velocityY * velocityY);
    if (speedNorm > 1e-4f) {
        float arrowLength = 40.f;
        sf::Vector2f start = shape.getPosition() + sf::Vector2f(sizeX / 2, sizeY / 2);
        sf::Vector2f dir = sf::Vector2f(velocityX, velocityY) / speedNorm;
        sf::Vector2f end = start + dir * arrowLength;

        sf::Vertex line[] = {
            sf::Vertex(start, sf::Color::Yellow),
            sf::Vertex(end, sf::Color::Yellow)
        };
        window.draw(line, 2, sf::Lines);

        float headSize = 10.f;
        sf::Vector2f left = end + sf::Vector2f(-dir.y, dir.x) * (headSize / 2.f) - dir * headSize;
        sf::Vector2f right = end + sf::Vector2f(dir.y, -dir.x) * (headSize / 2.f) - dir * headSize;
        sf::ConvexShape arrowHead;
        arrowHead.setPointCount(3);
        arrowHead.setPoint(0, end);
        arrowHead.setPoint(1, left);
        arrowHead.setPoint(2, right);
        arrowHead.setFillColor(sf::Color::Yellow);
        window.draw(arrowHead);
    }
}

bool Enemy::isDisappear() {
    if (hp <= 0) {
        return(true);
    } else {
        return(false);
    }
}

void Enemy::contactPlayerAttack(Object playerAttack) {
    hp -= playerAttack.damage;
    velocityAdd(
        playerAttack.knockbackPower * cos(playerAttack.angle * 3.1415f / 180.f),
        playerAttack.knockbackPower * sin(playerAttack.angle * 3.1415f / 180.f)
    );
    if (hasChopGauge) {
        // 攻撃が右からか左からかを判定（右からなら true）
        bool isFromRight = cos(playerAttack.angle * 3.1415f / 180.f) > 0;

        // 与えるゲージ減少量は、たとえば playerAttack.damage の半分にする
        float chopDamage = playerAttack.damage * 0.5f;

        applyChopDamage(chopDamage, isFromRight);
    }
    debugPrint("playerAttack!! hp:" + std::to_string(hp));
}

void Enemy::applyChopDamage(float amount, bool isFromRight) {
    if (!hasChopGauge) return;

    if (isFromRight) {
        chopGaugeRight += amount;
    } else {
        chopGaugeLeft += amount;
    }

    // 合計が最大値を超えたらスタン
    if ((chopGaugeRight + chopGaugeLeft) >= maxChopGauge && State != EnemyState::Stan) {
        State = EnemyState::Stan;
        stunTimeRemaining = stunDuration;
        // 必要ならここでゲージリセット
        chopGaugeRight = 0.f;
        chopGaugeLeft = 0.f;
        activeTime = 0.f;
    }

    // 各ゲージはmaxを超えないよう制限
    if (chopGaugeRight > maxChopGauge) chopGaugeRight = maxChopGauge;
    if (chopGaugeLeft > maxChopGauge) chopGaugeLeft = maxChopGauge;
}

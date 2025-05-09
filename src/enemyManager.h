#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "enemy.h"
#include "laser.h"
#include "player.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <vector>

class EnemyManager {
public:
    EnemyManager();
    void configure(std::shared_ptr<sf::Texture> tex,
                   int maxEn, int health, float cooldown,
                   float speed, float scale = 1.f);
    void update(float dt, const sf::Vector2f& playerPos,
                bool allowSpawn = true);
    void draw(sf::RenderWindow& window);
    int handleLaserCollisions(std::vector<Laser>& lasers);
    void handlePlayerCollisions(Player& player, int damage);

    // Get current alive enemy count
    int getAliveCount() const;

private:
    std::shared_ptr<sf::Texture> enemyTexture;
    float enemyScale;
    float spawnCooldown;
    int maxEnemies;
    int baseHealth;
    float baseSpeed;
    float spawnTimer;
    std::mt19937 rng;
    std::uniform_int_distribution<> hpDist;
    std::uniform_real_distribution<float> speedDist;
    std::vector<Enemy> enemies;
    void spawnEnemy(const sf::Vector2f& playerPos);
};

#endif

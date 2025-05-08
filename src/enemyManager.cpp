#include "enemyManager.h"
#include "hitbox.h"
#include <algorithm>
#include <cstdlib>

EnemyManager::EnemyManager()
    : rng(std::random_device{}()),
      hpDist(baseHealth, baseHealth),
      speedDist(baseSpeed, baseSpeed),
      enemyScale(1.f),
      spawnCooldown(2.f),
      maxEnemies(10),
      baseHealth(3),
      baseSpeed(60.f),
      spawnTimer(0.f)
{}

void EnemyManager::configure(std::shared_ptr<sf::Texture> tex, int maxEn, int health, float cooldown, float speed, float scale) {
    enemyTexture = tex;
    maxEnemies = maxEn;
    baseHealth = health;
    spawnCooldown = cooldown;
    baseSpeed = speed;
    enemyScale = scale;
    hpDist = std::uniform_int_distribution<>(health, health);
    speedDist = std::uniform_real_distribution<float>(speed, speed);
}

void EnemyManager::update(float dt, const sf::Vector2f& playerPos, bool allowSpawn) {
    spawnTimer += dt;
    if (allowSpawn && spawnTimer >= spawnCooldown && enemies.size() < static_cast<size_t>(maxEnemies)) {
        spawnEnemy(playerPos);
        spawnTimer = 0.f;
    }
    for (auto& e : enemies) e.update(dt, playerPos);
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& e){ return !e.isAlive(); }),
        enemies.end()
    );
}

void EnemyManager::draw(sf::RenderWindow& window) {
    for (auto& e : enemies) e.draw(window);
}

int EnemyManager::handleLaserCollisions(std::vector<Laser>& lasers) {
    int kills = 0;
    for (auto it = lasers.begin(); it != lasers.end();) {
        bool hit = false;
        for (auto& e : enemies) {
            if (it->getGlobalBounds().intersects(e.getGlobalBounds())) {
                if (e.takeDamage() <= 0) ++kills;
                hit = true;
                break;
            }
        }
        if (hit) it = lasers.erase(it);
        else ++it;
    }
    return kills;
}

void EnemyManager::handlePlayerCollisions(Player& player, int damage) {
    for (auto& e : enemies) {
        if (e.getGlobalBounds().intersects(player.getHitbox().getBounds()))
            player.takeDamage(damage);
    }
}

void EnemyManager::spawnEnemy(const sf::Vector2f& playerPos) {
    float x = static_cast<float>(std::rand() % 800);
    int hp = hpDist(rng);
    float spd = speedDist(rng);
    enemies.emplace_back(x, -50.f, spd, hp, enemyTexture, enemyScale);
}

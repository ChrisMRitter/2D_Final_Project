#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "enemy.h"
#include "laser.h"
#include "player.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <vector>

// EnemyManager class handles spawning, updating, drawing, and managing all enemies in the game
class EnemyManager {
public:
    EnemyManager(); // Constructor

    // Virtual destructor to allow proper cleanup in derived classes (e.g., for boss fights)
    virtual ~EnemyManager() = default; //Virtual destructor

    // Configure the enemy manager with texture and behavior parameters
    void configure(std::shared_ptr<sf::Texture> tex,
                   int maxEn, int health, float cooldown,
                   float speed, float scale = 1.f);

    // Update all enemies and handle spawning logic
    virtual void update(float dt, const sf::Vector2f& playerPos,
                bool allowSpawn = true);

    // Draw all enemies to the render window
    virtual void draw(sf::RenderWindow& window);

    // Handle collisions between lasers and enemies, return number of kills
    virtual int handleLaserCollisions(std::vector<Laser>& lasers);

    // Handle collisions between enemies and the player
    virtual void handlePlayerCollisions(Player& player, int damage);

    // Get the number of currently alive enemies
    int getAliveCount() const;

protected:
    std::shared_ptr<sf::Texture> enemyTexture; // Shared texture used by all enemies
    float enemyScale; // Scale factor for enemy sprites
    float spawnCooldown; // Time interval between enemy spawns
    int maxEnemies; // Maximum number of enemies allowed at once
    int baseHealth; // Base health value for enemies
    float baseSpeed; // Base speed value for enemies
    float spawnTimer; // Timer to track time since last spawn
    std::mt19937 rng; // Random number generator
    std::uniform_int_distribution<> hpDist; // Distribution for random enemy health
    std::uniform_real_distribution<float> speedDist; // Distribution for random enemy speed
    std::vector<Enemy> enemies; // Container for all active enemies

    // Spawns a new enemy (can be overridden for custom spawn logic)
    virtual void spawnEnemy(const sf::Vector2f& playerPos);
};

#endif

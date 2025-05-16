#include "enemyManager.h"
#include "hitbox.h"
#include "gameconstants.h"
#include <algorithm>
#include <cstdlib>

// Constructor initializes default values and random number generators
EnemyManager::EnemyManager()
    : rng(std::random_device{}()), // Random number generator seeded with hardware entropy
      baseHealth(3), // Default enemy health
      baseSpeed(60.f), // Default enemy speed
      spawnCooldown(2.f), // Time between spawns
      enemyScale(1.f), // Default enemy sprite scale
      maxEnemies(10), // Maximum number of enemies allowed at once
      spawnTimer(0.f) // Time to track spawn intervals
{
    // Distributions for randomized health and speed
    //Note: The idae for a distribution-based health system was from Claude
    hpDist    = std::uniform_int_distribution<>(baseHealth, baseHealth);
    speedDist = std::uniform_real_distribution<float>(baseSpeed, baseSpeed);
}

// Configures the enemy manager with new parameters
void EnemyManager::configure(std::shared_ptr<sf::Texture> tex,
                             int maxEn, int health, float cooldown,
                             float speed, float scale) {
    enemyTexture = std::move(tex);
    maxEnemies   = maxEn;
    baseHealth   = health;
    spawnCooldown= cooldown;
    baseSpeed    = speed;
    enemyScale   = scale;
    // Update distributions with new values
    hpDist       = std::uniform_int_distribution<>(health, health);
    speedDist    = std::uniform_real_distribution<float>(speed, speed);
}

// Updates all enemies and handles spawning logic
void EnemyManager::update(float dt,
                          const sf::Vector2f& playerPos,
                          bool allowSpawn) {
    spawnTimer += dt;
    // Spawn a new enemy if allowed and cooldown has passed
    if (allowSpawn && spawnTimer >= spawnCooldown &&
        enemies.size() < static_cast<size_t>(maxEnemies)) {
        spawnEnemy(playerPos);
        spawnTimer = 0.f;
    }

    // Update each enemy's behavior
    for (auto& e : enemies) e.update(dt, playerPos);

    // Remove dead enemies from the list
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
                       [](const Enemy& e){ return !e.isAlive(); }),
        enemies.end()
    );
}

// Draws all enemies to the screen
void EnemyManager::draw(sf::RenderWindow& window) {
    for (auto& e : enemies) e.draw(window);
}

// Handles collisions between lasers and enemies
int EnemyManager::handleLaserCollisions(std::vector<Laser>& lasers) {
    int kills = 0;
    // Iterate through lasers
    for (auto it = lasers.begin(); it != lasers.end();) {
        bool hit = false;
        
        // Check collisions with each enemy
        for (auto& e : enemies) {
            if (it->getGlobalBounds().intersects(e.getGlobalBounds())) {
                if (e.takeDamage() <= 0) ++kills;
                // Count kill if enemy dies
                hit = true;
                break;
            }
        }

        // Remove laser if it hit an enemy
        if (hit) it = lasers.erase(it);
        else     ++it;
    }
    return kills;
}

// Handles collisions between enemies and the player
void EnemyManager::handlePlayerCollisions(Player& player, int damage) {
    for (auto& e : enemies) {
        if (e.getGlobalBounds().intersects(
                player.getHitbox().getBounds())) {
            player.takeDamage(damage); // Apply damage to player
        }
    }
}

// Spawns a new enemy at a random horizontal position above the screen
// to give the player time to react
void EnemyManager::spawnEnemy(const sf::Vector2f& /*playerPos*/) {
    int maxX = static_cast<int>(VIEW_WIDTH);
    float x   = static_cast<float>(std::rand() % maxX); // Random X position: AI-assisted
    float y   = -50.f; // Spawn above the screen
    int   hp  = hpDist(rng); // Randomized health
    float spd = speedDist(rng); // Randomized speed

    // Choose enemy type based on base health
    EnemyType type = (baseHealth == 2) ? EnemyType::GREEN_ENEMY : EnemyType::RED_ENEMY;

    // Add new enemy to the list
    enemies.emplace_back(x, y, spd, hp, enemyTexture, enemyScale, type);
}

// Returns the number of currently alive enemies
int EnemyManager::getAliveCount() const {
    return static_cast<int>(enemies.size());
}

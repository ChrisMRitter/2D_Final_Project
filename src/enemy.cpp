#include "enemy.h"
#include <cmath> // For std::sin and std::sqrt

// Constructor for the Enemy class
Enemy::Enemy(float x, float y, float speed, int health,
    std::shared_ptr<sf::Texture> texture, float scale,
    EnemyType type, sf::IntRect texRect)
    : texture(texture) // Shared pointer to the texture used by the enemy
    , speed(speed) // Movement speed of the enemy
    , health(health) // Current health
    , maxHealth(health) // Maximum health (initially same as current)
    , scale(scale) // Scale factor for the sprite
    , type(type) // Set the texture for the sprite
{

    // If no texture rectangle is provided, use the full texture
    sprite.setTexture(*texture);
    if (texRect.width == 0 || texRect.height == 0) {
        auto ts = texture->getSize();
        texRect = sf::IntRect(0, 0, int(ts.x), int(ts.y));
    }

    // Set the portion of the texture to use
    sprite.setTextureRect(texRect);

    // Set the origin to the center of the sprite for proper rotation and movement
    sprite.setOrigin(texRect.width / 2.f, texRect.height / 2.f);

    // Set the initial position of the enemy
    sprite.setPosition(x, y);

    // Apply scaling to the sprite
    sprite.setScale(scale, scale);
}

// Update function to move the enemy based on its type and the player's position
void Enemy::update(float dt, sf::Vector2f playerPos) {
    // Mothership has a unique movement pattern (side-to-side)
    if (type == EnemyType::MOTHERSHIP) {
        static float time = 0.0f;
        time += dt;

        // Oscillate horizontally using sine wave
        float xOffset = std::sin(time) * 50.0f * dt;
        sprite.move(xOffset, 0);

    // Keep mothership within screen bounds (assumes screen width is 800)
    sf::Vector2f pos = sprite.getPosition();
    if (pos.x < 100.f) pos.x = 100.f;
    if (pos.x > 800.f - 100.f) pos.x = 800.f - 100.f;
    sprite.setPosition(pos);
} else {
    // Regular enemies move toward the player
    sf::Vector2f dir = playerPos - sprite.getPosition();
    float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
    // Normalize direction and move toward player
    if (len > 0.f) {
        dir /= len;
        sprite.move(dir * speed * dt);
        }
    }
}

// Draw the enemy sprite to the window
void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

// Get the global bounding box of the enemy (used for collision detection)
sf::FloatRect Enemy::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

// Get the current health of the enemy
int Enemy::getHealth() const { return health; }

// Set the enemy's health to a specific value
void Enemy::setHealth(int h) { health = h; }

// Reduce health by 1 and mark as dead if health reaches 0
int Enemy::takeDamage() { health--; if (health <= 0) alive = false; return health; }

// Instantly kill the enemy
void Enemy::killEnemy() { alive = false; }

// Check if the enemy is still alive
bool Enemy::isAlive() const { return alive; }

// src/BulletPattern.cpp
#include "BulletPattern.h"
#include <cmath>
#include <random>

// Constructor: Initializes default bullet pattern and intensity
BulletPattern::BulletPattern()
    : currentPattern(BulletPatternType::SINGLE_SHOT), intensity(1)
{
}

// Sets the current bullet pattern type
void BulletPattern::setType(BulletPatternType type) {
    currentPattern = type;
}

// Sets the intensity of the pattern (clamped between 1 and 5)
void BulletPattern::setIntensity(int intensity) {
    this->intensity = std::min(5, std::max(1, intensity)); // Clamp between 1-5
}

// Executes the current bullet pattern from a given position toward a target
void BulletPattern::execute(sf::Vector2f position, sf::Vector2f targetPos, std::vector<Laser>& lasers) {
    switch (currentPattern) {
        case BulletPatternType::SINGLE_SHOT:
            fireSingleShot(position, targetPos, lasers);
            break;
        case BulletPatternType::SPREAD:
            fireSpread(position, targetPos, lasers);
            break;
        case BulletPatternType::CIRCULAR:
            fireCircular(position, lasers);
            break;
        case BulletPatternType::TARGETED:
            fireTargeted(position, targetPos, lasers);
            break;
    }
}

// Fires a single laser directly at the target
void BulletPattern::fireSingleShot(sf::Vector2f pos, sf::Vector2f targetPos, std::vector<Laser>& lasers) {
    // Calculate direction to target
    sf::Vector2f direction = targetPos - pos;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction /= length;
    } else {
        direction = sf::Vector2f(0.f, 1.f); // Default down direction
    }
    
    // Create and add the laser
    lasers.emplace_back(pos.x, pos.y, 300.f, direction, true); // true = isEnemy
}

// Fires a spread of lasers in an arc toward the target
void BulletPattern::fireSpread(sf::Vector2f pos, sf::Vector2f targetPos, std::vector<Laser>& lasers) {
    // Calculate base direction to target
    sf::Vector2f baseDir = targetPos - pos;
    float length = std::sqrt(baseDir.x * baseDir.x + baseDir.y * baseDir.y);
    float baseAngle = 0.f;
    
    if (length > 0) {
        baseDir /= length;
        baseAngle = atan2(baseDir.y, baseDir.x); // Angle to target
    } else {
        baseAngle = 3.14159f / 2.f; // Default downward
    }
    
    // Number of bullets based on intensity
    int numBullets = 3 + intensity * 2; // 5 to 13 bullets
    float spreadAngle = 3.14159f / 3.f; // 60 degrees total spread
    float angleStep = spreadAngle / (numBullets - 1);
    float startAngle = baseAngle - spreadAngle / 2.f;
    
    // Fire bullets in a spread pattern
    for (int i = 0; i < numBullets; i++) {
        float angle = startAngle + angleStep * i;
        sf::Vector2f direction(cos(angle), sin(angle));
        lasers.emplace_back(pos.x, pos.y, 300.f, direction, true);
    }
}

// Fires lasers in a full circular pattern around the origin
void BulletPattern::fireCircular(sf::Vector2f pos, std::vector<Laser>& lasers) {
    // Number of bullets based on intensity
    int numBullets = 8 + intensity * 2; // 10 to 18 bullets
    float angleStep = 2.f * 3.14159f / numBullets;
    
    // Fire bullets in a circular pattern
    for (int i = 0; i < numBullets; i++) {
        float angle = angleStep * i;
        sf::Vector2f direction(cos(angle), sin(angle));
        lasers.emplace_back(pos.x, pos.y, 300.f, direction, true);
    }
}

// Fires one laser directly at the target and additional ones with slight random offsets
void BulletPattern::fireTargeted(sf::Vector2f pos, sf::Vector2f targetPos, std::vector<Laser>& lasers) {
    // Number of bullets based on intensity
    int numBullets = 2 + intensity; // 3 to 7 bullets
    
    // Calculate base direction to target
    sf::Vector2f baseDir = targetPos - pos;
    float length = std::sqrt(baseDir.x * baseDir.x + baseDir.y * baseDir.y);
    if (length > 0) {
        baseDir /= length;
    } else {
        baseDir = sf::Vector2f(0.f, 1.f); // Default downward
    }
    
    // Fire the main laser directly at the target
    lasers.emplace_back(pos.x, pos.y, 300.f, baseDir, true);
    
    // Fire additional lasers with slight random directional offsets
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-0.3f, 0.3f); // Random offset
    
    for (int i = 1; i < numBullets; i++) {
        sf::Vector2f offset(dist(gen), dist(gen));
        sf::Vector2f direction = baseDir + offset;
        
        // Normalize direction
        float dirLen = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (dirLen > 0) {
            direction /= dirLen; // Normalize
        }
        
        lasers.emplace_back(pos.x, pos.y, 300.f, direction, true);
    }
}

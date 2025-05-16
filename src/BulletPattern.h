// src/BulletPattern.h
#ifndef BULLET_PATTERN_H
#define BULLET_PATTERN_H

#include "laser.h"
#include <SFML/Graphics.hpp>
#include <vector>

// Enum representing the different types of bullet patterns available
enum class BulletPatternType {
    SINGLE_SHOT,  // Fires a single bullet directly at the player
    SPREAD,       // Fires multiple bullets in a spread arc
    CIRCULAR,     // Fires bullets in a full 360-degree circle
    TARGETED      // Fires multiple bullets aimed at the player with slight variation
};

// BulletPattern class handles the logic for generating various enemy bullet patterns
class BulletPattern {
public:
    BulletPattern(); // Constructor initializes default pattern and intensity

    // Sets the current bullet pattern type
    void setType(BulletPatternType type);

    // Sets the intensity of the pattern (affects number of bullets)
    void setIntensity(int intensity);

    // Executes the current pattern from a given position toward a target
    void execute(sf::Vector2f position, sf::Vector2f targetPos, std::vector<Laser>& lasers);
    
private:
    BulletPatternType currentPattern; // Currently selected bullet pattern
    int intensity; // Intensity level (1-5), controls bullet count
    
    // Internal methods for each bullet pattern type
    void fireSingleShot(sf::Vector2f pos, sf::Vector2f targetPos, std::vector<Laser>& lasers);
    void fireSpread(sf::Vector2f pos, sf::Vector2f targetPos, std::vector<Laser>& lasers);
    void fireCircular(sf::Vector2f pos, std::vector<Laser>& lasers);
    void fireTargeted(sf::Vector2f pos, sf::Vector2f targetPos, std::vector<Laser>& lasers);
};

#endif // BULLET_PATTERN_H

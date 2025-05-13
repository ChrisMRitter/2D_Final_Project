// src/BulletPattern.h
#ifndef BULLET_PATTERN_H
#define BULLET_PATTERN_H

#include "laser.h"
#include <SFML/Graphics.hpp>
#include <vector>

enum class BulletPatternType {
    SINGLE_SHOT,  // Basic single shot at player
    SPREAD,       // Multiple shots in a spread
    CIRCULAR,     // 360-degree pattern
    TARGETED      // Multiple shots at player position
};

class BulletPattern {
public:
    BulletPattern();
    
    void setType(BulletPatternType type);
    void setIntensity(int intensity);
    
    void execute(sf::Vector2f position, sf::Vector2f targetPos, std::vector<Laser>& lasers);
    
private:
    BulletPatternType currentPattern;
    int intensity; // 1-5, controls number of bullets
    
    // Pattern implementation methods
    void fireSingleShot(sf::Vector2f pos, sf::Vector2f targetPos, std::vector<Laser>& lasers);
    void fireSpread(sf::Vector2f pos, sf::Vector2f targetPos, std::vector<Laser>& lasers);
    void fireCircular(sf::Vector2f pos, std::vector<Laser>& lasers);
    void fireTargeted(sf::Vector2f pos, sf::Vector2f targetPos, std::vector<Laser>& lasers);
};

#endif // BULLET_PATTERN_H
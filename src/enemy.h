#ifndef ENEMY_H
#define ENEMY_H

#include "enemyTypes.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Enemy {
public:
  // Constructor to initialize an enemy with position, speed, health, texture, scale, type, and a texture rectangle
  Enemy(float x, float y, float speed, int health,
        std::shared_ptr<sf::Texture> texture, float scale,
        EnemyType type = EnemyType::GREEN_ENEMY,
        sf::IntRect texRect = sf::IntRect());

  virtual void update(float dt, sf::Vector2f playerPos);

  // Draws the enemy sprite
  void draw(sf::RenderWindow &window);

  // Returns the bounds of the enemy sprite (used for collision detection)
  sf::FloatRect getGlobalBounds() const;

  // Getters and Setters for health
  int getHealth() const;
  void setHealth(int health);

  // Reduces health by 1 and returns new health, enemy is dead at 0
  int takeDamage();

  // Instantly kills the enemy (sets alive to false)
  void killEnemy();

  // Returns whether the enemy is still alive or not
  bool isAlive() const;

  // Getters and Setters for enemy type
  EnemyType getType() const { return type; }
  void setType(EnemyType t) { type = t; }

protected:
  std::shared_ptr<sf::Texture> texture; // Shared texture
  sf::Sprite sprite; // Enemy Sprite
  float speed; // Movement speed
  int health; // Current health
  int maxHealth; // Maximum health
  float scale; // Sprite Scaling
  bool alive = true; // Indicates if the enemy is still alive
  EnemyType type; // Type of enemy to determine behavior (different between the red and green enemies)
};

#endif

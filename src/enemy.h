// src/enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include "enemyTypes.h" // Include custom enum or definitions for enemy types
#include <SFML/Graphics.hpp> // Include SFML for graphics rendering
#include <memory> // For std::shared_ptr

// Enemy class represents a game enemy with movement, health, and rendering logic
class Enemy {
public:
  // Constructor to initialize an enemy with position, speed, health, texture, scale, type, and optional texture rectangle
  Enemy(float x, float y, float speed, int health,
        std::shared_ptr<sf::Texture> texture, float scale,
        EnemyType type = EnemyType::GREEN_ENEMY,
        sf::IntRect texRect = sf::IntRect());

  // Virtual update method to allow polymorphic behavior in derived enemy types
  virtual void update(float dt, sf::Vector2f playerPos);

  // Draws the enemy sprite to the given render window
  void draw(sf::RenderWindow &window);

  // Returns the global bounding box of the enemy sprite (used for collision detection)
  sf::FloatRect getGlobalBounds() const;

  // Accessor and mutator methods for health
  int getHealth() const;
  void setHealth(int health);

  // Reduces health by 1 and returns new health; marks enemy as dead if health reaches 0
  int takeDamage();

  // Instantly kills the enemy (sets alive to false)
  void killEnemy();

  // Returns whether the enemy is still alive
  bool isAlive() const;

  // Accessor and mutator methods for the enemy type
  EnemyType getType() const { return type; }
  void setType(EnemyType t) { type = t; }

protected:
  std::shared_ptr<sf::Texture> texture; // Shared texture used by the enemy sprite
  sf::Sprite sprite; // SFML sprite representing the enemy
  float speed; // Movement speed of the enemy
  int health; // Current health
  int maxHealth; // Maximum health
  float scale; // Scale factor for sprite
  bool alive = true; // Flag indicating if the enemy is still alive
  EnemyType type; // Type of enemy (used to determine behavior)
};

#endif

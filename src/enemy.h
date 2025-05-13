// src/enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include "enemyTypes.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Enemy {
public:
  Enemy(float x, float y, float speed, int health,
        std::shared_ptr<sf::Texture> texture, float scale,
        EnemyType type = EnemyType::GREEN_ENEMY,
        sf::IntRect texRect = sf::IntRect());

  virtual void update(float dt, sf::Vector2f playerPos);
  void draw(sf::RenderWindow &window);
  sf::FloatRect getGlobalBounds() const;
  int getHealth() const;
  void setHealth(int health);
  int takeDamage();
  void killEnemy();
  bool isAlive() const;
  EnemyType getType() const { return type; }
  void setType(EnemyType t) { type = t; }

protected:
  std::shared_ptr<sf::Texture> texture;
  sf::Sprite sprite;
  float speed;
  int health;
  int maxHealth;
  float scale;
  bool alive = true;
  EnemyType type;
};

#endif
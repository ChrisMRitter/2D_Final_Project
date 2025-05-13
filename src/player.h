
#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "laser.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Hitbox;
class Player {
public:
  Player(std::string texturePath, float speed);
  ~Player();

  // Get the player's hitbox
  const Hitbox &getHitbox() const;
  // Teleport the player (for cutscenes, etc.)
  void setPosition(const sf::Vector2f &pos);
  // Function to take damage
  void takeDamage(int amount);
  // Function to check if the player can take damage
  bool canTakeDamage() const;
  // Handle keyboard/mouse input
  void handleInput(float dt, std::vector<Laser> &lasers, sf::Vector2f mousePos);
  // Draw the player
  void draw(sf::RenderWindow &window);
  // Update per-frame (movement, firing cooldowns, etc.)
  void update(float dt, std::vector<Laser> &lasers, sf::Vector2f mousePos);
  // Get the player's position
  sf::Vector2f getPosition();
  // Get the player's global bounds
  sf::FloatRect getGlobalBounds();
  // Get current health
  int getHealth() const;
  // Fire a laser in a given direction
  void createLaser(std::vector<Laser> &lasers, sf::Vector2f playerPos,
                   sf::Vector2f fireDirection);
  void decreaseLaserCooldown(int killCount); //laser cooldown decreases as you kill chasers

  float getLaserCooldown() const; //getter for laser cooldown

  void resetLaserCooldown(); //reset laser cooldown (on level transition)

private:
  Hitbox *hitbox;

  float damageCooldown = 0.5f;
  float timeSinceLastHit = 0.f;
  sf::Texture texture;
  sf::Sprite sprite;
  float speed = 100.f;
  int health = 10;
  int curentFrame = 0;
  float timePerFrame = 0.2f;
  float timeSinceLastFrame = 0.f;
  float laserCooldown = 0.5f;
  float timeSinceLastLaser = 0.f;
  int direction = 1;

  sf::SoundBuffer laserBuffer;
  sf::Sound laserSound;

  void handleAnimation(int direction, float dt);

  float minLaserCooldown = 0.1f; //minimum laser cooldown
  float cooldownReduction = 0.025f; //25ms reduction in cooldown per kill
};

#endif

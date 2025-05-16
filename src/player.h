#pragma once
#ifndef PLAYER_H
#define PLAYER_H

// Include dependencies
#include "laser.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

// Forward declaration to avoid circular dependency
class Hitbox;

class Player {
public:
  // Constructor: initializes the player with a texture and speed
  Player(std::string texturePath, float speed);

  // Destructor: cleans up dynamically allocated memory
  ~Player();

  // Returns a reference to the player's hitbox (for collision detection)
  const Hitbox &getHitbox() const;

  // Sets the player's position (used for teleporting or cutscenes)
  void setPosition(const sf::Vector2f &pos);

  // Reduces player's health by a specified amount
  void takeDamage(int amount);

  // Checks if the player is currently allowed to take damage (based on cooldown)
  bool canTakeDamage() const;

  // Handle keyboard and mouse input for movement and shooting
  void handleInput(float dt, std::vector<Laser> &lasers, sf::Vector2f mousePos);

// Renders the player sprite to the window
  void draw(sf::RenderWindow &window);

// Updates the player's state each frame (movement, cooldowns, etc.)
  void update(float dt, std::vector<Laser> &lasers, sf::Vector2f mousePos);

// Returns the player's current position
  sf::Vector2f getPosition();

// Returns the bounding box of the player's sprite (used for collision)
  sf::FloatRect getGlobalBounds();

// Returns the player's current health
  int getHealth() const;

  // Sets the player's health to a new value
  void setHealth(int newHealth);

  // Creates and fires a laser in a given direction
  void createLaser(std::vector<Laser> &lasers, sf::Vector2f playerPos,
                   sf::Vector2f fireDirection);

  // Reduces the laser cooldown based on the number of enemies killed
  void decreaseLaserCooldown(int killCount); //laser cooldown decreases as you kill chasers

  // Returns the current laser cooldown value
  float getLaserCooldown() const; //getter for laser cooldown

  // Resets the laser cooldown to its default value (e.g., on level transition)
  void resetLaserCooldown(); //reset laser cooldown (on level transition)

private:
  Hitbox *hitbox; // Pointer to the player's hitbox

  // Damage handling
  float damageCooldown = 0.5f; // Cooldown time between taking damage
  float timeSinceLastHit = 0.f; // Time since last damage taken
  // Graphics
  sf::Texture texture; // Player texture
  sf::Sprite sprite; // Player sprite
  // Movement and state
  float speed = 100.f; // Movement speed
  int health = 10; // Player health

  // Animation (currently unused)
  int curentFrame = 0;
  float timePerFrame = 0.2f;
  float timeSinceLastFrame = 0.f;
  int direction = 1;

  // Laser firing
  float laserCooldown = 0.5f;
  float timeSinceLastLaser = 0.f;
  float minLaserCooldown = 0.1f; // minimum laser cooldown
  float cooldownReduction = 0.025f; // 25ms reduction in cooldown per kill

  int direction = 1;

  // Sound
  sf::SoundBuffer laserBuffer; // Buffer for laser sound
  sf::Sound laserSound; // Sound object for playing laser sound

  // Handles animation updates (currently a placeholder)
  void handleAnimation(int direction, float dt);
};

#endif

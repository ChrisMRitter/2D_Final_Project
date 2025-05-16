// textManager.h
// The textManager class is intended to provide a
// singular class to manage the majority of text.

#pragma once
#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>

// Class responsible for managing all on-screen text elements
class textManager {
public:
  // Loads a font from the specified file path
  bool loadFont(const std::string &path);

  // Updates the player's health text and positions it on the screen
  void updatePlayerHealth(int health, sf::Vector2f viewCenter,
                          sf::Vector2f viewSize);

  // Draws all managed text elements to the given render window
  void draw(sf::RenderWindow &window);

  // Adds a specified amount to the player's score
  void addScore(int amount);

  // Updates the score display text and positions it on the screen
  void updateScoreDisplay(sf::Vector2f viewCenter, sf::Vector2f viewSize);

  // Returns the current score value
  int getScore();

  // Updates the laser cooldown display (shown as fire rate) and positions it
  void updateLaserCooldownDisplay(float cooldown, sf::Vector2f viewCenter, sf::Vector2f viewSize);

private:
  sf::Font font; // Font used for all text
  sf::Text playerHealthText; // Text displaying player's health
  int score = 0; // Player's current score
  sf::Text scoreText; // Text displaying the score
  sf::Text laserCooldownText; // Text displaying the laser cooldown
};

#endif

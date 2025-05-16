// textManager.cpp
// The textManager class is intended to provide a
// singular class to manage the majority of text.

#include "textManager.h"
#include <iostream>

// Loads a font from the specified path and sets up the player health text
bool textManager::loadFont(const std::string &path) {
  if (!font.loadFromFile(path)) {
    std::cerr << "Failed to load font: " << path << std::endl;
    return false;
  }

  // Configure player health text appearance
  playerHealthText.setFont(font);
  playerHealthText.setCharacterSize(24);
  playerHealthText.setFillColor(sf::Color::Red);
  playerHealthText.setStyle(sf::Text::Bold);
  return true;
}

// Updates the player's health display text and positions it near the top center of the screen
void textManager::updatePlayerHealth(int health, sf::Vector2f viewCenter,
                                     sf::Vector2f viewSize) {
  playerHealthText.setString("Health: " + std::to_string(health));
  playerHealthText.setPosition(
      viewCenter.x - playerHealthText.getGlobalBounds().width / 2.f,
      viewCenter.y - viewSize.y / 2.f + 10.f);
}

// Adds a specified amount to the player's score
void textManager::addScore(int amount) { score += amount; }

//  Returns the current score
int textManager::getScore() { return score; }

// Updates the score display text and positions it below the health text
void textManager::updateScoreDisplay(sf::Vector2f viewCenter,
                                     sf::Vector2f viewSize) {
  scoreText.setFont(font);
  scoreText.setCharacterSize(20);
  scoreText.setFillColor(sf::Color::White);
  scoreText.setString("Score: " + std::to_string(score));

  scoreText.setPosition(viewCenter.x - scoreText.getGlobalBounds().width / 2.f,
                        viewCenter.y - viewSize.y / 2.f +
                            40.f); // 40 pixels from top
}

// Draws all text elements (health, score, laser cooldown) to the window
void textManager::draw(sf::RenderWindow &window) {
  window.draw(playerHealthText);
  window.draw(scoreText);
  window.draw(laserCooldownText);
}

// Updates the laser cooldown display text and positions it below the score
void textManager::updateLaserCooldownDisplay(float cooldown, sf::Vector2f viewCenter, sf::Vector2f viewSize) {
  laserCooldownText.setFont(font);
  laserCooldownText.setCharacterSize(16);
  laserCooldownText.setFillColor(sf::Color::Yellow);
  
  // Format the cooldown as fire rate (shots per second), rounded to 2 decimal places
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "Fire Rate: %.2f", 1.0f / cooldown);
  laserCooldownText.setString(buffer);

  laserCooldownText.setPosition(
      viewCenter.x - laserCooldownText.getGlobalBounds().width / 2.f,
      viewCenter.y - viewSize.y / 2.f + 70.f); // 70 pixels from top
}

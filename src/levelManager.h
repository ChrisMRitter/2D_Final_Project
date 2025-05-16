#pragma once
#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

// Include dependencies for game components
#include "Level3Manager.h"
#include "animatedBackground.h"
#include "asteroid.h"
#include "enemyManager.h"
#include "gameconstants.h"
#include "laser.h"
#include "player.h"
#include "textManager.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

/*
The LevelManager class handles level progression, enemy spawning,
asteroid management, cutscenes, and interactions between game systems.
*/
class LevelManager {
public:
  // Constructor: initializes the level manager with references to game systems and textures
  LevelManager(textManager *textMgr, AnimatedBackground *background,
               Player *player, std::shared_ptr<sf::Texture> greenEnemyTex,
               std::shared_ptr<sf::Texture> redEnemyTex,
               std::shared_ptr<sf::Texture> mothershipTex,
               std::shared_ptr<sf::Texture> asteroidTex);

  // Updates the current level state (enemies, asteroids, cutscenes, etc.)
  void update(float dt, std::vector<Laser> &lasers);

  // Draws all level-related entities (enemies, asteroids)
  void draw(sf::RenderWindow &window);

  // Returns whether a cutscene is currently active
  bool isCutsceneActive() const { return cutsceneActive; }

  // Returns the current level number
  int getLevel() const { return currentLevel; }

private:
  // Loads and initializes a specific level
  void loadLevel(int levelNumber);

  // Configures enemy managers based on the level
  void setupEnemiesForLevel(int levelNumber);

  // References to external systems
  textManager *textMgr;
  AnimatedBackground *bg;
  Player *player;

  // Shared textures for different enemy types and asteroids
  std::shared_ptr<sf::Texture> greenEnemyTexture;
  std::shared_ptr<sf::Texture> redEnemyTexture;
  std::shared_ptr<sf::Texture> mothershipTexture;
  std::shared_ptr<sf::Texture> asteroidTexture; // new

  // Enemy managers for the current level
  std::vector<std::unique_ptr<EnemyManager>> enemyManagers;

  // Asteroids present in the level
  std::vector<Asteroid> asteroids; // new

  // Level state tracking
  int currentLevel = 0; // Current level number
  int totalKills = 0; // Total enemies killed
  int chasersSpawned = 0; // Number of chasers spawned
  bool cutsceneActive = false; // Whether a cutscene is playing

  // Asteroid spawning control
  float asteroidSpawnTimer = 0.f;    // Time since last asteroid spawn
  float asteroidSpawnCooldown = 0.f; // Time between asteroid spawns

  // Constants
  static constexpr int kChasersToSpawn = 30; // Enemies to defeat per level
  static constexpr int kScoreThreshold = 3000; // Score needed for progression (unused here)
  const float driftSpeed = 100.f; // Speed of player drift during cutscenes
};

#endif

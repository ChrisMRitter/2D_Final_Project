#pragma once
#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

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

class LevelManager {
public:
  LevelManager(textManager *textMgr, AnimatedBackground *background,
               Player *player, std::shared_ptr<sf::Texture> greenEnemyTex,
               std::shared_ptr<sf::Texture> redEnemyTex,
               std::shared_ptr<sf::Texture> mothershipTex,
               std::shared_ptr<sf::Texture> asteroidTex);

  void update(float dt, std::vector<Laser> &lasers);
  void draw(sf::RenderWindow &window);

  bool isCutsceneActive() const { return cutsceneActive; }
  int getLevel() const { return currentLevel; }

private:
  void loadLevel(int levelNumber);
  void setupEnemiesForLevel(int levelNumber);

  textManager *textMgr;
  AnimatedBackground *bg;
  Player *player;

  std::shared_ptr<sf::Texture> greenEnemyTexture;
  std::shared_ptr<sf::Texture> redEnemyTexture;
  std::shared_ptr<sf::Texture> mothershipTexture;
  std::shared_ptr<sf::Texture> asteroidTexture; // new

  std::vector<std::unique_ptr<EnemyManager>> enemyManagers;
  std::vector<Asteroid> asteroids; // new

  int currentLevel = 0;
  int totalKills = 0;
  int chasersSpawned = 0;
  bool cutsceneActive = false;

  // Asteroid spawn
  float asteroidSpawnTimer = 0.f;    // new
  float asteroidSpawnCooldown = 0.f; // new

  static constexpr int kChasersToSpawn = 30;
  static constexpr int kScoreThreshold = 3000;
  const float driftSpeed = 100.f;
};

#endif

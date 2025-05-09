#pragma once
#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "animatedBackground.h"
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
               Player *player, std::shared_ptr<sf::Texture> chaserTex);

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
  std::shared_ptr<sf::Texture> chaserTexture;

  std::vector<std::unique_ptr<EnemyManager>> enemyManagers;

  int currentLevel = 0;
  int totalKills = 0;
  int chasersSpawned = 0;
  bool cutsceneActive = false;

  static constexpr int kChasersToSpawn = 30;
  static constexpr int kScoreThreshold = 3000;
  const float driftSpeed = 100.f;
};

#endif

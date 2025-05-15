#include "levelManager.h"
#include "Level3Manager.h"
#include "hitbox.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>

LevelManager::LevelManager(textManager *textMgr, AnimatedBackground *background,
                           Player *player,
                           std::shared_ptr<sf::Texture> greenEnemyTex,
                           std::shared_ptr<sf::Texture> redEnemyTex,
                           std::shared_ptr<sf::Texture> mothershipTex,
                           std::shared_ptr<sf::Texture> asteroidTex)
    : textMgr(textMgr), bg(background), player(player),
      greenEnemyTexture(std::move(greenEnemyTex)),
      redEnemyTexture(std::move(redEnemyTex)),
      mothershipTexture(std::move(mothershipTex)),
      asteroidTexture(std::move(asteroidTex)) {
  std::srand(static_cast<unsigned>(std::time(nullptr)));
  loadLevel(1);
}

void LevelManager::loadLevel(int levelNumber) {
  currentLevel = levelNumber;
  totalKills = 0;
  chasersSpawned = 0;
  cutsceneActive = false;
  asteroids.clear();
  asteroidSpawnTimer = 0.f;

  // Set asteroid spawn rate per level
  if (levelNumber == 1)
    asteroidSpawnCooldown = 3.f;
  else if (levelNumber == 2)
    asteroidSpawnCooldown = 1.5f;
  else
    asteroidSpawnCooldown = std::numeric_limits<float>::max();

  setupEnemiesForLevel(levelNumber);

  // Reset player to bottom-center for the new level
  player->setPosition({VIEW_WIDTH / 2.f, VIEW_HEIGHT - 100.f});
  // Reset the laser cooldown when loading a new level
  player->resetLaserCooldown();

  // Set player health based on level
  if (levelNumber == 2) {
    player->setHealth(15); // More health for Level 2
  } else if (levelNumber == 3) {
    player->setHealth(25); // More health for level 3
  }
}

void LevelManager::setupEnemiesForLevel(int levelNumber) {
  enemyManagers.clear();

  if (levelNumber == 1) {
    // Level 1: Green Enemies
    auto mgr = std::make_unique<EnemyManager>();
    mgr->configure(greenEnemyTexture,
                   /*maxEn=*/kChasersToSpawn,
                   /*health=*/2,
                   /*cooldown=*/0.5f,
                   /*speed=*/120.f,
                   /*scale=*/0.1f);
    enemyManagers.emplace_back(std::move(mgr));
  } else if (levelNumber == 2) {
    // Level 2: Red Enemies/Mutants
    auto mgr = std::make_unique<EnemyManager>();
    mgr->configure(redEnemyTexture,
                   /*maxEn=*/kChasersToSpawn,
                   /*health=*/3,
                   /*cooldown=*/0.5f,
                   /*speed=*/140.f,
                   /*scale=*/0.15f);
    enemyManagers.emplace_back(std::move(mgr));
  } else if (levelNumber == 3) {
    // Level 3: Mixed enemies and mothership
    auto level3Mgr = std::make_unique<Level3Manager>();
    level3Mgr->configureLevel3(greenEnemyTexture, redEnemyTexture,
                               mothershipTexture,
                               /*maxSquads=*/5,
                               /*squadSize=*/6);
    enemyManagers.emplace_back(std::move(level3Mgr));
  }
}

void LevelManager::update(float dt, std::vector<Laser> &lasers) {
  // Check if we're in a cutscene or if we have managers to update
  if (enemyManagers.empty() && !cutsceneActive) {
    return; // Nothing to do if no managers and not in cutscene
  }

  // Handle cutscene logic (for level transitions)
  if (cutsceneActive) {
    // Drift ship up
    sf::Vector2f pos = player->getPosition();
    pos.y -= driftSpeed * dt;
    player->setPosition(pos);

    auto pb = player->getGlobalBounds();
    if (pb.top + pb.height < 0.f) {
      // Load background for next level
      std::string bgPath;
      if (currentLevel == 1) {
        bgPath =
            "./Assets/Sprites/test_sprites/gassprite.png"; // Level 2 background
      } else if (currentLevel == 2) {
        bgPath = "./Assets/Sprites/test_sprites/blackholesprite.png";
      }

      auto newBg = std::make_shared<sf::Texture>();
      if (!newBg->loadFromFile(bgPath)) {
        std::cerr << "Failed to load Level " << (currentLevel + 1)
                  << " background\n";
      } else {
        bg->loadSheet(newBg, /*fps=*/60.f);
        bg->setScaleFactor(0.3f);
      }
      loadLevel(currentLevel + 1);
    }
    return;
  }

  // Special handling for Level 3
  if (currentLevel == 3) {
    if (enemyManagers.empty())
      return; // No managers in level 3, nothing to do

    auto &mgr = *enemyManagers.front();
    auto *level3Mgr = dynamic_cast<Level3Manager *>(&mgr);
    if (!level3Mgr) {
      std::cerr << "Error: Level 3 manager is not a Level3Manager\n";
      return;
    }

    // Update Level 3 manager
    level3Mgr->update(dt, player->getPosition(), true);

    // Handle collision detection
    int kills = level3Mgr->handleLaserCollisions(lasers);

    // Apply cooldown reduction for Level 3
    if (kills > 0) {
      player->decreaseLaserCooldown(kills);
    }

    totalKills += kills;
    textMgr->addScore(kills * 100);

    // Handle player collisions with enemies
    level3Mgr->handlePlayerCollisions(*player, /*damage=*/1);

    // Handle enemy laser firing
    if (level3Mgr->canMothershipFireLaser()) {
      level3Mgr->fireLaserFromMothership(lasers, player->getPosition());
    }

    // Handle player collisions with enemy lasers
    for (auto it = lasers.begin(); it != lasers.end();) {
      if (!it->isFromEnemy()) {
        // Skip player lasers
        ++it;
        continue;
      }

      // Check collision with player
      if (it->getGlobalBounds().intersects(player->getHitbox().getBounds())) {
        player->takeDamage(1);
        it = lasers.erase(it);
      } else {
        ++it;
      }
    }

    // Check if mothership is defeated
    if (level3Mgr->isMothershipPhaseActive() &&
        level3Mgr->isMothershipDefeated()) {
      // Game won! Handle victory state
      std::cout << "Mothership defeated! Victory!\n";
      // Could transition to a victory screen or next level
    }

    return;
  }

  // Asteroids for Levels 1 & 2
  asteroidSpawnTimer += dt;
  if (asteroidSpawnTimer >= asteroidSpawnCooldown) {
    float x = static_cast<float>(std::rand() % static_cast<int>(VIEW_WIDTH));
    asteroids.emplace_back(*asteroidTexture, sf::Vector2f{x, -32.f},
                           sf::Vector2f{0.f, 200.f});
    asteroidSpawnTimer = 0.f;
  }

  for (auto it = asteroids.begin(); it != asteroids.end();) {
    it->update(dt);
    if (it->checkCollision(player->getGlobalBounds())) {
      player->takeDamage(1);
      it = asteroids.erase(it);
    } else if (it->getBounds().top > VIEW_HEIGHT) {
      it = asteroids.erase(it);
    } else {
      ++it;
    }
  }

  // Regular handling for Levels 1 & 2 (only reached if not in cutscene)
  if (enemyManagers.empty())
    return; // Safety check

  auto &mgr = *enemyManagers.front();
  chasersSpawned = totalKills + mgr.getAliveCount();
  bool allowSpawn = (chasersSpawned < kChasersToSpawn);
  mgr.update(dt, player->getPosition(), allowSpawn);

  int kills = mgr.handleLaserCollisions(lasers);
  // If enemies were killed, decrease the laser cooldown
  if (kills > 0 && currentLevel > 1) { // Static laser cooldown for level 1
    player->decreaseLaserCooldown(kills);
  }
  totalKills += kills;
  textMgr->addScore(kills * 100);

  mgr.handlePlayerCollisions(*player, /*damage=*/1);

  // Check if level is complete - New win condition: finish when all chasers are
  // defeated
  if (currentLevel < 3 && totalKills >= kChasersToSpawn) {
    cutsceneActive = true;
    enemyManagers.clear(); // Clear sprites
  }
}

void LevelManager::draw(sf::RenderWindow &window) {
  if (!cutsceneActive) {
    for (auto &mgr : enemyManagers)
      mgr->draw(window);
    for (auto &ast : asteroids)
      ast.draw(window);
  }
}

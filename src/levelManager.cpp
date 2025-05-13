#include "levelManager.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

LevelManager::LevelManager(textManager *textMgr, AnimatedBackground *background,
                           Player *player,
                           std::shared_ptr<sf::Texture> chaserTex)
    : textMgr(textMgr), bg(background), player(player),
      chaserTexture(std::move(chaserTex)) {
  loadLevel(1);
}

void LevelManager::loadLevel(int levelNumber) {
  currentLevel = levelNumber;
  totalKills = 0;
  chasersSpawned = 0;
  cutsceneActive = false;
  setupEnemiesForLevel(levelNumber);

  // Reset player to bottom‐center for the new level
  player->setPosition({VIEW_WIDTH / 2.f, VIEW_HEIGHT - 100.f});
  // Reset the laser cooldown when loading a new level
  player->resetLaserCooldown();
}

void LevelManager::setupEnemiesForLevel(int levelNumber) {
  enemyManagers.clear();
  if (levelNumber == 1) {
    auto mgr = std::make_unique<EnemyManager>();
    mgr->configure(chaserTexture,
                   /*maxEn=*/kChasersToSpawn,
                   /*health=*/2,
                   /*cooldown=*/0.5f,
                   /*speed=*/120.f,
                   /*scale=*/0.1f);
    enemyManagers.emplace_back(std::move(mgr));
  }
  //enemyManagers.clear();
  if (levelNumber == 2) {
    auto mgr = std::make_unique<EnemyManager>();
    mgr->configure(chaserTexture,
      /*maxEn=*/kChasersToSpawn,
      /*health=*/3,
      /*cooldown=*/0.5f,
      /*speed=*/140.f,
      /*scale=*/0.2f);
      enemyManagers.emplace_back(std::move(mgr));
    
  }
  // TODO: add Level 2 & 3 setups


}

void LevelManager::update(float dt, std::vector<Laser> &lasers) {
  
  //if (enemyManagers.empty()) return; //debugging: if no enemy managers, return

  

  // safe because we only enter L1 when enemyManagers non‐empty
  auto &mgr = *enemyManagers.front();

  if (!cutsceneActive) {
    chasersSpawned = totalKills + mgr.getAliveCount();
    bool allowSpawn = (chasersSpawned < kChasersToSpawn);
    mgr.update(dt, player->getPosition(), allowSpawn);

    int kills = mgr.handleLaserCollisions(lasers);
    // If enemies were killed, decrease the laser cooldown
    if (kills > 0 && currentLevel > 1) { //static laser Cooldown for level 1
      player->decreaseLaserCooldown(kills);
    }
    totalKills += kills;
    textMgr->addScore(kills * 100);

    

    mgr.handlePlayerCollisions(*player, /*damage=*/1);

    if (textMgr->getScore() >= kScoreThreshold &&
        chasersSpawned >= kChasersToSpawn) {
      cutsceneActive = true;
      enemyManagers.clear(); // clear sprites
    }

  } else {
    // drift ship up
    sf::Vector2f pos = player->getPosition();
    pos.y -= driftSpeed * dt;
    player->setPosition(pos);

    if (pos.y + player->getGlobalBounds().height < 0) {
      // load your actual Level 2 background here:
      auto newBg = std::make_shared<sf::Texture>();
      if (!newBg->loadFromFile("./Assets/Sprites/test_sprites/gassprite.png")) {
        std::cerr << "Failed to load Level 2 background\n";
      } else {
        bg->loadSheet(newBg, /*fps=*/6.f);
        bg->setScaleFactor(0.5f);
      }
      loadLevel(2);
    }
  }
}

void LevelManager::draw(sf::RenderWindow &window) {
  if (!cutsceneActive) {
    for (auto &mgr : enemyManagers)
      mgr->draw(window);
  }
}

#include "levelManager.h"
#include "Level3Manager.h"
#include "hitbox.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>

LevelManager::LevelManager(textManager *textMgr,
                           AnimatedBackground *background,
                           Player *player,
                           std::shared_ptr<sf::Texture> greenEnemyTex,
                           std::shared_ptr<sf::Texture> redEnemyTex,
                           std::shared_ptr<sf::Texture> mothershipTex,
                           std::shared_ptr<sf::Texture> asteroidTex)
    : textMgr(textMgr),
      bg(background),
      player(player),
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

    // set asteroid spawn rate per level
    if (levelNumber == 1)       asteroidSpawnCooldown = 3.f;
    else if (levelNumber == 2)  asteroidSpawnCooldown = 1.5f;
    else                        asteroidSpawnCooldown = std::numeric_limits<float>::max();

    setupEnemiesForLevel(levelNumber);

    // Reset player
    player->setPosition({VIEW_WIDTH / 2.f, VIEW_HEIGHT - 100.f});
    player->resetLaserCooldown();
    if (levelNumber == 2)       player->setHealth(15);
    else if (levelNumber == 3)  player->setHealth(25);
}

void LevelManager::setupEnemiesForLevel(int levelNumber) {
    enemyManagers.clear();
    if (levelNumber == 1) {
        auto mgr = std::make_unique<EnemyManager>();
        mgr->configure(greenEnemyTexture, kChasersToSpawn, 2, 0.5f, 120.f, 0.1f);
        enemyManagers.emplace_back(std::move(mgr));
    } else if (levelNumber == 2) {
        auto mgr = std::make_unique<EnemyManager>();
        mgr->configure(redEnemyTexture, kChasersToSpawn, 3, 0.5f, 140.f, 0.15f);
        enemyManagers.emplace_back(std::move(mgr));
    } else {
        auto lvl3 = std::make_unique<Level3Manager>();
        lvl3->configureLevel3(greenEnemyTexture,
                              redEnemyTexture,
                              mothershipTexture,
                              /*maxSquads=*/5,
                              /*squadSize=*/6);
        enemyManagers.emplace_back(std::move(lvl3));
    }
}

void LevelManager::update(float dt, std::vector<Laser> &lasers) {
    if (enemyManagers.empty() && !cutsceneActive) return;

    // Cutscene handling
    if (cutsceneActive) {
        sf::Vector2f pos = player->getPosition();
        pos.y -= driftSpeed * dt;
        player->setPosition(pos);

        auto pb = player->getGlobalBounds();
        if (pb.top + pb.height < 0.f) {
            std::string bgPath;
            if (currentLevel == 1)
                bgPath = "./Assets/Sprites/test_sprites/gassprite.png";
            else if (currentLevel == 2)
                bgPath = "./Assets/Sprites/test_sprites/blackholesprite.png";

            auto newBg = std::make_shared<sf::Texture>();
            if (newBg->loadFromFile(bgPath)) {
                bg->loadSheet(newBg, /*fps=*/60.f);
                bg->setScaleFactor(0.3f);
            }
            loadLevel(currentLevel + 1);
        }
        return;
    }

    // Level 3 special logic
    if (currentLevel == 3) {
        auto &mgr    = *enemyManagers.front();
        auto *l3     = dynamic_cast<Level3Manager*>(&mgr);
        l3->update(dt, player->getPosition(), true);

        int kills = l3->handleLaserCollisions(lasers);
        if (kills > 0) player->decreaseLaserCooldown(kills);
        totalKills += kills;
        textMgr->addScore(kills * 100);

        l3->handlePlayerCollisions(*player, 1);
        if (l3->canMothershipFireLaser())
            l3->fireLaserFromMothership(lasers, player->getPosition());

        for (auto it = lasers.begin(); it != lasers.end();) {
            if (it->isFromEnemy() &&
                it->getGlobalBounds().intersects(player->getHitbox().getBounds())) {
                player->takeDamage(1);
                it = lasers.erase(it);
            } else ++it;
        }
        return;
    }

    // Asteroids for Levels 1 & 2
    asteroidSpawnTimer += dt;
    if (asteroidSpawnTimer >= asteroidSpawnCooldown) {
        float x = static_cast<float>(std::rand() % static_cast<int>(VIEW_WIDTH));
        asteroids.emplace_back(
            *asteroidTexture,
            sf::Vector2f{x, -32.f},
            sf::Vector2f{0.f, 200.f}
        );
        asteroidSpawnTimer = 0.f;
    }
    for (auto it = asteroids.begin(); it != asteroids.end();) {
        it->update(dt);
        if (it->checkCollision(player->getGlobalBounds())) {
            player->takeDamage(1);
            it = asteroids.erase(it);
        } else if (it->getBounds().top > VIEW_HEIGHT) {
            it = asteroids.erase(it);
        } else ++it;
    }

    // Enemy handling for Levels 1 & 2
    auto &mgr = *enemyManagers.front();
    chasersSpawned = totalKills + mgr.getAliveCount();
    bool allowSpawn = (chasersSpawned < kChasersToSpawn);
    mgr.update(dt, player->getPosition(), allowSpawn);

    int kills = mgr.handleLaserCollisions(lasers);
    if (kills > 0 && currentLevel > 1)
        player->decreaseLaserCooldown(kills);
    totalKills += kills;
    textMgr->addScore(kills * 100);
    mgr.handlePlayerCollisions(*player, 1);

    // New win condition: finish when all chasers are defeated
    if (currentLevel < 3 && totalKills >= kChasersToSpawn) {
        cutsceneActive = true;
        enemyManagers.clear();
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


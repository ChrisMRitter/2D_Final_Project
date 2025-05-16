// src/Level3Manager.cpp
#include "Level3Manager.h"
#include "hitbox.h"
#include "gameconstants.h"
#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>

/*
Level3Manager extends EnemyManager to implement a boss fight scenario
featuring enemy squads and a mothership with bullet-hell attack patterns.
*/

//Note: Much of this file and Level3Manager.h was AI generated and human edited. I wanted a boss fight but wasn't sure how to implement it. Otherwise, level 3 would've been
//a slight variation on the other levels

Level3Manager::Level3Manager() 
    : mothershipPhaseActive(false),
      mothershipLaserCooldown(1.5f),
      timeSinceLastMothershipLaser(0.0f),
      mothershipHealth(50),
      maxMothershipHealth(50),
      squadSize(6),
      squadsSpawned(0),
      maxSquads(5),
      timeSinceLastSquad(0.0f),
      squadCooldown(5.0f)
{
    // Try to load font for health bar
    if (!font.loadFromFile("./Assets/Fonts/VeniteAdoremus-rgRBA.ttf")) {
        std::cerr << "Failed to load font for mothership health bar\n";
    }
    
    // Setup health bar
    healthBarBackground.setSize(sf::Vector2f(200.f, 20.f));
    healthBarBackground.setFillColor(sf::Color(100, 100, 100, 200));
    healthBarBackground.setPosition(VIEW_WIDTH / 2.f - 100.f, 30.f);
    
    healthBarForeground.setSize(sf::Vector2f(200.f, 20.f));
    healthBarForeground.setFillColor(sf::Color::Red);
    healthBarForeground.setPosition(VIEW_WIDTH / 2.f - 100.f, 30.f);
    
    healthText.setFont(font);
    healthText.setCharacterSize(16);
    healthText.setFillColor(sf::Color::White);
    healthText.setPosition(VIEW_WIDTH / 2.f - 20.f, 31.f);
}

void Level3Manager::configureLevel3(
    std::shared_ptr<sf::Texture> greenEnemyTex,
    std::shared_ptr<sf::Texture> redEnemyTex,
    std::shared_ptr<sf::Texture> mothershipTex,
    int maxSquads, int squadSize)
{
    // Store textures
    greenEnemyTexture = greenEnemyTex;
    redEnemyTexture = redEnemyTex;
    mothershipTexture = mothershipTex;
    
    // Configure squad settings
    this->maxSquads = maxSquads;
    this->squadSize = squadSize;

    mothershipHealth = 500;
    maxMothershipHealth = 500; // I don't remember why I did this but I'm a little scared to remove it.

    mothershipLaserCooldown = 0.3f;
    
    // Set up distributions for enemy properties: I got this idea from Claude
    rng = std::mt19937(std::random_device{}());
    hpDist = std::uniform_int_distribution<>(2, 3); // 2 for green, 3 for red
    speedDist = std::uniform_real_distribution<float>(120.f, 140.f); // Range between green and red speeds
}

void Level3Manager::update(float dt, const sf::Vector2f& playerPos, bool allowSpawn) {
    // Update existing enemies
    for (auto& e : enemies) {
        e.update(dt, playerPos);
    }
    
    // Remove dead enemies
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
                      [](const Enemy& e){ return !e.isAlive(); }),
        enemies.end()
    );
    
    if (mothershipPhaseActive) {
        // Mothership battle phase logic
        timeSinceLastMothershipLaser += dt;
        
        // Find mothership in enemies array
        for (auto& e : enemies) {
            if (e.getType() == EnemyType::MOTHERSHIP) {
                // Update health bar
                updateMothershipHealthBar();
                break;
            }
        }
    } 
    else if (allowSpawn && squadsSpawned < maxSquads) {
        // Squad phase - spawn squads with cooldown
        timeSinceLastSquad += dt;
        if (timeSinceLastSquad >= squadCooldown && 
            enemies.size() < static_cast<size_t>(maxSquads * squadSize)) {
            spawnSquad();
            timeSinceLastSquad = 0.f;
            squadsSpawned++;
        }
    }
    
    // Check if all squads are defeated to start mothership phase
    if (!mothershipPhaseActive && squadsSpawned >= maxSquads && enemies.empty()) {
        // All squads defeated, start mothership phase
        // Create mothership at top-center of screen
        float mothershipX = VIEW_WIDTH / 2.f;
        float mothershipY = 150.f;
        
        //Mothership data
        enemies.emplace_back(mothershipX, mothershipY, 0.f, mothershipHealth, 
                          mothershipTexture, 0.8f, EnemyType::MOTHERSHIP);
        
        mothershipPhaseActive = true;
    }
}

void Level3Manager::draw(sf::RenderWindow& window) {
    // Draw all enemies
    for (auto& e : enemies) {
        e.draw(window);
    }
    
    // Draw health bar if mothership is active
    if (mothershipPhaseActive) {
        window.draw(healthBarBackground);
        window.draw(healthBarForeground);
        window.draw(healthText);
    }
}

int Level3Manager::handleLaserCollisions(std::vector<Laser>& lasers) {
    int kills = 0;
    
    // Check collisions only with player lasers (not enemy lasers)
    //A bit excessive for the build we ended up with, but at one point I was planning on making the regular enemies shoot lasers.
    for (auto it = lasers.begin(); it != lasers.end();) {
        // Skip enemy lasers
        if (it->isFromEnemy()) {
            ++it;
            continue;
        }
        
        bool hit = false;
        for (auto& e : enemies) {
            if (it->getGlobalBounds().intersects(e.getGlobalBounds())) {
                if (e.takeDamage() <= 0) {
                    if (e.getType() != EnemyType::MOTHERSHIP) {
                        ++kills; // Only count non-mothership enemies for kill count
                    }
                }
                hit = true;
                break;
            }
        }
        
        if (hit) {
            it = lasers.erase(it);
        } else {
            ++it;
        }
    }
    
    return kills;
}
//TODO: Refactor and remove this, use the already implemented code. DRY
void Level3Manager::handlePlayerCollisions(Player& player, int damage) {
    // Check collisions between player and enemies
    for (auto& e : enemies) {
        // Mothership doesn't damage on collision
        if (e.getType() == EnemyType::MOTHERSHIP) {
            continue;
        }
        
        if (e.getGlobalBounds().intersects(player.getHitbox().getBounds())) {
            player.takeDamage(damage);
        }
    }
    
}

// Checks if all squads have been spawned and defeated
bool Level3Manager::areAllSquadsDefeated() const {
    return squadsSpawned >= maxSquads && 
           std::all_of(enemies.begin(), enemies.end(), 
                      [](const Enemy& e) { return e.getType() == EnemyType::MOTHERSHIP; });
}

// Returns whether the mothership has been defeated
bool Level3Manager::isMothershipDefeated() const {
    // Check if mothership is in enemies list and not alive
    return mothershipPhaseActive && 
           std::none_of(enemies.begin(), enemies.end(), 
                       [](const Enemy& e) { return e.getType() == EnemyType::MOTHERSHIP; });
}

// Checks if the mothership can fire based on cooldown
bool Level3Manager::canMothershipFireLaser() const {
    // Can fire if mothership phase is active and cooldown has passed
    return mothershipPhaseActive && 
           timeSinceLastMothershipLaser >= mothershipLaserCooldown &&
           std::any_of(enemies.begin(), enemies.end(),
                      [](const Enemy& e) { return e.getType() == EnemyType::MOTHERSHIP; });
}

// Fires laser(s) at player
void Level3Manager::fireLaserFromMothership(std::vector<Laser>& lasers, const sf::Vector2f& playerPos) {
    if (!canMothershipFireLaser()) return; //error handling
    
    // Find the mothership
    for (auto& e : enemies) {
        if (e.getType() == EnemyType::MOTHERSHIP) {
            // Get mothership position
            sf::FloatRect bounds = e.getGlobalBounds();
            sf::Vector2f mothershipPos(bounds.left + bounds.width/2.f, //Center X
                bounds.top + bounds.height); //Bottom Y
            
            // Choose a random pattern based on mothership health percentage
            // Mothership will become more dangerous as it gets damaged
            float healthPercent = (float)e.getHealth() / maxMothershipHealth;
            int patternType = 0;

            //Note: The math for this section was AI suggested
            if (healthPercent > 0.75f) {
            // Phase 1: Simple aimed shots and occasional spread
                patternType = (rand() % 10 > 7) ? 1 : 0;
            } else if (healthPercent > 0.5f) {
            // Phase 2: More spreads and occasional circle
                patternType = (rand() % 10 > 7) ? 2 : 1;
            } else if (healthPercent > 0.25f) {
            // Phase 3: Circles and sprays
                patternType = (rand() % 10 > 7) ? 3 : 2;
            } else {
                // Phase 4: Everything at once - true bullet hell
                patternType = rand() % 4;
            }
                        
            // Fire the chosen pattern
            switch (patternType) {
                case 0: // Single shot at player
                    fireSingleShot(mothershipPos, playerPos, lasers);
                    break;
                case 1: // Spread pattern
                    fireSpreadPattern(mothershipPos, playerPos, lasers);
                    break;
                case 2: // Circle pattern
                    fireCirclePattern(mothershipPos, lasers);
                    break;
                case 3: // Grid pattern
                    fireGridPattern(mothershipPos, lasers);
                    break;
            }
                        
            // Reset cooldown timer
            timeSinceLastMothershipLaser = 0.f;
            break;
        }
    }
}

// Spawns a new enemy squad
void Level3Manager::spawnSquad() {
    // Spawn a squad of enemies at the top of the screen
    for (int i = 0; i < squadSize; ++i) {
        // Calculate position in formation
        float xOffset = (i - squadSize/2.f) * 60.f;
        float x = VIEW_WIDTH / 2.f + xOffset;
        float y = -50.f; // Just above the screen
        
        // Clamp to screen bounds
        x = std::max(50.f, std::min(x, VIEW_WIDTH - 50.f));
        
        // Randomly determine if this is a green or red enemy
        bool isRed = (std::rand() % 3 == 0); // 1/3 chance for red enemies
        
        if (isRed) {
            // Level 2 style enemy (red, tougher, faster)
            enemies.emplace_back(x, y, 140.f, 3, redEnemyTexture, 0.2f, EnemyType::RED_ENEMY);
        } else {
            // Level 1 style enemy (green, weak, fast)
            enemies.emplace_back(x, y, 120.f, 2, greenEnemyTexture, 0.1f, EnemyType::GREEN_ENEMY);
        }
    }
}

// Updates the visual health bar of the mothership
void Level3Manager::updateMothershipHealthBar() {
    // Find mothership and update health bar
    for (auto& e : enemies) {
        if (e.getType() == EnemyType::MOTHERSHIP) {
            float healthRatio = static_cast<float>(e.getHealth()) / maxMothershipHealth;
            healthBarForeground.setSize(sf::Vector2f(200.f * healthRatio, 20.f));
            healthText.setString(std::to_string(e.getHealth()) + "/" + std::to_string(maxMothershipHealth));
            break;
        }
    }
}

// Fires a single laser
void Level3Manager::fireSingleShot(const sf::Vector2f& pos, const sf::Vector2f& targetPos, std::vector<Laser>& lasers) {
    // Calculate direction towards player
    sf::Vector2f direction = targetPos - pos;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) direction /= length;
    
    // Fire single shot
    lasers.emplace_back(pos.x, pos.y, 300.f, direction, true);
}

// Fires a spread of lasers
void Level3Manager::fireSpreadPattern(const sf::Vector2f& pos, const sf::Vector2f& targetPos, std::vector<Laser>& lasers) {
    // Calculate base direction towards player
    sf::Vector2f baseDir = targetPos - pos;
    float length = std::sqrt(baseDir.x * baseDir.x + baseDir.y * baseDir.y);
    if (length > 0) baseDir /= length;
    
    // Get base angle in radians
    float baseAngle = atan2(baseDir.y, baseDir.x);
    
    // Fire 5 shots in a spread
    for (int i = -2; i <= 2; i++) {
        // Rotate the base direction by i*15 degrees
        float angle = baseAngle + (i * 15.f * 3.14159f / 180.f);
        sf::Vector2f dir(cos(angle), sin(angle));
        lasers.emplace_back(pos.x, pos.y, 300.f, dir, true);
    }
}

// Fires lasers in a circular pattern
void Level3Manager::fireCirclePattern(const sf::Vector2f& pos, std::vector<Laser>& lasers) {
    // Fire 12 shots in a circle
    for (int i = 0; i < 12; i++) {
        float angle = i * 30.f * 3.14159f / 180.f;
        sf::Vector2f dir(cos(angle), sin(angle));
        lasers.emplace_back(pos.x, pos.y, 300.f, dir, true);
    }
}

// Fires lasers in a grid pattern
void Level3Manager::fireGridPattern(const sf::Vector2f& pos, std::vector<Laser>& lasers) {
    // Fire a grid of lasers (5 rows x 3 columns)
    for (int row = -2; row <= 2; row++) {
        for (int col = -1; col <= 1; col++) {
            if (col == 0 && row == 0) continue; // Skip center
            
            sf::Vector2f dir(col, row);
            float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (length > 0) dir /= length;
            
            lasers.emplace_back(pos.x, pos.y, 300.f, dir, true);
        }
    }
}

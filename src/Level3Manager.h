// src/Level3Manager.h
#ifndef LEVEL3_MANAGER_H
#define LEVEL3_MANAGER_H

#include "enemyManager.h"
#include "enemyTypes.h"
#include "hitbox.h"
#include <SFML/Graphics.hpp>
#include <memory>

/*
Level3Manager extends EnemyManager to implement a boss fight scenario
featuring enemy squads and a mothership with bullet-hell attack patterns.
*/
class Level3Manager : public EnemyManager {
public:
    Level3Manager(); // Constructor
    ~Level3Manager() = default; // Default destructor

    // Configures Level 3 with enemy textures and squad parameters
    void configureLevel3(
        std::shared_ptr<sf::Texture> greenEnemyTex,
        std::shared_ptr<sf::Texture> redEnemyTex,
        std::shared_ptr<sf::Texture> mothershipTex,
        int maxSquads, int squadSize);

    // Overrides for game loop integration
    void update(float dt, const sf::Vector2f& playerPos, 
                bool allowSpawn = true) override;
    void draw(sf::RenderWindow& window) override;
    int handleLaserCollisions(std::vector<Laser>& lasers) override;
    void handlePlayerCollisions(Player& player, int damage) override;

    // Level 3 specific methods

    // Checks if all squads have been spawned and defeated
    bool areAllSquadsDefeated() const;
    // Returns whether the mothership phase is active
    bool isMothershipPhaseActive() const { return mothershipPhaseActive; }
    // Returns whether the mothership has been defeated
    bool isMothershipDefeated() const;
    // Checks if the mothership can fire based on cooldown
    bool canMothershipFireLaser() const;
    // Fires laser(s) at player
    void fireLaserFromMothership(std::vector<Laser>& lasers, const sf::Vector2f& playerPos);

private:
    void spawnSquad(); // Spawns a new enemy squad
    void updateMothershipHealthBar(); // Updates the visual health bar of the mothership

    // Textures for different enemy types
    std::shared_ptr<sf::Texture> greenEnemyTexture;
    std::shared_ptr<sf::Texture> redEnemyTexture;
    std::shared_ptr<sf::Texture> mothershipTexture;

    // Mothership properties
    bool mothershipPhaseActive = false; // Whether the mothership is currently active
    float mothershipLaserCooldown = 1.5f; // Time between mothership laser shots
    float timeSinceLastMothershipLaser = 0.0f; // Timer tracking time since last laser
    int mothershipHealth = 50; // Current health of the mothership
    int maxMothershipHealth = 50; // Maximum health of the mothership

    //Bullet Hell pattern methods
    void fireSingleShot(const sf::Vector2f& pos, const sf::Vector2f& targetPos, std::vector<Laser>& lasers); // Fires a single laser
    void fireSpreadPattern(const sf::Vector2f& pos, const sf::Vector2f& targetPos, std::vector<Laser>& lasers); // Fires a spread of lasers
    void fireCirclePattern(const sf::Vector2f& pos, std::vector<Laser>& lasers); // Fires lasers in a circular pattern
    void fireGridPattern(const sf::Vector2f& pos, std::vector<Laser>& lasers); // Fires lasers in a grid pattern

    // Squad spawning properties
    int squadSize = 6; // Number of enemies per squad
    int squadsSpawned = 0; // Number of squads spawned so far
    int maxSquads = 5; // Maximum number of squads to spawn
    float timeSinceLastSquad = 0.0f; // Timer for spawning
    float squadCooldown = 5.0f; // Time between squad spawns

    // Health bar properties
    sf::RectangleShape healthBarBackground; // Background of the health bar
    sf::RectangleShape healthBarForeground; // Foreground (red) of the health bar
    sf::Text healthText; // Text displaying health value
    sf::Font font; // Font used for health text
};

#endif // LEVEL3_MANAGER_H

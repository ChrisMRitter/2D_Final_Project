// src/Level3Manager.h
#ifndef LEVEL3_MANAGER_H
#define LEVEL3_MANAGER_H

#include "enemyManager.h"
#include "enemyTypes.h"
#include "hitbox.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Level3Manager : public EnemyManager {
public:
    Level3Manager();
    ~Level3Manager() = default;

    void configureLevel3(
        std::shared_ptr<sf::Texture> greenEnemyTex,
        std::shared_ptr<sf::Texture> redEnemyTex,
        std::shared_ptr<sf::Texture> mothershipTex,
        int maxSquads, int squadSize);

    void update(float dt, const sf::Vector2f& playerPos, 
                bool allowSpawn = true) override;
    void draw(sf::RenderWindow& window) override;
    int handleLaserCollisions(std::vector<Laser>& lasers) override;
    void handlePlayerCollisions(Player& player, int damage) override;

    // Level 3 specific methods
    bool areAllSquadsDefeated() const;
    bool isMothershipPhaseActive() const { return mothershipPhaseActive; }
    bool isMothershipDefeated() const;
    bool canMothershipFireLaser() const;
    void fireLaserFromMothership(std::vector<Laser>& lasers, const sf::Vector2f& playerPos);

private:
    void spawnSquad();
    void updateMothershipHealthBar();

    // Textures for different enemy types
    std::shared_ptr<sf::Texture> greenEnemyTexture;
    std::shared_ptr<sf::Texture> redEnemyTexture;
    std::shared_ptr<sf::Texture> mothershipTexture;

    // Mothership properties
    bool mothershipPhaseActive = false;
    float mothershipLaserCooldown = 1.5f;
    float timeSinceLastMothershipLaser = 0.0f;
    int mothershipHealth = 50;
    int maxMothershipHealth = 50;

    //Bullet Hell pattern methods
    void fireSingleShot(const sf::Vector2f& pos, const sf::Vector2f& targetPos, std::vector<Laser>& lasers);
    void fireSpreadPattern(const sf::Vector2f& pos, const sf::Vector2f& targetPos, std::vector<Laser>& lasers);
    void fireCirclePattern(const sf::Vector2f& pos, std::vector<Laser>& lasers);
    void fireGridPattern(const sf::Vector2f& pos, std::vector<Laser>& lasers);

    // Squad properties
    int squadSize = 6;
    int squadsSpawned = 0;
    int maxSquads = 5;
    float timeSinceLastSquad = 0.0f;
    float squadCooldown = 5.0f;

    // Health bar properties
    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBarForeground;
    sf::Text healthText;
    sf::Font font;
};

#endif // LEVEL3_MANAGER_H
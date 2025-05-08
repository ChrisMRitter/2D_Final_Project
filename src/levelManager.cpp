#include "levelManager.h"
#include <iostream>
#include <memory>

// Default constructor
LevelManager::LevelManager() {
	loadLevel(1); // Load the first level by default
}

void LevelManager::loadLevel(int levelNumber) {
	currentLevel = levelNumber;

	setupEnemiesForLevel(levelNumber);
}

void LevelManager::setupEnemiesForLevel(int levelNumber) {
	enemyManagers.clear();
	
	switch (levelNumber) {
	case 1:
	{
		auto manager = std::make_unique<EnemyManager>();
		// need to configure enemy 1 (e.g. set up sprite)
		// set text manager
		enemyManagers.push_back(std::move(manager));
	}
	case 2:
	{
		auto manager = std::make_unique<EnemyManager>();
		// need to configure enemy 1 (e.g. set up sprite)
		// set text manager
		enemyManagers.push_back(std::move(manager));
		auto manager2 = std::make_unique<EnemyManager>();
		// need to configure enemy 2 (e.g. set up sprite)
		// set text manager
		enemyManagers.push_back(std::move(manager2));
	}
	case 3:
	{
		auto manager = std::make_unique<EnemyManager>();
		// need to configure enemy 1 (e.g. set up sprite)
		// set text manager
		enemyManagers.push_back(std::move(manager));
		auto manager2 = std::make_unique<EnemyManager>();
		// need to configure enemy 2 (e.g. set up sprite)
		// set text manager
		enemyManagers.push_back(std::move(manager2));
		auto manager3 = std::make_unique<EnemyManager>();
		// need to configure enemy 3 (e.g. set up sprite)
		// set text manager
		enemyManagers.push_back(std::move(manager3));
		break;
	}
	};

}

void LevelManager::update(float dt, Player& player, std::vector<Laser>& lasers) {
	// Update each enemy manager (for each enemy type)
	for (auto& manager : enemyManagers) {
		// update with dt, player, and lasers
	}
}

void LevelManager::draw(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2f& playerPos) {
	for (auto& manager : enemyManagers) {
		manager->draw(window);
	}
}

// Need to set up text manager
/*
* void LevelManager::setTextManager(textManager* textMgr) {
	textManager = textMgr;
}
*/

int LevelManager::getLevel() {
	return currentLevel;
}

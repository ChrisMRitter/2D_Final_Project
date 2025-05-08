#pragma once
#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <SFML/Graphics.hpp>
#include "enemyManager.h"
#include "player.h"
#include "laser.h"
#include "textManager.h"
#include <memory>

class LevelManager {
public:
	LevelManager();

	void update(float dt, Player& player, std::vector<Laser>& lasers);
	void draw(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2f& playerPos);

	void loadLevel(int levelNumber);
	void setTextManager(textManager* textMgr);
	int getLevel();
private:
	int currentLevel = 0;
	std::vector<std::unique_ptr<EnemyManager>> enemyManagers;
	textManager* textManager = nullptr;

	void setupEnemiesForLevel(int levelNumber);
};

#endif

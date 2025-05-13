// src/enemyTypes.h
#pragma once
#ifndef ENEMY_TYPES_H
#define ENEMY_TYPES_H

enum class EnemyType {
    GREEN_ENEMY,  // Small, weak, fast (Level 1)
    RED_ENEMY,    // Larger, tougher, faster (Level 2)
    MOTHERSHIP    // Boss (Level 3)
};

#endif // ENEMY_TYPES_H
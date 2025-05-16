// src/enemyTypes.h
#pragma once
#ifndef ENEMY_TYPES_H
#define ENEMY_TYPES_H

enum class EnemyType {
    GREEN_ENEMY,  // Small, weak, fast enemy (introduced in Level 1)
    RED_ENEMY,    // Larger, tougher, faster enemy (introduced in Level 2)
    MOTHERSHIP    // Boss (introduced in Level 3)
};

#endif // ENEMY_TYPES_H

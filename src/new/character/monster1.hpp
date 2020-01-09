#ifndef MONSTER1_HPP
#define MONSTER1_HPP

#include "enemy.hpp"
#include "player1.hpp"

class Monster1 : public Enemy {
public:
    Monster1()
        : Enemy(30,
                0,
                0,

                1,
                1,

                kTileSize,
                static_cast<int>(kTileSize * 1.5),

                4,
                3,
                3,

                2) {
        set_damage(10);
    }

    bool Attack(Character *player);
    bool IA(bool *collision_map, list<Character *> &enemies, Character *p1, Character *p2);
};

#endif // MONSTER1_HPP
#ifndef MONSTER2_HPP
#define MONSTER2_HPP

#include "..\projectile\projectile.hpp"
#include "enemy.hpp"

class Monster2 : public Enemy {
public:
    Monster2()
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
    }

    bool Attack(list<Character *> &players, list<Projectile *> &projectiles, bool *projectile_hit_map);
    bool IA(bool *collision_map, bool *projectile_hit_map, list<Character *> &enemies, Character *p1, Character *p2);
};

#endif // MONSTER2_HPP
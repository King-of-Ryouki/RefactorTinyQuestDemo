#ifndef PLAYER2_H
#define PLAYER2_H

#include "..\projectile\projectile.hpp" // class Player2 ==> fire_ball
#include "player.hpp"

class Player2 : public Player {
public:
    Player2(const int &init_hp,
            const int &init_mp,
            const int &skill_cost)
        : Player(init_hp,
                 init_mp,
                 skill_cost,

                 1,
                 1,

                 kTileSize,
                 static_cast<int>(kTileSize * 1.5),

                 4,
                 4,
                 2,

                 12) {
    }

    virtual ~Player2() {}

    void Draw(const int &tex_id, const int &tex_w, const int &tex_h, const bool &run);

    bool Attack(list<Character *> &enemies, list<Projectile *> &list_Projectile, bool *projectile_hit_map);
};

#endif // PLAYER2_H
#ifndef PLAYER1_H
#define PLAYER1_H

#include "player.hpp"

class Player1 : public Player {
public:
    Player1(const int &init_hp,
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

                 7) {
        set_damage(10);
        set_logic_status(LOOK_DOWN);
    }

    virtual ~Player1() {}

    void set_logic_status(int logic_status);
    int get_logic_state();

    void Draw(const int &tex_id, const int &tex_w, const int &tex_h, const bool &run);

    bool Attack(Character *enemy);

    void StopP1();
    void StopDoingSkillP1();

    void set_shield_state(int render_state);
    void TestShieldProtection(int attack_state, int damage, int x, int y);

private:
    int logic_status_;
};

#endif // PLAYER1_H
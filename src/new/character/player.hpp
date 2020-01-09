#ifndef PLAYER_H
#define PLAYER_H

#include "character.hpp"

const int CHARGE_BREAK = 30;
const int COOLDOWN     = 6; // frames of minimum wait between attack and attack

const int SUPERATTACK_COST = 60;
const int ATTACK_COST      = 10;

class Player : public Character {
public:
    // class Player
    Player(const int &init_hp,
           const int &init_mp,
           const int &skill_cost,

           const int &base_tiles_w,
           const int &base_tiles_h,

           const int &hit_w,
           const int &hit_h,

           const int &frame_texture_tile,
           const int &frame_delay_walk,
           const int &frame_delay_attack,

           const int &super_attack_frame_offset)

        : Character(init_hp,
                    init_mp,
                    skill_cost,

                    base_tiles_w,
                    base_tiles_h,

                    hit_w,
                    hit_h,

                    frame_texture_tile,
                    frame_delay_walk,
                    frame_delay_attack),

          atk_cd_(0),
          step_length_(STEP_LENGTH),

          super_attack_frame_offset_(super_attack_frame_offset) {}

    virtual ~Player() {}

    void NextFrame(const int &max); // max is the total frames of the animation

    bool CollidesWithEnemy(list<Character *> &enemies, const int &next_tile);
    bool CollidesUD(bool *collision_map, list<Character *> &enemies, bool up);
    bool CollidesLR(bool *collision_map, list<Character *> &enemies, bool right, bool vertical);

    void RestrictedMoveUp(bool *collision_map, list<Character *> &enemies, const int &h1, const int &h2, const int &game_height);
    void RestrictedMoveDown(bool *collision_map, list<Character *> &enemies, const int &h1, const int &h2, const int &game_height, const int &bottom_limit);

    void MoveDown(bool *collision_map, list<Character *> &enemies);
    void MoveUp(bool *collision_map, list<Character *> &enemies);

    void MoveRight(bool *collision_map, list<Character *> &enemies, bool vertical);
    void MoveLeft(bool *collision_map, list<Character *> &enemies, bool vertical);

    void set_frame_attack_delay(const int &frame_attack_delay);
    int get_frame_attack_delay();

    void set_atk_cd(const int &atk_cd);
    int get_atk_cd();

    void StartSuperAttack();
    bool IsAttacking();
    bool IsSuperAttacking();
    bool IsDoingSkill();
    bool IsWalking();

    void TransformIntoSkillState();

    void Stop();
    void StopDoingSkill();

private:
    int atk_cd_;
    int step_length_; // kTileSize ha de ser multiplo de step_length_

    int super_attack_frame_offset_;

protected:
    void TextureOffsetOfPlayerState(const int &tex_id, const int &tex_w, const int &tex_h, const bool &run);
};

#endif // PLAYER_H
#ifndef ENEMY
#define ENEMY

#include "character.hpp"

class Enemy : public Character {
public:
    Enemy(const int &init_hp,
          const int &init_mp,
          const int &skill_cost,

          const int &base_tiles_w,
          const int &base_tiles_h,

          const int &hit_w,
          const int &hit_h,

          const int &frame_texture_tile,
          const int &frame_delay_walk,
          const int &frame_delay_attack,

          const int &frame_delay_dying)
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

          frame_delay_dying_(frame_delay_dying),
          step_length_(STEP_LENGTH) {}

    virtual ~Enemy() {}

    void Draw(const int &tex_id, const int &tex_w, const int &tex_h, const bool &run);

    void NextFrame(const int &max);
    void Move();

private:
    int frame_delay_dying_;
    int step_length_; // 圖塊大小為步數的倍數 // Tile Size must be multiple of step length
};

#endif // ENEMY
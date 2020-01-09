#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "..\character\player1.hpp"
#include "..\scene.hpp"

enum ENUM_PROJECTILE {

    PROJECTILE_FLYING_DOWN = 0,
    PROJECTILE_FLYING_UP,
    PROJECTILE_FLYING_RIGHT,
    PROJECTILE_FLYING_LEFT,

    PROJECTILE_HIT_DOWN,
    PROJECTILE_HIT_UP,
    PROJECTILE_HIT_RIGHT,
    PROJECTILE_HIT_LEFT,

};

class Projectile {
public:
    Projectile(const int &projectile_hit_h,
               const int &projectile_hit_w,
               const int &frame_texture_tile,
               const int &proj_damage)
        : projectile_hit_h_(projectile_hit_h),
          projectile_hit_w_(projectile_hit_w),
          frame_texture_tile_(frame_texture_tile),
          projectile_damage_(proj_damage) {
        seq         = 0,
        delay       = 0,
        frame_delay = 3;

        projectile_step_length_ = 4 * STEP_LENGTH;

        replace_over_  = false;
        replace_under_ = false;
    }

    virtual ~Projectile() {}

    virtual void OffsetProjectileTexture(const int &tex_id, const int &tex_w, const int &tex_h, const bool &run) = 0;

    void set_projectile_pos(const int &posx, const int &posy);

    int get_character_x();
    int get_character_y();

    int get_projectile_hit_w();
    int get_projectile_hit_h();

    bool HitMapAtUpOrDown(bool *projectile_hit_map, const bool &up);
    bool CheckHitMapAtLeftOrRight(bool *projectile_hit_map, const bool &right);

    void LevitateUp(bool *projectile_hit_map);
    void LevitateDown(bool *projectile_hit_map);
    void LevitateLeft(bool *projectile_hit_map);
    void LevitateRight(bool *projectile_hit_map);

    bool ComputeEnemyCollisions(list<Character *> &enemies);
    bool Logic(bool *projectile_hit_map, list<Character *> &enemies);

    void set_projectile_status(const int &projectile_status);
    int get_projectile_status();

    int get_frame_seq();
    int get_frame_delay();
    int get_frame_texture_tile();

    int get_projectile_damage();
    int get_projectile_step_length();

    void set_replace_over(const bool &replace_over); // displace == 替换
    void set_replace_under(const bool &replace_under);

    void NextFrame(const int &max);
    bool IsEnding();

    bool Intersection(Rect box1, Rect box2);

    Rect getFrameBox();
    Rect getHitBox();

protected:
    void DrawProjectile(const int &tex_id, const float &begin_tex_x, const float &begin_tex_y, const float &final_tex_x, const float &final_tex_y);

    int projectile_damage_;

    // 敵人的可被擊中範圍, 不用被 kTileSize 整除, 但 hitbox 必須在 frame 的 (center/center) // pixel dimensions of the hittable part of a sprite frame, it is not necessary to be divisible by kTileSize, but in the associated sprites the hitbox must be in the center / center of each frame
    int projectile_hit_h_;
    int projectile_hit_w_;

    // the lower left pixel of the projectile hitbox

    int projectile_x_;
    int projectile_y_;

    int replace_over_;
    int replace_under_;

    int seq,
        delay,
        projectile_status_,
        frame_delay,
        frame_texture_tile_,
        projectile_step_length_;
};

class Arrow : public Projectile {
public:
    Arrow() : Projectile(kTileSize / 4, kTileSize / 4, 2, 10) {}

    void OffsetProjectileTexture(const int &tex_id, const int &tex_w, const int &tex_h, const bool &run);
};

class FireBall : public Projectile {
public:
    FireBall() : Projectile(kTileSize, kTileSize, 2, 10) {}

    void OffsetProjectileTexture(const int &tex_id, const int &tex_w, const int &tex_h, const bool &run);
};

#endif // PROJECTILE_H
#ifndef CHARACTER_HPP // base of 敵我雙方 // activity object
#define CHARACTER_HPP

#include "..\scene.hpp"

const int STEP_LENGTH = (kTileSize / 8);

enum ENUM_CHARACTER_STATUS {

    LOOK_DOWN = 0,
    LOOK_UP,
    LOOK_RIGHT,
    LOOK_LEFT,

    WALK_DOWN,
    WALK_UP,
    WALK_RIGHT,
    WALK_LEFT,

    DAMAGE_DOWN,
    DAMAGE_UP,
    DAMAGE_RIGHT,
    DAMAGE_LEFT,

    ATTACK_DOWN,
    ATTACK_UP,
    ATTACK_RIGHT,
    ATTACK_LEFT,

    SKILL_DOWN,
    SKILL_UP,
    SKILL_RIGHT,
    SKILL_LEFT,

    SKILL_WALK_DOWN,
    SKILL_WALK_UP,
    SKILL_WALK_RIGHT,
    SKILL_WALK_LEFT,

    DYING,
    SUPER_ATTACK,
};

class Character {
public:
    Character(const int &init_hp,
              const int &init_mp,
              const int &skill_cost,

              const int &base_tiles_w,
              const int &base_tiles_h,

              const int &hit_w,
              const int &hit_h,

              const int &frame_texture_tile,
              const int &frame_delay_walk,
              const int &frame_delay_attack)
        : // 能力值
          max_hp_(init_hp),
          now_hp_(init_hp),
          max_mp_(init_mp),
          now_mp_(init_mp),
          skill_cost_(skill_cost),

          // 其他
          base_tile_w_(base_tiles_w),
          base_tile_h_(base_tiles_h),

          hit_box_w_(hit_w),
          hit_box_h_(hit_h),

          texture_tile_frame_(frame_texture_tile),
          frame_delay_walk_(frame_delay_walk),
          frame_delay_attack_(frame_delay_attack),

          character_status_(LOOK_DOWN),
          frame_seq_(0),
          frame_delay_(0) {}

    virtual ~Character() {}

    virtual void Draw(const int &tex_id, const int &tex_w, const int &tex_h, const bool &run) = 0;

    void set_character_x(const int &character_x);
    int get_character_x();

    void set_character_y(const int &character_y);
    int get_character_y();

    void set_tile(const int &character_x, const int &character_y);

    int get_base_tile_w();
    int get_base_tile_h();

    void set_character_status(const int &character_status);
    int get_character_status();

    void set_frame_delay(const int &frame_delay);
    int get_frame_delay();

    // * hp and mp

    int get_max_hp();

    int get_max_mp();

    void set_now_hp(const int &now_hp);
    int get_now_hp();

    void set_now_mp(const int &now_mp);
    int get_now_mp();

    //

    void set_skill_cost(const int &skill_cost);
    int get_skill_cost();

    void set_frame_seq(const int &frame);
    int get_frame_seq();

    void set_damage(const int &damage);
    int get_damage();

    //

    int get_frame_top();
    int get_frame_bottom();
    int get_frame_texture_tile();

    //

    bool Intersection(Rect box1, Rect box2); // bool Intersection(Rect box1, int px, int py) { if (px >= box1.left && px < box1.right && py >= box1.bottom && py < box1.top) { return true; } return false; }

    void StartAttack();

    void Stop();

    //

    bool IsLooking();
    bool IsWalking();
    bool IsAttacking();
    bool IsDamaged();

    void DrawCharacter(

        const int &tex_id,

        const float &begin_tex_x,
        const float &begin_tex_y,

        const float &final_tex_x,
        const float &final_tex_y

    );

    vector<int> getTiles();
    // 返回 Character base 相交的位置
    // return the positions of the tiles with which the base of the Character intersects
    Rect getHitBox();

protected:
    int frame_delay_walk_;
    int frame_delay_attack_;

private:
    // bug's 左下角像素的攻擊框 // the left lower pixel of the bug's hitbox
    int character_x_;
    int character_y_;

    // 精靈幀的可擊中部分的像素尺寸，沒有必要被kTileSize整除，但在相關的精靈中，命中框必須位於每個幀的中心/中心 // pixel dimensions of the hittable part of a sprite frame, it is not necessary to be divisible by kTileSize, but in the associated sprites the hitbox must be in the center / center of each frame
    int hit_box_w_;
    int hit_box_h_;

    // commissionable base 的圖塊尺寸 // dimensions in tiles of the commissionable base
    int base_tile_w_;
    int base_tile_h_;

    // 材質圖中被小妖佔據的 frame 一側的數字 // the number in texture tiles that occupies the side of a frame of the sprite
    int texture_tile_frame_;

    int skill_cost_;
    int damage_;
    int character_status_;

    int frame_seq_;   // seq 是現在必須繪製的動畫序列的 frame, delay 是等待其中一個 frames to another // seq is the frame of the animation sequence that has to be painted now.
    int frame_delay_; //  delay is the wait of one of those frames to another

    int max_hp_;
    int now_hp_;

    int max_mp_;
    int now_mp_;
};

#endif // CHARACTER_HPP

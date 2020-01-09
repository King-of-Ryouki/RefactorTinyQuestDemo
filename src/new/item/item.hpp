#ifndef ITEM
#define ITEM

#include "..\character\player.hpp"

class BaseOfItem {
public:
    BaseOfItem();
    virtual ~BaseOfItem() {}

    void set_tile(int tx, int ty);

    int get_texture_frame();

    void DrawItem(int tex_id, int tex_w, int tex_h, bool run);

    Rect getHitBox();

private:
    int tex_x_, tex_y_, tex_z_;
    int texture_frame_, frame_delay_, frame_tmp_;

    void NextFrame(int max_texture_frame);
};

class ItemOfHeal : public BaseOfItem {
public:
    bool Heal(Player &player);
};

#endif // ITEM
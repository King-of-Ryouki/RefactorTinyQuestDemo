#include "item.hpp"

BaseOfItem::BaseOfItem() : frame_tmp_(0),
                           frame_delay_(4),
                           texture_frame_(0),
                           tex_z_(kTileSize / 4) {
}

void BaseOfItem::set_tile(int tex_x, int tex_y) {

    tex_x_ = tex_x * kTileSize;
    tex_y_ = tex_y * kTileSize;
}

int BaseOfItem::get_texture_frame() { return texture_frame_; }

void BaseOfItem::DrawItem(int tex_id, int tex_w, int tex_h, bool run) {

    float tex_offset_x = 1.0f / float(tex_w / kTextureTileSize);
    float tex_offset_y = 1.0f / float(tex_h / kTextureTileSize);

    float begin_tex_x = tex_offset_x * get_texture_frame();
    float begin_tex_y = tex_offset_y * 1;

    float final_tex_x = tex_offset_x + begin_tex_x;
    float final_tex_y = tex_offset_y - begin_tex_y;

    if (run) { BaseOfItem::NextFrame(4); } // total frames of the "heart.png" animation

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glBegin(GL_POLYGON);

    glTexCoord2f(begin_tex_x, begin_tex_y);
    glVertex3i(tex_x_,
               tex_y_,
               tex_y_ - tex_z_);

    glTexCoord2f(final_tex_x, begin_tex_y);
    glVertex3i(tex_x_ + kTileSize,
               tex_y_,
               tex_y_ - tex_z_);

    glTexCoord2f(final_tex_x, final_tex_y);
    glVertex3i(tex_x_ + kTileSize,
               tex_y_ + kTileSize,
               tex_y_ - tex_z_);

    glTexCoord2f(begin_tex_x, final_tex_y);
    glVertex3i(tex_x_,
               tex_y_ + kTileSize,
               tex_y_ - tex_z_);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}

Rect BaseOfItem::getHitBox() {

    Rect HitBox;

    HitBox.right  = tex_x_ + kTileSize;
    HitBox.left   = tex_x_;
    HitBox.top    = tex_y_ + kTileSize;
    HitBox.bottom = tex_y_;

    return HitBox;
}

void BaseOfItem::NextFrame(int max_texture_frame) {

    ++frame_tmp_;

    if (frame_tmp_ == frame_delay_) {
        ++texture_frame_;
        texture_frame_ %= max_texture_frame;
        frame_tmp_ = 0;
    }
}

bool ItemOfHeal::Heal(Player &PlayerHp) {

    if (PlayerHp.get_now_hp() == PlayerHp.get_max_hp()) { return false; }

    int now_hp = PlayerHp.get_now_hp() + 40;

    (now_hp > PlayerHp.get_max_hp())
        ? PlayerHp.set_now_hp(PlayerHp.get_max_hp())
        : PlayerHp.set_now_hp(now_hp);

    return true;
}
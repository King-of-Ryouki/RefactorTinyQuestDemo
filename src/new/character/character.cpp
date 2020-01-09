#include "character.hpp"

void Character::set_character_x(const int &character_x) { character_x_ = character_x; }
int Character::get_character_x() { return character_x_; }

void Character::set_character_y(const int &character_y) { character_y_ = character_y; }
int Character::get_character_y() { return character_y_; }

void Character::set_tile(const int &character_x, const int &character_y) {

    character_x_ = character_x * kTileSize;
    character_y_ = character_y * kTileSize;
}

int Character::get_base_tile_w() { return base_tile_w_ * kTileSize; }
int Character::get_base_tile_h() { return base_tile_h_ * kTileSize; }

void Character::set_character_status(const int &character_status) { character_status_ = character_status; }
int Character::get_character_status() { return character_status_; }

void Character::set_frame_delay(const int &frame_delay) { frame_delay_ = frame_delay; }
int Character::get_frame_delay() { return frame_delay_; }

// * hp and mp

int Character::get_max_hp() { return max_hp_; }
int Character::get_max_mp() { return max_mp_; }

void Character::set_now_hp(const int &now_hp) { now_hp_ = now_hp; }
int Character::get_now_hp() { return now_hp_; }

void Character::set_now_mp(const int &now_mp) { now_mp_ = now_mp; }
int Character::get_now_mp() { return now_mp_; }

//

void Character::set_skill_cost(const int &skill_cost) { skill_cost_ = skill_cost; }
int Character::get_skill_cost() { return skill_cost_; }

void Character::set_frame_seq(const int &frame) { frame_seq_ = frame; }
int Character::get_frame_seq() { return frame_seq_; }

void Character::set_damage(const int &damage) { damage_ = damage; }
int Character::get_damage() { return damage_; }

//

int Character::get_frame_top() { return character_y_ + (texture_tile_frame_ * kTileSize) / 2; }
int Character::get_frame_bottom() { return character_y_ - (texture_tile_frame_ * kTileSize - hit_box_h_) / 2; }
int Character::get_frame_texture_tile() { return texture_tile_frame_; }

//

bool Character::Intersection(Rect box1, Rect box2) {

    if (box1.top > box2.bottom &&
        box2.top > box1.bottom &&
        box1.left < box2.right &&
        box2.left < box1.right) {
        return true;
    }

    return false;
}

void Character::StartAttack() {

    switch (character_status_) {
        case LOOK_DOWN: character_status_ = ATTACK_DOWN; break;
        case LOOK_UP: character_status_ = ATTACK_UP; break;
        case LOOK_RIGHT: character_status_ = ATTACK_RIGHT; break;
        case LOOK_LEFT: character_status_ = ATTACK_LEFT; break;

        case WALK_DOWN: character_status_ = ATTACK_DOWN; break;
        case WALK_UP: character_status_ = ATTACK_UP; break;
        case WALK_RIGHT: character_status_ = ATTACK_RIGHT; break;
        case WALK_LEFT: character_status_ = ATTACK_LEFT; break;
    }

    frame_seq_   = 0;
    frame_delay_ = 0;
}

void Character::Stop() {

    switch (character_status_) {
        case WALK_DOWN: character_status_ = LOOK_DOWN; break;
        case WALK_UP: character_status_ = LOOK_UP; break;
        case WALK_RIGHT: character_status_ = LOOK_RIGHT; break;
        case WALK_LEFT: character_status_ = LOOK_LEFT; break;

        case ATTACK_DOWN: character_status_ = LOOK_DOWN; break;
        case ATTACK_UP: character_status_ = LOOK_UP; break;
        case ATTACK_RIGHT: character_status_ = LOOK_RIGHT; break;
        case ATTACK_LEFT: character_status_ = LOOK_LEFT; break;

        case DAMAGE_DOWN: character_status_ = LOOK_DOWN; break;
        case DAMAGE_UP: character_status_ = LOOK_UP; break;
        case DAMAGE_RIGHT: character_status_ = LOOK_RIGHT; break;
        case DAMAGE_LEFT: character_status_ = LOOK_LEFT; break;
    }
}

//

bool Character::IsLooking() {

    if (character_status_ == LOOK_DOWN ||
        character_status_ == LOOK_UP ||
        character_status_ == LOOK_RIGHT ||
        character_status_ == LOOK_LEFT) {
        return true;
    }

    return false;
}
bool Character::IsWalking() {

    if (character_status_ == WALK_DOWN ||
        character_status_ == WALK_UP ||
        character_status_ == WALK_RIGHT ||
        character_status_ == WALK_LEFT) {
        return true;
    }

    return false;
}
bool Character::IsAttacking() {

    if (character_status_ == ATTACK_DOWN ||
        character_status_ == ATTACK_UP ||
        character_status_ == ATTACK_RIGHT ||
        character_status_ == ATTACK_LEFT) {
        return true;
    }

    return false;
}
bool Character::IsDamaged() {

    if (character_status_ == DAMAGE_DOWN ||
        character_status_ == DAMAGE_UP ||
        character_status_ == DAMAGE_RIGHT ||
        character_status_ == DAMAGE_LEFT) {
        return true;
    }

    return false;
}

void Character::DrawCharacter(const int &tex_id,
                              const float &begin_tex_x,
                              const float &begin_tex_y,
                              const float &final_tex_x,
                              const float &final_tex_y) {

    Rect hitbox = getHitBox();

    int screen_x = Character::get_character_x();
    int screen_y = Character::get_character_y();

    int hitbox_w = hitbox.right - hitbox.left;
    int hitbox_h = hitbox.top - hitbox.bottom;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glBegin(GL_POLYGON);

    glTexCoord2f(begin_tex_x, begin_tex_y);
    glVertex3i(screen_x - (kTileSize * texture_tile_frame_ - hitbox_w) / 2,
               screen_y - (kTileSize * texture_tile_frame_ - hitbox_h) / 2,
               screen_y);

    glTexCoord2f(final_tex_x, begin_tex_y);
    glVertex3i(screen_x + (kTileSize * texture_tile_frame_ + hitbox_w) / 2,
               screen_y - (kTileSize * texture_tile_frame_ - hitbox_h) / 2,
               screen_y);

    glTexCoord2f(final_tex_x, final_tex_y);
    glVertex3i(screen_x + (kTileSize * texture_tile_frame_ + hitbox_w) / 2,
               screen_y + (kTileSize * texture_tile_frame_ + hitbox_h) / 2,
               screen_y);

    glTexCoord2f(begin_tex_x, final_tex_y);
    glVertex3i(screen_x - (kTileSize * texture_tile_frame_ - hitbox_w) / 2,
               screen_y + (kTileSize * texture_tile_frame_ + hitbox_h) / 2,
               screen_y);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}

vector<int> Character::getTiles() {

    // 根據 base 的尺寸// according to the dimensions of the base
    // bug 佔據的位置數量 // are the number of tiles of land that occupies the base of the bug
    int occupied_tiles_w = base_tile_w_;
    int occupied_tiles_h = base_tile_h_;

    if ((character_x_ % kTileSize) != 0) { ++occupied_tiles_w; }
    if ((character_y_ % kTileSize) != 0) { ++occupied_tiles_h; }

    // bug 所在 tiles 的坐標 // are the coords in tiles where the bug is
    int tile_x = character_x_ / kTileSize;
    int tile_y = character_y_ / kTileSize;

    vector<int> tiles;

    for (int i = 0; i < occupied_tiles_w; ++i) {
        for (int j = 0; j < occupied_tiles_h; ++j) {
            tiles.push_back((tile_x + i) + ((tile_y + j) * kScene_W));
        }
    }

    return tiles;
}

Rect Character::getHitBox() {

    Rect hitbox;

    hitbox.top    = character_y_ + hit_box_h_;
    hitbox.bottom = character_y_;
    hitbox.left   = character_x_;
    hitbox.right  = character_x_ + hit_box_w_;

    return hitbox;
}
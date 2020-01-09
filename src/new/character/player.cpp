#include "player.hpp"

void Player::NextFrame(const int &max) {

    Character::set_frame_delay(Character::get_frame_delay() + 1);

    if (Character::get_character_status() == ATTACK_DOWN ||
        Character::get_character_status() == ATTACK_UP ||
        Character::get_character_status() == ATTACK_RIGHT ||
        Character::get_character_status() == ATTACK_LEFT ||
        Character::get_character_status() == SUPER_ATTACK) {
        if (Character::get_frame_delay() == frame_delay_attack_) {

            Character::set_frame_seq(get_frame_seq() + 1);
            Character::set_frame_seq(get_frame_seq() % max);
            Character::set_frame_delay(0);
        }

    } else {

        if (Character::get_frame_delay() == frame_delay_walk_) {

            Character::set_frame_seq(get_frame_seq() + 1);
            Character::set_frame_seq(get_frame_seq() % max);
            Character::set_frame_delay(0);
        }
    }
}

bool Player::CollidesWithEnemy(list<Character *> &enemies, const int &next_tile) {

    list<Character *>::iterator iter_Character;

    for (iter_Character = enemies.begin(); iter_Character != enemies.end(); ++iter_Character) {
        if ((*iter_Character)->Character::get_character_status() != DYING) {
            vector<int> vec_tiles = (*iter_Character)->Character::getTiles();

            for (short int i = 0; i < vec_tiles.size(); ++i) {
                if (vec_tiles[ i ] == next_tile) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Player::CollidesUD(bool *collision_map, list<Character *> &enemies, bool up) {

    int tile_x = Character::get_character_x() / kTileSize;
    int tile_y; // the coords in tiles where is the bug after taking the step

    // kScene_H 檢查是為了避免使用碰撞矩陣之外的位置進行計算 // the kScene_H check is to avoid making calculations with positions outside the collision matrix
    (up && ((Character::get_character_y() + Character::get_base_tile_h()) / kTileSize) < kScene_H)
        ? tile_y = (Character::get_character_y() + Character::get_base_tile_h()) / kTileSize
        : tile_y = (Character::get_character_y()) / kTileSize;

    int width_tiles = Character::get_base_tile_w() / kTileSize; // 碰撞的水平圖塊數量 // number of horizontal tiles with which collides

    if ((Character::get_character_x() % kTileSize) != 0) {
        ++width_tiles;
    }

    // 如果碰撞敵人 not slide // I evaluate collisions against enemies, they do not produce slide
    for (int j = 0; j < width_tiles; ++j) {
        if (Player::CollidesWithEnemy(enemies, (tile_x + j) + (tile_y * kScene_W))) {
            return true;
        }
    }

    // 如果碰撞場景 slide // I evaluate collisions against the scenario, they produce slide
    for (int j = 1; j < (width_tiles - 1); ++j) {
        if (collision_map[ (tile_x + j) + (tile_y * kScene_W) ]) {
            return true;
        }
    }

    // if all tiles consulted except the last one are transferable and I have the potential to turn left, I do slideL
    if (!collision_map[ (tile_x) + (tile_y * kScene_W) ]) {
        if (collision_map[ (tile_x + (width_tiles - 1)) + (tile_y * kScene_W) ]) {
            if (Character::get_character_status() != WALK_RIGHT &&
                Character::get_character_status() != WALK_LEFT &&
                Character::get_character_status() != SKILL_WALK_RIGHT &&
                Character::get_character_status() != SKILL_WALK_LEFT) {

                Character::set_character_x(Character::get_character_x() - step_length_);
            }

            return true;
        }

        return false;

    } else { // if all tiles consulted except the first one are transferable and I have the potential to turn to the right, I do slideR

        if (!collision_map[ (tile_x + (width_tiles - 1)) + (tile_y * kScene_W) ]) {
            if (Character::get_character_status() != WALK_RIGHT &&
                Character::get_character_status() != WALK_LEFT &&
                Character::get_character_status() != SKILL_WALK_RIGHT && /*&& (x % kTileSize) >= kTileSize/2*/
                Character::get_character_status() != SKILL_WALK_LEFT) {

                Character::set_character_x(Character::get_character_x() + step_length_);
            }
        }

        return true;
    }
}

bool Player::CollidesLR(bool *collision_map, list<Character *> &enemies, bool right, bool vertical) {

    int tile_x;
    int tile_y = Character::get_character_y() / kTileSize; // the coords in tiles where is the bug after taking the step

    (right && ((Character::get_character_x() + Character::get_base_tile_w()) / kTileSize) < kScene_W)
        ? tile_x = (Character::get_character_x() + Character::get_base_tile_w()) / kTileSize
        : tile_x = Character::get_character_x() / kTileSize;

    int height_tiles = Character::get_base_tile_h() / kTileSize; // number of tiles in vertical with which collides

    if ((Character::get_character_y() % kTileSize) != 0) {
        ++height_tiles;
    }

    // 如果碰撞敵人 not slide // I evaluate collisions against enemies, they do not produce slide
    for (int j = 0; j < height_tiles; ++j) {
        if (Player::CollidesWithEnemy(enemies, (tile_x) + ((tile_y + j) * kScene_W))) {
            return true;
        }
    }

    // 如果碰撞場景 slide // I evaluate collisions against the scenario, they produce slide
    for (int j = 1; j < (height_tiles - 1); ++j) {
        return (collision_map[ (tile_x) + ((tile_y + j) * kScene_W) ]);
    }

    // if all the tiles consulted minus the highest are transferable and I have the potential to go down, I do slideD
    if (!collision_map[ (tile_x) + (tile_y * kScene_W) ]) {
        if (collision_map[ (tile_x) + ((tile_y + (height_tiles - 1)) * kScene_W) ]) {
            if (!vertical /*&& (y % kTileSize) < kTileSize/2*/) {
                Character::set_character_y(Character::get_character_y() - step_length_);
            }

            return true;
        }

        return false;

    } else { // if all the tiles consulted except the lowest one are transferable and I have the potential to go up, I do slideU

        if (!collision_map[ (tile_x) + ((tile_y + (height_tiles - 1)) * kScene_W) ]) {
            if (!vertical) {
                Character::set_character_y(Character::get_character_y() + step_length_);
            }
        }

        return true;
    }
}

void Player::RestrictedMoveUp(bool *collision_map, list<Character *> &enemies, const int &h1, const int &h2, const int &game_height) {

    if (h1 <= h2 || h1 - h2 <= game_height - kTileSize) {

        Player::MoveUp(collision_map, enemies);

    } else {

        if (Character::get_character_status() != WALK_UP &&
            Character::get_character_status() != WALK_RIGHT &&
            Character::get_character_status() != WALK_LEFT &&
            Character::get_character_status() != SKILL_WALK_UP &&
            Character::get_character_status() != SKILL_WALK_RIGHT &&
            Character::get_character_status() != SKILL_WALK_LEFT) {

            (Player::IsDoingSkill()) ? Character::set_character_status(SKILL_WALK_UP)
                                     : Character::set_character_status(WALK_UP);

            Character::set_frame_seq(0);
            Character::set_frame_delay(0);
        }
    }
}

void Player::RestrictedMoveDown(bool *collision_map, list<Character *> &enemies, const int &h1, const int &h2, const int &game_height, const int &bottom_limit) {

    if (h1 > bottom_limit && (h1 > h2 || h2 - h1 <= game_height - kTileSize)) {
        Player::MoveDown(collision_map, enemies);
    } else {
        if (Character::get_character_status() != WALK_DOWN &&
            Character::get_character_status() != WALK_RIGHT &&
            Character::get_character_status() != WALK_LEFT &&
            Character::get_character_status() != SKILL_WALK_DOWN &&
            Character::get_character_status() != SKILL_WALK_RIGHT &&
            Character::get_character_status() != SKILL_WALK_LEFT) {

            (Player::IsDoingSkill()) ? Character::set_character_status(SKILL_WALK_DOWN)
                                     : Character::set_character_status(WALK_DOWN);

            Character::set_frame_seq(0);
            Character::set_frame_delay(0);
        }
    }
}

void Player::MoveDown(bool *collision_map, list<Character *> &enemies) {
    if (Character::get_character_status() != WALK_DOWN &&
        Character::get_character_status() != WALK_RIGHT &&
        Character::get_character_status() != WALK_LEFT &&
        Character::get_character_status() != SKILL_WALK_DOWN &&
        Character::get_character_status() != SKILL_WALK_RIGHT &&
        Character::get_character_status() != SKILL_WALK_LEFT) {

        (Player::IsDoingSkill()) ? Character::set_character_status(SKILL_WALK_DOWN)
                                 : Character::set_character_status(WALK_DOWN);

        Character::set_frame_seq(0);
        Character::set_frame_delay(0);
    }

    // whats next tile?
    if ((Character::get_character_y() % kTileSize) == 0) {

        int player_pos_y = Character::get_character_y();

        Character::set_character_y(Character::get_character_y() - step_length_);

        if (CollidesUD(collision_map, enemies, false)) {
            Character::set_character_y(player_pos_y);
        }

    } else {

        Character::set_character_y(Character::get_character_y() - step_length_);
    }
}

void Player::MoveUp(bool *collision_map, list<Character *> &enemies) {

    if (Character::get_character_status() != WALK_UP &&
        Character::get_character_status() != WALK_RIGHT &&
        Character::get_character_status() != WALK_LEFT &&
        Character::get_character_status() != SKILL_WALK_UP &&
        Character::get_character_status() != SKILL_WALK_RIGHT &&
        Character::get_character_status() != SKILL_WALK_LEFT) {

        (Player::IsDoingSkill()) ? Character::set_character_status(SKILL_WALK_UP)
                                 : Character::set_character_status(WALK_UP);

        Character::set_frame_seq(0);
        Character::set_frame_delay(0);
    }

    // whats next tile?
    if ((Character::get_character_y() % kTileSize) == 0) {

        int player_pos_y = Character::get_character_y();

        Character::set_character_y(Character::get_character_y() + step_length_);

        if (CollidesUD(collision_map, enemies, true)) {
            Character::set_character_y(player_pos_y);
        }

    } else {

        Character::set_character_y(Character::get_character_y() + step_length_);
    }
}

void Player::MoveRight(bool *collision_map, list<Character *> &enemies, bool vertical) {

    if (Character::get_character_status() != WALK_RIGHT &&
        Character::get_character_status() != SKILL_WALK_RIGHT) {

        (Player::IsDoingSkill())
            ? Character::set_character_status(SKILL_WALK_RIGHT)
            : Character::set_character_status(WALK_RIGHT);

        Character::set_frame_seq(0);
        Character::set_frame_delay(0);
    }

    // whats next tile?
    if ((Character::get_character_x() % kTileSize) == 0) {

        int tmp_x = Character::get_character_x();

        Character::set_character_x(Character::get_character_x() + step_length_);

        if (Player::CollidesLR(collision_map, enemies, true, vertical)) {
            Character::set_character_x(tmp_x);
        }

    } else {

        Character::set_character_x(Character::get_character_x() + step_length_);
    }
}

void Player::MoveLeft(bool *collision_map, list<Character *> &enemies, bool vertical) {
    if (Character::get_character_status() != WALK_LEFT &&
        Character::get_character_status() != SKILL_WALK_LEFT) {

        (Player::IsDoingSkill())
            ? Character::set_character_status(SKILL_WALK_LEFT)
            : Character::set_character_status(WALK_LEFT);

        Character::set_frame_seq(0);
        Character::set_frame_delay(0);
    }

    // whats next tile?
    if ((Character::get_character_x() % kTileSize) == 0) { // there will be a tile change if I move

        int tmp_x = Character::get_character_x();

        Character::set_character_x(Character::get_character_x() - step_length_);

        if (Player::CollidesLR(collision_map, enemies, false, vertical)) {
            Character::set_character_x(tmp_x);
        }

    } else {

        Character::set_character_x(Character::get_character_x() - step_length_);
    }
}

void Player::set_frame_attack_delay(const int &frame_attack_delay) { frame_delay_attack_ = frame_attack_delay; }
int Player::get_frame_attack_delay() { return frame_delay_attack_; }

void Player::set_atk_cd(const int &atk_cd) { atk_cd_ = atk_cd; }
int Player::get_atk_cd() { return atk_cd_; }

void Player::StartSuperAttack() {

    Character::set_character_status(SUPER_ATTACK);
    Character::set_frame_seq(0);
    Character::set_frame_delay(0);
}

bool Player::IsAttacking() {

    if (Character::get_character_status() == ATTACK_DOWN ||
        Character::get_character_status() == ATTACK_UP ||
        Character::get_character_status() == ATTACK_RIGHT ||
        Character::get_character_status() == ATTACK_LEFT ||
        Character::get_character_status() == SUPER_ATTACK) {

        return true;
    }

    return false;
}

bool Player::IsSuperAttacking() {

    if (Character::get_character_status() == SUPER_ATTACK) {
        return true;
    }

    return false;
}
bool Player::IsDoingSkill() {

    if (Character::get_character_status() == SKILL_DOWN ||
        Character::get_character_status() == SKILL_UP ||
        Character::get_character_status() == SKILL_RIGHT ||
        Character::get_character_status() == SKILL_LEFT ||
        Character::get_character_status() == SKILL_WALK_DOWN ||
        Character::get_character_status() == SKILL_WALK_UP ||
        Character::get_character_status() == SKILL_WALK_RIGHT ||
        Character::get_character_status() == SKILL_WALK_LEFT) {

        return true;
    }

    return false;
}

bool Player::IsWalking() {

    if (Character::get_character_status() == WALK_DOWN ||
        Character::get_character_status() == WALK_UP ||
        Character::get_character_status() == WALK_RIGHT ||
        Character::get_character_status() == WALK_LEFT ||
        Character::get_character_status() == SKILL_WALK_DOWN ||
        Character::get_character_status() == SKILL_WALK_UP ||
        Character::get_character_status() == SKILL_WALK_RIGHT ||
        Character::get_character_status() == SKILL_WALK_LEFT) {

        return true;
    }

    return false;
}

void Player::TransformIntoSkillState() {

    switch (Character::get_character_status()) {

        case LOOK_DOWN: Character::set_character_status(SKILL_DOWN); break;
        case LOOK_UP: Character::set_character_status(SKILL_UP); break;
        case LOOK_RIGHT: Character::set_character_status(SKILL_RIGHT); break;
        case LOOK_LEFT: Character::set_character_status(SKILL_LEFT); break;

        case WALK_DOWN: Character::set_character_status(SKILL_WALK_DOWN); break;
        case WALK_UP: Character::set_character_status(SKILL_WALK_UP); break;
        case WALK_RIGHT: Character::set_character_status(SKILL_WALK_RIGHT); break;
        case WALK_LEFT: Character::set_character_status(SKILL_WALK_LEFT); break;
    }

    Character::set_frame_seq(0);
    Character::set_frame_delay(0);
}

void Player::Stop() {

    switch (Character::get_character_status()) {

        case WALK_DOWN: Character::set_character_status(LOOK_DOWN); break;
        case WALK_UP: Character::set_character_status(LOOK_UP); break;
        case WALK_RIGHT: Character::set_character_status(LOOK_RIGHT); break;
        case WALK_LEFT: Character::set_character_status(LOOK_LEFT); break;

        case SKILL_WALK_DOWN: Character::set_character_status(SKILL_DOWN); break;
        case SKILL_WALK_UP: Character::set_character_status(SKILL_UP); break;
        case SKILL_WALK_RIGHT: Character::set_character_status(SKILL_RIGHT); break;
        case SKILL_WALK_LEFT: Character::set_character_status(SKILL_LEFT); break;

        case ATTACK_DOWN: Character::set_character_status(LOOK_DOWN); break;
        case ATTACK_UP: Character::set_character_status(LOOK_UP); break;
        case ATTACK_RIGHT: Character::set_character_status(LOOK_RIGHT); break;
        case ATTACK_LEFT: Character::set_character_status(LOOK_LEFT); break;

        case DAMAGE_DOWN: Character::set_character_status(LOOK_DOWN); break;
        case DAMAGE_UP: Character::set_character_status(LOOK_UP); break;
        case DAMAGE_RIGHT: Character::set_character_status(LOOK_RIGHT); break;
        case DAMAGE_LEFT: Character::set_character_status(LOOK_LEFT); break;

        case SUPER_ATTACK: Character::set_character_status(LOOK_DOWN); break;
    }
}

void Player::StopDoingSkill() {

    switch (Character::get_character_status()) {

        case SKILL_DOWN: Character::set_character_status(LOOK_DOWN); break;
        case SKILL_UP: Character::set_character_status(LOOK_UP); break;
        case SKILL_RIGHT: Character::set_character_status(LOOK_RIGHT); break;
        case SKILL_LEFT: Character::set_character_status(LOOK_LEFT); break;

        case SKILL_WALK_DOWN: Character::set_character_status(LOOK_DOWN); break;
        case SKILL_WALK_UP: Character::set_character_status(LOOK_UP); break;
        case SKILL_WALK_RIGHT: Character::set_character_status(LOOK_RIGHT); break;
        case SKILL_WALK_LEFT: Character::set_character_status(LOOK_LEFT); break;
    }
}

void Player::TextureOffsetOfPlayerState(const int &tex_id,
                                        const int &tex_w,
                                        const int &tex_h,
                                        const bool &run) {

    float player_tex_x = 0;
    float player_tex_y = 0;

    float player_tex_offset_x = 1.0f / static_cast<float>(tex_w / (Character::get_frame_texture_tile() * kTextureTileSize));
    float player_tex_offset_y = 1.0f / static_cast<float>(tex_h / (Character::get_frame_texture_tile() * kTextureTileSize));

    bool reverse = false;

    switch (Character::get_character_status()) {
            // * Look

        case LOOK_DOWN:
            player_tex_x = player_tex_offset_x * 0;
            player_tex_y = player_tex_offset_y;
            break;
        case LOOK_UP:
            player_tex_x = player_tex_offset_x * 1;
            player_tex_y = player_tex_offset_y;
            break;
        case LOOK_RIGHT:
            player_tex_x = player_tex_offset_x * 2;
            player_tex_y = player_tex_offset_y;
            break;
        case LOOK_LEFT:
            player_tex_x = player_tex_offset_x * 2;
            player_tex_y = player_tex_offset_y;

            reverse = true;
            break;

        case DYING:
            player_tex_x = player_tex_offset_x * 3;
            player_tex_y = player_tex_offset_y;
            break;

            // * Skill

        case SKILL_DOWN:
            player_tex_x = player_tex_offset_x * 0;
            player_tex_y = player_tex_offset_y * (10 + Character::get_frame_seq());
            if (run) { Player::NextFrame(2); }
            break;
        case SKILL_UP:
            player_tex_x = player_tex_offset_x * 1;
            player_tex_y = player_tex_offset_y * (10 + Character::get_frame_seq());
            if (run) { Player::NextFrame(2); }
            break;
        case SKILL_RIGHT:
            player_tex_x = player_tex_offset_x * 2;
            player_tex_y = player_tex_offset_y * (10 + Character::get_frame_seq());
            if (run) { Player::NextFrame(2); }
            break;
        case SKILL_LEFT:
            player_tex_x = player_tex_offset_x * 2;
            player_tex_y = player_tex_offset_y * (10 + Character::get_frame_seq());
            if (run) { Player::NextFrame(2); }

            reverse = true;
            break;

            // * Walk

        case WALK_DOWN:
            player_tex_x = player_tex_offset_x * 0;
            player_tex_y = player_tex_offset_y * (2 + Character::get_frame_seq());
            if (run) { Player::NextFrame(4); }
            break;
        case WALK_UP:
            player_tex_x = player_tex_offset_x * 1;
            player_tex_y = player_tex_offset_y * (2 + Character::get_frame_seq());
            if (run) { Player::NextFrame(4); }
            break;
        case WALK_RIGHT:
            player_tex_x = player_tex_offset_x * 2;
            player_tex_y = player_tex_offset_y * (2 + Character::get_frame_seq());
            if (run) { Player::NextFrame(4); }
            break;
        case WALK_LEFT:
            player_tex_x = player_tex_offset_x * 2;
            player_tex_y = player_tex_offset_y * (2 + Character::get_frame_seq());
            if (run) { Player::NextFrame(4); }

            reverse = true;
            break;

            // * Skillwalk

        case SKILL_WALK_DOWN:
            player_tex_x = player_tex_offset_x * 0;
            player_tex_y = player_tex_offset_y * (11 + Character::get_frame_seq());
            if (run) { Player::NextFrame(2); }
            break;
        case SKILL_WALK_UP:
            player_tex_x = player_tex_offset_x * 1;
            player_tex_y = player_tex_offset_y * (11 + Character::get_frame_seq());
            if (run) { Player::NextFrame(2); }
            break;
        case SKILL_WALK_RIGHT:
            player_tex_x = player_tex_offset_x * 2;
            player_tex_y = player_tex_offset_y * (11 + Character::get_frame_seq());
            if (run) { Player::NextFrame(2); }
            break;
        case SKILL_WALK_LEFT:
            player_tex_x = player_tex_offset_x * 2;
            player_tex_y = player_tex_offset_y * (11 + Character::get_frame_seq());
            if (run) { Player::NextFrame(2); }

            reverse = true;
            break;

            // * Damage

        case DAMAGE_DOWN:
            player_tex_x = player_tex_offset_x * 0;
            player_tex_y = player_tex_offset_y * (13 + Character::get_frame_seq());
            if (run) { Player::NextFrame(2); }
            break;
        case DAMAGE_UP:
            player_tex_x = player_tex_offset_x * 1;
            player_tex_y = player_tex_offset_y * (13 + Character::get_frame_seq());
            if (run) { Player::NextFrame(2); }
            break;
        case DAMAGE_RIGHT:
            player_tex_x = player_tex_offset_x * 2;
            player_tex_y = player_tex_offset_y * (13 + Character::get_frame_seq());
            if (run) { Player::NextFrame(2); }
            break;
        case DAMAGE_LEFT:
            player_tex_x = player_tex_offset_x * 2;
            player_tex_y = player_tex_offset_y * (13 + Character::get_frame_seq());
            if (run) { Player::NextFrame(2); }

            reverse = true;
            break;

            // * Attack

        case ATTACK_DOWN:
            player_tex_x = player_tex_offset_x * 0;
            player_tex_y = player_tex_offset_y * (6 + Character::get_frame_seq());
            if (run) { Player::NextFrame(4); }
            break;
        case ATTACK_UP:
            player_tex_x = player_tex_offset_x * 1;
            player_tex_y = player_tex_offset_y * (6 + Character::get_frame_seq());
            if (run) { Player::NextFrame(4); }
            break;
        case ATTACK_RIGHT:
            player_tex_x = player_tex_offset_x * 2;
            player_tex_y = player_tex_offset_y * (6 + Character::get_frame_seq());
            if (run) { Player::NextFrame(4); }
            break;
        case ATTACK_LEFT:
            player_tex_x = player_tex_offset_x * 2;
            player_tex_y = player_tex_offset_y * (6 + Character::get_frame_seq());
            if (run) { Player::NextFrame(4); }

            reverse = true;
            break;

            // * Superattack

        case SUPER_ATTACK:
            player_tex_x = player_tex_offset_x * 3;
            player_tex_y = player_tex_offset_y * (2 + Character::get_frame_seq());
            if (run) { Player::NextFrame(super_attack_frame_offset_); }
            break;
    }

    float player_final_tex_x = player_tex_x + player_tex_offset_x;
    float player_final_tex_y = player_tex_y - player_tex_offset_y;

    if (!reverse) {

        Character::DrawCharacter(

            tex_id,

            player_tex_x,
            player_tex_y,

            player_final_tex_x,
            player_final_tex_y

        );

    } else {

        Character::DrawCharacter(

            tex_id,

            player_final_tex_x,
            player_tex_y,

            player_tex_x,
            player_final_tex_y

        );
    }
}
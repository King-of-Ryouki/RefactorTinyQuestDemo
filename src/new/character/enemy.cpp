#include "enemy.hpp"

void Enemy::Draw(const int &tex_id, const int &tex_w, const int &tex_h, const bool &run) {

    float enemy_tex_x;
    float enemy_tex_y;

    float enemy_tex_offset_x = 1.0f / static_cast<float>(tex_w / (Enemy::get_frame_texture_tile() * kTextureTileSize));
    float enemy_tex_offset_y = 1.0f / static_cast<float>(tex_h / (Enemy::get_frame_texture_tile() * kTextureTileSize));

    bool reverse = false;

    switch (Character::get_character_status()) {
        case LOOK_DOWN:
            enemy_tex_x = enemy_tex_offset_x * 5;
            enemy_tex_y = enemy_tex_offset_y * 1;
            break;
        case LOOK_UP:
            enemy_tex_x = enemy_tex_offset_x * 5;
            enemy_tex_y = enemy_tex_offset_y * 2;
            break;
        case LOOK_RIGHT:
            enemy_tex_x = enemy_tex_offset_x * 5;
            enemy_tex_y = enemy_tex_offset_y * 3;
            break;
        case LOOK_LEFT:
            enemy_tex_x = enemy_tex_offset_x * 5;
            enemy_tex_y = enemy_tex_offset_y * 3;

            reverse = true;
            break;

        case WALK_DOWN:
            enemy_tex_x = enemy_tex_offset_x * Character::get_frame_seq();
            enemy_tex_y = enemy_tex_offset_y * 1;
            if (run) { Enemy::NextFrame(6); }
            break;
        case WALK_UP:
            enemy_tex_x = enemy_tex_offset_x * Character::get_frame_seq();
            enemy_tex_y = enemy_tex_offset_y * 2;
            if (run) { Enemy::NextFrame(6); }
            break;
        case WALK_RIGHT:
            enemy_tex_x = enemy_tex_offset_x * Character::get_frame_seq();
            enemy_tex_y = enemy_tex_offset_y * 3;
            if (run) { Enemy::NextFrame(6); }
            break;
        case WALK_LEFT:
            enemy_tex_x = enemy_tex_offset_x * Character::get_frame_seq();
            enemy_tex_y = enemy_tex_offset_y * 3;
            if (run) { Enemy::NextFrame(6); }

            reverse = true;
            break;

        case DAMAGE_DOWN:
            enemy_tex_x = enemy_tex_offset_x * Character::get_frame_seq();
            enemy_tex_y = enemy_tex_offset_y * 4;
            if (run) { Enemy::NextFrame(6); }
            break;
        case DAMAGE_UP:
            enemy_tex_x = enemy_tex_offset_x * Character::get_frame_seq();
            enemy_tex_y = enemy_tex_offset_y * 5;
            if (run) { Enemy::NextFrame(6); }
            break;
        case DAMAGE_RIGHT:
            enemy_tex_x = enemy_tex_offset_x * Character::get_frame_seq();
            enemy_tex_y = enemy_tex_offset_y * 6;
            if (run) { Enemy::NextFrame(6); }
            break;
        case DAMAGE_LEFT:
            enemy_tex_x = enemy_tex_offset_x * Character::get_frame_seq();
            enemy_tex_y = enemy_tex_offset_y * 6;
            if (run) { Enemy::NextFrame(6); }

            reverse = true;
            break;

        case ATTACK_DOWN:
            enemy_tex_x = enemy_tex_offset_x * Character::get_frame_seq();
            enemy_tex_y = enemy_tex_offset_y * 7;
            if (run) { Enemy::NextFrame(10); }
            break;
        case ATTACK_UP:
            enemy_tex_x = enemy_tex_offset_x * Character::get_frame_seq();
            enemy_tex_y = enemy_tex_offset_y * 8;
            if (run) { Enemy::NextFrame(10); }
            break;
        case ATTACK_RIGHT:
            enemy_tex_x = enemy_tex_offset_x * Character::get_frame_seq();
            enemy_tex_y = enemy_tex_offset_y * 9;
            if (run) { Enemy::NextFrame(10); }
            break;
        case ATTACK_LEFT:
            enemy_tex_x = enemy_tex_offset_x * Character::get_frame_seq();
            enemy_tex_y = enemy_tex_offset_y * 9;
            if (run) { Enemy::NextFrame(10); }

            reverse = true;
            break;

        case DYING:
            enemy_tex_x = enemy_tex_offset_x * Character::get_frame_seq();
            enemy_tex_y = enemy_tex_offset_y * 10;
            if (run) { Enemy::NextFrame(10); }
            break;
    }

    float enemy_final_tex_x = enemy_tex_x + enemy_tex_offset_x;
    float enemy_final_tex_y = enemy_tex_y - enemy_tex_offset_y;

    float aux;
    if (reverse) { aux = enemy_tex_x, enemy_tex_x = enemy_final_tex_x, enemy_final_tex_x = aux; }

    Enemy::DrawCharacter(tex_id, enemy_tex_x, enemy_tex_y, enemy_final_tex_x, enemy_final_tex_y);
}

void Enemy::NextFrame(const int &max) {

    Character::set_frame_delay(Character::get_frame_delay() + 1);

    if (Character::get_character_status() == ATTACK_DOWN ||
        Character::get_character_status() == ATTACK_UP ||
        Character::get_character_status() == ATTACK_RIGHT ||
        Character::get_character_status() == ATTACK_LEFT) {

        if (Character::get_frame_delay() == frame_delay_attack_) {

            Character::set_frame_seq(Character::get_frame_seq() + 1);
            Character::set_frame_seq(Character::get_frame_seq() % max);
            Character::set_frame_delay(0);
        }

    } else if (Character::get_character_status() == DYING) {

        if (Character::get_frame_delay() == frame_delay_dying_) {

            Character::set_frame_seq(Character::get_frame_seq() + 1);
            Character::set_frame_seq(Character::get_frame_seq() % max);
            Character::set_frame_delay(0);
        }
    }

    if (Character::get_frame_delay() == frame_delay_walk_) {

        Character::set_frame_seq(Character::get_frame_seq() + 1);
        Character::set_frame_seq(Character::get_frame_seq() % max);
        Character::set_frame_delay(0);
    }
}

void Enemy::Move() {

    switch (Character::get_character_status()) {

        case WALK_DOWN: Character::set_character_y(Character::get_character_y() - step_length_); break;
        case WALK_UP: Character::set_character_y(Character::get_character_y() + step_length_); break;
        case WALK_RIGHT: Character::set_character_x(Character::get_character_x() + step_length_); break;
        case WALK_LEFT: Character::set_character_x(Character::get_character_x() - step_length_); break;
    }
}
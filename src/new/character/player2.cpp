#include "player2.hpp"

void Player2::Draw(const int &tex_id, const int &tex_w, const int &tex_h, const bool &run) {
    Player::TextureOffsetOfPlayerState(tex_id, tex_w, tex_h, run);
}

bool Player2::Attack(list<Character *> &enemies, list<Projectile *> &list_Projectile, bool *projectile_hit_map) {

    // the offset serves to reposition the projectile so that we only have to check collision
    // at the intersection between tiles (whatever the player's post and the projectile's step)

    FireBall cFireBall;

    int projectile_pos_x;
    int projectile_pos_y;

    int proyectile_step = cFireBall.get_projectile_step_length();

    int offset_x = Character::get_character_x() % proyectile_step;
    int offset_y = Character::get_character_y() % proyectile_step;

    if (Character::get_character_status() != SUPER_ATTACK &&
        Character::get_frame_seq() == 2 &&
        Character::get_frame_delay() == 0) {

        switch (Character::get_character_status()) {

            case ATTACK_UP: {

                list_Projectile.push_back(new FireBall);

                projectile_pos_x = Character::get_character_x() + (Character::get_base_tile_w() - list_Projectile.back()->Projectile::get_projectile_hit_w()) / 2;
                projectile_pos_y = Character::get_character_y() + Character::get_base_tile_h() - offset_y;

                list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_UP);
                list_Projectile.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                // 判斷投射物是否出現在敵人或物體之上 // evaluate if the projectile appears on top of an enemy or object
                if (list_Projectile.back()->Projectile::ComputeEnemyCollisions(enemies) ||
                    list_Projectile.back()->Projectile::HitMapAtUpOrDown(projectile_hit_map, false) ||
                    list_Projectile.back()->Projectile::HitMapAtUpOrDown(projectile_hit_map, true)) {
                    list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_HIT_UP);
                }

                break;
            }

            case ATTACK_DOWN: {

                list_Projectile.push_back(new FireBall);

                projectile_pos_x = Character::get_character_x() + (Character::get_base_tile_w() - list_Projectile.back()->Projectile::get_projectile_hit_w()) / 2;
                projectile_pos_y = Character::get_character_y() - list_Projectile.back()->Projectile::get_projectile_hit_h() + (proyectile_step - offset_y) % proyectile_step;

                list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_DOWN);
                list_Projectile.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                // 判斷投射物是否出現在敵人或物體之上 // evaluate if the projectile appears on top of an enemy or object
                if (list_Projectile.back()->Projectile::ComputeEnemyCollisions(enemies) ||
                    list_Projectile.back()->Projectile::HitMapAtUpOrDown(projectile_hit_map, false)) {
                    list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_HIT_DOWN);
                }

                break;
            }

            case ATTACK_LEFT: {

                list_Projectile.push_back(new FireBall);

                projectile_pos_x = Character::get_character_x() - list_Projectile.back()->Projectile::get_projectile_hit_w() + (proyectile_step - offset_x) % proyectile_step;
                projectile_pos_y = Character::get_character_y() + (Character::get_base_tile_h() - list_Projectile.back()->Projectile::get_projectile_hit_h()) / 2;

                list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_LEFT);
                list_Projectile.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                // 判斷投射物是否出現在敵人或物體之上 // evaluate if the projectile appears on top of an enemy or object
                if (list_Projectile.back()->Projectile::ComputeEnemyCollisions(enemies) ||
                    list_Projectile.back()->Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, false)) {
                    list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_HIT_LEFT);
                }

                break;
            }

            case ATTACK_RIGHT: {

                list_Projectile.push_back(new FireBall);

                projectile_pos_x = Character::get_character_x() + Character::get_base_tile_w() - offset_x;
                projectile_pos_y = Character::get_character_y() + (Character::get_base_tile_h() - list_Projectile.back()->Projectile::get_projectile_hit_h()) / 2;

                list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_RIGHT);
                list_Projectile.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                // 判斷投射物是否出現在敵人或物體之上 // evaluate if the projectile appears on top of an enemy or object
                if (list_Projectile.back()->Projectile::ComputeEnemyCollisions(enemies) ||
                    list_Projectile.back()->Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, false) ||
                    list_Projectile.back()->Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, true)) {

                    list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_HIT_RIGHT);
                }

                break;
            }
        }

        Character::set_now_mp(Character::get_now_mp() - ATTACK_COST);

        return true;

    } else if (Character::get_character_status() == SUPER_ATTACK &&
               Character::get_frame_delay() == 0 &&
               Character::get_now_mp() >= ATTACK_COST / 2) {

        // * 發動技能

        switch (Character::get_frame_seq()) { // 第幾幀數

            case 3: {

                list_Projectile.push_back(new FireBall);

                projectile_pos_x = Character::get_character_x() + (Character::get_base_tile_w() - list_Projectile.back()->Projectile::get_projectile_hit_w()) / 2;
                projectile_pos_y = Character::get_character_y() + kTileSize + Character::get_base_tile_h() - offset_y;

                list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_UP);
                list_Projectile.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                // 判斷投射物是否出現在敵人或物體之上 // evaluate if the projectile appears on top of an enemy or object
                if (list_Projectile.back()->Projectile::ComputeEnemyCollisions(enemies) ||
                    list_Projectile.back()->Projectile::HitMapAtUpOrDown(projectile_hit_map, false) ||
                    list_Projectile.back()->Projectile::HitMapAtUpOrDown(projectile_hit_map, true)) {

                    list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_HIT_UP);
                }

                Character::set_now_mp(Character::get_now_mp() - ATTACK_COST / 2);

                return true;
                break;
            }

            case 4: {

                list_Projectile.push_back(new FireBall);

                projectile_pos_x = Character::get_character_x() + Character::get_base_tile_w() - offset_x;
                projectile_pos_y = Character::get_character_y() + kTileSize + (Character::get_base_tile_h() - list_Projectile.back()->Projectile::get_projectile_hit_h()) / 2;

                list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_RIGHT);
                list_Projectile.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                // 判斷投射物是否出現在敵人或物體之上 // evaluate if the projectile appears on top of an enemy or object
                if (list_Projectile.back()->Projectile::ComputeEnemyCollisions(enemies) ||
                    list_Projectile.back()->Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, false) ||
                    list_Projectile.back()->Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, true)) {

                    list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_HIT_RIGHT);
                }

                list_Projectile.back()->Projectile::set_replace_over(true);

                Character::set_now_mp(Character::get_now_mp() - ATTACK_COST / 2);

                return true;
                break;
            }

            case 5: {

                list_Projectile.push_back(new FireBall);

                projectile_pos_x = Character::get_character_x() + Character::get_base_tile_w() - offset_x;
                projectile_pos_y = Character::get_character_y() + kTileSize + (Character::get_base_tile_h() - list_Projectile.back()->Projectile::get_projectile_hit_h()) / 2;

                list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_RIGHT);
                list_Projectile.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                // 判斷投射物是否出現在敵人或物體之上 // evaluate if the projectile appears on top of an enemy or object
                if (list_Projectile.back()->Projectile::ComputeEnemyCollisions(enemies) ||
                    list_Projectile.back()->Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, false) ||
                    list_Projectile.back()->Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, true)) {

                    list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_HIT_RIGHT);
                }

                Character::set_now_mp(Character::get_now_mp() - ATTACK_COST / 2);

                return true;
                break;
            }

            case 6: {

                list_Projectile.push_back(new FireBall);

                projectile_pos_x = Character::get_character_x() + Character::get_base_tile_w() - offset_x;
                projectile_pos_y = Character::get_character_y() + kTileSize + (Character::get_base_tile_h() - list_Projectile.back()->Projectile::get_projectile_hit_h()) / 2;

                list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_RIGHT);
                list_Projectile.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                // 判斷投射物是否出現在敵人或物體之上 // evaluate if the projectile appears on top of an enemy or object
                if (list_Projectile.back()->Projectile::ComputeEnemyCollisions(enemies) ||
                    list_Projectile.back()->Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, false) ||
                    list_Projectile.back()->Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, true)) {

                    list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_HIT_RIGHT);
                }

                list_Projectile.back()->Projectile::set_replace_under(true);

                Character::set_now_mp(Character::get_now_mp() - ATTACK_COST / 2);

                return true;
                break;
            }

            case 7: {

                list_Projectile.push_back(new FireBall);

                projectile_pos_x = Character::get_character_x() + (Character::get_base_tile_w() - list_Projectile.back()->Projectile::get_projectile_hit_w()) / 2;
                projectile_pos_y = Character::get_character_y() + kTileSize - list_Projectile.back()->Projectile::get_projectile_hit_h() + (proyectile_step - offset_y) % proyectile_step;

                list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_DOWN);
                list_Projectile.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                // 判斷投射物是否出現在敵人或物體之上 // evaluate if the projectile appears on top of an enemy or object
                if (list_Projectile.back()->Projectile::ComputeEnemyCollisions(enemies) ||
                    list_Projectile.back()->Projectile::HitMapAtUpOrDown(projectile_hit_map, false)) {

                    list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_HIT_DOWN);
                }

                Character::set_now_mp(Character::get_now_mp() - ATTACK_COST / 2);

                return true;
                break;
            }

            case 8: {

                list_Projectile.push_back(new FireBall);

                projectile_pos_x = Character::get_character_x() - list_Projectile.back()->Projectile::get_projectile_hit_w() + (proyectile_step - offset_x) % proyectile_step;
                projectile_pos_y = Character::get_character_y() + kTileSize + (Character::get_base_tile_h() - list_Projectile.back()->Projectile::get_projectile_hit_h()) / 2;

                list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_LEFT);
                list_Projectile.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                // 判斷投射物是否出現在敵人或物體之上 // evaluate if the projectile appears on top of an enemy or object
                if (list_Projectile.back()->Projectile::ComputeEnemyCollisions(enemies) ||
                    list_Projectile.back()->Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, false)) {

                    list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_HIT_LEFT);
                }

                list_Projectile.back()->Projectile::set_replace_under(true);

                Character::set_now_mp(Character::get_now_mp() - ATTACK_COST / 2);

                return true;
                break;
            }

            case 9: {

                list_Projectile.push_back(new FireBall);

                projectile_pos_x = Character::get_character_x() - list_Projectile.back()->Projectile::get_projectile_hit_w() + (proyectile_step - offset_x) % proyectile_step;
                projectile_pos_y = Character::get_character_y() + kTileSize + (Character::get_base_tile_h() - list_Projectile.back()->Projectile::get_projectile_hit_h()) / 2;

                list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_LEFT);
                list_Projectile.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                // 判斷投射物是否出現在敵人或物體之上 // evaluate if the projectile appears on top of an enemy or object
                if (list_Projectile.back()->Projectile::ComputeEnemyCollisions(enemies) ||
                    list_Projectile.back()->Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, false)) {
                    list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_HIT_LEFT);
                }

                Character::set_now_mp(Character::get_now_mp() - ATTACK_COST / 2);

                return true;
                break;
            }

            case 10: {

                list_Projectile.push_back(new FireBall);

                projectile_pos_x = Character::get_character_x() - list_Projectile.back()->Projectile::get_projectile_hit_w() + (proyectile_step - offset_x) % proyectile_step;
                projectile_pos_y = Character::get_character_y() + kTileSize + (Character::get_base_tile_h() - list_Projectile.back()->Projectile::get_projectile_hit_h()) / 2;

                list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_LEFT);
                list_Projectile.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                // 判斷投射物是否出現在敵人或物體之上 // evaluate if the projectile appears on top of an enemy or object
                if (list_Projectile.back()->Projectile::ComputeEnemyCollisions(enemies) ||
                    list_Projectile.back()->Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, false)) {
                    list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_HIT_LEFT);
                }
                list_Projectile.back()->Projectile::set_replace_over(true);

                Character::set_now_mp(Character::get_now_mp() - ATTACK_COST / 2);

                return true;
                break;
            }

            case 11: {

                list_Projectile.push_back(new FireBall);

                projectile_pos_x = Character::get_character_x() + (Character::get_base_tile_w() - list_Projectile.back()->Projectile::get_projectile_hit_w()) / 2;
                projectile_pos_y = Character::get_character_y() + kTileSize + Character::get_base_tile_h() - offset_y;

                list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_UP);
                list_Projectile.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                // 判斷投射物是否出現在敵人或物體之上 // evaluate if the projectile appears on top of an enemy or object
                if (list_Projectile.back()->Projectile::ComputeEnemyCollisions(enemies) ||
                    list_Projectile.back()->Projectile::HitMapAtUpOrDown(projectile_hit_map, false) ||
                    list_Projectile.back()->Projectile::HitMapAtUpOrDown(projectile_hit_map, true)) {
                    list_Projectile.back()->Projectile::set_projectile_status(PROJECTILE_HIT_UP);
                }

                Character::set_now_mp(Character::get_now_mp() - ATTACK_COST / 2);

                return true;
                break;
            }
        }
    }

    return false;
}
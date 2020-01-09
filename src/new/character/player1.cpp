#include "player1.hpp"

void Player1::set_logic_status(int logic_status) { logic_status_ = logic_status; }
int Player1::get_logic_state() { return logic_status_; }

void Player1::Draw(const int &tex_id, const int &tex_w, const int &tex_h, const bool &run) {
    Player::TextureOffsetOfPlayerState(tex_id, tex_w, tex_h, run);
}

bool Player1::Attack(Character *enemy) {

    bool hit = false;

    if (Character::get_character_status() != SUPER_ATTACK &&
        Character::get_frame_seq() == 3 &&
        Character::get_frame_delay() == 0) {
        if (Character::get_character_status() == ATTACK_UP) {

            Rect AttackArea;

            AttackArea.left   = Character::get_character_x() - kTileSize;
            AttackArea.right  = Character::get_character_x() + Character::get_base_tile_w() + kTileSize;
            AttackArea.bottom = Character::get_character_y() + Character::get_base_tile_h();
            AttackArea.top    = Character::get_character_y() + Character::get_base_tile_h() + kTileSize;

            if (enemy->Character::get_character_status() != DYING &&
                Intersection(AttackArea, enemy->getHitBox())) {
                int new_health = enemy->get_now_hp() - get_damage();

                if (new_health <= 0) {
                    enemy->Character::set_character_status(DYING);
                } else {
                    enemy->Character::set_character_status(DAMAGE_DOWN);
                }

                if (new_health < 0) { new_health = 0; }

                enemy->set_now_hp(new_health);
                enemy->set_frame_seq(0);
                enemy->set_frame_delay(0);

                hit = true;
            }

        } else if (Character::get_character_status() == ATTACK_DOWN) {

            Rect AttackArea;

            AttackArea.left   = Character::get_character_x() - kTileSize;
            AttackArea.right  = Character::get_character_x() + kTileSize + Character::get_base_tile_w();
            AttackArea.bottom = Character::get_character_y() - kTileSize / 2;
            AttackArea.top    = Character::get_character_y();

            if (enemy->Character::get_character_status() != DYING &&
                Intersection(AttackArea, enemy->getHitBox())) {
                int new_health = enemy->get_now_hp() - get_damage();

                if (new_health <= 0) {
                    enemy->Character::set_character_status(DYING);
                } else {
                    enemy->Character::set_character_status(DAMAGE_UP);
                }

                if (new_health < 0) { new_health = 0; }

                enemy->set_now_hp(new_health);
                enemy->set_frame_seq(0);
                enemy->set_frame_delay(0);

                hit = true;
            }

        } else if (Character::get_character_status() == ATTACK_LEFT) {

            Rect AttackArea;

            AttackArea.left   = Character::get_character_x() - (kTileSize / 5) * 7;
            AttackArea.right  = Character::get_character_x();
            AttackArea.bottom = Character::get_character_y();
            AttackArea.top    = Character::get_character_y() + Character::get_base_tile_h();

            if (enemy->Character::get_character_status() != DYING &&
                Intersection(AttackArea, enemy->getHitBox())) {
                int new_health = enemy->get_now_hp() - get_damage();

                if (new_health <= 0) {
                    enemy->Character::set_character_status(DYING);
                } else {
                    enemy->Character::set_character_status(DAMAGE_RIGHT);
                }

                if (new_health < 0) { new_health = 0; }

                enemy->set_now_hp(new_health);
                enemy->set_frame_seq(0);
                enemy->set_frame_delay(0);

                hit = true;
            }

        } else if (Character::get_character_status() == ATTACK_RIGHT) {

            Rect AttackArea;

            AttackArea.left   = Character::get_character_x() + Character::get_base_tile_w();
            AttackArea.right  = Character::get_character_x() + Character::get_base_tile_w() + (kTileSize / 5) * 7;
            AttackArea.bottom = Character::get_character_y();
            AttackArea.top    = Character::get_character_y() + Character::get_base_tile_h();

            if (enemy->Character::get_character_status() != DYING && Intersection(AttackArea, enemy->getHitBox())) {
                int new_health = enemy->get_now_hp() - get_damage();

                if (new_health <= 0) {
                    enemy->Character::set_character_status(DYING);
                } else {
                    enemy->Character::set_character_status(DAMAGE_LEFT);
                }

                if (new_health < 0) { new_health = 0; }

                enemy->set_now_hp(new_health);
                enemy->set_frame_seq(0);
                enemy->set_frame_delay(0);

                hit = true;
            }
        }

    } else if (Character::get_character_status() == SUPER_ATTACK &&
               (Character::get_frame_seq() > 0 ||
                Character::get_frame_seq() < 5) &&
               Character::get_frame_delay() == 0) {

        if (Character::get_frame_seq() == 1) {

            Rect AttackArea;

            AttackArea.left   = Character::get_character_x() - kTileSize * 2;
            AttackArea.right  = Character::get_character_x() + Character::get_base_tile_w();
            AttackArea.bottom = Character::get_character_y();
            AttackArea.top    = Character::get_character_y() + Character::get_base_tile_h() + kTileSize * 2;

            if (enemy->Character::get_character_status() != DYING && Intersection(AttackArea, enemy->getHitBox())) {
                int new_health = enemy->get_now_hp() - get_damage();

                if (new_health <= 0) {
                    enemy->Character::set_character_status(DYING);
                } else {
                    enemy->Character::set_character_status(DAMAGE_DOWN);
                }

                if (new_health < 0) { new_health = 0; }

                enemy->set_now_hp(new_health);
                enemy->set_frame_seq(0);
                enemy->set_frame_delay(0);

                hit = true;
            }

        } else if (Character::get_frame_seq() == 2) {

            Rect AttackArea;

            AttackArea.left   = Character::get_character_x() - kTileSize * 2;
            AttackArea.right  = Character::get_character_x() + Character::get_base_tile_w();
            AttackArea.bottom = Character::get_character_y() - kTileSize;
            AttackArea.top    = Character::get_character_y() + Character::get_base_tile_h();

            if (enemy->Character::get_character_status() != DYING && Intersection(AttackArea, enemy->getHitBox())) {
                int new_health = enemy->get_now_hp() - get_damage();

                if (new_health <= 0) {
                    enemy->Character::set_character_status(DYING);
                } else {
                    enemy->Character::set_character_status(DAMAGE_RIGHT);
                }

                if (new_health < 0) { new_health = 0; }

                enemy->set_now_hp(new_health);
                enemy->set_frame_seq(0);
                enemy->set_frame_delay(0);

                hit = true;
            }

        } else if (Character::get_frame_seq() == 3) {

            Rect AttackArea;

            AttackArea.left   = Character::get_character_x();
            AttackArea.right  = Character::get_character_x() + Character::get_base_tile_w() + kTileSize * 2;
            AttackArea.bottom = Character::get_character_y() - kTileSize;
            AttackArea.top    = Character::get_character_y() + Character::get_base_tile_h();

            if (enemy->Character::get_character_status() != DYING && Intersection(AttackArea, enemy->getHitBox())) {
                int new_health = enemy->get_now_hp() - get_damage();

                if (new_health <= 0) {
                    enemy->Character::set_character_status(DYING);
                } else {
                    enemy->Character::set_character_status(DAMAGE_UP);
                }

                if (new_health < 0) { new_health = 0; }

                enemy->set_now_hp(new_health);
                enemy->set_frame_seq(0);
                enemy->set_frame_delay(0);

                hit = true;
            }

        } else if (Character::get_frame_seq() == 4) {

            Rect AttackArea;

            AttackArea.left   = Character::get_character_x();
            AttackArea.right  = Character::get_character_x() + Character::get_base_tile_w() + kTileSize * 2;
            AttackArea.bottom = Character::get_character_y();
            AttackArea.top    = Character::get_character_y() + Character::get_base_tile_h() + kTileSize * 2;

            if (enemy->Character::get_character_status() != DYING && Intersection(AttackArea, enemy->getHitBox())) {
                int new_health = enemy->get_now_hp() - get_damage();

                (new_health <= 0) ? enemy->Character::set_character_status(DYING)
                                  : enemy->Character::set_character_status(DAMAGE_LEFT);

                if (new_health < 0) { new_health = 0; }

                enemy->set_now_hp(new_health);
                enemy->set_frame_seq(0);
                enemy->set_frame_delay(0);

                hit = true;
            }
        }
    }

    return hit;
}

void Player1::StopP1() {

    Stop();
    Player1::set_logic_status(Character::get_character_status());
}

void Player1::StopDoingSkillP1() {

    Player1::StopDoingSkill();
    Player1::set_logic_status(Character::get_character_status());
}

void Player1::set_shield_state(int logic_status) {

    // if the logic_status was one of skill in stopped we must transform it into one of movement in case we move with the keys,
    // since the logic_status is maintained while the skill is done. If we kept standing the Stop function would correct us the status
    switch (logic_status) {

        case SKILL_DOWN: Character::set_character_status(SKILL_WALK_DOWN); break;
        case SKILL_UP: Character::set_character_status(SKILL_WALK_UP); break;
        case SKILL_RIGHT: Character::set_character_status(SKILL_WALK_RIGHT); break;
        case SKILL_LEFT: Character::set_character_status(SKILL_WALK_LEFT); break;

        default: {
            if (logic_status == SKILL_WALK_DOWN ||
                logic_status == SKILL_WALK_UP ||
                logic_status == SKILL_WALK_RIGHT ||
                logic_status == SKILL_WALK_LEFT) {
                Character::set_character_status(logic_status);
            } else {
                Player1::TransformIntoSkillState(); // if the logic_status was not a skill we transformed it into a skill
            }
        }
    }
}

void Player1::TestShieldProtection(int attack_state, int damage, int x, int y) {

    bool shield_useful = false;

    int damage_state;

    if (attack_state == ATTACK_DOWN) {
        if ((Character::get_character_status() == SKILL_UP) ||
            (Character::get_character_status() == SKILL_WALK_UP) ||
            ((Character::get_character_status() == SKILL_LEFT || Character::get_character_status() == SKILL_WALK_LEFT) && x < Character::get_character_x()) ||
            ((Character::get_character_status() == SKILL_RIGHT || Character::get_character_status() == SKILL_WALK_RIGHT) && x > Character::get_character_x())) {

            shield_useful = true;
        }

        damage_state = DAMAGE_UP;

    } else if (attack_state == ATTACK_UP) {

        if ((Character::get_character_status() == SKILL_DOWN) ||
            (Character::get_character_status() == SKILL_WALK_DOWN) ||
            ((Character::get_character_status() == SKILL_LEFT || Character::get_character_status() == SKILL_WALK_LEFT) && x < Character::get_character_x()) ||
            ((Character::get_character_status() == SKILL_RIGHT || Character::get_character_status() == SKILL_WALK_RIGHT) && x > Character::get_character_x())) {

            shield_useful = true;
        }

        damage_state = DAMAGE_DOWN;

    } else if (attack_state == ATTACK_RIGHT) {

        if ((Character::get_character_status() == SKILL_LEFT) ||
            (Character::get_character_status() == SKILL_WALK_LEFT) ||
            ((Character::get_character_status() == SKILL_DOWN || Character::get_character_status() == SKILL_WALK_DOWN) && y < Character::get_character_y()) ||
            ((Character::get_character_status() == SKILL_UP || Character::get_character_status() == SKILL_WALK_UP) && y > Character::get_character_y())) {

            shield_useful = true;
        }

        damage_state = DAMAGE_LEFT;

    } else if (attack_state == ATTACK_LEFT) {

        if ((Character::get_character_status() == SKILL_RIGHT) ||
            (Character::get_character_status() == SKILL_WALK_RIGHT) ||
            ((Character::get_character_status() == SKILL_DOWN || Character::get_character_status() == SKILL_WALK_DOWN) && y < Character::get_character_y()) ||
            ((Character::get_character_status() == SKILL_UP || Character::get_character_status() == SKILL_WALK_UP) && y > Character::get_character_y())) {

            shield_useful = true;
        }

        damage_state = DAMAGE_RIGHT;
    }

    if (shield_useful) {

        int new_sp = Player1::get_now_mp() - damage;

        if (new_sp >= 0) {

            Character::set_now_mp(new_sp);

        } else {

            Character::set_now_mp(0);

            int new_health = Character::get_now_hp() + new_sp;

            if (new_health <= 0) {
                Character::set_now_hp(0);
                Character::set_character_status(DYING);
            } else {
                Character::set_now_hp(new_health);
                Character::set_character_status(damage_state);
            }

            Character::set_frame_seq(0);
            Character::set_frame_delay(0);
        }

    } else {

        int new_health = Character::get_now_hp() - damage;

        if (new_health <= 0) {

            Character::set_now_hp(0);
            Character::set_character_status(DYING);

        } else {

            Character::set_now_hp(new_health);
            Character::set_character_status(damage_state);
        }

        Character::set_frame_seq(0);
        Character::set_frame_delay(0);
    }
}
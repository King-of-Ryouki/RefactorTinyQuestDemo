#include "monster1.hpp"

bool Monster1::Attack(Character *cAttackPlayer) {

    Rect AttackArea;

    bool hit = false;

    if (Character::get_frame_seq() == 3 &&
        Character::get_frame_delay() == 0) {

        switch (Character::get_character_status()) {

            case ATTACK_DOWN: {
                AttackArea.top    = Character::get_character_y();
                AttackArea.bottom = Character::get_character_y() - kTileSize;
                AttackArea.left   = Character::get_character_x();
                AttackArea.right  = Character::get_character_x() + Character::get_base_tile_w();

                if (cAttackPlayer->Character::get_character_status() != DYING && Intersection(AttackArea, cAttackPlayer->getHitBox())) {
                    Player1 *P1 = dynamic_cast<Player1 *>(cAttackPlayer);

                    if (P1 != NULL && P1->IsDoingSkill()) {
                        P1->TestShieldProtection(ATTACK_DOWN, Character::get_damage(), Character::get_character_x(), Character::get_character_y());
                    } else {
                        int new_health = cAttackPlayer->Character::get_now_hp() - Character::get_damage();

                        if (new_health <= 0) {
                            cAttackPlayer->Character::set_character_status(DYING);
                        } else if (!dynamic_cast<Player *>(cAttackPlayer)->IsSuperAttacking()) {
                            cAttackPlayer->Character::set_character_status(DAMAGE_UP);
                            cAttackPlayer->Character::set_frame_seq(0);
                            cAttackPlayer->Character::set_frame_delay(0);
                        }

                        if (new_health < 0) { new_health = 0; }
                        cAttackPlayer->set_now_hp(new_health);

                        hit = true;
                    }
                }

                break;
            }

            case ATTACK_UP: {
                AttackArea.top    = Character::get_character_y() + Character::get_base_tile_h() + kTileSize;
                AttackArea.bottom = Character::get_character_y() + Character::get_base_tile_h();
                AttackArea.left   = Character::get_character_x();
                AttackArea.right  = Character::get_character_x() + Character::get_base_tile_w();

                if (cAttackPlayer->Character::get_character_status() != DYING && Intersection(AttackArea, cAttackPlayer->getHitBox())) {
                    Player1 *P1 = dynamic_cast<Player1 *>(cAttackPlayer);

                    if (P1 != NULL && P1->IsDoingSkill()) {
                        P1->TestShieldProtection(ATTACK_UP, Character::get_damage(), Character::get_character_x(), Character::get_character_y());
                    } else {
                        int new_health = cAttackPlayer->Character::get_now_hp() - Character::get_damage();

                        if (new_health <= 0) {
                            cAttackPlayer->Character::set_character_status(DYING);
                        } else if (!dynamic_cast<Player *>(cAttackPlayer)->IsSuperAttacking()) {
                            cAttackPlayer->Character::set_character_status(DAMAGE_DOWN);
                            cAttackPlayer->Character::set_frame_seq(0);
                            cAttackPlayer->Character::set_frame_delay(0);
                        }

                        if (new_health < 0) { new_health = 0; }
                        cAttackPlayer->set_now_hp(new_health);

                        hit = true;
                    }
                }

                break;
            }

            case ATTACK_RIGHT: {
                AttackArea.top    = Character::get_character_y() + Character::get_base_tile_h();
                AttackArea.bottom = Character::get_character_y();
                AttackArea.left   = Character::get_character_x() + Character::get_base_tile_w();
                AttackArea.right  = Character::get_character_x() + Character::get_base_tile_w() + kTileSize;

                if (cAttackPlayer->Character::get_character_status() != DYING && Intersection(AttackArea, cAttackPlayer->getHitBox())) {
                    Player1 *P1 = dynamic_cast<Player1 *>(cAttackPlayer);

                    if (P1 != NULL && P1->IsDoingSkill()) {
                        P1->TestShieldProtection(ATTACK_RIGHT, Character::get_damage(), Character::get_character_x(), Character::get_character_y());
                    } else {
                        int new_health = cAttackPlayer->Character::get_now_hp() - Character::get_damage();

                        if (new_health <= 0) {
                            cAttackPlayer->Character::set_character_status(DYING);
                        } else if (!dynamic_cast<Player *>(cAttackPlayer)->IsSuperAttacking()) {
                            cAttackPlayer->Character::set_character_status(DAMAGE_LEFT);
                            cAttackPlayer->Character::set_frame_seq(0);
                            cAttackPlayer->Character::set_frame_delay(0);
                        }

                        if (new_health < 0) { new_health = 0; }
                        cAttackPlayer->set_now_hp(new_health);

                        hit = true;
                    }
                }

                break;
            }

            case ATTACK_LEFT: {
                AttackArea.top    = Character::get_character_y() + Character::get_base_tile_h();
                AttackArea.bottom = Character::get_character_y();
                AttackArea.left   = Character::get_character_x() - kTileSize;
                AttackArea.right  = Character::get_character_x();

                if (cAttackPlayer->Character::get_character_status() != DYING && Intersection(AttackArea, cAttackPlayer->getHitBox())) {
                    Player1 *P1 = dynamic_cast<Player1 *>(cAttackPlayer);

                    if (P1 != NULL && P1->IsDoingSkill()) {
                        P1->TestShieldProtection(ATTACK_LEFT, Character::get_damage(), Character::get_character_x(), Character::get_character_y());
                    } else {
                        int new_health = cAttackPlayer->Character::get_now_hp() - Character::get_damage();

                        if (new_health <= 0) {
                            cAttackPlayer->Character::set_character_status(DYING);
                        } else if (!dynamic_cast<Player *>(cAttackPlayer)->IsSuperAttacking()) {
                            cAttackPlayer->Character::set_character_status(DAMAGE_RIGHT);
                            cAttackPlayer->Character::set_frame_seq(0);
                            cAttackPlayer->Character::set_frame_delay(0);
                        }

                        if (new_health < 0) { new_health = 0; }
                        cAttackPlayer->set_now_hp(new_health);

                        hit = true;
                    }
                }

                break;
            }
        }
    }

    return hit;
}

bool Monster1::IA(bool *collision_map, list<Character *> &enemies, Character *p1, Character *p2) {

    Rect AttackAreaV;

    AttackAreaV.left   = Character::get_character_x();
    AttackAreaV.right  = Character::get_character_x() + Character::get_base_tile_w();
    AttackAreaV.bottom = Character::get_character_y() - kTileSize;
    AttackAreaV.top    = Character::get_character_y() + Character::get_base_tile_h() + kTileSize;

    Rect AttackAreaH;

    AttackAreaH.left   = Character::get_character_x() - kTileSize;
    AttackAreaH.right  = Character::get_character_x() + Character::get_base_tile_w() + kTileSize;
    AttackAreaH.bottom = Character::get_character_y();
    AttackAreaH.top    = Character::get_character_y() + Character::get_base_tile_h();

    // 如果有任何玩家到達, 則分配相應的攻擊狀態 // if any player is reach, assign the corresponding attack status
    Rect hitbox_p1 = p1->getHitBox();
    Rect hitbox_p2 = p2->getHitBox();

    if (Intersection(AttackAreaV, hitbox_p1)) {

        if (hitbox_p1.bottom < Character::get_character_y()) {
            Character::set_character_status(ATTACK_DOWN);
        } else {
            Character::set_character_status(ATTACK_UP);
        }

        Character::set_frame_seq(0);
        Character::set_frame_delay(0);

        return true;

    } else if (Intersection(AttackAreaH, hitbox_p1)) {

        if (hitbox_p1.left < Character::get_character_x()) {
            Character::set_character_status(ATTACK_LEFT);
        } else {
            Character::set_character_status(ATTACK_RIGHT);
        }

        Character::set_frame_seq(0);
        Character::set_frame_delay(0);

        return true;

    } else if (Intersection(AttackAreaV, hitbox_p2)) {

        if (hitbox_p2.bottom < Character::get_character_y()) {
            Character::set_character_status(ATTACK_DOWN);
        } else {
            Character::set_character_status(ATTACK_UP);
        }

        Character::set_frame_seq(0);
        Character::set_frame_delay(0);

        return true;

    } else if (Intersection(AttackAreaH, hitbox_p2)) {

        if (hitbox_p2.left < Character::get_character_x()) {
            Character::set_character_status(ATTACK_LEFT);
        } else {
            Character::set_character_status(ATTACK_RIGHT);
        }

        Character::set_frame_seq(0);
        Character::set_frame_delay(0);

        return true;
    }

    int i = 0;

    // 如果無法攻擊, 請計算路徑並指定相應的運動狀態 // If you can not attack, calculate the path and assign the corresponding movement status
    // 如果他們在你移動時攻擊你, 你可能已經從瓷磚的邊緣移位了, 有必要重新安置它 // if they attack you while you are moving, you may have been displaced from the margins of the tile, it is necessary to relocate it
    if (Character::get_character_x() % kTileSize != 0) {

        if (Character::get_character_x() % kTileSize < kTileSize / 2) {
            Character::set_character_status(WALK_LEFT);
        } else {
            Character::set_character_status(WALK_RIGHT);
        }

    } else if (Character::get_character_y() % kTileSize != 0) {

        if (Character::get_character_y() % kTileSize < kTileSize / 2) {
            Character::set_character_status(WALK_DOWN);
        } else {
            Character::set_character_status(WALK_UP);
        }

    } else {

        int new_collision_map[ kScene_Area ]; // 0表示空虛, 1表示障礙物, 2表示玩家 // 0 for emptiness, 1 for obstacle, 2 for players

        vector<int> tiles;

        list<Character *>::iterator it;

        //我創建了一個新的碰撞圖, 其中也反映了 bug 佔用的圖塊
        // I create a new map of collisions in which are also reflected the tiles occupied by bugs
        for (i = 0; i < kScene_Area; ++i) { new_collision_map[ i ] = collision_map[ i ]; }

        for (it = enemies.begin(); it != enemies.end(); it++) {

            if ((*it) != this) {
                tiles = (*it)->getTiles();

                for (i = 0; i < tiles.size(); ++i) {
                    new_collision_map[ tiles[ i ] ] = 1;
                }
            }
        }

        tiles = p1->getTiles();
        for (i = 0; i < tiles.size(); ++i) { new_collision_map[ tiles[ i ] ] = 2; }

        tiles = p2->getTiles();
        for (i = 0; i < tiles.size(); ++i) { new_collision_map[ tiles[ i ] ] = 2; }

        // * WALKUP

        int best_status = WALK_UP;

        float min_dist = sqrt(pow(Character::get_character_x() - p1->Character::get_character_x(), 2.0f) +
                              pow((Character::get_character_y() + kTileSize) - p1->Character::get_character_y(), 2.0f));

        float dist = sqrt(pow(Character::get_character_x() - p2->Character::get_character_x(), 2.0f) +
                          pow((Character::get_character_y() + kTileSize) - p2->Character::get_character_y(), 2.0f));

        if (dist < min_dist) { min_dist = dist, best_status = WALK_UP; }

        //  * WALKDOWN

        dist = sqrt(pow(Character::get_character_x() - p1->Character::get_character_x(), 2.0f) +
                    pow((Character::get_character_y() - kTileSize) - p1->Character::get_character_y(), 2.0f));

        if (dist < min_dist) { min_dist = dist, best_status = WALK_DOWN; }

        dist = sqrt(
            pow(Character::get_character_x() - p2->Character::get_character_x(), 2.0f) +
            pow((Character::get_character_y() - kTileSize) - p2->Character::get_character_y(), 2.0f));

        if (dist < min_dist) { min_dist = dist, best_status = WALK_DOWN; }

        // * WALKRIGHT

        dist = sqrt(pow((Character::get_character_x() + kTileSize) - p1->Character::get_character_x(), 2.0f) +
                    pow(Character::get_character_y() - p1->Character::get_character_y(), 2.0f));

        if (dist < min_dist) { min_dist = dist, best_status = WALK_RIGHT; }

        dist = sqrt(pow((Character::get_character_x() + kTileSize) - p2->Character::get_character_x(), 2.0f) +
                    pow(Character::get_character_y() - p2->Character::get_character_y(), 2.0f));

        if (dist < min_dist) { min_dist = dist, best_status = WALK_RIGHT; }

        // * WALKLEFT

        dist = sqrt(pow((Character::get_character_x() - kTileSize) - p1->Character::get_character_x(), 2.0f) +
                    pow(Character::get_character_y() - p1->Character::get_character_y(), 2.0f));

        if (dist < min_dist) { min_dist = dist, best_status = WALK_LEFT; }

        dist = sqrt(pow((Character::get_character_x() - kTileSize) - p2->Character::get_character_x(), 2.0f) +
                    pow(Character::get_character_y() - p2->Character::get_character_y(), 2.0f));

        if (dist < min_dist) { min_dist = dist, best_status = WALK_LEFT; }

        // *

        // 如果可以實現的話, 決定到達玩家的最短路線方向
        // determine by what direction is the shortest way to reach a player, if it is achievable
        vector<int> occupied_tiles = getTiles();
        vector<int> choices(0);

        bool illegal_move = false;

        for (i = 0; i < occupied_tiles.size() && !illegal_move; ++i) {
            if (new_collision_map[ occupied_tiles[ i ] + kScene_W ]) { illegal_move = true; }
        }

        if (!illegal_move) {
            choices.push_back(WALK_UP);
        } else {
            illegal_move = false;
        }

        for (i = 0; i < occupied_tiles.size() && !illegal_move; ++i) {
            if (new_collision_map[ occupied_tiles[ i ] - kScene_W ]) { illegal_move = true; }
        }
        if (!illegal_move) {
            choices.push_back(WALK_DOWN);
        } else {
            illegal_move = false;
        }

        for (i = 0; i < occupied_tiles.size() && !illegal_move; ++i) {
            if (new_collision_map[ occupied_tiles[ i ] + 1 ]) { illegal_move = true; }
        }
        if (!illegal_move) {
            choices.push_back(WALK_RIGHT);
        } else {
            illegal_move = false;
        }

        for (i = 0; i < occupied_tiles.size() && !illegal_move; ++i) {
            if (new_collision_map[ occupied_tiles[ i ] - 1 ]) { illegal_move = true; }
        }
        if (!illegal_move) {
            choices.push_back(WALK_LEFT);
        } else {
            illegal_move = false;
        }

        if (choices.size()) {
            Character::set_character_status(choices[ rand() % choices.size() ]); // 隨機分配 // assign a random address from among the possible

            for (i = 0; i < choices.size(); ++i) {
                if (choices[ i ] == best_status) {
                    Character::set_character_status(best_status);
                }
            } // if one of those possible directions was the best, assign that

            // 沿著指定的方向移動 (如果有的話),  以區分我到達新圖塊的情況
            // start to move in the assigned direction, if there is one, to differentiate myself from the case in which I arrive to a new tile
            Move();
        }
    }

    return false;
}
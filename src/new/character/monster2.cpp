#include "monster2.hpp"

bool Monster2::Attack(list<Character *> &players, list<Projectile *> &projectiles, bool *projectile_hit_map) {

    if (Character::get_frame_seq() == 6 && Character::get_frame_delay() == 0) {

        int projectile_pos_x, projectile_pos_y;

        projectiles.push_back(new Arrow);

        switch (Character::get_character_status()) {

            case ATTACK_DOWN: {

                projectile_pos_x = get_character_x() + (Character::get_base_tile_w() - projectiles.back()->Projectile::get_projectile_hit_w()) / 2;
                projectile_pos_y = get_character_y();

                projectiles.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_DOWN);
                projectiles.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                if (projectiles.back()->Projectile::ComputeEnemyCollisions(players) ||
                    projectiles.back()->Projectile::HitMapAtUpOrDown(projectile_hit_map, false)) {

                    projectiles.back()->Projectile::set_projectile_status(PROJECTILE_HIT_DOWN);
                }

                break;
            }

            case ATTACK_UP: {

                projectile_pos_x = get_character_x() + (Character::get_base_tile_w() - projectiles.back()->Projectile::get_projectile_hit_w()) / 2;
                projectile_pos_y = get_character_y() + Character::get_base_tile_h();

                projectiles.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_UP);
                projectiles.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                // 拋射物是否擊中 // evaluate if the projectile appears on top of an enemy or object
                if (projectiles.back()->Projectile::ComputeEnemyCollisions(players) ||
                    projectiles.back()->Projectile::HitMapAtUpOrDown(projectile_hit_map, false) ||
                    projectiles.back()->Projectile::HitMapAtUpOrDown(projectile_hit_map, true)) {

                    projectiles.back()->Projectile::set_projectile_status(PROJECTILE_HIT_UP);
                }

                break;
            }

            case ATTACK_RIGHT: {

                projectile_pos_x = get_character_x() + Character::get_base_tile_w();
                projectile_pos_y = get_character_y() + (Character::get_base_tile_h() - projectiles.back()->Projectile::get_projectile_hit_h()) / 2;

                projectiles.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_RIGHT);
                projectiles.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                if (projectiles.back()->Projectile::ComputeEnemyCollisions(players) ||
                    projectiles.back()->Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, false) ||
                    projectiles.back()->Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, true)) {

                    projectiles.back()->Projectile::set_projectile_status(PROJECTILE_HIT_RIGHT);
                }

                break;
            }

            case ATTACK_LEFT: {

                projectile_pos_x = get_character_x();
                projectile_pos_y = get_character_y() + (Character::get_base_tile_h() - projectiles.back()->Projectile::get_projectile_hit_h()) / 2;

                projectiles.back()->Projectile::set_projectile_status(PROJECTILE_FLYING_LEFT);
                projectiles.back()->Projectile::set_projectile_pos(projectile_pos_x, projectile_pos_y);

                if (projectiles.back()->Projectile::ComputeEnemyCollisions(players) ||
                    projectiles.back()->Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, false)) {

                    projectiles.back()->Projectile::set_projectile_status(PROJECTILE_HIT_LEFT);
                }

                break;
            }
        }

        return true;
    }

    return false;
}

bool Monster2::IA(bool *collision_map, bool *projectile_hit_map, list<Character *> &enemies, Character *p1, Character *p2) {

    Rect hitbox_p1 = p1->getHitBox();
    Rect hitbox_p2 = p2->getHitBox();

    Arrow cArrow;

    int arrow_x = get_character_x() + (Character::get_base_tile_w() - cArrow.get_projectile_hit_w()) / 2;
    int arrow_y = get_character_y() + (Character::get_base_tile_h() - cArrow.get_projectile_hit_h()) / 2;

    short int i = 0;

    bool collision = false;

    // 如果有任何玩家到達, 則分配相應的攻擊狀態 // if any player is reach, assign the corresponding attack status
    if (hitbox_p1.right > arrow_x && hitbox_p1.left < arrow_x + cArrow.get_projectile_hit_w()) {

        if (hitbox_p1.bottom > get_character_y()) {

            for (i = ((get_character_y() + Character::get_base_tile_h()) / kTileSize);
                 i <= (hitbox_p1.bottom / kTileSize) && !collision; ++i) {

                collision = projectile_hit_map[ (get_character_x() / kTileSize) + i * kScene_W ];
            }

            if (!collision) {

                Character::set_character_status(ATTACK_UP);
                Character::set_frame_seq(0);
                Character::set_frame_delay(0);

                return true;
            }

        } else {

            for (i = (hitbox_p1.bottom / kTileSize); i <= ((get_character_y() - Character::get_base_tile_h()) / kTileSize) && !collision; ++i) {
                collision = projectile_hit_map[ (get_character_x() / kTileSize) + i * kScene_W ];
            }

            if (!collision) {

                Character::set_character_status(ATTACK_DOWN);
                Character::set_frame_seq(0);
                Character::set_frame_delay(0);

                return true;
            }
        }

    } else if (hitbox_p1.top > arrow_y && hitbox_p1.bottom < arrow_y + cArrow.get_projectile_hit_h()) {

        if (hitbox_p1.left > get_character_x()) {

            for (i = ((get_character_x() + Character::get_base_tile_w()) / kTileSize); i <= (hitbox_p1.left / kTileSize) && !collision; ++i) {
                collision = projectile_hit_map[ i + (get_character_y() / kTileSize) * kScene_W ];
            }

            if (!collision) {

                Character::set_character_status(ATTACK_RIGHT);
                Character::set_frame_seq(0);
                Character::set_frame_delay(0);

                return true;
            }

        } else {

            for (i = (hitbox_p1.left / kTileSize); i <= ((get_character_x() - Character::get_base_tile_w()) / kTileSize) && !collision; ++i) {
                collision = projectile_hit_map[ (get_character_x() / kTileSize) + i * kScene_W ];
            }

            if (!collision) {

                Character::set_character_status(ATTACK_LEFT);
                Character::set_frame_seq(0);
                Character::set_frame_delay(0);

                return true;
            }
        }

    } else if (hitbox_p2.right > arrow_x && hitbox_p2.left < arrow_x + cArrow.get_projectile_hit_w()) {

        if (hitbox_p2.bottom > get_character_y()) {

            for (i = ((get_character_y() + Character::get_base_tile_h()) / kTileSize); i <= (hitbox_p2.bottom / kTileSize) && !collision; ++i) {
                collision = projectile_hit_map[ (get_character_x() / kTileSize) + i * kScene_W ];
            }

            if (!collision) {

                Character::set_character_status(ATTACK_UP);
                Character::set_frame_seq(0);
                Character::set_frame_delay(0);

                return true;
            }

        } else {

            for (i = (hitbox_p2.bottom / kTileSize); i <= ((get_character_y() - Character::get_base_tile_h()) / kTileSize) && !collision; ++i) {
                collision = projectile_hit_map[ (get_character_x() / kTileSize) + i * kScene_W ];
            }

            if (!collision) {
                Character::set_character_status(ATTACK_DOWN);
                Character::set_frame_seq(0);
                Character::set_frame_delay(0);

                return true;
            }
        }

    } else if (hitbox_p2.top > arrow_y && hitbox_p2.bottom < arrow_y + cArrow.get_projectile_hit_h()) {

        if (hitbox_p2.left > get_character_x()) {

            for (i = ((get_character_x() + Character::get_base_tile_w()) / kTileSize); i <= (hitbox_p2.left / kTileSize) && !collision; ++i) {
                collision = projectile_hit_map[ i + (get_character_y() / kTileSize) * kScene_W ];
            }

            if (!collision) {

                Character::set_character_status(ATTACK_RIGHT);
                Character::set_frame_seq(0);
                Character::set_frame_delay(0);

                return true;
            }

        } else {

            for (i = (hitbox_p2.left / kTileSize); i <= ((get_character_x() - Character::get_base_tile_w()) / kTileSize) && !collision; ++i) {
                collision = projectile_hit_map[ (get_character_x() / kTileSize) + i * kScene_W ];
            }

            if (!collision) {

                Character::set_character_status(ATTACK_LEFT);
                Character::set_frame_seq(0);
                Character::set_frame_delay(0);

                return true;
            }
        }
    }

    // 如果無法攻擊, 請計算路徑並指定相應的移動狀態
    // 如果他們在你移動時攻擊你, 你可能已經從瓷磚的邊緣移位了, 有必要重新安置它
    //
    // If you can't attack, calculate the path and assign the corresponding status of movement
    // if they attack you while you are moving, you may have been displaced from the margins of the tile, it is necessary to relocate it
    if (get_character_x() % kTileSize != 0) {

        if (get_character_x() % kTileSize < kTileSize / 2) {
            Character::set_character_status(WALK_LEFT);
        } else {
            Character::set_character_status(WALK_RIGHT);
        }

    } else if (get_character_y() % kTileSize != 0) {

        if (get_character_y() % kTileSize < kTileSize / 2) {
            Character::set_character_status(WALK_DOWN);
        } else {
            Character::set_character_status(WALK_UP);
        }

    } else {

        int new_collision_map[ kScene_Area ]; // 0表示空虛, 1表示障礙物, 2表示玩家 // 0 for emptiness, 1 for obstacle, 2 for players

        vector<int> tiles;

        list<Character *>::iterator it;

        // 創建了一個新的碰撞圖, 它也顯示了bug佔用的圖塊
        // created a new collision map which also shows the tiles occupied by bugs
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

        // 確定到達玩家的最短路線的方向, 如果可以實現的話
        // determine by what direction is the shortest way to reach a player, if it is achievable
        vector<int> occupied_tiles = getTiles();
        vector<int> choices(0);

        bool illegal_move = false;

        int best_state = WALK_RIGHT;

        // * WALKRIGHT

        float min_dist = abs((get_character_x() + kTileSize) - p1->get_character_x());
        float dist     = abs((get_character_x() + kTileSize) - p2->get_character_x());
        if (dist < min_dist) { min_dist = dist, best_state = WALK_RIGHT; }

        // * WALKDOWN

        dist = abs((get_character_y() - kTileSize) - p1->get_character_y());
        if (dist < min_dist) { min_dist = dist, best_state = WALK_DOWN; }
        dist = abs((get_character_y() - kTileSize) - p2->get_character_y());
        if (dist < min_dist) { min_dist = dist, best_state = WALK_DOWN; }

        // * WALKUP

        dist = abs((get_character_y() + kTileSize) - p1->get_character_y());
        if (dist < min_dist) { min_dist = dist, best_state = WALK_UP; }
        dist = abs((get_character_y() + kTileSize) - p2->get_character_y());
        if (dist < min_dist) { min_dist = dist, best_state = WALK_UP; }

        // * WALKLEFT

        dist = abs((get_character_x() - kTileSize) - p1->get_character_x());
        if (dist < min_dist) { min_dist = dist, best_state = WALK_LEFT; }
        dist = abs((get_character_x() - kTileSize) - p2->get_character_x());
        if (dist < min_dist) { min_dist = dist, best_state = WALK_LEFT; }

        // *

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
            Character::set_character_status(choices[ rand() % choices.size() ]);

            for (i = 0; i < choices.size(); ++i) {
                if (choices[ i ] == best_state) {
                    Character::set_character_status(best_state);
                }
            }

            // 沿著指定的方向移動 (如果有的話),  以區分我到達新圖塊的情況
            // I start to move in the assigned direction, if there is one, to differentiate myself from the case in which I arrive to a new tile
            Move();
        }
    }

    return false;
}
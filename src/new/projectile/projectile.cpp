#include "projectile.hpp"

void Projectile::set_projectile_pos(const int &posx, const int &posy) { projectile_x_ = posx, projectile_y_ = posy; }

int Projectile::get_character_x() { return projectile_x_; }
int Projectile::get_character_y() { return projectile_y_; }

int Projectile::get_projectile_hit_w() { return projectile_hit_w_; }
int Projectile::get_projectile_hit_h() { return projectile_hit_h_; }

bool Projectile::HitMapAtUpOrDown(bool *projectile_hit_map, const bool &up) {

    int tile_x = projectile_x_ / kTileSize;
    int tile_y;

    (up && ((projectile_y_ + projectile_hit_h_) / kTileSize) < kScene_H)
        ? tile_y = (projectile_y_ + projectile_hit_h_) / kTileSize
        : tile_y = (projectile_y_) / kTileSize;

    int width_tiles = (projectile_hit_w_ - 1) / kTileSize + 1;

    if (projectile_x_ % kTileSize + projectile_hit_w_ > kTileSize) {
        ++width_tiles;
    }

    for (int j = 0; j < width_tiles; ++j) {
        if (projectile_hit_map[ (tile_x + j) + tile_y * kScene_W ]) {
            return true;
        }
    }

    return false;
}

bool Projectile::CheckHitMapAtLeftOrRight(bool *projectile_hit_map, const bool &right) {

    int tile_x;
    int tile_y = projectile_y_ / kTileSize;

    (right && ((projectile_x_ + projectile_hit_w_) / kTileSize) < kScene_W)
        ? tile_x = (projectile_x_ + projectile_hit_w_) / kTileSize
        : tile_x = projectile_x_ / kTileSize;

    int height_tiles = (projectile_hit_h_ - 1) / kTileSize + 1;
    if ((projectile_y_ % kTileSize + projectile_hit_h_) > kTileSize) {
        ++height_tiles;
    }

    for (int j = 0; j < height_tiles; ++j) {
        if (projectile_hit_map[ tile_x + (tile_y + j) * kScene_W ]) {
            return true;
        }
    }

    return false;
}

void Projectile::LevitateUp(bool *projectile_hit_map) {

    if ((projectile_y_ % kTileSize) == 0) {

        int tmp_y = projectile_y_;

        projectile_y_ += projectile_step_length_;

        if (Projectile::HitMapAtUpOrDown(projectile_hit_map, true)) {

            projectile_y_      = tmp_y;
            projectile_status_ = PROJECTILE_HIT_UP; // if it hits something, it will explode
            seq                = 0;
            delay              = 0;
        }

    } else {

        projectile_y_ += projectile_step_length_;
    }
}

void Projectile::LevitateDown(bool *projectile_hit_map) {

    if ((projectile_y_ % kTileSize) == 0) {

        int tmp_y = projectile_y_;

        projectile_y_ -= projectile_step_length_;

        if (Projectile::HitMapAtUpOrDown(projectile_hit_map, false)) {

            projectile_y_      = tmp_y;
            projectile_status_ = PROJECTILE_HIT_DOWN; // if it hits something, it will explode
            seq                = 0;
            delay              = 0;
        }

    } else {

        projectile_y_ -= projectile_step_length_;
    }
}

void Projectile::LevitateLeft(bool *projectile_hit_map) {

    if ((projectile_x_ % kTileSize) == 0) {

        int xaux = projectile_x_;

        projectile_x_ -= projectile_step_length_;

        if (Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, false)) {

            projectile_x_      = xaux;
            projectile_status_ = PROJECTILE_HIT_LEFT; // if it hits something, it will explode
            seq                = 0;
            delay              = 0;
        }

    } else {

        projectile_x_ -= projectile_step_length_;
    }
}
void Projectile::LevitateRight(bool *projectile_hit_map) {

    if ((projectile_x_ % kTileSize) == 0) {

        int xaux = projectile_x_;

        projectile_x_ += projectile_step_length_;

        if (Projectile::CheckHitMapAtLeftOrRight(projectile_hit_map, true)) {

            projectile_x_      = xaux;
            projectile_status_ = PROJECTILE_HIT_RIGHT; // if it hits something, it will explode
            seq                = 0;
            delay              = 0;
        }

    } else {

        projectile_x_ += projectile_step_length_;
    }
}

bool Projectile::ComputeEnemyCollisions(list<Character *> &enemies) {

    bool collision = false;

    list<Character *>::iterator it;
    for (it = enemies.begin(); it != enemies.end(); ++it) {
        if ((*it)->get_character_status() != DYING && Intersection(getHitBox(), (*it)->getHitBox())) {

            Player1 *P1 = dynamic_cast<Player1 *>(*it);

            if (P1 != NULL && P1->IsDoingSkill()) {

                switch (projectile_status_) {

                    case PROJECTILE_FLYING_UP:
                        P1->TestShieldProtection(ATTACK_UP, get_projectile_damage(), get_character_x(), get_character_y());
                    case PROJECTILE_FLYING_DOWN:
                        P1->TestShieldProtection(ATTACK_DOWN, get_projectile_damage(), get_character_x(), get_character_y());
                    case PROJECTILE_FLYING_LEFT:
                        P1->TestShieldProtection(ATTACK_LEFT, get_projectile_damage(), get_character_x(), get_character_y());
                    case PROJECTILE_FLYING_RIGHT:
                        P1->TestShieldProtection(ATTACK_RIGHT, get_projectile_damage(), get_character_x(), get_character_y());
                }

            } else {

                int new_health = (*it)->get_now_hp() - get_projectile_damage();

                if (new_health <= 0) {

                    (*it)->Character::set_character_status(DYING);

                } else if (dynamic_cast<Player *>(*it) == NULL || !dynamic_cast<Player *>(*it)->IsSuperAttacking()) {

                    switch (projectile_status_) {

                        case PROJECTILE_FLYING_UP: (*it)->Character::set_character_status(DAMAGE_DOWN); break;
                        case PROJECTILE_FLYING_DOWN: (*it)->Character::set_character_status(DAMAGE_UP); break;
                        case PROJECTILE_FLYING_LEFT: (*it)->Character::set_character_status(DAMAGE_RIGHT); break;
                        case PROJECTILE_FLYING_RIGHT: (*it)->Character::set_character_status(DAMAGE_LEFT); break;
                    }

                    (*it)->set_frame_seq(0);
                    (*it)->set_frame_delay(0);
                }

                if (new_health < 0) { new_health = 0; }
                (*it)->set_now_hp(new_health);
            }

            collision = true;
        }
    }

    return collision;
}
bool Projectile::Logic(bool *projectile_hit_map, list<Character *> &enemies) {

    if (!IsEnding()) {
        // if it hits an enemy, the ending animation will start
        if (ComputeEnemyCollisions(enemies)) {

            switch (projectile_status_) {

                case PROJECTILE_FLYING_UP: projectile_status_ = PROJECTILE_HIT_UP; break;
                case PROJECTILE_FLYING_DOWN: projectile_status_ = PROJECTILE_HIT_DOWN; break;
                case PROJECTILE_FLYING_RIGHT: projectile_status_ = PROJECTILE_HIT_RIGHT; break;
                case PROJECTILE_FLYING_LEFT: projectile_status_ = PROJECTILE_HIT_LEFT; break;
            }

            seq = 0, delay = 0;

            return true;

        } else {

            switch (projectile_status_) {

                case PROJECTILE_FLYING_UP: LevitateUp(projectile_hit_map); break;
                case PROJECTILE_FLYING_DOWN: LevitateDown(projectile_hit_map); break;
                case PROJECTILE_FLYING_LEFT: LevitateLeft(projectile_hit_map); break;
                case PROJECTILE_FLYING_RIGHT: LevitateRight(projectile_hit_map); break;
            }

            if (replace_over_) {
                LevitateUp(projectile_hit_map);
            } else if (replace_under_) {
                LevitateDown(projectile_hit_map);
            }

            if (IsEnding()) { return true; }
        }
    }

    return false;
}

void Projectile::set_projectile_status(const int &projectile_status) { projectile_status_ = projectile_status; }
int Projectile::get_projectile_status() { return projectile_status_; }

int Projectile::get_frame_seq() { return seq; }
int Projectile::get_frame_delay() { return delay; }
int Projectile::get_frame_texture_tile() { return frame_texture_tile_; }

int Projectile::get_projectile_damage() { return projectile_damage_; }
int Projectile::get_projectile_step_length() { return projectile_step_length_; }

void Projectile::set_replace_over(const bool &replace_over) { replace_over_ = replace_over; }
void Projectile::set_replace_under(const bool &replace_over) { replace_under_ = replace_over; }

void Projectile::NextFrame(const int &max) {

    ++delay;

    if (delay == frame_delay) {
        ++seq;
        seq %= max;
        delay = 0;
    }
}

bool Projectile::IsEnding() {

    if (projectile_status_ == PROJECTILE_HIT_UP ||
        projectile_status_ == PROJECTILE_HIT_DOWN ||
        projectile_status_ == PROJECTILE_HIT_RIGHT ||
        projectile_status_ == PROJECTILE_HIT_LEFT) {

        return true;
    } else {
        return false;
    }
}

bool Projectile::Intersection(Rect box1, Rect box2) {

    // 路口
    if (box1.top > box2.bottom &&
        box2.top > box1.bottom &&
        box1.left < box2.right &&
        box2.left < box1.right) {

        return true;
    }

    return false;
}

Rect Projectile::getFrameBox() {

    float frame_box = (kTileSize * frame_texture_tile_ - projectile_hit_h_) / 2;

    Rect FrameBox;

    FrameBox.bottom = frame_box - projectile_y_;
    FrameBox.top    = frame_box + projectile_y_ + projectile_hit_h_;
    FrameBox.left   = frame_box - projectile_x_;
    FrameBox.right  = frame_box + projectile_x_ + projectile_hit_w_;

    return FrameBox;
}
Rect Projectile::getHitBox() {

    Rect HitBox;

    HitBox.bottom = projectile_y_;
    HitBox.top    = projectile_y_ + projectile_hit_h_;
    HitBox.left   = projectile_x_;
    HitBox.right  = projectile_x_ + projectile_hit_w_;

    return HitBox;
}

void Projectile::DrawProjectile(const int &tex_id, const float &begin_tex_x, const float &begin_tex_y, const float &final_tex_x, const float &final_tex_y) {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glBegin(GL_POLYGON);

    glTexCoord2f(begin_tex_x, begin_tex_y);
    glVertex3i(projectile_x_ - (kTileSize * frame_texture_tile_ - projectile_hit_w_) / 2,
               projectile_y_ - (kTileSize * frame_texture_tile_ - projectile_hit_h_) / 2, projectile_y_);

    glTexCoord2f(final_tex_x, begin_tex_y);
    glVertex3i(projectile_x_ + (kTileSize * frame_texture_tile_ + projectile_hit_w_) / 2,
               projectile_y_ - (kTileSize * frame_texture_tile_ - projectile_hit_h_) / 2, projectile_y_);

    glTexCoord2f(final_tex_x, final_tex_y);
    glVertex3i(projectile_x_ + (kTileSize * frame_texture_tile_ + projectile_hit_w_) / 2,
               projectile_y_ + (kTileSize * frame_texture_tile_ + projectile_hit_h_) / 2, projectile_y_);

    glTexCoord2f(begin_tex_x, final_tex_y);
    glVertex3i(projectile_x_ - (kTileSize * frame_texture_tile_ - projectile_hit_w_) / 2,
               projectile_y_ + (kTileSize * frame_texture_tile_ + projectile_hit_h_) / 2, projectile_y_);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void Arrow::OffsetProjectileTexture(const int &tex_id, const int &tex_w, const int &tex_h, const bool &run) {

    bool reverse_tex = false;

    float offset_tex_x = 1.0f / static_cast<float>(tex_w / (Arrow::get_frame_texture_tile() * kTextureTileSize));
    float offset_tex_y = 1.0f / static_cast<float>(tex_h / (Arrow::get_frame_texture_tile() * kTextureTileSize));

    float begin_tex_x = offset_tex_x * Arrow::get_frame_seq();
    float begin_tex_y = 0;

    switch (Arrow::get_projectile_status()) {

        case PROJECTILE_FLYING_DOWN:
            begin_tex_y = offset_tex_y * (0 + Arrow::get_frame_seq());
            break;
        case PROJECTILE_FLYING_UP:
            begin_tex_y = offset_tex_y * (1 + Arrow::get_frame_seq());
            break;
        case PROJECTILE_FLYING_RIGHT:
            begin_tex_y = offset_tex_y * (2 + Arrow::get_frame_seq());
            break;
        case PROJECTILE_FLYING_LEFT:
            begin_tex_y = offset_tex_y * (2 + Arrow::get_frame_seq());

            reverse_tex = true;
            break;

        case PROJECTILE_HIT_DOWN:
            begin_tex_y = offset_tex_y * (0 + Arrow::get_frame_seq());
            Arrow::NextFrame(1);
            break;
        case PROJECTILE_HIT_UP:
            begin_tex_y = offset_tex_y * (1 + Arrow::get_frame_seq());
            Arrow::NextFrame(1);
            break;
        case PROJECTILE_HIT_RIGHT:
            begin_tex_y = offset_tex_y * (2 + Arrow::get_frame_seq());
            Arrow::NextFrame(1);
            break;
        case PROJECTILE_HIT_LEFT:
            begin_tex_y = offset_tex_y * (2 + Arrow::get_frame_seq());
            Arrow::NextFrame(1);

            reverse_tex = true;
            break;
    }

    float final_tex_x = begin_tex_x + offset_tex_x;
    float final_tex_y = begin_tex_y;

    begin_tex_y = begin_tex_y + offset_tex_y;

    float tmp;
    if (reverse_tex) { tmp = begin_tex_x, begin_tex_x = final_tex_x, final_tex_x = tmp; }

    Projectile::DrawProjectile(tex_id, begin_tex_x, begin_tex_y, final_tex_x, final_tex_y);
}

void FireBall::OffsetProjectileTexture(const int &tex_id, const int &tex_w, const int &tex_h, const bool &run) {

    bool reverse_tex = false;

    float offset_tex_x = 1.0f / static_cast<float>(tex_w / (FireBall::get_frame_texture_tile() * kTextureTileSize));
    float offset_tex_y = 1.0f / static_cast<float>(tex_h / (FireBall::get_frame_texture_tile() * kTextureTileSize));

    float begin_tex_x = offset_tex_x * FireBall::get_frame_seq();
    float begin_tex_y;

    switch (FireBall::get_projectile_status()) {
        case PROJECTILE_FLYING_DOWN:
            begin_tex_y = offset_tex_y * 1;
            if (run) { FireBall::NextFrame(3); }
            break;
        case PROJECTILE_FLYING_UP:
            begin_tex_y = offset_tex_y * 2;
            if (run) { FireBall::NextFrame(3); }
            break;
        case PROJECTILE_FLYING_RIGHT:
            begin_tex_y = offset_tex_y * 3;
            if (run) { FireBall::NextFrame(3); }
            break;
        case PROJECTILE_FLYING_LEFT:
            begin_tex_y = offset_tex_y * 3;
            if (run) { FireBall::NextFrame(3); }

            reverse_tex = true;
            break;
    }

    // 火球爆炸

    if (FireBall::get_projectile_status() == PROJECTILE_HIT_DOWN ||
        FireBall::get_projectile_status() == PROJECTILE_HIT_UP ||
        FireBall::get_projectile_status() == PROJECTILE_HIT_RIGHT ||
        FireBall::get_projectile_status() == PROJECTILE_HIT_LEFT) {

        (Projectile::get_frame_seq() <= 3)
            ? begin_tex_y = offset_tex_y * 4
            : begin_tex_y = offset_tex_y * 5;

        if (run) { FireBall::NextFrame(6); }
    }

    float final_tex_x = begin_tex_x + offset_tex_x;
    float final_tex_y = begin_tex_y - offset_tex_y;

    float tmp;
    if (reverse_tex) { tmp = begin_tex_x, begin_tex_x = final_tex_x, final_tex_x = tmp; }

    Projectile::DrawProjectile(tex_id, begin_tex_x, begin_tex_y, final_tex_x, final_tex_y);
}
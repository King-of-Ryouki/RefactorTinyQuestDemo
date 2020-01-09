#include "player_status_bar.hpp"

void PlayerStatusBar::set_player_status_bar_x(const int &player_status_bar_x) { player_status_bar_x_ = player_status_bar_x; }
int PlayerStatusBar::get_player_status_bar_x() { return player_status_bar_x_; }

void PlayerStatusBar::set_player_status_bar_y(const int &player_status_bar_y) { player_status_bar_y_ = player_status_bar_y; }
int PlayerStatusBar::get_player_status_bar_y() { return player_status_bar_y_; }

void PlayerStatusBar::set_player_status_bar_x_offset(const int &player_status_bar_x_offset) { player_status_bar_x_offset_ = player_status_bar_x_offset; }

int PlayerStatusBar::get_player_status_bar_w() { return player_status_bar_w_; }

void PlayerStatusBar::OverlayDraw(const int &texture_id, const int &texture_w, const int &texture_h) {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glBegin(GL_POLYGON);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3i(player_status_bar_x_,
               player_status_bar_y_,
               -2);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3i(player_status_bar_x_ + texture_w,
               player_status_bar_y_,
               -2);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3i(player_status_bar_x_ + texture_w,
               player_status_bar_y_ + texture_h,
               -2);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3i(player_status_bar_x_,
               player_status_bar_y_ + texture_h,
               -2);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void PlayerStatusBar::DrawBars(const int &player_max_hp, const int &player_now_hp, const int &player_max_mp, const int &player_now_mp) {

    float hp_percentage = static_cast<float>(player_now_hp) / player_max_hp;
    float hp_bar_length = hp_percentage *
                          (player_status_bar_w_ - (2.5 * kTileSize));
    // float hp_bar_length = hp_percentage * ((player_status_bar_w_ - 2.5) * kTileSize);

    // hp green ==> yellow ==> red
    (hp_percentage >= 0.5f) ? glColor3f((1.0f - hp_percentage) * 2, 1.0f, 0.0f)
                            : glColor3f(1.0f, hp_percentage * 2, 0.0f);

    glBegin(GL_POLYGON);

    glVertex3i(player_status_bar_x_ + player_status_bar_x_offset_, player_status_bar_y_ + 5 * (kTileSize / 8), -1);
    glVertex3i(player_status_bar_x_ + player_status_bar_x_offset_ + hp_bar_length, player_status_bar_y_ + 5 * (kTileSize / 8), -1);
    glVertex3i(player_status_bar_x_ + player_status_bar_x_offset_ + hp_bar_length, player_status_bar_y_ + 7 * (kTileSize / 8), -1);
    glVertex3i(player_status_bar_x_ + player_status_bar_x_offset_, player_status_bar_y_ + 7 * (kTileSize / 8), -1);

    glEnd();

    float mp_bar_length = (static_cast<float>(player_now_mp) / player_max_mp) *
                          (player_status_bar_w_ - (2.5 * kTileSize));

    glColor3f(0.2f, 0.5f, 1.0f);
    glBegin(GL_POLYGON);

    glVertex3i(player_status_bar_x_ + player_status_bar_x_offset_, player_status_bar_y_ + 1 * (kTileSize / 8), -1);
    glVertex3i(player_status_bar_x_ + player_status_bar_x_offset_ + mp_bar_length, player_status_bar_y_ + 1 * (kTileSize / 8), -1);
    glVertex3i(player_status_bar_x_ + player_status_bar_x_offset_ + mp_bar_length, player_status_bar_y_ + 3 * (kTileSize / 8), -1);
    glVertex3i(player_status_bar_x_ + player_status_bar_x_offset_, player_status_bar_y_ + 3 * (kTileSize / 8), -1);

    glEnd();
    glColor4f(1, 1, 1, 1); // we return the color to white after drawing the bars (so as not to affect the textures)
}
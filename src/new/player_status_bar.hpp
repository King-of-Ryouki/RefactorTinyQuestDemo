#ifndef PlayerStatusBar_HPP
#define PlayerStatusBar_HPP

#include "./load/load_image.hpp"
#include "scene.hpp"

class PlayerStatusBar {
public:
    PlayerStatusBar(int player_status_bar_x_offset)
        : player_status_bar_x_offset_(player_status_bar_x_offset),
          player_status_bar_w_(96),
          player_status_bar_h_(4) {
    }

    void set_player_status_bar_x(const int &player_status_bar_x);
    int get_player_status_bar_x();

    void set_player_status_bar_y(const int &player_status_bar_y);
    int get_player_status_bar_y();

    int get_player_status_bar_w();

    void set_player_status_bar_x_offset(const int &player_status_bar_x_offset);

    void OverlayDraw(const int &texture_id, const int &texture_w, const int &texture_h);

    void DrawBars(const int &player_max_hp, const int &player_now_hp, const int &player_max_mp, const int &player_now_mp);

private:
    int player_status_bar_x_;
    int player_status_bar_y_;

    int player_status_bar_x_offset_;

    int player_status_bar_w_;
    int player_status_bar_h_;
};

#endif // PlayerStatusBar_HPP
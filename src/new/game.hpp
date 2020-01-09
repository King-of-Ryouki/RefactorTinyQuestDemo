#ifndef GAME_HPP
#define GAME_HPP

#include "_pch.hpp"

#include "player_status_bar.hpp"
#include "shader.hpp"

#include ".\character\monster1.hpp"
#include ".\character\monster2.hpp"
#include ".\character\player1.hpp"
#include ".\character\player2.hpp"

#include ".\load\load_image.hpp"
#include ".\load\load_sound.hpp"

#include ".\item\item.hpp"
#include ".\projectile\projectile.hpp"

class Game {
public:
    Game() {}
    virtual ~Game() {}

    void Reshape(const int &w, const int &h);
    bool GameLevelInit(const int &game_level);
    bool Loop();

    // input
    void ReadKeyboard(unsigned char key, const int &x, const int &y, const bool &press);
    void ReadSpecialKeyboard(const unsigned char &key, const int &x, const int &y, const bool &press);
    void ReadMouse(const int &button, const int &status, const int &x, const int &y) {}

    // process
    bool Process();

    // output
    void Render();

private:
    void UpdateCamera(const int &h1, const int &h2);
    bool LoadDynamicLayer(const int &game_level);

    // 初始窗口的尺寸 // dimensions of the initial window
    int game_w_ = kTileSize * (kScene_W - 2);
    int game_h_ = static_cast<int>(game_w_ * 1.5);

    // Game States
    const int game_runing_       = 1;
    const int game_gameover_     = 2;
    const int game_endgame_      = 3;
    const int game_level_change_ = 4;
    const int game_total_levels_ = 2;

    // Players control
    char p1_up_     = 'i';
    char p1_down_   = 'k';
    char p1_left_   = 'j';
    char p1_right_  = 'l';
    char p1_attack_ = 'z';
    char p1_skill_  = 'x';

    char p2_up_     = GLUT_KEY_UP;
    char p2_down_   = GLUT_KEY_DOWN;
    char p2_left_   = GLUT_KEY_LEFT;
    char p2_right_  = GLUT_KEY_RIGHT;
    char p2_attack_ = '*';
    char p2_skill_  = '-';

    unsigned char keys_[ 256 ];

    float time_;

    int game_status_, game_level_;

    bool p1_attacks_, p2_attacks_;
    bool ia, epilepsia_mode;

    list<Character *> enemies;
    list<Projectile *> projectiles;
    list<FireBall *> cFireBall;
    list<BaseOfItem *> tokens;
};

// Debug: click on left bar to add breakpoint, F11 to execute line to line
// control + alt + Q and in expression put an object or this to "listen" to its variables

// 調試：單擊左側欄添加斷點，單擊F11執行行到行
// control + alt + Q 和表達式中的一個對像或者這個"監聽"它的變量

#endif // GAME_HPP
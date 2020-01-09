#include "game.hpp"

Scene cScene;
Sound cSound;
GetImage getImg;
Shader cShader;

Player1 P1(150, 150, 0);
Player2 P2(100, 150, 0);

Rect VisibleArea;

PlayerStatusBar PlayerStatusBar_1(32);
PlayerStatusBar PlayerStatusBar_2(8);

void Game::Reshape(const int &w, const int &h) { glViewport(0, 0, w, h); } // 視窗大小 // w and h are the dimensions of the game window

bool Game::GameLevelInit(const int &game_level) {

    game_status_ = game_runing_;
    time_        = 1.0f;

    ia             = true;
    epilepsia_mode = false;

    // 圖形初始化 // graphics initialization
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    VisibleArea.left   = kTileSize;
    VisibleArea.right  = kTileSize + game_w_;
    VisibleArea.top    = kTileSize + game_h_;
    VisibleArea.bottom = kTileSize;

    // 當調用 glVertex 時, 每個對象的深度由其z值定義, 視野ira從z = 0（近）到kSceneHeight * kTileSize（遠）
    // the depth of each object is defined by its z value when making calls to glVertex, the field of vision ira from z = 0 (near) to kSceneHeight * kTileSize (far)

    // 最後2個值是近和遠, 3是條/生命, 界面和菜單的疊加 -2 是地形的 z, 範圍 : [-3, （kSceneHeight * kTileSize） + 2]
    // last 2 values are near and far, 3 is for the overlays of bars / lives, interface and menu, -2 is for the z of the terrain, range: [-3, (kSceneHeight * kTileSize) +2]
    glOrtho(VisibleArea.left, VisibleArea.right, VisibleArea.bottom, VisibleArea.top, 3, -kScene_H * kTileSize - 2 - 1);

    glMatrixMode(GL_MODELVIEW);
    glAlphaFunc(GL_GREATER, 0.05f);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);

    // 材質初始化 // texture initialization
    getImg.InitImages();

    int tex_w;
    int tex_h;

    // 地形初始化 // terrain initialization
    getImg.set_img_size(IMG_TILESET, &tex_w, &tex_h);

    bool res = true;

    game_level_ = game_level;
    res         = cScene.LoadLevel(game_level_, tex_w, tex_h);
    if (!res) { return false; }

    cScene.ComputeCollisionMaps();

    // overlays initialization
    PlayerStatusBar_1.set_player_status_bar_x(VisibleArea.left);
    PlayerStatusBar_1.set_player_status_bar_y(VisibleArea.bottom);

    PlayerStatusBar_2.set_player_status_bar_x(VisibleArea.right - PlayerStatusBar_2.get_player_status_bar_w());
    PlayerStatusBar_2.set_player_status_bar_y(VisibleArea.bottom);

    LoadDynamicLayer(game_level); // player, enemies and token initialization

    cSound.Load();  // sound initialization
    cShader.Load(); // shader initialization

    switch (game_level) {

        case 1: cSound.PlaySound(SOUND_AMBIENT_1); break;
        case 2: cSound.PlaySound(SOUND_AMBIENT_2); break;
    }

    return res;
}

bool Game::Loop() {

    bool res = true;

    int t1 = glutGet(GLUT_ELAPSED_TIME);
    int t2;

    if (game_status_ == game_runing_) {
        res = Process();

        if (res) { Render(); }
    }

    if (game_status_ == game_level_change_) {

        cSound.StopAllSound();
        cSound.PlaySound(SOUND_VICTORY);

        do {
            t2 = glutGet(GLUT_ELAPSED_TIME);

            if ((t2 - t1 - 3000) > 0) { time_ = static_cast<float>(t2 - t1 - 3000) / static_cast<float>(6000 - 3000); }

            Render();

        } while (t2 - t1 < 6000);

        list<Character *>::iterator it_e;
        for (it_e = enemies.begin(); it_e != enemies.end(); ++it_e) {
            delete (*it_e);
        }
        enemies.clear();

        list<Projectile *>::iterator it_p;
        for (it_p = projectiles.begin(); it_p != projectiles.end(); ++it_p) {
            delete (*it_p);
        }
        projectiles.clear();

        list<BaseOfItem *>::iterator it_t;
        for (it_t = tokens.begin(); it_t != tokens.end(); ++it_t) {
            delete (*it_t);
        }
        tokens.clear();

        GameLevelInit(game_level_ + 1);

        game_status_ = game_runing_;
    }

    if (game_status_ == game_gameover_) {

        cSound.StopAllSound();
        cSound.PlaySound(SOUND_GAMEOVER);

        do {

            t2 = glutGet(GLUT_ELAPSED_TIME);

            time_ = static_cast<float>(t2 - t1) / 3000;

            if (time_ > 1) { time_ = 1.0f; }

            if ((t2 - t1 - 7000) > 0) {
                time_ += static_cast<float>(t2 - t1 - 7000) / (10700 - 7000);
            }

            Render();

        } while (t2 - t1 < 10700);

        res = false;
    }

    if (game_status_ == game_endgame_) {
        cSound.StopAllSound();
        cSound.PlaySound(SOUND_VICTORY);

        do {
            t2 = glutGet(GLUT_ELAPSED_TIME);

            if ((t2 - t1 - 3000) > 0) {
                time_ = static_cast<float>(t2 - t1 - 3000) / (6000 - 3000);
            }

            Render();

        } while (t2 - t1 < 6000);

        res = false;
    }

    do {
        t2 = glutGet(GLUT_ELAPSED_TIME);
    } while (t2 - t1 < 1000 / 30); //30 fps = 1000/30

    return res;
}

// input
void Game::ReadKeyboard(unsigned char key, const int &x, const int &y, const bool &press) {

    if (key >= 'A' && key <= 'Z') { key += 32; }

    // 斜走, 放開移動鍵, 變更角色 frames // when you stop pressing a side direction key when moving diagonally, the player's frames may change
    if (!press && (key == p1_left_ || key == p1_right_) && !P1.IsAttacking()) { P1.StopP1(); }

    // 放開技能鍵, 變更角色 frames // when you stop pressing the skill key, the player's frames change
    if (!press && key == p1_skill_) { P1.StopDoingSkillP1(); }
    if (!press && key == p2_skill_) { P2.StopDoingSkill(); }

    // only an attack is recognized if the attack key is deactivated after being pressed and the skill key was not being pressed
    if (!press && key == p1_attack_ && keys_[ p1_attack_ ] && !keys_[ p1_skill_ ]) { p1_attacks_ = true; }
    if (!press && key == p2_attack_ && keys_[ p2_attack_ ] && !keys_[ p2_skill_ ]) { p2_attacks_ = true; }

    // at the moment the skill key is pressed the load is lostinput_
    if (press && key == p1_skill_) { P1.set_skill_cost(0); }
    if (press && key == p2_skill_) { P2.set_skill_cost(0); }

    if (key != GLUT_KEY_UP && key != GLUT_KEY_DOWN && key != GLUT_KEY_LEFT && key != GLUT_KEY_RIGHT) { keys_[ key ] = press; }
}

void Game::ReadSpecialKeyboard(const unsigned char &key, const int &x, const int &y, const bool &press) {
    // 斜走, 放開移動鍵, 變更角色 frames // when you stop pressing a side direction key when moving diagonally, the player's frames may change
    if (!press && (key == p2_left_ || key == p2_right_) && !P2.IsAttacking()) { P2.Stop(); }

    if (key == GLUT_KEY_UP || key == GLUT_KEY_DOWN || key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT) { keys_[ key ] = press; }
    if (key == GLUT_KEY_F1 && press) { ia = !ia; }
    if (key == GLUT_KEY_F10 && press) { epilepsia_mode = !epilepsia_mode; }
    if (key == GLUT_KEY_F10 && !press) { time_ = 0.0f; }
}

//Process
bool Game::Process() {

    bool res        = true;
    bool stand      = true;
    bool p1_defends = false;

    int h1 = P1.get_character_y();
    int h2 = P2.get_character_y();

    // process input
    if (keys_[ 27 ]) { res = false; }

    if (!P1.IsAttacking() &&
        !P1.IsDamaged()) {

        int character_status = P1.get_character_status();

        bool doing_skill = P1.IsDoingSkill();

        // when a skill is active, the status stays in a fixed direction so instead of the status attribute, use the logic_state of P1 to calculate the movement and the slide
        if (doing_skill) { P1.set_character_status(P1.get_logic_state()); }

        if (keys_[ p1_up_ ]) {

            P1.RestrictedMoveUp(cScene.get_collision_map(), enemies, h1, h2, game_h_);
            stand = false;

        } else if (keys_[ p1_down_ ]) {

            P1.RestrictedMoveDown(cScene.get_collision_map(), enemies, h1, h2, game_h_, VisibleArea.bottom);
            stand = false;
        }

        if (keys_[ p1_left_ ]) {

            bool vertical = keys_[ p1_up_ ] || keys_[ p1_down_ ];

            P1.MoveLeft(cScene.get_collision_map(), enemies, vertical);

            stand = false;

        } else if (keys_[ p1_right_ ]) {

            bool vertical = keys_[ p1_up_ ] || keys_[ p1_down_ ];

            P1.MoveRight(cScene.get_collision_map(), enemies, vertical);

            stand = false;
        }

        P1.set_logic_status(P1.get_character_status()); // after the possible movements I updated the logic_state of Player 1 in case it was necessary to use it in the next iteration of the gameloop

        if (!doing_skill) { character_status = P1.get_character_status(); } // in this case imput_state will never be a status of skill

        if (keys_[ p1_skill_ ]) {

            (P1.get_now_mp() > 0)
                ? P1.set_shield_state(character_status)
                : P1.StopDoingSkillP1();

        } else if (keys_[ p1_attack_ ]) {

            P1.set_skill_cost(P1.get_skill_cost() + 1);

            if (P1.get_skill_cost() == CHARGE_BREAK) {
                cSound.PlaySound(SOUND_CHARGED);
            }
        }

        if (stand) { P1.StopP1(); } // if I do not do anything, I show a status LOOK
        stand = true;
    }

    if (keys_[ p1_skill_ ]) { p1_defends = true; }

    if (!P2.IsAttacking() && !P2.IsDamaged()) {
        if (keys_[ p2_up_ ]) {

            P2.RestrictedMoveUp(cScene.get_collision_map(), enemies, h2, h1, game_h_);
            stand = false;

        } else if (keys_[ p2_down_ ]) {

            P2.RestrictedMoveDown(cScene.get_collision_map(), enemies, h2, h1, game_h_, VisibleArea.bottom);
            stand = false;
        }

        if (keys_[ p2_left_ ]) {

            bool vertical = keys_[ p2_up_ ] || keys_[ p2_down_ ];

            P2.MoveLeft(cScene.get_collision_map(), enemies, vertical);

            stand = false;

        } else if (keys_[ p2_right_ ]) {

            bool vertical = keys_[ p2_up_ ] || keys_[ p2_down_ ];

            P2.MoveRight(cScene.get_collision_map(), enemies, vertical);

            stand = false;
        }

        if (keys_[ p2_skill_ ]) {

            if (P2.get_now_mp() < P2.get_max_mp()) {

                P2.set_now_mp(P2.get_now_mp() + 1);

                if (!P2.IsDoingSkill()) { P2.TransformIntoSkillState(); }

            } else {

                P2.StopDoingSkill();
            }

        } else if (keys_[ p2_attack_ ]) {

            P2.set_skill_cost(P2.get_skill_cost() + 1);

            if (P2.get_skill_cost() == CHARGE_BREAK) {
                cSound.PlaySound(SOUND_CHARGED);
            }
        }

        if (stand) { P2.Stop(); } // if I do not do anything, I show a status LOOK
    }

    // game logic
    list<Character *> players;

    list<Projectile *>::iterator it_p;
    list<Character *>::iterator it_e;
    list<BaseOfItem *>::iterator it_t;

    players.push_back(&P1);
    players.push_back(&P2);

    // clean used shots, defeated enemies and finished states
    for (it_p = projectiles.begin(); it_p != projectiles.end();) {
        if ((*it_p)->IsEnding() && (*it_p)->get_frame_seq() == 0 && (*it_p)->get_frame_delay() == 0) {

            delete (*it_p); // released the dynamic memory assigned by the new one that created the projectile

            it_p = projectiles.erase(it_p); // deleted the pointer to the projectile and updated the iterator

        } else {

            Rect framebox = (*it_p)->getFrameBox();

            if (framebox.left >= VisibleArea.right ||
                framebox.right <= VisibleArea.left ||
                framebox.bottom >= VisibleArea.top ||
                framebox.top <= VisibleArea.bottom) { //si sale del campo de vision lo borro

                delete (*it_p);                 // released the dynamic memory assigned by the new one that created the projectile
                it_p = projectiles.erase(it_p); // deleted the pointer to the projectile and updated the iterator

            } else {

                ++it_p;
            }
        }
    }

    for (it_e = enemies.begin(); it_e != enemies.end();) {

        if ((*it_e)->get_character_status() == DYING &&
            (*it_e)->get_frame_seq() == 0 &&
            (*it_e)->get_frame_delay() == 0) {

            delete (*it_e);             // released the dynamic memory assigned by the new one that created the enemy
            it_e = enemies.erase(it_e); // deleted the pointer to the enemy and updated the iterator

        } else if (dynamic_cast<Enemy *>(*it_e)->get_frame_top() < VisibleArea.bottom) { // if it leaves the field of vision, erase it

            delete (*it_e);
            it_e = enemies.erase(it_e);

        } else {

            if (((*it_e)->IsAttacking() || (*it_e)->IsDamaged()) &&
                (*it_e)->get_frame_seq() == 0 &&
                (*it_e)->get_frame_delay() == 0) {

                (*it_e)->Stop();
            }

            if ((*it_e)->IsWalking() &&
                (*it_e)->get_character_x() % kTileSize == 0 &&
                (*it_e)->get_character_y() % kTileSize == 0) {

                (*it_e)->Stop();
            }

            ++it_e;
        }
    }

    if ((P1.IsAttacking() || P1.IsDamaged()) && P1.get_frame_seq() == 0 && P1.get_frame_delay() == 0) { P1.StopP1(); }
    if ((P2.IsAttacking() || P2.IsDamaged()) && P2.get_frame_seq() == 0 && P2.get_frame_delay() == 0) { P2.Stop(); }

    // enemy logic
    if (!P1.IsAttacking() &&
        !P1.IsDamaged() &&
        P1.get_atk_cd() == 0 &&
        p1_attacks_) {

        if (P1.get_skill_cost() >= CHARGE_BREAK) {
            if (P1.get_now_mp() >= SUPERATTACK_COST) {

                P1.StartSuperAttack();
                P1.set_now_mp(P1.get_now_mp() - SUPERATTACK_COST);

                cSound.PlaySound(SOUND_SUPERSLASH);

            } else {

                cSound.PlaySound(SOUND_FAIL);
            }

        } else {

            if (P1.get_now_mp() >= ATTACK_COST) {

                P1.StartAttack();
                P1.set_now_mp(P1.get_now_mp() - ATTACK_COST);

                cSound.PlaySound(SOUND_SLASH);

            } else {

                cSound.PlaySound(SOUND_FAIL);
            }
        }

        P1.set_skill_cost(0);
        P1.set_atk_cd(COOLDOWN);

        p1_attacks_ = false;
    }

    if (!P2.IsAttacking() &&
        !P2.IsDamaged() &&
        P2.get_atk_cd() == 0 &&
        p2_attacks_) {

        if (P2.get_skill_cost() >= CHARGE_BREAK) {
            if (P2.get_now_mp() >= ATTACK_COST) {

                P2.StartSuperAttack();

                cSound.PlaySound(SOUND_SUPERFIREBALL);

            } else {

                cSound.PlaySound(SOUND_FAIL);
            }

        } else {

            if (P2.get_now_mp() >= ATTACK_COST) {

                P2.StartAttack();

                cSound.PlaySound(SOUND_FIREBALL);

            } else {

                cSound.PlaySound(SOUND_FAIL);
            }
        }

        P2.set_skill_cost(0);
        P2.set_atk_cd(COOLDOWN);

        p2_attacks_ = false;
    }

    if (!P1.IsAttacking()) {

        if (P1.get_atk_cd() > 0) {
            P1.set_atk_cd(P1.get_atk_cd() - 1);
        }

        if (!P1.IsDamaged() && !p1_defends && P1.get_now_mp() < P1.get_max_mp()) {
            P1.set_now_mp(P1.get_now_mp() + 1);
        }

    } else {

        list<Character *>::iterator it;

        for (it = enemies.begin(); it != enemies.end(); it++) {
            if (P1.Attack(*it)) {
                cSound.PlaySound(SOUND_HIT);

                if ((*it)->get_character_status() == DYING) {
                    cSound.PlaySound(SOUND_DEATH);
                }
            }
        }
    }

    if (!P2.IsAttacking()) {
        if (P2.get_atk_cd() > 0) {
            P2.set_atk_cd(P2.get_atk_cd() - 1);
        }

    } else {

        if (P2.Attack(enemies, projectiles, cScene.get_projectile_hit_map())) {
            if (projectiles.back()->IsEnding()) {
                cSound.PlaySound(SOUND_EXPLOSION);
            }
        }
    }

    for (it_e = enemies.begin(); it_e != enemies.end(); ++it_e) {

        if (ia) {
            if (dynamic_cast<Enemy *>(*it_e)->get_frame_bottom() <= VisibleArea.top) {
                if ((*it_e)->IsWalking()) {

                    dynamic_cast<Enemy *>(*it_e)->Move();

                } else if ((*it_e)->IsAttacking()) {

                    if (dynamic_cast<Monster1 *>(*it_e) != NULL) {

                        list<Character *>::iterator it;

                        for (it = players.begin(); it != players.end(); it++) {
                            if (dynamic_cast<Monster1 *>(*it_e)->Attack(*it)) {
                                cSound.PlaySound(SOUND_HIT);
                            }
                        }
                    }

                    if (dynamic_cast<Monster2 *>(*it_e) != NULL) {
                        dynamic_cast<Monster2 *>(*it_e)->Attack(players, projectiles, cScene.get_projectile_hit_map());
                    }

                } else if ((*it_e)->IsLooking() && (*it_e)->get_character_y() <= VisibleArea.top) { // the AI only runs when it does not have tasks already assigned

                    if (dynamic_cast<Monster1 *>(*it_e) != NULL && dynamic_cast<Monster1 *>(*it_e)->IA(cScene.get_collision_map(), enemies, &P1, &P2)) {
                        cSound.PlaySound(SOUND_SLASH);
                    }
                    if (dynamic_cast<Monster2 *>(*it_e) != NULL && dynamic_cast<Monster2 *>(*it_e)->IA(cScene.get_collision_map(), cScene.get_projectile_hit_map(), enemies, &P1, &P2)) {
                        cSound.PlaySound(SOUND_ARROW);
                    }
                }
            }

        } else if (!(*it_e)->IsDamaged()) {

            (*it_e)->Stop();
        }
    }

    for (it_p = projectiles.begin(); it_p != projectiles.end(); ++it_p) {
        if (dynamic_cast<FireBall *>(*it_p) != NULL && (*it_p)->Logic(cScene.get_projectile_hit_map(), enemies)) {
            cSound.PlaySound(SOUND_EXPLOSION);
        }

        if (dynamic_cast<Arrow *>(*it_p) != NULL) {
            (*it_p)->Logic(cScene.get_projectile_hit_map(), players);
        }
    }

    for (it_t = tokens.begin(); it_t != tokens.end();) {

        bool used = false;

        if (dynamic_cast<ItemOfHeal *>(*it_t) != NULL && P1.Intersection(P1.getHitBox(), (*it_t)->getHitBox())) {
            used = dynamic_cast<ItemOfHeal *>(*it_t)->Heal(P1);
        } else if (dynamic_cast<ItemOfHeal *>(*it_t) != NULL && P2.Intersection(P2.getHitBox(), (*it_t)->getHitBox())) {
            used = dynamic_cast<ItemOfHeal *>(*it_t)->Heal(P2);
        }

        if (used) {

            delete (*it_t);
            it_t = tokens.erase(it_t);

            cSound.PlaySound(SOUND_TOKEN);

        } else {

            ++it_t;
        }
    }

    // Updates
    if (P1.get_character_status() == DYING ||
        P2.get_character_status() == DYING) {

        game_status_ = game_gameover_;

    } else if (P1.get_character_y() >= VisibleArea.top - kTileSize &&
               P2.get_character_y() >= VisibleArea.top - kTileSize) {

        (game_level_ < game_total_levels_)
            ? game_status_ = game_level_change_
            : game_status_ = game_endgame_;

    } else {

        UpdateCamera(h1, h2);
        cSound.Update();
    }

    return res;
}

// output
void Game::Render() {

    int tex_w;
    int tex_h;

    int tex_w2;
    int tex_h2;

    bool run = (game_status_ == game_runing_);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 激活著色器 // activating shaders
    if (epilepsia_mode) {

        time_ += 0.8f;

        if (time_ >= 180.0f) { time_ = 0.0f; }

        glActiveTexture(GL_TEXTURE0);

        cShader.Attach(SHADER_EPILEPTIC_F);
        cShader.Activate();

        cShader.setUniform(const_cast<char *>("time_"), abs(sin(time_)));
        cShader.setUniform(const_cast<char *>("tex"), 0);
        cShader.setUniform(const_cast<char *>("flatcolor"), 0);

    } else if (time_ > 0.0f) {

        glActiveTexture(GL_TEXTURE0);

        (game_status_ == game_gameover_)
            ? cShader.Attach(SHADER_GREYSCALEFADE_F)
            : cShader.Attach(SHADER_FADE_F);

        cShader.Activate();

        cShader.setUniform(const_cast<char *>("time_"), time_);
        cShader.setUniform(const_cast<char *>("tex"), 0);
        cShader.setUniform(const_cast<char *>("flatcolor"), 0);
    }

    // * painted

    getImg.set_img_size(IMG_TILESET, &tex_w, &tex_h);
    cScene.Draw(getImg.get_img_id(IMG_TILESET), tex_w, tex_h, run);

    // * Image of player

    getImg.set_img_size(IMG_PLAYER_1, &tex_w, &tex_h);
    if (P1.get_skill_cost() >= CHARGE_BREAK && P1.get_skill_cost() % 4 < 2) {
        P1.Draw(getImg.get_img_id(IMG_PLAYER_1_SUPER_ATTACKING), tex_w, tex_h, run);
    } else {
        P1.Draw(getImg.get_img_id(IMG_PLAYER_1), tex_w, tex_h, run);
    }

    getImg.set_img_size(IMG_PLAYER_2, &tex_w, &tex_h);
    if (P2.get_skill_cost() >= CHARGE_BREAK && P2.get_skill_cost() % 4 < 2) {
        P2.Draw(getImg.get_img_id(IMG_PLAYER_2_SUPER_ATTACKING), tex_w, tex_h, run);
    } else {
        P2.Draw(getImg.get_img_id(IMG_PLAYER_2), tex_w, tex_h, run);
    }

    // * Image of enemy

    getImg.set_img_size(IMG_MONSTER_1, &tex_w, &tex_h);
    getImg.set_img_size(IMG_MONSTER_2, &tex_w2, &tex_h2);

    list<Character *>::iterator it_e;
    for (it_e = enemies.begin(); it_e != enemies.end(); ++it_e) {

        if (dynamic_cast<Monster1 *>(*it_e) != NULL) {
            (*it_e)->Draw(getImg.get_img_id(IMG_MONSTER_1), tex_w, tex_h, run);
        }
        if (dynamic_cast<Monster2 *>(*it_e) != NULL) {
            (*it_e)->Draw(getImg.get_img_id(IMG_MONSTER_2), tex_w2, tex_h2, run);
        }
    }

    // * Image of heal

    getImg.set_img_size(IMG_HEART, &tex_w, &tex_h);

    list<BaseOfItem *>::iterator it_t;
    for (it_t = tokens.begin(); it_t != tokens.end(); ++it_t) {
        if (dynamic_cast<ItemOfHeal *>(*it_t) != NULL) {
            (*it_t)->BaseOfItem::DrawItem(getImg.get_img_id(IMG_HEART), tex_w, tex_h, run);
        }
    }

    // * Image of projectile

    getImg.set_img_size(IMG_FIREBALL, &tex_w, &tex_h);
    getImg.set_img_size(IMG_ARROW, &tex_w2, &tex_h2);

    list<Projectile *>::reverse_iterator it_p;

    glEnable(GL_BLEND); // turn blending on
    //glDisable(GL_DEPTH_TEST);    // turn depth testing off
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 新的火球蓋過舊的 // iterate backwards so that the last fireballs created are painted on top of the previous ones, that way we will visualize the correct alpha values
    for (it_p = projectiles.rbegin(); it_p != projectiles.rend(); ++it_p) {

        if (dynamic_cast<FireBall *>(*it_p) != NULL) {
            (*it_p)->OffsetProjectileTexture(getImg.get_img_id(IMG_FIREBALL), tex_w, tex_h, run);
        }
        if (dynamic_cast<Arrow *>(*it_p) != NULL) {
            (*it_p)->OffsetProjectileTexture(getImg.get_img_id(IMG_ARROW), tex_w2, tex_h2, run);
        }
    }

    if (time_ > 0.0f) { cShader.setUniform(const_cast<char *>("flatcolor"), 1); }

    PlayerStatusBar_1.DrawBars(P1.get_max_hp(), P1.get_now_hp(), P1.get_max_mp(), P1.get_now_mp());
    PlayerStatusBar_2.DrawBars(P2.get_max_hp(), P2.get_now_hp(), P2.get_max_mp(), P2.get_now_mp());

    if (time_ > 0.0f) { cShader.setUniform(const_cast<char *>("flatcolor"), 0); }

    PlayerStatusBar_1.OverlayDraw(getImg.get_img_id(IMG_OVERLAY_1), getImg.get_img_w(IMG_OVERLAY_1), getImg.get_img_h(IMG_OVERLAY_1));
    PlayerStatusBar_2.OverlayDraw(getImg.get_img_id(IMG_OVERLAY_2), getImg.get_img_w(IMG_OVERLAY_2), getImg.get_img_h(IMG_OVERLAY_2));

    glDisable(GL_BLEND); // turn blending off
    // glEnable(GL_DEPTH_TEST);    // turn depth testing on
    glAlphaFunc(GL_GREATER, 0.05f);

    // deactivating shaders
    if (epilepsia_mode) {

        cShader.Detach(SHADER_EPILEPTIC_F);
        cShader.Deactivate();

    } else if (time_ > 0.0f) {

        if (game_status_ == game_gameover_) {

            cShader.Detach(SHADER_GREYSCALEFADE_F);

        } else {

            cShader.Detach(SHADER_FADE_F);

            time_ -= 0.01f;
        }

        cShader.Deactivate();
    }

    glutSwapBuffers();
}

// ******************************
// *
// *
// * calss Game
// * private:
// *
// *
// ******************************

void Game::UpdateCamera(const int &h1, const int &h2) {

    int mean;
    (h1 >= h2) ? mean = h2 + (h1 - h2) / 2
               : mean = h1 + (h2 - h1) / 2;

    if (mean > (game_h_ / 2) &&
        mean < (kScene_H * kTileSize - (game_h_) / 2) - kTileSize * 2 &&
        mean - (game_h_ / 2) + kTileSize > VisibleArea.bottom) {

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        VisibleArea.bottom = mean - (game_h_ / 2) + kTileSize;
        VisibleArea.top    = mean + (game_h_ / 2) + kTileSize;

        glOrtho(VisibleArea.left, VisibleArea.right,
                VisibleArea.bottom, VisibleArea.top,
                3, -kScene_H * kTileSize - 2 - 1);

        // overlays 隨著 camera 移動 // overlays move next to the camera
        PlayerStatusBar_1.set_player_status_bar_y(VisibleArea.bottom);
        PlayerStatusBar_2.set_player_status_bar_y(VisibleArea.bottom);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
}

bool Game::LoadDynamicLayer(const int &game_level) {

    FILE *fd;

    char c, file[ 32 ];

    int i, j, id;

    (game_level_ < 10) ? sprintf(file, "Levels/%s0%d%s", DYNAMIC_FILENAME, game_level, (char *) FILENAME_EXT)
                       : sprintf(file, "Levels/%s%d%s", DYNAMIC_FILENAME, game_level, (char *) FILENAME_EXT);

    fd = fopen(file, "r");

    if (fd == NULL) { return false; }

    for (j = kScene_H - 1; j >= 0; --j) {
        for (i = 0; i < kScene_W; ++i) {

            fscanf(fd, "%i", &id);

            switch (id) {

                case 1: P1.set_tile(i, j); break;
                case 2: P2.set_tile(i, j); break;

                case 3:
                    enemies.push_back(new Monster1);
                    enemies.back()->set_tile(i, j);
                    break;

                case 4:
                    enemies.push_back(new Monster2);
                    enemies.back()->set_tile(i, j);
                    break;

                case 5:
                    tokens.push_back(new ItemOfHeal);
                    tokens.back()->set_tile(i, j);
                    break;
            }

            fscanf(fd, "%c", &c); // 傳遞逗號或輸入 // pass comma or enter
        }
    }

    fclose(fd);

    return true;
}
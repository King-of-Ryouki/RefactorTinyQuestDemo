#ifndef BASE_OF_SCENE_HPP // 場景
#define BASE_OF_SCENE_HPP

#include "_pch.hpp"

#define LEVEL_FILENAME "static_layer"
#define DYNAMIC_FILENAME "dynamic_layer"
#define FILENAME_EXT ".txt"

// 場景
const int kScene_W    = 20;
const int kScene_H    = 160;
const int kScene_Area = kScene_W * kScene_H; // 陣列需用常數

const int kTileSize        = 16; // 顯示圖大小 px // the size in px of a tile on screen
const int kTextureTileSize = 16; // 材質圖大小 px (通常為16或32) // the size in px on the side of a tile in the texture (typically 16 or 32)

// 地形 // 場景
class Tile {
public:
    void set_pos_x(int pos_x);
    int get_pos_x();

    void set_pos_y(int pos_y);
    int get_pos_y();

    void set_tex_coord_x(float texture_x);
    float get_tex_coord_x();

    void set_tex_coord_y(float texture_y);
    float get_tex_coord_y();

private:
    // 顯示位置 // position on screen
    int pos_x_;
    int pos_y_;

    // 場景材質的座標 // texture coordinates in the tileset
    float texture_coord_x_;
    float texture_coord_y_;
};

class Scene {
public:
    Scene();
    virtual ~Scene() {}

    bool *get_collision_map();
    bool *get_projectile_hit_map();

    void Draw(int texture_id, int texture_w, int texture_h, bool run);
    void DrawAnimatedTiles(int texture_id, int texture_w, int texture_h);

    bool LoadLevel(int level, int texture_w, int texture_h);
    void ComputeCollisionMaps();

private:
    // 用於不同行為的工具集 // sets of tools for different behaviors
    vector<Tile> vec_animated_tiles_; // 動畫 圖
    vector<Tile> vec_animated_tiles_up_;

    int z_tileup_[ kScene_Area ];
    int z_tiledown_[ kScene_Area ];

    int always_up_[ kScene_Area ];
    int always_up_alpha_[ kScene_Area ];
    int always_down_[ kScene_Area ];

    int terrain_flat_[ kScene_Area ];  // 地形 平面
    int terrain_solid_[ kScene_Area ]; // 地形 實體

    int solid_alpha_[ kScene_Area ];
    int bridge_[ kScene_Area ];

    // 顯示列表 // display lists
    bool collision_map[ kScene_Area ];      // 地形碰撞
    bool projectile_hit_map[ kScene_Area ]; // 投射物地形碰撞

    int id_z_tile_up_;
    int id_z_tile_down_;

    int id_always_up_;
    int id_always_up_alpha_;
    int id_always_down_;

    int id_terrain_flat_;
    int id_terrain_solid_;

    int id_solid_alpha_;
    int id_bridge_;

    int seq, delay, frame_delay;

    int tile_frames_ = 4; // 動畫圖的幀數 // the number of frames that the animated tiles will have
    int tile_delay_  = 3; // 移動到另一幀 // what it will take an animated tile to move from one frame to another
};

class Rect {
public:
    int left;
    int right;
    int bottom;
    int top;
};

#endif // BASE_OF_SCENE_HPP
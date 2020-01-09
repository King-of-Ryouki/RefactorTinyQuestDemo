#ifndef DATA_H
#define DATA_H

#include "..\_pch.hpp"

// 圖片 id // image identifiers
enum EenumImage {

    IMG_TILESET = 0,

    IMG_PLAYER_1,
    IMG_PLAYER_1_SUPER_ATTACKING,

    IMG_PLAYER_2,
    IMG_PLAYER_2_SUPER_ATTACKING,

    IMG_MONSTER_1,
    IMG_MONSTER_2,

    IMG_OVERLAY_1,
    IMG_OVERLAY_2,

    IMG_ARROW,
    IMG_FIREBALL,

    IMG_HEART,

    IMG_ALL_IMG_ID_SUM // 圖片陣列大小 // image array size
};

class Texture {
public:
    bool LoadTexture(const char *filename);

    int get_texture_id();

    void get_texture_size(int *texture_w, int *texture_h);

    int get_texture_w(const int &img_tex_id);
    int get_texture_h(const int &img_tex_id);

private:
    GLuint texture_id_;

    int texture_w_;
    int texture_h_;
};

class GetImage {

public:
    bool InitImages();

    int get_img_id(const int &img_id);

    void set_img_size(const int &img_id, int *w, int *h);

    int get_img_w(const int &img_tex_id);
    int get_img_h(const int &img_tex_id);

private:
    Texture Textures_[ IMG_ALL_IMG_ID_SUM ];

    bool LoadImage(const int &img_tex_id, const char *texture_path);
};

#endif // DATA_H
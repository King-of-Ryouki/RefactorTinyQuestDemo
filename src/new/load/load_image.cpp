#include "load_image.hpp"

bool Texture::LoadTexture(const char *texture_path) {
    // * OpenCV

    cv::Mat texture = cv::imread(texture_path, cv::IMREAD_UNCHANGED);
    // cv::imshow("OpenCV", texture);
    // flip(texture, texture, 0); // 圖片上下翻轉

    texture_w_ = texture.cols; // 宽
    texture_h_ = texture.rows; // 高

    if (texture.empty()) {

        cout << "texture is empty" << endl;
        return false;
    }

    // * RGBA or RGB // OpenCV is BGR

    int internal_format, color_type;

    switch (texture.channels()) {

        case 4: internal_format = GL_RGBA, color_type = GL_BGRA_EXT; break;
        case 3: internal_format = GL_RGB, color_type = GL_BGR_EXT; break;

        default: return false;
    }

    // * OpenGL texture

    glEnable(GL_TEXTURE_2D);                   // 啟用纹理
    glGenTextures(1, &texture_id_);            // 生成紋理的數量
    glBindTexture(GL_TEXTURE_2D, texture_id_); // 綁定纹理

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);       // 橫向
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);       // 直向
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 放大
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 縮小

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, texture_w_, texture_h_, 0, color_type, GL_UNSIGNED_BYTE, texture.data);

    // if (!mipmap) { glTexImage2D(GL_TEXTURE_2D, 0, colors, texture_w_, texture_h_, 0, internal_format, GL_UNSIGNED_BYTE, Image->getPixels()); } else { gluBuild2DMipmaps(GL_TEXTURE_2D, colors, texture_w_, texture_h_, internal_formats, GL_UNSIGNED_BYTE, Image->getPixels()); }

    glEnd();                  // 结束貼圖
    glDisable(GL_TEXTURE_2D); // 停用纹理

    return true;
}

int Texture::get_texture_id() { return texture_id_; }

void Texture::get_texture_size(int *texture_w, int *texture_h) {
    *texture_w = texture_w_;
    *texture_h = texture_h_;
}

int Texture::get_texture_w(const int &img_tex_id) { return texture_w_; }
int Texture::get_texture_h(const int &img_tex_id) { return texture_h_; }

// ******************************
// *
// *
// *
// * class  GetImage
// *
// *
// *
// ******************************

bool GetImage::InitImages() {
    if (!GetImage::LoadImage(IMG_TILESET, const_cast<char *>("./Textures/tileset.png"))) { return false; }

    if (!GetImage::LoadImage(IMG_OVERLAY_1, const_cast<char *>("./Textures/overlay1.png"))) { return false; }
    if (!GetImage::LoadImage(IMG_OVERLAY_2, const_cast<char *>("./Textures/overlay2.png"))) { return false; }

    if (!GetImage::LoadImage(IMG_PLAYER_1, const_cast<char *>("./Textures/player1.png"))) { return false; }
    if (!GetImage::LoadImage(IMG_PLAYER_1_SUPER_ATTACKING, const_cast<char *>("./Textures/player1_super_attacking.png"))) { return false; }
    if (!GetImage::LoadImage(IMG_PLAYER_2, const_cast<char *>("./Textures/player2.png"))) { return false; }
    if (!GetImage::LoadImage(IMG_PLAYER_2_SUPER_ATTACKING, const_cast<char *>("./Textures/player2_super_attacking.png"))) { return false; }

    if (!GetImage::LoadImage(IMG_MONSTER_1, const_cast<char *>("./Textures/monster1.png"))) { return false; }
    if (!GetImage::LoadImage(IMG_MONSTER_2, const_cast<char *>("./Textures/monster2.png"))) { return false; }

    if (!GetImage::LoadImage(IMG_HEART, const_cast<char *>("./Textures/heart.png"))) { return false; }
    if (!GetImage::LoadImage(IMG_ARROW, const_cast<char *>("./Textures/arrow.png"))) { return false; }
    if (!GetImage::LoadImage(IMG_FIREBALL, const_cast<char *>("./Textures/fireball.png"))) { return false; }

    return true;
}

int GetImage::get_img_id(const int &img_tex_id) {
    return Textures_[ img_tex_id ].Texture::get_texture_id();
}

void GetImage::set_img_size(const int &img_tex_id, int *img_tex_w, int *img_tex_h) {
    Textures_[ img_tex_id ].Texture::get_texture_size(img_tex_w, img_tex_h);
}

int GetImage::get_img_w(const int &img_tex_id) { return Textures_[ img_tex_id ].get_texture_w(img_tex_id); }
int GetImage::get_img_h(const int &img_tex_id) { return Textures_[ img_tex_id ].get_texture_h(img_tex_id); }

bool GetImage::LoadImage(const int &img_tex_id, const char *texture_path) {

    if (Textures_[ img_tex_id ].Texture::LoadTexture(texture_path)) {
        return true;
    }

    return false;
}
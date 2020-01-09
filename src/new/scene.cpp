#include "scene.hpp"

// ******************************
// *
// *
// *
// * class Tile
// *
// *
// *
// ******************************

void Tile::set_pos_x(int pos_x) { pos_x_ = pos_x; }
int Tile::get_pos_x() { return pos_x_; }

void Tile::set_pos_y(int pos_y) { pos_y_ = pos_y; }
int Tile::get_pos_y() { return pos_y_; }

void Tile::set_tex_coord_x(float texture_x) { texture_coord_x_ = texture_x; }
float Tile::get_tex_coord_x() { return texture_coord_x_; }

void Tile::set_tex_coord_y(float texture_y) { texture_coord_y_ = texture_y; }
float Tile::get_tex_coord_y() { return texture_coord_y_; }

// ******************************
// *
// *
// *
// * class Scene
// *
// *
// *
// ******************************

//
//    ___
//   | Tile |
//  x|___|
//    y

//             tile_y = 0
//              _____________
//  tile_x = 0 | Texture |
//             |_________|
//             |

Scene::Scene() { seq = 0, delay = 0, frame_delay = 6; }

bool *Scene::get_collision_map() { return collision_map; }
bool *Scene::get_projectile_hit_map() { return projectile_hit_map; }

void Scene::Draw(int texture_id, int texture_w, int texture_h, bool run) {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    DrawAnimatedTiles(texture_id, texture_w, texture_h);

    glCallList(id_always_up_alpha_);
    glCallList(id_always_up_);
    glCallList(id_z_tile_down_);
    glCallList(id_z_tile_up_);

    glCallList(id_always_down_);
    glCallList(id_bridge_);
    glCallList(id_solid_alpha_);
    glCallList(id_terrain_solid_);
    glCallList(id_terrain_flat_);

    glDisable(GL_TEXTURE_2D);

    // 動畫圖塊 Next Frame () // Next Frame () for animated tiles
    if (run) {

        ++delay;

        if (delay == tile_delay_) { ++seq, seq %= tile_frames_, delay = 0; } // 移動到另一幀 // what it will take an animated tile to move from one frame to another
    }
}

void Scene::DrawAnimatedTiles(int texture_id, int texture_w, int texture_h) {

    unsigned int i;

    int pos_x;
    int pos_y;

    float coord_x_tile;
    float coord_y_tile;

    float texture_offset_x = 1.0f / float(texture_w / kTextureTileSize);
    float texture_offset_y = 1.0f / float(texture_h / kTextureTileSize);

    for (i = 0; i < vec_animated_tiles_.size(); ++i) {

        pos_x = vec_animated_tiles_[ i ].get_pos_x();
        pos_y = vec_animated_tiles_[ i ].get_pos_y();

        coord_x_tile = vec_animated_tiles_[ i ].get_tex_coord_x();
        coord_y_tile = vec_animated_tiles_[ i ].get_tex_coord_y();

        glBegin(GL_POLYGON);

        glTexCoord2f(coord_x_tile + texture_offset_x * seq, coord_y_tile + texture_offset_y);
        glVertex3i(pos_x, pos_y, (kScene_H * kTileSize) + 2);
        glTexCoord2f(coord_x_tile + texture_offset_x * (seq + 1), coord_y_tile + texture_offset_y);
        glVertex3i(pos_x + kTileSize, pos_y, (kScene_H * kTileSize) + 2);

        glTexCoord2f(coord_x_tile + texture_offset_x * (seq + 1), coord_y_tile);
        glVertex3i(pos_x + kTileSize, pos_y + kTileSize, (kScene_H * kTileSize) + 2);
        glTexCoord2f(coord_x_tile + texture_offset_x * seq, coord_y_tile);
        glVertex3i(pos_x, pos_y + kTileSize, (kScene_H * kTileSize) + 2);

        glEnd();
    }

    for (i = 0; i < vec_animated_tiles_up_.size(); ++i) {

        pos_x = vec_animated_tiles_up_[ i ].get_pos_x();
        pos_y = vec_animated_tiles_up_[ i ].get_pos_y();

        coord_x_tile = vec_animated_tiles_up_[ i ].get_tex_coord_x();
        coord_y_tile = vec_animated_tiles_up_[ i ].get_tex_coord_y();

        glBegin(GL_POLYGON);

        glTexCoord2f(coord_x_tile + texture_offset_x * seq, coord_y_tile + texture_offset_y);
        glVertex3i(pos_x, pos_y, 0);
        glTexCoord2f(coord_x_tile + texture_offset_x * (seq + 1), coord_y_tile + texture_offset_y);
        glVertex3i(pos_x + kTileSize, pos_y, 0);

        glTexCoord2f(coord_x_tile + texture_offset_x * (seq + 1), coord_y_tile);
        glVertex3i(pos_x + kTileSize, pos_y + kTileSize, 0);
        glTexCoord2f(coord_x_tile + texture_offset_x * seq, coord_y_tile);
        glVertex3i(pos_x, pos_y + kTileSize, 0);

        glEnd();
    }
}

void Scene::ComputeCollisionMaps() { // 計算地形碰撞

    // int AA = 20, BB = 161;
    // for (int i = 0; i < AA * BB; i++)
    for (int i = 0; i < kScene_Area; i++) {

        collision_map[ i ]      = (!bridge_[ i ] && (terrain_solid_[ i ] || solid_alpha_[ i ] || always_down_[ i ] || z_tileup_[ i ]));
        projectile_hit_map[ i ] = (!bridge_[ i ] && (always_down_[ i ] || z_tileup_[ i ]));
    }
}

bool Scene::LoadLevel(int level, int texture_w, int texture_h) {

    FILE *fd;

    // 清除前一關 // clean the information from the previous level
    vec_animated_tiles_.clear();
    vec_animated_tiles_up_.clear();

    bool res = true;

    char file[ 32 ], c;

    float coord_x_tile, coord_y_tile;

    float texture_offset_x = 1.0f / float(texture_w / kTextureTileSize);
    float texture_offset_y = 1.0f / float(texture_h / kTextureTileSize);

    int tiles_w = (texture_w / kTextureTileSize);
    int i, j, id;

    int pos_x;
    int pos_y;

    (level < 10) ? sprintf(file, "Levels/%s0%d%s", (char *) LEVEL_FILENAME, level, (char *) FILENAME_EXT)
                 : sprintf(file, "Levels/%s%d%s", (char *) LEVEL_FILENAME, level, (char *) FILENAME_EXT);

    fd = fopen(file, "r");
    if (fd == NULL) { return false; }

    id_terrain_flat_ = glGenLists(1);
    glNewList(id_terrain_flat_, GL_COMPILE);
    glBegin(GL_QUADS);

    for (j = kScene_H - 1; j >= 0; --j) {

        pos_x = 0;
        pos_y = (kTileSize * j);

        for (i = 0; i < kScene_W; i++) {

            fscanf(fd, "%i", &id);
            terrain_flat_[ (j * kScene_W) + i ] = id;

            coord_x_tile = texture_offset_x * ((id - 1) % tiles_w);
            coord_y_tile = texture_offset_y * ((id - 1) / tiles_w);

            if (id) { // 頂點必須採用的紋理坐標; 繪製頂點的位置 // texture coords that the vertex has to take; where the vertex was drawn;

                glTexCoord2f(coord_x_tile, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x, pos_y, (kScene_H * kTileSize) + 2);
                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x + kTileSize, pos_y, (kScene_H * kTileSize) + 2);

                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile);
                glVertex3i(pos_x + kTileSize, pos_y + kTileSize, (kScene_H * kTileSize) + 2);
                glTexCoord2f(coord_x_tile, coord_y_tile);
                glVertex3i(pos_x, pos_y + kTileSize, (kScene_H * kTileSize) + 2);
            }

            pos_x += kTileSize;
            fscanf(fd, "%c", &c); // 傳遞逗號或輸入 // pass comma or enter
        }
    }

    glEnd();
    glEndList();

    id_terrain_solid_ = glGenLists(1);
    glNewList(id_terrain_solid_, GL_COMPILE);
    glBegin(GL_QUADS);

    for (j = kScene_H - 1; j >= 0; --j) {

        pos_x = 0;
        pos_y = (kTileSize * j);

        for (i = 0; i < kScene_W; i++) {

            fscanf(fd, "%i", &id);
            terrain_solid_[ (j * kScene_W) + i ] = id;

            coord_x_tile = texture_offset_x * ((id - 1) % tiles_w);
            coord_y_tile = texture_offset_y * ((id - 1) / tiles_w);

            if (id) { // 頂點必須採用的紋理坐標; 繪製頂點的位置 // texture coords that the vertex has to take; where the vertex was drawn;

                glTexCoord2f(coord_x_tile, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x, pos_y, (kScene_H * kTileSize) + 2);
                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x + kTileSize, pos_y, (kScene_H * kTileSize) + 2);

                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile);
                glVertex3i(pos_x + kTileSize, pos_y + kTileSize, (kScene_H * kTileSize) + 2);
                glTexCoord2f(coord_x_tile, coord_y_tile);
                glVertex3i(pos_x, pos_y + kTileSize, (kScene_H * kTileSize) + 2);
            }

            pos_x += kTileSize;
            fscanf(fd, "%c", &c); // 傳遞逗號或輸入 // pass comma or enter
        }
    }

    glEnd();
    glEndList();

    id_solid_alpha_ = glGenLists(1);
    glNewList(id_solid_alpha_, GL_COMPILE);
    glBegin(GL_QUADS);

    for (j = kScene_H - 1; j >= 0; --j) {

        pos_x = 0;
        pos_y = (kTileSize * j);

        for (i = 0; i < kScene_W; i++) {

            fscanf(fd, "%i", &id);
            solid_alpha_[ (j * kScene_W) + i ] = id;

            coord_x_tile = texture_offset_x * ((id - 1) % tiles_w);
            coord_y_tile = texture_offset_y * ((id - 1) / tiles_w);

            if (id) { // 頂點必須採用的紋理坐標; 繪製頂點的位置 // texture coords that the vertex has to take; where the vertex was drawn;

                glTexCoord2f(coord_x_tile, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x, pos_y, (kScene_H * kTileSize) + 2);
                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x + kTileSize, pos_y, (kScene_H * kTileSize) + 2);

                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile);
                glVertex3i(pos_x + kTileSize, pos_y + kTileSize, (kScene_H * kTileSize) + 2);
                glTexCoord2f(coord_x_tile, coord_y_tile);
                glVertex3i(pos_x, pos_y + kTileSize, (kScene_H * kTileSize) + 2);
            }

            pos_x += kTileSize;
            fscanf(fd, "%c", &c); // 傳遞逗號或輸入 // pass comma or enter
        }
    }

    glEnd();
    glEndList();

    //en este bucle almaceno las tiles animadas del nivel inferior, si las hubiera
    for (j = kScene_H - 1; j >= 0; --j) {

        pos_x = 0;
        pos_y = (kTileSize * j);

        for (i = 0; i < kScene_W; i++) {
            fscanf(fd, "%i", &id);

            if (id) {

                Tile TmpTile;

                TmpTile.set_pos_x(pos_x);
                TmpTile.set_pos_y(pos_y);

                TmpTile.set_tex_coord_x(texture_offset_x * ((id - 1) % tiles_w));
                TmpTile.set_tex_coord_y(texture_offset_y * ((id - 1) / tiles_w));

                vec_animated_tiles_.push_back(TmpTile);
            }

            pos_x += kTileSize;
            fscanf(fd, "%c", &c); // 傳遞逗號或輸入 // pass comma or enter
        }
    }

    id_bridge_ = glGenLists(1);
    glNewList(id_bridge_, GL_COMPILE);
    glBegin(GL_QUADS);

    for (j = kScene_H - 1; j >= 0; --j) {

        pos_x = 0;
        pos_y = (kTileSize * j);

        for (i = 0; i < kScene_W; i++) {

            fscanf(fd, "%i", &id);
            bridge_[ (j * kScene_W) + i ] = id;

            coord_x_tile = texture_offset_x * ((id - 1) % tiles_w);
            coord_y_tile = texture_offset_y * ((id - 1) / tiles_w);

            if (id) { // 頂點必須採用的紋理坐標; 繪製頂點的位置 // texture coords that the vertex has to take; where the vertex was drawn;

                glTexCoord2f(coord_x_tile, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x, pos_y, (kScene_H * kTileSize) + 1);
                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x + kTileSize, pos_y, (kScene_H * kTileSize) + 1);

                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile);
                glVertex3i(pos_x + kTileSize, pos_y + kTileSize, (kScene_H * kTileSize) + 1);
                glTexCoord2f(coord_x_tile, coord_y_tile);
                glVertex3i(pos_x, pos_y + kTileSize, (kScene_H * kTileSize) + 1);
            }

            pos_x += kTileSize;
            fscanf(fd, "%c", &c); // 傳遞逗號或輸入 // pass comma or enter
        }
    }

    glEnd();
    glEndList();

    id_always_down_ = glGenLists(1);
    glNewList(id_always_down_, GL_COMPILE);
    glBegin(GL_QUADS);

    for (j = kScene_H - 1; j >= 0; --j) {

        pos_x = 0;
        pos_y = (kTileSize * j);

        for (i = 0; i < kScene_W; i++) {

            fscanf(fd, "%i", &id);
            always_down_[ (j * kScene_W) + i ] = id;

            coord_x_tile = texture_offset_x * ((id - 1) % tiles_w);
            coord_y_tile = texture_offset_y * ((id - 1) / tiles_w);

            if (id) { // 頂點必須採用的紋理坐標; 繪製頂點的位置 // texture coords that the vertex has to take; where the vertex was drawn;

                glTexCoord2f(coord_x_tile, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x, pos_y, (kScene_H * kTileSize) + 2);
                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x + kTileSize, pos_y, (kScene_H * kTileSize) + 2);

                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile);
                glVertex3i(pos_x + kTileSize, pos_y + kTileSize, (kScene_H * kTileSize) + 2);
                glTexCoord2f(coord_x_tile, coord_y_tile);
                glVertex3i(pos_x, pos_y + kTileSize, (kScene_H * kTileSize) + 2);
            }

            pos_x += kTileSize;
            fscanf(fd, "%c", &c); // 傳遞逗號或輸入 // pass comma or enter
        }
    }

    glEnd();
    glEndList();

    id_z_tile_up_ = glGenLists(1);
    glNewList(id_z_tile_up_, GL_COMPILE);
    glBegin(GL_QUADS);

    for (j = kScene_H - 1; j >= 0; --j) {

        pos_x = 0;
        pos_y = (kTileSize * j);

        for (i = 0; i < kScene_W; i++) {

            fscanf(fd, "%i", &id);
            z_tileup_[ (j * kScene_W) + i ] = id;

            coord_x_tile = texture_offset_x * ((id - 1) % tiles_w);
            coord_y_tile = texture_offset_y * ((id - 1) / tiles_w);

            if (id) { // 頂點必須採用的紋理坐標; 繪製頂點的位置 // texture coords that the vertex has to take; where the vertex was drawn;

                glTexCoord2f(coord_x_tile, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x, pos_y, pos_y + kTileSize);
                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x + kTileSize, pos_y, pos_y + kTileSize);

                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile);
                glVertex3i(pos_x + kTileSize, pos_y + kTileSize, pos_y + kTileSize);
                glTexCoord2f(coord_x_tile, coord_y_tile);
                glVertex3i(pos_x, pos_y + kTileSize, pos_y + kTileSize);
            }

            pos_x += kTileSize;
            fscanf(fd, "%c", &c); // 傳遞逗號或輸入 // pass comma or enter
        }
    }

    glEnd();
    glEndList();

    id_z_tile_down_ = glGenLists(1);
    glNewList(id_z_tile_down_, GL_COMPILE);
    glBegin(GL_QUADS);

    for (j = kScene_H - 1; j >= 0; --j) {

        pos_x = 0;
        pos_y = (kTileSize * j);

        for (i = 0; i < kScene_W; i++) {

            fscanf(fd, "%i", &id);
            z_tiledown_[ (j * kScene_W) + i ] = id;

            coord_x_tile = texture_offset_x * ((id - 1) % tiles_w);
            coord_y_tile = texture_offset_y * ((id - 1) / tiles_w);

            if (id) { // 頂點必須採用的紋理坐標; 繪製頂點的位置 // texture coords that the vertex has to take; where the vertex was drawn;

                glTexCoord2f(coord_x_tile, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x, pos_y, pos_y);
                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x + kTileSize, pos_y, pos_y);

                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile);
                glVertex3i(pos_x + kTileSize, pos_y + kTileSize, pos_y);
                glTexCoord2f(coord_x_tile, coord_y_tile);
                glVertex3i(pos_x, pos_y + kTileSize, pos_y);
            }

            pos_x += kTileSize;
            fscanf(fd, "%c", &c); // 傳遞逗號或輸入 // pass comma or enter
        }
    }

    glEnd();
    glEndList();

    id_always_up_ = glGenLists(1);
    glNewList(id_always_up_, GL_COMPILE);
    glBegin(GL_QUADS);

    // always_up_
    for (j = kScene_H - 1; j >= 0; --j) {

        pos_x = 0;
        pos_y = (kTileSize * j);

        for (i = 0; i < kScene_W; i++) {

            fscanf(fd, "%i", &id);
            always_up_[ (j * kScene_W) + i ] = id;

            coord_x_tile = texture_offset_x * ((id - 1) % tiles_w);
            coord_y_tile = texture_offset_y * ((id - 1) / tiles_w);

            if (id) { // 頂點必須採用的紋理坐標; 繪製頂點的位置 // texture coords that the vertex has to take; where the vertex was drawn;

                glTexCoord2f(coord_x_tile, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x, pos_y, 0);
                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x + kTileSize, pos_y, 0);
                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile);
                glVertex3i(pos_x + kTileSize, pos_y + kTileSize, 0);
                glTexCoord2f(coord_x_tile, coord_y_tile);
                glVertex3i(pos_x, pos_y + kTileSize, 0);
            }

            pos_x += kTileSize;
            fscanf(fd, "%c", &c); // 傳遞逗號或輸入 // pass comma or enter
        }
    }

    glEnd();
    glEndList();

    id_always_up_alpha_ = glGenLists(1);
    glNewList(id_always_up_alpha_, GL_COMPILE);
    glBegin(GL_QUADS);

    // always_up_
    for (j = kScene_H - 1; j >= 0; --j) {

        pos_x = 0;
        pos_y = (kTileSize * j);

        for (i = 0; i < kScene_W; i++) {

            fscanf(fd, "%i", &id);
            always_up_alpha_[ (j * kScene_W) + i ] = id;

            coord_x_tile = texture_offset_x * ((id - 1) % tiles_w);
            coord_y_tile = texture_offset_y * ((id - 1) / tiles_w);

            if (id) { // 頂點必須採用的紋理坐標; 繪製頂點的位置 // texture coords that the vertex has to take; where the vertex was drawn;

                glTexCoord2f(coord_x_tile, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x, pos_y, 0);
                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile + texture_offset_y);
                glVertex3i(pos_x + kTileSize, pos_y, 0);

                glTexCoord2f(coord_x_tile + texture_offset_x, coord_y_tile);
                glVertex3i(pos_x + kTileSize, pos_y + kTileSize, 0);
                glTexCoord2f(coord_x_tile, coord_y_tile);
                glVertex3i(pos_x, pos_y + kTileSize, 0);
            }

            pos_x += kTileSize;
            fscanf(fd, "%c", &c); // 傳遞逗號或輸入 // pass comma or enter
        }
    }

    glEnd();
    glEndList();

    // 在這個循環中我存儲了上層的動畫圖塊（如果有的話） // in this loop I store the animated tiles of the upper level, if any
    for (j = kScene_H - 1; j >= 0; --j) {

        pos_x = 0;
        pos_y = (kTileSize * j);

        for (i = 0; i < kScene_W; i++) {

            fscanf(fd, "%i", &id);

            if (id) {

                Tile TmpTile;

                TmpTile.set_pos_x(pos_x);
                TmpTile.set_pos_y(pos_y);

                TmpTile.set_tex_coord_x(texture_offset_x * ((id - 1) % tiles_w));
                TmpTile.set_tex_coord_y(texture_offset_y * ((id - 1) / tiles_w));

                vec_animated_tiles_up_.push_back(TmpTile);
            }

            pos_x += kTileSize;
            fscanf(fd, "%c", &c); // 傳遞逗號或輸入 // pass comma or enter
        }
    }

    fclose(fd);

    return res;
}
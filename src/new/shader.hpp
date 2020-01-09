#ifndef SHADER_H
#define SHADER_H

#include "_pch.hpp"

const int SHADER_SIMPLE_V        = 0;
const int SHADER_SIMPLE_G        = 1;
const int SHADER_SIMPLE_F        = 2;
const int SHADER_RED_F           = 3;
const int SHADER_CTR_F           = 4;
const int SHADER_FADE_F          = 5;
const int SHADER_GREYSCALEFADE_F = 6;
const int SHADER_EPILEPTIC_F     = 7;
const int NUM_SHADERS            = 8;

class Shader { // 著色器
public:
    void Attach(int shader_id);
    void Detach(int shader_id);

    void Activate();   // 啟用, 活動
    void Deactivate(); // 停用

    void Load();

    // 訪問 uniforms 總是在 linkar 之後完成 (Activate() 方法)
    // access to uniforms is always done after linkar (Activate () method)
    template <class T>
    void setUniform(char *uniform, T value) {

        GLint loc = glGetUniformLocation(prog, uniform);

        if (loc != -1) {
            (typeid(T) == typeid(int))
                ? glUniform1i(loc, value)
                : glUniform1f(loc, value);
        }
    }

private:
    GLuint prog;
    GLuint shaders[ NUM_SHADERS ];
};

#endif // SHADER_H
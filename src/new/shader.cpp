#include "shader.hpp"

// !v| e.g. not in class -----

string get_file_contents(const char *filename) {

    std::ifstream ifs(filename);
    string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return content;
}

void glcppShaderSource(GLuint shader, string const &shader_string) {

    GLchar const *shader_source = shader_string.c_str();
    GLint const shader_length   = shader_string.size();
    glShaderSource(shader, 1, &shader_source, &shader_length);
}

void load_shader(GLuint shaderobject, char *const shadersourcefilename) {
    glcppShaderSource(shaderobject, get_file_contents(shadersourcefilename));
}

// !^| e.g. not in class -----

void Shader::Attach(int shader_id) { glAttachShader(prog, shaders[ shader_id ]); }
void Shader::Detach(int shader_id) { glDetachShader(prog, shaders[ shader_id ]); }

void Shader::Activate() {

    glLinkProgram(prog);
    glUseProgram(prog);
}

void Shader::Deactivate() { glUseProgram(0); }

void Shader::Load() {

    prog = glCreateProgram();

    shaders[ SHADER_SIMPLE_V ] = glCreateShader(GL_VERTEX_SHADER);
    load_shader(shaders[ SHADER_SIMPLE_V ], const_cast<char *>("Shaders/simple.vert"));
    glCompileShader(shaders[ SHADER_SIMPLE_V ]);

    shaders[ SHADER_SIMPLE_G ] = glCreateShader(GL_GEOMETRY_SHADER);
    load_shader(shaders[ SHADER_SIMPLE_G ], const_cast<char *>("Shaders/simple.geom"));
    glCompileShader(shaders[ SHADER_SIMPLE_G ]);

    shaders[ SHADER_SIMPLE_F ] = glCreateShader(GL_FRAGMENT_SHADER);
    load_shader(shaders[ SHADER_SIMPLE_F ], const_cast<char *>("Shaders/simple.frag"));
    glCompileShader(shaders[ SHADER_SIMPLE_F ]);

    shaders[ SHADER_RED_F ] = glCreateShader(GL_FRAGMENT_SHADER);
    load_shader(shaders[ SHADER_RED_F ], const_cast<char *>("Shaders/red.frag"));
    glCompileShader(shaders[ SHADER_RED_F ]);

    shaders[ SHADER_CTR_F ] = glCreateShader(GL_FRAGMENT_SHADER);
    load_shader(shaders[ SHADER_CTR_F ], const_cast<char *>("Shaders/ctr.frag"));
    glCompileShader(shaders[ SHADER_CTR_F ]);

    shaders[ SHADER_FADE_F ] = glCreateShader(GL_FRAGMENT_SHADER);
    load_shader(shaders[ SHADER_FADE_F ], const_cast<char *>("Shaders/fade.frag"));
    glCompileShader(shaders[ SHADER_FADE_F ]);

    shaders[ SHADER_GREYSCALEFADE_F ] = glCreateShader(GL_FRAGMENT_SHADER);
    load_shader(shaders[ SHADER_GREYSCALEFADE_F ], const_cast<char *>("Shaders/greyscalefade.frag"));
    glCompileShader(shaders[ SHADER_GREYSCALEFADE_F ]);

    shaders[ SHADER_EPILEPTIC_F ] = glCreateShader(GL_FRAGMENT_SHADER);
    load_shader(shaders[ SHADER_EPILEPTIC_F ], const_cast<char *>("Shaders/epileptic.frag"));
    glCompileShader(shaders[ SHADER_EPILEPTIC_F ]);

    // 直接加載 shader 的 source // load the source of the shader directly
    // static const char *fragment_source = {
    // "uniform float time;"
    // "void main()"
    // "{"
    // "     gl_FragColor = mix(gl_Color,vec4(1,0,0,1),time);"
    // "}"
    // };
    // glShaderSource(shaders[SHADER_RED_F], 1, &fragment_source, NULL);
}

#pragma once

// #define GLEW_STATIC
#include <GL/glew.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "../utils/utils.hpp"


class Shader {
public:
    unsigned int id;

    Shader(const char *vSdPath, const char *fSdPath) {
        // 1. 从文件路径中获取顶点/片段着色器
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // 保证ifstream对象可以抛出异常：
        // vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        // fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            // 打开文件
            vShaderFile.open(vSdPath);
            fShaderFile.open(fSdPath);
            std::stringstream vShaderStream, fShaderStream;
            // 读取文件的缓冲内容到数据流中
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // 关闭文件处理器
            vShaderFile.close();
            fShaderFile.close();
            // 转换数据流到string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();

        //
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vShaderCode, NULL);
        glCompileShader(vertexShader);
        // fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
        glCompileShader(fragmentShader);
        // link shaders
        this->id = glCreateProgram();
        glAttachShader(this->id, vertexShader);
        glAttachShader(this->id, fragmentShader);
        glLinkProgram(this->id);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void use() {
        glUseProgram(this->id);
    }
};

/**
 * 使用opengl渲染
 */

class Render {
public:
    FT_Library ft = nullptr;
    FT_Face face = nullptr;
    
    Render() {
        // font 初始化
        if (FT_Init_FreeType(&this->ft))
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        if (FT_New_Face(this->ft, "res/fonts/arial.ttf", 0, &this->face))
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        // 动态 width
        FT_Set_Pixel_Sizes(this->face, 0, 48);
        // FT_Set_Char_Size()
    }

    ~Render() {
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    bool init();

    void typeText(const char *, std::vector<std::string> &, int, Rect &);
    void drawText(const char *, int, float, float);

    void typeImage(std::string &, float, float, Rect &);
    void drawImage(std::string &, Rect &);

    void drawRect(Shader &, Rect &, GLuint);

    void drawShape();
};

#include "render.hpp"

#include "../defs.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <map>
#include <string>

// init shader
Shader *priShader;
Shader *fontShader;

// gl
unsigned int EBO;
unsigned int priVBO, priVAO;
unsigned int texVBO, texVAO;

bool Render::init() {
    //
    priShader = new Shader("res/shader/primary.vss", "res/shader/primary.fss");
    fontShader = new Shader("res/shader/font.vss", "res/shader/font.fss");

    // todo
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 索引
    glGenBuffers(1, &EBO);
    int index[] = {
        0, 1, 2, //
        0, 1, 3, //
    };

    // set priVAO/priVBO
    glGenVertexArrays(1, &priVAO);
    glGenBuffers(1, &priVBO);

    glBindVertexArray(priVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, priVBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
    // 位置
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    /*******************************/
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // set texVAO/texVBO
    glGenVertexArrays(1, &texVAO);
    glGenBuffers(1, &texVBO);

    glBindVertexArray(texVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
    // 位置
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    // 纹理坐标
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    /*******************************/
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 纹理对齐
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    return true;
}

/*******************************************************/

void checkCh(Render *pthis, char c) {
    if (!pthis->Characters.count(c)) {
        if (FT_Load_Char(pthis->face, c, FT_LOAD_RENDER)) {
            // todo
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            return;
        }

        // 生成纹理
        unsigned int texture;
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            pthis->face->glyph->bitmap.width,
            pthis->face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            pthis->face->glyph->bitmap.buffer);
        // 设置纹理选项
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 储存字符供之后使用
        Character character = {
            texture,
            {pthis->face->glyph->bitmap.width, pthis->face->glyph->bitmap.rows},
            {pthis->face->glyph->bitmap_left, pthis->face->glyph->bitmap_top},
            pthis->face->glyph->advance.x};
        pthis->Characters.insert(std::pair<char, Character>(c, character));
    }
}

void Render::typeText(const char *str, std::vector<std::string> &lines, int size, Rect &rect) {
    float w = rect.w, h = 0;
    float maxh = 0, maxl = 0;
    auto line = new std::string();
    while (*str != '\0') {
        checkCh(this, *str);
        auto &ch = this->Characters[*str];
        w -= (ch.Advance >> 6) * size / 48;
        if (w < 0) {
            lines.push_back(*line);
            line = new std::string();
            w = rect.w;
            str--;
        } else {
            *line += *str;
            maxh = maxh < ch.Bearing[1] ? ch.Bearing[1] : maxh;
            maxl = maxl < ch.Size[1] - ch.Bearing[1] ? ch.Size[1] - ch.Bearing[1] : maxl;
        }
        str++;
    }
    lines.push_back(*line);
    w = rect.w;
    h = lines.size() * (maxh + maxl) * 1.1;
    rect.setLeftTop(rect.x, rect.y, w, h);
}

void Render::drawText(const char *str, int size, float x, float y) {
    Rect rect;
    float scale = size / 48;
    while (*str != '\0') {
        checkCh(this, *str);
        auto &ch = this->Characters[*str];
        rect.set(x + ch.Bearing[0] * scale, y - (ch.Size[1] - ch.Bearing[1]) * scale,
                 ch.Size[0] * scale, ch.Size[1] * scale);
        this->drawRect(*fontShader, rect, ch.TextureID);
        x += (ch.Advance >> 6) * scale;
        str++;
    }
}

/*********************************************************/

struct _PIC_INFO {
    u_int tex;
    int width;
    int height;
} typedef PIC_INFO;

std::map<std::string, PIC_INFO> pic_buffer;

void checkImg(std::string &imgPath) {
    if (!pic_buffer.count(imgPath)) {
        unsigned int texture;
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        // 为当前绑定的纹理对象设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 加载并生成纹理
        int width, height, nrChannels;
        unsigned char *data = stbi_load(imgPath.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
        pic_buffer.insert(std::pair<std::string, PIC_INFO>(imgPath, {texture, width, height}));
    }
}

void Render::typeImage(std::string &imgPath, float width, float height, Rect &rect) {
    checkImg(imgPath);
    auto info = pic_buffer[imgPath];
    if (width && height) {
        rect.w = width;
        rect.h = height;
    } else if (!width && !height) {
        rect.w = info.width;
        rect.h = info.height;
    } else {
        if (width) {
            rect.w = width;
            rect.h = width / info.width * info.height;
        } else if (height) {
            rect.h = height;
            rect.w = height / info.height * info.width;
        }
    }
    rect.resetLeftTop();
}

void Render::drawImage(std::string &imgPath, Rect &rect) {
    checkImg(imgPath);
    auto info = pic_buffer[imgPath];
    this->drawRect(*priShader, rect, info.tex);
}

/*********************************************************/

void Render::drawRect(Shader &shader, Rect &rect, GLuint texId) {
    auto window = WindowManager.currentWindow->first;
    if (texId) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texId);
        glUniform1i(glGetUniformLocation(shader.id, "tex"), 0); // 手动设置
        float vertices[6][4] = {
            {rect.points[0][0] * 2 / window->width - 1, rect.points[0][1] * 2 / window->height + 1, 0.0f, 0.0f},
            {rect.points[1][0] * 2 / window->width - 1, rect.points[1][1] * 2 / window->height + 1, 1.0f, 1.0f},
            {rect.points[2][0] * 2 / window->width - 1, rect.points[2][1] * 2 / window->height + 1, 0.0f, 1.0f},
            {rect.points[3][0] * 2 / window->width - 1, rect.points[3][1] * 2 / window->height + 1, 1.0f, 0.0f},
        };
        glBindBuffer(GL_ARRAY_BUFFER, texVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindVertexArray(texVAO);
    } else {
        float vertices[6][2] = {
            {rect.points[0][0] * 2 / window->width - 1, rect.points[0][1] * 2 / window->height - 1},
            {rect.points[1][0] * 2 / window->width - 1, rect.points[1][1] * 2 / window->height - 1},
            {rect.points[2][0] * 2 / window->width - 1, rect.points[2][1] * 2 / window->height - 1},
            {rect.points[3][0] * 2 / window->width - 1, rect.points[3][1] * 2 / window->height - 1},
        };
        glBindBuffer(GL_ARRAY_BUFFER, priVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);
        glBindVertexArray(priVAO);
    }
    shader.use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFlush();
}
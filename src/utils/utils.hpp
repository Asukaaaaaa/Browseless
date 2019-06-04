#pragma once

enum RectState {
    Left_Bottom,
    Left_Top
};

class Rect {
public:
    float x = 0, y = 0;
    float w = 0, h = 0;
    float points[4][2] = {0};
    RectState state = Left_Bottom;

    Rect() {
        //
    }

    Rect(float x, float y, float w, float h) {
        this->set(x, y, w, h);
    }

    void set(float x, float y, float w, float h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        auto &p = this->points;
        {
            p[0][0] = x;
            p[0][1] = y + h;
            p[1][0] = x + w;
            p[1][1] = y;
            p[2][0] = x;
            p[2][1] = y;
            p[3][0] = x + w;
            p[3][1] = y + h;
        }
        this->state = Left_Bottom;
    }

    void setLeftTop(float x, float y, float w, float h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        auto &p = this->points;
        {
            p[0][0] = x;
            p[0][1] = y;
            p[1][0] = x + w;
            p[1][1] = y - h;
            p[2][0] = x;
            p[2][1] = y - h;
            p[3][0] = x + w;
            p[3][1] = y;
        }
        this->state = Left_Top;
    }

    void reset() {
        this->set(this->x, this->y, this->w, this->h);
    }

    void resetLeftTop() {
        this->setLeftTop(this->x, this->y, this->w, this->h);
    }

    void trans() {
        // todo
    }
};
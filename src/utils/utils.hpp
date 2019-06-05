#pragma once

class Rect {
public:
    float x = 0, y = 0;
    float w = 0, h = 0;
    float points[4][2] = {0};

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
            p[0][1] = y;
            p[1][0] = x + w;
            p[1][1] = y + h;
            p[2][0] = x;
            p[2][1] = y + h;
            p[3][0] = x + w;
            p[3][1] = y;
        }
    }

    void reset() {
        this->set(this->x, this->y, this->w, this->h);
    }

    void trans(int w, int h) {
        auto &p = this->points;
        {
            p[0][0] = 2 * p[0][0] / w - 1;
            p[0][1] = -2 * p[0][1] / h + 1;
            p[1][0] = 2 * p[1][0] / w - 1;
            p[1][1] = -2 * p[1][1] / h + 1;
            p[2][0] = 2 * p[2][0] / w - 1;
            p[2][1] = -2 * p[2][1] / h + 1;
            p[3][0] = 2 * p[3][0] / w - 1;
            p[3][1] = -2 * p[3][1] / h + 1;
        }
    }
};
#pragma once

#include <string>
#include <vector>

#include "../utils/utils.hpp"

class Node {
public:
    Rect rect;
    Rect contentRect;
    Node *father = nullptr;
    std::vector<Node *> childs;

    Node() {
        this->rect.setLeftTop(0, 0, 0, 0);
    }

    Node(Node *node) {
        this->father = node;
        this->rect.setLeftTop(node->contentRect.points[2][0], node->contentRect.points[2][1], node->contentRect.w, 0);
    }

    ~Node() {
        // todo
    }

    int appendChild(Node *node) {
        if (node) {
            node->father = this;
            this->childs.push_back(node);
            return this->childs.size();
        } else
            return 0;
    };

    virtual void computeSelf() {
        if (this->father) {
            auto &r = this->father->rect;
            auto &p = r.points[2];
            this->contentRect.setLeftTop(p[0], p[1], r.w, 0);
            this->rect.setLeftTop(p[0], p[1], r.w, 0);
        }
    }

    virtual Rect *compute() {
        this->computeSelf();
        for (auto child : this->childs) {
            auto rect = child->compute();
            this->rect.h += rect->h;
            this->rect.resetLeftTop();
        }
        return &this->rect;
    };

    virtual void drawSelf() {
        // todo draw color
    }

    virtual void draw() {
        this->drawSelf();
        for (auto child : this->childs) {
            child->draw();
        }
    };
};

class TextNode : public Node {
public:
    const char *str = nullptr;
    std::vector<std::string> lines;
    int size;

    TextNode(const char *str, int size = 48) : str(str) {
        this->size = size;
    }

    ~TextNode() {
        //
    }

    void computeSelf();
    void drawSelf();
};

class ImgNode : public Node {
public:
    std::string path;
    int width;
    int height;

    ImgNode(const char *str, float w = 0, float h = 0) : path(str) {
        this->width = w;
        this->height = h;
    }

    ~ImgNode() {
        //
    }

    void computeSelf();
    void drawSelf();
};

class Dom {
public:
    Node root;

    Dom() {
        this->root.rect.setLeftTop(0, 0, 800, 0);
        this->root.contentRect.setLeftTop(0, 0, 800, 0);
    }

    ~Dom() {
        //
    }

    void load(std::string);

    void release();

    void draw() {
        this->root.draw();
    }
};

#include "dom.hpp"
#include "../defs.hpp"

// Node

// TextNode

void TextNode::computeSelf() {
    this->Node::computeSelf();
    MainRender.typeText(this->str, this->lines, this->size, this->contentRect);
    auto r = this->contentRect;
    this->rect.setLeftTop(r.x, r.y, r.w, r.h);
}

void TextNode::drawSelf() {
    int n = this->lines.size();
    for (int i = 0; i < n; ++i) {
        auto line = this->lines[i];
        MainRender.drawText(line.c_str(), this->size, this->rect.points[2][0], this->rect.points[2][1] + this->rect.h * i / size);
    }
}

// ImgNode

void ImgNode::computeSelf() {
    //
}

void ImgNode::drawSelf() {
    //
}

// Dom

// todo
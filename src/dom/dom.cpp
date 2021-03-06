#include "dom.hpp"

#include "../defs.hpp"

#include <pugixml.hpp>

#include <string>

// Node

// TextNode

void TextNode::computeSelf() {
    this->Node::computeSelf();
    MainRender.typeText(this->str, this->lines, this->size, this->contentRect);
    this->rect.h = this->contentRect.h;
    this->rect.reset();
}

void TextNode::drawSelf() {
    int n = this->lines.size();
    for (int i = 0; i < n; ++i) {
        auto line = this->lines[i];
        MainRender.drawText(line.c_str(), this->size, this->rect.points[0][0], this->rect.points[0][1] + this->rect.h * (i + 1) / n);
    }
}

// ImgNode

void ImgNode::computeSelf() {
    this->Node::computeSelf();
    MainRender.typeImage(this->path, this->width, this->height, this->contentRect);
    this->rect.h = this->contentRect.h;
    this->rect.reset();
}

void ImgNode::drawSelf() {
    MainRender.drawImage(this->path, this->contentRect);
}

// Dom

void childGen(pugi::xml_node &xnode, Node *node) {
    for (auto xchild : xnode) {
        std::string tag = xchild.name();
        Node *child;
        if (tag == "div") {
            child = new Node();
        } else if (tag == "p") {
            child = new TextNode(xchild.child_value());
        } else if (tag == "img") {
            child = new ImgNode(xchild.attribute("src").value(), xchild.attribute("width").as_float(), xchild.attribute("height").as_float());
        } else {
            return;
        }
        node->appendChild(child);
        childGen(xchild, child);
    }
}

void Dom::load(std::string path) {
    pugi::xml_document doc;
    if (doc.load_file(path.c_str())) {
        pugi::xml_node root = doc.child("root");
        childGen(root, &this->root);
    }
}

// todo
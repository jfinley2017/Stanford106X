/*
 * CS 106B/X Tiles
 * This file contains the declaration of the TileNode structure.
 *
 * Please DO NOT MODIFY this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp
 * @version 2016/11/03
 * - 16au 106X initial version
 */

#include "tilenode.h"
#include <sstream>

// declare static variables
int TileNode::nextNodeNumber = 1;
int TileNode::instanceCount = 0;

TileNode::TileNode(int x, int y, int width, int height,
         const std::string& color, TileNode* prev, TileNode* next)
        : x(x), y(y), width(width), height(height), color(color),
          prev(prev), next(next), number(nextNodeNumber++) {
    instanceCount++;
}

TileNode::~TileNode() {
    if (instanceCount > 0) {
        instanceCount--;
    }
}

// implementation note:
// 'contains' is <= bottom/right side because of 1px border width
bool TileNode::contains(int x, int y) const {
    return this->x <= x && x <= this->x + this->width
        && this->y <= y && y <= this->y + this->height;
}

void TileNode::draw(GWindow& window) const {
    window.setColor(color);
    window.fillRect(x, y, width, height);
    window.setColor(0x0);   // black
    window.drawString("#" + integerToString(number), x + 2, y + 15);
    window.drawRect(x, y, width, height);
}

std::string TileNode::toString() const {
    std::ostringstream out;
    out << *this;   // calls operator <<
    return out.str();
}

std::ostream& operator <<(std::ostream& out, const TileNode& node) {
    return out << "Tile #" << node.number << " @" << &node
            << "{x=" << node.x << ",y=" << node.y
            << ",w=" << node.width << ",h=" << node.height
            << ",color=\"" << node.color << "\""
            << ",prev=" << node.prev << ",next=" << node.next << "}";
}

int TileNode::getInstanceCount() {
    return instanceCount;
}

void TileNode::resetInstanceCount() {
    instanceCount = 0;
}

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

#pragma once

#include <iostream>
#include <string>
#include "gobjects.h"
#include "gwindow.h"

/*
 * Each Tile structure represents a single rectangular tile in the simulation.
 * A tile knows its position, size, and color.
 * It contains members that can draw itself, tell you whether it touches a
 * given x/y pixel, and print it on the console for debugging.
 */
struct TileNode {
public:
    const int x;            // (x, y) position of tile's top-left corner
    const int y;
    const int width;        // tile's dimensions in pixels
    const int height;
    std::string color;      // tile's color as a hex RRGGBB string like "#ff00ff" for purple

    TileNode* prev;         // prev tile node pointer, if any
    TileNode* next;         // next tile node pointer, if any

    /*
     * Constructor initializes all members to have the given values.
     * All parameters after x/y/w/h are optional;
     * any omitted parameters will be set to zero-equivalent values.
     *
     * Examples:
     * TileNode* node1 = new TileNode(x, y, w, h, color);
     * TileNode* node2 = new TileNode(x, y, w, h, color, prev, next);
     */
    TileNode(int x, int y, int width, int height,
             const std::string& color = "",
             TileNode* prev = nullptr, TileNode* next = nullptr);

    /*
     * Destructor; called implicitly when nodes are deleted.
     */
    ~TileNode();

    /*
     * Returns true if the given x/y coordinate is found inside the tile.
     */
    bool contains(int x, int y) const;

    /*
     * Draws the tile on the given window.
     */
    void draw(GWindow& window) const;

    /*
     * Returns a string representation of the tile in the format of operator <<.
     */
    std::string toString() const;

private:
    // unique number of each node; set internally
    const int number;

    // counters of all nodes that have ever been created
    static int nextNodeNumber;
    static int instanceCount;

    // declare operator << as friend so it can see node number variable
    friend std::ostream& operator <<(std::ostream& out, const TileNode& node);

    // helper methods for grading
    static int getInstanceCount();
    static void resetInstanceCount();
};

/*
 * Outputs the given tile to the given output stream (e.g. cout) in the same
 * format as returned by its toString member function.
 * The tile's memory address, data, and next pointer are all printed.
 *
 * If you have a non-NULL pointer TileNode* p, you can print the node by writing:
 * cout << *p << endl;
 */
std::ostream& operator <<(std::ostream& out, const TileNode& node);


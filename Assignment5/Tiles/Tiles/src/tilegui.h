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
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"
#include "tilelist.h"

class TileGUI {
public:
    // constants
    // set this to false for the same graphical output every time
    static const bool RANDOM = true;

    /*
     * Frees the memory used by the GUI.
     */
    ~TileGUI();

    /*
     * Creates one or more new tiles with a random x/y position,
     * width, height, and color, and adds it to the given tile list.
     */
    void addRandomTiles(TileList& tlist, int count = 1, bool toFront = true);

    /*
     * Responds to events that occur in the GUI window.
     */
    void eventLoop();

    /*
     * Constructs the graphical window and widgets on the screen.
     */
    void initialize();

private:
    bool processEvent(TileList& tlist);
    bool processActionEvent(GActionEvent& actionEvent, TileList& tlist);
    bool processKeyEvent(GKeyEvent& keyEvent, TileList& tlist);
    bool processMouseEvent(GMouseEvent& mouseEvent, TileList& tlist);

    GWindow* window;
    GButton* gbAddFront;
    GButton* gbAddBack;
    GButton* gbClear;
    GButton* gbDebug;
    GRadioButton* grbHighlight;
    GRadioButton* grbRaise;
    GRadioButton* grbLower;
    GRadioButton* grbRemove;
    GRadioButton* grbRemoveAll;
    GTextField* gtfAddNumber;
    GLabel* glMousePos;

    // constants
    static const int WINDOW_WIDTH = 600;
    static const int WINDOW_HEIGHT = 500;
    static const int MIN_SIZE = 50;
    static const int MAX_SIZE = 150;
    static const int MIN_COLOR = 50;
    static const int MAX_COLOR = 255;
    static const int TILE_COUNT = 1;
};


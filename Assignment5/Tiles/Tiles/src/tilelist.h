// This is the H file you will edit and turn in. (TODO: Remove this comment!)

#pragma once

#include <string>
#include "gwindow.h"
#include "tilenode.h"
using namespace std;

class TileList {
public:
    TileList();
    ~TileList();
    /**
     * @brief addFront - adds a new tile to the top
     */
    void addFront(int x, int y, int width, int height, string color);

    /**
     * @brief addBack - adds a new tile to the back
     */
    void addBack(int x, int y, int width, int height, string color);

    /**
     * @brief clear - clears the window, removing all tiles
     * destroys the list, deleting all memory allocated on the heap
     */
    void clear();
    void debug();

    /**
     * @brief drawAll - iterates over the list back to front, drawing tiles in the correct order
     */
    void drawAll(GWindow& window);

    /**
     * @brief highlight - turns the tile which contains the x,y coordinate yellow
     */
    void highlight(int x, int y);

    /**
     * @brief lower - pushed the tile containing the x,y coordinate to the back
     */
    void lower(int x, int y);

    /**
     * @brief raise - raises the tile at the x,y coordinate to the front
     */
    void raise(int x, int y);

    /**
     * @brief remove - removes the tile which contains the x,y coordinate
     */
    void remove(int x, int y);

    /**
     * @brief lower - pushed the tile containing the x,y coordinate to the back
     */
    void removeAll(int x, int y);

private:
    /**
    * @brief List pointers.
    * Top elements are at the front, back elements are at the back.
    * To draw in proper order, iterate back to front.
    */
   TileNode* front = nullptr;
   TileNode* back = nullptr;

   /**
    * @brief getTile - returns the tile which contains the x,y coordinate
    */
   TileNode* getTile(int x, int y);

   /**
    * @brief removeNode - overloads remove to allow for a tile to be directly removed
    * without lookup
    * @param node - node to remove
    */
   void remove(TileNode* node);

   /**
    * @brief removeFront - removes front (top) tile
    */
   void removeFront();

   /**
    * @brief removeBack - removes back (bottom) tile
    */
   void removeBack();

};


// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include "tilelist.h"

TileList::TileList() {
    // TODO: write this constructor
}

TileList::~TileList() {
    clear();
}

void TileList::debug() {
    // TODO: write this function (optional)
}

/**
 * @brief addFront - adds a new tile to the top
 */
void TileList::addFront(int x, int y, int width, int height, string color) {
    TileNode* newTile = new TileNode(x,y,width,height,color);
    if(front == nullptr)
    {
        front = newTile;
        back = newTile;
        return;
    }
    else
    {
        TileNode* temp = front;
        newTile->next = temp;
        temp->prev = newTile;
        front = newTile;
    }
}

/**
 * @brief addBack - adds a new tile to the back
 */
void TileList::addBack(int x, int y, int width, int height, string color) {
    TileNode* newTile = new TileNode(x,y,width,height,color);
    if(back == nullptr)
    {
        front = newTile;
        back = newTile;
        return;
    }
    else
    {
        newTile->prev = back;
        back->next = newTile;
        back = newTile;
    }
}

/**
 * @brief clear - clears the window, removing all tiles
 * destroys the list, deleting all memory allocated on the heap
 */
void TileList::clear() {
    TileNode* trash = front;
    while(trash != nullptr)
    {
        front = front->next;
        delete trash;
        trash = front;
    }
    back = nullptr;
}

/**
 * @brief drawAll - iterates over the list back to front, drawing tiles in the correct order
 */
void TileList::drawAll(GWindow& window) {
    TileNode* current = back;
    while(current != nullptr)
    {
        window.setColor(current->color);
        window.fillRect(current->x,current->y,current->width,current->height);
        current = current->prev;
    }
}

/**
 * @brief highlight - turns the tile which contains the x,y coordinate yellow
 */
void TileList::highlight(int x, int y) {
    TileNode* tileToHighlight = getTile(x,y);
    if(tileToHighlight)
    {
        tileToHighlight->color = "yellow";
    }
}

/**
 * @brief lower - pushed the tile containing the x,y coordinate to the back
 */
void TileList::lower(int x, int y){
    TileNode* tileToLower = getTile(x,y);
    if(tileToLower)
    {
        addBack(tileToLower->x, tileToLower->y, tileToLower->width, tileToLower->height, tileToLower->color);
        remove(tileToLower);
    }

}

/**
 * @brief raise - raises the tile at the x,y coordinate to the front
 */
void TileList::raise(int x, int y) {
    TileNode* tileToRaise = getTile(x,y);
    if(tileToRaise)
    {
        addFront(tileToRaise->x,tileToRaise->y,tileToRaise->width, tileToRaise->height,tileToRaise->color);
        remove(tileToRaise);
    }
}

/**
 * @brief remove - removes the tile which contains the x,y coordinate
 */
void TileList::remove(int x, int y) {
    TileNode* trash = getTile(x,y);
    remove(trash);
}

/**
 * @brief remove - overloads remove to allow for a tile to be directly removed
 * without lookup
 * @param node - node to remove
 */
void TileList::remove(TileNode* node){
    TileNode* trash = node;
    if(trash)
    {
       if(trash == front)
       {
           removeFront();
           return;
       }
       if(trash == back)
       {
           removeBack();
           return;
       }
       TileNode* current = trash->prev;
       current->next = trash->next;
       current->next->prev = current;
       delete trash;
    }
}

/**
 * @brief lower - pushed the tile containing the x,y coordinate to the back
 */
void TileList::removeAll(int x, int y) {
    TileNode* current = front;
    while(current)
    {
        if(current->contains(x,y))
        {
            remove(current);
        }
        current = current->next;
    }
}

/**
 * @brief getTile - returns the tile which contains the x,y coordinate
 */
TileNode* TileList::getTile(int x, int y) {
    TileNode* current = front;
    while(current != nullptr)
    {
        if(current->contains(x,y))
        {
            return current;
        }
        current=current->next;
    }
    return nullptr;
}

/**
 * @brief removeFront - removes front (top) tile
 */
void TileList::removeFront() {
    TileNode* trash = front;
    if(trash)
    {
        front = front->next;
        if(front)
        {
            front->prev = nullptr;
        }
        else
        {
            back = nullptr;
        }
        delete trash;
    }
}

/**
 * @brief removeBack - removes back (bottom) tile
 */
void TileList::removeBack() {
    TileNode* trash = back;
    if(trash)
    {
        back = back->prev;
        if(back)
        {
            back->next = nullptr;
        }
        else
        {
            front = nullptr;
        }
        delete trash;
    }

}

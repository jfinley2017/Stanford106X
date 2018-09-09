/*
 * CS 106B/X Tiles
 * This GUI uses your TileList class and contains the 'main'
 * function to set up the overall program's graphical user interface.
 * Some commands print debug messages that appear on the bottom console.
 *
 * Please DO NOT MODIFY this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp
 * @version 2016/11/03
 * - 16au 106X initial version
 */

#include "tilegui.h"
#include <iomanip>
#include "random.h"
#include "strlib.h"

// commenting out due to some kind of GUI memory bug; not a big deal
TileGUI::~TileGUI() {
//    delete gbAddFront;
//    delete gbAddBack;
//    delete gbClear;
//    delete gbDebug;
//    delete grbHighlight;
//    delete grbRaise;
//    delete grbLower;
//    delete grbRemove;
//    delete grbRemoveAll;
//    delete glMousePos;
//    delete window;
}

void TileGUI::addRandomTiles(TileList& tlist, int count, bool toFront) {
    for (int i = 0; i < count; i++) {
        // pick random coords and size
        int width = randomInteger(MIN_SIZE, MAX_SIZE);
        int height = randomInteger(MIN_SIZE, MAX_SIZE);
        int x = randomInteger(0, WINDOW_WIDTH - width);
        int y = randomInteger(0, WINDOW_HEIGHT - height);

        // pick random color
        int r = randomInteger(MIN_COLOR, MAX_COLOR);
        int g = randomInteger(MIN_COLOR, MAX_COLOR);
        int b = randomInteger(MIN_COLOR, MAX_COLOR);
        std::ostringstream colorout;
        colorout << "#" << std::setbase(16) << std::setfill('0') << r << g << b;
        std::string color = colorout.str();

        // add to top of tile list
        std::cout << (toFront ? "addFront" : "addBack")
                  << " (x=" << x << ", y=" << y << ", w=" << width
                  << ", h=" << height << ", color=\"" << color << "\")" << std::endl;
        if (toFront) {
            tlist.addFront(x, y, width, height, color);
        } else {
            tlist.addBack(x, y, width, height, color);
        }
    }
}

void TileGUI::eventLoop() {
    TileList tlist;

    // event loop waits for user to click on tiles
    while (true) {
        if (processEvent(tlist)) {
            window->clearCanvas();
            std::cout << "drawAll" << std::endl;
            tlist.drawAll(*window);
            window->repaint();
        } else if (!window->isVisible()) {
            // if the user closes the main graphical window, shut down
            break;
        }
    }
}

void TileGUI::initialize() {
    window = new GWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    window->center();
    window->setWindowTitle("CS 106X Tiles");
    window->setRepaintImmediately(false);

    gbAddFront = new GButton("Add Front");
    gbAddFront->setMnemonic('F');
    gbAddFront->setAccelerator("ctrl F");

    gbAddBack = new GButton("Add Back");
    gbAddBack->setMnemonic('B');
    gbAddBack->setAccelerator("ctrl B");

    gbClear = new GButton("Clear");

    gbDebug = new GButton("Debug");

    grbHighlight = new GRadioButton("Highlight");
    grbHighlight->setMnemonic('H');
    grbHighlight->setAccelerator("ctrl H");
    grbHighlight->setSelected(true);

    grbRaise = new GRadioButton("Raise");
    grbRaise->setMnemonic('R');
    grbRaise->setAccelerator("ctrl R");

    grbLower = new GRadioButton("Lower");
    grbLower->setMnemonic('L');
    grbLower->setAccelerator("ctrl L");

    grbRemove = new GRadioButton("Remove");
    grbRemove->setMnemonic('m');
    grbRemove->setAccelerator("ctrl M");

    grbRemoveAll = new GRadioButton("Remove All");
    grbRemoveAll->setMnemonic('v');
    grbRemoveAll->setAccelerator("ctrl V");

    gtfAddNumber = new GTextField(2);
    gtfAddNumber->setText(integerToString(TILE_COUNT));
    gtfAddNumber->requestFocus();

    glMousePos = new GLabel("(x=???, y=???)");

    window->addToRegion(grbHighlight, "North");
    window->addToRegion(grbRaise, "North");
    window->addToRegion(grbLower, "North");
    window->addToRegion(grbRemove, "North");
    window->addToRegion(grbRemoveAll, "North");
    window->addToRegion(gbAddFront, "South");
    window->addToRegion(gbAddBack, "South");
    window->addToRegion(gtfAddNumber, "South");
    window->addToRegion(gbClear, "South");
    window->addToRegion(glMousePos, "South");
    window->addToRegion(gbDebug, "South");
}


/*
 * Waits for one mouse or window event from the graphical subsystem,
 * then handles the event appropriately if it is a mouse button press,
 * telling the tile list to highlight / raise / lower / delete etc.
 * Returns true if an event was processed (and TileList needs to be redrawn).
 */
bool TileGUI::processEvent(TileList& tlist) {
    GEvent event = waitForEvent(MOUSE_EVENT | KEY_EVENT | WINDOW_EVENT | ACTION_EVENT);
    int eventClass = event.getEventClass();
    if (eventClass == ACTION_EVENT) {
        GActionEvent actionEvent(event);
        return processActionEvent(actionEvent, tlist);
    } else if (eventClass == MOUSE_EVENT) {
        GMouseEvent mouseEvent(event);
        return processMouseEvent(mouseEvent, tlist);
    } else if (eventClass == KEY_EVENT) {
        GKeyEvent keyEvent(event);
        return processKeyEvent(keyEvent, tlist);
    } else {
        return false;
    }
}

bool TileGUI::processActionEvent(GActionEvent& actionEvent, TileList& tlist) {
    std::string cmd = actionEvent.getActionCommand();
    if (cmd == "Add Front") {
        if (gtfAddNumber->valueIsInteger()) {
            int count = gtfAddNumber->getValueAsInteger();
            addRandomTiles(tlist, count, /* toFront */ true);
            return true;
        }
    } else if (cmd == "Add Back") {
        if (gtfAddNumber->valueIsInteger()) {
            int count = gtfAddNumber->getValueAsInteger();
            addRandomTiles(tlist, count, /* toFront */ false);
            return true;
        }
    } else if (cmd == "Clear") {
        std::cout << "clear" << std::endl;
        tlist.clear();
        return true;
    } else if (cmd == "Debug") {
        std::cout << "debug" << std::endl;
        tlist.debug();
        return true;
    }
    return false;
}

bool TileGUI::processKeyEvent(GKeyEvent& keyEvent, TileList& tlist) {
    int type = keyEvent.getEventType();
    if (type == KEY_PRESSED) {
        if (tolower(keyEvent.getKeyChar()) == 'a') {
            // listen for 'A' to add a new random tile
            addRandomTiles(tlist, /* count */ 1,
                           /* toFront */ !keyEvent.isShiftKeyDown());
            return true;
        } else if (tolower(keyEvent.getKeyChar()) == 'c') {
            // listen for 'C' to clear all tiles
            tlist.clear();
            return true;
        }
    }
    return false;
}

bool TileGUI::processMouseEvent(GMouseEvent& mouseEvent, TileList& tlist) {
    int x = (int) mouseEvent.getX();
    int y = (int) mouseEvent.getY();
    int type = mouseEvent.getEventType();
    if (type == MOUSE_PRESSED) {
        if (grbHighlight->isSelected()) {
            std::cout << "highlight (x=" << x << ", y=" << y << ")" << std::endl;
            tlist.highlight(x, y);
        } else if (grbRaise->isSelected()) {
            std::cout << "raise (x=" << x << ", y=" << y << ")" << std::endl;
            tlist.raise(x, y);
        } else if (grbLower->isSelected()) {
            std::cout << "lower (x=" << x << ", y=" << y << ")" << std::endl;
            tlist.lower(x, y);
        } else if (grbRemove->isSelected()) {
            std::cout << "remove (x=" << x << ", y=" << y << ")" << std::endl;
            tlist.remove(x, y);
        } else if (grbRemoveAll->isSelected()) {
            std::cout << "removeAll (x=" << x << ", y=" << y << ")" << std::endl;
            tlist.removeAll(x, y);
        }
        return true;
    } else if (type == MOUSE_MOVED) {
        std::ostringstream out;
        out << "(x=" << std::setw(3) << std::setfill('0') << x << ", y="
            << std::setw(3) << std::setfill('0') << y << ")";
        glMousePos->setText(out.str());
    }
    return false;
}


// main function for overall program
int main() {
    // possibly use the same random numbers every time for testing
    if (!TileGUI::RANDOM) {
        setRandomSeed(106);
    }

    // set up GUI and widgets
    TileGUI gui;
    gui.initialize();
    gui.eventLoop();

    return 0;
}

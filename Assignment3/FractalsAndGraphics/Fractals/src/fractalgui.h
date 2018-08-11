/*
 * CS 106B/X Fractal Recursion Problems
 * This instructor-provided file declares a class representing the overall
 * graphical user interface (GUI) for this part of the program.
 * See fractalgui.cpp for the implementation of each member.
 *
 * You don't need to modify this file.
 * Your recursive functions should work with an unmodified version of this file.
 *
 * @author Marty Stepp
 * @version 2016/10/16
 * - modified for 16au with additional fractals (tree)
 * @version 2015/04/24
 * - modified for 15sp version, including exception-handling
 * @version 2014/10/13
 * - modified for 14au version, including new version of floodFill
 */

#ifndef _fractalgui_h
#define _fractalgui_h

#include <string>
#include "gevents.h"
#include "ginteractors.h"
#include "gobjects.h"
#include "gwindow.h"
#include "map.h"

class FractalGUI {
public:
    /*
     * Removes all shapes from the screen.
     */
    void clear();

    /*
     * The GUI's main loop that waits for events and handles them.
     */
    void eventLoop();

    /*
     * Creates and configures all graphical controls in the GUI.
     */
    void initialize();

private:
    /*
     * Initiates a flood fill operation on the window.
     */
    void doFloodFill(double x, double y);

    /*
     * Initiates a draw Sierpinski triangle operation on the window.
     */
    void doSierpinski();

    /*
     * Initiates a recursive tree operation on the window.
     */
    void doTree();

    /*
     * Adds a bunch of rectangles to the screen for use with the floodFill function.
     */
    void floodFill_addShapes();

    /*
     * Returns a map of available colors for flood fill.
     */
    const Map<std::string, int>& getColorMap();

    /*
     * Processes one action event (button click) in the GUI window.
     */
    void handleActionEvent(GActionEvent& actionEvent);

    /*
     * Processes one mouse event in the GUI window.
     */
    void handleMouseEvent(GMouseEvent& mouseEvent);

    /*
     * Returns true if all text fields have valid integer coordinates entered into them.
     */
    bool hasAllCoords();

    /*
     * Sets the bottom status bar text to display in the southern area.
     * If isError is true, shows it in a red error color.
     */
    void setStatusText(const std::string& text, bool isError = false);

    // constants
    static const int WINDOW_WIDTH = 700;
    static const int WINDOW_HEIGHT = 550;
    static const int FLOOD_FILL_NUM_SHAPES = 100;
    static const int FLOOD_FILL_MIN_RECT_SIZE = 20;
    static const int FLOOD_FILL_MAX_RECT_SIZE = 100;
    static const int DRAG_RECT_COLOR = 0xaa8822;

    // to 'rig' random number generator; set to <= 0 to disable
    static const int FLOOD_FILL_RANDOM_SEED = 106;

    // overall window
    GWindow* window;

    // west buttons to draw fractals
    GButton* sierpinskiButton;
    GButton* treeButton;
    GButton* floodFillButton;
    GButton* diffButton;
    GButton* clearButton;

    // flood fill colors drop-down
    GChooser* colorChooser;

    // north text fields for figure parameters
    GTextField* xField;
    GTextField* yField;
    GTextField* sizeField;
    GTextField* orderField;
    GCheckBox* animationBox;

    // south status bar
    GLabel* statusBar;

    // temporary dragging rectangle for figure size
    GRect* dragRect;

    // temporary flags to remember if we are in midst of a flood fill
    bool floodFillMode = false;
    bool floodFillInProgress = false;
};

#endif // _fractalgui_h

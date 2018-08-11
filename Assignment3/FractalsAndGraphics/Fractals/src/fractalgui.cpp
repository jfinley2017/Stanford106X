/*
 * CS 106B/X Fractal Recursion Problems
 * This instructor-provided file defines the members of a class representing the
 * overall graphical user interface (GUI) for this part of the program.
 * See fractalgui.h for the class and member declarations.
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

#include "fractalgui.h"
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "filelib.h"
#include "gfilechooser.h"
#include "grid.h"
#include "random.h"
#include "strlib.h"
#include "timer.h"
#include "simpio.h"
#include "fractals.h"

void FractalGUI::clear() {
    window->clearCanvas();
    if (dragRect) {
        delete dragRect;
        dragRect = nullptr;
    }
}

void FractalGUI::doFloodFill(double x, double y) {
    colorChooser->setEnabled(false);
    std::string colorStr = colorChooser->getSelectedItem();
    int color = getColorMap()[colorStr];

    std::ostringstream out;
    out << "Flood fill: x=" << std::dec << x << " y=" << y
        << " color " << std::hex << std::setw(6) << std::setfill('0') << color
        << std::dec << " ...";
    std::string statusText = out.str();
    setStatusText(statusText);
    window->setColor(0x0);   // black; drawTree should change it
    floodFillInProgress = true;

    Timer timmy;
    timmy.start();

    int pixelsColored = floodFill(*window, x, y, color);

    floodFillInProgress = false;
    long elapsed = timmy.stop();

    statusText = integerToString(pixelsColored) + " pixels colored "
            + " (" + longToString(elapsed) + "ms)";
    setStatusText(statusText);

    colorChooser->setEnabled(true);
}

void FractalGUI::doSierpinski() {
    if (!hasAllCoords()) {
        setStatusText("Error: You must set an x, y, size, and order first.", /* isError */ true);
        return;
    }
    floodFillMode = false;
    int x = xField->getValueAsInt();
    int y = yField->getValueAsInt();
    int size = sizeField->getValueAsInt();
    int order = orderField->getValueAsInt();
    std::string statusText = "Sierpinski triangle: x=" + integerToString(x)
            + " y=" + integerToString(y)
            + " size=" + integerToString(size)
            + " order=" + integerToString(order);
    setStatusText(statusText + " ...");
    window->setColor(0x0);   // black

    Timer timmy;
    timmy.start();

    drawSierpinskiTriangle(*window, x, y, size, order);

    long elapsed = timmy.stop();
    statusText += " (" + longToString(elapsed) + "ms)";
    setStatusText(statusText);
}

void FractalGUI::doTree() {
    if (!hasAllCoords()) {
        setStatusText("Error: You must set an x, y, size, and order first.", /* isError */ true);
        return;
    }
    floodFillMode = false;
    int x = xField->getValueAsInt();
    int y = yField->getValueAsInt();
    int size = sizeField->getValueAsInt();
    int order = orderField->getValueAsInt();

    std::string statusText = "Recursive tree: x=" + integerToString(x)
            + " y=" + integerToString(y)
            + " size=" + integerToString(size)
            + " order=" + integerToString(order);
    setStatusText(statusText + " ...");
    window->setColor(0x0);   // black; drawTree should change it

    Timer timmy;
    timmy.start();

    drawTree(*window, x, y, size, order);

    long elapsed = timmy.stop();
    statusText += " (" + longToString(elapsed) + "ms)";
    setStatusText(statusText);
}

void FractalGUI::eventLoop() {
    while (true) {
        GEvent event = waitForEvent(ACTION_EVENT | MOUSE_EVENT | WINDOW_EVENT);
        bool animated = animationBox->isChecked();
        window->setRepaintImmediately(animated);
        if (event.getEventClass() == ACTION_EVENT) {
            GActionEvent actionEvent(event);
            handleActionEvent(actionEvent);
        } else if (event.getEventClass() == MOUSE_EVENT) {
            GMouseEvent mouseEvent(event);
            handleMouseEvent(mouseEvent);
        } else if (event.getEventClass() == WINDOW_EVENT) {
            if (event.getEventType() == WINDOW_CLOSED) {
                break;
            }
        }

        if (!animated) {
            window->repaint();
        }
    }
}

void FractalGUI::floodFill_addShapes() {
    // use buffered image to store individual pixels
    double canvasWidth = window->getCanvasWidth();
    double canvasHeight = window->getCanvasHeight();

    // draw several random shapes
    if (FLOOD_FILL_RANDOM_SEED > 0) {
        setRandomSeed(FLOOD_FILL_RANDOM_SEED);
    }

    Vector<std::string> colorVector = getColorMap().keys();
    for (int i = 0; i < FLOOD_FILL_NUM_SHAPES; i++) {
        double w = randomInteger(FLOOD_FILL_MIN_RECT_SIZE, FLOOD_FILL_MAX_RECT_SIZE);
        double h = randomInteger(FLOOD_FILL_MIN_RECT_SIZE, FLOOD_FILL_MAX_RECT_SIZE);
        double x = randomInteger(-w, canvasWidth);
        double y = randomInteger(-h, canvasHeight);
        int color = getColorMap()[randomElement(colorVector)];
        window->setColor(color);
        window->fillRect(x, y, w, h);
    }
    if (!window->isRepaintImmediately()) {
        window->repaint();
    }
}

const Map<std::string, int>& FractalGUI::getColorMap() {
    static Map<std::string, int> colorMap;
    if (colorMap.isEmpty()) {
        colorMap["Red"]    = 0x8c1515;   // Stanford red
        colorMap["Yellow"] = 0xeeee00;   // yellow
        colorMap["Blue"]   = 0x0000cc;   // blue
        colorMap["Green"]  = 0x00cc00;   // green
        colorMap["Purple"] = 0xcc00cc;   // purple
        colorMap["Orange"] = 0xff8800;   // orange
    }
    return colorMap;
}


void FractalGUI::handleActionEvent(GActionEvent& actionEvent) {
    if (actionEvent.getSource() == sierpinskiButton) {

        doSierpinski();
    } else if (actionEvent.getSource() == treeButton) {
        doTree();
    } else if (actionEvent.getSource() == floodFillButton) {
        if (!actionEvent.isShiftKeyDown()) {
            setStatusText("Flood fill: adding rectangles");
            floodFill_addShapes();
        }
        floodFillMode = true;
    } else if (actionEvent.getSource() == diffButton) {
        if (actionEvent.isShiftKeyDown()) {
            // save current window to an image (probably for teacher only)
            std::string filename = GFileChooser::showSaveDialog();
            if (!filename.empty()) {
                window->saveCanvasPixels(filename);
            }
        } else {
            // pop up DiffImage tool
            std::string filename = openFileDialog("Choose image to compare to", "output");
            if (!filename.empty()) {
                window->compareToImage(filename);
            }
        }
    } else if (actionEvent.getSource() == clearButton) {
        clear();
        floodFillMode = false;
        setStatusText("Graphics cleared.");
    }
}

/*
 * This is probably not decomposed as much as it should be; each kind of mouse
 * event should be in its own function.  Oh well.
 */
void FractalGUI::handleMouseEvent(GMouseEvent& mouseEvent) {
    int mx = (int) mouseEvent.getX();
    int my = (int) mouseEvent.getY();
    if (mouseEvent.getEventType() == MOUSE_MOVED) {
        if (true) {
            int px = window->getPixel(mx, my);
            std::string statusText = "(x="
                    + integerToString((int) mouseEvent.getX()) + ", y="
                    + integerToString((int) mouseEvent.getY()) + "), color="
                    + convertRGBToColor(px);
            setStatusText(statusText);
        }
    } else if (mouseEvent.getEventType() == MOUSE_PRESSED) {
        if (!mouseEvent.isLeftClick()) {
            if (dragRect) {
                window->remove(dragRect);
                delete dragRect;
                dragRect = nullptr;
                window->repaint();
                return;
            }
        }

        xField->setValue(mx);
        yField->setValue(my);

        if (floodFillMode) {
            doFloodFill(mx, my);
        }
    } else if (mouseEvent.getEventType() == MOUSE_DRAGGED) {
        if (!floodFillMode) {
            int x = xField->getValueAsInt();
            int y = yField->getValueAsInt();
            int mx = (int) mouseEvent.getX();
            int my = (int) mouseEvent.getY();
            if (mx > x && my > y) {
                int size = std::min(mx - x, my - y);
                sizeField->setValue(size);
                if (dragRect) {
                    dragRect->setBounds(x, y, size, size);
                } else {
                    dragRect = new GRect(x, y, size, size);
                    dragRect->setColor(DRAG_RECT_COLOR);
                    // dragRect->setLineWidth(2);
                    window->add(dragRect);
                }
            }
        }
    } else if (mouseEvent.getEventType() == MOUSE_RELEASED) {
        if (!floodFillMode && dragRect) {
            window->remove(dragRect);
            delete dragRect;
            dragRect = nullptr;
        }
    }
}

bool FractalGUI::hasAllCoords() {
    return stringIsInteger(xField->getText())
            && stringIsInteger(yField->getText())
            && stringIsInteger(sizeField->getText())
            && stringIsInteger(orderField->getText());
}


/*
 * Helper to create one button in the GUI.
 */
static GButton* createButton(const std::string& cmd, const std::string& icon) {
    GButton* button = new GButton();
    button->setText(cmd);
    button->setTextPosition(SwingConstants::SWING_CENTER, SwingConstants::SWING_BOTTOM);
    button->setActionCommand(cmd);
    button->setTooltip(cmd);
    button->setIcon(icon);
    return button;
}

void FractalGUI::initialize() {
    // GObject::setAntiAliasing(false);
    floodFillMode = false;
    floodFillInProgress = false;
    dragRect = nullptr;

    // create window
    window = new GWindow(WINDOW_WIDTH, WINDOW_HEIGHT, /* visible */ false);
    window->setResizable(true);
    window->setTitle("CS 106X Fractals");

    // create controls
    sierpinskiButton = createButton("Sierpinski Triangle", "triangle-icon.png");
    treeButton = createButton("Recursive Tree", "tree-icon.png");
    floodFillButton = createButton("Flood Fill", "paintbrush-icon.png");
    diffButton = createButton("Compare Output", "diff-icon.png");
    clearButton = createButton("Clear", "recycle-icon.png");
    colorChooser = new GChooser();
    colorChooser->setTooltip("Fill Color");
    colorChooser->addItems(getColorMap().keys());
    statusBar = new GLabel(" ");
    animationBox = new GCheckBox("Animate?");
    animationBox->setChecked(true);
    xField = new GTextField(4);
    xField->setPlaceholder("x");
    yField = new GTextField(4);
    yField->setPlaceholder("y");
    sizeField = new GTextField(4);
    sizeField->setPlaceholder("size");
    orderField = new GTextField(4);
    orderField->setValue(1);
    orderField->setPlaceholder("order");

    // perform layout
    window->addToRegion(new GLabel("x"), "North");
    window->addToRegion(xField, "North");
    window->addToRegion(new GLabel("y"), "North");
    window->addToRegion(yField, "North");
    window->addToRegion(new GLabel("size"), "North");
    window->addToRegion(sizeField, "North");
    window->addToRegion(new GLabel("order"), "North");
    window->addToRegion(orderField, "North");
    window->addToRegion(animationBox, "North");

    window->addToRegion(sierpinskiButton, "West");
    window->addToRegion(treeButton, "West");
    window->addToRegion(floodFillButton, "West");
    window->addToRegion(colorChooser, "West");
    window->addToRegion(diffButton, "West");
    window->addToRegion(clearButton, "West");

    window->addToRegion(statusBar, "South");

    window->pack();
    window->setWidth(WINDOW_WIDTH);
    window->center();
    window->setVisible(true);
}

void FractalGUI::setStatusText(const std::string& text, bool isError) {
    statusBar->setColor(isError ? 0xbb0000 : 0x0);
    statusBar->setText(text);
}

/*
 * Main function to run the program.
 */
int main() {
    FractalGUI gui;
    gui.initialize();
    gui.eventLoop();
    return 0;
}

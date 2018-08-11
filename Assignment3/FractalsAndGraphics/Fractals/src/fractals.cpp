// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include "fractals.h"
#include <math.h>
using namespace std;

// const
static const double SIN60 = sin(60 * (M_PI / 180));
static const double COS60 = cos(60 * (M_PI / 180));
static const int brown  = 0x8b7765;
static const int green = 0x2e8b57;

// Major problem: moving the cursor or otherwise interacting with the CANVAS while an operation
// other than floodFill is taking place stops and operation adn renders the entire window unresponsive (including close).
// This does not appear to be an issue on my end, however it exists regardless.


/**
 * Helper function which draws a triangle
 * @param x - location of leftmost corner
 * @param y - location of leftmost corner
 * @param size - height of triangle
 */
void drawTriangle(GWindow &window, double x, double y, double size);

/**
 * Helper recursive function which takes a stem and then draws <order> tree layers from that stem
 * @param stem - point to draw the tree layer from
 * @param size - length of "branches"
 * @param order - order number of the current layer
 * @param theta - angle which @stem was drawn at
 */
void drawTreeLayer(GWindow &window, GPoint stem, double size, int order, double theta);

/**
 * Recursive function which traverses a color block
 * @param x - pixel x
 * @param y - pixel y
 * @param startColor - color which was initially chosen to start the floodFill
 * @param endColor - color which was chosen to fill with
 * @return number of pixels colored
 */
int fillColor(GWindow &window, int x, int y, int startColor, int endColor);

/////////////////////////////////////
/// Sierpinski Triangle
void drawTriangle(GWindow& window, double x, double y, double size)
{
    // could have used drawPolar here, in the end its the same though
    window.drawLine(x,y, x+size,y);
    window.drawLine(x,y, x+size*COS60, y+size*SIN60);
    window.drawLine(x+size*COS60, y+size*SIN60, x+size, y);

}


void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order)
{
    if(x < 0 || y < 0 || size < 0 || order < 0)
    {
        throw("Illegal argument in drawSierpinskiTriangle call. Arguments must not be negative");
    }

    if(order == 1)
    {
        drawTriangle(window,x,y,size);
    }
    else
    {
        drawSierpinskiTriangle(window, x,y,size/2,order-1);
        drawSierpinskiTriangle(window, x+size/2, y,size/2,order-1);
        drawSierpinskiTriangle(window, x+(size/2)*COS60, y+(size/2)*SIN60, size/2, order-1);
    }
}

/////////////////////////////////////
/// Tree
void drawTreeLayer(GWindow& window, GPoint stem, double size, int order, double theta)
{
    if(order == 0)
    {
        return;
    }
    else
    {
        // using a for loop here to call recursive calls to remove duplication of code
        for(int i = -3; i < 4; i++)
        {
            order < 2 ? window.setColor(green) : window.setColor(brown);
            GPoint newBranchStem = window.drawPolarLine(stem, size/2, theta + (i*15));
            drawTreeLayer(window, newBranchStem, size/2, order-1, theta + (i*15));
        }
    }
}

void drawTree(GWindow& window, double x, double y, double size, int order)
{
    if(x < 0 || y < 0 || size < 0 || order < 0 || !window.inCanvasBounds(x,y))
    {
        throw("Illegal argument in drawTree call. Arguments must not be negative, and must be within canvas bounds");
    }

    if(order == 0)
    {
        return;
    }
    window.setColor(brown);
    GPoint stem = window.drawPolarLine(x+size/2,y+size, size/2 , 90);
    drawTreeLayer(window,stem, size/2, order-1, 90);

}

/////////////////////////////////////
/// Floodfill
int fillColor(GWindow& window, int x, int y, int startColor, int endColor)
{
    if(window.getPixel(x,y) == endColor || window.getPixel(x,y) != startColor)
    {
        return 0;
    }
    else
    {
        int result = 0;
        window.setPixel(x,y,endColor);
        // traverse one pixel up,down,left,right from the current pixel if that new pixel is in bounds
        if(window.inCanvasBounds(x+1,y))
        {
            result += fillColor(window,x+1,y,startColor,endColor);
        }
        if(window.inCanvasBounds(x-1,y))
        {
            result += fillColor(window,x-1,y,startColor,endColor);
        }
        if(window.inCanvasBounds(x,y-1))
        {
            result += fillColor(window,x,y-1,startColor,endColor);
        }
        if(window.inCanvasBounds(x,y+1))
        {
            result += fillColor(window,x,y+1,startColor,endColor);
        }
        return result;
    }
}

int floodFill(GWindow& window, int x, int y, int color)
{
    if(color == window.getPixel(x,y))
    {
        return 0;
    }
    else
    {
        return fillColor(window,x,y,window.getPixel(x,y),color);
    }
}

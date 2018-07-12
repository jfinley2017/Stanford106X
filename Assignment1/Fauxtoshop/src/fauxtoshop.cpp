#include <iostream>
#include "console.h"
#include "gwindow.h"
#include "grid.h"
#include "simpio.h"
#include "strlib.h"
#include "gbufferedimage.h"
#include "gevents.h"
#include "math.h" //for sqrt and exp in the optional Gaussian kernel
#include "gmath.h" // for sinDegrees(), cosDegrees(), PI
#include <limits>
using namespace std;

static const int    WHITE = 0xFFFFFF;
static const int    BLACK = 0x000000;
static const int    GREEN = 0x00FF00;
static const int    RED = 0xFF0000;
int    iMin = std::numeric_limits<int>::min();
int    iMax = std::numeric_limits<int>::max();
static const double    DIFFERENCE_ALPHA = .40;
static const string  EXIT_CODE = "";

bool     doFauxtoshop(GWindow &gw, GBufferedImage &img);

bool     openImageFromFilename(GBufferedImage& img, string filename);
bool 	 saveImageToFilename(const GBufferedImage &img, string filename);
void     getMouseClickLocation(int &row, int &col);
bool     updateWindow(GWindow &gw, GBufferedImage &newImage);
Vector<double> gaussKernelForRadius(int radius);

void doScatter(const GBufferedImage &originalImg, GBufferedImage &editedImg);
void doEdgeDetection(const GBufferedImage &originalImg, GBufferedImage &editedImg);
void doGreenScreen(const GBufferedImage &originalImg, GBufferedImage &editedImg);
void doFindDifference(const GBufferedImage &originalImg, GBufferedImage &editedImg);
void doRotation(const GBufferedImage &originalImg, GBufferedImage &editedImg);
void doGaussianBlur(const GBufferedImage &originalImg, GBufferedImage &editedImg);

int getWeightedColor(int pixel, double weight);
bool getImage(GBufferedImage &img, string prompt);
int getIntegerRangeInclusive(string prompt, int min, int max);
int getIntegerRangeExclusive(string prompt, int min, int max);
int getImageDifference(const GBufferedImage &originalImg, GBufferedImage &comparisonImg);
bool isEdge(const Grid<int> &inGrid, int row, int col, int threshold);
int getPixelDifference(int pixelOne, int pixelTwo);


/* STARTER CODE FUNCTION - DO NOT EDIT
 *
 * This main simply declares a GWindow and a GBufferedImage for use
 * throughout the program. By asking you not to edit this function,
 * we are enforcing that the GWindow have a lifespan that spans the
 * entire duration of execution (trying to have more than one GWindow,
 * and/or GWindow(s) that go in and out of scope, can cause program
 * crashes).
 */
int main() {
    GWindow gw;
    gw.setTitle("Fauxtoshop");
    gw.setVisible(true);
    GBufferedImage img;

    bool shouldContinue = true;
    while(shouldContinue)
    {
        shouldContinue = doFauxtoshop(gw, img);
    }

    gw.clear();
    gw.close();

    return 0;
}

/*
 * Runs Fauxtoshop, returns true if should cycle (user did not quit)
 *
 *
 *
 */
bool doFauxtoshop(GWindow &gw, GBufferedImage &img) {

    cout << "Welcome to Fauxtoshop!" << endl;
    if(!getImage(img, "Enter the name of the image file to open (or " + (EXIT_CODE == "" ? "leave blank" : EXIT_CODE)  + " to quit): "))
    {
        return false;
    }

    updateWindow(gw, img);

    cout << "What image filter would you like to apply?" << endl
         << "           1 - Scatter" << endl
         << "           2 - Edge detection" << endl
         << "           3 - Green screen with another image" << endl
         << "           4 - Compare image with another image" << endl
         << "           5 - Rotation " << endl
         << "           6 - Gaussian Blur" << endl;

    int choice = getIntegerRangeInclusive("Menu choice: ", 1,6);

    GBufferedImage editedImage;
    switch(choice)
    {
        case 1:
        {
            doScatter(img, editedImage);
            updateWindow(gw, editedImage);
            break;
        }
        case 2:
        {
            doEdgeDetection(img, editedImage);
            updateWindow(gw, editedImage);
            break;
        }
        case 3:
        {
            doGreenScreen(img, editedImage);
            updateWindow(gw, editedImage);
            break;
        }
        case 4:
        {
            doFindDifference(img, editedImage);
            updateWindow(gw, editedImage);
            break;
        }
        case 5:
        {
            doRotation(img, editedImage);
            updateWindow(gw, editedImage);
            break;
        }
        case 6:
        {
            doGaussianBlur(img, editedImage);
            updateWindow(gw, editedImage);
            break;
        }

        default:
        {
            break;
        }
    }

    while(1)
    {
        string saveName;
        cout << "Enter filename to save, or " << (EXIT_CODE == "" ? "leave blank" : EXIT_CODE) << " to exit: ";
        getLine(std::cin, saveName);
        if(saveName == EXIT_CODE)
        {
            break;
        }

        if(!saveImageToFilename(editedImage, saveName))
        {
            cout << "Image: "<< saveName << " could not be saved" << endl;

        }
        else
        {
            break;
        }

    }

    gw.clear();
    return true;
}

void doScatter(const GBufferedImage &originalImg, GBufferedImage &editedImg)
{

    int scatterRadius = getIntegerRangeInclusive("Scatter Radius [1-100]", 1, 100);

    Grid<int> originalImgGrid = originalImg.toGrid();
    Grid<int> scatteredImgGrid(originalImgGrid.height(), originalImgGrid.width(), WHITE);

    for(int row = 0; row < originalImgGrid.height(); row++)
    {
        for(int col = 0; col < originalImgGrid.width(); col++)
        {
            int newRow = -1;
            int newCol = -1;
            while(!originalImgGrid.inBounds(newRow,newCol))
            {

                newRow = randomInteger(max(row - scatterRadius, 0), min(row + scatterRadius, originalImgGrid.height()) );
                newCol = randomInteger(max(col - scatterRadius, 0), min(col + scatterRadius, originalImgGrid.width()) );

            }

            scatteredImgGrid.set(row,col, originalImgGrid[newRow][newCol]);
        }

    }
    editedImg.fromGrid(scatteredImgGrid);
    return;

}

void doEdgeDetection(const GBufferedImage &originalImg, GBufferedImage &editedImg)
{

    int differenceThreshold = getIntegerRangeInclusive("Difference Threshold (>=0): ", 0, iMax);

    Grid<int> originalImgGrid = originalImg.toGrid();
    Grid<int> edgeImgGrid(originalImgGrid.height(), originalImgGrid.width(), WHITE);

    for(int row = 0; row < originalImgGrid.height(); row++)
    {
        for(int col = 0; col < originalImgGrid.width(); col++)
        {

            if(isEdge(originalImgGrid, row, col, differenceThreshold))
            {
                edgeImgGrid[row][col] = BLACK;
            }

        }

    }

    editedImg.fromGrid(edgeImgGrid);

}


void doGreenScreen(const GBufferedImage &original, GBufferedImage &editedImg)
{
    GBufferedImage stickerImage;
    if(!getImage(stickerImage, "Enter the name of the sticker image: "))
    {
        return;
    }

    int toleranceThreshold = getIntegerRangeInclusive("Enter the threshold value: ",1,100);

    int rowSelection =  -1;
    int colSelection = -1;
    string rowColInput = "";
    while(1)
    {
        cout << "Enter coordinates (int, int) for the sticker image, or use blank to choose with mouse: ";
        getLine(std::cin, rowColInput);
        if(rowColInput == "")
        {
            cout <<"Click on your background image to choose a location" << endl;
            getMouseClickLocation(rowSelection, colSelection);
            break;
        }

        // this far in, we know the user opted not to use the mouse and has specified a position
        int validFormat = sscanf(rowColInput.c_str(), "(%d,%d)", &rowSelection, &colSelection);
        if(validFormat != 2)
        {
            cout << "Invalid format specified, try again" << endl;
        }
        else
        {
            if((original.inBounds(rowSelection, colSelection)))
            {
                break;
            }
            // this far in, we know that the coordinates given were in the correct format, but ended up being out of bounds of the background image
            cout << "Input of: (" << rowSelection << "," << colSelection << ") is invalid, try again" << endl;
        }
    }

    cout << "You chose: (" << rowSelection << "," << colSelection <<")" << endl;

    Grid<int> editedGrid = original.toGrid();
    Grid<int> stickerGrid = stickerImage.toGrid();

    for(int row = 0; row < stickerGrid.height(); row++)
    {
        for(int col = 0; col < stickerGrid.width(); col++)
        {
            if(editedGrid.inBounds(row + rowSelection, col+ colSelection))
            {
                if(getPixelDifference(stickerGrid[row][col], GREEN) > toleranceThreshold)
                {
                    editedGrid[row+rowSelection][col+colSelection] = stickerGrid[row][col];
                }
            }
        }

    }

    editedImg.fromGrid(editedGrid);
}

void doFindDifference(const GBufferedImage &originalImg, GBufferedImage &editedImg)
{

    GBufferedImage comparisonImg;

    if(!getImage(comparisonImg, "Enter the name of the comparison image: "))
    {
        return;
    }
    int numDifferences = 0;

    int differenceTolerance = getIntegerRangeInclusive("Enter difference tolerance [0-100]: ", 0,100);

    Grid<int> originalImgGrid = originalImg.toGrid();
    Grid<int> comparisonImgGrid = comparisonImg.toGrid();
    Grid<int> editedImgGrid(max(originalImgGrid.height(), comparisonImgGrid.height()), max(originalImgGrid.width(), comparisonImgGrid.width()), WHITE);

    for(int row = 0; row < editedImgGrid.height(); row++)
    {
        for (int col = 0; col < editedImgGrid.width(); col++)
        {
            if(originalImgGrid.inBounds(row,col) && comparisonImgGrid.inBounds(row,col))
            {
                if(getPixelDifference(originalImgGrid[row][col], comparisonImgGrid[row][col]) >= differenceTolerance)
                {
                    numDifferences++;
                    editedImgGrid[row][col] = RED;
                }
                else
                {

                    editedImgGrid[row][col] = getWeightedColor(originalImgGrid[row][col],DIFFERENCE_ALPHA) + getWeightedColor(WHITE,1-DIFFERENCE_ALPHA);
                }
            }
            else
            {
                editedImgGrid[row][col] = RED;
            }
        }
    }

    cout << "The images differ in: " << numDifferences << " locations." << endl;
    editedImg.fromGrid(editedImgGrid);

}

void doRotation(const GBufferedImage &originalImg, GBufferedImage &editedImg)
{

    Grid<int> originalImgGrid = originalImg.toGrid();
    Grid<int> editedImgGrid(originalImgGrid.height(), originalImgGrid.width(), WHITE);
    int midRow = originalImgGrid.height() / 2;
    int midCol = originalImgGrid.width() / 2;
    editedImgGrid[midRow][midCol] = originalImgGrid[midRow][midCol];

    int degreeRotation = -361;
    while(1)
    {
        degreeRotation = getInteger("Enter degree to rotate (-360 - 360): ");
        if(degreeRotation >=-360 && degreeRotation <= 360)
        {
            break;
        }

    }

    double cosDegreesVal = cosDegrees(degreeRotation);
    double sinDegreesVal = sinDegrees(degreeRotation);
    for(int row = 0; row < editedImgGrid.height(); row++)
    {
        for (int col = 0; col < editedImgGrid.width(); col++)
        {
            int x = (row-midRow) * cosDegreesVal - (col-midCol) * sinDegreesVal + midRow;
            int y = (col-midCol) * cosDegreesVal + (row-midRow) * sinDegreesVal + midCol;
            if(originalImgGrid.inBounds(x,y))
            {
                editedImgGrid[row][col] = originalImgGrid[x][y];
            }
        }
    }
    editedImg.fromGrid(editedImgGrid);

}


/*
 * for each image row in input image:
 *   for each pixel in image row:
 *
 *     set accumulator to zero
 *
 *     for each kernel row in kernel:
 *        for each element in kernel row:
 *
 *           if element position  corresponding* to pixel position then
 *              multiply element value  corresponding* to pixel value
 *              add result to accumulator
 *           endif
 *
 *     set output image pixel to accumulator
 */
void doGaussianBlur(const GBufferedImage &originalImg, GBufferedImage &editedImg)
{
    Grid<int> originalImgGrid = originalImg.toGrid();
    Grid<int> editedImgGrid(originalImgGrid.height(), originalImgGrid.width(), WHITE);
    Grid<int> temp(originalImgGrid.height(), originalImgGrid.width(), WHITE);

    int radius = getIntegerRangeExclusive("Enter radius for Gaussian Blur (0 - inf): ", 0, iMax);
    Vector<double> gaussianKernal = gaussKernelForRadius(radius);
    int accumulator;

    // rows
    for(int row = 0; row < originalImgGrid.height(); row++)
    {
        for(int col = 0; col < originalImgGrid.width(); col++)
        {
            accumulator = 0;
            for(int kernalIterator = -radius; kernalIterator<= radius; kernalIterator++)
            {
                if(originalImgGrid.inBounds(row, col+kernalIterator))
                {
                    accumulator += getWeightedColor(originalImgGrid[row][col + kernalIterator],gaussianKernal[kernalIterator + radius]);
                }
                else
                {
                     accumulator += getWeightedColor(originalImgGrid[row][col - kernalIterator], gaussianKernal[kernalIterator + radius]);
                }
             }
            temp[row][col] = accumulator;
        }
    }

    // cols
    for(int col = 0; col < originalImgGrid.width(); col++)
    {
        for(int row = 0; row < originalImgGrid.height(); row++)
        {
            accumulator = 0;
            for(int kernalIterator = -radius; kernalIterator<= radius; kernalIterator++)
            {
                if(originalImgGrid.inBounds(row + kernalIterator, col))
                {
                    accumulator += getWeightedColor(temp[row + kernalIterator][col], gaussianKernal[kernalIterator + radius]);
                }
                else
                {
                     accumulator += getWeightedColor(temp[row - kernalIterator][col], gaussianKernal[kernalIterator + radius]);
                }

             }
             editedImgGrid[row][col] = accumulator;
        }
    }

    editedImg.fromGrid(editedImgGrid);

}

int getWeightedColor(int pixel, double weight)
{

    int pixelOneRed, pixelOneGreen, pixelOneBlue;
    GBufferedImage::getRedGreenBlue(pixel, pixelOneRed, pixelOneGreen, pixelOneBlue);
    return GBufferedImage::createRgbPixel(pixelOneRed * weight, pixelOneGreen * weight, pixelOneBlue * weight);

}


// determines the difference between two images
int getImageDifference(const GBufferedImage &originalImg, GBufferedImage &comparisonImg)
{

    return originalImg.countDiffPixels(comparisonImg);

}

int getIntegerRangeInclusive(string prompt, int min, int max)
{
    int inputValue;
    while(1)
    {
        inputValue = getInteger(prompt);
        if(inputValue >=min && inputValue <=max)
        {
            return inputValue;
        }
         cout << "Input not in range: [" << (min == iMin ? "-inf" : to_string(min)) << "," << (max == iMax ? "inf" : to_string(max)) << "]" << endl;
    }
}

int getIntegerRangeExclusive(string prompt, int min, int max)
{
    int inputValue;
    while(1)
    {
        inputValue = getInteger(prompt);
        if(inputValue > min && inputValue < max)
        {
            return inputValue;
        }
         cout << "Input not in range: (" << (min == iMin ? "-inf" : to_string(min)) << "," << (max == iMax ? "inf" : to_string(max)) << ")" << endl;

    }
}


// determines if a pixel is an edge pixel by utilizing the difference function
bool isEdge(const Grid<int> &inGrid, int row, int col, int threshold)
{

    for(int i=-1; i<=1; i++)
    {
        for(int j=-1; j<=1; j++)
        {

            if(inGrid.inBounds(row+i,col+j))
            {
                if(getPixelDifference(inGrid.get(row,col), inGrid.get(row+i,col+j)) > threshold)
                {
                    cout << "Found Edge" << row << col << endl;
                    return true;
                }
            }
        }
    }

    return false;
}

// finds the difference between two pixels
int getPixelDifference(int pixelOne, int pixelTwo)
{
    int pixelOneRed, pixelOneGreen, pixelOneBlue,
        pixelTwoRed, pixelTwoGreen, pixelTwoBlue;

    GBufferedImage::getRedGreenBlue(pixelOne, pixelOneRed, pixelOneGreen, pixelOneBlue);
    GBufferedImage::getRedGreenBlue(pixelTwo, pixelTwoRed, pixelTwoGreen, pixelTwoBlue);

    int difference = max( max(abs(pixelOneBlue - pixelTwoBlue), abs(pixelOneRed - pixelTwoRed)) , abs(pixelOneGreen - pixelTwoGreen));
    return difference;
}

// clears the current window and then, if the image provided is valid, sets the window to the new image
bool updateWindow(GWindow &gw, GBufferedImage &newImage)
{
    if(newImage.getHeight() > 0 && newImage.getWidth() >  0)
    {
        gw.clear();
        gw.setCanvasSize(newImage.getWidth(), newImage.getHeight());
        gw.add(&newImage,0,0);
        return true;
    }

    return false;

}

// Used to open an image via a string that the user provides
bool getImage(GBufferedImage &img, string prompt)
{
    while(1)
    {
        string imgToOpenName;
        cout << prompt;
        getLine(std::cin, imgToOpenName);
        if(imgToOpenName == EXIT_CODE)
        {
            return false;

        }

        if(!openImageFromFilename(img, imgToOpenName))
        {
            cout << "Image: "<< imgToOpenName << " could not be opened" << endl;

        }
        else
        {
            break;
        }

    }
    return true;
}





/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Attempts to open the image file 'filename'.
 *
 * This function returns true when the image file was successfully
 * opened and the 'img' object now contains that image, otherwise it
 * returns false.
 */
bool openImageFromFilename(GBufferedImage& img, string filename) {
    try { img.load(filename); }
    catch (...) { return false; }
    return true;
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Attempts to save the image file to 'filename'.
 *
 * This function returns true when the image was successfully saved
 * to the file specified, otherwise it returns false.
 */
bool saveImageToFilename(const GBufferedImage &img, string filename) {
    try { img.save(filename); }
    catch (...) { return false; }
    return true;
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Waits for a mouse click in the GWindow and reports click location.
 *
 * When this function returns, row and col are set to the row and
 * column where a mouse click was detected.
 */
void getMouseClickLocation(int &row, int &col) {
    GMouseEvent me;
    do {
        me = getNextEvent(MOUSE_EVENT);
    } while (me.getEventType() != MOUSE_CLICKED);
    row = me.getY();
    col = me.getX();
}

/* HELPER FUNCTION
 *
 * This is a helper function for the Gaussian blur option.
 *
 * The function takes a radius and computes a 1-dimensional Gaussian blur kernel
 * with that radius. The 1-dimensional kernel can be applied to a
 * 2-dimensional image in two separate passes: first pass goes over
 * each row and does the horizontal convolutions, second pass goes
 * over each column and does the vertical convolutions. This is more
 * efficient than creating a 2-dimensional kernel and applying it in
 * one convolution pass.
 *
 * This code is based on the C# code posted by Stack Overflow user
 * "Cecil has a name" at this link:
 * http://stackoverflow.com/questions/1696113/how-do-i-gaussian-blur-an-image-without-using-any-in-built-gaussian-functions
 *
 */
Vector<double> gaussKernelForRadius(int radius) {
    if (radius < 1) {
        Vector<double> empty;
        return empty;
    }
    Vector<double> kernel(radius * 2 + 1);
    double magic1 = 1.0 / (2.0 * radius * radius);
    double magic2 = 1.0 / (sqrt(2.0 * PI) * radius);
    int r = -radius;
    double div = 0.0;
    for (int i = 0; i < kernel.size(); i++) {
        double x = r * r;
        kernel[i] = magic2 * exp(-x * magic1);
        r++;
        div += kernel[i];
    }
    for (int i = 0; i < kernel.size(); i++) {
        kernel[i] /= div;
    }
    return kernel;
}

// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include <iostream>
#include <limits>
#include <fstream>
#include <deque>
#include <map>
#include "console.h"
#include "string.h"
#include "strlib.h"
#include "simpio.h"
#include "random.h"

using namespace std;

int    iMin = std::numeric_limits<int>::min();
int    iMax = std::numeric_limits<int>::max();


void doNGrams(int NGrams, ifstream& fileStream);
int populateMap(int NGrams, map<vector<string>, vector<string> > &nGramMap ,ifstream &fileStream);

void addToMap(map<vector<string>, vector<string> > &nGramMap, vector<string> &Key, string& Val);
vector<string> dequeToString(deque<string> &inputDeque);
int getIntegerRangeInclusive(string prompt, string errorPrompt, int min, int max);
int getIntegerRangeExclusive(string prompt, string errorPrompt, int min, int max);
void printMap(map<vector<string>, vector<string> > &mapToPrint, int numToPrint);
vector<vector<string> > getKeys(map<vector<string>, vector<string> > &map);


int main() {

    ifstream input;
    string fileName;
    int NGrams;

    while(1)
    {
        cout << "Input file name: ";
        getLine(std::cin, fileName);
        input.open(fileName);
        if(input.good())
        {
            break;
        }
        else
        {
            cout << "File: " << fileName << " is not valid" << endl;
        }
    }

    NGrams = getIntegerRangeInclusive("Value of N: ", "N must be 2 or greater", 2, iMax);
    doNGrams(NGrams,input);

    cout << "Exiting." << endl;
    return 0;
}



void doNGrams(int NGrams, ifstream &fileStream)
{
    map<vector<string>, vector<string>> nGramMap;

    int numWords = populateMap(NGrams, nGramMap, fileStream);

    if(!(numWords > 0))
    {
        cout << "Error populating NGram map" << endl;
    }

    while(1)
    {

        int numToPrint = getIntegerRangeInclusive("Number of words to print (0 to quit): ", "Must be at least " + integerToString(NGrams) + "Words.", NGrams, numWords);

        if(numToPrint == 0)
        {
            break;
        }

        printMap(nGramMap, numToPrint);

    }
}


int populateMap(int NGrams, map<vector<string>, vector<string> > &nGramMap, ifstream &fileStream)
{

    streampos start = fileStream.tellg();
    string word;
    deque<string> window;
    int numWords = 0;

    // iterate through entire file
    while(fileStream >> word )
    {
        if(word.substr(0,4) == "http")
        {
            continue;
        }


        if(window.size() != NGrams-1)
        {
            window.push_back(word);
        }

        else
        {
            vector<string> Key = dequeToString(window);
            addToMap(nGramMap, Key , word);
            window.pop_front();
            window.push_back(word);

        }
        numWords++;
    }

    // wrap around
    fileStream.clear();
    fileStream.seekg(start);

    for(int i = 0; i < NGrams - 1; i++)
    {
        fileStream >> word;

        vector<string> Key = dequeToString(window);
        addToMap(nGramMap, Key, word);
        window.pop_front();
        window.push_back(word);

    }

    return numWords;
}

void printMap(map<vector<string>, vector<string> > &mapToPrint, int numToPrint)
{
    string output;
    vector<vector<string> > Keys = getKeys(mapToPrint);
    int numPrinted = 0;
    int rand = randomInteger(0,Keys.size()-1);
    deque<string> window;


    // initialize window, printing the elements added
    cout << "... ";
    for(int i = 0; i < Keys[rand].size(); i++)
    {
        window.push_back(Keys[rand][i]);
        output += Keys[rand][i] + " ";
    }

    numPrinted += window.size();

    // go through the map
    while(numPrinted != numToPrint)
    {
         vector<string> key = dequeToString(window);
         window.pop_front();
         if(mapToPrint[key].size() > 1)
         {
             int randChoice = randomInteger(0, mapToPrint[key].size()-1);
             output += mapToPrint[key][randChoice] + " ";
             window.push_back(mapToPrint[key][randChoice]);
         }
         else
         {
             output += mapToPrint[key][0] + " ";
             window.push_back(mapToPrint[key][0]);
         }
         numPrinted++;
    }
    cout << output + "..." << endl;

}

vector<vector<string> > getKeys(map<vector<string>, vector<string> > &map)
{
    vector<vector<string> > keys;
    for(auto& p : map)
    {
        keys.push_back(p.first);
    }
    return keys;
}

vector<string> dequeToString(deque<string> &inputDeque)
{

    vector<string> newVector;
    for(int i = 0; i< inputDeque.size(); i++)
    {
        newVector.push_back(inputDeque[i]);
    }
    return newVector;
}

void addToMap(map<vector<string>, vector<string> > &nGramMap, vector<string> &Key, string& Val)
{

    if(nGramMap.find(Key) != nGramMap.end())
    {
        nGramMap[Key].push_back(Val);
    }
    else
    {

        vector<string> newVector{Val};
        nGramMap.insert(pair<vector<string>,vector<string>>(Key, newVector));

    }

}

int getIntegerRangeInclusive(string prompt, string errorPrompt, int min, int max)
{
    int inputValue;
    while(1)
    {
        inputValue = getInteger(prompt);
        if(inputValue >=min && inputValue <=max)
        {
            return inputValue;
        }
        cout << errorPrompt << endl << "Range:" << "[" << (min == iMin ? "-inf" : to_string(min)) << "," << (max == iMax ? "inf" : to_string(max)) << "]" << endl;
    }
}

int getIntegerRangeExclusive(string prompt, string errorPrompt, int min, int max)
{
    int inputValue;
    while(1)
    {
        inputValue = getInteger(prompt);
        if(inputValue > min && inputValue < max)
        {
            return inputValue;
        }
        cout << errorPrompt << endl << "Range:" << "(" << (min == iMin ? "-inf" : to_string(min)) << "," << (max == iMax ? "inf" : to_string(max)) << ")" << endl;

    }
}

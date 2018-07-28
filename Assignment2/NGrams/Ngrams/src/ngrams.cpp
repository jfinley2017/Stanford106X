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

// Main worker functions
void doNGrams(int NGrams, ifstream& fileStream);

// Reads through a file and populates a nGram map based on the NGrams provided.
void populateMap(int NGrams, map<vector<string>, vector<string> > &nGramMap ,ifstream &fileStream);

// Utility functions used to generate output based on an existing map. generateOutputSentences attempts to generate full sentences, can fail.
void generateOutput(map<vector<string>, vector<string> > &mapToPrint, int numToPrint);
void generateOutputSentences(map<vector<string>, vector<string> > &mapToPrint, int numToPrint);

// Utility functions used to grab keys. getStartKeys filters for keys which begin with a capital letter
vector<vector<string> > getStartKeys(map<vector<string>, vector<string> > &map);
vector<vector<string> > getKeys(map<vector<string>, vector<string> > &map);

// Resets a filestream and set the position to the start, then loops over returning the number of words found.
// Resets the filestream back to 0 after execution.
int getNumWordsFile(ifstream &fileStream);

// Adds to the specified map. If the key exists, value will be pushed to the end, else it will generate a new index based on the value Key
void addToMap(map<vector<string>, vector<string> > &nGramMap, vector<string> &Key, string& Val);

// Iterates over a deque, adding the elements found to a vector, returns the vector.
vector<string> dequeToString(deque<string> &inputDeque);

// Utility functions to get user input from a range, prints the error prompt with the range specified if the input is out of range.
int getIntegerRangeInclusive(string prompt, string errorPrompt, int min, int max);
int getIntegerRangeExclusive(string prompt, string errorPrompt, int min, int max);


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

    int numWords = getNumWordsFile(input);
    NGrams = getIntegerRangeInclusive("Value of N: ", "N must be at least 2 and less than the size of the file provided (" + integerToString(numWords) + ")", 2, numWords);

    doNGrams(NGrams,input);

    cout << "Exiting." << endl;
    return 0;
}



void doNGrams(int NGrams, ifstream &fileStream)
{
    map<vector<string>, vector<string>> nGramMap;
    populateMap(NGrams, nGramMap, fileStream);

    while(1)
    {
        int numToPrint = getIntegerRangeInclusive("Number of words to print (0 to quit): ", "Must be at least " + integerToString(NGrams) + " Words.", NGrams, iMax);

        if(numToPrint == 0)
        {
            break;
        }

         getYesOrNo("Attempt to print full sentences? (y,n)", "Please answer yes or no", "no")
                 ? generateOutputSentences(nGramMap, numToPrint) : generateOutput(nGramMap, numToPrint);
    }
}


void populateMap(int NGrams, map<vector<string>, vector<string> > &nGramMap, ifstream &fileStream)
{

    string word;
    deque<string> window;

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
    }

    // wrap around
    fileStream.clear();
    fileStream.seekg(ios_base::beg);

    for(int i = 0; i < NGrams - 1; i++)
    {
        fileStream >> word;
        vector<string> Key = dequeToString(window);
        addToMap(nGramMap, Key, word);
        window.pop_front();
        window.push_back(word);

    }
}

void generateOutput(map<vector<string>, vector<string> > &mapToPrint, int numToPrint)
{

    vector<vector<string> > Keys = getKeys(mapToPrint);
    deque<string> window;
    string output;

    // initialize window, printing the elements added
    int startIndex = randomInteger(0,Keys.size()-1);
    cout << "... ";
    for(int i = 0; i < Keys[startIndex].size(); i++)
    {
        window.push_back(Keys[startIndex][i]);
        output += Keys[startIndex][i] + " ";
    }

    // go through the map
    int numPrinted = window.size();
    while(numPrinted < numToPrint)
    {
         vector<string> key = dequeToString(window);
         if(mapToPrint[key].size() > 1)
         {
             int randChoice = randomInteger(0, mapToPrint[key].size()-1);
             output += mapToPrint[key][randChoice] + " ";
             window.pop_front();
             window.push_back(mapToPrint[key][randChoice]);
         }
         else
         {
             output += mapToPrint[key][0] + " ";
             window.pop_front();
             window.push_back(mapToPrint[key][0]);
         }
         numPrinted++;
    }
    cout << output + "..." << endl;

}


void generateOutputSentences(map<vector<string>, vector<string> > &mapToPrint, int numToPrint)
{
    vector<vector<string> > startKeys = getStartKeys(mapToPrint);

    if(startKeys.empty())
    {
        cout << "This input contains no starts of sentences." << endl;
        return;
    }

    deque<string> window;
    string output;

    int startIndex = randomInteger(0,startKeys.size()-1);
    for(int i = 0; i < startKeys[startIndex].size(); i++)
    {
        window.push_back(startKeys[startIndex][i]);
        output += startKeys[startIndex][i] + " ";
    }

    // go through the map
    int numPrinted = window.size();
    while(numPrinted < numToPrint)
    {
        while(1)
        {
            vector<string> key = dequeToString(window);
            if(mapToPrint[key].size() > 1)
            {
                int randChoice = randomInteger(0, mapToPrint[key].size()-1);
                output += mapToPrint[key][randChoice] + " ";
                numPrinted++;
                window.pop_front();
                window.push_back(mapToPrint[key][randChoice]);
                if(mapToPrint[key][randChoice].back() == '.' || mapToPrint[key][randChoice].back() == '?' || mapToPrint[key][randChoice].back() == '!')
                {
                    break;
                }
            }
            else
            {
                output += mapToPrint[key][0] + " ";
                numPrinted++;
                window.pop_front();
                window.push_back(mapToPrint[key][0]);
                if(mapToPrint[key][0].back() == '.' || mapToPrint[key][0].back() == '?' || mapToPrint[key][0].back() == '!')
                {
                    break;
                }
            }
        }
    }
    cout << output << endl;
}

vector<vector<string> > getStartKeys(map<vector<string>, vector<string> > &map)
{
    vector<vector<string> > keys;
    for(auto& p : map)
    {
        if( isupper(p.first[0].at(0)) )
        {
            keys.push_back(p.first);
        }

    }
    return keys;
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

int getNumWordsFile(ifstream &fileStream)
{
    fileStream.clear();
    fileStream.seekg(ios_base::beg);

    string word;
    int numWords = 0;

    while(fileStream >> word)
    {
        numWords++;
    }

    fileStream.clear();
    fileStream.seekg(ios_base::beg);
    return numWords;
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

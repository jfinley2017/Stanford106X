// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <set>
#include <unordered_set>
#include "console.h"
#include "simpio.h"
using namespace std;


static string QUITSTRING = "";

bool doWordLadder(unordered_set<string> dictionary, bool bShouldBeWord, bool bShouldAddLetters, bool bShouldRemoveLetters);
//gets a word which contains only alphanumerical characters, return false if the user opts to quit
bool getWord(string &inputString, string prompt, const unordered_set<string> &dict, bool bMustBeWord);
// checks if a string contains only alphanumerical characters, returns false if none are found
bool containsNonAlpha(const string &s);
// checks if the string exists in the current dictionary
bool isWord(const string word, const unordered_set<string> &dict);
// loads the dictionary into a set
void loadDictionary(ifstream &inDictionary, unordered_set<string> &outDictionary);
// constructs a ladder from word1 to word2 using dictionary
stack<string> constructLadder(const string &word1, const string &word2, const unordered_set<string> &dictionary, bool bShouldAddLetters, bool bShouldRemoveLetters);
// finds a word 1 step from the provided word
vector<string> findNeighbor(const string &word, const unordered_set<string> &dictionary, unordered_set<string> &filter, bool bShouldAddLetters, bool bShouldRemoveLetters);
string toLower(const string &s);


int main() {
    cout << "Welcome to CS 106X Word Ladder!" << endl;
    cout << "Give me two English words, and I will change the first into the second by changing one letter at a time." << endl << endl;

    ifstream dictionary;
    unordered_set<string> cachedDictionary;

   //this could (and should) be moved to a seperate function but im lazy and uninterested in overengineering anymore here
   loadDictionary(dictionary, cachedDictionary);
   bool bMustBeWord = getYesOrNo("Should input be words?", "Should be yes or no");
   bool bShouldAddLetters = getYesOrNo("Should BFS add letters while attempting to form ladder?: ", "Should be yes or no");
   bool bShouldRemoveLetters = getYesOrNo("Should BFS remove letters while attempting to form ladder?:", "Should be yes or no");
   cout << endl;

   // continue to run word ladder until quit
   while(1)
   {
      if(!doWordLadder(cachedDictionary, bMustBeWord, bShouldAddLetters, bShouldRemoveLetters))
        {
            break;
        }
    }

    cout << "Have a nice day." << endl;
    return 0;
}


bool doWordLadder(unordered_set<string> dictionary, bool bMustBeWord, bool bShouldAddLetters, bool bShouldRemoveLetters)
{
    string startWord;
    string endWord;
    vector<string> addedWords;

    if(!getWord(startWord, "Enter first word (enter to quit): ", dictionary, bMustBeWord))
    {
        return false;
    }

    while(1)
    {
        if(!getWord(endWord, "Enter second word (enter to quit): ", dictionary, bMustBeWord))
        {
            return false;
        }
        if(endWord.size() < startWord.size() && !bShouldRemoveLetters)
        {
            cout << "Second word size is less than first and bShouldRemoveLetters is disabled, try again" << endl;
            continue;
        }
        else if(endWord.size() > startWord.size() && !bShouldAddLetters)
        {
             cout << "Second word size is greater than first and bShouldAddLetters is disabled, try again" << endl;
             continue;
        }
        else
        {
             break;
        }
    }

    // if the user elected to allow for words outside of the dictionary
    // check to see if the words are 'real' words, add them to the dictionary if not
    // words in added words will be removed at the end of each run
    startWord = toLower(startWord);
    endWord = toLower(endWord);
    if(!bMustBeWord)
    {
        if(!isWord(startWord, dictionary))
        {
            addedWords.push_back(startWord);
            dictionary.insert(startWord);
        }

        if(!isWord(endWord, dictionary))
        {
            addedWords.push_back(endWord);
            dictionary.insert(endWord);
        }
    }

    // print result of the constructedLadder
    stack<string> finalStack;
    finalStack = constructLadder(startWord, endWord, dictionary, bShouldAddLetters, bShouldRemoveLetters);
    if(finalStack.empty())
    {
        cout << "No ladder could be constructed from " << startWord << " to " << endWord << endl << endl;
    }
    else
    {
        cout << "A ladder from " << endWord << " back to " << startWord << endl;
        while(!finalStack.empty())
        {
            cout << finalStack.top() + " ";
            finalStack.pop();
        }
        cout << endl << endl;
    }

    // remove previously added words
    for(int i = 0; i < addedWords.size(); i++)
    {
        dictionary.erase(addedWords[i]);
    }

    return true;
}

stack<string> constructLadder(const string &word1, const string &word2, const unordered_set<string> &dictionary, bool bShouldAddLetters, bool bShouldRemoveLetters)
{
    stack<string> temp;
    temp.push(word1);
    queue<stack<string> > ladderQueue;
    ladderQueue.push(temp);
    unordered_set<string> usedWords;

    if(word1 != word2)
    {
         usedWords.insert(word1);
    }

    // while there are still stacks in the queue (we haven't hit a dead end)
    while(!ladderQueue.empty())
    {
        temp = ladderQueue.front();
        ladderQueue.pop();
        // find the neighbors of the element ontop of the first stack in the queue
        vector<string> neighbors = findNeighbor(temp.top(), dictionary, usedWords, bShouldAddLetters, bShouldRemoveLetters);
        // for all the neighbors found, add them to the top of a new stack which contains the previous elements at the top of the queue
        // if the neighbor is equal to the second word, we are done. return the new stack with the second word on top
        for(int i = 0; i < neighbors.size(); i++)
        {
            stack<string> neighborStack = temp;
            neighborStack.push(neighbors[i]);
            if(neighbors[i] == word2)
            {
                return neighborStack;
            }
            else
            {
                ladderQueue.push(neighborStack);
            }
        }
    }

    stack<string> nullStack;
    return nullStack;
}

vector<string> findNeighbor(const string &word, const unordered_set<string> &dictionary, unordered_set<string>& filter, bool bShouldAddLetters, bool bShouldRemoveLetters)
{
    vector<string> neighbors;
    for(int i = 0; i < word.size(); i++)
    {
        //for each letter of the alphabet
        for(char iteratorChar = 'a'; iteratorChar < 'z'; iteratorChar++)
        {
            // insert the iteratorChar into position i of the OG word, testing if the result is a word
            string temp = word;
            temp[i] = iteratorChar;
            if(filter.find(temp) == filter.end() && isWord(temp, dictionary))
            {
                neighbors.push_back(temp);
                filter.insert(temp);
            }
            // iterate over the OG word, sliding the iterator char into incrementing positions of the OG word
            if(bShouldAddLetters)
            {
                for(string::iterator itr = temp.begin(); itr != temp.end(); itr++)
                {
                    temp = word;
                    temp.insert(itr, iteratorChar);
                    if(filter.find(temp) == filter.end() && isWord(temp, dictionary))
                    {
                        neighbors.push_back(temp);
                        filter.insert(temp);
                    }
                }
            }
        }
        // erase the current position, test if the result is a word
        if(bShouldRemoveLetters)
        {
            string temp = word;
            temp.erase(i,1);
            if(filter.find(temp) == filter.end() && isWord(temp, dictionary))
            {
                neighbors.push_back(temp);
                filter.insert(temp);
            }
        }
    }
    return neighbors;
}

void loadDictionary(ifstream &inDictionary, unordered_set<string> &outDictionary)
{
    string dictionaryFileName;
    while(1)
    {
        cout << "Dictionary file name: ";
        getline(cin, dictionaryFileName);
        inDictionary.open(dictionaryFileName, ifstream::in);
        if(inDictionary.good())
        {
             break;
        }
        else
        {
           cout << "Invalid filename for dictionary, try again" << endl;
        }
    }

    string word;
    while(inDictionary >> word)
    {
        outDictionary.insert(word);
    }
    cout << endl;
}

bool isWord(const string word, const unordered_set<string> &dict)
{
    if(dict.find(toLower(word)) != dict.end())
    {
        return true;
    }
    return false;
}


bool getWord(string &inputString, const string prompt, const unordered_set<string> &dict, bool bMustBeWord)
{
    while(1)
    {
        cout << prompt;
        getline(cin, inputString);
        //user wants to quit
        if(inputString == QUITSTRING)
        {
            return false;
        }
        else if(containsNonAlpha(inputString))
        {
            cout << "Input must not contain non-alphanumeric characters, try again." << endl;
            continue;
        }
        else if(bMustBeWord && !isWord(inputString, dict))
        {
            cout << "Input must be a word contained in dictionary, try again." << endl;
            continue;
        }
        else
        {
            return true;
        }
    }
}

bool containsNonAlpha(const string &s)
{

    for(int i = 0; i < s.size(); i++)
    {
        if(!isalpha(s[i]))
        {
            return true;
        }
    }
    return false;
}


string toLower(const string &s)
{
   string returnString;
   for(int i = 0; i < s.size(); i++)
   {
        returnString.push_back(tolower(s[i]));

   }
   return returnString;
}


#include <string>
#include "grid.h"
#include "lexicon.h"
#include "set.h"
#include "vector.h"
#include "random.h"
#include "bogglegui.h"
using namespace std;


/**
 * @brief wordSearchHelper - recursively iterates over nearby tiles stemming from rowpos, colpos
 * @param rowpos - stem row position
 * @param colpos - stem col position
 * @param board - board to iterate over
 * @param word - word to search for, chopping off the pos 0 element at each iteration
 * @param usedLetters - positions (row,col) that we have used before, prevents the use of the same letter twice
 * @return - true if the word is size 0 or if letter was found
 *
 * Assumes word size is > 0
 */
bool humanWordSearchHelper(int rowpos, int colpos, const Grid<char> &board, const string &word, set<std::pair<int, int> > &usedLetters);


/**
 * @brief computerWordSearchHelper - recursively searches the board for all valid words.
 * A word is valid if it exists in the dictionary, and can be formed from adjacent tiles on the board
 * @param rowpos - stem row position
 * @param colpos - stem col position
 * @param word - accumulation of characters from prior iterations.
 * is checked at each iteration to determine if the characters form a word, or
 * are a prefix of a word. If neither case, the word is aborted. this variable is backtracked
 * so after each iteration, the last character is removed (resulting in the word being empty at the top
 * of the call tree)
 * @param dictionary - lexicon which contains all existing words
 * @param allWords - storage for all valid words
 * @param board - board which is being searched
 * @param usedLetters - storage for tiles which have already been used. prevents doublecounting.
 * this variable is backtracked, so after each iteration we remove tiles added, resulting in an
 * empty set at the top of the call tree
 */
void computerWordSearchHelper(int rowpos, int colpos, string &word, const Lexicon& dictionary, Set<string>& allWords, const Grid<char>& board, set<std::pair<int,int>> &usedLetters);

/**
 * @brief numPointsScored - generates points based on word size
 * @return - number of points scored
 */
int numPointsScored(const string &word);

/**
 * @brief generateRandomBoard
 * generates a random board with no cube (represented by vector lettercubes) used twice
 * @param size - size of board
 * @param letterCubes - string with each character representing a face of the cube
 * @return randomized size*size board based on the vector of cubes and the size passed in
 */
Grid<char> generateRandomBoard(int size, const Vector<std::string>& letterCubes)
{

    Grid<char> board(size, size);
    Vector<std::string> unusedCubes = letterCubes;
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            int selectedCube = randomInteger(0,unusedCubes.size()-1);
            std::string cube = unusedCubes[selectedCube];
            board[i][j] = cube[randomInteger(0,cube.size()-1)];
            unusedCubes.remove(selectedCube);
        }
    }
    return board;

}

/**
 * @brief humanWordSearch - searches the board to determine if the input word is valid
 * @param board - valid x * x board, initialized with valid A-Z characters
 * @param dictionary - lexicon containing all valid words
 * @param word - search term
 * @return true if the word is both valid in the dictionary and valid on the board
 *
 * Assumes worde size is > 0
 */
bool humanWordSearch(const Grid<char>& board, const Lexicon& dictionary, const string& word)
{
    // there are no zero sized words
    if(word.size() < 1)
    {
        throw("Word size must be > 0");
    }

    BoggleGUI::clearHighlighting();
    if(word.size() < BoggleGUI::MIN_WORD_LENGTH)
    {
        return false;
    }

    // if the dictionary doesnt contain the word, it isnt valid
    if (!dictionary.contains(word))
    {
        return false;
    }

    // iterating over the entire board..
    for(int i = 0; i < board.numRows(); i++)
    {
        for(int j = 0; j < board.numCols(); j++)
        {
            //choose
            BoggleGUI::setHighlighted(i,j,true,true);
            set<std::pair<int,int>> usedLetters;
            usedLetters.insert(pair<int,int>(i,j));

            // explore
            // call wordSearchHelper to search the positions next to ours (row -1 -> row +1, col -1 -> col +1)
            // only return early if we found a valid path
            if(humanWordSearchHelper(i,j,board,word, usedLetters))
            {
                BoggleGUI::scorePointsHuman(numPointsScored(word));
                return true;
            }

            // unchoose
            BoggleGUI::setHighlighted(i,j,false,true);
            // could have left this out, but its here for clarity instead
            usedLetters.erase(pair<int,int>(i,j));

        }
    }

    // otherwise the letter at word[0] does not exist on the board
    // thus the word is not valid
    return false;

}

/**
 * @brief humanWordSearchHelper - recursively iterates over nearby tiles stemming from rowpos, colpos
 * @param rowpos - stem row position
 * @param colpos - stem col position
 * @param board - board to iterate over
 * @param word - word to search for, chopping off the pos 0 element at each call
 * @param usedLetters - positions (row,col) that we have used before, prevents the use of the same letter twice
 * @return - true if the word is size 0 or if letter was found
 */
bool humanWordSearchHelper(int rowpos, int colpos, const Grid<char>& board, const string& word, set<std::pair<int,int>> &usedLetters)
{
    if(word.size() < 1)
    {
        throw("Word size must be > 0");
    }

    // if there is only one character remaining, and we have found a square that matches that character
    // we're done
    if(word.size() == 1 && word[0] == board[rowpos][colpos])
    {
        return true;
    }

    // if the location passed doesnt match the start of the word
    // invalid location
    if(word[0] != board[rowpos][colpos])
    {
        return false;
    }

    //choose (tile around rowpos,colpos)
    for(int i = -1; i <=1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            if( board.inBounds(rowpos+i,colpos+j)
                && usedLetters.find(std::pair<int,int>(rowpos + i, colpos + j)) == usedLetters.end()) // hasnt previously been used
            {
               //choose
               usedLetters.insert(pair<int,int>(rowpos+i,colpos+j));
               BoggleGUI::setHighlighted(rowpos+i,colpos+j,true,true);

               // explore + unchoose, as we are passing in rowpos+i, colpos+j
               // and a substr of word, which will be undone after return.
               if(humanWordSearchHelper(rowpos+i,colpos+j,board,word.substr(1,word.size()-1),usedLetters))
               {
                   // only want to return early if we found a valid path
                   return true;
               }

               // unchoose part2
               BoggleGUI::setHighlighted(rowpos+i,colpos+j,false,true);
               usedLetters.erase(pair<int,int>(rowpos+i,colpos+j));
            }
        }
    }


    // if we have reached this point, then the letter at the front of the word passed to us
    // could not be found, thus it is not possible to continue and the word is not valid on the board
    return false;

}

/**
 * @brief computerWordSearch - starter for recursion, iterating over entire board
 * @param board - board to search
 * @param dictionary - lexicon containing all words that exist
 * @param humanWords - valid words already found by human
 * @return all words that are valid on the board passed
 */
Set<string> computerWordSearch(const Grid<char>& board, const Lexicon& dictionary, const Set<string>& humanWords)
{

    Set<string> allWords = humanWords;
    for(int i = 0; i < board.numRows(); i++)
    {
        for(int j = 0; j < board.numCols(); j++)
        {
            // variables in here are recreated at each iteration of the forloop above
            // no need to backtrack obviously
            string word;
            word.push_back(board[i][j]);

            set<std::pair<int,int>> usedLetters;
            usedLetters.insert(pair<int,int>(i,j));

            computerWordSearchHelper(i,j,word,dictionary,allWords, board, usedLetters);
        }
    }
    return allWords;

}

/**
 * @brief computerWordSearchHelper - recursively searches the board for all valid words.
 * A word is valid if it exists in the dictionary, and can be formed from adjacent tiles on the board
 * @param rowpos - stem row position
 * @param colpos - stem col position
 * @param word - accumulation of characters from prior iterations.
 * is checked at each iteration to determine if the characters form a word, or
 * are a prefix of a word. If neither case, the word is aborted. this variable is backtracked
 * so after each iteration, the last character is removed (resulting in the word being empty at the top
 * of the call tree)
 * @param dictionary - lexicon which contains all existing words
 * @param allWords - storage for all valid words
 * @param board - board which is being searched
 * @param usedLetters - storage for tiles which have already been used. prevents doublecounting.
 * this variable is backtracked, so after each iteration we remove tiles added, resulting in an
 * empty set at the top of the call tree
 */
void computerWordSearchHelper(int rowpos, int colpos, string &word, const Lexicon& dictionary, Set<string>& allWords, const Grid<char>& board, set<std::pair<int,int>> &usedLetters)
{
   if(dictionary.contains(word) && word.size() >= BoggleGUI::MIN_WORD_LENGTH && !allWords.contains(word))
   {
       BoggleGUI::scorePointsComputer(numPointsScored(word));
       allWords.insert(word);
   }
   // the word isnt a valid word, and isnt the valid start of a word
   if(!dictionary.containsPrefix(word))
   {
       return;
   }
   // search the neighbors
   for(int i = -1; i <= 1; i++)
   {
       for(int j = -1; j <= 1; j++)
       {
           // if we havent already visited this tile
           if(board.inBounds(rowpos+i,colpos+j) && usedLetters.find(pair<int,int>(rowpos + i, colpos +j)) == usedLetters.end())
           {
               // choose
               word.push_back(board[rowpos+i][colpos+j]);
               usedLetters.insert(pair<int,int>(rowpos+i,colpos+j));

               // explore
               computerWordSearchHelper(rowpos+i,colpos+j,word,dictionary,allWords,board,usedLetters);

               // undo
               word.pop_back();
               usedLetters.erase(pair<int,int>(rowpos+i,colpos+j));
           }
       }
   }
}

/**
 * @brief numPointsScored - generates points based on word size
 * @return - number of points scored
 */
int numPointsScored(const string &word)
{
    if(word.size() > 7)
    {
        return 11;
    }
    else if (word.size() <= 4)
    {
        return 1;
    }
    else
    {
        switch(word.size())
        {
            case 5:
                return 2;
            case 6:
                return 3;
            case 7:
                return 5;
        }
    }
}

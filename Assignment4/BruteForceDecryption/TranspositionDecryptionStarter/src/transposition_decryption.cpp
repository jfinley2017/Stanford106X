#include <iostream>
#include <ctype.h> // for isspace, isalpha
#include "console.h"
#include "simpio.h"  // for getLine
#include "string.h"
#include "vector.h"  // for Vector
#include "lexicon.h"
#include <queue>
#include <limits>
#include <assert.h>
using namespace std;

// Transposition Cipher
const char PAD = '~'; // padding character for cipher
const int MAX_KEY_LEN = 9;
const int TOP_X = 5;
const int NUM_OPTIONS = 3;
int    iMin = std::numeric_limits<int>::min();
int    iMax = std::numeric_limits<int>::max();

struct DecryptionGuess {
    string potentialPlaintext;
    double wordPercentage;

    bool operator == (const DecryptionGuess &other)
    {
         return this->wordPercentage == other.wordPercentage;
    }

    bool operator != (const DecryptionGuess &other)
    {
         return this->wordPercentage != other.wordPercentage;
    }

    bool operator <(const DecryptionGuess &other)
    {
         return this->wordPercentage < other.wordPercentage;
    }

    bool operator <=(const DecryptionGuess &other)
    {
        return this->wordPercentage <= other.wordPercentage;
    }

    bool operator >(const DecryptionGuess &other)
    {
         return this->wordPercentage > other.wordPercentage;
    }

    bool operator >=(const DecryptionGuess &other)
    {
         return this->wordPercentage >= other.wordPercentage;
    }

};

/**
 * @brief encrypt - uses a transpositional cipher method to encrypt the strint at plaintext
 * @param plaintext - string to be encrypted
 * @param key - key to encrypt with
 * @return - encrypted string
 */
string encrypt(string plaintext, string key);

/**
 * @brief decrypt - decrypts the string at ciphertext using the key
 * @param ciphertext - string to be decrypted
 * @param key - key to decrypt with
 * @return plaintext decrypted string
 */
string decrypt(string ciphertext, string key);

/**
 * @brief cipherToPlaintext - uses the permutation to convert the cipher to plaintext
 * @param ciphertext - text to decipher
 * @param permutation - the offset from the start of the row where the character at
 * the cooresponding index in cipherGrid should be printed
 * @return - plaintext
 *
 * Sort of confusing algorithm.
 * We are treating plaintext as a flat 2d array essentially while iterating Row-Major over cipherGrid (the representation of the ciphertext)
 * permutation = [0,5,6,2,1,3,4]
 * cipherGrid =
 *     j = 0  1  2  3  4  5  6
 * i = 0  [M][M][E][E][E][T][_]
 * permu  [0][5][6][2][1][3][4]
 *
 * cipherGrid[0][0] = plaintext[(rowsPassed * sizeOfRows) + permutation[rowIterator]]
 *
 * plaintext = M______
 * plaintext = M____M_
 * plaintext = M____ME
 * plaintext = M_E__ME
 * plaintext = MEE__ME
 * plaintext = MEET_ME
 * plaintext = MEET_ME (added space)
 *
 */
string cipherToPlaintext(string ciphertext, Vector<int> printOrder);

/* This function produces a vector of DecryptionGuess structs that holds
 * the top X number of best-guesses based on a brute force decryption
 * of a ciphertext.
 */
Vector<DecryptionGuess> bruteForce(string ciphertext);

/**
 * @brief bruteForceHelper - recursive backtracking function which generates all permutations of the deque
 * @param alphaOrdering - deque containing digits to find permutations of
 * @param chosen - permutation being built
 * @param permutations - all permutations
 */
void bruteForceHelper(deque<int> &toBePermuted, Vector<int> &chosen, Vector<Vector<int>> &permutations );

////////////////////////////////
/// Helpers
///


// splits the string at s into strings deliminated by ' ' or _, ignores non-alpha
Vector<string> splitWords(const string &s);

// sorts a DecryptionGuess vector
// insertionsort
void sortDecryptionGuess(Vector<DecryptionGuess> &guesses);


// returns the sorting permutation of a string key
Vector<int> getPermutation(const string &key);

int getIntegerRangeInclusive(string prompt, int min, int max);

////////////////////////////////
// Tests
void testSplit()
{
    Vector<string> test1 = {"this", "that", "them"};
    string testString = "this. that_them";
    Vector<string> result = splitWords(testString);
    if(test1 != result)
    {
        throw("Test1 failed");
    }
}

void testEncrypt()
{
    assert(encrypt("MEET ME AT SEVEN PM IN GATES", "COMPSCI") == "M_ENME_EEVISET_GEAN_T_PA_SMT");
}

void testDecrypt()
{

    assert(decrypt("M_ENME_EEVISET_GEAN_T_PA_SMT", "COMPSCI") == "MEET ME AT SEVEN PM IN GATES");

}

void testBrute()
{
    bool bFoundText = false;
    Vector<DecryptionGuess> guesses = bruteForce(encrypt("Imsko is a sleepy motherfucker", "training"));
    for(DecryptionGuess guess : guesses)
    {
        if(guess.potentialPlaintext == "Imsko is a sleepy motherfucker")
        {
            bFoundText = true;
            break;
        }
    }

    assert(bFoundText);
}


// driver
int main() {

    bool bShouldContinue = true;
    while (bShouldContinue)
    {
        cout << "Welcome to the Transposition Cipher Machine!" << endl;
        cout << "Please choose:" << endl;
        cout << "1) encrypt text" << endl;
        cout << "2) decrypt text" << endl;
        cout << "3) brute force decrypt without the key" << endl;

        int encryptChoice = getIntegerRangeInclusive("Select an option: ", 0, NUM_OPTIONS);
        switch(encryptChoice)
        {
            case 0:
            {
                bShouldContinue = false;
                break;
            }
            case 1:
            {
                string text, key;
                getLine("Enter text to encrypt: ",text);
                getLine("Enter key to encrypt with: ", key);
                cout << "Text: " << encrypt(text,key) << endl << endl;
                break;
            }
            case 2:
            {
                string ciphertext,key;
                getLine("Enter encrypted text: ",ciphertext);
                getLine("Enter key: ", key);
                cout << "Text: " << decrypt(ciphertext,key) << endl << endl;
                break;
            }
            case 3:
            {
                string ciphertext;
                getLine("Enter encrypted text to bruteforce: ", ciphertext);
                Vector<DecryptionGuess> guesses = bruteForce(ciphertext);
                for(int i = 0; i < TOP_X; i++)
                {
                    cout << i+1 << ": " << guesses[i].potentialPlaintext << endl << endl;
                }

                break;
            }
        }
    }
    cout << "Goodbye!" << endl;
    return 0;
}

/**
 * @brief encrypt - uses a transpositional cipher method to encrypt the strint at plaintext
 * @param plaintext - string to be encrypted
 * @param key - key to encrypt with
 * @return - encrypted string
 */
string encrypt(string plaintext, string key)
{

    // pad to divide evenly among columns
    while(plaintext.size() % key.size() != 0)
    {
        plaintext.push_back(PAD);
    }
    // init grid that will eventually hold the sorted values of the newly encrypted phrase
    Vector<string> cipherGrid(key.size());
    for(int i = 0; i < plaintext.size(); i++)
    {
       plaintext[i] == ' ' ? cipherGrid[i % key.size()] += '_' : cipherGrid[i % key.size()] += plaintext[i];
    }

    // sort, swapping both the key value and the grid values
    for(int i = 0; i < key.size(); i++)
    {
         for(int j = i+1; j < key.size(); j++)
         {
             if(key[j] < key[i])
             {

                string tempString = cipherGrid[j];
                cipherGrid[j] = cipherGrid[i];
                cipherGrid[i] = tempString;

                char temp = key[j];
                key[j] = key[i];
                key[i] = temp;

             }
         }
    }
    // combine the columns together to get the final phrase
    string ciphertext ="";
    for(string fragment : cipherGrid)
    {
        ciphertext += fragment;
    }
    return ciphertext;

}

/**
 * @brief decrypt - decrypts the string at ciphertext using the key
 * @param ciphertext - string to be decrypted
 * @param key - key to decrypt with
 * @return plaintext decrypted string
 */
string decrypt(string ciphertext, string key)
{
    if (ciphertext.size() % key.size() != 0)
    {
        throw("Ciphertext length is not divisible by key length!");
    }

    return cipherToPlaintext(ciphertext, getPermutation(key));

}


/**
 * @brief cipherToPlaintext - uses the permutation to convert the cipher to plaintext
 * @param ciphertext - text to decipher
 * @param permutation - the offset from the start of the row where the character at
 * the cooresponding index in cipherGrid should be printed
 * @return - plaintext
 *
 * Sort of confusing algorithm.
 * We are treating plaintext as a flat 2d array essentially while iterating Row-Major over cipherGrid (the representation of the ciphertext)
 * permutation = [0,5,6,2,1,3,4]
 * cipherGrid =
 *     j = 0  1  2  3  4  5  6
 * i = 0  [M][M][E][E][E][T][_]
 * permu  [0][5][6][2][1][3][4]
 *
 * cipherGrid[0][0] = plaintext[(rowsPassed * sizeOfRows) + permutation[rowIterator]]
 *
 * plaintext = M______
 * plaintext = M____M_
 * plaintext = M____ME
 * plaintext = M_E__ME
 * plaintext = MEE__ME
 * plaintext = MEET_ME
 * plaintext = MEET_ME (added space)
 *
 */
string cipherToPlaintext(string ciphertext, Vector<int> permutation)
{

    Vector<string> cipherGrid;
    int numCols = permutation.size();
    int numRows = ciphertext.size() / numCols;
    for(int i = 0; i < numCols; i++)
    {
        cipherGrid.push_back(ciphertext.substr(i*numRows, numRows));
    }

    string plaintext;
    plaintext.resize(ciphertext.size());
    // row major order
    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numCols; j++)
        {
            //skip pad characters and decrement the length of the created string
            if(cipherGrid[j][i] == PAD)
            {
                plaintext.pop_back();
            }
            else if(cipherGrid[j][i] == '_')
            {
                plaintext[(i * numCols) + permutation[j]] = ' ';
            }
            else
            {
                plaintext[(i * numCols) + permutation[j]] = cipherGrid[j][i];
            }
        }
    }

    return plaintext;
}

/* This function produces a vector of DecryptionGuess structs that holds
 * the top X number of best-guesses based on a brute force decryption
 * of a ciphertext.
 */
Vector<DecryptionGuess> bruteForce(string ciphertext)
{

    Vector<DecryptionGuess> decrypt;
    Lexicon dictionary("dictionary.txt");

    deque<int> permutationChoices;
    for(int i = 0; i < MAX_KEY_LEN; i++)
    {
        permutationChoices.push_back(i);
    }

    //storage of all possible key permutations
    Vector<Vector<int>> permutations;
    Vector<int> chosen;
    for(int i = MAX_KEY_LEN; i > 0; i--)
    {
         if(ciphertext.size() % i == 0)
         {
             permutationChoices.resize(i);
             bruteForceHelper(permutationChoices,chosen,permutations);
         }
    }

    for(int i =0; i < permutations.size(); i++)
    {
        DecryptionGuess newGuess;
        newGuess.potentialPlaintext = cipherToPlaintext(ciphertext,permutations[i]);
        double wordCount = 0.0;
        Vector<string> words = splitWords(newGuess.potentialPlaintext);
        for(string word : words)
        {
            if(dictionary.contains(word))
            {
                wordCount++;
            }

        }

        newGuess.wordPercentage = (wordCount / words.size()) * 100;
        decrypt.push_back(newGuess);
    }

    sortDecryptionGuess(decrypt);
    return decrypt;
}

/**
 * @brief bruteForceHelper - recursive backtracking function which generates all permutations of the deque
 * @param alphaOrdering - deque containing digits to find permutations of
 * @param chosen - permutation being built
 * @param permutations - all permutations
 */
void bruteForceHelper(deque<int> &toBePermuted, Vector<int> &chosen, Vector<Vector<int>> &permutations )
{
    if(toBePermuted.empty())
    {
        permutations.push_back(chosen);
    }
    else
    {
        for(int i = 0; i < toBePermuted.size(); i++)
        {
            int chosenInt = toBePermuted[i];
            chosen.push_back(chosenInt);
            deque<int>::iterator it = toBePermuted.begin() + i;
            toBePermuted.erase(it);

            bruteForceHelper(toBePermuted,chosen,permutations);

            chosen.remove(chosen.size() -1 );
            it = toBePermuted.begin() + i;
            toBePermuted.insert(it,chosenInt);


        }
    }
}

////////////////////////////////
// Helper functions

// sorts a DecryptionGuess vector
void sortDecryptionGuess(Vector<DecryptionGuess> &guesses)
{
    // insertion sort
    for(int i = 0; i < guesses.size(); i++)
    {
        for(int j = i+1; j < guesses.size(); j++)
        {
            if(guesses[j] > guesses[i])
            {
               DecryptionGuess temp = guesses[j];
               guesses[j] = guesses[i];
               guesses[i] = temp;
            }
        }
    }
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

// splits the string at s into strings deliminated by ' ' or _, ignores non-alphas
Vector<string> splitWords(const string &s)
{
    Vector<string> words;
    string word = "";
    for(char letter : s)
    {
        if( (letter == ' ' || letter == '_') && word != "")
        {
            words.push_back(word);
            word = "";
        }
        else if(isalpha(letter))
        {
            word += letter;
        }
    }
    words.push_back(word);
    return words;
}


// returns the sorting permutation of a string key
Vector<int> getPermutation(const string &key)
{
    Vector<int> permutation;

    for(int i = 0; i < key.size(); i++)
    {
        permutation.push_back(i);
    }

    for(int i = 0; i < key.size(); i++)
        {
            for(int j = i+1; j < key.size(); j++)
            {
                if(key[permutation[i]] > key[permutation[j]])
                {
                    int temp = permutation[i];
                    permutation[i] = permutation[j];
                    permutation[j] = temp;
                }
            }
        }

    return permutation;
}

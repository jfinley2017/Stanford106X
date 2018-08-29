
#include <string>
#include "letterinventory.h"
#include <vector.h>
#include <queue>
#include "set.h"
using namespace std;


/**
 * @brief findAnagramsHelper helper which recursively builds anagrams based on an original
 * phrase stored in @param inventory.
 * @param inventory - unused letters, starts with all letters then recursively removes used letters
 * based on words selected on each iteration
 * @param max - max number of words allowed in a valid anagram
 * @param choices - queue containing all valid words that can be used based on the original phrase
 * this is diminished as we select words to include in our anagram
 * @param result - anagram that is being formed. is added to each time we find valid words to include
 * @return num anagrams created
 */
int findAnagramsHelper(LetterInventory &inventory, int max, queue<string> &choices, Vector<string>& result)
{

    //base case, no more letters to use, result contains the anagram
    if(inventory.isEmpty())
    {
       cout << result.toString() << endl;
       return 1;
    }

    // if we reached the max words specified, but no anagrams were found
    // break out
    if(result.size() == max && max > 0)
    {
        return 0;
    }

    //sum of num anagrams created by our choice trees
    int count = 0;
    for(int i = 0; i < choices.size(); i++)
    {
        // select the word at the front of the queue
        string word = choices.front();
        choices.pop();
        if(inventory.contains(word))
        {
            // choose
            inventory.subtract(word);
            result.push_back(word);

            // explore
            count += findAnagramsHelper(inventory, max, choices, result);

            // undo
            inventory.add(word);
            result.remove(result.size()-1);

        }
        // add the word back to the queue, as will need to explore it in future calls
        choices.push(word);
    }
    return count;
}


/**
 * @brief findAnagrams, loader function which creates an empty vector to store the resulting anagrams in and begins recursion
 * @param phrase - user inputted phrase
 * @param max - user inputted anagram max length
 * @param dictionary - lexicon containing valid words which we will use to generate choices
 * @return number anagrams created
 */
int findAnagrams(const string& phrase, int max, const Set<string>& dictionary) {

    LetterInventory inventory = LetterInventory(phrase);

    // loads the choices (words which could be formed from the letters of the inputted phrase) based on the provided dictionary
    queue<string> choices;
    for(string word : dictionary)
    {
        if(inventory.contains(word))
        {
            choices.push(word);
        }
    }

    Vector<string> result;
    return findAnagramsHelper(inventory, max, choices, result);
}



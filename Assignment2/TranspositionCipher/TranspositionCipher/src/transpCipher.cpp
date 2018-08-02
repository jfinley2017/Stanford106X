#include <iostream>
#include "console.h"
#include "simpio.h"  // for getLine
#include "vector.h"  // for Vector
using namespace std;

// Transposition Cipher

const char PAD = '~'; // padding character for cipher

// Function Prototypes
// Add more functions of your own, as well
string encrypt(string plaintext, string key);
string decrypt(string ciphertext, string key);

int main() {

    int encryptChoice = 1;
    while (encryptChoice > 0) {
        cout << "Welcome to the Transposition Cipher Machine!" << endl;
        cout << "Please choose:" << endl;
        cout << "1) encrypt text" << endl;
        cout << "2) decrypt text" << endl;
        encryptChoice = getInteger("Please type your choice, 0 to end:","Please type an integer!");

        if (encryptChoice > 0) {
            cout << "Please enter the text to " << (encryptChoice == 1 ? "encrypt: " : "decrypt: ");
            string text = getLine();
            string key = getLine("Please type in a key: ");
            string output;
            if (encryptChoice == 1) {
                output = encrypt(text, key);
                cout << "Encrypted ";

            } else if (encryptChoice == 2) {
                output = decrypt(text, key);
                cout << "Decrypted ";
            }
            cout << "text:" << endl << endl << "\"" << output << "\"" << endl << endl;
        }
    }
    cout << "Goodbye!" << endl;

    return 0;
}

string encrypt(string plaintext, string key)
{
    string ciphertext;

    while(plaintext.size() % key.size() != 0)
    {
        plaintext.push_back(PAD);
    }
    vector<string> cipherVector;
    cipherVector.resize(key.size());

    for(int i = 0; i < plaintext.size(); i++)
    {
        if(plaintext[i] == ' ')
        {
             cipherVector[i % key.size()] += '_';
        }
        else
        {
             cipherVector[i % key.size()] += plaintext[i];
        }
    }

    for(int i = 0; i < key.size(); i++)
    {
        for(int j = i+1; j < key.size(); j++)
        {
            if(key[j] < key[i])
            {
               char tempC = key[i];
               key[i] = key[j];
               key[j] = tempC;
               string tempS = cipherVector[i];
               cipherVector[i] = cipherVector[j];
               cipherVector[j] = tempS;

            }
        }
    }

    for(int i = 0; i < cipherVector.size(); i++)
    {
        ciphertext += cipherVector[i];
    }

    cout << ciphertext;
    cout << endl;
    return ciphertext;
}

string decrypt(string ciphertext, string key)
{
    if (ciphertext.size() % key.size() != 0)
    {
        throw("Ciphertext length is not divisible by key length!");
    }

    string plaintext;
    vector<string> cypherTextVector;
    vector<int> sortOrder;
    cypherTextVector.resize(key.size());

    //initialize
    for(int i = 0; i < key.size(); i++)
    {
        for(int j = 0; j < ciphertext.size() / key.size(); j++)
        {
            cypherTextVector[i] += ciphertext[i * (ciphertext.size() / key.size()) + j];
        }

        sortOrder.push_back(i);

    }

    //sort
    //sortOrder will now contain a value cooresponding to the unsorted key
    //ex: COMPSCI->CCIMOPS [0,5,6,2,1,3,4]
    //thus we can now print in the order designated by sortOrder
    for(int i = 0; i < key.size(); i++)
    {
        for(int j = i+1; j < key.size(); j++)
        {
            if(key[sortOrder[i]] > key[sortOrder[j]])
            {
                int tempI = sortOrder[i];
                sortOrder[i] = sortOrder[j];
                sortOrder[j] = tempI;
            }
        }
    }

    //utilize sortOrder to print the decrypt vector in order by treating plaintext like a flat 2d array
    //the element at cypherTextVector[i][j] should be placed at plaintext[row][sortedOrder[j]]
    //rows are of size key.size, thus i*key.size gives us the row offset
    //column is determined by sortedOrder[j], which gives us the column the element was originally in
    plaintext.resize(ciphertext.size());
    for(int i = 0; i < (ciphertext.size() / key.size()); i++)
    {
        for(int j = 0; j < key.size(); j++)
        {
            //skip pad characters and decrement the length of the created string
            if(cypherTextVector[j][i] == PAD)
            {
                plaintext.pop_back();
                continue;
            }
            else if(cypherTextVector[j][i] == '_')
            {
                 plaintext[(i * key.size()) + sortOrder[j]] = ' ';
            }
            else
            {
                 plaintext[(i * key.size()) + sortOrder[j]] = cypherTextVector[j][i];
            }
        }
    }

    return plaintext;
}

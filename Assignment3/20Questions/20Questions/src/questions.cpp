// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include <iostream>
using namespace std;
#include "simpio.h"
#include "strlib.h"


void skip(istream &input);

// is only called when false is answered
// if the next line is a question, skip the next two lines (the yes/no child of that question)
// else just skip one line to put us at the no result
void skip(istream &input)
{
    string line1;
    getline(input,line1);
    if(startsWith(line1, "Q:"))
    {
        skip(input);
        skip(input);
        return;
    }


    return;

}

// recursive function which asks the questions and waits for answers
// if the question found at the current line is discovered to be an answer, return the result as the result of the game
// else continue working down the QA file until an answer is found
bool playQuestionGame(istream& input)
{
    string question;
    getline(input,question);
    if(startsWith(question, "A:"))
    {
        return getYesOrNo(question,"Yes or no answers.");
    }
    // ask the question at the line, skip to the next correct line if false
    else
    {
        if(getYesOrNo(question,question) == true)
        {
            return playQuestionGame(input);
        }
        else
        {
            skip(input);
            return playQuestionGame(input);
        }
    }
}

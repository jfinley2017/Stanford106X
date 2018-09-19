// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include "questiontree.h"
#include "strlib.h"
#include "simpio.h"

using namespace std;

bool isQuestion(const string& phrase) {
    return startsWith(phrase,"Q:") ? true : false;
}

// helper function, returns string with the format "A:{a-z}."
string formatAnswer(const string &answer) {
    string newString;
    if(!startsWith(answer, "A:"))
    {
        newString += "A:" + answer;
    }
    else
    {
        newString = answer;
    }
    return newString;
}

// helper function, returns string with the format "Q:{a-z}?"
string formatQuestion(const string &question) {
    string newString;
    if(!startsWith(question, "Q:"))
    {
        newString += "Q:" + question;
    }
    else
    {
        newString = question;
    }
    if(!(question.at(question.size()-1) == '?'))
    {
        newString += '?';
    }
    return newString;
}

string getFormattedQuestion(const string &prompt)
{
    string inputtedString = getLine(prompt);
    inputtedString = formatQuestion(inputtedString);
    return inputtedString;
}

string getFormattedAnswer(const string &prompt)
{
    string inputtedString = getLine(prompt);
    inputtedString = formatAnswer(inputtedString);
    return inputtedString;
}

QuestionTree::QuestionTree() {
    root = nullptr;
    gamesPlayed = 0;
    gamesWon = 0;
}

QuestionTree::~QuestionTree() {
     clear();
}

int QuestionTree::getGamesLost() const {
   return gamesPlayed - gamesWon;
}

int QuestionTree::getGamesWon() const {
    return gamesWon;
}

void QuestionTree::playGame() {
    playGame(root);
}

void QuestionTree::playGame(QuestionNode* root) {
    if(!root)
    {
        return;
    }
    // check if we have reached an answer line
    if(!isQuestion(root->data))
    {
        attemptAnswer(root);
        gamesPlayed++;
        // do other answer-y stuff here
        return;
    }
    askQuestion(root->data) ? playGame(root->yes) : playGame(root->no);
    return;
}


bool QuestionTree::askQuestion(const string& question) {
   return getYesOrNo(question,"Yes or no");
}

bool QuestionTree::attemptAnswer(QuestionNode* answerNode) {
    // two paths here
    // if the answer is correct, we win
    // otherwise we lost and we must update our questions with a new question and answer
    if(getYesOrNo("Are you thinking of: " + answerNode->data.substr(2,answerNode->data.size()-2),"Yes or no"))
    {
        gamesWon++;
        return true;
    }
    else
    {
        // the answer that we attempted
        QuestionNode* wrongAnswer = new QuestionNode(answerNode->data);
        // the answer that the user was thinking of
        QuestionNode* rightAnswer = new QuestionNode(getFormattedAnswer("I lose, what were you thinking of?"));

        // place the new question at this current node (the one passed in to use as an answer previously)
        string questionPrompt = "Type a yes or no question to distinguish "
                                 + rightAnswer->data.substr(2,rightAnswer->data.size()-2) + " from "
                                 + wrongAnswer->data.substr(2,wrongAnswer->data.size()-2);

        answerNode->data = getFormattedQuestion(questionPrompt);

        // the yes and no children are dependent on pathToRightAnswer (one is the submitted answer, and one is the previous answer)
        bool pathToRightAnswer = getYesOrNo("To get to the correct answer, how should that question be answered? (yes or no) ");
        if(pathToRightAnswer == true)
        {
            answerNode->yes = rightAnswer;
            answerNode->no = wrongAnswer;
        }
        else
        {
            answerNode->yes = wrongAnswer;
            answerNode->no = rightAnswer;
        }
        return false;
    }
}

void QuestionTree::clear() {
    clear(root);
    root = nullptr;
}

void QuestionTree::clear(QuestionNode* clearRoot)
{
    if(!clearRoot)
    {
        return;
    }
    else
    {
        // pre-order iteration through tree
        clear(clearRoot->yes);
        clear(clearRoot->no);
        cout << "deleting " << clearRoot->data << endl;
        delete clearRoot;
    }
}

void QuestionTree::readData(istream& input) {

    if(input.peek() == -1)
    {
        return;
    }
    root = readDataHelper(input);
    return;
}

QuestionNode* QuestionTree::readDataHelper(istream& input) {
    if(input.peek() == -1)
    {
        return nullptr;
    }
    QuestionNode* newNode = new QuestionNode();
    getLine(input,newNode->data);
    if(isQuestion(newNode->data))
    {
        newNode->yes = readDataHelper(input);
        newNode->no = readDataHelper(input);
    }
    return newNode;
}

void QuestionTree::writeData(ostream& output) {
    writeData(output,root);
}

void QuestionTree::writeData(ostream& output, QuestionNode* printRoot) {
    if(!printRoot)
    {
        return;
    }
    output << printRoot->data + '\n';
    writeData(output,printRoot->yes);
    writeData(output,printRoot->no);
}


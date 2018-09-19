#ifndef _questiontree_h
#define _questiontree_h

#include <iostream>
#include <string>
#include "questionnode.h"

class QuestionTree {
public:
    QuestionTree();
    ~QuestionTree();
    int getGamesLost() const;
    int getGamesWon() const;
    void playGame();
    void readData(std::istream& input);
    void writeData(std::ostream& output);

private:
    // TODO: declare needed private members here
    QuestionNode* root;
    int gamesPlayed;
    int gamesWon;

    /**
     * @brief playGame - recursively works down the questiontree starting at root asking questions
     * and answrers
     */
    void playGame(QuestionNode* root);

    /**
     * @brief attemptAnswer - helper function which attempts to answer a session
     * adds a user prompted question and answer if the answer at answerNode is not what
     * the user was thinking of
     * @return  - true if the answer at answerNode was the one the user was thinking of
     */
    bool attemptAnswer(QuestionNode* answerNode);

    /**
     * @brief askQuestion - asks a question
     * @return true if the user responds with yes
     */
    bool askQuestion(const std::string& question);

    /**
     * @brief writeData - recursively works down the questiontree with pre-order traversal, writing what is found
     * at printRoot->data to output
     * @param output - place to write to
     */
    void writeData(std::ostream& output, QuestionNode* printRoot);

    /**
     * @brief readDataHelper - recursive function which reads a QuestionTree from a file in:
     * Q
     * Answer yes
     * Answer no
     * format
     */
    QuestionNode* readDataHelper(std::istream& input);

    /**
     * @brief clear - clears the tree
     */
    void clear();

    /**
     * @brief recursively iterates down the tree postorderly, deleting each node it comes across
     */
    void clear(QuestionNode* clearRoot);
};

#endif // _questiontree_h

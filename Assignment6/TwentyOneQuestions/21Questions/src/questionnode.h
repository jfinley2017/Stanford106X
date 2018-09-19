/**
 * CS 106B/X 21 Questions
 *
 * This file implements the bodies of the QuestionNode members declared in
 * questionnode.h.
 *
 * You don't need to modify this file.
 * Your recursive functions should work with an unmodified version of this file.
 *
 * @author Marty Stepp
 * @version 2016/11/13
 * - 16au 106x version
 */

#ifndef _questionnode_h
#define _questionnode_h

#include <string>

struct QuestionNode {
public:
    std::string data;    // question or answer text
    QuestionNode* yes;   // pointer to "yes" subtree (left)
    QuestionNode* no;    // pointer to "no"  subtree (right)

    QuestionNode(const std::string& data = std::string(),
                 QuestionNode* yes = nullptr,
                 QuestionNode* no = nullptr);
};

#endif // _questionnode_h

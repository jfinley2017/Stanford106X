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

#include "questionnode.h"
#include "strlib.h"

QuestionNode::QuestionNode(const std::string& data, QuestionNode* yes, QuestionNode* no)
        : data(data), yes(yes), no(no) {
    // empty
}

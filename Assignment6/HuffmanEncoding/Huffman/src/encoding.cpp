// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include "encoding.h"
#include "priorityqueue.h"
#include <cmath>
#include <stack>
#include <cctype>
using namespace std;

const int PSUEDO_EOF = 256;

Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    char c;
    while(input.get(c))
    {
        if(freqTable.containsKey(c))
        {
            freqTable[c]++;
        }
        else
        {
            freqTable.add(c,1);
        }
    }
    // add EOF character
    freqTable.add(PSUEDO_EOF,1);
    return freqTable;
}

HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    if(freqTable.isEmpty())
    {
        return nullptr;
    }
    PriorityQueue<HuffmanNode*> queue;
    Vector<int> keys = freqTable.keys();
    for(int key : keys)
    {
        HuffmanNode* newNode = new HuffmanNode(key,freqTable[key]);
        queue.add(newNode,newNode->count);
    }
    // combine every two nodes until one remains
    while(queue.size() != 1)
    {
        HuffmanNode* combinedNode = new HuffmanNode();
        combinedNode->zero = queue.dequeue();
        combinedNode->character = NOT_A_CHAR;
        combinedNode->one = queue.dequeue();
        combinedNode->count = combinedNode->zero->count + combinedNode->one->count;
        queue.add(combinedNode,combinedNode->count);
    }

    return queue.front();
}
/**
 * @brief buildEncodingMap - recursively backtracking helper functions to build an encoding map off of encodiing tree treeRoot
 * @param treeRoot - root of tree to build map
 * @param encodingMap - map contaiining characters and their binary string representations
 * @param code - binary string being build from prior choices
 */
void buildEncodingMap(HuffmanNode* treeRoot, Map<int,string> &encodingMap, string& code){
    //base cases
    if(!treeRoot)
    {
        return;
    }
    if(treeRoot->isLeaf())
    {
        encodingMap.add(treeRoot->character,code);
        return;
    }
    else
    {
        //choose, explore, unchoose
        buildEncodingMap(treeRoot->zero,encodingMap,code += "0");
        code.pop_back();
        buildEncodingMap(treeRoot->one,encodingMap,code += "1");
        code.pop_back();
    }
}

Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    if(encodingTree == nullptr)
    {
        return encodingMap;
    }
    string code = "";
    buildEncodingMap(encodingTree, encodingMap, code);
    return encodingMap;
}

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    char c;
    while(input.get(c))
    {
        string bitString = encodingMap[c];
        for(char bit : bitString)
        {
            output.writeBit(bit - '0');
        }
    }
    // add EOF character
    string bitString = encodingMap[PSUEDO_EOF];
    for(char bit : bitString)
    {
        output.writeBit(bit - '0');
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    int inputBit;
    HuffmanNode* current = encodingTree;
    while(input.peek() != -1)
    {
        inputBit = input.readBit();
        if(inputBit == 0)
        {
            current = current->zero;
        }
        else
        {
            current = current->one;
        }
        if(current->character != NOT_A_CHAR)
        {
            if(current->character == PSUEDO_EOF)
            {
                return;
            }
            output.put(current->character);
            current = encodingTree;
        }
    }
}

void compress(istream& input, obitstream& output) {
    if(input.peek() == -1)
    {
        return;
    }
    Map<int,int> freqTable = buildFrequencyTable(input);
    output << freqTable;
    input.clear();
    input.seekg(0, ios::beg);
    encodeData(input,buildEncodingMap(buildEncodingTree(freqTable)), output);

}


void decompress(ibitstream& input, ostream& output) {
    if(input.peek() == -1)
    {
        return;
    }
    Map<int,int> freqTable;
    input >> freqTable;
    decodeData(input,buildEncodingTree(freqTable),output);
}


void freeTree(HuffmanNode* node) {
    if(!node)
    {
        return;
    }
    freeTree(node->one);
    freeTree(node->zero);
    delete node;
}

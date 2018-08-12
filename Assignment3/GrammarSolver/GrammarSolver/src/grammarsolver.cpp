// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include "grammarsolver.h"
#include "strlib.h"
#include "random.h"
#include <string.h>
#include <unordered_map>
using namespace std;

/**
 * generateSentence - recursive function which returns a full sentence starting from the non-terminal symbol specified.
 * @param rules - unordered_map mapping non-terminal symbols to their follow up elements (be it another non-terminal symbol or a terminal word.
 * @param startSymbol - non-terminal symbol at which to start
 * @return a string generated from rules which began at startSymbol
 */
string generateSentence(unordered_map<string, vector<vector<string>> > &rules, const string &startSymbol);

/**
 * @brief loadRules - loads grammar data from the textfile specified from inputFile into an unorderedMap such that:
 * rules[non-terminating] = [rule1][rule2][rule3]...[ruleN]
 * @param inputFile - file to load grammar data from
 * @param rules - unordered map as specified above
 */
void loadRules(istream &inputFile, unordered_map<string, vector<vector<string>> > &rules);

/**
 * @brief isNonTerminal - returns a boolean based on whether the provided string is non-terminal.
 * non-terminal is defined as starting with < and ending with >
 * @param string - string to test if non-terminal
 * @return true if the string specified is non-terminal
 */
bool isNonTerminal(const string &string);

Vector<string> grammarGenerate(istream& input, string symbol, int times)
{

    unordered_map<string, vector<vector<string>> > rules;
    loadRules(input, rules);

    Vector<string> sentences;
    for(int i = 1; i <= times; i++)
    {
        string sentence = generateSentence(rules,symbol);
        sentences.push_back(sentence);
    }
    return sentences;

}

string generateSentence(unordered_map<string, vector<vector<string>> > &rules, const string &startSymbol)
{
    if(!isNonTerminal(startSymbol))
    {
        return startSymbol;
    }
    else
    {

        string sentenceFragment;
        int randomInt = randomInteger(0, rules[startSymbol].size() - 1);
        for(int i = 0; i < rules[startSymbol][randomInt].size(); i++)
        {
            sentenceFragment += generateSentence(rules, rules[startSymbol][randomInt][i]) + " ";
        }

        return trim(sentenceFragment);
    }
}


void loadRules(istream &inputFile, unordered_map<string, vector<vector<string>> > &rules)
{
    string line;
    while(getline(inputFile, line))
    {
        //terminalSymbolSplit[0] now contains non-terminal, [1] contains the rules
        vector<string> terminalSymbolSplit = stringSplit(line, "::=");

        // Exception: duplicate non-terminal.
        if(rules.find(terminalSymbolSplit[0])!= rules.end())
        {
            throw ("Duplicate Non-Terminal Symbol: " + terminalSymbolSplit[0] + " found while loading input file.");
        }

        // further split the terminal rules such that:
        // terminalRules[0] = [rule1part1][rule1part2][rule1part3]
        // |
        // termianRules[1] = [rule2part1][rule2part2][rule2part3]
        vector<string> ruleSplit = stringSplit(terminalSymbolSplit[1],"|");
        vector<vector<string>> terminalRules;
        for(int i = 0; i < ruleSplit.size(); i++)
        {
            vector<string> ruleWhitespaceSplit = stringSplit(ruleSplit[i], " ");
            terminalRules.push_back(ruleWhitespaceSplit);
        }

        // rules[terminalSymbolSplit[0] = [terminalRules[0]][termianRules[1]]
        rules[terminalSymbolSplit[0]] = terminalRules;
    }
}


bool isNonTerminal(const string &string)
{
    if(string[0] == '<' && string[string.size()-1] == '>')
    {
        return true;
    }
    return false;
}

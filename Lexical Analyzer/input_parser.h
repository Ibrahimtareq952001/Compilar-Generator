#ifndef INPUT_PARSER_H_
#define INPUT_PARSER_H_

#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <set>
#include <map>
#include <assert.h>

#include "NFA.h"

using namespace std;

namespace LexicalAnalyzer
{
    class InputParser
    {
    private:
        string inPath;
        const set<string> operators = {"|", " ", "*", "+", "-", "(", ")"};
        map<string, int> precedence = {{"|", 1}, {" ", 2}, {"*", 3}, {"+", 3}, {"(", 4}};
        int num_keywords = 0;
        int num_punc = 0;
        int i_keyword = 0;
        int i_punc = 0;
        int i_reg = 0;
        set<char> punctuations;
        map<int, string> priorityStrings;
        map<string, NFA> basicExps;
        map<string, string> regDefs;
        vector<NFA> nfas;
        NFA combinedNFA;
        void preprocess();
        void preprocessKeywords(string line);
        void preprocessPunctuations(string line);
        void preprocessRegDefs(string line);
        string trim(string inStr);
        string removeSpaces(string inStr);
        vector<string> tokenize(string regDef);
        vector<string> transformToCanonicalReg(vector<string> tokens);
        vector<string> addConcatSymbol(vector<string> tokens);
        NFA buildNFA(vector<string> tokens);
        void nfaStep(stack<string> &operatorsStack, stack<NFA> &operandsStack);
        void handleKeywords(string line);
        void handlePunctuations(string line);
        void handleRegDef(string line, int equalIdx);
        void handleRegEx(string line, int colonIdc);

    public:
        InputParser(string inputPath);
        void parse();
        NFA getCombinedNFA();
        map<int, string> getPriorityStrings();
        set<char> getPunctuations();
    };
}

#endif /* INPUT_PARSER_H_ */
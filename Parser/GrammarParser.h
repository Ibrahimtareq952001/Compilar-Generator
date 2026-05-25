#ifndef GrammarParser_H
#define GrammarParser_H

#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

namespace Parser
{
    class GrammarParser
    {
    private:
        vector<string> terminals;
        vector<string> nonTerminals;
        map<string, vector<vector<string>>> productions;
        map<string, vector<string>> formattedProds;
        string startSymbol;
        set<string> derivedNonTerminalsSet, usedNonTerminalsSet, terminalsSet;
        void parse(string inPath);
        bool isTerminal(string token);
        bool verifyCompleteGrammar();
        void eliminateLeftRecursion();
        void eliminateLeftRecursionStep(string Ai, vector<vector<string>> AiRules);
        void leftFactor();
        void leftFactorStep(string Ai, vector<vector<string>> AiRules);
        void substitute(vector<string> &rule, vector<vector<string>> &allRules, vector<vector<string>> &substitution);
        void formatProductions();
        void writeGrammarToFile();

    public:
        vector<string> getTerminals();
        vector<string> getNonTerminals();
        string getStartSymbol();
        map<string, vector<string>> getProductions();
        GrammarParser(string inPath);

        void parse();
    };
}

#endif /* GrammarParser_H_ */
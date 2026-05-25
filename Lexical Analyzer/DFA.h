#ifndef DFA_H
#define DFA_H

#include <iostream>
#include <utility>
#include <string>
#include <map>
#include <set>
#include <queue>

using namespace std;

namespace LexicalAnalyzer
{
    class DFA
    {
    private:
        map<pair<int, char>, set<int>> NFA; // state number --> next states
        set<char> alphabet;
        map<int, int> nfaFinalStates; // state number --> priority

        map<int, set<int>> dfaStates;
        map<pair<int, char>, int> Dfa;
        map<int, int> dfaFinalStates;
        set<string> ids;

        set<int> epsilonClosure(int state);

        set<int> getNextStates(set<int> states, char symbol);

        void subsetConstructionAlgorithm();

        void dfaMinimization();

    public:
        DFA(const map<pair<int, char>, set<int>> &nfa,
            const set<char> &alph,
            const map<int, int> nfaFinal);

        map<int, set<int>> getDFAStates() const;

        // Getter for DFA
        map<pair<int, char>, int> getDFA() const;

        // Getter for dfaFinalStates
        map<int, int> getDFAFinalStates() const;
    };
}

#endif // DFA_H
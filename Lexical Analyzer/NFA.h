#ifndef NFA_H
#define NFA_H

#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>

using namespace std;

namespace LexicalAnalyzer
{
    class NFA
    {
    private:
        map<int, vector<pair<int, char>>> stateTransitions; // Map of state number to other states with input
        map<int, vector<int>> epsilonTransitions;           // Epsilon transitions
        set<char> alphabet;                                 // String alphabet
        map<int, int> statePriorities;                      // Map of state number to priority
        map<int, string> priorityStrings;                   // Map of priority number to the string
        // 0 --> keyWord
        // 1 --> pun
        int startState = 0, endState = 0;
        int priority = -1; // Set of punctuations
        //    int numStates;                                      // Total number of states in the NFA
        void updateNumStates(int from, int to);

    public:
        NFA();
        void setPriority(int p);
        void addSympol(char input);
        void addTransition(int from, int to, char input);
        void addEpsilonTransition(int from, int to);
        map<int, int> getStatePriorities() const;
        map<int, vector<pair<int, char>>> getStateTransitions() const;
        map<int, vector<int>> getEpsilonTransitions() const;
        void setStatePriority(int state, int priority);
        const map<int, string> getPriorityStrings() const;
        void setPriorityString(int priority, const string &str);
        void addPunctuation(char punctuation);
        set<int> move(const set<int> &currentStates, char input);
        static NFA combine(vector<NFA> vec);
        map<pair<int, char>, set<int>> getNfaTransitions() const;
        set<int> epsilonClosure(const set<int> &states);
        static NFA kleeneClosure(NFA &nfa1);
        static NFA positiveClosure(NFA &nfa1);
        static NFA concatenate(NFA &nfa1, NFA &nfa2);
        static NFA Or(NFA &nfa1, NFA &nfa2);
        set<int> setUnion(const set<int> &set1, const set<int> &set2);
        set<char> getAlphabet() const;
        set<char> getPunctuations() const;
    };
}

#endif
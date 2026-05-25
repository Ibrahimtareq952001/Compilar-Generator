#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>

#include "NFA.h"

using namespace std;
using namespace LexicalAnalyzer;

NFA::NFA() {}

void NFA::setPriority(int p)
{
    priority = p;
}

void NFA::addSympol(char input)
{
    stateTransitions[0].push_back(make_pair(1, input)); // Add transition with input symbol
    endState = 1;
}

void NFA::addTransition(int from, int to, char input)
{
    stateTransitions[from].push_back(make_pair(to, input)); // Add transition with input symbol
    alphabet.insert(input);                                 // Add input symbol to the alphabet
}

// Function to add epsilon transition
void NFA::addEpsilonTransition(int from, int to)
{
    epsilonTransitions[from].push_back(to); // Add epsilon transition
}

NFA NFA::positiveClosure(NFA &nfa1)
{
    map<int, int> m;
    NFA result;
    m[nfa1.startState] = 1;
    for (int i = nfa1.startState + 1; i <= nfa1.endState; i++)
        m[i] = m[i - 1] + 1;
    result.addEpsilonTransition(m[nfa1.endState], m[nfa1.startState]);

    for (const auto &transition : nfa1.stateTransitions)
    {
        int from = m[transition.first]; // Adjust the 'from' state by the offset
        for (const auto &pair : transition.second)
        {
            int to = m[pair.first]; // Adjust the 'to' state by the offset
            char input = pair.second;
            result.addTransition(from, to, input); // Add transitions from 'other' NFA
        }
    }

    for (const auto &transition : nfa1.epsilonTransitions)
    {
        int from = m[transition.first]; // Adjust the 'from' state by the offset
        for (int to : transition.second)
        {
            result.addEpsilonTransition(from, m[to]); // Adjust the 'to' state by the offset
        }
    }

    int newStartState = 0;
    // Create a new final state and connect it to the final states of both NFAs via epsilon transitions
    result.endState = m[nfa1.endState] + 1;
    result.addEpsilonTransition(newStartState, m[nfa1.startState]);
    result.addEpsilonTransition(m[nfa1.endState], result.endState);

    result.startState = 0;

    return result;
}

// Function to apply Kleene closure operation
NFA NFA::kleeneClosure(NFA &nfa1)
{
    map<int, int> m;
    NFA result;
    m[nfa1.startState] = 1;
    for (int i = nfa1.startState + 1; i <= nfa1.endState; i++)
        m[i] = m[i - 1] + 1;
    result.addEpsilonTransition(m[nfa1.endState], m[nfa1.startState]);

    for (const auto &transition : nfa1.stateTransitions)
    {
        int from = m[transition.first]; // Adjust the 'from' state by the offset
        for (const auto &pair : transition.second)
        {
            int to = m[pair.first]; // Adjust the 'to' state by the offset
            char input = pair.second;
            result.addTransition(from, to, input); // Add transitions from 'other' NFA
        }
    }

    for (const auto &transition : nfa1.epsilonTransitions)
    {
        int from = m[transition.first]; // Adjust the 'from' state by the offset
        for (int to : transition.second)
        {
            result.addEpsilonTransition(from, m[to]); // Adjust the 'to' state by the offset
        }
    }

    int newStartState = 0;
    // Create a new final state and connect it to the final states of both NFAs via epsilon transitions
    result.endState = m[nfa1.endState] + 1;
    result.addEpsilonTransition(newStartState, result.endState);
    result.addEpsilonTransition(newStartState, m[nfa1.startState]);
    result.addEpsilonTransition(m[nfa1.endState], result.endState);

    result.startState = 0;

    return result;
}

NFA NFA::concatenate(NFA &nfa1, NFA &nfa2)
{
    map<int, int> m;
    m[nfa2.startState] = nfa1.endState;
    for (int i = nfa2.startState + 1; i <= nfa2.endState; i++)
        m[i] = m[i - 1] + 1;

    // Merge transitions from the 'other' NFA to this NFA
    for (const auto &transition : nfa2.stateTransitions)
    {
        int from = m[transition.first]; // Adjust the 'from' state by the offset
        for (const auto &pair : transition.second)
        {
            int to = m[pair.first]; // Adjust the 'to' state by the offset
            char input = pair.second;
            nfa1.addTransition(from, to, input); // Add transitions from 'other' NFA
        }
    }

    // Merge epsilon transitions from 'other' NFA to this NFA
    for (const auto &transition : nfa2.epsilonTransitions)
    {
        int from = m[transition.first]; // Adjust the 'from' state by the offset
        for (int to : transition.second)
        {
            nfa1.addEpsilonTransition(from, m[to]); // Adjust the 'to' state by the offset
        }
    }
    nfa1.endState = m[nfa2.endState];
    return nfa1;
}

NFA NFA::Or(NFA &nfa1, NFA &nfa2)
{
    NFA result;
    map<int, int> m1, m2;
    m1[nfa1.startState] = 1;
    for (int i = nfa1.startState + 1; i <= nfa1.endState; i++)
        m1[i] = m1[i - 1] + 1;

    m2[nfa2.startState] = m1[nfa1.endState] + 1;
    for (int i = nfa2.startState + 1; i <= nfa2.endState; i++)
        m2[i] = m2[i - 1] + 1;
    // Add a new start state
    int newStartState = 0;
    result.addEpsilonTransition(newStartState, m1[nfa1.startState]); // Connect to the start state of the first NFA
    result.addEpsilonTransition(newStartState, m2[nfa2.startState]); // Connect to the start state of the second NFA

    for (const auto &transition : nfa1.stateTransitions)
    {
        int from = m1[transition.first]; // Adjust the 'from' state by the offset
        for (const auto &pair : transition.second)
        {
            int to = m1[pair.first]; // Adjust the 'to' state by the offset
            char input = pair.second;
            result.addTransition(from, to, input); // Add transitions from 'other' NFA
        }
    }

    // Merge epsilon transitions from 'other' NFA to this NFA
    for (const auto &transition : nfa1.epsilonTransitions)
    {
        int from = m1[transition.first]; // Adjust the 'from' state by the offset
        for (int to : transition.second)
        {
            result.addEpsilonTransition(from, m1[to]); // Adjust the 'to' state by the offset
        }
    }

    for (const auto &transition : nfa2.stateTransitions)
    {
        int from = m2[transition.first]; // Adjust the 'from' state by the offset
        for (const auto &pair : transition.second)
        {
            int to = m2[pair.first]; // Adjust the 'to' state by the offset
            char input = pair.second;
            result.addTransition(from, to, input); // Add transitions from 'other' NFA
        }
    }

    // Merge epsilon transitions from 'other' NFA to this NFA
    for (const auto &transition : nfa2.epsilonTransitions)
    {
        int from = m2[transition.first]; // Adjust the 'from' state by the offset
        for (int to : transition.second)
        {
            result.addEpsilonTransition(from, m2[to]); // Adjust the 'to' state by the offset
        }
    }
    result.startState = 0;

    // Create a new final state and connect it to the final states of both NFAs via epsilon transitions
    result.endState = m2[nfa2.endState] + 1;

    result.addEpsilonTransition(m1[nfa1.endState], result.endState); // Connect to the final state of the first NFA
    result.addEpsilonTransition(m2[nfa2.endState], result.endState); // Connect to the final state of the second NFA

    return result;
}

map<int, int> NFA::getStatePriorities() const
{
    return statePriorities;
}

// Getters for accessing NFA components
map<int, vector<pair<int, char>>> NFA::getStateTransitions() const
{
    return stateTransitions;
}

map<int, vector<int>> NFA::getEpsilonTransitions() const
{
    return epsilonTransitions;
}

set<char> NFA::getAlphabet() const
{
    return alphabet;
}

const map<int, string> NFA::getPriorityStrings() const
{
    return priorityStrings;
}

NFA NFA::combine(vector<NFA> vec)
{
    vector<map<int, int>> m;
    NFA result;
    map<int, int> curr;
    curr[vec[0].startState] = 1;
    m.push_back(curr);
    for (int j = 0; j < vec.size(); j++)
    {
        for (int i = vec[j].startState + 1; i <= vec[j].endState; i++)
            m[j][i] = m[j][i - 1] + 1;
        if (j != vec.size() - 1)
        {
            map<int, int> curr2;
            curr2[vec[j + 1].startState] = m[j][vec[j].endState] + 1;
            m.push_back(curr2);
        }
    }

    // Add a new start state
    int newStartState = 0;
    for (int j = 0; j < vec.size(); j++)
    {
        result.addEpsilonTransition(newStartState, m[j][vec[j].startState]); // Connect to the start state of each NFA
    }

    for (int j = 0; j < vec.size(); j++)
    {
        for (const auto &transition : vec[j].stateTransitions)
        {
            int from = m[j][transition.first]; // Adjust the 'from' state by the offset
            for (const auto &pair : transition.second)
            {
                int to = m[j][pair.first]; // Adjust the 'to' state by the offset
                char input = pair.second;
                result.addTransition(from, to, input); // Add transitions from 'other' NFA
            }
        }

        // Merge epsilon transitions from 'other' NFA to this NFA
        for (const auto &transition : vec[j].epsilonTransitions)
        {
            int from = m[j][transition.first]; // Adjust the 'from' state by the offset
            for (int to : transition.second)
            {
                result.addEpsilonTransition(from, m[j][to]); // Adjust the 'to' state by the offset
            }
        }
    }
    // assign final states with their priorities
    for (int j = 0; j < vec.size(); j++)
    {
        int state, pri;
        state = m[j][vec[j].endState];
        pri = vec[j].priority;
        result.statePriorities.insert(make_pair(state, pri));
    }

    result.startState = 0;
    return result;
}

map<pair<int, char>, set<int>> NFA::getNfaTransitions() const
{
    map<pair<int, char>, set<int>> NfaTransitions;
    for (const auto ele : stateTransitions)
    {
        int s = ele.first;
        for (const auto p : ele.second)
        {
            NfaTransitions[make_pair(s, p.second)].insert(p.first);
        }
    }
    for (const auto ele : epsilonTransitions)
    {
        int s = ele.first;
        for (int p : ele.second)
        {
            NfaTransitions[make_pair(s, '\0')].insert(p);
        }
    }
    return NfaTransitions;
}
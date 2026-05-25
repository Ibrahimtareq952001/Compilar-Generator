#include <iostream>
#include <fstream>
#include <assert.h>
#include <set>
#include <algorithm>

#include "GrammarParser.h"

using namespace std;
using namespace Parser;

GrammarParser::GrammarParser(string inPath)
{
    parse(inPath);
    for (auto terminal : terminalsSet)
    {
        terminals.push_back(terminal);
    }
    assert(verifyCompleteGrammar());
    eliminateLeftRecursion();
    leftFactor();
    formatProductions();
    writeGrammarToFile();
}

void GrammarParser::writeGrammarToFile()
{
    ofstream outFile("output files/LL1 grammar.txt");
    for (auto p : formattedProds)
    {
        outFile << p.first << " -> ";
        vector<string> rules = p.second;
        for (int i = 0; i < rules.size(); i++)
        {
            string rule = rules[i];
            if (rule.empty())
            {
                outFile << "EPSILON";
            }
            outFile << rule;
            if (i < rules.size() - 1)
                outFile << " | ";
        }
        outFile << endl;
    }
    outFile.close();
}

void GrammarParser::eliminateLeftRecursionStep(string Ai, vector<vector<string>> AiRules)
{
    // A -> Ab | Ac | d | f
    // A -> dA' | fA'
    // A' -> bA' | cA' | EPSILON
    vector<vector<string>> leftRecursionRules, LL1Rules;
    for (auto &AiRule : AiRules)
    {
        if (Ai == AiRule[0])
        { // left recursion
            leftRecursionRules.push_back(AiRule);
        }
        else
        {
            LL1Rules.push_back(AiRule);
        }
    }
    if (leftRecursionRules.empty())
        return;

    string newNonTerminal = Ai + "\'";
    vector<vector<string>> newNonTerminalRules;

    for (auto &rule : LL1Rules)
    {
        rule.push_back(newNonTerminal);
    }
    productions[Ai] = LL1Rules;

    for (auto &rule : leftRecursionRules)
    {
        vector<string> newRule;
        newRule.insert(newRule.end(), rule.begin() + 1, rule.end());
        newRule.push_back(newNonTerminal);
        newNonTerminalRules.push_back(newRule);
    }
    vector<string> epsilonRule = {""};
    newNonTerminalRules.push_back(epsilonRule);
    productions[newNonTerminal] = newNonTerminalRules;

    // add Ai' immediately after Ai
    int AiIndex = find(nonTerminals.begin(), nonTerminals.end(), Ai) - nonTerminals.begin();
    assert(AiIndex < nonTerminals.size());
    nonTerminals.insert(nonTerminals.begin() + AiIndex + 1, newNonTerminal);
}

void GrammarParser::leftFactorStep(string Ai, vector<vector<string>> AiRules)
{
    vector<vector<string>> newAiRules;
    string newNonTerminal = Ai + "\"";
    map<string, vector<vector<string>>> groups;

    int AiIndex = find(nonTerminals.begin(), nonTerminals.end(), Ai) - nonTerminals.begin();
    assert(AiIndex < nonTerminals.size());

    int numNewNonTerminals = 0;
    for (auto AiRule : AiRules)
    {
        string start = AiRule[0];
        groups[start].push_back(AiRule);
    }
    for (auto group : groups)
    {
        string start = group.first;
        vector<vector<string>> commonStartRules = group.second;
        if (commonStartRules.size() > 1)
        {
            numNewNonTerminals++;
            newNonTerminal += to_string(numNewNonTerminals);
            vector<string> LL1Rule = {start, newNonTerminal};
            newAiRules.push_back(LL1Rule);

            vector<vector<string>> newNonTerminalRules;
            for (auto rule : commonStartRules)
            {
                vector<string> newRule;
                newRule.insert(newRule.end(), rule.begin() + 1, rule.end());
                if (newRule.empty())
                    newRule.push_back("");
                newNonTerminalRules.push_back(newRule);
            }
            productions[newNonTerminal] = newNonTerminalRules;

            // add Ai' immediately after Ai
            nonTerminals.insert(nonTerminals.begin() + AiIndex + numNewNonTerminals, newNonTerminal);
        }
        else
        {
            newAiRules.push_back(commonStartRules[0]);
        }
    }
    productions[Ai] = newAiRules;
}

void GrammarParser::substitute(vector<string> &rule, vector<vector<string>> &allRules, vector<vector<string>> &substitution)
{
    for (auto &s : substitution)
    {
        s.insert(s.end(), rule.begin() + 1, rule.end());
    }
    rule = substitution[0];
    allRules.insert(allRules.end(), substitution.begin() + 1, substitution.end());
}

void GrammarParser::eliminateLeftRecursion()
{
    for (int i = 0; i < nonTerminals.size(); i++)
    {
        string Ai = nonTerminals[i];
        vector<vector<string>> AiRules = productions[Ai];
        for (int j = 0; j < i; j++)
        {
            string Aj = nonTerminals[j];
            for (int k = 0; k < AiRules.size(); k++)
            {
                vector<string> &AiRule = AiRules[k];
                if (AiRule[0] == Aj)
                {
                    vector<vector<string>> substitution = productions[Aj];
                    substitute(AiRule, AiRules, substitution);
                }
            }
        }
        eliminateLeftRecursionStep(Ai, AiRules);
    }
}

void GrammarParser::leftFactor()
{

    for (int i = 0; i < nonTerminals.size(); i++)
    {
        string nT = nonTerminals[i];
        leftFactorStep(nT, productions[nT]);
    }
}

bool GrammarParser::verifyCompleteGrammar()
{
    for (auto nonTerminal : usedNonTerminalsSet)
    {
        if (!derivedNonTerminalsSet.count(nonTerminal))
        {
            return false;
        }
    }
    return true;
}

void GrammarParser::formatProductions()
{
    for (auto p : productions)
    {
        string nT = p.first;
        vector<vector<string>> rules = p.second;
        vector<string> formattedRules;
        for (auto rule : rules)
        {
            string formattedRule;
            for (int i = 0; i < rule.size(); i++)
            {
                string token = rule[i];
                formattedRule += token;
                if (i < rule.size() - 1)
                {
                    formattedRule += " ";
                }
            }
            formattedRules.push_back(formattedRule);
        }
        formattedProds[nT] = formattedRules;
    }
}

void GrammarParser::parse(string inPath)
{
    ifstream inFile(inPath);
    string token;
    string currNonTerminal;
    vector<string> rule;
    bool expectRule = false;
    vector<vector<string>> rules;
    while (inFile >> token)
    {
        if (token == "#")
        { // new production
            assert(!expectRule);
            if (!rule.empty())
            {
                rules.push_back(rule);
                rule.clear();
            }
            if (!currNonTerminal.empty() && !rules.empty())
            {
                if (productions[currNonTerminal].empty())
                {
                    productions[currNonTerminal] = rules;
                }
                else
                {
                    productions[currNonTerminal].insert(productions[currNonTerminal].end(), rules.begin(), rules.end());
                }
            }
            inFile >> currNonTerminal;
            assert(!isTerminal(currNonTerminal));

            // first rule
            if (rules.empty())
                startSymbol = currNonTerminal;
            inFile >> token;
            assert(token == "=");
            rules.clear();
            expectRule = true;
            derivedNonTerminalsSet.insert(currNonTerminal);
            nonTerminals.push_back(currNonTerminal);
        }
        else if (token == "|")
        {
            assert(!currNonTerminal.empty());
            assert(!expectRule);
            assert(!rule.empty());
            rules.push_back(rule);
            rule.clear();
            expectRule = true;
        }
        else if (token == "\\L")
        {
            assert(!currNonTerminal.empty());
            rule.push_back("");
            expectRule = false;
        }
        else if (token[0] == '\\')
        {
            assert(!currNonTerminal.empty());
            assert(token.size() > 1);
            rule.push_back(token.substr(1));
            expectRule = false;
        }
        else
        {
            assert(!currNonTerminal.empty());
            expectRule = false;
            if (isTerminal(token))
            {
                token = token.substr(1, token.size() - 2);
                terminalsSet.insert(token);
                rule.push_back(token);
            }
            else
            {
                usedNonTerminalsSet.insert(token);
                rule.push_back(token);
            }
        }
    }

    assert(!expectRule);
    if (!rule.empty())
    {
        rules.push_back(rule);
        rule.clear();
    }
    if (!currNonTerminal.empty() && !rules.empty())
    {
        if (productions[currNonTerminal].empty())
        {
            productions[currNonTerminal] = rules;
        }
        else
        {
            productions[currNonTerminal].insert(productions[currNonTerminal].end(), rules.begin(), rules.end());
        }
    }
}

bool GrammarParser::isTerminal(string token)
{
    return token[0] == '\'' && token[token.size() - 1] == '\'';
}

vector<string> GrammarParser::getTerminals()
{
    return terminals;
}

vector<string> GrammarParser::getNonTerminals()
{
    return nonTerminals;
}

string GrammarParser::getStartSymbol()
{
    return startSymbol;
}

map<string, vector<string>> GrammarParser::getProductions()
{
    return formattedProds;
}
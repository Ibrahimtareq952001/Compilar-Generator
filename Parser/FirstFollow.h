#ifndef FirstFollow_H
#define FirstFollow_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>

using namespace std;

namespace Parser
{
    class FirstFollow
    {
    public:
        vector<std::string> terminals;
        vector<std::string> nonTerminals;
        map<string, vector<string>> first_complementary;
        map<std::string, std::vector<std::string>> CFG;
        map<std::string, std::vector<std::string>> first;
        map<std::string, std::vector<std::string>> follow;
        string Epsilon = "";

        bool isTerminal(string s);
        bool isNonTerminal(string s);
        map<string, vector<string>> computeFirstSets();
        map<string, vector<string>> computeFollowSets();
    };
}

#endif
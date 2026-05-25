#ifndef TOKENGENERATOR_H
#define TOKENGENERATOR_H

#include <iostream>
#include <utility>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>

namespace LexicalAnalyzer
{
    class tokenGenerator
    {
    private:
        std::map<int, std::string> priority; // priority number --> string (0 maps to keywords)
        std::map<std::pair<int, char>, int> Dfa;
        std::map<int, int> dfaFinalStates;
        std::string fileName;
        std::set<char> punctuations;
        std::vector<std::string> splitTokens;
        std::set<std::string> ids;
        std::vector<std::string> tokens;
        std::vector<std::string> values;
        std::vector<std::string> errors;
        std::vector<int> tp;

        void read();
        void matchAllSplitTokens();
        void match(std::string str, bool isError);

    public:
        tokenGenerator(const std::map<int, std::string> &prio,
                       const std::map<std::pair<int, char>, int> &dfa,
                       const std::map<int, int> &finalStates,
                       std::string filePath);

        std::set<std::string> getIds() const;

        // Getter for tokens
        std::vector<std::string> getTokens() const;

        std::vector<int> getTypes() const;

        // Getter for values
        std::vector<std::string> getValues() const;

        // Getter for errors
        std::vector<std::string> getErrors() const;

        std::string getNext();
    };
}

#endif // TOKENGENERATOR_H
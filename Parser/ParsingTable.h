#ifndef PARSINGTABLE_H_
#define PARSINGTABLE_H_

#include <iostream>
#include <vector>
#include <map>

using namespace std;

namespace Parser
{
    class ParsingTable {
    private:
        map<pair<string, string>, vector<string>> table;
        bool valid;
        map<string, vector<string>> fir;
        map<string, vector<string>> fol;

    public:
        ParsingTable(map<string, vector<string>> first, map<string, vector<string>> follow, map<string, vector<string>> comp);
        bool isValid();
        map<pair<string, string>, vector<string>> getTable();
        void printFirstFollow();
        void csvTable(vector<std::string> terminals , vector<std::string> nonterminals);
    };

}


#endif //PARSINGTABLE_H_
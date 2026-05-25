#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "ParsingTable.h"

using namespace std;

using namespace Parser;

ParsingTable::ParsingTable(map<string, vector<string>> first, map<string, vector<string>> follow, map<string, vector<string>> comp):
        fir(first),fol(follow)
{
    valid = true;
    bool sync ;
    for (const auto& pair : first) {
        sync=true;
        for (int i=0;i<pair.second.size();i++) {
            if (pair.second[i] != "") {
                table[make_pair(pair.first, pair.second[i])].push_back(comp[pair.first][i]) ;
            }
            else {
                sync= false;
                for(const auto& s : follow[pair.first]) {
                    if (!table[make_pair(pair.first, s)].empty()) {
                        valid = false; // not LL1
                        table.clear();
                        return;
                    }
                    else table[make_pair(pair.first, s)].push_back("");
                }
            }
        }
        if(sync){
            for(const auto& s : follow[pair.first]){
                if(table[make_pair(pair.first, s)].empty()){
                    table[make_pair(pair.first, s)].push_back("sync") ;
                }
            }
        }
    }

}
bool ParsingTable::isValid()
{
    return valid;
}

map<pair<string, string>, vector<string>> ParsingTable::getTable()
{
    return table;
}


void ParsingTable:: printFirstFollow(){
    ofstream outputFile(".\\output files\\FirstFollow.txt");
    for (const auto& pair : fir) {
        outputFile << "FIRST(" << pair.first << "): { ";
        for (const string& s : pair.second) {
            outputFile << s << " ";
        }
        outputFile << "}\n";
    }
    outputFile << "\n";

    for (const auto& pair : fol) {
        outputFile << "FOllOW(" << pair.first << "): { ";
        for (const string& s : pair.second) {
            outputFile << s << " ";
        }
        outputFile << "}\n";
    }
}

void ParsingTable:: csvTable(vector<std::string> terminals , vector<std::string> nonterminals){
    const std::string csvFileName = ".\\output files\\table.csv";
    std::ofstream csvFile1(csvFileName);
    csvFile1<<" ,";
    for (const auto& t : terminals)
        csvFile1 << t << ",";
    csvFile1<<"\n";
    for (const auto& nt : nonterminals) {
        csvFile1 << nt << ",";
        for (const auto& t : terminals){
            std::pair<std::string, std::string> searchKey(nt, t);
            if (table.find(searchKey) != table.end())
            {
                string ss = table[searchKey][0];
                if (ss == ""){
                    csvFile1<<nt<<" --> "<<"Epsilon ,";
                }
                else
                {
                    csvFile1<<nt<<" --> "<<ss<<",";

                }
            }
            else
            {
                csvFile1<<" ,";

            }
        }
        csvFile1<<"\n";

    }
}
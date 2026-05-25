#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <fstream>
#include "Lexical Analyzer/NFA.h"
#include "Lexical Analyzer/DFA.h"
#include "Lexical Analyzer/input handler.h"
#include "Lexical Analyzer/input_parser.h"
#include "Parser/FirstFollow.h"
#include "Parser/ParserOutput.h"
#include "Parser/ParsingTable.h"
#include "Parser/GrammarParser.h"

using namespace std;
using namespace LexicalAnalyzer;
using namespace Parser;

int main() {
    // Define file paths (cross-platform)
    std::string grammarPath = "input files/grammer.txt";
    std::string rulesPath = "input files/rules.txt";
    std::string testPath = "input files/test.txt";

    //------------------------------------- 1. input parsing -------------------------------------//
    cout << "=== Compilers Generator ===" << endl;
    cout << "Parsing input rules..." << endl;
    InputParser parser(rulesPath);
    parser.parse();
    cout << "Input parsing completed successfully!" << endl << endl;


    //------------------------------------------ 2. NFA ------------------------------------------//
    NFA nfa = parser.getCombinedNFA();
    map<int, string> priorityStrings = parser.getPriorityStrings();
    map<pair<int, char>, set<int>> nfaTransitions = nfa.getNfaTransitions();
    set<char> alphabet = nfa.getAlphabet();
    map<int, int> nfaFinalStates = nfa.getStatePriorities();


    //------------------------------------------ 3. DFA ------------------------------------------//
    DFA dfa = DFA(nfaTransitions,alphabet,nfaFinalStates);
    map<pair<int, char>, int> dfaTransitions = dfa.getDFA();
    map<int, int> dfaFinalStates = dfa.getDFAFinalStates();


    //------------------------------------------ 4. Generate tokens ------------------------------------//
    cout << "Generating tokens from lexical analysis..." << endl;
    tokenGenerator t = tokenGenerator(priorityStrings, dfaTransitions, dfaFinalStates, testPath);
    cout << "Token generation completed!" << endl << endl;


    //----------------------------------------- 5. LL(1) ------------------------------------------//
    cout << "Building LL(1) parser..." << endl;
    GrammarParser grammarParser(grammarPath);
    vector<std::string> terminals = grammarParser.getTerminals();
    terminals.push_back("$");
    vector<std::string> nonTerminals = grammarParser.getNonTerminals();
    map<std::string, std::vector<std::string>> CFG = grammarParser.getProductions();
    string start = grammarParser.getStartSymbol();
    cout << "Grammar parsed successfully!" << endl << endl;
    //-------------------------------------- 6. First&Follow --------------------------------------//
    FirstFollow grammar;
    grammar.terminals = terminals;
    grammar.nonTerminals = nonTerminals;
    grammar.CFG = CFG;

    map<string, vector<string>> first = grammar.computeFirstSets();
    map<string, vector<string>> follow = grammar.computeFollowSets();
    map<string, vector<string>> comp = grammar.first_complementary;

    //-------------------------------------- 7. create table --------------------------------------//
    // use first , follow , CFG , terminals and nonTerminals to outpt the next please
    map<pair<string,string>,vector<string>> table ;
    ParsingTable parsingTable(first,follow,comp);
    table=parsingTable.getTable();
    parsingTable.csvTable(grammar.terminals,grammar.nonTerminals);
    parsingTable.printFirstFollow();

    //------------------------------------- 8. stack handling -------------------------------------//
    bool val = parsingTable.isValid();
    vector<string> leftmostDerivation;
    if(val){
        ParserOutput po = ParserOutput(grammar.terminals,grammar.nonTerminals,start,t,table);
        leftmostDerivation = po.getOut();
        t=po.getT();

    }

    //----------------------------------------- 9. Output -----------------------------------------//
    cout << "Writing output files..." << endl;
    
    //9.1 DFA table :
    ofstream outputFile1("output files/DFA.txt");
    if (!outputFile1.is_open()) {
        std::cerr << "Error: Could not open output file for DFA" << std::endl;
        return 1;
    }
    for (const auto& ele : dfa.getDFAStates()) {
        for (const auto& ch : alphabet) {
            if(ch!='\0'){
                if(dfaTransitions.find(make_pair(ele.first,ch))!=dfaTransitions.end())
                    outputFile1 << "state  " << ele.first<< " --(" << ch << ")-->: state " << dfaTransitions[make_pair(ele.first,ch)] << endl;
                else
                    outputFile1 << "state  " << ele.first<< " --(" << ch << ")-->: Dead state"  << endl;
            }

        }
        outputFile1 << "--------------------------------------------------------------------------------------------" << endl;

    }
    outputFile1.close();

    //9.2 Tokens :
    set<string> ids = t.getIds() ;
    vector<string> tokens = t.getTokens();
    vector<string> values = t.getValues();
    vector<string> errors = t.getErrors();
    ofstream outputFile2("output files/Tokens.txt");
    if (!outputFile2.is_open()) {
        std::cerr << "Error: Could not open output file for Tokens" << std::endl;
        return 1;
    }
    int to =0 , e =0 , v=0;
    for (auto ct: t.getTypes()) {
        if(ct==0) {
            outputFile2 << "error" << endl;
        } else{
            outputFile2 << tokens[to] << endl;
            to++;
        }
    }
    outputFile2.close();
    
    //9.3 Values :
    ofstream outputFile3("output files/Values.txt");
    if (!outputFile3.is_open()) {
        std::cerr << "Error: Could not open output file for Values" << std::endl;
        return 1;
    }
    for (auto ct: t.getTypes()) {
        if(ct==0) {
            outputFile3 << errors[e] << endl;
            e++;
        } else{
            outputFile3 << values[v] << endl;
            v++;
        }
    }
    outputFile3.close();

    //9.4 leftmostDerivation :
    ofstream outputFile4("output files/leftmostDerivation.txt");
    if (!outputFile4.is_open()) {
        std::cerr << "Error: Could not open output file for Derivation" << std::endl;
        return 1;
    }
    if(val){
        for (auto s: leftmostDerivation)
            outputFile4<<s<<"\n";
    }
    else{
        outputFile4<<"Invalid Grammar!"<<endl;
        cout<<"Invalid Grammar!"<<endl;
    }
    outputFile4.close();
    
    cout << "All output files written successfully!" << endl;
    return 0;
}
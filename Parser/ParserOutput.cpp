#include <utility>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <stack>
#include "../Lexical Analyzer/input handler.h"
#include "ParserOutput.h"

using namespace std;
using namespace Parser;

// some globals
std::ofstream csvFile(".\\output files\\Stack.csv");
string matched ="";



ParserOutput::ParserOutput(vector<string> term, vector<string> nonterm, string sta, LexicalAnalyzer::tokenGenerator &to, const map<pair<string, string>, vector<string>> m)
        : terminals(term), nonTerminals(nonterm), start(sta), t(to), mp(m)
{
    st.push("$");
    st.push(start);
    getNextInput();
    csvFile<<"stack top ,input top , output \n";
    out.push_back(start);
    run();
}


void ParserOutput::getNextInput()
{
    inputs.push(t.getNext());
}

string ParserOutput::getStackValues(){
    string out = "";
    stack<string> cs;
    while (st.size()>1)
    {
        string curr = st.top();
        st.pop();
        out+=curr;
        out+=" ";
        cs.push(curr);
    }
    while (!cs.empty())
    {
        string curr = cs.top();
        cs.pop();
        st.push(curr);
    }
    return out;
}

vector<string> ParserOutput:: getOut(){
    return out;
}

void ParserOutput::run()
{
    string st_f = st.top();
    string in_f = inputs.top();
    auto it = find(terminals.begin(), terminals.end(), st_f);
    csvFile<<st_f<<","<<in_f<<",";
    if (it != terminals.end())
    {
        if (st_f == in_f && in_f == "$"){
            csvFile<<"accept";
            out.push_back("accept");
            return;
        }
        else if (st_f == in_f)
        {
            csvFile<<"match \" "<<st_f<<" \"";
            matched+=st_f;
            matched+=" ";
            st.pop();
            inputs.pop();
            getNextInput();
        }
        else
        {

            // report error
            csvFile<<"Error: missing "<<st_f<<" .. pop from stack ";
            out.push_back("--> Error: missing "+st_f+" .. Delete it ");
            st.pop();
            out.push_back(matched+getStackValues());


            // report error
            // csvFile<<"Error: missing "<<st_f<<" .. inserte it to input ";
            // out.push_back("--> Error: missing "+st_f+" .. inserte it to input ");
            // inputs.push(st_f);
        }
    }
    else
    {
        std::pair<std::string, std::string> searchKey(st_f, in_f);
        if (mp.find(searchKey) != mp.end())
        {
            string ss = mp[searchKey][0];

            if (ss == "sync")
            {
                // report error
                csvFile<<"Error: sync "<<st_f;
                out.push_back("--> Error: sync "+st_f);
                st.pop();
                out.push_back(matched+getStackValues());

            }
            else if (ss == "")
            {
                st.pop();
                csvFile<<st_f<<" --> Epsilon ";
                out.push_back(matched+getStackValues());

            }
            else
            {
                vector<string> v ;
                std::istringstream iss(ss);
                std::string word;
                while (iss >> word) {
                    v.push_back(word);
                }
                csvFile<<st_f<<" --> "<<ss;
                st.pop();
                for (int j = v.size() - 1; j >= 0; j--){
                    st.push(v[j]);
                }
                out.push_back(matched+getStackValues());
            }
        }
        else
        {
            csvFile<<"Error:(illegal "<<st_f<<" ) : discard " <<in_f;
            out.push_back("--> Error:(illegal "+st_f+" ) : discard " +in_f);
            inputs.pop();
            if(in_f!="$")
                getNextInput();
            else return;
        }
    }
    csvFile<<"\n";
    run();
}
LexicalAnalyzer::tokenGenerator ParserOutput::getT(){
    return t;
}
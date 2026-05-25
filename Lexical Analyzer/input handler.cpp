#include <utility>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <fstream>
#include <sstream>
#include <iterator>
#include "input handler.h"

using namespace std;
using namespace LexicalAnalyzer;


tokenGenerator::tokenGenerator(const std::map<int, std::string> &prio,
                               const std::map<std::pair<int, char>, int> &dfa,
                               const std::map<int, int> &finalStates,
                               string filePath)
        : priority(prio), Dfa(dfa), dfaFinalStates(finalStates), fileName(std::move(filePath))
{
    read();
    // put next in part 1 only
    // matchAllSplitTokens();
}

void tokenGenerator::read()
{
    string s, line;
    ifstream file(fileName);
    while (getline(file, line))
    {
        s += ' ' + line;
    }
    file.close();
    istringstream b(s);
    istream_iterator<std::string> beg(b), end;
    vector<std::string> splitTok(beg, end);
    splitTokens = splitTok;
}

void tokenGenerator::matchAllSplitTokens(){
    while(!splitTokens.empty()){
        match(splitTokens[0], false);
    }
}

string tokenGenerator::getNext()
{
    if (!splitTokens.empty())
    {
        match(splitTokens[0], false);
        string s = tokens[tokens.size() - 1];
        if(s=="assign") s="=";
        return s;
    }
    else
        return "$";
}

void tokenGenerator::match(string str, bool isError)
{
    int last = -1, lastPos = -1, error_idx = -1, curr = 0;
    if (dfaFinalStates.find(curr) != dfaFinalStates.end())
    {
        last = curr;
        lastPos = 0;
    }
    // get last accepted state and error is found.
    for (int i = 0; i < str.size(); i++)
    {
        if (!(Dfa.find(make_pair(curr, str[i])) != Dfa.end()))
        {
            error_idx = i;
            break;
        }
        curr = Dfa[make_pair(curr, str[i])];

        if (dfaFinalStates.find(curr) != dfaFinalStates.end())
        {
            last = curr;
            lastPos = i;
        }
    }

    // case 1 : str is a one token
    if (lastPos == str.size() - 1)
    {
        string type = priority[dfaFinalStates[curr]];
        tokens.push_back(type);
        values.push_back(str);
        if (type == "id")
        {
            ids.insert(str);
        }
        tp.push_back(1);
        splitTokens.erase(splitTokens.begin());
        return;
    }

        // case 2 : str is more than one token or error
    else
    {
        if (lastPos != -1)
        {
            string type = priority[dfaFinalStates[last]];
            tokens.push_back(type);
            values.push_back(str.substr(0, lastPos + 1));
            if (type == "id")
            {
                ids.insert(str.substr(0, lastPos + 1));
            }
            tp.push_back(1);
        }
        // case 2.1 : error
        if (lastPos == -1)
        {
            char e = str[0];
            if (isError)
                errors[errors.size() - 1] += e;
            else
            {
                errors.push_back({e});
                tp.push_back(0);
            }

            if (lastPos + 2 < str.size())
            {
                string reminder = str.substr(lastPos + 2);
                match(reminder, true);
                return;
            }
            else
            {
                return;
            }
        }
            // case 2.2 str is more than one token
        else
        {
            if (lastPos + 1 < str.size())
            {
                splitTokens[0] = str.substr(lastPos + 1);
            }
            else
            {
                splitTokens.erase(splitTokens.begin());
            }
        }
    }
}




set<string> tokenGenerator::getIds() const
{
    return ids;
}

// Getter for tokens
vector<string> tokenGenerator::getTokens() const
{
    return tokens;
}

// Getter for values
vector<string> tokenGenerator::getValues() const
{
    return values;
}

// Getter for errors
vector<string> tokenGenerator::getErrors() const
{
    return errors;
}
vector<int> tokenGenerator::getTypes() const
{
    return tp;
}
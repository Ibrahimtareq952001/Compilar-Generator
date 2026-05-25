#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <set>
#include <map>
#include <assert.h>

#include "input_parser.h"
#include "NFA.h"

using namespace std;
using namespace LexicalAnalyzer;
InputParser::InputParser(string inputPath)
{
    inPath = inputPath;
}

void InputParser::preprocessKeywords(string line)
{
    int n = line.size();
    string keyword;
    for (int i = 1; i < n - 1; i++)
    {
        if (keyword.empty() && line[i] == ' ')
            continue;
        else if (line[i] != ' ')
        {
            keyword += line[i];
        }
        else
        {
            num_keywords++;
            keyword = "";
        }
    }
    if (!keyword.empty())
    {
        num_keywords++;
    }
}

void InputParser::preprocessPunctuations(string line)
{
    int n = line.size();
    for (int i = 1; i < n - 1; i++)
    {
        if (line[i] == '\\')
            continue;
        num_punc++;
    }
}

void InputParser::preprocess()
{
    string line;
    ifstream inFile(inPath);
    int lineNum = 0;
    while (getline(inFile, line))
    {
        lineNum++;
        line = trim(line);
        int n = line.size();
        if (line.empty())
            continue;
        if (line[0] == '{' && line[n - 1] == '}')
        { // keywords
            preprocessKeywords(line);
        }
        else if (line[0] == '[' && line[n - 1] == ']')
        { // punctuations
            line = removeSpaces(line);
            preprocessPunctuations(line);
        }
        else
        {
            line = removeSpaces(line);
            int equalIndex = line.find('=');
            int colonIndex = line.find(':');
            // regular definition
            if ((equalIndex != -1 && colonIndex != -1 && equalIndex < colonIndex) || (equalIndex != -1 && colonIndex == -1))
            {
                string name = line.substr(0, equalIndex);
                string regDef = line.substr(equalIndex + 1);
                regDefs[name] = regDef;
            }
                // regular expression
            else if ((equalIndex != -1 && colonIndex != -1 && colonIndex < equalIndex) || (colonIndex != -1 && equalIndex == -1))
            {
                continue;
            }
                // neither a regular definition nor a regular expression
            else
            {
                throw runtime_error(
                        "line number " + to_string(lineNum) + " is invalid");
            }
        }
    }
}

string InputParser::trim(string inStr)
{
    int n = inStr.size();
    string outStr;
    int i = 0;
    while (inStr[i] == ' ' && i < n)
        i++;
    int j = n - 1;
    while (inStr[j] == ' ' && j > i)
        j--;
    return inStr.substr(i, j - i + 1);
}

string InputParser::removeSpaces(string inStr)
{
    string outStr = "";
    for (auto ch : inStr)
    {
        if (ch == ' ')
            continue;
        outStr += ch;
    }
    return outStr;
}

vector<string> InputParser::tokenize(string regDef)
{
    int regDefLen = regDef.size();
    vector<string> tokens;
    string currToken = "", validToken = "";
    int currIdx = 0, validIdx = -1;
    while (validIdx < regDefLen - 1)
    {
        if (currIdx == regDefLen)
        {
            tokens.push_back(validToken);
            validToken = "";
            currToken = "";
            currIdx = validIdx + 1;
        }
        string ch = string(1, regDef[currIdx]);
        if (operators.count(ch) && validToken.empty())
        {
            validIdx = currIdx;
            currIdx++;
            tokens.push_back(ch);
        }
        else if (ch == "\\" && validToken.empty())
        {
            tokens.push_back(regDef.substr(currIdx, 2));
            validIdx = currIdx + 1;
            currIdx += 2;
        }
        else if (operators.count(ch) || ch == "\\")
        {
            tokens.push_back(validToken);
            validToken = "";
            currToken = "";
            currIdx = validIdx + 1;
        }
        else
        {
            currToken += ch;
            currIdx++;
            if (!regDefs[currToken].empty() || currToken.size() == 1)
            {
                validToken = currToken;
                validIdx = currIdx - 1;
            }
        }
    }
    if (!validToken.empty())
    {
        tokens.push_back(validToken);
    }
    return tokens;
}

vector<string> InputParser::transformToCanonicalReg(vector<string> tokens)
{
    vector<string> canonicalTokens;
    int tokensLen = tokens.size();
    for (int i = 0; i < tokensLen; i++)
    {
        if (tokens[i] == "-")
        {
            if (i == 0 || i == tokensLen - 1)
            {
                throw runtime_error("Invalid Position for -");
            }

            // figure out whether operands are regular expressions or regular definitions.
            // operands of - must be symbols
            string start = tokens[i - 1], end = tokens[i + 1];
            if (regDefs[start].size() == 1)
                start = regDefs[start];
            else if (start.size() != 1)
                throw runtime_error("invalid first operand for -");
            if (regDefs[end].size() == 1)
                end = regDefs[end];
            else if (end.size() != 1)
            {
                throw runtime_error("invalid second operand for -");
            }

            if (start[0] > end[0])
                throw runtime_error("Invalid Range for -");
            else if (start[0] == end[0])
                i++; // skip end token
            else
            {
                canonicalTokens.push_back("|");
                for (char ch = start[0] + 1; ch < end[0]; ch++)
                {
                    canonicalTokens.push_back(string(1, ch));
                    canonicalTokens.push_back("|");
                }
            }
        }
        else
            canonicalTokens.push_back(tokens[i]);
    }
    return canonicalTokens;
}

vector<string> InputParser::addConcatSymbol(vector<string> tokens)
{
    int n = tokens.size();
    vector<string> modifiedTokens;
    modifiedTokens.push_back(tokens[0]);
    for (int i = 1; i < n; i++)
    {
        if ((!operators.count(tokens[i]) && !operators.count(tokens[i - 1])) || (!operators.count(tokens[i]) && tokens[i - 1] == ")") || (tokens[i] == "(" && !operators.count(tokens[i - 1])) || (!operators.count(tokens[i]) && tokens[i - 1] == "*") || (!operators.count(tokens[i]) && tokens[i - 1] == "+") || (tokens[i] == "(" && tokens[i - 1] == ")") || (tokens[i] == "(" && tokens[i - 1] == "+") || (tokens[i] == "(" && tokens[i - 1] == "*"))
        {
            modifiedTokens.push_back(" ");
        }
        modifiedTokens.push_back(tokens[i]);
    }
    // for (auto token : modifiedTokens)
    // 	cout << (token == " " ? "c" : token) << endl;
    return modifiedTokens;
}

void InputParser::nfaStep(stack<string> &operatorsStack, stack<NFA> &operandsStack)
{
    string op = operatorsStack.top();
    operatorsStack.pop();
    NFA finalNfa;
    if (op == "+")
    {
        NFA operand = operandsStack.top();
        operandsStack.pop();
        finalNfa = NFA::positiveClosure(operand);
    }
    else if (op == "*")
    {
        NFA operand = operandsStack.top();
        operandsStack.pop();
        finalNfa = NFA::kleeneClosure(operand);
    }
    else if (op == " ")
    {
        NFA operand2 = operandsStack.top();
        operandsStack.pop();
        NFA operand1 = operandsStack.top();
        operandsStack.pop();
        finalNfa = NFA::concatenate(operand1, operand2);
    }
    else if (op == "|")
    {
        NFA operand2 = operandsStack.top();
        operandsStack.pop();
        NFA operand1 = operandsStack.top();
        operandsStack.pop();
        finalNfa = NFA::Or(operand1, operand2);
    }
    else
        throw runtime_error("Unexpected Operator: " + op);
    operandsStack.push(finalNfa);
}

NFA InputParser::buildNFA(vector<string> tokens)
{
    stack<string> operatorsStack;
    stack<NFA> operandsStack;

    for (auto token : tokens)
    {
        if (operators.count(token))
        {
            if (token == ")")
            {
                while (operatorsStack.top() != "(")
                {
                    nfaStep(operatorsStack, operandsStack);
                }
                operatorsStack.pop();
            }
            else
            {
                while (!operatorsStack.empty() && precedence[token] <= precedence[operatorsStack.top()] && operatorsStack.top() != "(")
                {
                    nfaStep(operatorsStack, operandsStack);
                }
                operatorsStack.push(token);
            }
        }
        else if (!regDefs[token].empty())
        { // regular definition
            operandsStack.push(basicExps[token]);
        }
        else
        { // alphabet symbol
            assert(token.size() == 1 || (token.size() == 2 && token[0] == '\\'));
            NFA nfa;
            if (token.size() == 1)
                nfa.addSympol(token[0]);
            else if (token == "\\L")
                nfa.addSympol('\0');
            else if (token[0] == '\\')
                nfa.addSympol(token[1]);
            operandsStack.push(nfa);
        }
    }
    while (!operatorsStack.empty())
    {
        nfaStep(operatorsStack, operandsStack);
    }
    return operandsStack.top();
}

void InputParser::handleRegDef(string line, int equalIdx)
{
    string name = line.substr(0, equalIdx);
    string regDef = line.substr(equalIdx + 1);

    vector<string> tokens = addConcatSymbol(transformToCanonicalReg(tokenize(regDef)));
    NFA nfa = buildNFA(tokens);
    basicExps[name] = nfa;
}

void InputParser::handleRegEx(string line, int colonIdx)
{
    string name = line.substr(0, colonIdx);
    string regex = line.substr(colonIdx + 1);
    vector<string> tokens = addConcatSymbol(transformToCanonicalReg(tokenize(regex)));
    NFA nfa = buildNFA(tokens);
    int p = num_keywords + num_punc + i_reg;
    nfa.setPriority(p);
    nfas.push_back(nfa);
    priorityStrings[p] = name;
    i_reg++;
}

void InputParser::handleKeywords(string line)
{
    int n = line.size();
    string keyword;
    NFA nfa;
    for (int i = 1; i < n - 1; i++)
    {
        if (keyword.empty() && line[i] == ' ')
            continue;
        else if (keyword.empty())
        {
            keyword += line[i];
            nfa.addSympol(line[i]);
        }
        else if (line[i] == ' ')
        {
            nfa.setPriority(i_keyword);
            nfas.push_back(nfa);
            priorityStrings[i_keyword] = keyword;
            i_keyword++;
            NFA newNfa;
            nfa = newNfa;
            keyword = "";
        }
        else
        {
            keyword += line[i];
            NFA currSymbolNfa;
            currSymbolNfa.addSympol(line[i]);
            nfa = NFA::concatenate(nfa, currSymbolNfa);
        }
    }
    if (!keyword.empty())
    {
        nfa.setPriority(i_keyword);
        nfas.push_back(nfa);
        priorityStrings[i_keyword] = keyword;
        i_keyword++;
    }
}

void InputParser::handlePunctuations(string line)
{
    int n = line.size();
    for (int i = 1; i < n - 1; i++)
    {
        if (line[i] == '\\')
            continue;
        NFA nfa;
        nfa.addSympol(line[i]);
        int p = num_keywords + i_punc;
        nfa.setPriority(p);
        priorityStrings[p] = line[i];
        nfas.push_back(nfa);
        punctuations.insert(line[i]);
        i_punc++;
    }
}
void InputParser::parse()
{
    preprocess();
    string line;
    ifstream inFile(inPath);
    int lineNum = 0;
    while (getline(inFile, line))
    {
        lineNum++;
        line = trim(line);
        int n = line.size();
        if (line.empty())
            continue;
        if (line[0] == '{' && line[n - 1] == '}')
        { // keywords
            handleKeywords(line);
        }
        else if (line[0] == '[' && line[n - 1] == ']')
        { // punctuations
            line = removeSpaces(line);
            handlePunctuations(line);
        }
        else
        {
            line = removeSpaces(line);
            int equalIndex = line.find('=');
            int colonIndex = line.find(':');
            // regular definition
            if ((equalIndex != -1 && colonIndex != -1 && equalIndex < colonIndex) || (equalIndex != -1 && colonIndex == -1))
            {
                handleRegDef(line, equalIndex);
            }
                // regular expression
            else if ((equalIndex != -1 && colonIndex != -1 && colonIndex < equalIndex) || (colonIndex != -1 && equalIndex == -1))
            {
                handleRegEx(line, colonIndex);
            }
                // neither a regular definition nor a regular expression
            else
            {
                throw runtime_error(
                        "line number " + to_string(lineNum) + " is invalid");
            }
        }
    }
    combinedNFA = NFA::combine(nfas);
}

NFA InputParser::getCombinedNFA()
{
    return combinedNFA;
}

map<int, string> InputParser::getPriorityStrings()
{
    return priorityStrings;
}

set<char> InputParser::getPunctuations()
{
    return punctuations;
}
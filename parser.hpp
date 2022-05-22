#include <fstream>
#include <iostream>
#include <unordered_map>
#include "tokenizer.hpp"

#ifndef PARSER
#define PARSER
using namespace std;
class Parser
{
private:
    Tokenizer tk;
    unordered_map<string, string> symTable;
    string curLine;
    int parenthesisCount;
    string currentID;
    int negativeCount;
    bool positiveFlag;

public:
    Parser() : tk(" "), symTable(), parenthesisCount(0), currentID(""), negativeCount(0), positiveFlag(false)
    {
    }
    void parseProgram(string filename)
    {
        ifstream input(filename);
        if (input.is_open())
        {
            while (getline(input, curLine))
            {
                tk.setCurLine(curLine);
                try
                {
                    matchAssignment();
                    cout << currentID << " = " << symTable[currentID] << endl;
                }
                catch (string err)
                {
                    cerr << "Invalid statement" << '\n';
                    cerr << err << endl;
                    break;
                }
            }
        }
        else
        {
            cout << "file not found" << endl;
        }
    }

    bool matchAssignment()
    {
        // call to match functions will either succeeed or throw an error to terminate the program
        matchIdentifier();
        matchEq();
        string expressionResult = matchExp();
        symTable[currentID] = expressionResult;
        return matchSemi();
    }
    bool matchIdentifier()
    {
        string id = tk.tokenizeID();
        if (id == "-1")
        {
            throw string("Identifier is Invalid");
        }
        currentID = id;
        return true;
    }
    bool matchEq()
    {
        string sym = tk.tokenizeSymbol();
        if (sym != "=")
        {
            throw string("Expected =");
        }
        return true;
    }

    string matchExp()
    {
        string token;
        // First call in grammer Exp: Term Exp`
        int res = stoi(matchTerm());
        // loop as long as Expression operators are found
        while (tk.lookAhead() == 'E')
        {
            string op = tk.tokenizeSymbol();
            // Call to Exp`: (+ | -) Term Exp`
            int res2 = stoi(matchTerm());
            if (op == "-")
            {
                res -= res2;
            }
            else
            {
                res += res2;
            }
        }
        return to_string(res);
    }

    string matchTerm()
    {
        string token;
        // call to Term: Fact Term`
        int res = stoi(matchFact());
        // loop as long as term operator is found
        while (tk.lookAhead() == 'T')
        {
            tk.tokenizeSymbol();
            // call to Term`: * Fact Term`
            int res2 = stoi(matchFact());
            res *= res2;
        }
        return to_string(res);
    }

    string matchFact()
    {
        string id;
        string res;
        string unarySymb;
        bool prevPositiveFlag;
        switch (tk.lookAhead())
        {
        case '(':
            tk.tokenizeSymbol();
            ++parenthesisCount;
            // consume unary symbols before proceeding with expression in parenthesis.
            // dont check for positive flag since negative count should be 0 unless negative operators exist
            // before the +, e.g. -+-3
            unarySymb = (negativeCount % 2 == 0) ? "" : "-";
            negativeCount = 0;
            // store previous flag status so that flag status can be reset for expression within parenthesis
            // this allows evaluation of inner expression while keeping any + operators that are set outside the parenthesis
            prevPositiveFlag = positiveFlag;
            positiveFlag = false;
            res = matchExp();
            // convert result to positive if a + operator occured outside the parenthesis
            if (prevPositiveFlag && res.find("-") != string::npos)
            {
                res = res.substr(res.find("-") + 1);
            }
            res = unarySymb + res;
            return matchFact() == ")" ? res : throw string("Unexpected symbol: )");
        case 'E':
            unarySymb = tk.tokenizeSymbol();
            // check if a + symbol has been found, set as true or previous value
            // avoids reseting + when chained with -, e.g. +---x
            positiveFlag = unarySymb == "+" ? true : positiveFlag;
            // increment negative count only when the number has not been set to a positive
            (unarySymb == "-") && !positiveFlag ? ++negativeCount : 0;
            return matchFact();
        case 'L':
            res = tk.tokenizeLiteral();
            // consume unary operations
            // dont check for positive flag since negative count should be 0 unless negative operators exist
            // before the +, e.g. -+-3
            res = (negativeCount % 2 == 0) ? res : "-" + res;
            negativeCount = 0;
            positiveFlag = false;
            return res;
        case 'I':
            id = tk.tokenizeID();
            res = symTable.find(id) != symTable.end() ? symTable[id] : throw string("Identifer " + id + " is not defined");
            // consume unary operations
            // dont check for positive flag since negative count should be 0 unless negative operators exist
            // before the +, e.g. -+-3
            // convert result to positive if the stored value is a negative and a + is used before it is retrieved
            if (positiveFlag && res.find("-") != string::npos)
            {
                res = res.substr(res.find("-") + 1);
            }
            res = (negativeCount % 2 == 0) ? res : "-" + res;
            negativeCount = 0;
            positiveFlag = false;
            return res;
        case ')':
            tk.tokenizeSymbol();
            return parenthesisCount >= 0 ? ")" : throw string("Unexpected symbol: )");
        default:
            throw string("Unexpect symbol");
        }
        return "-2";
    }

    bool matchSemi()
    {
        string sym = tk.tokenizeSymbol();
        if (sym != ";")
        {
            throw string("Missing semicolon at end of statement");
        }
        return true;
    }
};

#endif
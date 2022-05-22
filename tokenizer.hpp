#include <iostream>
#include <string>
#include <vector>

using namespace std;
#ifndef TOKENIZER
#define TOKENIZER
class Tokenizer
{
private:
    string curToken;
    vector<string> tokens;
    string curLine;
    int linePos;
    // helper function to construct literal tokens

public:
    Tokenizer(const string &curLine) : curToken(""), linePos(0), curLine(curLine)
    {
    }

    void setCurLine(const string &newLine)
    {
        curLine = newLine;
        linePos = 0;
    }
    string tokenizeLiteral()
    {
        ignoreWhiteSpace();
        bool isZero = false;
        if (linePos < curLine.length() && isdigit(curLine[linePos]))
        {
            curToken += curLine[linePos];
            isZero = curLine[linePos] == '0';
            ++linePos;
        }
        while (linePos < curLine.length() && isdigit(curLine[linePos]))
        {
            if (isZero)
            {
                throw string("Invalid literal: numeric values cannot have leading zeros");
            }
            curToken += curLine[linePos];
            ++linePos;
        }
        // --linePos;
        string temp = curToken;
        curToken = "";
        return temp;
    }
    // helper function to construct ID tokens
    string tokenizeID()
    {
        ignoreWhiteSpace();
        if (linePos < curLine.length() && isLetter(curLine[linePos]))
        {
            curToken += curLine[linePos];
            ++linePos;
            while (linePos < curLine.length() && isIdChar(curLine[linePos]))
            {
                curToken += curLine[linePos];
                ++linePos;
            }
            // --linePos;
            string temp = curToken;
            curToken = "";
            /*check if generated token is a valid token
             */
            return temp;
        }
        return "-1";
    }

    string tokenizeSymbol()
    {
        while (linePos < curLine.length())
        {
            switch (curLine[linePos])
            {
            case '+':
                ++linePos;
                return "+";
            case '=':
                ++linePos;
                return "=";
            case '-':
                ++linePos;
                return "-";
            case '*':
                ++linePos;
                return "*";
            case '(':
                ++linePos;
                return "(";
            case ')':
                ++linePos;
                return ")";
            case ';':
                ++linePos;
                return ";";
            // ignore white space or new line chars
            case ' ':
            case '\n':
                ++linePos;
                break;
            default:
                cout << "Unrecognized token " << curLine[linePos] << '\n';
                return "-1";
            }
        }
        // error since we reach End of Line before finding a symbol;
        return "-1";
    }

    /*
     * return value meanings
     * I -> Identifier
     * L -> Literal
     * E -> Exp symbol
     * ; -> semicolon
     */
    char lookAhead()
    {
        ignoreWhiteSpace();
        char next = curLine[linePos];
        if (isLetter(next))
        {
            return 'I';
        }
        if (isdigit(next) != 0)
        {
            return 'L';
        }
        if (next == '+' || next == '-')
        {
            return 'E';
        }
        if (next == ';')
        {
            return next;
        }
        if (next == '*')
        {
            return 'T';
        }
        if (next == '(')
        {
            return '(';
        }
        if (next == ')')
        {
            return ')';
        }
        return '1';
    }

    void ignoreWhiteSpace()
    {
        while (linePos < curLine.length() && (curLine[linePos] == '\t' || curLine[linePos] == ' ' || curLine[linePos] == '\n'))
        {
            ++linePos;
        }
    }
    bool isNonZero(char c)
    {
        return c >= '1' && c <= '9';
    }
    bool isIdChar(char c)
    {
        return isdigit(c) != 0 || isalpha(c) != 0 || c == '_';
    }
    bool isLetter(char c)
    {
        return isalpha(c) != 0 || c == '_';
    }

    // basic tokenizer
    void parseToken(const string &curLine)
    {
        for (int i = 0; i < curLine.length(); ++i)
        {
            char curChar = curLine[i];
            // Tokenize literal
            if (isdigit(curChar) != 0)
            {
                switch (curChar)
                {
                case '0':
                    tokens.push_back("0");
                    break;
                default:
                    curToken += curChar;
                    // tokenizeLiteral(curLine, i);
                    tokens.push_back(curToken);
                    curToken = "";
                    break;
                }
            }
            // Tokenize ID
            else if (isalpha(curChar) != 0 || curChar == '_')
            {
                curToken += curChar;
                // tokenizeID(curLine, i);
                tokens.push_back(curToken);
                curToken = "";
            }
            // Tokenize special chars
            else
            {
                switch (curChar)
                {
                case '+':
                    tokens.push_back("+");
                    break;
                case '=':
                    tokens.push_back("=");
                    break;
                case '-':
                    tokens.push_back("=");
                    break;
                case '*':
                    tokens.push_back("*");
                    break;
                case '(':
                    tokens.push_back("(");
                    break;
                case ')':
                    tokens.push_back(")");
                    break;
                case ';':
                    tokens.push_back(";");
                    break;
                // ignore white space or new line chars
                case ' ':
                case '\n':
                    break;
                default:
                    cout << "Unrecognized token " << curChar << '\n';
                }
            }
        }
    }

    void printTokens()
    {
        for (auto x : tokens)
        {
            cout << x << endl;
        }
    }

    const vector<string> &getTokens()
    {
        return tokens;
    }
};

#endif
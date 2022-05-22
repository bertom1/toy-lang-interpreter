#include <iostream>
#include <fstream>
#include <string>
#include "parser.hpp"

using namespace std;
int main(int argc, char *argv[])
{
    // check arguments passed, attempt to open files only if they are given as CL arguments
    if (argc > 1)
    {
        Parser p{};
        for (int i = 1; i < argc; ++i)
        {
            p.parseProgram(argv[i]);
        }
    }
    else
    {
        cout << "No files" << endl;
    }
}
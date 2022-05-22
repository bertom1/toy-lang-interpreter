# Toy lang interpreter
A simple interpreter written in C++.
To run the interpreter:  
1. Clone the repository to your local machine
2. Create the binary file for main.cpp using a C++ compiler. An example using g++ is ``g++ -o interpreter main.cpp``
3. Execute the binary file and pass the files you wish to interpret as Command Line Arguments, e.g. ``./interpreter input1.txt``

The interpreter runs line by line and prints the result after successfully interpretting each line.  
If a syntax error is found in the line being read, an error is thrown and the current file is terminated, but any subsequent files will continue to be interpretted.



## Language Grammar
Assignment:
    Identifier = Exp;

Exp:
    Exp + Term | Exp - Term | Term  
    Term Exp\`  
Exp\`:
    + Term Exp\` | - Term Exp\` | empty

Term:
    Term * Fact  | Fact  
    Fact Term\`  
Term\`:
    * Fact Term\` | Empty

Fact:
    ( Exp ) | - Fact | + Fact | Literal | Identifier

Identifier:
    Letter [Letter | Digit]*

Letter:
    a|...|z|A|...|Z|_

Literal:
    0 | NonZeroDigit Digit*

NonZeroDigit:
    1|...|9

Digit:
    0|1|...|9

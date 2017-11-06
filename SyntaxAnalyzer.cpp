/******************************************************************************
* Syntax Analyzer
* David Poss, Douglas Galm
* CPSC 323
* Professor Choi
*
* Notes: see README for information on compiling and running. Should work on
* both linux and windows systems.
*
******************************************************************************/

#include "Token.h"
#include "Helpers.h"
#include "Lexer.h"
#include "Syntax.h"
#include <iostream>

int main() {
	std::string input = "sample.txt";




	bool keepGoing = true;
	while (keepGoing) {
		std::cout << "Enter a file to convert or hit Ctrl+c to quit\nFile: ";
		//std::cin >> input;

		Lexer lexer;
		if (lexer.getTokens(input)) {
			functionA(lexer.getLexemes(), true);
			lexer.printLexemes(converToOutputName(input));
		}
		std::cin.clear();
		keepGoing = false;
	}
	return 0;
}
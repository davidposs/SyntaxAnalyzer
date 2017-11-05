#include "Lexer.h"
#include "Syntax.h"
#include "Pair.h"
#include "Helpers.h"
#include <iostream>

int line_count = 0;


void retError(std::list<Pair>& lexemes) {
	std::cout << "Error on line....\n";
}

/* Gets token at the top of the list */
std::string getCurrentToken(std::list<Pair>& lexemes) {
	std::string token = lexemes.front().getToken();
	while (token == "\n" || token == "\t") {
		if (token == "\n") { line_count++; }
		lexemes.pop_front();
		token = lexemes.front().getToken();
	}
	token = lexemes.front().getToken();
	lexemes.pop_front();
	return token;
}

/* Gets the token+type pair at the top of the list */
Pair getPair(std::list<Pair>& lexemes) {
	std::string token = lexemes.front().getToken();
	while (token == "\n" || token == "\t") {
		lexemes.pop_front();
		token = lexemes.front().getToken();
	}
	token = lexemes.front().getToken();
	std::string type = lexemes.front().getType();
	lexemes.pop_front();
	return Pair(token, type);
}

bool functionA(std::list<Pair> lexemes, bool printSwitch) {
	std::cout << "A\n";
	std::string token;
	if (printSwitch) { 
		std::cout << "A-> B %% CD" << std::endl; 
	}

	if (functionB(lexemes, printSwitch)) {
		token = getCurrentToken(lexemes);
		std::cout << "In a/b " << token << std::endl;
		if (token == "%") {
			token = getCurrentToken(lexemes);
			if (token == "%") {
				if (!functionC(lexemes, printSwitch)) { retError(lexemes); return false; }
				if (!functionD(lexemes, printSwitch)) { retError(lexemes); return false; }
			}
			else { retError(lexemes); return false; }
		}
		else { retError(lexemes); return false; }
	}
	else { retError(lexemes); return false; }
	return true;
}
bool functionB(std::list<Pair>& lexemes, bool printSwitch) {
	std::cout << "B\n";
	if (functionE(lexemes, printSwitch)) {
		return true;
	}
	else if (functionEprime(printSwitch)) {
		return true;
	}
	return false;
}

bool functionE(std::list<Pair>& lexemes, bool printSwitch) {
	std::cout << "E\n";
	if (functionF(lexemes, printSwitch)) {
		if (functionE(lexemes, printSwitch)) {
			return true;
		}
		return true;
	}
	else {
		return false;
	}
}

bool functionF(std::list<Pair>& lexemes, bool printSwitch) {
	std::string token = getCurrentToken(lexemes);
	std::cout << "F - " << token << "\n";
	if (token == "@") {
		// see if next token is an identifier
		if (functionG(getPair(lexemes), printSwitch)) {
			//lexemes.pop_front();
			token = getCurrentToken(lexemes);
			std::cout << "In F - token is: " << token << std::endl;
			std::cout << "checking (" << std::endl;
			if (token == "(") {
				if (functionH(lexemes, printSwitch)) {
					token = lexemes.front().getToken();
					if (token == ")") {
						if (functionC(lexemes, printSwitch)) {
							return functionI(lexemes, printSwitch);
						}
					}
				}
			}
		}
	}
	// F did not follow @ G (H) C I pattern
	return false;
}

bool functionH(std::list<Pair>& lexemes, bool printSwitch) {
	std::cout << "H\n";
	if (functionJ(lexemes, printSwitch)) { return true;	}
	/* chanhged to return functionEprime from just calling it*/
	else { return functionEprime(printSwitch);	}
}

bool functionJ(std::list<Pair>& lexemes, bool printSwitch) {
	std::cout << "J\n";
	if (functionK(lexemes, printSwitch)) {
		std::string token = getCurrentToken(lexemes);
		if (token == ",") {
			if (functionJ(lexemes, printSwitch)) { return true;	}
		}
		else { return true; } /* token is not ","*/
	}
	return false;
}

bool functionK(std::list<Pair>& lexemes, bool printSwitch) {
	std::cout << "K\n";
	if (functionL(lexemes, printSwitch)) {
		//lexemes.pop_front();
		std::string token = getCurrentToken(lexemes);
		std::cout << " in k: " << token << std::endl;
		if (token == ":") {
			//if (functionM(lexemes, printSwitch)) { return true; }
			return functionM(lexemes, printSwitch);
		}
		else { return false; }
		return true;	
	}
	else {
		retError(lexemes);
		return false;
	}
}

bool functionM(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "M\n";
	std::string token = getCurrentToken(lexemes);

	return (token == "integer") || (token == "boolean") || (token == "floating");
}

bool functionI(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "I\n";
	std::string token = getCurrentToken(lexemes);
	if (token == "{") {
		if (!functionD(lexemes, printSwitch)) {	return false; }
		token = lexemes.front().getToken();
		return token == "}";
	}
	return false;
}

bool functionC(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "C\n";
	if (functionN(lexemes, printSwitch)) {
		return true;
	}
	else {
		return functionEprime(printSwitch);
	}
}

bool functionN(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "N\n";
	if (functionO(lexemes, printSwitch)) {
		std::string token = getCurrentToken(lexemes);
		if (token == ";") {
			if (functionN(lexemes, printSwitch)) { return true;	}
			else { return true;	}
		}
		else {
			retError(lexemes);
			return false;
		}
	}
	return true;
}

bool functionO(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "M\n";
	if (functionM(lexemes, printSwitch)) {
		return functionL(lexemes, printSwitch);
	}
	else {
		retError(lexemes);
		return false;
	}
}

bool functionL(std::list<Pair>& lexemes, bool printSwitch) {
	std::cout << "L\n";

	std::string token = getCurrentToken(lexemes);
	std::cout << "in l before g: " << token << std::endl;
	if (functionG(getPair(lexemes), printSwitch)) {
		//lexemes.pop_front(); // remove the token g read
		token = getCurrentToken(lexemes);
		std::cout << "Looking for , in L " << token << std::endl;
		if (token == ",") {
			std::cout << "looking for , " << std::endl;
			if (functionL(lexemes, printSwitch)) { return true; }
			else { return false; }
		}
		else { /* token is not "," */ return true; }
	}
	else { 
		std::cout << "L is bad with token: " << token << std::endl;
		return false; 
	}
	return true;
}

bool functionD(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "D\n";
	if (functionP(lexemes, printSwitch)) {
		if (functionD(lexemes, printSwitch)) {
			return true;
		}
		return true;
	}
	else {
		return false;
	}
}

bool functionP(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "P\n";
	if (functionQ(lexemes, printSwitch)) { return true; }
	if (functionR(lexemes, printSwitch)) { return true; }
	if (functionS(lexemes, printSwitch)) { return true; }
	if (functionT(lexemes, printSwitch)) { return true; }
	if (functionU(lexemes, printSwitch)) { return true; }
	if (functionV(lexemes, printSwitch)) { return true; }
	if (functionW(lexemes, printSwitch)) { return true; }
	return false;
}

bool functionQ(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "Q\n";
	std::string token = getCurrentToken(lexemes);

	if (token == "{") {
		if (!functionD(lexemes, printSwitch)) {
			return false;
		}
		token = lexemes.front().getToken();

		return token == "}";
	}
	else {
		return false;
	}
}

bool functionR(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "R\n";
	std::string token = getCurrentToken(lexemes);
	if (functionG(getPair(lexemes), printSwitch)) {
		std::string token = lexemes.front().getToken();
		if (token == ":=") {
			if (functionX(lexemes, printSwitch)) {
				token = lexemes.front().getToken();
				return token == ";";
			}
			else { return false; }
		}
		else { return false; }
	}
	else { return false; }
	return true;
}

bool functionS(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "S\n";
	std::string token = lexemes.front().getToken();
	if (token == "if") {
		token = lexemes.front().getToken();
		if (token == "(") {
			if (!functionY(lexemes, printSwitch)) {
				return false;
			}
			token = lexemes.front().getToken();
			if (token == ")") {
				if (!functionP(lexemes, printSwitch)) { return false; }
				if (!functionSprime(lexemes, printSwitch)) { return false; }
				return true;
			}
			else { return false; }
		}
		else { return false; }
	}
	else { return false; }
}

bool functionSprime(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "Sprime\n";
	std::string token = lexemes.front().getToken();
	if (token == "fi") {
		return true;
	}
	else if (token == "else") {
		if (functionP(lexemes, printSwitch)) {
			token = lexemes.front().getToken();
			if (token == "fi") {
				return true;
			}
		}
	}
	return false;
}

bool functionT(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "T\n";
	std::string token = lexemes.front().getToken();
	if (token == "return") {
		return functionTprime(lexemes, printSwitch);
		//if (functionTprime(lexemes, printSwitch)) { return true; }
		//else { return false; }
	}
	return false;
}

bool functionTprime(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "Tprime\n";
	if (functionX(lexemes, printSwitch)) {
		std::string token = lexemes.front().getToken();
		return token == ";";
	}
	std::string token = lexemes.front().getToken();
	return token == ";";
}

bool functionU(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "U\n";
	std::string token = lexemes.front().getToken();
	if (token == "write") {
		token = lexemes.front().getToken();
		if (token == "(") {
			if (functionX(lexemes, printSwitch)) {
				token = lexemes.front().getToken();
				if (token == ")") {
					token = lexemes.front().getToken();
					if (token == ";") {
						return true;
					}
				}
			}
		}
	}
	// if token != write
	return false;
}

bool functionV(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "V\n";
	std::string token = lexemes.front().getToken();
	if (token == "read") {
		token = lexemes.front().getToken();
		if (token == "(") {
			if (functionL(lexemes, printSwitch)) {
				token = lexemes.front().getToken();
				if (token == ")") {
					token = lexemes.front().getToken();
					if (token == ";") {
						return true;
					}
				}
			}
		}
	}
	// if token != read
	return false;
}

bool functionW(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "W\n";
	std::string token = lexemes.front().getToken();
	if (token == "while") {
		token = lexemes.front().getToken();
		if (token == "(") {
			if (functionX(lexemes, printSwitch)) {
				token = lexemes.front().getToken();
				if (token == ")") {
					if (functionP(lexemes, printSwitch)) {
						return true;
					}
				}
			}
		}
	}
	// if token != while
	return false;
}

bool functionY(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "Y\n";
	if (functionX(lexemes, printSwitch)) {
		if (functionZ(lexemes, printSwitch)) {
			if (functionX(lexemes, printSwitch)) { return true; }
			else { return false; }
		} 
		else { return false;  }
	}
	else { return false; }
	return true;
}

bool functionZ(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "Z\n";
	std::string token = getCurrentToken(lexemes);

	return (token == "=") || (token == "/=") || (token == ">")
		|| (token == "<") || (token == "=>") || (token == "<=");
}

bool functionX(std::list<Pair>&lexemes, bool printSwitch) {
	if (functionAprime(lexemes, printSwitch)) {
		if (functionXprime(lexemes, printSwitch)) {
			return true;
		}
		else {
			return true;
		}
	}
	else {
		return false;
	}
}

bool functionXprime(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "Xprime\n";
	std::string token = getCurrentToken(lexemes);
	if (token == "+") {
		if (functionAprime(lexemes, printSwitch)) {
			if (functionXprime(lexemes, printSwitch)) { return true; }
			else { return false; }
		}
		else { return false; }
	}
	else if (token == "-") {
		if (functionAprime(lexemes, printSwitch)) {
			if (functionXprime(lexemes, printSwitch)) { return true; }
			else { return false; }
		}
		else { return false; }
	}
	else { return functionEprime(printSwitch); }
}

bool functionAprime(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "Aprime\n";
	if (functionBprime(lexemes, printSwitch)) {
		if (functionA2(lexemes, printSwitch)) { return true; }
		else { return true; }
	}
	return false;
}

bool functionA2(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "A2\n";
	std::string token = getCurrentToken(lexemes);

	if (token == "*") {
		if (functionBprime(lexemes, printSwitch)) {
			if (functionA2(lexemes, printSwitch)) { return true; }
			else { return false; }
		}
		else { return false;  }
	}
	else if (token == "/") {
		if (functionBprime(lexemes, printSwitch)) {
			if (functionA2(lexemes, printSwitch)) { return true; }
			else { return false; }
		}
		else { return false; }
	}
	else { return functionEprime(printSwitch); }
}

bool functionBprime(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "Bprime\n";
	std::string token = getCurrentToken(lexemes);
	if (token == "-") {
		if (functionCprime(lexemes, printSwitch)) { return true; }
		else { return false; }
	}
	else { return functionCprime(lexemes, printSwitch); }
}

bool functionCprime(std::list<Pair>&lexemes, bool printSwitch) {
	std::cout << "Cprime\n";
	std::string token = getCurrentToken(lexemes);
	if (functionG(getPair(lexemes), printSwitch)) {
		token = getCurrentToken(lexemes);
		if (token == "[") {
			if (functionL(lexemes, printSwitch)) {
				token = lexemes.front().getToken();
				if (token == "]") {
					return true;
				}
			}
		}
		return true;
	}
	if (functionDprime(printSwitch)) { return true; }
	if (functionFprime(printSwitch)) { return true; }
	token = getCurrentToken(lexemes);
	if (token == "(") {
		if (functionX(lexemes, printSwitch)) {
			token = lexemes.front().getToken();
			if (token == ")") {
				return true;
			}
		}
	}
	return (token == "true") || (token == "false");
}

bool functionEprime(bool printSwitch) {
	std::cout << "Eprime\n";
	return true;
}

bool functionDprime(bool printSwitch) {
	std::cout << "Dprime\n";
	return true;
}

bool functionFprime(bool printSwitch) {
	std::cout << "Fprime\n";
	return true;
}

/* Returns true if the current token is an identifier */
bool functionG(Pair currToken, bool printSwitch) {
	std::cout << "G->identifier: " << currToken.getToken() << " " << std::endl;
	if (currToken.getType() == "identifier") {
		std::cout << currToken.getToken() << " is an identifier!\n";
	}
	else {
		std::cout << currToken.getToken() << " is not an identifier\n";
	}
	return currToken.getType() == "identifier";
}
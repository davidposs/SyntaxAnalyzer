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
	/* Only way I can separate \n and \t. Tried with \\n and \\t, no luck */
	while ((token[0] == '\\' && token[1] == 'n')
		|| (token[0] == '\\' && token[1] == 't')) {
		lexemes.pop_front();
		token = lexemes.front().getToken();
	}
	token = lexemes.front().getToken();
	lexemes.pop_front();
	//std::cout << "\t\t\t\tcurrent token is: \t\t" << token << std::endl;
	return token;
}

/* Gets the token+type pair at the top of the list */
Pair getPair(std::list<Pair>& lexemes) {
	std::string token = lexemes.front().getToken();
	while ((token[0] == '\\' && token[1] == 'n')
		|| (token[0] == '\\' && token[1] == 't')) {
		lexemes.pop_front();
		token = lexemes.front().getToken();
	}
	token = lexemes.front().getToken();
	std::string type = lexemes.front().getType();
	lexemes.pop_front();
	//std::cout << "\t\t\t\tcurrent token (pair) is: \t" << token << std::endl;
	return Pair(token, type);
}


void showTop(std::string fn, std::list<Pair>& lexemes) {
	std::cout << fn << ", " << lexemes.front().getToken() << std::endl;
}

bool functionA(std::list<Pair> lexemes, bool printSwitch) {
	std::cout << "<Rat17F>\n";
	std::string token;
	if (printSwitch) { 
		std::cout << "A-> B %% CD" << std::endl; 
	}

	if (functionB(lexemes, printSwitch)) {
		token = getCurrentToken(lexemes);
		std::cout << "In a/b " << token << std::endl;
		if (token == "%%") {
			//token = getCurrentToken(lexemes);
			if (functionC(lexemes, printSwitch)) {
				if (functionD(lexemes, printSwitch)) {
					std::cout << "\n\nCorrectly parsed!\n";
					return true;
				}
			}
			else { retError(lexemes); return false; }
		}
		else { retError(lexemes); return false; }
	}
	else { retError(lexemes); return false; }
	return true;
}

bool functionB(std::list<Pair>& lexemes, bool printSwitch) {
	if (printSwitch) {
		std::cout << "B-> E | E'\n";
	}
	if (functionE(lexemes, printSwitch)) {
		std::cout << "Created Optional Function Definition\n\n\n";
		return true;
	}
	else if (functionEprime(printSwitch)) {
		std::cout << "Created Optional Function Definition\n\n\n";
		return true;
	}
	return false;
}

bool functionE(std::list<Pair>& lexemes, bool printSwitch) {
	if (printSwitch) { std::cout << "E-> F | FE \n"; }
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
	if (printSwitch) {
		std::cout << "F-> @ G (H) C I\n";
	}
	std::string token = getCurrentToken(lexemes);
	if (token == "@") {
		// see if next token is an identifier
		if (functionG(lexemes, printSwitch)) {
			//lexemes.pop_front();
			token = getCurrentToken(lexemes);
			//std::cout << "In F - token is: " << token << std::endl;
			if (token == "(") {
				if (functionH(lexemes, printSwitch)) {
					//std::cout << "H passed " << std::endl;
					token = getCurrentToken(lexemes);
					//std::cout << "after h passed : " << token << std::endl;
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
	if (printSwitch) { std::cout << "H-> J | E' \n"; }
	if (functionJ(lexemes, printSwitch)) { return true;	}
	/* chanhged to return functionEprime from just calling it*/
	else { return functionEprime(printSwitch);	}
}

bool functionJ(std::list<Pair>& lexemes, bool printSwitch) {
	if (printSwitch) { std::cout << "J-> K | K, J \n"; }
	if (functionK(lexemes, printSwitch)) {
		Pair temp = getPair(lexemes);
		std::string token = temp.getToken();
		if (token == ",") {
			if (functionJ(lexemes, printSwitch)) { return true;	}
		}
		else { /* token is not "," */
			lexemes.push_front(temp);
			return true; } 
	}
	return false;
}

bool functionK(std::list<Pair>& lexemes, bool printSwitch) {
	if (printSwitch) {
		std::cout << "K-> L:M\n";
	}
	if (functionL(lexemes, printSwitch)) {
		std::string token = getCurrentToken(lexemes);
		//std::cout << " in k: " << token << std::endl;
		if (token == ":") {
			//std::cout << "returning true from k = : " << std::endl;
			return functionM(lexemes, printSwitch);
		}
		else { 
			return false; 
		}
		return true;	
	}
	else {
		retError(lexemes);
		return false;
	}
}

bool functionM(std::list<Pair>& lexemes, bool printSwitch) {
	if (printSwitch) {
		std::cout << "M-> integer | boolean | floating\n";
	}
	Pair temp = getPair(lexemes);
	std::string currToken = temp.getToken();
	//std::cout << "M->integer: " << currToken << " " << std::endl;
	if ((currToken == "integer")
		|| (currToken == "boolean")
		|| (currToken == "real")) {
		return true;
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
}

bool functionI(std::list<Pair>&lexemes, bool printSwitch) {
	if (printSwitch) {
		std::cout << "I->{ D }\n";
	}
	std::string token = getCurrentToken(lexemes);
	std::cout << "in I, token is : " << token << std::endl;
	if (token == "{") {
		if (functionD(lexemes, printSwitch)) {
			token = getCurrentToken(lexemes);
			std::cout << " after D in i, token is : " << token;
			return token == "}";
		}
		else { /* not function D */ 
			return false; 
		}
	}
	else { /* not { */
		return false;
	}
}

bool functionC(std::list<Pair>&lexemes, bool printSwitch) {
	if (printSwitch) { std::cout << "C->N | E' \n"; }
	if (functionN(lexemes, printSwitch)) {
		//std::cout << "returning from N is true " << std::endl;
		return true;
	}
	else {
		std::cout << "N did not pass in C " << std::endl;
		return functionEprime(printSwitch);
	}
}

bool functionN(std::list<Pair>&lexemes, bool printSwitch) {
	if (printSwitch) {
		std::cout << "N-> O; | O;N\n";
	}
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
	return false;
}

bool functionO(std::list<Pair>&lexemes, bool printSwitch) {
	if (printSwitch) {
		std::cout << "O-> ML\n";
	}
	if (functionM(lexemes, printSwitch)) {
		//std::cout << "returned true from M in o" << std::endl;
		return functionL(lexemes, printSwitch);
	}
	else {
		return false;
	}
}

bool functionL(std::list<Pair>& lexemes, bool printSwitch) {
	if (printSwitch) {
		std::cout << "L-> G | G, L\n";
	}
	std::string token;// = getCurrentToken(lexemes);
	//std::cout << "in l before g: " << token << std::endl;
	if (functionG(lexemes, printSwitch)) {
		Pair temp = getPair(lexemes);
		token = temp.getToken();
		std::cout << "Looking for , in L " << token << std::endl;
		if (token == ",") {
			//std::cout << "looking for , " << std::endl;
			if (functionL(lexemes, printSwitch)) { return true; }
			else { 
				lexemes.push_front(temp);
				return false; 
			}
		} // token is not ,
		else { //if (token == ":") {
			lexemes.push_front(temp);
			return true;
		}
	}
	else { 
		//std::cout << "L is bad with token: " << token << std::endl;
		return false; 
	}
	return true;
}

bool functionD(std::list<Pair>&lexemes, bool printSwitch) {
	if (printSwitch) {
		std::cout << "D-> P | P D\n";
	}
	if (functionP(lexemes, printSwitch)) {
		//showTop("D", lexemes);
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
	if (printSwitch) {
		std::cout << "P-> Q | R | S | T | U | V | W\n";
	}
	if (functionQ(lexemes, printSwitch)) { std::cout << " passed Q in P " << std::endl; return true; }
	if (functionR(lexemes, printSwitch)) { std::cout << " passed R in P " << std::endl; return true; }
	if (functionS(lexemes, printSwitch)) { std::cout << " passed S in P " << std::endl; return true; }
	if (functionT(lexemes, printSwitch)) { std::cout << " passed T in P " << std::endl; return true; }
	if (functionU(lexemes, printSwitch)) { std::cout << " passed U in P " << std::endl; return true; }
	if (functionV(lexemes, printSwitch)) { std::cout << " passed V in P " << std::endl; return true; }
	if (functionW(lexemes, printSwitch)) { std::cout << " passed W in P " << std::endl; return true; }
	return false;
}

bool functionQ(std::list<Pair>&lexemes, bool printSwitch) {
	if (printSwitch) {
		std::cout << "Q-> { D }\n";
	}
	std::string token;
	Pair temp = getPair(lexemes);
	token = temp.getToken();
	//std::cout << " in Q, token is : " << token << std::endl;
	if (token == "{") {
		if (functionD(lexemes, printSwitch)) {
			token = getCurrentToken(lexemes);
			//std::cout << " Q/d token is : " << token << std::endl;
			return token == "}";
		}
		else {
			return false;
		}
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
}

bool functionR(std::list<Pair>&lexemes, bool printSwitch) {
	if (printSwitch) {
		std::cout << "R->G := X\n";
	}
	std::string token;
	Pair temp = getPair(lexemes);
	token = temp.getToken();
	//std::cout << "in R, token is " << token << std::endl;
	if (functionG(lexemes, printSwitch)) {
		token = getCurrentToken(lexemes);
		if (token == ":=") {
			if (functionX(lexemes, printSwitch)) {
				std::cout << " Expression G := X\n";
				token = getCurrentToken(lexemes);
				return token == ";";
			}
			else { return false; }
		}
		else { return false; }
	}
	else { 
		/* Was not an identifier, put it back and keep looking */
		lexemes.push_front(temp);
		return false; 
	}


	return true;
}

bool functionS(std::list<Pair>&lexemes, bool printSwitch) {
	if (printSwitch) {
		std::cout << "S-> if ( Y ) P S'\n";
	}
	std::string token;
	Pair temp = getPair(lexemes);
	token = temp.getToken();
	if (token == "if") {
		token = getCurrentToken(lexemes);
		if (token == "(") {
			if (!functionY(lexemes, printSwitch)) {
				return false;
			}
			token = getCurrentToken(lexemes);
			if (token == ")") {
				if (!functionP(lexemes, printSwitch)) { return false; }
				if (!functionSprime(lexemes, printSwitch)) { return false; }
				return true;
			}
			else { return false; }
		}
		else { return false; }
	}
	else { 
		lexemes.push_front(temp);
		return false; 
	}
}

bool functionSprime(std::list<Pair>&lexemes, bool printSwitch) {
	if (printSwitch) { std::cout << "Sprime -> fi | else P fi\n"; }
	std::string token = getCurrentToken(lexemes);
	if (token == "fi") {
		return true;
	}
	else if (token == "else") {
		if (functionP(lexemes, printSwitch)) {
			token = getCurrentToken(lexemes);
			if (token == "fi") {
				return true;
			}
		}
	}
	return false;
}

bool functionT(std::list<Pair>&lexemes, bool printSwitch) {
	if (printSwitch) {
		std::cout << "T-> return T'\n";
	}
	std::string token;
	Pair temp = getPair(lexemes);
	token = temp.getToken();
	std::cout << " in T: " << token << std::endl;
	if (token == "return") {
		return functionTprime(lexemes, printSwitch);
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
}

bool functionTprime(std::list<Pair>&lexemes, bool printSwitch) {
	if (printSwitch) { std::cout << "Tprime-> ; | X;\n"; }
	if (functionX(lexemes, printSwitch)) {
		std::string token = getCurrentToken(lexemes);
		if (token == ";") {
			//std::cout << "matched ; in Tprime/x " << std::endl;
			return true;
		}
		else {
			//std::cout << " didn't match ; in Tprime/x" << std::endl;
		}
	}
	std::string token = getCurrentToken(lexemes);
	//std::cout << " Tprime without X " << token << std::endl;
	return token == ";";
}

bool functionU(std::list<Pair>&lexemes, bool printSwitch) {
	if (printSwitch) { std::cout << "U-> write ( X );\n"; }
	std::string token = getCurrentToken(lexemes);
	if (token == "write") {
		token = getCurrentToken(lexemes);
		if (token == "(") {
			if (functionX(lexemes, printSwitch)) {
				token = getCurrentToken(lexemes);
				if (token == ")") {
					token = getCurrentToken(lexemes);
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
	if (printSwitch) { std::cout << "V-> read ( L ); \n"; }
	std::string token = getCurrentToken(lexemes);
	if (token == "read") {
		token = getCurrentToken(lexemes);
		if (token == "(") {
			if (functionL(lexemes, printSwitch)) {
				token = getCurrentToken(lexemes);
				if (token == ")") {
					token = getCurrentToken(lexemes);
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
	if (printSwitch) { std::cout << "W-> while ( Y ) P\n"; }
	std::string token = getCurrentToken(lexemes);
	if (token == "while") {
		token = getCurrentToken(lexemes);
		if (token == "(") {
			if (functionX(lexemes, printSwitch)) {
				token = getCurrentToken(lexemes);
				if (token == ")") {
					//std::cout << "matched in W" << std::endl;
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
	if (printSwitch) { std::cout << "Y-> X Z X\n"; }
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
	if (printSwitch) { std::cout << "Z-> = | /= | > | < | => | <=\n"; }
	std::string token = getCurrentToken(lexemes);

	return (token == "=") || (token == "/=") || (token == ">")
		|| (token == "<") || (token == "=>") || (token == "<=");
}

bool functionX(std::list<Pair>&lexemes, bool printSwitch) {
	if (printSwitch) { std::cout << "X-> A' | A'X'\n"; }
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
	if (printSwitch) { std::cout << "Xprime-> +A'X' | -A'X' | E'\n"; }
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
	if (printSwitch) { std::cout << "Aprime-> B' | B'A2\n"; }
	if (functionBprime(lexemes, printSwitch)) {
		if (functionA2(lexemes, printSwitch)) { return true; }
		else { return true; }
	}
	return false;
}

bool functionA2(std::list<Pair>&lexemes, bool printSwitch) {
	if (printSwitch) { std::cout << "A2-> *B'A2 | /B'A2 | E'\n"; }
	std::string token = getCurrentToken(lexemes);
	std::cout << " in A2 " << token << std::endl;
	if (token == "*") {
		//std::cout << " multiplying " << std::endl;
		if (functionBprime(lexemes, printSwitch)) {
			if (functionA2(lexemes, printSwitch)) { return true; }
			else { return false; }
		}
		else { return false;  }
	}
	else if (token == "/") {
		if (functionBprime(lexemes, printSwitch)) {
			//std::cout << "dividing " << std::endl;
			if (functionA2(lexemes, printSwitch)) { return true; }
			else { return false; }
		}
		else { return false; }
	}
	else { return functionEprime(printSwitch); }
}

bool functionBprime(std::list<Pair>&lexemes, bool printSwitch) {
	if (printSwitch) { std::cout << "Bprime -> -C' | C'\n"; }
	Pair temp = getPair(lexemes);
	std::string token = temp.getToken();
	//std::cout << " in Bprime " << token << std::endl;
	if (token == "-") {
		//std::cout << " subtracting in Bprime " << std::endl;
		if (functionCprime(lexemes, printSwitch)) { return true; }
		else { return false; }
	}
	else {
		lexemes.push_front(temp);
		//std::cout << "  going to cprime " << token << std::endl;
		return functionCprime(lexemes, printSwitch); 
	}
}

bool functionCprime(std::list<Pair>&lexemes, bool printSwitch) {
	if (printSwitch) { std::cout << "Cprime-> G | D' | G[L] | (X) | F' | true | false\n"; }
	std::string token;// = getCurrentToken(lexemes);
	//std::cout << " in Cprime " << token << std::endl;
	if (functionG(lexemes, printSwitch)) {
		Pair temp = getPair(lexemes);
		token = temp.getToken();
		//std::cout << " G in cprime is true: " << token << std::endl;
		if (token == "[") {
			if (functionL(lexemes, printSwitch)) {
				token = getCurrentToken(lexemes);
				if (token == "]") {
					return true;
				}
			}
		}
		else {
			lexemes.push_front(temp);
		}
		//return true;
	}
	std::cout << "before d Prime: " << token << std::endl;
	if (functionDprime(lexemes, printSwitch)) { 
		std::cout << " Dprime in c is true " << std::endl; 
		return true;
	}
	if (functionFprime(lexemes, printSwitch)) { 
		//std::cout << " Fprime in c is true " << std::endl;
		return true; 
	}
	token = getCurrentToken(lexemes);
	if (token == "(") {
		if (functionX(lexemes, printSwitch)) {
			token = getCurrentToken(lexemes);
			if (token == ")") {
				return true;
			}
		}
	}
	return (token == "true") || (token == "false");
}

bool functionEprime(bool printSwitch) {
	if (printSwitch) { std::cout << "Eprime -> epsilon\n"; }
	std::cout << "Eprime\n";
	return true;
}

bool functionDprime(std::list<Pair>& lexemes, bool printSwitch) {
	if (printSwitch) { std::cout << "Dprime -> integer\n"; }

	Pair temp = getPair(lexemes);
	std::string currToken = temp.getToken();
	std::string currType = temp.getType();
	std::cout  << currToken  << std::endl;
	if (currType == "integer") {
		std::cout << currType << " is an integer!\n";
		return true;
	}
	else {
		std::cout << currType << " is not an integer\n";
		lexemes.push_front(temp);
		return false;
	}
}

bool functionFprime(std::list<Pair>& lexemes, bool printSwitch) {
	if (printSwitch) { std::cout << "Fprime-> real\n"; }
	std::cout << "Fprime\n";
	Pair temp = getPair(lexemes);
	std::string currToken = temp.getToken();
	std::string currType = temp.getType();
	std::cout << currToken << std::endl;
	if (currType == "real") {
		std::cout << currToken << " is a real!\n";
		return true;
	}
	else {
		std::cout << currToken << " is not a real\n";
		lexemes.push_front(temp);
		return false;
	}
	return true;
}

/* Returns true if the current token is an identifier */
bool functionG(std::list<Pair>& lexemes, bool printSwitch) {
	if (printSwitch) { std::cout << "Gprime-> identifier\n"; }
	Pair temp = getPair(lexemes);
	std::string currToken = temp.getToken();
	std::string currType = temp.getType();
	//std::cout << "G->identifier: " << currToken << std::endl;
	if (currType == "identifier") {
		//std::cout << currType << " is an identifier!\n";
		return true;
	}
	else {
	//	std::cout << currToken << " is not an identifier\n";
		lexemes.push_front(temp);
		return false;
	}
}
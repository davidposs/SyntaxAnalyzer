/******************************************************************************
* File:    Pair.cpp
* Authors: David Poss, Douglas Galm
*
* Usage:   To create lexeme and token-type pairs that can be easily printed,
*          modified, and inserted into a list.
*
******************************************************************************/

#include "Pair.h"

Pair::Pair() : token(""), type("") {}
Pair::Pair(std::string l, std::string t) : token(l), type(t) {}

std::string Pair::getToken() { return token; }
std::string Pair::getType()  { return type; }

std::ostream& operator<<(std::ostream& os, Pair P) {
	os << "Token: " << P.token << "\t\t Type: " << P.type;
	return os;
}

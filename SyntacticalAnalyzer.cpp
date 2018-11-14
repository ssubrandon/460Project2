#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	lex = new LexicalAnalyzer (filename);
	token_type t;
	// program ();
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
}

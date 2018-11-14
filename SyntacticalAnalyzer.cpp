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

int SyntacticalAnalyzer::program(){

}

int SyntacticalAnalyzer::more_defines(){

}			

int SyntacticalAnalyzer::define(){

}

int SyntacticalAnalyzer::stmt_list(){

}
				
int SyntacticalAnalyzer::stmt(){

}
							
int SyntacticalAnalyzer::literal(){

}
						
int SyntacticalAnalyzer::more_tokens(){

}
					
int SyntacticalAnalyzer::quoted_lit(){

}			
					
int SyntacticalAnalyzer::param_list(){

}
					
int SyntacticalAnalyzer::else_part(){

}
					
int SyntacticalAnalyzer::stmt_pair(){

}
					
int SyntacticalAnalyzer::stmt_pair_body(){

}
					
int SyntacticalAnalyzer::action(){

}
						
int SyntacticalAnalyzer::any_other_token(){

}

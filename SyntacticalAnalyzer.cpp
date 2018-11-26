//Global TODO: 1) need to output to a different file (not input file from all of the functions)

#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

/// The GetTokenName() Function is given to us by Watts in her lexical.cpp

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{//TODO: needs output file.
	lex = new LexicalAnalyzer (filename);
	token_type t;

	int i =0;
	string file = "";
	while( filename[i] != '.'){
		file += filename[i];
		i++;
	}
	token = lex -> GetToken();
	ruleFile.open(file + ".Zp2");
	//	listingFile.open(file +"Z.lst");
	//	debugFile.open(file + "Z.dbg"); 
	//token = lex -> GetToken();

	//listingFile << "Input file: " << filename << endl;
	//debugFile << "Input file: " << filename << endl;
	//lex->listing
	//lex->debug
	// GetLexeme()?
	errors = program();
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{//TODO: close output
	cout << errors << " Syntactical errors found in input file\n";
	lex->listing << errors << " Syntactical errors found in input file\n";
	lex->debug << errors << " Syntactical errors found in input file\n";
	ruleFile.close();
	delete lex;
}

int SyntacticalAnalyzer::program(){
	// Take in input file and applies rule 1

	ruleFile  << "Entering Program function; current token is: " << lex ->GetTokenName (token) << ", lexeme: " << lex->GetLexeme() <<  endl; 
	int errors = 0;
	ruleFile << "Using Rule 1" << endl;

	if(token != LPAREN_T){
		errors++;
		ReportError(string("expected a LPAREN_T, currently in program, but found  " + lex->GetTokenName(token)));
	}
	token = lex->GetToken();

	errors += define();

	if(token != LPAREN_T){
		errors++;
		ReportError(string("expected a LPAREN_T in program, but found  " + lex->GetTokenName(token)));
	}

	token = lex->GetToken();
	errors += more_defines();
	if (token != EOF_T) {
		errors ++;
		// lex -> ("Unable to find end of file token");

	} // end of if EOF
	ruleFile << "Exiting Program function; current token is: " << lex ->GetTokenName (token) << endl;

	return errors;

}

int SyntacticalAnalyzer::more_defines(){

	ruleFile << "Entering more_defines function; Current token is: " << lex ->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	if(token == IDENT_T){
		ruleFile << "Using Rule 3" << endl;
		errors += stmt_list();
		if(token != RPAREN_T){
			errors++;
			ReportError(string("expected a RPAREN_T in m_d, but found  " + lex->GetTokenName(token)));
		}
		token = lex->GetToken();
		ruleFile << "Exiting more_defines function; Current token is: " << lex ->GetTokenName (token) << endl; 

		return errors; 


	} // end left paren. if 

	ruleFile << "Using Rule 2" << endl;
	errors += define();

	if(token != LPAREN_T){
		errors++;
		ReportError(string("expected a LPAREN_T in m_d, but found  " + lex->GetTokenName(token)));
	}
	token = lex->GetToken();

	errors+= more_defines();
	ruleFile << "Exiting more_defines function; current token is: " << lex ->GetTokenName (token) << endl;

	return errors;

}			

int SyntacticalAnalyzer::define(){
	ruleFile << "Entering define function; Current token is: " << lex ->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;

	int errors = 0;

	ruleFile << "Using  Rule 2" << endl;

	if(token != DEFINE_T){
		errors++;
		ReportError(string("expected a DEFINE_T in define, but found  " + lex->GetTokenName(token)));
	}

	token = lex->GetToken();
	if(token != LPAREN_T){
		errors++;
		ReportError(string("expected a LPAREN_T in define, but found  " + lex->GetTokenName(token)));
	}

	token = lex->GetToken();
	if(token != IDENT_T){
		errors++;
		ReportError(string("expected a IDENT_T in define, but found  " + lex->GetTokenName(token)));
	}

	token = lex->GetToken();
	errors += param_list();


	if(token != RPAREN_T){
		errors++;
		ReportError(string("expected a RPAREN_T in define, but found  " + lex->GetTokenName(token)));
	}
	token = lex->GetToken();
	errors += stmt();
	errors += stmt_list();


	if(token != RPAREN_T){
		errors++;
		ReportError(string("expected a RPAREN_T in define, but found  " + lex->GetTokenName(token)));
	}
	token = lex->GetToken();
	ruleFile << "Exiting Define function; current token is: " << lex ->GetTokenName (token) << endl;


	return errors;

}

int SyntacticalAnalyzer::stmt_list(){
	ruleFile << "Entering stmt_list function; Current token is: " << lex ->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;

	int errors = 0;
	if(token == EOF_T){
		errors++;
		ReportError(string("EOF_T token detected ending function stmt_list"));
		ruleFile << "Exiting stmt_list function; current token is: " << lex ->GetTokenName (token) << endl;
		return errors;

	}

	if(token == RPAREN_T) {
		ruleFile << "Using Rule 6" << endl;
		ruleFile << "Exiting stmt_list function; current token is: " << lex ->GetTokenName (token) << endl;
		return errors;
	} // End of Right paren if 
	ruleFile << "Using Rule 5" << endl;
	errors += stmt();
	errors += stmt_list();
	ruleFile << "Exiting stmt_list function; current token is: " << lex ->GetTokenName (token) << endl;

	return errors;

}

int SyntacticalAnalyzer::stmt(){
	ruleFile << "Entering stmt function; Current token is: " << lex ->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;

	int errors = 0;

	switch(token){
		case IDENT_T:
			ruleFile << "Using Rule 8" << endl;
			token = lex->GetToken();

		case LPAREN_T:
			ruleFile << "Using Rule 9" << endl;
			//ruleFile << "stmt function complete. Current token is: " << lex->GetTokenName(token) << endl;
			token = lex->GetToken();
			errors += action();
			//returned token should be a RPAREN?
			if(token == RPAREN_T){
				token=lex->GetToken();
			}
			else{
				ReportError(string("RPAREN_T expected in stmt function, current token is: " + lex->GetTokenName(token)));
			}

		case NUMLIT_T: 
			ruleFile << "stmt function complete. Current token is: " << lex->GetTokenName(token) << endl;
			ruleFile << "Using Rule 7"  << endl;
			errors += literal();

		case STRLIT_T:
			//ruleFile << "stmt function complete. Current token is: " << lex->GetTokenName(token) << endl;
			ruleFile << "Using Rule 7"  << endl;
			errors += literal();

		case SQUOTE_T:
			//ruleFile << "stmt function complete. Current token is: " << lex->GetTokenName(token) << endl;
			ruleFile << "Using Rule 7"  << endl;
			errors += literal();

		default: //no rule found
			errors++;
			ReportError(string("No rule found in stmt function, current token is: " + lex->GetTokenName(token)));

	}
	ruleFile << "Exiting stmt_list function; current token is: " << lex ->GetTokenName (token) << endl;
	return errors;	
}

int SyntacticalAnalyzer::literal(){
	ruleFile << "Entering literal function; Current token is: " << lex ->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors =0;
	/*
	   if (token != SQUOTE_T && token != NUMLIT_T && token != STRLIT_T){
	   errors ++;
	//token = lex->GetToken();
	}
	*/
	if (token == NUMLIT_T)
	{ 
		ruleFile << "Using Rule 10" << endl;
		token = lex->GetToken();
	}
	if (token == STRLIT_T)
	{
		ruleFile << "Using Rule 11" << endl;
		token = lex->GetToken();
	}
	if ( token == SQUOTE_T )
	{
		ruleFile << "Using Rule 12" << endl;
		ruleFile << "literal function complete. Current token is: " << lex->GetTokenName(token) << endl;
		token = lex->GetToken();
		errors += quoted_lit(); 
	}
	else{
		errors++;
		ReportError(string("expected either NUMLIT_T, STRLIT_T, or SQUOTE_T in literal but got: " + lex->GetTokenName(token)));
	}
	ruleFile << "Exiting literal function; current token is: " << lex ->GetTokenName (token) << endl;
	return errors;
}

int SyntacticalAnalyzer::more_tokens(){
	ruleFile << "Entering more_tokens function; Current token is: " << lex ->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;

	int errors = 0;
	/*
	   if (token == EOF_T){
	   errors++;
	// report error
	ruleFile << "more_tokens function complete. Current token is: " << lex->GetTokenName(token) << endl;
	return errors;

	}
	*/
	if (token == RPAREN_T) {
		ruleFile << "Using Rule 15" << endl;
		ruleFile << "Exiting more_tokens function; current token is: " << lex ->GetTokenName (token) << endl;
		return errors;
	}
	ruleFile << "Using Rule 14" << endl;
	errors += any_other_token();
	errors += more_tokens();
	ruleFile << "Exiting more_tokens function; current token is: " << lex ->GetTokenName (token) << endl;

	return errors;

}

int SyntacticalAnalyzer::quoted_lit(){
	ruleFile << "Entering quoted_lit function; Current token is: " << lex ->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	ruleFile << "Using Rule 13" << endl;
	errors += any_other_token();
	ruleFile << "Exiting quoted_lit function; current token is: " << lex ->GetTokenName (token) << endl;

	return errors;

}			

int SyntacticalAnalyzer::param_list(){
	ruleFile << "Entering param_list function; Current token is: " << lex ->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;



	if(token == RPAREN_T) {
		ruleFile << "Using Rule 17" << endl;    
		ruleFile << "Exiting param_list function; current token is: " << lex ->GetTokenName (token) << endl;
		return errors;
	}
	else if (token == IDENT_T){
		ruleFile << "Using Rule 16" << endl;
		token = lex->GetToken();
		errors += param_list();
	}
	else{//we have an error
		errors++;
		ReportError(string("expected a IDENT_T or RPAREN_T in paramList, but found  " + lex->GetTokenName(token)));
		ruleFile << "Exiting param_list function; current token is: " << lex ->GetTokenName (token) << endl;
		return errors;
	}
	ruleFile << "Exiting param_list function; current token is: " << lex ->GetTokenName (token) << endl;

	/* I think this is wrong -Brandon	
	   while(token != IDENT_T) {
	   errors++;
	   ReportError(string("expected a IDENT_T in paramList, but found  " + lex->GetTokenName(token)));
	   token = lex->GetToken();
	   }
	   */
}


int SyntacticalAnalyzer::else_part(){
	ruleFile << "Entering else_part function; Current token is: " << lex ->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;

	int errors = 0;
	if (token == EOF_T) {
		errors++;
		ReportError(string("Reached an EOF_T but expected something else in else part"));
		ruleFile << "Exiting else_part function; current token is: " << lex ->GetTokenName (token) << endl;
		return errors;
	} // end EOF if 
	if (token == RPAREN_T) {
		ruleFile << "Using Rule 19" << endl;
		ruleFile << "Exiting else_part function; current token is: " << lex ->GetTokenName (token) << endl;
		return errors;
	} // end Right paren if  
	ruleFile << "Using Rule 18" << endl;
	errors += stmt();
	ruleFile << "Exiting else_part function; current token is: " << lex ->GetTokenName (token) << endl;

	return errors;
}

int SyntacticalAnalyzer::stmt_pair(){
	ruleFile << "Entering stmt_pair function; Current token is: " << lex ->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;

	int errors = 0;
	if(token != LPAREN_T){
		errors ++;
		//lex->ReportError("Giggity Broken ");
		//exit(1);;
		token = lex->GetToken();	
	} // end left paren if
	token = lex->GetToken();
	ruleFile << "Using Rule 20" << endl;
	errors += stmt_pair_body();
	while(token != RPAREN_T){

		errors++;
		ReportError(string("expected a RPAREN_T in stmtpair, but found  " + lex->GetTokenName(token)));
		//lex->ReportError("It done broke! ");
		//exit(1);
		token = lex->GetToken();
	} // end right paren if 
	token = lex->GetToken();
	ruleFile << "Exiting stmt_pair function; current token is: " << lex ->GetTokenName (token) << endl;

	return errors;

}

int SyntacticalAnalyzer::stmt_pair_body(){
	ruleFile << "Entering stmt_pair_body function; Current token is: " << lex ->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	if (token == ELSE_T){
		//rule 23
		token = lex->GetToken();
		errors += stmt();
		if (token != RPAREN_T){
			errors++;
			ReportError(string("expected a RPAREN_T in stmt_pair_body, but found  " + lex->GetTokenName(token)));
		}
	}

	if(token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T ||token == IDENT_T || token== LPAREN_T) {
		//rule 22
		errors += stmt();
		errors += stmt();
		if (token != RPAREN_T){
			errors++;
			ReportError(string("expected a RPAREN_T in stmt_pair_body, but found  " + lex->GetTokenName(token)));
		}
		token = lex->GetToken();
		errors += stmt_pair();
	}

	ruleFile << "Exiting stmt_pair_body function; current token is: " << lex ->GetTokenName (token) << endl;
	return errors;

}

int SyntacticalAnalyzer::action(){
	ruleFile << "Entering Action function; Current token is: " << lex ->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	if (token == IF_T)
	{//applying rule 24
		ruleFile << "Using Rule 24" << endl;
		token = lex->GetToken();
		errors += stmt();
		errors += stmt();
		errors+= else_part();
		ruleFile << "Exiting action function; current token is: " << lex ->GetTokenName (token) << endl;
		return errors;
	}
	if (token == COND_T)
	{//applying rule 25
		ruleFile << "Using Rule 25" << endl;
		token = lex->GetToken();

		if (token != LPAREN_T){
			errors++;
			ReportError(string("LPAREN_T expected in action, but see " + lex->GetTokenName(token)));
		}

		token = lex->GetToken();
		errors+= stmt_pair_body();
		ruleFile << "Exiting action function; current token is: " << lex ->GetTokenName (token) << endl;
		return errors;
	}
	if ( token == LISTOP_T){
		//RULE 26
		ruleFile << "Using Rule 26" << endl;
		errors += stmt();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == CONS_T){
		//RULE 27
		ruleFile << "Using Rule 27" << endl;
		token = lex->GetToken();
		errors += stmt();
		errors += stmt();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == AND_T){
		//RULE 28
		ruleFile << "Using Rule 28" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == OR_T){
		//RULE 29
		ruleFile << "Using Rule 29" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == NOT_T){
		//RULE 30
		ruleFile << "Using Rule 30" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == NUMBERP_T){
		//RULE 31
		ruleFile << "Using Rule 31" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == LISTP_T){
		//RULE 32
		ruleFile << "Using Rule 32" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == ZEROP_T){
		//RULE 33
		ruleFile << "Using Rule 33" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == NULLP_T){
		//RULE 34
		ruleFile << "Using Rule 34" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == STRINGP_T){
		//RULE 35
		ruleFile << "Using Rule 35" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == PLUS_T){
		//RULE 36
		ruleFile << "Using Rule 36" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == MINUS_T){
		//RULE 37
		ruleFile << "Using Rule 37" << endl;
		token = lex->GetToken();
		errors += stmt();
		errors += stmt_list();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == DIV_T){
		//RULE 38
		ruleFile << "Using Rule 38" << endl;
		token = lex->GetToken();
		errors += stmt();
		errors += stmt_list();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == MULT_T){
		//RULE 39
		ruleFile << "Using Rule 39" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == MODULO_T){
		//RULE 40
		ruleFile << "Using Rule 40" << endl;
		token = lex->GetToken();
		errors += stmt();
		errors += stmt();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == ROUND_T){
		//RULE 41
		ruleFile << "Using Rule 41" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == EQUALTO_T){
		//RULE 42
		ruleFile << "Using Rule 42" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == GT_T){
		//RULE 43
		ruleFile << "Using Rule 43" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == LT_T){
		//RULE 44
		ruleFile << "Using Rule 44" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == GTE_T){
		//RULE 45
		ruleFile << "Using Rule 45" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == LTE_T){
		//RULE 46
		ruleFile << "Using Rule 46" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == IDENT_T){
		//RULE 47
		ruleFile << "Using Rule 47" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == DISPLAY_T){
		//RULE 48
		ruleFile << "Using Rule 48" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == NEWLINE_T){
		//RULE 49
		ruleFile << "Using Rule 49" << endl;
		token = lex->GetToken();
		ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	//no rule found
	//ruleFile << "error in action" << endl;
	errors++;
	ReportError(string("no rule found in action, curren token is " + lex->GetTokenName(token)));
	token = lex->GetToken();
	ruleFile << "Exiting action function; current token is: " << lex->GetTokenName(token) << endl;
	return errors;

}

int SyntacticalAnalyzer::any_other_token(){
	int errors = 0;
	string error_type;
	ruleFile << "Entering any_other_token function; Current token is: " << lex ->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;

	switch(token){
		case LPAREN_T:
			ruleFile << "Using Rule 50:\n";
			token = lex->GetToken();
			errors += more_tokens();
			if(token != RPAREN_T){
				errors++;
				ReportError(string("Expected RPARENT_T in any_other_token, but found " + lex->GetTokenName(token)));
			}
			else token = lex->GetToken();
			break;
		case IDENT_T:
			ruleFile << "Using Rule 51\n";
			token = lex->GetToken();
			break;
		case NUMLIT_T:
			ruleFile << "Using Rule 52\n";
			token = lex->GetToken();
			break;
		case STRLIT_T:
			ruleFile << "Using Rule 53\n";
			token = lex->GetToken();
			break;
		case CONS_T:
			ruleFile << "Using Rule 54\n";
			token = lex->GetToken();
			break;
		case IF_T:
			ruleFile << "Using Rule 55\n";
			token = lex->GetToken();
			break;
		case DISPLAY_T:
			ruleFile << "Using Rule 56\n";
			token = lex->GetToken();
			break;
		case NEWLINE_T:
			ruleFile << "Using Rule 57\n";
			token = lex->GetToken();
			break;
		case LISTOP_T:
			ruleFile << "Using Rule 58\n";
			token = lex->GetToken();
			break;
		case AND_T:
			ruleFile << "Using Rule 59\n";
			token = lex->GetToken();
			break;
		case OR_T:
			ruleFile << "Using Rule 60\n";
			token = lex->GetToken();
			break;
		case NOT_T:
			ruleFile << "Using Rule 61\n";
			token = lex->GetToken();
			break;
		case DEFINE_T:
			ruleFile << "Using Rule 62\n";
			token = lex->GetToken();
			break;
		case NUMBERP_T:
			ruleFile << "Using Rule 63\n";
			token = lex->GetToken();
			break;
		case LISTP_T:
			ruleFile << "Using Rule 64\n";
			token = lex->GetToken();
			break;
		case ZEROP_T:
			ruleFile << "Using Rule 65\n";
			token = lex->GetToken();
			break;
		case NULLP_T:
			ruleFile << "Using Rule 66\n";
			token = lex->GetToken();
			break;
		case STRINGP_T:
			ruleFile << "Using Rule 67\n";
			token = lex->GetToken();
			break;
		case PLUS_T:
			ruleFile << "Using Rule 68\n";
			token = lex->GetToken();
			break;
		case MINUS_T:
			ruleFile << "Using Rule 69\n";
			token = lex->GetToken();
			break;
		case DIV_T:
			ruleFile << "Using Rule 70\n";
			token = lex->GetToken();
			break;
		case MULT_T:
			ruleFile << "Using Rule 71\n";
			token = lex->GetToken();
			break;
		case MODULO_T:
			ruleFile << "Using Rule 72\n";
			token = lex->GetToken();
			break;
		case ROUND_T:
			ruleFile << "Using Rule 73\n";
			token = lex->GetToken();
			break;
		case EQUALTO_T:
			ruleFile << "Using Rule 74\n";
			token = lex->GetToken();
			break;
		case GT_T:
			ruleFile << "Using Rule 75\n";
			token = lex->GetToken();
			break;
		case LT_T:
			ruleFile << "Using Rule 76\n";
			token = lex->GetToken();
			break;
		case GTE_T:
			ruleFile << "Using Rule 77\n";
			token = lex->GetToken();
			break;
		case LTE_T:
			ruleFile << "Using Rule 78\n";
			token = lex->GetToken();
			break;
		case SQUOTE_T:
			ruleFile << "Using Rule 79\n";
			token = lex->GetToken();
			errors += any_other_token();
			break;
		case COND_T:
			ruleFile << "Using Rule 80\n";
			token = lex->GetToken();
			break;
		case ELSE_T:
			ruleFile << "Using Rule 81\n";
			token = lex->GetToken();
			break;
		default:
			//no rule found
			errors++;
			ReportError(string("No rule found in any_other_token function, current token is: " + lex->GetTokenName(token)));
			token = lex->GetToken();
	}

	//if the current token at this point is not apart of the follows then that is an error?
	ruleFile << "Exiting any_other_token function; current token is: " << lex->GetTokenName(token) << endl;
	return errors;
}

void SyntacticalAnalyzer::ReportError (const string & msg){
	lex->listing << "Syntax error at " << lex->get_line_num() << ',' << lex->get_pos() << ": " << msg << endl;
	lex->debug << "Syntax error at " << lex->get_line_num() << ',' << lex->get_pos() << ": " << msg << endl;

}

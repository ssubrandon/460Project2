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
	program ();

}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{//TODO: close output
	delete lex;
	//listingFile.close();
	//debugFile.close();
	ruleFile.close();

}

int SyntacticalAnalyzer::program(){
	// Take in input file and applies rule 1

	ruleFile  << "Current token is: " << lex ->GetTokenName (token) <<  endl; 
	int errors = 0;
	ruleFile << "Applying Rule 1" << endl;

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
	ruleFile << "Function complete, current token is: " << lex ->GetTokenName (token) << endl;

	return errors;

}

int SyntacticalAnalyzer::more_defines(){

	ruleFile << "Starting more_defines function. Current token is: " << lex ->GetTokenName(token) << endl;
	int errors = 0;
	if(token == IDENT_T){
		ruleFile << "Applying Rule 3" << endl;
		errors += stmt_list();
		if(token != RPAREN_T){
			errors++;
			ReportError(string("expected a RPAREN_T in m_d, but found  " + lex->GetTokenName(token)));
		}
		token = lex->GetToken();
		ruleFile << "more_defines function complete. Current token is: " << lex ->GetTokenName (token) << endl; 

		return errors; 


	} // end left paren. if 

	ruleFile << "Applying Rule 2" << endl;
	errors += define();

	if(token != LPAREN_T){
		errors++;
		ReportError(string("expected a LPAREN_T in m_d, but found  " + lex->GetTokenName(token)));
	}
	token = lex->GetToken();

	errors+= more_defines();
	ruleFile << "more_defines function complete. Current token is: " << lex ->GetTokenName (token) << endl;

	return errors;

}			

int SyntacticalAnalyzer::define(){

	ruleFile << "Start of  define. Current token is: " << lex->GetTokenName(token) <<  endl;

	int errors = 0;


	ruleFile << " Applying  Rule 2" << endl;


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
	ruleFile << "Define function complete. Current token is: " << lex->GetTokenName (token) << endl;


	return errors;

}

int SyntacticalAnalyzer::stmt_list(){
	ruleFile << "Starting stmt_List function. Current token is: " << lex->GetTokenName (token) << endl;

	int errors = 0;

	if(token == RPAREN_T) {
		ruleFile << "Applying Rule 6" << endl;
		ruleFile << "stmt_list function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	} // End of Right paren if 
	ruleFile << "Applying Rule 5" << endl;
	errors += stmt();
	errors += stmt_list();
	ruleFile << "stmt_list function complete. Current token is: " << lex ->GetTokenName(token) << endl;

	return errors;

}

int SyntacticalAnalyzer::stmt(){

	ruleFile << "Starting stmt function. Current token is: " << lex->GetTokenName (token) << endl;

	int errors = 0;
	if(token == IDENT_T) {
		ruleFile << "Applying Rule 8" << endl;
		token = lex->GetToken();
		ruleFile << "stmt function complete. Current token is: " << lex->GetTokenName(token) << endl;

		return errors;
	}

	if(token == LPAREN_T) {
		ruleFile << "Applying Rule 9" << endl;

		token = lex->GetToken();
		errors += action();

	}
	else{
		ruleFile << "Applying Rule 7" << endl;
		errors += literal();
	}
	ruleFile << "stmt function complete. Current token is: "
		<< lex->GetTokenName(token) << endl;

	return errors;

}

int SyntacticalAnalyzer::literal(){
	ruleFile << "Entering literal function; current token is: " << lex->GetTokenName(token) << endl;
	int errors =0;
	if (token != SQUOTE_T && token != NUMLIT_T && token != STRLIT_T){
		errors ++;
		token = lex->GetToken();
	}
	if (token == NUMLIT_T)
	{ 
		ruleFile << "Applying Rule 11" << endl;
	}
	if (token == STRLIT_T)
	{
		ruleFile << "Applying Rule 12" << endl;
	}
	if ( token == SQUOTE_T )
	{
		//    "Applying rule 12"
		token = lex->GetToken();
		errors += quoted_lit(); 
	}
	ruleFile << "literal function complete. Current token is: " << lex->GetTokenName(token) << endl;
	return errors;
}

int SyntacticalAnalyzer::more_tokens(){

	ruleFile << "Entering More_Tokens function; current token is: " << lex->GetTokenName(token) << endl;

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
		ruleFile << "Applying Rule 15" << endl;
		ruleFile << "more_tokens function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	ruleFile << "Applying Rule 14" << endl;
	errors += any_other_token();
	errors += more_tokens();
	ruleFile << "more_tokens function complete. Current token is: " << lex->GetTokenName(token) << endl;

	return errors;

}

int SyntacticalAnalyzer::quoted_lit(){

	int errors = 0;
	ruleFile << "Starting quoted_lit function. Current token is: " << lex->GetTokenName(token) << endl;

	ruleFile << "Applying Rule 13" << endl;
	errors += any_other_token();
	ruleFile << "quoted_lit function complete. Current token is: " << lex->GetTokenName(token) << endl;

	return errors;

}			

int SyntacticalAnalyzer::param_list(){

	ruleFile << " param_list function starting. Current token is: " << lex->GetTokenName(token) << endl;
	int errors = 0;

 

	if(token == RPAREN_T) {
		ruleFile << "Applying Rule 17" << endl;    
		ruleFile << "param_list function complete. Current token is: " << lex->GetTokenName (token) << endl;
		return errors;
	}
	else if (token == IDENT_T){
		ruleFile << "param_list function complete. Current token is: " << lex->GetTokenName (token) << endl;
		ruleFile << "Applying Rule 16" << endl;
		token = lex->GetToken();
		errors += param_list();
	}
	else{//we have an error
		errors++;
		ReportError(string("expected a IDENT_T or RPAREN_T in paramList, but found  " + lex->GetTokenName(token)));
		return errors;
		ruleFile << "param_list function complete. Current token is: " << lex->GetTokenName(token) << endl;
	}

	       /* I think this is wrong -Brandon	
		while(token != IDENT_T) {
			errors++;
			ReportError(string("expected a IDENT_T in paramList, but found  " + lex->GetTokenName(token)));
			token = lex->GetToken();
		}
		*/
}


int SyntacticalAnalyzer::else_part(){

	ruleFile << "Starting else_part function. Current token is: " << lex->GetTokenName(token) << endl;

	int errors = 0;
	if (token == EOF_T) {
		errors++;
		ReportError(string("Reached an EOF_T but expected something else in else part"));
		ruleFile << "else_part function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	} // end EOF if 
	if (token == RPAREN_T) {
		ruleFile << "Applying Rule 19" << endl;
		ruleFile << "else_part function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	} // end Right paren if  
	ruleFile << "Applying Rule 18" << endl;
	errors += stmt();
	ruleFile << "else_part function complete. Current token is: " << lex->GetTokenName(token) << endl;

	return errors;
}

int SyntacticalAnalyzer::stmt_pair(){

	ruleFile << "Entering Stmt_Pair function; current token is: " << lex->GetTokenName(token) << endl;

	int errors = 0;
	if(token != LPAREN_T){
		errors ++;
		//lex->ReportError("Giggity Broken ");
		//exit(1);;
		token = lex->GetToken();	
	} // end left paren if
	token = lex->GetToken();
	ruleFile << "Applying Rule 20" << endl;
	errors += stmt_pair_body();
	while(token != RPAREN_T){

		errors++;
		ReportError(string("expected a RPAREN_T in stmtpair, but found  " + lex->GetTokenName(token)));
		//lex->ReportError("It done broke! ");
		//exit(1);
		token = lex->GetToken();
	} // end right paren if 
	token = lex->GetToken();
	ruleFile << "stmt_pair function complete. Current token is: " << lex->GetTokenName(token) << endl;

	return errors;

}

int SyntacticalAnalyzer::stmt_pair_body(){

	ruleFile << "Starting stmt_pair_body function. Current token is: " << lex->GetTokenName(token)<< endl;
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



	/*
	   if(token == LPAREN_T) {
	   ruleFile << "Applying Rule 21" << endl;
	   token = lex->GetToken();
	   errors += action();

	   while(token != RPAREN_T ) {
	   errors++;
//lex->ReportError ("Brandon! Fix it! Please ... <3 ) ");
//exit(1);;
token = lex->GetToken();
}
token = lex->GetToken();
errors += stmt();
}
// end left paren if

if(token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T) {
ruleFile << "Applying Rule 23" << endl;
errors += literal();
}
else{
ruleFile << "Applying Rule 22" << endl;
errors += action();
} // end literal and action if else

*/ruleFile << "Exiting Stmt_Pair_Body function; current token is: " << lex->GetTokenName(token) << endl;
return errors;

}

int SyntacticalAnalyzer::action(){
	int errors = 0;
	ruleFile << "Entering Action function; current token is: " << lex->GetTokenName(token) << endl;
	if (token == IF_T)
	{//applying rule 24
		ruleFile << "Applying Rule 24" << endl;
		token = lex->GetToken();
		errors += stmt();
		errors += stmt();
		errors+= else_part();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if (token == COND_T)
	{//applying rule 25
		ruleFile << "Applying Rule 25" << endl;
		token = lex->GetToken();
		
		if (token != LPAREN_T){
			errors++;
			ReportError(string("LPAREN_T expected in action, but see " + lex->GetTokenName(token)));
		}
		
		token = lex->GetToken();
		errors+= stmt_pair_body();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == LISTOP_T){
		//RULE 26
		ruleFile << "Applying Rule 26" << endl;
		errors += stmt();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == CONS_T){
		//RULE 27
		ruleFile << "Applying Rule 27" << endl;
		token = lex->GetToken();
		errors += stmt();
		errors += stmt();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == AND_T){
		//RULE 28
		ruleFile << "Applying Rule 28" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == OR_T){
		//RULE 29
		ruleFile << "Applying Rule 29" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == NOT_T){
		//RULE 30
		ruleFile << "Applying Rule 30" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == NUMBERP_T){
		//RULE 31
		ruleFile << "Applying Rule 31" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == LISTP_T){
		//RULE 32
		ruleFile << "Applying Rule 32" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == ZEROP_T){
		//RULE 33
		ruleFile << "Applying Rule 33" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == NULLP_T){
		//RULE 34
		ruleFile << "Applying Rule 34" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == STRINGP_T){
		//RULE 35
		ruleFile << "Applying Rule 35" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == PLUS_T){
		//RULE 36
		ruleFile << "Applying Rule 36" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == MINUS_T){
		//RULE 37
		ruleFile << "Applying Rule 37" << endl;
		token = lex->GetToken();
		errors += stmt();
		errors += stmt_list();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == DIV_T){
		//RULE 38
		ruleFile << "Applying Rule 38" << endl;
		token = lex->GetToken();
		errors += stmt();
		errors += stmt_list();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == MULT_T){
		//RULE 39
		ruleFile << "Applying Rule 39" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == MODULO_T){
		//RULE 40
		ruleFile << "Applying Rule 40" << endl;
		token = lex->GetToken();
		errors += stmt();
		errors += stmt();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == ROUND_T){
		//RULE 41
		ruleFile << "Applying Rule 41" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == EQUALTO_T){
		//RULE 42
		ruleFile << "Applying Rule 42" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == GT_T){
		//RULE 43
		ruleFile << "Applying Rule 43" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == LT_T){
		//RULE 44
		ruleFile << "Applying Rule 44" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == GTE_T){
		//RULE 45
		ruleFile << "Applying Rule 45" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == LTE_T){
		//RULE 46
		ruleFile << "Applying Rule 46" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == IDENT_T){
		//RULE 47
		ruleFile << "Applying Rule 47" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == DISPLAY_T){
		//RULE 48
		ruleFile << "Applying Rule 48" << endl;
		token = lex->GetToken();
		errors += stmt();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if ( token == NEWLINE_T){
		//RULE 49
		ruleFile << "Applying Rule 49" << endl;
		token = lex->GetToken();
		ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	//no rule found
	//ruleFile << "error in action" << endl;
	errors++;
	ReportError(string("no rule found in action, curren token is " + lex->GetTokenName(token)));
	token = lex->GetToken();
	ruleFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
	return errors;

}

int SyntacticalAnalyzer::any_other_token(){
	int errors = 0;
	string error_type;
	ruleFile << "Entering Any_other_token function; current token is: " << lex->GetTokenName(token) << endl;
	
	switch(token){
		case LPAREN_T:
			ruleFile << "Applying Rule 50:\n";
			token = lex->GetToken();
			errors += more_tokens();
			if(token != RPAREN_T){
				errors++;
				ReportError(string("Expected RPARENT_T in a_o_t, but found " + lex->GetTokenName(token)));
			}
			else token = lex->GetToken();
			break;
		case IDENT_T:
			ruleFile << "Applying Rule 51\n";
			token = lex->GetToken();
			break;
		case NUMLIT_T:
			ruleFile << "Applying Rule 52\n";
			token = lex->GetToken();
			break;
		case STRLIT_T:
			ruleFile << "Applying Rule 53\n";
			token = lex->GetToken();
			break;
		case CONS_T:
			ruleFile << "Applying Rule 54\n";
			token = lex->GetToken();
			break;
		case IF_T:
			ruleFile << "Applying Rule 55\n";
			token = lex->GetToken();
			break;
		case DISPLAY_T:
			ruleFile << "Applying Rule 56\n";
			token = lex->GetToken();
			break;
		case NEWLINE_T:
			ruleFile << "Applying Rule 57\n";
			token = lex->GetToken();
			break;
		case LISTOP_T:
			ruleFile << "Applying Rule 58\n";
			token = lex->GetToken();
			break;
		case AND_T:
			ruleFile << "Applying Rule 59\n";
			token = lex->GetToken();
			break;
		case OR_T:
			ruleFile << "Applying Rule 60\n";
			token = lex->GetToken();
			break;
		case NOT_T:
			ruleFile << "Applying Rule 61\n";
			token = lex->GetToken();
			break;
		case DEFINE_T:
			ruleFile << "Applying Rule 62\n";
			token = lex->GetToken();
			break;
		case NUMBERP_T:
			ruleFile << "Applying Rule 63\n";
			token = lex->GetToken();
			break;
		case LISTP_T:
			ruleFile << "Applying Rule 64\n";
			token = lex->GetToken();
			break;
		case ZEROP_T:
			ruleFile << "Applying Rule 65\n";
			token = lex->GetToken();
			break;
		case NULLP_T:
			ruleFile << "Applying Rule 66\n";
			token = lex->GetToken();
			break;
		case STRINGP_T:
			ruleFile << "Applying Rule 67\n";
			token = lex->GetToken();
			break;
		case PLUS_T:
			ruleFile << "Applying Rule 68\n";
			token = lex->GetToken();
			break;
		case MINUS_T:
			ruleFile << "Applying Rule 69\n";
			token = lex->GetToken();
			break;
		case DIV_T:
			ruleFile << "Applying Rule 70\n";
			token = lex->GetToken();
			break;
		case MULT_T:
			ruleFile << "Applying Rule 71\n";
			token = lex->GetToken();
			break;
		case MODULO_T:
			ruleFile << "Applying Rule 72\n";
			token = lex->GetToken();
			break;
		case ROUND_T:
			ruleFile << "Applying Rule 73\n";
			token = lex->GetToken();
			break;
		case EQUALTO_T:
			ruleFile << "Applying Rule 74\n";
			token = lex->GetToken();
			break;
		case GT_T:
			ruleFile << "Applying Rule 75\n";
			token = lex->GetToken();
			break;
		case LT_T:
			ruleFile << "Applying Rule 76\n";
			token = lex->GetToken();
			break;
		case GTE_T:
			ruleFile << "Applying Rule 77\n";
			token = lex->GetToken();
			break;
		case LTE_T:
			ruleFile << "Applying Rule 78\n";
			token = lex->GetToken();
			break;
		case SQUOTE_T:
			ruleFile << "Applying Rule 79\n";
			token = lex->GetToken();
			errors += any_other_token();
			break;
		case COND_T:
			ruleFile << "Applying Rule 80\n";
			token = lex->GetToken();
			break;
		case ELSE_T:
			ruleFile << "Applying Rule 81\n";
			token = lex->GetToken();
			break;
		default:
			//no rule found
			errors++;
			ReportError(string("No rule found in any_other_token function, current token is: " + lex->GetTokenName(token)));
			token = lex->GetToken();
	}

	//if the current token at this point is not apart of the follows then that is an error?
	/*
	while(!follows){
		errors++;
		ReportError(string("Expected RPAREN_T, found: " + lex->GetTokenName(token)));
		token = lex->GetToken();	
	}
	*/

	ruleFile << "Exiting any_other_token function; current token is: " << lex->GetTokenName(token) << endl;
	return errors;
}

void SyntacticalAnalyzer::ReportError (const string & msg){
  lex->listing << "Syntax error at " << lex->get_line_num() << ',' << lex->get_pos() << ": " << msg << endl;
  lex->debug << "Syntax error at " << lex->get_line_num() << ',' << lex->get_pos() << ": " << msg << endl;

}

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
	token = lex -> GetToken();
	input.open(filename);
	listingFile.open("P2-1.lst");
	ruleFile.open("P2-1.p2");
	debugFile.open("P2-1.dbg");
	listingFile << "Input file: " << filename << endl;
	// GetLexeme()?
	// program ();
	
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{//TODO: close output
	delete lex;
	input.close();
	listingFile.close();
	ruleFile.close();
	debugFile.close();
}

int SyntacticalAnalyzer::program(){
	// Take in input file and applies rule 1
	
	input  << "Current token is: " << lex ->GetTokenName (token) <<  endl; 
	int errors = 0;
	input << "Applying Rule 1" << endl;
	errors += define();
	errors += more_defines();
	if (token != EOF_T) {
		errors ++;
		// lex -> ("Unable to find end of file token");

	} // end of if EOF
	input << "Function complete, current token is: " << lex ->GetTokenName (token) << endl;
	return errors;

}

int SyntacticalAnalyzer::more_defines(){

	input << "Starting more_defines function. Current token is: " << lex ->GetTokenName(token) << endl;
	int errors = 0;
	if(token == LPAREN_T){
		input << "Applying Rule 3" << endl;
		errors += define();
		errors += more_defines();
		input << "more_defines function complete. Current token is: " << lex ->GetTokenName (token) << endl; 
		return errors; 


	} // end left paren. if 

	input << "Applying Rule 4" << endl;
	input << "more_defines function complete. Current token is: " << lex ->GetTokenName (token) << endl;
	return errors;

}			

int SyntacticalAnalyzer::define(){

	input << "Start of  define. Current token is: " << lex->GetTokenName(token) <<  endl;

	int errors = 0;
	
	input << " Applying  Rule 2" << endl;

    if(token != LPAREN_T){
        errors++;
    }

    token = lex->GetToken();
    if(token != DEFINE_T){
        errors++;
    }

    token = lex->GetToken();
    if(token != LPAREN_T){
        errors++;
    }

    token = lex->GetToken();
    if(token != IDENT_T){
        errors++;
    }

    token = lex->GetToken();
    errors += param_list();


    if(token != RPAREN_T){
        errors++;
    }

    token = lex->GetToken();
    errors += stmt();
    errors += stmt_list();

    token = lex->GetToken();
    if(token != RPAREN_T){
        errors++;
    }

   input << "Define function complete. Current token is: " << lex->GetTokenName (token) << endl;

    return errors;

}

int SyntacticalAnalyzer::stmt_list(){ //
	input << "Starting stmt_List function. Current token is: " << lex->GetTokenName (token) << endl;

	int errors = 0;

	if(token == RPAREN_T) {
		input << "Applying Rule 6" << endl;
		input << "stmt_list function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	} // End of Right paren if 
	input << "Applying Rule 5" << endl;
	errors += stmt();
	errors += stmt_list();
	input << "stmt_list function complete. Current token is: " << lex ->GetTokenName(token) << endl;
	return errors;

}
				
int SyntacticalAnalyzer::stmt(){

	input << "Starting stmt function. Current token is: " << lex->GetTokenName (token) << endl;
    
	int errors = 0;
	if(token == IDENT_T) {
		input << "Applying Rule 8" << endl;
		token = lex->GetToken();
		input << "stmt function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}

	if(token == LPAREN_T) {
		input << "Applying Rule 9" << endl;
		token = lex->GetToken();
		// errors += Action();
		while(token != RPAREN_T && token != EOF_T) {

			errors++;
			//lex->ReportError ("Missing ) after action");
			//exit(1);;
			token = lex->GetToken();
		}
		token = lex->GetToken();
	} else {
		input << "Applying Rule 7" << endl;
		errors += literal();
	}
	input << "stmt function complete. Current token is: "
			<< lex->GetTokenName(token) << endl;
	return errors;

}
							
int SyntacticalAnalyzer::literal(){

}
						
int SyntacticalAnalyzer::more_tokens(){

input << "Entering More_Tokens function; current token is: " << lex->GetTokenName(token) << endl;
    
	int errors = 0;
	if (token == EOF_T){
		errors++;
		// report error
		input << "more_tokens function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if (token == RPAREN_T) {
		input << "Applying Rule 15" << endl;
		input << "more_tokens function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	input << "Applying Rule 14" << endl;
	errors += any_other_token();
	errors += more_tokens();
	input << "more_tokens function complete. Current token is: " << lex->GetTokenName(token) << endl;
	return errors;

}
					
int SyntacticalAnalyzer::quoted_lit(){

	int errors = 0;
	input << "Starting quoted_lit function. Current token is: " << lex->GetTokenName(token) << endl;
	
	input << "Applying Rule 13" << endl;
	errors += any_other_token();
	input << "quoted_lit function complete. Current token is: " << lex->GetTokenName(token) << endl;
	return errors;

}			
					
int SyntacticalAnalyzer::param_list(){

	input << " param_list function complete. Current token is: " << lex->GetTokenName(token) << endl;
	int errors = 0;
    
    if(token == RPAREN_T) {
        input << "Applying Rule 17" << endl;
        
        input << "param_list function complete. Current token is: " << lex->GetTokenName (token) << endl;
        return errors;
    } else {
        while(token != IDENT_T) {
            errors++;
           // lex->ReportError ("Error in P List Missing Ident");
            //exit(1);
            token = lex->GetToken();
        }
        input << "Applying Rule 16" << endl;
        token = lex->GetToken();
        errors += param_list();
    }
    
    input << "param_list function complete. Current token is: " << lex->GetTokenName(token) << endl;
    return errors;

}
					
int SyntacticalAnalyzer::else_part(){

input << "Starting else_part function. Current token is: " << lex->GetTokenName(token) << endl;
    
	int errors = 0;
	if (token == EOF_T) {
		errors++;
		input << "else_part function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	} // end EOF if 
	if (token == RPAREN_T) {
		input << "Applying Rule 19" << endl;
		input << "else_part function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	} // end Right paren if  

	input << "Applying Rule 18" << endl;
	errors += stmt();
	input << "else_part function complete. Current token is: " << lex->GetTokenName(token) << endl;
	return errors;
}
					
int SyntacticalAnalyzer::stmt_pair(){

	input << "Entering Stmt_Pair function; current token is: " << lex->GetTokenName(token) << endl;
    
	int errors = 0;
	while (token != LPAREN_T){
		errors ++;
		//lex->ReportError("Giggity Broken ");
		//exit(1);;
		token = lex->GetToken();	
	} // end left paren if
	token = lex->GetToken();
	input << "Applying Rule 20" << endl;
	errors	+= stmt_pair_body();
	while (token != RPAREN_T){
		errors++;
		//lex->ReportError("It done broke! ");
		//exit(1);;
		token = lex->GetToken();
	} // end right paren if 
	token = lex->GetToken();
	input << "stmt_pair function complete. Current token is: " << lex->GetTokenName(token) << endl;
	return errors;


}
					
int SyntacticalAnalyzer::stmt_pair_body(){

	input << "Starting stmt_pair_body function. Current token is: " << lex->GetTokenName(token)<< endl;
	int errors = 0;
    
    if(token == LPAREN_T) {
        input << "Applying Rule 21" << endl;
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
    } // end left paren if

    if(token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T) {
        input << "Applying Rule 23" << endl;
        errors += literal();
    }else{
        input << "Applying Rule 22" << endl;
        errors += action();
    } // end literal and action if else
    input << "Exiting Stmt_Pair_Body function; current token is: " << lex->GetTokenName(token) << endl;
    return errors;

}
					
int SyntacticalAnalyzer::action(){

}
						
int SyntacticalAnalyzer::any_other_token(){

}

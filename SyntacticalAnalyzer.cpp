#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;


/// The GetTokenName() Function is given to us by Watts in her lexical.cpp

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	lex = new LexicalAnalyzer (filename);
	token_type t;
	inputP2file.open();
	token = lex -> GetToken();
	// GetLexeme()?
	// program ();
	
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
	inputP2file.close();
}

int SyntacticalAnalyzer::program(){
	// Take in input file and applies rule 1
	
	inputP2file  << "Current token is: " << lex ->GetTokenName (token) <<  endl; 
	int errors = 0;
	inputP2file << "Applying Rule 1" << endl;
	errors += define();
	errors += more_defines();
	if (token != EOF_T) {
		errors ++;
		// lex -> ("Unable to find end of file token");

	} // end of if EOF
	inputP2file << "Function complete, current token is: " << lex ->GetTokenName (token) << endl;
	return errors;

}

int SyntacticalAnalyzer::more_defines(){

	inputP2file << "Starting more_defines function. Current token is: " << lex ->GetTokenName(token) << endl;
	int errors = 0;
	if(token == LPAREN_T){
		inputP2file << "Applying Rule 3" << endl;
		errors += define();
		errors += more_defines();
		inputP2file << "more_defines function complete. Current token is: " << lex ->GetTokenName (token) << endl; 
		return errors; 


	} // end left paren. if 

	inputP2file << "Applying Rule 4" << endl;
	inputP2file << "more_defines function complete. Current token is: " << lex ->GetTokenName (token) << endl;
	return errors;

}			

int SyntacticalAnalyzer::define(){

	inputP2file << "Start of  define. Current token is: " << lex->GetTokenName(token) <<  endl;

	int errors = 0;
	
	inputP2file << " Applying  Rule 2" << endl;

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

   inputP2file << "Define function complete. Current token is: " << lex->GetTokenName (token) << endl;

    return errors;

}

int SyntacticalAnalyzer::stmt_list(){
	inputP2file << "Starting stmt_List function. Current token is: " << lex->GetTokenName (token) << endl;

	int errors = 0;

	if(token == RPAREN_T) {
		inputP2file << "Applying Rule 6" << endl;
		inputP2file << "stmt_list function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	} // End of Right paren if 
	inputP2file << "Applying Rule 5" << endl;
	errors += stmt();
	errors += stmt_list();
	inputP2file << "stmt_list function complete. Current token is: " << lex ->GetTokenName(token) << endl;
	return errors;

}
				
int SyntacticalAnalyzer::stmt(){

	inputP2file << "Starting stmt function. Current token is: " << lex->GetTokenName (token) << endl;
    
	int errors = 0;
	if(token == IDENT_T) {
		inputP2file << "Applying Rule 8" << endl;
		token = lex->GetToken();
		inputP2file << "stmt function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}

	if(token == LPAREN_T) {
		inputP2file << "Applying Rule 9" << endl;
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
		inputP2file << "Applying Rule 7" << endl;
		errors += literal();
	}
	inputP2file << "stmt function complete. Current token is: "
			<< lex->GetTokenName(token) << endl;
	return errors;

}
							
int SyntacticalAnalyzer::literal(){

}
						
int SyntacticalAnalyzer::more_tokens(){

inputP2file << "Entering More_Tokens function; current token is: " << lex->GetTokenName(token) << endl;
    
	int errors = 0;
	if (token == EOF_T){
		errors++;
		// report error
		inputP2file << "more_tokens function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	if (token == RPAREN_T) {
		inputP2file << "Applying Rule 15" << endl;
		inputP2file << "more_tokens function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	inputP2file << "Applying Rule 14" << endl;
	errors += any_other_token();
	errors += more_tokens();
	inputP2file << "more_tokens function complete. Current token is: " << lex->GetTokenName(token) << endl;
	return errors;

}
					
int SyntacticalAnalyzer::quoted_lit(){

	int errors = 0;
	inputP2file << "Starting quoted_lit function. Current token is: " << lex->GetTokenName(token) << endl;
	
	inputP2file << "Applying Rule 13" << endl;
	errors += any_other_token();
	inputP2file << "quoted_lit function complete. Current token is: " << lex->GetTokenName(token) << endl;
	return errors;

}			
					
int SyntacticalAnalyzer::param_list(){

	inputP2file << " param_list function complete. Current token is: " << lex->GetTokenName(token) << endl;
	int errors = 0;
    
    if(token == RPAREN_T) {
        inputP2file << "Applying Rule 17" << endl;
        
        inputP2file << "param_list function complete. Current token is: " << lex->GetTokenName (token) << endl;
        return errors;
    } else {
        while(token != IDENT_T) {
            errors++;
           // lex->ReportError ("Error in P List Missing Ident");
            //exit(1);
            token = lex->GetToken();
        }
        inputP2file << "Applying Rule 16" << endl;
        token = lex->GetToken();
        errors += param_list();
    }
    
    inputP2file << "param_list function complete. Current token is: " << lex->GetTokenName(token) << endl;
    return errors;

}
					
int SyntacticalAnalyzer::else_part(){

inputP2file << "Starting else_part function. Current token is: " << lex->GetTokenName(token) << endl;
    
	int errors = 0;
	if (token == EOF_T) {
		errors++;
		inputP2file << "else_part function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	} // end EOF if 
	if (token == RPAREN_T) {
		inputP2file << "Applying Rule 19" << endl;
		inputP2file << "else_part function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	} // end Right paren if  

	inputP2file << "Applying Rule 18" << endl;
	errors += stmt();
	inputP2file << "else_part function complete. Current token is: " << lex->GetTokenName(token) << endl;
	return errors;
}
					
int SyntacticalAnalyzer::stmt_pair(){

	inputP2file << "Entering Stmt_Pair function; current token is: " << lex->GetTokenName(token) << endl;
    
	int errors = 0;
	while (token != LPAREN_T){
		errors ++;
		//lex->ReportError("Giggity Broken ");
		//exit(1);;
		token = lex->GetToken();	
	} // end left paren if
	token = lex->GetToken();
	inputP2file << "Applying Rule 20" << endl;
	errors	+= stmt_pair_body();
	while (token != RPAREN_T){
		errors++;
		//lex->ReportError("It done broke! ");
		//exit(1);;
		token = lex->GetToken();
	} // end right paren if 
	token = lex->GetToken();
	inputP2file << "stmt_pair function complete. Current token is: " << lex->GetTokenName(token) << endl;
	return errors;


}
					
int SyntacticalAnalyzer::stmt_pair_body(){

	inputP2file << "Starting stmt_pair_body function. Current token is: " << lex->GetTokenName(token)<< endl;
	int errors = 0;
    
    if(token == LPAREN_T) {
        inputP2file << "Applying Rule 21" << endl;
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
        inputP2file << "Applying Rule 23" << endl;
        errors += literal();
    }else{
        inputP2file << "Applying Rule 22" << endl;
        errors += action();
    } // end literal and action if else
    inputP2file << "Exiting Stmt_Pair_Body function; current token is: " << lex->GetTokenName(token) << endl;
    return errors;

}
					
int SyntacticalAnalyzer::action(){

}
						
int SyntacticalAnalyzer::any_other_token(){

}

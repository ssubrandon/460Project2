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

	if(token != LPAREN_T){
	  errors++;
	}
	token = lex->GetToken();
	errors += define();

	if(token != LPAREN_T){
	  errors++;
	}

	token = lex->GetToken();
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
	if(token == IDENT_T){
		inputP2file << "Applying Rule 3" << endl;
		errors += stmt_list();
		if(token != RPAREN_T){
		  errors++;
		}
		token = lex->GetToken();
		inputP2file << "more_defines function complete. Current token is: " << lex ->GetTokenName (token) << endl; 
		return errors; 


	} // end left paren. if 

	inputP2file << "Applying Rule 2" << endl;
	errors += define();

	if(token != LPAREN_T){
	  errors++;
	}
	token = lex->GetToken();
	
	errors+= more_defines();
	inputP2file << "more_defines function complete. Current token is: " << lex ->GetTokenName (token) << endl;
	return errors;

}			

int SyntacticalAnalyzer::define(){

	inputP2file << "Start of  define. Current token is: " << lex->GetTokenName(token) <<  endl;

	int errors = 0;
	
	inputP2file << " Applying  Rule 2" << endl;

    
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

    
    if(token != RPAREN_T){
        errors++;
    }
    token = lex->GetToken();
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
	  errors += action();
	 
	} else {
		inputP2file << "Applying Rule 7" << endl;
		errors += literal();
	}
	inputP2file << "stmt function complete. Current token is: "
			<< lex->GetTokenName(token) << endl;
	return errors;

}
							
int SyntacticalAnalyzer::literal(){
  inputP2file << "Entering literal function; current token is: " << lex->GetTokenName(token) << endl;
  int errors =0;
  if (token != SQUOTE_T && token != NUMLIT_T && token != STRLIT_T){
      errors ++;
      token = lex->GetToken();
  }
  if ( token == SQUOTE_T )
    {
      //    "Applying rule 12"
     token = lex->GetToken();
     errors += quoted_lit(); 
    }
  inputP2file << "literal function complete. Current token is: " << lex->GetTokenName(token) << endl;
  return errors;
}
						
int SyntacticalAnalyzer::more_tokens(){

inputP2file << "Entering More_Tokens function; current token is: " << lex->GetTokenName(token) << endl;
    
	int errors = 0;
	/*
	if (token == EOF_T){
		errors++;
		// report error
		inputP2file << "more_tokens function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	*/
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
  }
  
  while(token != IDENT_T) {
    errors++;
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
  if(token != LPAREN_T){
    errors ++;
    //lex->ReportError("Giggity Broken ");
    //exit(1);;
    token = lex->GetToken();	
  } // end left paren if
  token = lex->GetToken();
  inputP2file << "Applying Rule 20" << endl;
  errors += stmt_pair_body();
  while(token != RPAREN_T){
    errors++;
    //lex->ReportError("It done broke! ");
    //exit(1);
    token = lex->GetToken();
  } // end right paren if 
  token = lex->GetToken();
  inputP2file << "stmt_pair function complete. Current token is: " << lex->GetTokenName(token) << endl;
  return errors;
  
  
}
					
int SyntacticalAnalyzer::stmt_pair_body(){

  inputP2file << "Starting stmt_pair_body function. Current token is: " << lex->GetTokenName(token)<< endl;
  int errors = 0;
  if (token == ELSE_T){
    //rule 23
    token = lex->GetToken();
    errors += stmt();
    if (token != RPAREN_T)
      errors++;
   
  }
  
  if(token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T ||token == IDENT_T || token== LPAREN_T)) {
  //rule 22
    errors += stmt();
    erros += stmt();
    if (token != RPAREN_T){
      errors++;
    }
    token = lex->GetToken();
    errors += stmt_pair();
  }


  
  /*
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
}
  // end left paren if
  
  if(token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T) {
    inputP2file << "Applying Rule 23" << endl;
    errors += literal();
  }
  else{
    inputP2file << "Applying Rule 22" << endl;
    errors += action();
  } // end literal and action if else
  
  */inputP2file << "Exiting Stmt_Pair_Body function; current token is: " << lex->GetTokenName(token) << endl;
  return errors;

}
					
int SyntacticalAnalyzer::action(){
  errors = 0;
  if (token == IF_T)
    {//applying rule 24
      outputfileP2 << "Applying Rule 24" << endl;
      token = lex->GetToken();
      errors += stmt();
      errors += stmt();
      errors+= else_part();
      outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
      return errors;
    }
   if (token == COND_T)
    {//applying rule 25
       outputfileP2 << "Applying Rule 25" << endl;
      token = lex->GetToken();
      if (token != LPRARENT_T)
	  errors++;
      token = lex->GetToken();
      errors+= stmt_pair_body();
       outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
      return errors;
    }
   if ( token == LISTOP_T){
     //RULE 26
      outputfileP2 << "Applying Rule 26" << endl;
     errors += stmt();
      outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == CONS_T){
     //RULE 27
     outputfileP2 << "Applying Rule 27" << endl;
     token = lex->GetToken();
     errors += stmt();
     errors += stmt();
      outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == AND_T){
     //RULE 28
     outputfileP2 << "Applying Rule 28" << endl;
     token = lex->GetToken();
     errors += stmt();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == OR_T){
     //RULE 29
     outputfileP2 << "Applying Rule 29" << endl;
     token = lex->GetToken();
     errors += stmt();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == NOT_T){
     //RULE 30
     outputfileP2 << "Applying Rule 30" << endl;
     token = lex->GetToken();
     errors += stmt();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == NUMBERP_T){
     //RULE 31
     outputfileP2 << "Applying Rule 31" << endl;
     token = lex->GetToken();
     errors += stmt();
      outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == LISTP_T){
     //RULE 32
     outputfileP2 << "Applying Rule 32" << endl;
     token = lex->GetToken();
     errors += stmt();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == ZEROP_T){
     //RULE 33
     outputfileP2 << "Applying Rule 33" << endl;
     token = lex->GetToken();
     errors += stmt();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == NULLP_T){
     //RULE 34
     outputfileP2 << "Applying Rule 34" << endl;
     token = lex->GetToken();
     errors += stmt();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == STRINGP_T){
     //RULE 35
     outputfileP2 << "Applying Rule 35" << endl;
     token = lex->GetToken();
     errors += stmt();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == PLUS_T){
     //RULE 36
     outputfileP2 << "Applying Rule 36" << endl;
     token = lex->GetToken();
     errors += stmt_list();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == MINUS_T){
     //RULE 37
     outputfileP2 << "Applying Rule 37" << endl;
     token = lex->GetToken();
     errors += stmt();
     errors += stmt_list();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == DIV_T){
     //RULE 38
     outputfileP2 << "Applying Rule 38" << endl;
     token = lex->GetToken();
     errors += stmt();
     errors += stmt_list();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == MULT_T){
     //RULE 39
     outputfileP2 << "Applying Rule 39" << endl;
     token = lex->GetToken();
     errors += stmt_list();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == MODULO_T){
     //RULE 40
     outputfileP2 << "Applying Rule 40" << endl;
     token = lex->GetToken();
     errors += stmt();
     errors += stmt();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == ROUND_T){
     //RULE 41
     outputfileP2 << "Applying Rule 41" << endl;
     token = lex->GetToken();
     errors += stmt();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == EQUALTO_T){
     //RULE 42
     outputfileP2 << "Applying Rule 42" << endl;
     token = lex->GetToken();
     errors += stmt_list();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == GT_T){
     //RULE 43
     outputfileP2 << "Applying Rule 43" << endl;
     token = lex->GetToken();
     errors += stmt_list();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == LT_T){
     //RULE 44
     outputfileP2 << "Applying Rule 44" << endl;
     token = lex->GetToken();
     errors += stmt_list();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == GTE_T){
     //RULE 45
     outputfileP2 << "Applying Rule 45" << endl;
     token = lex->GetToken();
     errors += stmt_list();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == LTE_T){
     //RULE 46
     outputfileP2 << "Applying Rule 46" << endl;
     token = lex->GetToken();
     errors += stmt_list();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == IDENT_T){
     //RULE 47
     outputfileP2 << "Applying Rule 47" << endl;
     token = lex->GetToken();
     errors += stmt_list();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == GTE_T){
     //RULE 48
     outputfileP2 << "Applying Rule 48" << endl;
     token = lex->GetToken();
     errors += stmt();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == GTE_T){
     //RULE 49
     outputfileP2 << "Applying Rule 49" << endl;
     token = lex->GetToken();
     outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   //no rule found
   outputfileP2 << "error in actrion" << endl;
   errors++;
   token = lex->GetToken();
   outputfileP2 << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
   return errors;

}
						
int SyntacticalAnalyzer::any_other_token(){

}

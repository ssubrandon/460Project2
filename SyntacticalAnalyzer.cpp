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
	int i =0;
	string file = "";
	while( filename[i] != '.'){
	  file += filename[i];
	}
	ruleFile.open(file + "_rule.txt");
	listingFile.open(file +".lst");
	debugFile.open(file + ".dbg"); 
	token = lex -> GetToken();
	// GetLexeme()?
	// program ();
	
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
	listingFile.close();
}

int SyntacticalAnalyzer::program(){
	// Take in input file and applies rule 1
  
	listingFile  << "Current token is: " << lex ->GetTokenName (token) <<  endl; 
	int errors = 0;
	listingFile << "Applying Rule 1" << endl;

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
	listingFile << "Function complete, current token is: " << lex ->GetTokenName (token) << endl;
	return errors;

}

int SyntacticalAnalyzer::more_defines(){

	listingFile << "Starting more_defines function. Current token is: " << lex ->GetTokenName(token) << endl;
	int errors = 0;
	if(token == IDENT_T){
		listingFile << "Applying Rule 3" << endl;
		errors += stmt_list();
		if(token != RPAREN_T){
		  errors++;
		}
		token = lex->GetToken();
		listingFile << "more_defines function complete. Current token is: " << lex ->GetTokenName (token) << endl; 
		return errors; 


	} // end left paren. if 

	listingFile << "Applying Rule 2" << endl;
	errors += define();

	if(token != LPAREN_T){
	  errors++;
	}
	token = lex->GetToken();
	
	errors+= more_defines();
	listingFile << "more_defines function complete. Current token is: " << lex ->GetTokenName (token) << endl;
	return errors;

}			

int SyntacticalAnalyzer::define(){

	listingFile << "Start of  define. Current token is: " << lex->GetTokenName(token) <<  endl;

	int errors = 0;
	
	listingFile << " Applying  Rule 2" << endl;

    
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
   listingFile << "Define function complete. Current token is: " << lex->GetTokenName (token) << endl;

    return errors;

}

int SyntacticalAnalyzer::stmt_list(){
	listingFile << "Starting stmt_List function. Current token is: " << lex->GetTokenName (token) << endl;

	int errors = 0;

	if(token == RPAREN_T) {
		listingFile << "Applying Rule 6" << endl;
		listingFile << "stmt_list function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	} // End of Right paren if 
	listingFile << "Applying Rule 5" << endl;
	errors += stmt();
	errors += stmt_list();
	listingFile << "stmt_list function complete. Current token is: " << lex ->GetTokenName(token) << endl;
	return errors;

}
				
int SyntacticalAnalyzer::stmt(){

	listingFile << "Starting stmt function. Current token is: " << lex->GetTokenName (token) << endl;
    
	int errors = 0;
	if(token == IDENT_T) {
		listingFile << "Applying Rule 8" << endl;
		token = lex->GetToken();
		listingFile << "stmt function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}

	if(token == LPAREN_T) {
	  listingFile << "Applying Rule 9" << endl;
	  token = lex->GetToken();
	  errors += action();
	 
	}
	else{
	    listingFile << "Applying Rule 7" << endl;
	  errors += literal();
	}
	listingFile << "stmt function complete. Current token is: "
			<< lex->GetTokenName(token) << endl;
	return errors;

}
							
int SyntacticalAnalyzer::literal(){
  listingFile << "Entering literal function; current token is: " << lex->GetTokenName(token) << endl;
  int errors =0;
  if (token != SQUOTE_T && token != NUMLIT_T && token != STRLIT_T){
      errors ++;
      token = lex->GetToken();
  }
  if (token == NUMLIT_T)
    { 
       listingFile << "Applying Rule 11" << endl;
    }
    if (token == STRLIT_T)
    {
      listingFile << "Applying Rule 12" << endl;
    }
  if ( token == SQUOTE_T )
    {
      //    "Applying rule 12"
     token = lex->GetToken();
     errors += quoted_lit(); 
    }
  listingFile << "literal function complete. Current token is: " << lex->GetTokenName(token) << endl;
  return errors;
}
						
int SyntacticalAnalyzer::more_tokens(){

listingFile << "Entering More_Tokens function; current token is: " << lex->GetTokenName(token) << endl;
    
	int errors = 0;
	/*
	if (token == EOF_T){
		errors++;
		// report error
		listingFile << "more_tokens function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	*/
	if (token == RPAREN_T) {
		listingFile << "Applying Rule 15" << endl;
		listingFile << "more_tokens function complete. Current token is: " << lex->GetTokenName(token) << endl;
		return errors;
	}
	listingFile << "Applying Rule 14" << endl;
	errors += any_other_token();
	errors += more_tokens();
	listingFile << "more_tokens function complete. Current token is: " << lex->GetTokenName(token) << endl;
	return errors;

}
					
int SyntacticalAnalyzer::quoted_lit(){

	int errors = 0;
	listingFile << "Starting quoted_lit function. Current token is: " << lex->GetTokenName(token) << endl;
	
	listingFile << "Applying Rule 13" << endl;
	errors += any_other_token();
	listingFile << "quoted_lit function complete. Current token is: " << lex->GetTokenName(token) << endl;
	return errors;

}			
					
int SyntacticalAnalyzer::param_list(){

  //  listingFile << " param_list function complete. Current token is: " << lex->GetTokenName(token) << endl;
  int errors = 0;
    
  if(token == RPAREN_T) {
    listingFile << "Applying Rule 17" << endl;    
    listingFile << "param_list function complete. Current token is: " << lex->GetTokenName (token) << endl;
    return errors;
  }
  else{ 
    while(token != IDENT_T) {
      errors++;
      token = lex->GetToken();
    }
    listingFile << "Applying Rule 16" << endl;
    token = lex->GetToken();
    errors += param_list();
  }
 
  //listingFile << "param_list function complete. Current token is: " << lex->GetTokenName(token) << endl;
  return errors;
}

					
int SyntacticalAnalyzer::else_part(){

  listingFile << "Starting else_part function. Current token is: " << lex->GetTokenName(token) << endl;
  
  int errors = 0;
  if (token == EOF_T) {
    errors++;
    listingFile << "else_part function complete. Current token is: " << lex->GetTokenName(token) << endl;
    return errors;
  } // end EOF if 
  if (token == RPAREN_T) {
    listingFile << "Applying Rule 19" << endl;
    listingFile << "else_part function complete. Current token is: " << lex->GetTokenName(token) << endl;
    return errors;
  } // end Right paren if  
  listingFile << "Applying Rule 18" << endl;
  errors += stmt();
  listingFile << "else_part function complete. Current token is: " << lex->GetTokenName(token) << endl;
  return errors;
}
					
int SyntacticalAnalyzer::stmt_pair(){
  
  listingFile << "Entering Stmt_Pair function; current token is: " << lex->GetTokenName(token) << endl;
  
  int errors = 0;
  if(token != LPAREN_T){
    errors ++;
    //lex->ReportError("Giggity Broken ");
    //exit(1);;
    token = lex->GetToken();	
  } // end left paren if
  token = lex->GetToken();
  listingFile << "Applying Rule 20" << endl;
  errors += stmt_pair_body();
  while(token != RPAREN_T){
    errors++;
    //lex->ReportError("It done broke! ");
    //exit(1);
    token = lex->GetToken();
  } // end right paren if 
  token = lex->GetToken();
  listingFile << "stmt_pair function complete. Current token is: " << lex->GetTokenName(token) << endl;
  return errors;
  
  
}
					
int SyntacticalAnalyzer::stmt_pair_body(){

  listingFile << "Starting stmt_pair_body function. Current token is: " << lex->GetTokenName(token)<< endl;
  int errors = 0;
  if (token == ELSE_T){
    //rule 23
    token = lex->GetToken();
    errors += stmt();
    if (token != RPAREN_T)
      errors++;
   
  }
  
  if(token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T ||token == IDENT_T || token== LPAREN_T) {
  //rule 22
    errors += stmt();
    errors += stmt();
    if (token != RPAREN_T){
      errors++;
    }
    token = lex->GetToken();
    errors += stmt_pair();
  }


  
  /*
   if(token == LPAREN_T) {
        listingFile << "Applying Rule 21" << endl;
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
    listingFile << "Applying Rule 23" << endl;
    errors += literal();
  }
  else{
    listingFile << "Applying Rule 22" << endl;
    errors += action();
  } // end literal and action if else
  
  */listingFile << "Exiting Stmt_Pair_Body function; current token is: " << lex->GetTokenName(token) << endl;
  return errors;

}
					
int SyntacticalAnalyzer::action(){
 int errors = 0;
  if (token == IF_T)
    {//applying rule 24
      listingFile << "Applying Rule 24" << endl;
      token = lex->GetToken();
      errors += stmt();
      errors += stmt();
      errors+= else_part();
      listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
      return errors;
    }
   if (token == COND_T)
    {//applying rule 25
       listingFile << "Applying Rule 25" << endl;
      token = lex->GetToken();
      if (token != LPAREN_T)
	  errors++;
      token = lex->GetToken();
      errors+= stmt_pair_body();
       listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
      return errors;
    }
   if ( token == LISTOP_T){
     //RULE 26
      listingFile << "Applying Rule 26" << endl;
     errors += stmt();
      listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == CONS_T){
     //RULE 27
     listingFile << "Applying Rule 27" << endl;
     token = lex->GetToken();
     errors += stmt();
     errors += stmt();
      listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == AND_T){
     //RULE 28
     listingFile << "Applying Rule 28" << endl;
     token = lex->GetToken();
     errors += stmt();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == OR_T){
     //RULE 29
     listingFile << "Applying Rule 29" << endl;
     token = lex->GetToken();
     errors += stmt();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == NOT_T){
     //RULE 30
     listingFile << "Applying Rule 30" << endl;
     token = lex->GetToken();
     errors += stmt();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == NUMBERP_T){
     //RULE 31
     listingFile << "Applying Rule 31" << endl;
     token = lex->GetToken();
     errors += stmt();
      listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == LISTP_T){
     //RULE 32
     listingFile << "Applying Rule 32" << endl;
     token = lex->GetToken();
     errors += stmt();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == ZEROP_T){
     //RULE 33
     listingFile << "Applying Rule 33" << endl;
     token = lex->GetToken();
     errors += stmt();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == NULLP_T){
     //RULE 34
     listingFile << "Applying Rule 34" << endl;
     token = lex->GetToken();
     errors += stmt();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == STRINGP_T){
     //RULE 35
     listingFile << "Applying Rule 35" << endl;
     token = lex->GetToken();
     errors += stmt();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == PLUS_T){
     //RULE 36
     listingFile << "Applying Rule 36" << endl;
     token = lex->GetToken();
     errors += stmt_list();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == MINUS_T){
     //RULE 37
     listingFile << "Applying Rule 37" << endl;
     token = lex->GetToken();
     errors += stmt();
     errors += stmt_list();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == DIV_T){
     //RULE 38
     listingFile << "Applying Rule 38" << endl;
     token = lex->GetToken();
     errors += stmt();
     errors += stmt_list();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == MULT_T){
     //RULE 39
     listingFile << "Applying Rule 39" << endl;
     token = lex->GetToken();
     errors += stmt_list();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == MODULO_T){
     //RULE 40
     listingFile << "Applying Rule 40" << endl;
     token = lex->GetToken();
     errors += stmt();
     errors += stmt();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == ROUND_T){
     //RULE 41
     listingFile << "Applying Rule 41" << endl;
     token = lex->GetToken();
     errors += stmt();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == EQUALTO_T){
     //RULE 42
     listingFile << "Applying Rule 42" << endl;
     token = lex->GetToken();
     errors += stmt_list();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == GT_T){
     //RULE 43
     listingFile << "Applying Rule 43" << endl;
     token = lex->GetToken();
     errors += stmt_list();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == LT_T){
     //RULE 44
     listingFile << "Applying Rule 44" << endl;
     token = lex->GetToken();
     errors += stmt_list();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == GTE_T){
     //RULE 45
     listingFile << "Applying Rule 45" << endl;
     token = lex->GetToken();
     errors += stmt_list();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == LTE_T){
     //RULE 46
     listingFile << "Applying Rule 46" << endl;
     token = lex->GetToken();
     errors += stmt_list();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == IDENT_T){
     //RULE 47
     listingFile << "Applying Rule 47" << endl;
     token = lex->GetToken();
     errors += stmt_list();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == GTE_T){
     //RULE 48
     listingFile << "Applying Rule 48" << endl;
     token = lex->GetToken();
     errors += stmt();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   if ( token == GTE_T){
     //RULE 49
     listingFile << "Applying Rule 49" << endl;
     token = lex->GetToken();
     listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
     return errors;
   }
   //no rule found
   listingFile << "error in actrion" << endl;
   errors++;
   token = lex->GetToken();
   listingFile << "Exiting Action function; current token is: " << lex->GetTokenName(token) << endl;
   return errors;

}
						
int SyntacticalAnalyzer::any_other_token(){

}

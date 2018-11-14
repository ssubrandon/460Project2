/*******************************************************************************
* Assignment: Project 1 - Lexical Analyzer for Scheme to C++ Translator        *
* Author: Dr. Watts                                                            *
* Date: Fall 2018                                                              *
* File: LexicalAnalyzer.cpp                                                    *
*                                                                              *
* Description: This file contains the                                          *
*******************************************************************************/

#include <iomanip>
#include <cstdlib>
#include "LexicalAnalyzer.h"

using namespace std;

/*******************************************************************************
* Variable: token_names                                                        *
*                                                                              *
* Description: The token_names variable is an array of string containing the   *
*              names of the tokens. It should be indexed using an element      *
*              of the token_type enumerate type.                               *
*              This array variable is used by the GetTokenName function.       *
*******************************************************************************/

string token_names[] = {"{}", "IDENT_T", "NUMLIT_T", "STRLIT_T", "CONS_T", "IF_T", "COND_T", 
			"DISPLAY_T", "NEWLINE_T", "LISTOP_T", "AND_T", "OR_T", "NOT_T", 
			"DEFINE_T", "NUMBERP_T", "LISTP_T", "ZEROP_T", "NULLP_T", "STRINGP_T",
			"MODULO_T", "ROUND_T", "ELSE_T", "PLUS_T", "MINUS_T", "DIV_T", "MULT_T", 
			"EQUALTO_T", "GT_T", "LT_T", "GTE_T", "LTE_T", "LPAREN_T", "RPAREN_T",
			"SQUOTE_T", "ERROR_T", "EOF_T"}; 

/*******************************************************************************
* Variable: table                                                              *
*                                                                              *
* Description: The table variable contains the tabular representation of the   *
*              DFA used to identify the lexemes in a file and associate them   *
*              with the approprate element in the token_type enymerated type.  *
*              This 2D-array variable is used by the GetToken function.        *
*******************************************************************************/

static int table [][21] = 
//	  a   c   d   r   b   0   .   (   )   +   -   *   /   '   =   <   >   ?   _   "  other
       {{32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},  // 0 not used
	{ 8,  9,  8,  8,  8,  2,  3, 24, 24,  5,  6, 24, 24, 24, 24,  7,  7, 32, 32, 12, 24},  // 1 starting
	{22, 22, 22, 22, 22,  2,  4, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},  // 2 number
	{31, 31, 31, 31, 31,  4, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31},  // 3 period
	{22, 22, 22, 22, 22,  4, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},  // 4 number after decimal
	{23, 23, 23, 23, 23,  2,  3, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23},  // 5 plus
	{23, 23, 23, 23, 23,  2,  3, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23},  // 6 minus
	{23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 24, 23, 23, 23, 23, 23, 23},  // 7 less than
	{ 8,  8,  8,  8,  8,  8, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 24,  8, 21, 21},  // 8 letter other than c
	{10,  8, 11,  8,  8,  8, 21, 21, 21, 24, 21, 10, 21, 21, 21, 21, 21, 21,  8, 21, 21},  // 9 letter c
	{ 8,  8, 11, 13,  8,  8, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,  8, 21, 21},  // 10 letter a after c
	{ 8,  8, 11, 13,  8,  8, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,  8, 21, 21},  // 11 letter d after c, ca, or cd...
	{12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 25, 12},  // 12 character after "
	{ 8,  8, 11, 13,  8,  8, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,  8, 20, 20}}; // 13 r seen - is it the end of a listop?

/*******************************************************************************
* Function:                                                                    *
*                                                                              *
* Parameters:                                                                  *
* Return value:                                                                *
* Description: This function will                                              *
*******************************************************************************/

LexicalAnalyzer::LexicalAnalyzer (char * filename)
{
	cout << "Input file: " << filename << endl;
	string name = filename;
	string extension;
	if (name.length() > 3)
		extension = name.substr (name.length()-3, 3);
	if (extension != ".ss")
	{
		cout << "Invalid file extension; must be '.ss'\n";
		exit (1);
	}
	input.open (filename);
	if (input.fail())
	{
		cout << "File " << filename << " not found\n";
		exit (2);
	}
	string p1name = name.substr (0, name.length()-3) + ".p1"; 
	p1file.open (p1name.c_str());
	string listingname = name.substr (0, name.length()-3) + ".lst"; 
	listing.open (listingname.c_str());
	listing << "Input file: " << filename << endl;
	string debugname = name.substr (0, name.length()-3) + ".dbg"; 
	debug.open (debugname.c_str());
	debug << "Input file: " << filename << endl;
	line = " ";
	linenum = 0;
	pos = 0;
	lexeme = "";
	errors = 0;

	symbols.insert (pair<string,token_type> ("cons", CONS_T));
	symbols.insert (pair<string,token_type> ("if", IF_T));
	symbols.insert (pair<string,token_type> ("cond", COND_T));
	symbols.insert (pair<string,token_type> ("display", DISPLAY_T));
	symbols.insert (pair<string,token_type> ("newline", NEWLINE_T));
	symbols.insert (pair<string,token_type> ("and", AND_T));
	symbols.insert (pair<string,token_type> ("or", OR_T));
	symbols.insert (pair<string,token_type> ("not", NOT_T));
	symbols.insert (pair<string,token_type> ("define", DEFINE_T));
	symbols.insert (pair<string,token_type> ("number?", NUMBERP_T));
	symbols.insert (pair<string,token_type> ("list?", LISTP_T));
	symbols.insert (pair<string,token_type> ("zero?", ZEROP_T));
	symbols.insert (pair<string,token_type> ("null?", NULLP_T));
	symbols.insert (pair<string,token_type> ("string?", STRINGP_T));
	symbols.insert (pair<string,token_type> ("modulo", MODULO_T));
	symbols.insert (pair<string,token_type> ("round", ROUND_T));
	symbols.insert (pair<string,token_type> ("else", ELSE_T));
	symbols.insert (pair<string,token_type> ("+", PLUS_T));
	symbols.insert (pair<string,token_type> ("-", MINUS_T));
	symbols.insert (pair<string,token_type> ("/", DIV_T));
	symbols.insert (pair<string,token_type> ("*", MULT_T));
	symbols.insert (pair<string,token_type> ("=", EQUALTO_T));
	symbols.insert (pair<string,token_type> (">", GT_T));
	symbols.insert (pair<string,token_type> ("<", LT_T));
	symbols.insert (pair<string,token_type> (">=", GTE_T));
	symbols.insert (pair<string,token_type> ("<=", LTE_T));
	symbols.insert (pair<string,token_type> ("(", LPAREN_T));
	symbols.insert (pair<string,token_type> (")", RPAREN_T));
	symbols.insert (pair<string,token_type> ("'", SQUOTE_T));
}

/*******************************************************************************
* Function:                                                                    *
*                                                                              *
* Parameters:                                                                  *
* Return value:                                                                *
* Description: This function will                                              *
*******************************************************************************/

LexicalAnalyzer::~LexicalAnalyzer ()
{
	input.close ();
	cout << errors << " errors found in input file\n";
	listing << errors << " errors found in input file\n";
	listing.close();
	debug << errors << " errors found in input file\n";
	debug.close();
}

/*******************************************************************************
* Function:                                                                    *
*                                                                              *
* Parameters:                                                                  *
* Return value:                                                                *
* Description: This function will                                              *
*******************************************************************************/

token_type LexicalAnalyzer::GetToken ()
{
	error_type = 1;
	lexeme = "";
	int state = 1;
	token_type token = NONE;
	map <string, token_type>::iterator itr;
	if (input.fail())
	{
		debug << '\t' << setw(16) << left << "EOF_T" << endl;
		p1file << '\t' << setw(16) << left << "EOF_T" << endl;
		return EOF_T;
	}
	static string valid = "acdrb0.()+-*/'=<>?_\"";
	while (isspace(line[pos]) || line[pos] == ';') // end of line or beginning of comment
		if (line[pos] == ';' || ++pos >= line.length())
		{
			getline (input, line);
			if (input.fail())
			{
				debug << '\t' << setw(16) << left << "EOF_T" << endl;
				p1file << '\t' << setw(16) << left << "EOF_T" << endl;
				return EOF_T;
			}
			linenum++;
			listing << setw(4) << right << linenum << ": " << line << endl;	
			debug << setw(4) << right << linenum << ": " << line << endl;	
			line += ' ';
			pos = 0;
		}
	while (token == NONE)
	{
		char c = line[pos++];
		lexeme += c;
		if (state == 12 && pos == line.length())
		{ // used if end of line found in a string literal
			token = ERROR_T;
			error_type = 2;
			break;	
		}
		if (isalpha(c) && (c != 'a' && c != 'c' && c != 'd' && c != 'r')) 
			c = 'b';
		else if (isdigit(c))
			c = '0';
		int col = 0;
		while (col < valid.length() && valid[col] != c)
			col++;
		state = table[state][col];
		switch (state)
		{
		    case 20: // car, cdr, cadr, cddr, and other list operators
			pos--;
			//cout << lexeme << endl;
			lexeme = lexeme.erase(lexeme.length()-1,1);
			//cout << lexeme << endl;
			token = LISTOP_T;
			break;
		    case 21: // symbol or keyword
			pos--;
			lexeme = lexeme.erase(lexeme.length()-1,1);
			itr = symbols.find (lexeme);
			if (itr != symbols.end())
				token = itr->second;
			else 
				token = IDENT_T;
			break;
		    case 22: // numeric literal
			pos--;
			lexeme = lexeme.erase(lexeme.length()-1,1);
			token = NUMLIT_T;
			break;
		    case 23: // operator/symbol with backup
			pos--;
			lexeme = lexeme.erase(lexeme.length()-1,1);
		    case 24: // operator/symbol without backup
			itr = symbols.find (lexeme);
			if (itr != symbols.end())
				token = itr->second;
			else if (lexeme[lexeme.length()-1] == '?') // allow ? at end of identifier
			{
				pos--;
				lexeme = lexeme.erase(lexeme.length()-1,1);
				token = IDENT_T;
			}
			else
				token = ERROR_T;
			break;
		    case 25: // string literal
			token = STRLIT_T;
			break;
		    case 31: // error with backup
			pos--;
			lexeme = lexeme.erase(lexeme.length()-1,1);
		    case 32: // error
			token = ERROR_T;
		}
		if (token == ERROR_T)
			switch (error_type)
			{
				case 1: ReportError (string("Invalid character found: ") + lexeme);
					break;
				case 2: ReportError (string("Unterminated string literal: ") + lexeme);
					break;
				default: ReportError (string("Unidentified error: ") + lexeme);
			}
	}
	debug << '\t' << setw(16) << left << token_names[token] << lexeme << endl;
	p1file << '\t' << setw(16) << left << token_names[token] << lexeme << endl;
	return token;
}

/*******************************************************************************
* Function:                                                                    *
*                                                                              *
* Parameters:                                                                  *
* Return value:                                                                *
* Description: This function will                                              *
*******************************************************************************/

string LexicalAnalyzer::GetTokenName (token_type t) const
{
	return token_names [t];
}

/*******************************************************************************
* Function:                                                                    *
*                                                                              *
* Parameters:                                                                  *
* Return value:                                                                *
* Description: This function will                                              *
*******************************************************************************/

string LexicalAnalyzer::GetLexeme () const
{
	return lexeme;
}

/*******************************************************************************
* Function:                                                                    *
*                                                                              *
* Parameters:                                                                  *
* Return value:                                                                *
* Description: This function will                                              *
*******************************************************************************/

void LexicalAnalyzer::ReportError (const string & msg)
{
	listing << "Error at " << linenum << ',' << pos << ": " << msg << endl;
	debug << "Error at " << linenum << ',' << pos << ": " << msg << endl;
	errors++;
}

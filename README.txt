Assingment: Project2 ( Syntactical Analyzer)
Authors; Caleb, Nick, and Brandon

Description:
	This is project 2 in which we were tasked with making a symantical analyzer that works with the lexical
	analyzer that we constructed in project1. For this assignment we were given a language that we were
	to make a first and follows for to map out the path our program would take to analyze a scheme code
	file and determine if the code is syntactically correct. If there are any errors the program would report
	to the .lst file saying on hwat line and location hwere the error occured. This program also generates
	a .p2 file the contents of this file can be used as a map to trace through the steps that the program
	took when analyzing the code.

What works:
     We believe that most of the rules work as inteded when dealing with a syntactically correct code with some exceptions.

What does not work:
     Unfortunantly we ran into some issues when dealing with code that was not syntactically correct. We had a rough time recovering
     from some errros. We run into a problem in definine specifically where if the code braks and is braught back to that function
     the function cood continuosly loop and cause a seg fault. We in a specific test case that would cause our code to take a strange
     path of functions recursivly, we attempted to error shoot this but could not resolve the issue in time.

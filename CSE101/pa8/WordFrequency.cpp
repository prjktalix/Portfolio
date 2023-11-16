// -----------------------------------------------------------------------------
//  Brian Camilo. bcamilo
//  2023 Winter CSE101 pa8
//  WordFrequency.cpp
//  This file contains code for one of the main program
// -----------------------------------------------------------------------------

#include<iostream>
#include<fstream>
#include<string>
#include "Dictionary.h"

using namespace std;

int main(int argc, char * argv[]){

	// code from FileIO.cpp
	ifstream in;
	ofstream out;
	string line;
	Dictionary d;
	size_t begin, end, len;
	string token;
   	string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$%^&*()-_=+0123456789";

	// check command line for correct number of arguments
	if( argc != 3 ){
		cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
		return(EXIT_FAILURE);
	}

	// open files for reading and writing 
	in.open(argv[1]);
	if( !in.is_open() ){
		cerr << "Unable to open file " << argv[1] << " for reading" << endl;
		return(EXIT_FAILURE);
	}

	out.open(argv[2]);
	if( !out.is_open() ){
		cerr << "Unable to open file " << argv[2] << " for writing" << endl;
		return(EXIT_FAILURE);
	}

	// read each line of a file
	while(getline(in, line)){
		len = line.length();
		
		// get first token
		begin = min(line.find_first_not_of(delim, 0), len);
		end = min(line.find_first_of(delim, begin), len);
		token = line.substr(begin, end-begin);

		//while we stil have a token
		while(token != ""){

			// convert each for to all lower case characters
			for (size_t i = 0; i < token.length(); i++) {
				token[i] = tolower(token[i]);
			}
			if (d.contains(token)) {	// if it exist
				d.setValue(token, d.getValue(token) + 1);
			} else {
				d.setValue(token, 1);
			}	
			// get next token
			begin = min(line.find_first_not_of(delim, end + 1), len);
			end   = min(line.find_first_of(delim, begin), len);
			token = line.substr(begin, end-begin);
		}
	}
	
	out << d << endl;


	in.close();
	out.close();

	return(EXIT_SUCCESS);

	
}

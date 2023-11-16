// -----------------------------------------------------------------------------
//  Brian Camilo. bcamilo
//  2023 Winter CSE101 pa8
//  Order.cpp
//  This file contains code for one of the main program
// -----------------------------------------------------------------------------

#include<iostream>
#include<fstream>
#include<string>
#include "Dictionary.h"

using namespace std;

int main(int argc, char *argv[]){
	
	// code from FileIO.cpp
	ifstream in;
	ofstream out;
	string line;
	Dictionary d;
	int count = 0;


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

	while(getline(in, line)){
		count++;
		d.setValue(line,count);
	}

	out << d << endl;
	out <<d.pre_string() << endl;

	in.close();
	out.close();

	return(EXIT_SUCCESS);
   }
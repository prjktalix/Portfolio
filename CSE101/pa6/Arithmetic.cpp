/*****************************************************
*  Arithmetic.cpp
*  Brian Camilo
*  2023 Winter CSE101 pa6
*  Main program that prints out arithmetic operations
*****************************************************/

#include<iostream>
#include<fstream>
#include<string>
#include "BigInteger.h"
#include "List.h"

using namespace std;

int main(int argc, char *argv[]){
	
	// code from FileIO.cpp
	ifstream in;
	ofstream out;
	string stringA, stringB;

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

	in >> stringA;
	in >> stringB;

	BigInteger A = BigInteger(stringA);
	BigInteger B = BigInteger(stringB);
	BigInteger num_three = BigInteger("3");
	BigInteger num_two = BigInteger("2");
	BigInteger num_nine = BigInteger("9");
	BigInteger num_sixteen = BigInteger("16");

	out << A << "\n\n";
	out << B << "\n\n";
	out << A + B << "\n\n";
	out << A - B << "\n\n";
	out << A - A << "\n\n";
	out << num_three * A - num_two * B  << "\n\n";
	out << A * B << "\n\n";
	out << A * num_two << "\n\n";
	out << B * num_two << "\n\n";
	out << (num_nine * A * A * A * A) + num_sixteen * (B * B * B * B * B) << "\n\n";

	in.close();
	out.close();

	return(EXIT_SUCCESS);
   }